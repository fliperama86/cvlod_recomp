#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

// Round 17 (Task B): LOD_FIX_TEXT_MEASURE_GUARD, defense-in-depth.
// func_80167078/func_801670E4 (below) were found self-recursive by a
// systematic sweep of every RECOMP_FUNC in the 0x80160000-0x80180000
// vaddr range (the text/figure family already guarded piecemeal in
// funcs_57.c across rounds 14/15 -- func_80168AA0/func_8016B878); see
// docs/issue27-31-ni0e-findings.md round 17 for the sweep methodology and
// full function list. Same depth-128-cap pattern: a strict no-op skip of
// the entire function body once nesting would exceed 128 levels, which
// only ever fires on an already-corrupted/circular walk. Independent of
// LOD_ENABLE_NI0E_TRACE (must compile/run standalone).
#ifndef LOD_FIX_TEXT_MEASURE_GUARD
#define LOD_FIX_TEXT_MEASURE_GUARD 0
#endif

#if LOD_FIX_TEXT_MEASURE_GUARD
#include <stdio.h>

static int lod_text_guard_depth_80167078 = 0;
static int lod_text_guard_logged_80167078 = 0;
static int lod_text_guard_depth_801670E4 = 0;
static int lod_text_guard_logged_801670E4 = 0;
#endif  // LOD_FIX_TEXT_MEASURE_GUARD

RECOMP_FUNC void func_80164864(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80164864: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80164868: sw          $ra, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r31;
    // 0x8016486C: sw          $s7, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r23;
    // 0x80164870: sw          $s6, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r22;
    // 0x80164874: sw          $s5, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r21;
    // 0x80164878: sw          $s4, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r20;
    // 0x8016487C: sw          $s3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r19;
    // 0x80164880: sw          $s2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r18;
    // 0x80164884: sw          $s1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r17;
    // 0x80164888: sw          $s0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r16;
    // 0x8016488C: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x80164890: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80164894: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80164898: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x8016489C: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x801648A0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801648A4: lw          $s7, 0x24($a0)
    ctx->r23 = MEM_W(ctx->r4, 0X24);
    // 0x801648A8: lw          $s4, 0x34($a0)
    ctx->r20 = MEM_W(ctx->r4, 0X34);
    // 0x801648AC: jalr        $t9
    // 0x801648B0: addiu       $a1, $v0, 0x4
    ctx->r5 = ADD32(ctx->r2, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801648B0: addiu       $a1, $v0, 0x4
    ctx->r5 = ADD32(ctx->r2, 0X4);
    after_0:
    // 0x801648B4: beq         $v0, $zero, L_801648D4
    if (ctx->r2 == 0) {
        // 0x801648B8: lui         $s5, 0x801D
        ctx->r21 = S32(0X801D << 16);
            goto L_801648D4;
    }
    // 0x801648B8: lui         $s5, 0x801D
    ctx->r21 = S32(0X801D << 16);
    // 0x801648BC: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x801648C0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801648C4: jalr        $t9
    // 0x801648C8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801648C8: nop

    after_1:
    // 0x801648CC: b           L_80164AAC
    // 0x801648D0: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
        goto L_80164AAC;
    // 0x801648D0: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
L_801648D4:
    // 0x801648D4: addiu       $s5, $s5, -0x7D40
    ctx->r21 = ADD32(ctx->r21, -0X7D40);
    // 0x801648D8: addiu       $s3, $s0, 0x34
    ctx->r19 = ADD32(ctx->r16, 0X34);
    // 0x801648DC: lhu         $v1, 0xC($s3)
    ctx->r3 = MEM_HU(ctx->r19, 0XC);
    // 0x801648E0: lw          $v0, 0x2B34($s5)
    ctx->r2 = MEM_W(ctx->r21, 0X2B34);
    // 0x801648E4: addiu       $s6, $zero, 0x2
    ctx->r22 = ADD32(0, 0X2);
    // 0x801648E8: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801648EC: beq         $v0, $v1, L_8016492C
    if (ctx->r2 == ctx->r3) {
        // 0x801648F0: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_8016492C;
    }
    // 0x801648F0: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x801648F4: bne         $s6, $v1, L_8016490C
    if (ctx->r22 != ctx->r3) {
        // 0x801648F8: nop
    
            goto L_8016490C;
    }
    // 0x801648F8: nop

    // 0x801648FC: lhu         $t6, 0x8($s3)
    ctx->r14 = MEM_HU(ctx->r19, 0X8);
    // 0x80164900: ori         $t7, $t6, 0x2000
    ctx->r15 = ctx->r14 | 0X2000;
    // 0x80164904: b           L_8016492C
    // 0x80164908: sh          $t7, 0x8($s3)
    MEM_H(0X8, ctx->r19) = ctx->r15;
        goto L_8016492C;
    // 0x80164908: sh          $t7, 0x8($s3)
    MEM_H(0X8, ctx->r19) = ctx->r15;
L_8016490C:
    // 0x8016490C: bnel        $s6, $v0, L_80164930
    if (ctx->r22 != ctx->r2) {
        // 0x80164910: lhu         $v1, 0x8($s3)
        ctx->r3 = MEM_HU(ctx->r19, 0X8);
            goto L_80164930;
    }
    goto skip_0;
    // 0x80164910: lhu         $v1, 0x8($s3)
    ctx->r3 = MEM_HU(ctx->r19, 0X8);
    skip_0:
    // 0x80164914: lh          $t8, 0x0($s7)
    ctx->r24 = MEM_H(ctx->r23, 0X0);
    // 0x80164918: andi        $t0, $t8, 0x7FFF
    ctx->r8 = ctx->r24 & 0X7FFF;
    // 0x8016491C: sh          $t0, 0x0($s7)
    MEM_H(0X0, ctx->r23) = ctx->r8;
    // 0x80164920: lhu         $t1, 0x8($s3)
    ctx->r9 = MEM_HU(ctx->r19, 0X8);
    // 0x80164924: ori         $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 | 0X1000;
    // 0x80164928: sh          $t2, 0x8($s3)
    MEM_H(0X8, ctx->r19) = ctx->r10;
L_8016492C:
    // 0x8016492C: lhu         $v1, 0x8($s3)
    ctx->r3 = MEM_HU(ctx->r19, 0X8);
L_80164930:
    // 0x80164930: addiu       $s6, $zero, 0x2
    ctx->r22 = ADD32(0, 0X2);
    // 0x80164934: or          $s0, $s4, $zero
    ctx->r16 = ctx->r20 | 0;
    // 0x80164938: andi        $t3, $v1, 0x1000
    ctx->r11 = ctx->r3 & 0X1000;
    // 0x8016493C: beql        $t3, $zero, L_8016497C
    if (ctx->r11 == 0) {
        // 0x80164940: andi        $t7, $v1, 0x2000
        ctx->r15 = ctx->r3 & 0X2000;
            goto L_8016497C;
    }
    goto skip_1;
    // 0x80164940: andi        $t7, $v1, 0x2000
    ctx->r15 = ctx->r3 & 0X2000;
    skip_1:
    // 0x80164944: lh          $v0, 0x2E($s4)
    ctx->r2 = MEM_H(ctx->r20, 0X2E);
    // 0x80164948: addiu       $t4, $zero, 0x80
    ctx->r12 = ADD32(0, 0X80);
    // 0x8016494C: slti        $at, $v0, 0x80
    ctx->r1 = SIGNED(ctx->r2) < 0X80 ? 1 : 0;
    // 0x80164950: bne         $at, $zero, L_80164970
    if (ctx->r1 != 0) {
        // 0x80164954: addiu       $t6, $v0, 0x1
        ctx->r14 = ADD32(ctx->r2, 0X1);
            goto L_80164970;
    }
    // 0x80164954: addiu       $t6, $v0, 0x1
    ctx->r14 = ADD32(ctx->r2, 0X1);
    // 0x80164958: sh          $t4, 0x2E($s4)
    MEM_H(0X2E, ctx->r20) = ctx->r12;
    // 0x8016495C: lhu         $t5, 0x8($s3)
    ctx->r13 = MEM_HU(ctx->r19, 0X8);
    // 0x80164960: andi        $t9, $t5, 0xEFFF
    ctx->r25 = ctx->r13 & 0XEFFF;
    // 0x80164964: sh          $t9, 0x8($s3)
    MEM_H(0X8, ctx->r19) = ctx->r25;
    // 0x80164968: b           L_80164978
    // 0x8016496C: andi        $v1, $t9, 0xFFFF
    ctx->r3 = ctx->r25 & 0XFFFF;
        goto L_80164978;
    // 0x8016496C: andi        $v1, $t9, 0xFFFF
    ctx->r3 = ctx->r25 & 0XFFFF;
L_80164970:
    // 0x80164970: sh          $t6, 0x2E($s4)
    MEM_H(0X2E, ctx->r20) = ctx->r14;
    // 0x80164974: lhu         $v1, 0x8($s3)
    ctx->r3 = MEM_HU(ctx->r19, 0X8);
L_80164978:
    // 0x80164978: andi        $t7, $v1, 0x2000
    ctx->r15 = ctx->r3 & 0X2000;
L_8016497C:
    // 0x8016497C: beql        $t7, $zero, L_801649C0
    if (ctx->r15 == 0) {
        // 0x80164980: lw          $t4, 0x2B34($s5)
        ctx->r12 = MEM_W(ctx->r21, 0X2B34);
            goto L_801649C0;
    }
    goto skip_2;
    // 0x80164980: lw          $t4, 0x2B34($s5)
    ctx->r12 = MEM_W(ctx->r21, 0X2B34);
    skip_2:
    // 0x80164984: lh          $v0, 0x2E($s4)
    ctx->r2 = MEM_H(ctx->r20, 0X2E);
    // 0x80164988: slti        $at, $v0, 0x2
    ctx->r1 = SIGNED(ctx->r2) < 0X2 ? 1 : 0;
    // 0x8016498C: beq         $at, $zero, L_801649B8
    if (ctx->r1 == 0) {
        // 0x80164990: addiu       $t3, $v0, -0x1
        ctx->r11 = ADD32(ctx->r2, -0X1);
            goto L_801649B8;
    }
    // 0x80164990: addiu       $t3, $v0, -0x1
    ctx->r11 = ADD32(ctx->r2, -0X1);
    // 0x80164994: sh          $zero, 0x2E($s4)
    MEM_H(0X2E, ctx->r20) = 0;
    // 0x80164998: lh          $t8, 0x0($s7)
    ctx->r24 = MEM_H(ctx->r23, 0X0);
    // 0x8016499C: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x801649A0: or          $t0, $t8, $at
    ctx->r8 = ctx->r24 | ctx->r1;
    // 0x801649A4: sh          $t0, 0x0($s7)
    MEM_H(0X0, ctx->r23) = ctx->r8;
    // 0x801649A8: lhu         $t1, 0x8($s3)
    ctx->r9 = MEM_HU(ctx->r19, 0X8);
    // 0x801649AC: andi        $t2, $t1, 0xDFFF
    ctx->r10 = ctx->r9 & 0XDFFF;
    // 0x801649B0: b           L_801649BC
    // 0x801649B4: sh          $t2, 0x8($s3)
    MEM_H(0X8, ctx->r19) = ctx->r10;
        goto L_801649BC;
    // 0x801649B4: sh          $t2, 0x8($s3)
    MEM_H(0X8, ctx->r19) = ctx->r10;
L_801649B8:
    // 0x801649B8: sh          $t3, 0x2E($s4)
    MEM_H(0X2E, ctx->r20) = ctx->r11;
L_801649BC:
    // 0x801649BC: lw          $t4, 0x2B34($s5)
    ctx->r12 = MEM_W(ctx->r21, 0X2B34);
L_801649C0:
    // 0x801649C0: sh          $t4, 0xC($s3)
    MEM_H(0XC, ctx->r19) = ctx->r12;
    // 0x801649C4: lh          $t9, 0x0($s7)
    ctx->r25 = MEM_H(ctx->r23, 0X0);
    // 0x801649C8: bltzl       $t9, L_80164AAC
    if (SIGNED(ctx->r25) < 0) {
        // 0x801649CC: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_80164AAC;
    }
    goto skip_3;
    // 0x801649CC: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_3:
L_801649D0:
    // 0x801649D0: lbu         $t6, 0x12($s4)
    ctx->r14 = MEM_BU(ctx->r20, 0X12);
    // 0x801649D4: lbu         $t0, 0x13($s4)
    ctx->r8 = MEM_BU(ctx->r20, 0X13);
    // 0x801649D8: lhu         $t4, 0x52C($s5)
    ctx->r12 = MEM_HU(ctx->r21, 0X52C);
    // 0x801649DC: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x801649E0: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x801649E4: sllv        $t8, $t7, $t6
    ctx->r24 = S32(ctx->r15 << (ctx->r14 & 31));
    // 0x801649E8: sllv        $t2, $t1, $t0
    ctx->r10 = S32(ctx->r9 << (ctx->r8 & 31));
    // 0x801649EC: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x801649F0: mtc1        $t2, $f6
    ctx->f6.u32l = ctx->r10;
    // 0x801649F4: lw          $t3, 0x4($s3)
    ctx->r11 = MEM_W(ctx->r19, 0X4);
    // 0x801649F8: mtc1        $t4, $f8
    ctx->f8.u32l = ctx->r12;
    // 0x801649FC: cvt.s.w     $f14, $f4
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    ctx->f14.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80164A00: addu        $v0, $t3, $s2
    ctx->r2 = ADD32(ctx->r11, ctx->r18);
    // 0x80164A04: cvt.s.w     $f20, $f6
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    ctx->f20.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80164A08: bgez        $t4, L_80164A20
    if (SIGNED(ctx->r12) >= 0) {
        // 0x80164A0C: cvt.s.w     $f0, $f8
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
            goto L_80164A20;
    }
    // 0x80164A0C: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80164A10: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x80164A14: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80164A18: nop

    // 0x80164A1C: add.s       $f0, $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f10.fl;
L_80164A20:
    // 0x80164A20: lwc1        $f16, 0x10($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X10);
    // 0x80164A24: lwc1        $f18, 0x18($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X18);
    // 0x80164A28: mul.s       $f12, $f16, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x80164A2C: nop

    // 0x80164A30: mul.s       $f22, $f18, $f0
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f22.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80164A34: jal         0x80147AB0
    // 0x80164A38: nop

    func_80147AB0(rdram, ctx);
        goto after_2;
    // 0x80164A38: nop

    after_2:
    // 0x80164A3C: swc1        $f0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f0.u32l;
    // 0x80164A40: mov.s       $f12, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    ctx->f12.fl = ctx->f22.fl;
    // 0x80164A44: jal         0x80147AB0
    // 0x80164A48: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    func_80147AB0(rdram, ctx);
        goto after_3;
    // 0x80164A48: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_3:
    // 0x80164A4C: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80164A50: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x80164A54: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80164A58: bne         $s1, $s6, L_801649D0
    if (ctx->r17 != ctx->r22) {
        // 0x80164A5C: swc1        $f0, 0x1C($s0)
        MEM_W(0X1C, ctx->r16) = ctx->f0.u32l;
            goto L_801649D0;
    }
    // 0x80164A5C: swc1        $f0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f0.u32l;
    // 0x80164A60: lhu         $t5, 0x52C($s5)
    ctx->r13 = MEM_HU(ctx->r21, 0X52C);
    // 0x80164A64: sll         $t9, $t5, 4
    ctx->r25 = S32(ctx->r13 << 4);
    // 0x80164A68: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x80164A6C: sll         $t9, $t9, 4
    ctx->r25 = S32(ctx->r25 << 4);
    // 0x80164A70: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x80164A74: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x80164A78: sh          $t9, 0xA($s3)
    MEM_H(0XA, ctx->r19) = ctx->r25;
    // 0x80164A7C: jal         0x80097330
    // 0x80164A80: andi        $a0, $t9, 0xFFFF
    ctx->r4 = ctx->r25 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_4;
    // 0x80164A80: andi        $a0, $t9, 0xFFFF
    ctx->r4 = ctx->r25 & 0XFFFF;
    after_4:
    // 0x80164A84: bgez        $v0, L_80164A94
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80164A88: sra         $t7, $v0, 10
        ctx->r15 = S32(SIGNED(ctx->r2) >> 10);
            goto L_80164A94;
    }
    // 0x80164A88: sra         $t7, $v0, 10
    ctx->r15 = S32(SIGNED(ctx->r2) >> 10);
    // 0x80164A8C: addiu       $at, $v0, 0x3FF
    ctx->r1 = ADD32(ctx->r2, 0X3FF);
    // 0x80164A90: sra         $t7, $at, 10
    ctx->r15 = S32(SIGNED(ctx->r1) >> 10);
L_80164A94:
    // 0x80164A94: addiu       $t6, $t7, 0x80
    ctx->r14 = ADD32(ctx->r15, 0X80);
    // 0x80164A98: sh          $t6, 0x2C($s4)
    MEM_H(0X2C, ctx->r20) = ctx->r14;
    // 0x80164A9C: jal         0x80146AC0
    // 0x80164AA0: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    func_80146AC0(rdram, ctx);
        goto after_5;
    // 0x80164AA0: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    after_5:
    // 0x80164AA4: sw          $v0, 0x38($s7)
    MEM_W(0X38, ctx->r23) = ctx->r2;
    // 0x80164AA8: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
L_80164AAC:
    // 0x80164AAC: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80164AB0: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x80164AB4: lw          $s0, 0x24($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X24);
    // 0x80164AB8: lw          $s1, 0x28($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X28);
    // 0x80164ABC: lw          $s2, 0x2C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X2C);
    // 0x80164AC0: lw          $s3, 0x30($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X30);
    // 0x80164AC4: lw          $s4, 0x34($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X34);
    // 0x80164AC8: lw          $s5, 0x38($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X38);
    // 0x80164ACC: lw          $s6, 0x3C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X3C);
    // 0x80164AD0: lw          $s7, 0x40($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X40);
    // 0x80164AD4: jr          $ra
    // 0x80164AD8: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x80164AD8: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_80164AE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80164AE0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80164AE4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80164AE8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80164AEC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80164AF0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80164AF4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80164AF8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80164AFC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80164B00: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80164B04: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80164B08: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80164B0C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80164B10: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80164B14: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80164B18: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80164B1C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80164B20: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80164B24: lw          $t9, 0x1DE0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1DE0);
    // 0x80164B28: jalr        $t9
    // 0x80164B2C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80164B2C: nop

    after_0:
    // 0x80164B30: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80164B34: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80164B38: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80164B3C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80164B40: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80164B44: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80164B48: jr          $ra
    // 0x80164B4C: nop

    return;
    // 0x80164B4C: nop

;}
RECOMP_FUNC void func_80164B50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80164B50: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80164B54: lhu         $t6, -0x5470($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X5470);
    // 0x80164B58: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80164B5C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80164B60: sltiu       $at, $t6, 0x1C
    ctx->r1 = ctx->r14 < 0X1C ? 1 : 0;
    // 0x80164B64: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80164B68: beq         $at, $zero, L_80164CC8
    if (ctx->r1 == 0) {
        // 0x80164B6C: lw          $v1, 0x70($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X70);
            goto L_80164CC8;
    }
    // 0x80164B6C: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x80164B70: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80164B74: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80164B78: addu        $at, $at, $t6
    gpr jr_addend_80164B80 = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x80164B7C: lw          $t6, -0x5300($at)
    ctx->r14 = ADD32(ctx->r1, -0X5300);
    // 0x80164B80: jr          $t6
    // 0x80164B84: nop

    switch (jr_addend_80164B80 >> 2) {
        case 0: goto L_80164B88; break;
        case 1: goto L_80164CC8; break;
        case 2: goto L_80164CC8; break;
        case 3: goto L_80164BB0; break;
        case 4: goto L_80164BD8; break;
        case 5: goto L_80164CC8; break;
        case 6: goto L_80164C00; break;
        case 7: goto L_80164C28; break;
        case 8: goto L_80164CC8; break;
        case 9: goto L_80164CC8; break;
        case 10: goto L_80164CC8; break;
        case 11: goto L_80164C50; break;
        case 12: goto L_80164CC8; break;
        case 13: goto L_80164CC8; break;
        case 14: goto L_80164CC8; break;
        case 15: goto L_80164CC8; break;
        case 16: goto L_80164CC8; break;
        case 17: goto L_80164C78; break;
        case 18: goto L_80164CC8; break;
        case 19: goto L_80164CC8; break;
        case 20: goto L_80164CC8; break;
        case 21: goto L_80164CC8; break;
        case 22: goto L_80164CC8; break;
        case 23: goto L_80164CC8; break;
        case 24: goto L_80164CC8; break;
        case 25: goto L_80164CC8; break;
        case 26: goto L_80164CC8; break;
        case 27: goto L_80164CA0; break;
        default: switch_error(__func__, 0x80164B80, 0x8019AD00);
    }
    // 0x80164B84: nop

L_80164B88:
    // 0x80164B88: lhu         $t7, 0x18($v1)
    ctx->r15 = MEM_HU(ctx->r3, 0X18);
    // 0x80164B8C: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x80164B90: addiu       $t9, $t9, -0x2204
    ctx->r25 = ADD32(ctx->r25, -0X2204);
    // 0x80164B94: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80164B98: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80164B9C: sll         $t8, $t8, 3
    ctx->r24 = S32(ctx->r24 << 3);
    // 0x80164BA0: addu        $a0, $t8, $t9
    ctx->r4 = ADD32(ctx->r24, ctx->r25);
    // 0x80164BA4: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164BA8: b           L_80164CD0
    // 0x80164BAC: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164BAC: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164BB0:
    // 0x80164BB0: lhu         $t0, 0x18($v1)
    ctx->r8 = MEM_HU(ctx->r3, 0X18);
    // 0x80164BB4: lui         $t2, 0x802E
    ctx->r10 = S32(0X802E << 16);
    // 0x80164BB8: addiu       $t2, $t2, 0x5B7C
    ctx->r10 = ADD32(ctx->r10, 0X5B7C);
    // 0x80164BBC: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x80164BC0: addu        $t1, $t1, $t0
    ctx->r9 = ADD32(ctx->r9, ctx->r8);
    // 0x80164BC4: sll         $t1, $t1, 3
    ctx->r9 = S32(ctx->r9 << 3);
    // 0x80164BC8: addu        $a0, $t1, $t2
    ctx->r4 = ADD32(ctx->r9, ctx->r10);
    // 0x80164BCC: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164BD0: b           L_80164CD0
    // 0x80164BD4: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164BD4: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164BD8:
    // 0x80164BD8: lhu         $t3, 0x18($v1)
    ctx->r11 = MEM_HU(ctx->r3, 0X18);
    // 0x80164BDC: lui         $t5, 0x802E
    ctx->r13 = S32(0X802E << 16);
    // 0x80164BE0: addiu       $t5, $t5, 0x621C
    ctx->r13 = ADD32(ctx->r13, 0X621C);
    // 0x80164BE4: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80164BE8: addu        $t4, $t4, $t3
    ctx->r12 = ADD32(ctx->r12, ctx->r11);
    // 0x80164BEC: sll         $t4, $t4, 3
    ctx->r12 = S32(ctx->r12 << 3);
    // 0x80164BF0: addu        $a0, $t4, $t5
    ctx->r4 = ADD32(ctx->r12, ctx->r13);
    // 0x80164BF4: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164BF8: b           L_80164CD0
    // 0x80164BFC: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164BFC: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164C00:
    // 0x80164C00: lhu         $t6, 0x18($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X18);
    // 0x80164C04: lui         $t8, 0x802F
    ctx->r24 = S32(0X802F << 16);
    // 0x80164C08: addiu       $t8, $t8, -0x76A8
    ctx->r24 = ADD32(ctx->r24, -0X76A8);
    // 0x80164C0C: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80164C10: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80164C14: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x80164C18: addu        $a0, $t7, $t8
    ctx->r4 = ADD32(ctx->r15, ctx->r24);
    // 0x80164C1C: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164C20: b           L_80164CD0
    // 0x80164C24: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164C24: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164C28:
    // 0x80164C28: lhu         $t9, 0x18($v1)
    ctx->r25 = MEM_HU(ctx->r3, 0X18);
    // 0x80164C2C: lui         $t1, 0x802F
    ctx->r9 = S32(0X802F << 16);
    // 0x80164C30: addiu       $t1, $t1, -0x7800
    ctx->r9 = ADD32(ctx->r9, -0X7800);
    // 0x80164C34: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80164C38: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80164C3C: sll         $t0, $t0, 3
    ctx->r8 = S32(ctx->r8 << 3);
    // 0x80164C40: addu        $a0, $t0, $t1
    ctx->r4 = ADD32(ctx->r8, ctx->r9);
    // 0x80164C44: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164C48: b           L_80164CD0
    // 0x80164C4C: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164C4C: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164C50:
    // 0x80164C50: lhu         $t2, 0x18($v1)
    ctx->r10 = MEM_HU(ctx->r3, 0X18);
    // 0x80164C54: lui         $t4, 0x802E
    ctx->r12 = S32(0X802E << 16);
    // 0x80164C58: addiu       $t4, $t4, 0x60C0
    ctx->r12 = ADD32(ctx->r12, 0X60C0);
    // 0x80164C5C: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80164C60: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x80164C64: sll         $t3, $t3, 3
    ctx->r11 = S32(ctx->r11 << 3);
    // 0x80164C68: addu        $a0, $t3, $t4
    ctx->r4 = ADD32(ctx->r11, ctx->r12);
    // 0x80164C6C: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164C70: b           L_80164CD0
    // 0x80164C74: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164C74: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164C78:
    // 0x80164C78: lhu         $t5, 0x18($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X18);
    // 0x80164C7C: lui         $t7, 0x802E
    ctx->r15 = S32(0X802E << 16);
    // 0x80164C80: addiu       $t7, $t7, 0x6EEC
    ctx->r15 = ADD32(ctx->r15, 0X6EEC);
    // 0x80164C84: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x80164C88: addu        $t6, $t6, $t5
    ctx->r14 = ADD32(ctx->r14, ctx->r13);
    // 0x80164C8C: sll         $t6, $t6, 3
    ctx->r14 = S32(ctx->r14 << 3);
    // 0x80164C90: addu        $a0, $t6, $t7
    ctx->r4 = ADD32(ctx->r14, ctx->r15);
    // 0x80164C94: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164C98: b           L_80164CD0
    // 0x80164C9C: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164C9C: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164CA0:
    // 0x80164CA0: lhu         $t8, 0x18($v1)
    ctx->r24 = MEM_HU(ctx->r3, 0X18);
    // 0x80164CA4: lui         $t0, 0x802E
    ctx->r8 = S32(0X802E << 16);
    // 0x80164CA8: addiu       $t0, $t0, 0x43F4
    ctx->r8 = ADD32(ctx->r8, 0X43F4);
    // 0x80164CAC: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80164CB0: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x80164CB4: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x80164CB8: addu        $a0, $t9, $t0
    ctx->r4 = ADD32(ctx->r25, ctx->r8);
    // 0x80164CBC: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164CC0: b           L_80164CD0
    // 0x80164CC4: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
        goto L_80164CD0;
    // 0x80164CC4: sw          $a0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r4;
L_80164CC8:
    // 0x80164CC8: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80164CCC: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_80164CD0:
    // 0x80164CD0: lhu         $t1, 0x2($a0)
    ctx->r9 = MEM_HU(ctx->r4, 0X2);
    // 0x80164CD4: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80164CD8: sh          $t1, 0x8($v0)
    MEM_H(0X8, ctx->r2) = ctx->r9;
    // 0x80164CDC: lw          $t2, 0x70($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X70);
    // 0x80164CE0: beq         $t2, $zero, L_80164CF4
    if (ctx->r10 == 0) {
        // 0x80164CE4: nop
    
            goto L_80164CF4;
    }
    // 0x80164CE4: nop

    // 0x80164CE8: jal         0x80143234
    // 0x80164CEC: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_80143234(rdram, ctx);
        goto after_0;
    // 0x80164CEC: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_0:
    // 0x80164CF0: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
L_80164CF4:
    // 0x80164CF4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80164CF8: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80164CFC: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80164D00: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x80164D04: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x80164D08: jalr        $t9
    // 0x80164D0C: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80164D0C: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_1:
    // 0x80164D10: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x80164D14: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80164D18: lh          $t3, 0xE($a0)
    ctx->r11 = MEM_H(ctx->r4, 0XE);
    // 0x80164D1C: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x80164D20: addu        $v0, $a0, $t4
    ctx->r2 = ADD32(ctx->r4, ctx->r12);
    // 0x80164D24: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x80164D28: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x80164D2C: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80164D30: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80164D34: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x80164D38: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x80164D3C: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x80164D40: lw          $t9, 0x1DE0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1DE0);
    // 0x80164D44: jalr        $t9
    // 0x80164D48: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80164D48: nop

    after_2:
    // 0x80164D4C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80164D50: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80164D54: jr          $ra
    // 0x80164D58: nop

    return;
    // 0x80164D58: nop

;}
RECOMP_FUNC void func_80164D5C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80164D5C: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80164D60: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x80164D64: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80164D68: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x80164D6C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80164D70: bne         $t6, $at, L_80164D8C
    if (ctx->r14 != ctx->r1) {
        // 0x80164D74: or          $a3, $a0, $zero
        ctx->r7 = ctx->r4 | 0;
            goto L_80164D8C;
    }
    // 0x80164D74: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80164D78: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80164D7C: jal         0x80147718
    // 0x80164D80: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    requestBit_test(rdram, ctx);
        goto after_0;
    // 0x80164D80: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_0:
    // 0x80164D84: beq         $v0, $zero, L_80164DE4
    if (ctx->r2 == 0) {
        // 0x80164D88: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_80164DE4;
    }
    // 0x80164D88: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
L_80164D8C:
    // 0x80164D8C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80164D90: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80164D94: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80164D98: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x80164D9C: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    // 0x80164DA0: jalr        $t9
    // 0x80164DA4: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80164DA4: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_1:
    // 0x80164DA8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80164DAC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80164DB0: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x80164DB4: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x80164DB8: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x80164DBC: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80164DC0: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80164DC4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80164DC8: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80164DCC: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80164DD0: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80164DD4: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80164DD8: lw          $t9, 0x1DE0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1DE0);
    // 0x80164DDC: jalr        $t9
    // 0x80164DE0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80164DE0: nop

    after_2:
L_80164DE4:
    // 0x80164DE4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80164DE8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80164DEC: jr          $ra
    // 0x80164DF0: nop

    return;
    // 0x80164DF0: nop

;}
RECOMP_FUNC void func_80164DF4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80164DF4: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80164DF8: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80164DFC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80164E00: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80164E04: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80164E08: lhu         $v0, 0x3C($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X3C);
    // 0x80164E0C: lw          $s1, 0x38($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X38);
    // 0x80164E10: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x80164E14: andi        $v0, $v0, 0xF
    ctx->r2 = ctx->r2 & 0XF;
    // 0x80164E18: beq         $v0, $zero, L_80164E34
    if (ctx->r2 == 0) {
        // 0x80164E1C: addiu       $a1, $s1, 0x4
        ctx->r5 = ADD32(ctx->r17, 0X4);
            goto L_80164E34;
    }
    // 0x80164E1C: addiu       $a1, $s1, 0x4
    ctx->r5 = ADD32(ctx->r17, 0X4);
    // 0x80164E20: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80164E24: beq         $v0, $at, L_80164EAC
    if (ctx->r2 == ctx->r1) {
        // 0x80164E28: addiu       $a1, $s1, 0x4
        ctx->r5 = ADD32(ctx->r17, 0X4);
            goto L_80164EAC;
    }
    // 0x80164E28: addiu       $a1, $s1, 0x4
    ctx->r5 = ADD32(ctx->r17, 0X4);
    // 0x80164E2C: b           L_80164ED4
    // 0x80164E30: addiu       $s0, $a0, 0x34
    ctx->r16 = ADD32(ctx->r4, 0X34);
        goto L_80164ED4;
    // 0x80164E30: addiu       $s0, $a0, 0x34
    ctx->r16 = ADD32(ctx->r4, 0X34);
L_80164E34:
    // 0x80164E34: lhu         $a0, 0x0($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X0);
    // 0x80164E38: jal         0x80016E48
    // 0x80164E3C: addiu       $a2, $s1, 0x10
    ctx->r6 = ADD32(ctx->r17, 0X10);
    func_80016E48(rdram, ctx);
        goto after_0;
    // 0x80164E3C: addiu       $a2, $s1, 0x10
    ctx->r6 = ADD32(ctx->r17, 0X10);
    after_0:
    // 0x80164E40: addiu       $s0, $s2, 0x34
    ctx->r16 = ADD32(ctx->r18, 0X34);
    // 0x80164E44: sw          $v0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r2;
    // 0x80164E48: lwc1        $f6, 0x4($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80164E4C: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x80164E50: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x80164E54: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    // 0x80164E58: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80164E5C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80164E60: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80164E64: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x80164E68: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x80164E6C: swc1        $f18, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f18.u32l;
    // 0x80164E70: lwc1        $f6, 0x8($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X8);
    // 0x80164E74: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x80164E78: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80164E7C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80164E80: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x80164E84: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x80164E88: swc1        $f18, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f18.u32l;
    // 0x80164E8C: lwc1        $f6, 0xC($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0XC);
    // 0x80164E90: lwc1        $f4, 0x18($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X18);
    // 0x80164E94: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80164E98: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80164E9C: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x80164EA0: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x80164EA4: b           L_80164ED4
    // 0x80164EA8: swc1        $f18, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f18.u32l;
        goto L_80164ED4;
    // 0x80164EA8: swc1        $f18, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f18.u32l;
L_80164EAC:
    // 0x80164EAC: jal         0x80016DD8
    // 0x80164EB0: lhu         $a0, 0x0($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X0);
    func_80016DD8(rdram, ctx);
        goto after_1;
    // 0x80164EB0: lhu         $a0, 0x0($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X0);
    after_1:
    // 0x80164EB4: addiu       $s0, $s2, 0x34
    ctx->r16 = ADD32(ctx->r18, 0X34);
    // 0x80164EB8: sw          $v0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r2;
    // 0x80164EBC: lwc1        $f4, 0x4($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80164EC0: swc1        $f4, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f4.u32l;
    // 0x80164EC4: lwc1        $f6, 0x8($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X8);
    // 0x80164EC8: swc1        $f6, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f6.u32l;
    // 0x80164ECC: lwc1        $f8, 0xC($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0XC);
    // 0x80164ED0: swc1        $f8, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f8.u32l;
L_80164ED4:
    // 0x80164ED4: jal         0x8001717C
    // 0x80164ED8: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
    func_8001717C(rdram, ctx);
        goto after_2;
    // 0x80164ED8: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
    after_2:
    // 0x80164EDC: bnel        $v0, $zero, L_80164F00
    if (ctx->r2 != 0) {
        // 0x80164EE0: lw          $a0, 0xC($s0)
        ctx->r4 = MEM_W(ctx->r16, 0XC);
            goto L_80164F00;
    }
    goto skip_0;
    // 0x80164EE0: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
    skip_0:
    // 0x80164EE4: lw          $t9, 0x10($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X10);
    // 0x80164EE8: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80164EEC: jalr        $t9
    // 0x80164EF0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80164EF0: nop

    after_3:
    // 0x80164EF4: b           L_80164F88
    // 0x80164EF8: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_80164F88;
    // 0x80164EF8: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80164EFC: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
L_80164F00:
    // 0x80164F00: lw          $a1, 0x1C($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X1C);
    // 0x80164F04: jal         0x80016FB8
    // 0x80164F08: lw          $a2, 0x20($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X20);
    func_80016FB8(rdram, ctx);
        goto after_4;
    // 0x80164F08: lw          $a2, 0x20($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X20);
    after_4:
    // 0x80164F0C: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
    // 0x80164F10: lw          $a1, 0x24($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X24);
    // 0x80164F14: jal         0x8001700C
    // 0x80164F18: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    func_8001700C(rdram, ctx);
        goto after_5;
    // 0x80164F18: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    after_5:
    // 0x80164F1C: lw          $t6, 0x10($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X10);
    // 0x80164F20: lui         $t7, 0x8016
    ctx->r15 = S32(0X8016 << 16);
    // 0x80164F24: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80164F28: addiu       $t7, $t7, 0x5084
    ctx->r15 = ADD32(ctx->r15, 0X5084);
    // 0x80164F2C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80164F30: sw          $t6, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r14;
    // 0x80164F34: sw          $t7, 0x10($s2)
    MEM_W(0X10, ctx->r18) = ctx->r15;
    // 0x80164F38: addiu       $a0, $s2, 0x8
    ctx->r4 = ADD32(ctx->r18, 0X8);
    // 0x80164F3C: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    // 0x80164F40: jalr        $t9
    // 0x80164F44: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x80164F44: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    after_6:
    // 0x80164F48: lh          $t8, 0xE($s2)
    ctx->r24 = MEM_H(ctx->r18, 0XE);
    // 0x80164F4C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80164F50: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80164F54: sll         $t0, $t8, 1
    ctx->r8 = S32(ctx->r24 << 1);
    // 0x80164F58: addu        $v0, $s2, $t0
    ctx->r2 = ADD32(ctx->r18, ctx->r8);
    // 0x80164F5C: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x80164F60: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x80164F64: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80164F68: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80164F6C: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80164F70: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x80164F74: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
    // 0x80164F78: lw          $t9, 0x1DE0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1DE0);
    // 0x80164F7C: jalr        $t9
    // 0x80164F80: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80164F80: nop

    after_7:
    // 0x80164F84: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_80164F88:
    // 0x80164F88: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80164F8C: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80164F90: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80164F94: jr          $ra
    // 0x80164F98: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80164F98: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80164F9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80164F9C: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80164FA0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80164FA4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80164FA8: lhu         $t6, 0x3C($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X3C);
    // 0x80164FAC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80164FB0: lw          $a2, 0x38($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X38);
    // 0x80164FB4: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x80164FB8: bne         $t7, $zero, L_80165070
    if (ctx->r15 != 0) {
        // 0x80164FBC: addiu       $v1, $a0, 0x34
        ctx->r3 = ADD32(ctx->r4, 0X34);
            goto L_80165070;
    }
    // 0x80164FBC: addiu       $v1, $a0, 0x34
    ctx->r3 = ADD32(ctx->r4, 0X34);
    // 0x80164FC0: lhu         $v0, 0x8($v1)
    ctx->r2 = MEM_HU(ctx->r3, 0X8);
    // 0x80164FC4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80164FC8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80164FCC: andi        $v0, $v0, 0xF
    ctx->r2 = ctx->r2 & 0XF;
    // 0x80164FD0: beq         $v0, $zero, L_80164FE8
    if (ctx->r2 == 0) {
        // 0x80164FD4: addiu       $a1, $a2, 0x4
        ctx->r5 = ADD32(ctx->r6, 0X4);
            goto L_80164FE8;
    }
    // 0x80164FD4: addiu       $a1, $a2, 0x4
    ctx->r5 = ADD32(ctx->r6, 0X4);
    // 0x80164FD8: beq         $v0, $at, L_80165048
    if (ctx->r2 == ctx->r1) {
        // 0x80164FDC: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80165048;
    }
    // 0x80164FDC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80164FE0: b           L_80165058
    // 0x80164FE4: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
        goto L_80165058;
    // 0x80164FE4: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
L_80164FE8:
    // 0x80164FE8: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    // 0x80164FEC: jal         0x8014314C
    // 0x80164FF0: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    func_8014314C(rdram, ctx);
        goto after_0;
    // 0x80164FF0: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    after_0:
    // 0x80164FF4: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x80164FF8: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x80164FFC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165000: jal         0x8014314C
    // 0x80165004: addiu       $a1, $v1, 0x10
    ctx->r5 = ADD32(ctx->r3, 0X10);
    func_8014314C(rdram, ctx);
        goto after_1;
    // 0x80165004: addiu       $a1, $v1, 0x10
    ctx->r5 = ADD32(ctx->r3, 0X10);
    after_1:
    // 0x80165008: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    // 0x8016500C: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    // 0x80165010: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165014: jal         0x8014314C
    // 0x80165018: addiu       $a1, $a1, 0x10
    ctx->r5 = ADD32(ctx->r5, 0X10);
    func_8014314C(rdram, ctx);
        goto after_2;
    // 0x80165018: addiu       $a1, $a1, 0x10
    ctx->r5 = ADD32(ctx->r5, 0X10);
    after_2:
    // 0x8016501C: lw          $t8, 0x28($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X28);
    // 0x80165020: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
    // 0x80165024: multu       $v0, $t8
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r24)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80165028: mflo        $t9
    ctx->r25 = lo;
    // 0x8016502C: nop

    // 0x80165030: nop

    // 0x80165034: multu       $t9, $t0
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80165038: mflo        $t1
    ctx->r9 = lo;
    // 0x8016503C: sw          $t1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r9;
    // 0x80165040: b           L_80165058
    // 0x80165044: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
        goto L_80165058;
    // 0x80165044: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
L_80165048:
    // 0x80165048: jal         0x8014314C
    // 0x8016504C: addiu       $a1, $v1, 0x10
    ctx->r5 = ADD32(ctx->r3, 0X10);
    func_8014314C(rdram, ctx);
        goto after_3;
    // 0x8016504C: addiu       $a1, $v1, 0x10
    ctx->r5 = ADD32(ctx->r3, 0X10);
    after_3:
    // 0x80165050: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    // 0x80165054: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
L_80165058:
    // 0x80165058: beql        $t2, $zero, L_80165074
    if (ctx->r10 == 0) {
        // 0x8016505C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80165074;
    }
    goto skip_0;
    // 0x8016505C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80165060: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80165064: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165068: jalr        $t9
    // 0x8016506C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x8016506C: nop

    after_4:
L_80165070:
    // 0x80165070: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165074:
    // 0x80165074: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80165078: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8016507C: jr          $ra
    // 0x80165080: nop

    return;
    // 0x80165080: nop

;}
RECOMP_FUNC void func_80165084(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165084: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165088: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016508C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80165090: jal         0x8001717C
    // 0x80165094: lw          $a0, 0x40($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X40);
    func_8001717C(rdram, ctx);
        goto after_0;
    // 0x80165094: lw          $a0, 0x40($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X40);
    after_0:
    // 0x80165098: beq         $v0, $zero, L_801650A8
    if (ctx->r2 == 0) {
        // 0x8016509C: lw          $t7, 0x18($sp)
        ctx->r15 = MEM_W(ctx->r29, 0X18);
            goto L_801650A8;
    }
    // 0x8016509C: lw          $t7, 0x18($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X18);
    // 0x801650A0: jal         0x80016EBC
    // 0x801650A4: lw          $a0, 0x40($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X40);
    func_80016EBC(rdram, ctx);
        goto after_1;
    // 0x801650A4: lw          $a0, 0x40($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X40);
    after_1:
L_801650A8:
    // 0x801650A8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801650AC: lw          $t9, 0x34($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X34);
    // 0x801650B0: jalr        $t9
    // 0x801650B4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801650B4: nop

    after_2:
    // 0x801650B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801650BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801650C0: jr          $ra
    // 0x801650C4: nop

    return;
    // 0x801650C4: nop

;}
RECOMP_FUNC void func_801650D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801650D0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801650D4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801650D8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x801650DC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801650E0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801650E4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801650E8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x801650EC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x801650F0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801650F4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801650F8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801650FC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80165100: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80165104: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80165108: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8016510C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80165110: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80165114: lw          $t9, 0x1DFC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1DFC);
    // 0x80165118: jalr        $t9
    // 0x8016511C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8016511C: nop

    after_0:
    // 0x80165120: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80165124: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80165128: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8016512C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80165130: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165134: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80165138: jr          $ra
    // 0x8016513C: nop

    return;
    // 0x8016513C: nop

;}
RECOMP_FUNC void func_80165140(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165140: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80165144: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80165148: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8016514C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80165150: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80165154: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x80165158: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016515C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80165160: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x80165164: jalr        $t9
    // 0x80165168: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80165168: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_0:
    // 0x8016516C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x80165170: bne         $v0, $zero, L_80165190
    if (ctx->r2 != 0) {
        // 0x80165174: sw          $v0, 0x24($s1)
        MEM_W(0X24, ctx->r17) = ctx->r2;
            goto L_80165190;
    }
    // 0x80165174: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x80165178: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x8016517C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80165180: jalr        $t9
    // 0x80165184: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80165184: nop

    after_1:
    // 0x80165188: b           L_801652B8
    // 0x8016518C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801652B8;
    // 0x8016518C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165190:
    // 0x80165190: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x80165194: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80165198: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8016519C: ori         $t7, $t6, 0x800
    ctx->r15 = ctx->r14 | 0X800;
    // 0x801651A0: sh          $t7, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r15;
    // 0x801651A4: lw          $t8, 0x2B14($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2B14);
    // 0x801651A8: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x801651AC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801651B0: sw          $t8, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r24;
    // 0x801651B4: lh          $t0, 0x28D0($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X28D0);
    // 0x801651B8: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801651BC: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x801651C0: addu        $t2, $t2, $t1
    ctx->r10 = ADD32(ctx->r10, ctx->r9);
    // 0x801651C4: lw          $t2, 0x1DC8($t2)
    ctx->r10 = MEM_W(ctx->r10, 0X1DC8);
    // 0x801651C8: sw          $t2, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r10;
    // 0x801651CC: lw          $t3, 0x2B0C($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X2B0C);
    // 0x801651D0: sw          $t3, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r11;
    // 0x801651D4: lw          $t4, 0x7C($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X7C);
    // 0x801651D8: jal         0x801431CC
    // 0x801651DC: sw          $t4, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r12;
    sceneStateCopy(rdram, ctx);
        goto after_2;
    // 0x801651DC: sw          $t4, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r12;
    after_2:
    // 0x801651E0: lh          $t5, 0x0($s1)
    ctx->r13 = MEM_H(ctx->r17, 0X0);
    // 0x801651E4: ori         $t9, $t5, 0x1000
    ctx->r25 = ctx->r13 | 0X1000;
    // 0x801651E8: sh          $t9, 0x0($s1)
    MEM_H(0X0, ctx->r17) = ctx->r25;
    // 0x801651EC: lw          $a1, 0x40($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X40);
    // 0x801651F0: jal         0x801464B0
    // 0x801651F4: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    func_801464B0(rdram, ctx);
        goto after_3;
    // 0x801651F4: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    after_3:
    // 0x801651F8: sw          $v0, 0x68($s1)
    MEM_W(0X68, ctx->r17) = ctx->r2;
    // 0x801651FC: lw          $a1, 0x40($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X40);
    // 0x80165200: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    // 0x80165204: jal         0x80146504
    // 0x80165208: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_80146504(rdram, ctx);
        goto after_4;
    // 0x80165208: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_4:
    // 0x8016520C: sw          $v0, 0x74($s0)
    MEM_W(0X74, ctx->r16) = ctx->r2;
    // 0x80165210: jal         0x80146900
    // 0x80165214: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80146900(rdram, ctx);
        goto after_5;
    // 0x80165214: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_5:
    // 0x80165218: lw          $t6, 0x10($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X10);
    // 0x8016521C: addiu       $v0, $s1, 0x34
    ctx->r2 = ADD32(ctx->r17, 0X34);
    // 0x80165220: lui         $t7, 0x8016
    ctx->r15 = S32(0X8016 << 16);
    // 0x80165224: addiu       $t7, $t7, 0x5588
    ctx->r15 = ADD32(ctx->r15, 0X5588);
    // 0x80165228: sw          $t6, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r14;
    // 0x8016522C: sw          $t7, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r15;
    // 0x80165230: lw          $t8, 0x8($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X8);
    // 0x80165234: sw          $zero, 0xC($v0)
    MEM_W(0XC, ctx->r2) = 0;
    // 0x80165238: sw          $zero, 0x8($v0)
    MEM_W(0X8, ctx->r2) = 0;
    // 0x8016523C: sll         $t0, $t8, 8
    ctx->r8 = S32(ctx->r24 << 8);
    // 0x80165240: sw          $t0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r8;
    // 0x80165244: lw          $t2, 0x70($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X70);
    // 0x80165248: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x8016524C: sw          $t1, 0x64($s1)
    MEM_W(0X64, ctx->r17) = ctx->r9;
    // 0x80165250: beq         $t2, $zero, L_80165260
    if (ctx->r10 == 0) {
        // 0x80165254: nop
    
            goto L_80165260;
    }
    // 0x80165254: nop

    // 0x80165258: jal         0x80143234
    // 0x8016525C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80143234(rdram, ctx);
        goto after_6;
    // 0x8016525C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_6:
L_80165260:
    // 0x80165260: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165264: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80165268: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x8016526C: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    // 0x80165270: jalr        $t9
    // 0x80165274: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80165274: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_7:
    // 0x80165278: lh          $t3, 0xE($s1)
    ctx->r11 = MEM_H(ctx->r17, 0XE);
    // 0x8016527C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80165280: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80165284: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x80165288: addu        $v0, $s1, $t4
    ctx->r2 = ADD32(ctx->r17, ctx->r12);
    // 0x8016528C: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x80165290: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x80165294: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80165298: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8016529C: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x801652A0: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x801652A4: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x801652A8: lw          $t9, 0x1DFC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1DFC);
    // 0x801652AC: jalr        $t9
    // 0x801652B0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x801652B0: nop

    after_8:
    // 0x801652B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801652B8:
    // 0x801652B8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x801652BC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801652C0: jr          $ra
    // 0x801652C4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x801652C4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_801652C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801652C8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801652CC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801652D0: lw          $a2, 0x24($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X24);
    // 0x801652D4: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x801652D8: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x801652DC: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x801652E0: addiu       $a1, $a2, 0x50
    ctx->r5 = ADD32(ctx->r6, 0X50);
    // 0x801652E4: jalr        $t9
    // 0x801652E8: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801652E8: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    after_0:
    // 0x801652EC: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x801652F0: beq         $v0, $zero, L_8016530C
    if (ctx->r2 == 0) {
        // 0x801652F4: lw          $a2, 0x1C($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X1C);
            goto L_8016530C;
    }
    // 0x801652F4: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x801652F8: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x801652FC: jalr        $t9
    // 0x80165300: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80165300: nop

    after_1:
    // 0x80165304: b           L_80165380
    // 0x80165308: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80165380;
    // 0x80165308: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8016530C:
    // 0x8016530C: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x80165310: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x80165314: lw          $t6, 0x2B0C($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X2B0C);
    // 0x80165318: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x8016531C: sw          $t6, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r14;
    // 0x80165320: lw          $t7, 0x7C($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X7C);
    // 0x80165324: sw          $t7, 0x24($a2)
    MEM_W(0X24, ctx->r6) = ctx->r15;
    // 0x80165328: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8016532C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80165330: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80165334: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80165338: sll         $t8, $v1, 1
    ctx->r24 = S32(ctx->r3 << 1);
    // 0x8016533C: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x80165340: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80165344: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80165348: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x8016534C: sb          $t1, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r9;
    // 0x80165350: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80165354: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80165358: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x8016535C: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80165360: lw          $t9, 0x1E04($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1E04);
    // 0x80165364: jalr        $t9
    // 0x80165368: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80165368: nop

    after_2:
    // 0x8016536C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80165370: lh          $t4, 0xE($a0)
    ctx->r12 = MEM_H(ctx->r4, 0XE);
    // 0x80165374: addiu       $t5, $t4, -0x1
    ctx->r13 = ADD32(ctx->r12, -0X1);
    // 0x80165378: sh          $t5, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r13;
    // 0x8016537C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80165380:
    // 0x80165380: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80165384: jr          $ra
    // 0x80165388: nop

    return;
    // 0x80165388: nop

;}
RECOMP_FUNC void func_8016538C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016538C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165390: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165394: lw          $t6, 0x64($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X64);
    // 0x80165398: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8016539C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801653A0: bne         $t6, $at, L_801653D8
    if (ctx->r14 != ctx->r1) {
        // 0x801653A4: lw          $v0, 0x24($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X24);
            goto L_801653D8;
    }
    // 0x801653A4: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x801653A8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801653AC: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x801653B0: addiu       $a0, $zero, 0x18D
    ctx->r4 = ADD32(0, 0X18D);
    // 0x801653B4: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    // 0x801653B8: jalr        $t9
    // 0x801653BC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801653BC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x801653C0: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x801653C4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801653C8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801653CC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801653D0: jalr        $t9
    // 0x801653D4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801653D4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_801653D8:
    // 0x801653D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801653DC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801653E0: jr          $ra
    // 0x801653E4: nop

    return;
    // 0x801653E4: nop

;}
RECOMP_FUNC void func_801653E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801653E8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801653EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801653F0: lw          $t6, 0x40($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X40);
    // 0x801653F4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801653F8: ori         $at, $at, 0x1111
    ctx->r1 = ctx->r1 | 0X1111;
    // 0x801653FC: slt         $at, $t6, $at
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x80165400: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80165404: beq         $at, $zero, L_8016541C
    if (ctx->r1 == 0) {
        // 0x80165408: lw          $t0, 0x24($a0)
        ctx->r8 = MEM_W(ctx->r4, 0X24);
            goto L_8016541C;
    }
    // 0x80165408: lw          $t0, 0x24($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X24);
    // 0x8016540C: addiu       $v0, $a0, 0x34
    ctx->r2 = ADD32(ctx->r4, 0X34);
    // 0x80165410: lw          $t7, 0xC($v0)
    ctx->r15 = MEM_W(ctx->r2, 0XC);
    // 0x80165414: addiu       $t8, $t7, 0x1234
    ctx->r24 = ADD32(ctx->r15, 0X1234);
    // 0x80165418: sw          $t8, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r24;
L_8016541C:
    // 0x8016541C: addiu       $v0, $a2, 0x34
    ctx->r2 = ADD32(ctx->r6, 0X34);
    // 0x80165420: lw          $t9, 0x8($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X8);
    // 0x80165424: lw          $t1, 0xC($v0)
    ctx->r9 = MEM_W(ctx->r2, 0XC);
    // 0x80165428: lw          $a3, 0x10($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X10);
    // 0x8016542C: lui         $at, 0x20
    ctx->r1 = S32(0X20 << 16);
    // 0x80165430: addu        $v1, $t9, $t1
    ctx->r3 = ADD32(ctx->r25, ctx->r9);
    // 0x80165434: addu        $a3, $a3, $at
    ctx->r7 = ADD32(ctx->r7, ctx->r1);
    // 0x80165438: slt         $at, $a3, $v1
    ctx->r1 = SIGNED(ctx->r7) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x8016543C: beq         $at, $zero, L_80165474
    if (ctx->r1 == 0) {
        // 0x80165440: sw          $v1, 0x8($v0)
        MEM_W(0X8, ctx->r2) = ctx->r3;
            goto L_80165474;
    }
    // 0x80165440: sw          $v1, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r3;
    // 0x80165444: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165448: sw          $zero, 0xC($v0)
    MEM_W(0XC, ctx->r2) = 0;
    // 0x8016544C: sw          $a3, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r7;
    // 0x80165450: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80165454: sw          $t0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r8;
    // 0x80165458: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x8016545C: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80165460: jalr        $t9
    // 0x80165464: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80165464: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
    // 0x80165468: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x8016546C: lw          $t0, 0x20($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X20);
    // 0x80165470: lw          $v1, 0x8($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X8);
L_80165474:
    // 0x80165474: sra         $t3, $v1, 8
    ctx->r11 = S32(SIGNED(ctx->r3) >> 8);
    // 0x80165478: sh          $t3, 0x5C($t0)
    MEM_H(0X5C, ctx->r8) = ctx->r11;
    // 0x8016547C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165480: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80165484: jr          $ra
    // 0x80165488: nop

    return;
    // 0x80165488: nop

;}
RECOMP_FUNC void func_8016548C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016548C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165490: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165494: lw          $t6, 0x64($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X64);
    // 0x80165498: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8016549C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801654A0: bne         $t6, $at, L_801654D8
    if (ctx->r14 != ctx->r1) {
        // 0x801654A4: lw          $v0, 0x24($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X24);
            goto L_801654D8;
    }
    // 0x801654A4: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x801654A8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801654AC: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x801654B0: addiu       $a0, $zero, 0x18D
    ctx->r4 = ADD32(0, 0X18D);
    // 0x801654B4: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    // 0x801654B8: jalr        $t9
    // 0x801654BC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801654BC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x801654C0: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x801654C4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801654C8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801654CC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801654D0: jalr        $t9
    // 0x801654D4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801654D4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_801654D8:
    // 0x801654D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801654DC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801654E0: jr          $ra
    // 0x801654E4: nop

    return;
    // 0x801654E4: nop

;}
RECOMP_FUNC void func_801654E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801654E8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801654EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801654F0: lw          $t6, 0x40($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X40);
    // 0x801654F4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801654F8: ori         $at, $at, 0x1111
    ctx->r1 = ctx->r1 | 0X1111;
    // 0x801654FC: slt         $at, $t6, $at
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x80165500: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80165504: beq         $at, $zero, L_8016551C
    if (ctx->r1 == 0) {
        // 0x80165508: lw          $t1, 0x24($a0)
        ctx->r9 = MEM_W(ctx->r4, 0X24);
            goto L_8016551C;
    }
    // 0x80165508: lw          $t1, 0x24($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X24);
    // 0x8016550C: addiu       $v0, $a0, 0x34
    ctx->r2 = ADD32(ctx->r4, 0X34);
    // 0x80165510: lw          $t7, 0xC($v0)
    ctx->r15 = MEM_W(ctx->r2, 0XC);
    // 0x80165514: addiu       $t8, $t7, 0x1234
    ctx->r24 = ADD32(ctx->r15, 0X1234);
    // 0x80165518: sw          $t8, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r24;
L_8016551C:
    // 0x8016551C: addiu       $v0, $a3, 0x34
    ctx->r2 = ADD32(ctx->r7, 0X34);
    // 0x80165520: lw          $t9, 0x8($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X8);
    // 0x80165524: lw          $t2, 0xC($v0)
    ctx->r10 = MEM_W(ctx->r2, 0XC);
    // 0x80165528: lw          $t0, 0x10($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X10);
    // 0x8016552C: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80165530: subu        $v1, $t9, $t2
    ctx->r3 = SUB32(ctx->r25, ctx->r10);
    // 0x80165534: slt         $at, $v1, $t0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r8) ? 1 : 0;
    // 0x80165538: beq         $at, $zero, L_80165570
    if (ctx->r1 == 0) {
        // 0x8016553C: sw          $v1, 0x8($v0)
        MEM_W(0X8, ctx->r2) = ctx->r3;
            goto L_80165570;
    }
    // 0x8016553C: sw          $v1, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r3;
    // 0x80165540: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165544: sw          $zero, 0xC($v0)
    MEM_W(0XC, ctx->r2) = 0;
    // 0x80165548: sw          $t0, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r8;
    // 0x8016554C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80165550: sw          $t1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r9;
    // 0x80165554: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80165558: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x8016555C: jalr        $t9
    // 0x80165560: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80165560: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80165564: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80165568: lw          $t1, 0x20($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X20);
    // 0x8016556C: lw          $v1, 0x8($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X8);
L_80165570:
    // 0x80165570: sra         $t4, $v1, 8
    ctx->r12 = S32(SIGNED(ctx->r3) >> 8);
    // 0x80165574: sh          $t4, 0x5C($t1)
    MEM_H(0X5C, ctx->r9) = ctx->r12;
    // 0x80165578: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8016557C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80165580: jr          $ra
    // 0x80165584: nop

    return;
    // 0x80165584: nop

;}
RECOMP_FUNC void func_80165588(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165588: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016558C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165590: lw          $t6, 0x38($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X38);
    // 0x80165594: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x80165598: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8016559C: beq         $t6, $zero, L_801655B4
    if (ctx->r14 == 0) {
        // 0x801655A0: addiu       $t9, $t9, 0x938
        ctx->r25 = ADD32(ctx->r25, 0X938);
            goto L_801655B4;
    }
    // 0x801655A0: addiu       $t9, $t9, 0x938
    ctx->r25 = ADD32(ctx->r25, 0X938);
    // 0x801655A4: lw          $a0, 0x38($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X38);
    // 0x801655A8: jalr        $t9
    // 0x801655AC: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801655AC: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    after_0:
    // 0x801655B0: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
L_801655B4:
    // 0x801655B4: addiu       $t7, $a1, 0x34
    ctx->r15 = ADD32(ctx->r5, 0X34);
    // 0x801655B8: sw          $t7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r15;
    // 0x801655BC: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x801655C0: jal         0x801469F4
    // 0x801655C4: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    func_801469F4(rdram, ctx);
        goto after_1;
    // 0x801655C4: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    after_1:
    // 0x801655C8: lw          $t8, 0x18($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X18);
    // 0x801655CC: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x801655D0: lw          $t9, 0x0($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X0);
    // 0x801655D4: jalr        $t9
    // 0x801655D8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801655D8: nop

    after_2:
    // 0x801655DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801655E0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801655E4: jr          $ra
    // 0x801655E8: nop

    return;
    // 0x801655E8: nop

;}
RECOMP_FUNC void func_80165600(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165600: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165604: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165608: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8016560C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80165610: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80165614: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80165618: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8016561C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80165620: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80165624: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80165628: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8016562C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80165630: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80165634: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80165638: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8016563C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80165640: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80165644: lw          $t9, 0x1FB0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1FB0);
    // 0x80165648: jalr        $t9
    // 0x8016564C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8016564C: nop

    after_0:
    // 0x80165650: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80165654: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80165658: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8016565C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80165660: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165664: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80165668: jr          $ra
    // 0x8016566C: nop

    return;
    // 0x8016566C: nop

;}
RECOMP_FUNC void func_80165670(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165670: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80165674: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80165678: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016567C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80165680: jal         0x80165A84
    // 0x80165684: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    func_80165A84(rdram, ctx);
        goto after_0;
    // 0x80165684: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    after_0:
    // 0x80165688: bne         $v0, $zero, L_801656A8
    if (ctx->r2 != 0) {
        // 0x8016568C: sw          $v0, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r2;
            goto L_801656A8;
    }
    // 0x8016568C: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x80165690: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x80165694: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80165698: jalr        $t9
    // 0x8016569C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8016569C: nop

    after_1:
    // 0x801656A0: b           L_80165828
    // 0x801656A4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80165828;
    // 0x801656A4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801656A8:
    // 0x801656A8: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x801656AC: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x801656B0: jal         0x80005A30
    // 0x801656B4: addiu       $a0, $zero, 0x201
    ctx->r4 = ADD32(0, 0X201);
    sceneLookup(rdram, ctx);
        goto after_2;
    // 0x801656B4: addiu       $a0, $zero, 0x201
    ctx->r4 = ADD32(0, 0X201);
    after_2:
    // 0x801656B8: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x801656BC: bne         $v0, $zero, L_801656DC
    if (ctx->r2 != 0) {
        // 0x801656C0: sw          $v0, 0x24($s1)
        MEM_W(0X24, ctx->r17) = ctx->r2;
            goto L_801656DC;
    }
    // 0x801656C0: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x801656C4: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x801656C8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801656CC: jalr        $t9
    // 0x801656D0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801656D0: nop

    after_3:
    // 0x801656D4: b           L_80165828
    // 0x801656D8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80165828;
    // 0x801656D8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801656DC:
    // 0x801656DC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801656E0: jal         0x801431CC
    // 0x801656E4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    sceneStateCopy(rdram, ctx);
        goto after_4;
    // 0x801656E4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_4:
    // 0x801656E8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801656EC: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x801656F0: lw          $a1, 0x2B14($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X2B14);
    // 0x801656F4: lhu         $t0, 0x2($s0)
    ctx->r8 = MEM_HU(ctx->r16, 0X2);
    // 0x801656F8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x801656FC: sw          $a1, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r5;
    // 0x80165700: lw          $t7, 0x20($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X20);
    // 0x80165704: ori         $t1, $t0, 0xD00
    ctx->r9 = ctx->r8 | 0XD00;
    // 0x80165708: addiu       $t4, $zero, 0x80
    ctx->r12 = ADD32(0, 0X80);
    // 0x8016570C: lw          $a0, 0x4($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X4);
    // 0x80165710: sh          $t1, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r9;
    // 0x80165714: addiu       $t9, $t9, 0x6504
    ctx->r25 = ADD32(ctx->r25, 0X6504);
    // 0x80165718: sw          $a0, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r4;
    // 0x8016571C: lw          $t2, 0x2B0C($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X2B0C);
    // 0x80165720: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x80165724: sw          $t2, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r10;
    // 0x80165728: lw          $t3, 0x7C($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X7C);
    // 0x8016572C: sb          $t4, 0x1F($s0)
    MEM_B(0X1F, ctx->r16) = ctx->r12;
    // 0x80165730: jalr        $t9
    // 0x80165734: sw          $t3, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r11;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80165734: sw          $t3, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r11;
    after_5:
    // 0x80165738: sw          $v0, 0x74($s0)
    MEM_W(0X74, ctx->r16) = ctx->r2;
    // 0x8016573C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80165740: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80165744: jal         0x80147584
    // 0x80165748: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80147584(rdram, ctx);
        goto after_6;
    // 0x80165748: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_6:
    // 0x8016574C: bne         $v0, $zero, L_8016576C
    if (ctx->r2 != 0) {
        // 0x80165750: sw          $v0, 0x24($sp)
        MEM_W(0X24, ctx->r29) = ctx->r2;
            goto L_8016576C;
    }
    // 0x80165750: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x80165754: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x80165758: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x8016575C: jalr        $t9
    // 0x80165760: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80165760: nop

    after_7:
    // 0x80165764: b           L_80165828
    // 0x80165768: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80165828;
    // 0x80165768: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016576C:
    // 0x8016576C: addiu       $a3, $s1, 0x34
    ctx->r7 = ADD32(ctx->r17, 0X34);
    // 0x80165770: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80165774: addiu       $a2, $zero, 0x99
    ctx->r6 = ADD32(0, 0X99);
    // 0x80165778: addiu       $t6, $zero, 0x80
    ctx->r14 = ADD32(0, 0X80);
    // 0x8016577C: sb          $a2, 0x2A($v0)
    MEM_B(0X2A, ctx->r2) = ctx->r6;
    // 0x80165780: sb          $a2, 0x29($v0)
    MEM_B(0X29, ctx->r2) = ctx->r6;
    // 0x80165784: sb          $a2, 0x28($v0)
    MEM_B(0X28, ctx->r2) = ctx->r6;
    // 0x80165788: sh          $t6, 0x2E($v0)
    MEM_H(0X2E, ctx->r2) = ctx->r14;
    // 0x8016578C: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x80165790: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80165794: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80165798: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
L_8016579C:
    // 0x8016579C: lwc1        $f4, 0x10($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X10);
    // 0x801657A0: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x801657A4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801657A8: swc1        $f4, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f4.u32l;
    // 0x801657AC: lwc1        $f6, 0x14($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X14);
    // 0x801657B0: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x801657B4: bne         $a1, $v0, L_8016579C
    if (ctx->r5 != ctx->r2) {
        // 0x801657B8: swc1        $f6, 0xC($a0)
        MEM_W(0XC, ctx->r4) = ctx->f6.u32l;
            goto L_8016579C;
    }
    // 0x801657B8: swc1        $f6, 0xC($a0)
    MEM_W(0XC, ctx->r4) = ctx->f6.u32l;
    // 0x801657BC: sw          $zero, 0x18($a3)
    MEM_W(0X18, ctx->r7) = 0;
    // 0x801657C0: jal         0x80146AC0
    // 0x801657C4: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    func_80146AC0(rdram, ctx);
        goto after_8;
    // 0x801657C4: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    after_8:
    // 0x801657C8: sw          $v0, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->r2;
    // 0x801657CC: jal         0x80143234
    // 0x801657D0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80143234(rdram, ctx);
        goto after_9;
    // 0x801657D0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_9:
    // 0x801657D4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801657D8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801657DC: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x801657E0: jalr        $t9
    // 0x801657E4: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x801657E4: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_10:
    // 0x801657E8: lh          $t7, 0xE($s1)
    ctx->r15 = MEM_H(ctx->r17, 0XE);
    // 0x801657EC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801657F0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801657F4: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x801657F8: addu        $v0, $s1, $t8
    ctx->r2 = ADD32(ctx->r17, ctx->r24);
    // 0x801657FC: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80165800: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80165804: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80165808: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x8016580C: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80165810: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80165814: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80165818: lw          $t9, 0x1FB0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1FB0);
    // 0x8016581C: jalr        $t9
    // 0x80165820: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x80165820: nop

    after_11:
    // 0x80165824: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165828:
    // 0x80165828: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016582C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80165830: jr          $ra
    // 0x80165834: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80165834: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80165838(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165838: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8016583C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165840: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80165844: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x80165848: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8016584C: lw          $a3, 0x34($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X34);
    // 0x80165850: lbu         $t7, 0x2B0C($v1)
    ctx->r15 = MEM_BU(ctx->r3, 0X2B0C);
    // 0x80165854: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80165858: sw          $t6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r14;
    // 0x8016585C: sb          $t7, 0x28($a3)
    MEM_B(0X28, ctx->r7) = ctx->r15;
    // 0x80165860: lbu         $t8, 0x2B0D($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0X2B0D);
    // 0x80165864: addiu       $a1, $v0, 0x4
    ctx->r5 = ADD32(ctx->r2, 0X4);
    // 0x80165868: sb          $t8, 0x29($a3)
    MEM_B(0X29, ctx->r7) = ctx->r24;
    // 0x8016586C: lbu         $t9, 0x2B0E($v1)
    ctx->r25 = MEM_BU(ctx->r3, 0X2B0E);
    // 0x80165870: sb          $t9, 0x2A($a3)
    MEM_B(0X2A, ctx->r7) = ctx->r25;
    // 0x80165874: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x80165878: jal         0x8014314C
    // 0x8016587C: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    func_8014314C(rdram, ctx);
        goto after_0;
    // 0x8016587C: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    after_0:
    // 0x80165880: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80165884: beq         $v0, $zero, L_801658E8
    if (ctx->r2 == 0) {
        // 0x80165888: lw          $a3, 0x1C($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X1C);
            goto L_801658E8;
    }
    // 0x80165888: lw          $a3, 0x1C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1C);
    // 0x8016588C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165890: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80165894: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80165898: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x8016589C: jalr        $t9
    // 0x801658A0: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801658A0: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_1:
    // 0x801658A4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x801658A8: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801658AC: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801658B0: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x801658B4: addu        $v0, $a0, $t2
    ctx->r2 = ADD32(ctx->r4, ctx->r10);
    // 0x801658B8: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801658BC: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801658C0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801658C4: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801658C8: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801658CC: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x801658D0: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801658D4: lw          $t9, 0x1FB0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1FB0);
    // 0x801658D8: jalr        $t9
    // 0x801658DC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801658DC: nop

    after_2:
    // 0x801658E0: b           L_80165A54
    // 0x801658E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80165A54;
    // 0x801658E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801658E8:
    // 0x801658E8: lbu         $t7, 0x12($a3)
    ctx->r15 = MEM_BU(ctx->r7, 0X12);
    // 0x801658EC: lbu         $t2, 0x13($a3)
    ctx->r10 = MEM_BU(ctx->r7, 0X13);
    // 0x801658F0: mtc1        $zero, $f3
    ctx->f_odd[(3 - 1) * 2] = 0;
    // 0x801658F4: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x801658F8: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x801658FC: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x80165900: sllv        $t1, $t8, $t7
    ctx->r9 = S32(ctx->r24 << (ctx->r15 & 31));
    // 0x80165904: mtc1        $t1, $f18
    ctx->f18.u32l = ctx->r9;
    // 0x80165908: sllv        $t4, $t3, $t2
    ctx->r12 = S32(ctx->r11 << (ctx->r10 & 31));
    // 0x8016590C: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x80165910: cvt.s.w     $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    ctx->f12.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80165914: addiu       $t0, $a2, 0x34
    ctx->r8 = ADD32(ctx->r6, 0X34);
    // 0x80165918: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016591C: or          $v0, $a3, $zero
    ctx->r2 = ctx->r7 | 0;
    // 0x80165920: or          $v1, $t0, $zero
    ctx->r3 = ctx->r8 | 0;
    // 0x80165924: cvt.s.w     $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    ctx->f14.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80165928: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
L_8016592C:
    // 0x8016592C: lwc1        $f8, 0x18($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X18);
    // 0x80165930: lwc1        $f10, 0x8($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X8);
    // 0x80165934: lwc1        $f18, 0x20($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X20);
    // 0x80165938: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x8016593C: add.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80165940: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80165944: swc1        $f16, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f16.u32l;
    // 0x80165948: lwc1        $f0, 0x18($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X18);
    // 0x8016594C: lwc1        $f4, 0xC($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0XC);
    // 0x80165950: cvt.d.s     $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f8.d = CVT_D_S(ctx->f0.fl);
    // 0x80165954: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x80165958: c.lt.d      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.d < ctx->f2.d;
    // 0x8016595C: swc1        $f6, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f6.u32l;
    // 0x80165960: bc1fl       L_80165978
    if (!c1cs) {
        // 0x80165964: c.lt.s      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
            goto L_80165978;
    }
    goto skip_0;
    // 0x80165964: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    skip_0:
    // 0x80165968: add.s       $f10, $f0, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = ctx->f0.fl + ctx->f12.fl;
    // 0x8016596C: b           L_8016598C
    // 0x80165970: swc1        $f10, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f10.u32l;
        goto L_8016598C;
    // 0x80165970: swc1        $f10, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f10.u32l;
    // 0x80165974: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
L_80165978:
    // 0x80165978: nop

    // 0x8016597C: bc1fl       L_80165990
    if (!c1cs) {
        // 0x80165980: lwc1        $f0, 0x20($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X20);
            goto L_80165990;
    }
    goto skip_1;
    // 0x80165980: lwc1        $f0, 0x20($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X20);
    skip_1:
    // 0x80165984: sub.s       $f16, $f0, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f12.fl;
    // 0x80165988: swc1        $f16, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f16.u32l;
L_8016598C:
    // 0x8016598C: lwc1        $f0, 0x20($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X20);
L_80165990:
    // 0x80165990: cvt.d.s     $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f18.d = CVT_D_S(ctx->f0.fl);
    // 0x80165994: c.lt.d      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.d < ctx->f2.d;
    // 0x80165998: nop

    // 0x8016599C: bc1fl       L_801659B4
    if (!c1cs) {
        // 0x801659A0: c.lt.s      $f14, $f0
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl < ctx->f0.fl;
            goto L_801659B4;
    }
    goto skip_2;
    // 0x801659A0: c.lt.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl < ctx->f0.fl;
    skip_2:
    // 0x801659A4: add.s       $f4, $f0, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = ctx->f0.fl + ctx->f14.fl;
    // 0x801659A8: b           L_801659C8
    // 0x801659AC: swc1        $f4, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f4.u32l;
        goto L_801659C8;
    // 0x801659AC: swc1        $f4, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f4.u32l;
    // 0x801659B0: c.lt.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl < ctx->f0.fl;
L_801659B4:
    // 0x801659B4: nop

    // 0x801659B8: bc1f        L_801659C8
    if (!c1cs) {
        // 0x801659BC: nop
    
            goto L_801659C8;
    }
    // 0x801659BC: nop

    // 0x801659C0: sub.s       $f6, $f0, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = ctx->f0.fl - ctx->f14.fl;
    // 0x801659C4: swc1        $f6, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f6.u32l;
L_801659C8:
    // 0x801659C8: bne         $a0, $a1, L_8016592C
    if (ctx->r4 != ctx->r5) {
        // 0x801659CC: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_8016592C;
    }
    // 0x801659CC: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x801659D0: lw          $t5, 0x18($t0)
    ctx->r13 = MEM_W(ctx->r8, 0X18);
    // 0x801659D4: addiu       $t6, $t5, 0x444
    ctx->r14 = ADD32(ctx->r13, 0X444);
    // 0x801659D8: sw          $t6, 0x18($t0)
    MEM_W(0X18, ctx->r8) = ctx->r14;
    // 0x801659DC: andi        $t8, $t6, 0xFFFF
    ctx->r24 = ctx->r14 & 0XFFFF;
    // 0x801659E0: sw          $t8, 0x18($t0)
    MEM_W(0X18, ctx->r8) = ctx->r24;
    // 0x801659E4: lhu         $a0, 0x1A($t0)
    ctx->r4 = MEM_HU(ctx->r8, 0X1A);
    // 0x801659E8: jal         0x80097330
    // 0x801659EC: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x801659EC: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    after_3:
    // 0x801659F0: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x801659F4: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x801659F8: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x801659FC: cvt.d.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.d = CVT_D_W(ctx->f8.u32l);
    // 0x80165A00: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80165A04: lui         $at, 0x4050
    ctx->r1 = S32(0X4050 << 16);
    // 0x80165A08: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x80165A0C: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x80165A10: div.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = DIV_D(ctx->f10.d, ctx->f16.d);
    // 0x80165A14: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80165A18: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x80165A1C: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x80165A20: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x80165A24: trunc.w.d   $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.u32l = TRUNC_W_D(ctx->f10.d);
    // 0x80165A28: mfc1        $t1, $f16
    ctx->r9 = (int32_t)ctx->f16.u32l;
    // 0x80165A2C: nop

    // 0x80165A30: addiu       $t3, $t1, 0x80
    ctx->r11 = ADD32(ctx->r9, 0X80);
    // 0x80165A34: jal         0x80146AC0
    // 0x80165A38: sh          $t3, 0x2C($a0)
    MEM_H(0X2C, ctx->r4) = ctx->r11;
    func_80146AC0(rdram, ctx);
        goto after_4;
    // 0x80165A38: sh          $t3, 0x2C($a0)
    MEM_H(0X2C, ctx->r4) = ctx->r11;
    after_4:
    // 0x80165A3C: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80165A40: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x80165A44: sw          $v0, 0x38($v1)
    MEM_W(0X38, ctx->r3) = ctx->r2;
    // 0x80165A48: lw          $t2, -0x7CC4($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X7CC4);
    // 0x80165A4C: sw          $t2, 0x24($v1)
    MEM_W(0X24, ctx->r3) = ctx->r10;
    // 0x80165A50: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80165A54:
    // 0x80165A54: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80165A58: jr          $ra
    // 0x80165A5C: nop

    return;
    // 0x80165A5C: nop

;}
RECOMP_FUNC void func_80165A60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165A60: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165A64: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165A68: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80165A6C: jalr        $t9
    // 0x80165A70: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80165A70: nop

    after_0:
    // 0x80165A74: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165A78: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80165A7C: jr          $ra
    // 0x80165A80: nop

    return;
    // 0x80165A80: nop

;}
RECOMP_FUNC void func_80165A84(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165A84: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80165A88: lh          $v0, -0x5470($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X5470);
    // 0x80165A8C: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x80165A90: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x80165A94: beq         $v0, $at, L_80165ABC
    if (ctx->r2 == ctx->r1) {
        // 0x80165A98: addiu       $at, $zero, 0x5
        ctx->r1 = ADD32(0, 0X5);
            goto L_80165ABC;
    }
    // 0x80165A98: addiu       $at, $zero, 0x5
    ctx->r1 = ADD32(0, 0X5);
    // 0x80165A9C: beq         $v0, $at, L_80165ADC
    if (ctx->r2 == ctx->r1) {
        // 0x80165AA0: addiu       $at, $zero, 0xB
        ctx->r1 = ADD32(0, 0XB);
            goto L_80165ADC;
    }
    // 0x80165AA0: addiu       $at, $zero, 0xB
    ctx->r1 = ADD32(0, 0XB);
    // 0x80165AA4: beq         $v0, $at, L_80165AFC
    if (ctx->r2 == ctx->r1) {
        // 0x80165AA8: addiu       $at, $zero, 0xF
        ctx->r1 = ADD32(0, 0XF);
            goto L_80165AFC;
    }
    // 0x80165AA8: addiu       $at, $zero, 0xF
    ctx->r1 = ADD32(0, 0XF);
    // 0x80165AAC: beql        $v0, $at, L_80165B20
    if (ctx->r2 == ctx->r1) {
        // 0x80165AB0: lhu         $t5, 0x18($v1)
        ctx->r13 = MEM_HU(ctx->r3, 0X18);
            goto L_80165B20;
    }
    goto skip_0;
    // 0x80165AB0: lhu         $t5, 0x18($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X18);
    skip_0:
    // 0x80165AB4: b           L_80165B3C
    // 0x80165AB8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80165B3C;
    // 0x80165AB8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80165ABC:
    // 0x80165ABC: lhu         $t6, 0x18($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X18);
    // 0x80165AC0: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80165AC4: addiu       $t8, $t8, 0x1E20
    ctx->r24 = ADD32(ctx->r24, 0X1E20);
    // 0x80165AC8: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80165ACC: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80165AD0: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x80165AD4: jr          $ra
    // 0x80165AD8: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    return;
    // 0x80165AD8: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
L_80165ADC:
    // 0x80165ADC: lhu         $t9, 0x18($v1)
    ctx->r25 = MEM_HU(ctx->r3, 0X18);
    // 0x80165AE0: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80165AE4: addiu       $t1, $t1, 0x1EC0
    ctx->r9 = ADD32(ctx->r9, 0X1EC0);
    // 0x80165AE8: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80165AEC: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80165AF0: sll         $t0, $t0, 3
    ctx->r8 = S32(ctx->r8 << 3);
    // 0x80165AF4: jr          $ra
    // 0x80165AF8: addu        $v0, $t0, $t1
    ctx->r2 = ADD32(ctx->r8, ctx->r9);
    return;
    // 0x80165AF8: addu        $v0, $t0, $t1
    ctx->r2 = ADD32(ctx->r8, ctx->r9);
L_80165AFC:
    // 0x80165AFC: lhu         $t2, 0x18($v1)
    ctx->r10 = MEM_HU(ctx->r3, 0X18);
    // 0x80165B00: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80165B04: addiu       $t4, $t4, 0x1F10
    ctx->r12 = ADD32(ctx->r12, 0X1F10);
    // 0x80165B08: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80165B0C: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x80165B10: sll         $t3, $t3, 3
    ctx->r11 = S32(ctx->r11 << 3);
    // 0x80165B14: jr          $ra
    // 0x80165B18: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    return;
    // 0x80165B18: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    // 0x80165B1C: lhu         $t5, 0x18($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X18);
L_80165B20:
    // 0x80165B20: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80165B24: addiu       $t7, $t7, 0x1F60
    ctx->r15 = ADD32(ctx->r15, 0X1F60);
    // 0x80165B28: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x80165B2C: addu        $t6, $t6, $t5
    ctx->r14 = ADD32(ctx->r14, ctx->r13);
    // 0x80165B30: sll         $t6, $t6, 3
    ctx->r14 = S32(ctx->r14 << 3);
    // 0x80165B34: jr          $ra
    // 0x80165B38: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
    return;
    // 0x80165B38: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
L_80165B3C:
    // 0x80165B3C: jr          $ra
    // 0x80165B40: nop

    return;
    // 0x80165B40: nop

;}
RECOMP_FUNC void func_80165B44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165B44: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165B48: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165B4C: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80165B50: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80165B54: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80165B58: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80165B5C: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80165B60: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80165B64: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80165B68: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80165B6C: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80165B70: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80165B74: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80165B78: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80165B7C: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80165B80: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80165B84: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80165B88: lw          $t9, 0x1FBC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1FBC);
    // 0x80165B8C: jalr        $t9
    // 0x80165B90: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80165B90: nop

    after_0:
    // 0x80165B94: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80165B98: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80165B9C: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80165BA0: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80165BA4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165BA8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80165BAC: jr          $ra
    // 0x80165BB0: nop

    return;
    // 0x80165BB0: nop

;}
RECOMP_FUNC void func_80165BB4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165BB4: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80165BB8: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x80165BBC: lw          $t6, 0x27A8($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X27A8);
    // 0x80165BC0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80165BC4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80165BC8: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80165BCC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80165BD0: addiu       $a1, $a1, 0x1F88
    ctx->r5 = ADD32(ctx->r5, 0X1F88);
    // 0x80165BD4: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80165BD8: jal         0x80147584
    // 0x80165BDC: sw          $t6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r14;
    func_80147584(rdram, ctx);
        goto after_0;
    // 0x80165BDC: sw          $t6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r14;
    after_0:
    // 0x80165BE0: bne         $v0, $zero, L_80165C00
    if (ctx->r2 != 0) {
        // 0x80165BE4: sw          $v0, 0x48($sp)
        MEM_W(0X48, ctx->r29) = ctx->r2;
            goto L_80165C00;
    }
    // 0x80165BE4: sw          $v0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r2;
    // 0x80165BE8: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80165BEC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165BF0: jalr        $t9
    // 0x80165BF4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80165BF4: nop

    after_1:
    // 0x80165BF8: b           L_80165DD4
    // 0x80165BFC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80165DD4;
    // 0x80165BFC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165C00:
    // 0x80165C00: addiu       $a2, $s0, 0x34
    ctx->r6 = ADD32(ctx->r16, 0X34);
    // 0x80165C04: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x80165C08: addiu       $v1, $zero, 0x99
    ctx->r3 = ADD32(0, 0X99);
    // 0x80165C0C: addiu       $t7, $zero, 0x80
    ctx->r15 = ADD32(0, 0X80);
    // 0x80165C10: addiu       $t0, $zero, 0x2
    ctx->r8 = ADD32(0, 0X2);
    // 0x80165C14: sh          $t7, 0x2C($v0)
    MEM_H(0X2C, ctx->r2) = ctx->r15;
    // 0x80165C18: sb          $v1, 0x2A($v0)
    MEM_B(0X2A, ctx->r2) = ctx->r3;
    // 0x80165C1C: sb          $v1, 0x29($v0)
    MEM_B(0X29, ctx->r2) = ctx->r3;
    // 0x80165C20: sb          $v1, 0x28($v0)
    MEM_B(0X28, ctx->r2) = ctx->r3;
    // 0x80165C24: sb          $t0, 0x2B($v0)
    MEM_B(0X2B, ctx->r2) = ctx->r8;
    // 0x80165C28: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x80165C2C: lh          $t1, -0x5470($t1)
    ctx->r9 = MEM_H(ctx->r9, -0X5470);
    // 0x80165C30: addiu       $at, $zero, 0x2E
    ctx->r1 = ADD32(0, 0X2E);
    // 0x80165C34: addiu       $a0, $zero, 0x204
    ctx->r4 = ADD32(0, 0X204);
    // 0x80165C38: bne         $t1, $at, L_80165C54
    if (ctx->r9 != ctx->r1) {
        // 0x80165C3C: lui         $a1, 0x8019
        ctx->r5 = S32(0X8019 << 16);
            goto L_80165C54;
    }
    // 0x80165C3C: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80165C40: addiu       $t2, $zero, 0x4
    ctx->r10 = ADD32(0, 0X4);
    // 0x80165C44: sb          $t2, 0x2B($v0)
    MEM_B(0X2B, ctx->r2) = ctx->r10;
    // 0x80165C48: sb          $zero, 0x2A($v0)
    MEM_B(0X2A, ctx->r2) = 0;
    // 0x80165C4C: sb          $zero, 0x29($v0)
    MEM_B(0X29, ctx->r2) = 0;
    // 0x80165C50: sb          $zero, 0x28($v0)
    MEM_B(0X28, ctx->r2) = 0;
L_80165C54:
    // 0x80165C54: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165C58: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x80165C5C: lw          $a1, 0x27A4($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A4);
    // 0x80165C60: jalr        $t9
    // 0x80165C64: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80165C64: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_2:
    // 0x80165C68: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x80165C6C: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x80165C70: bne         $v0, $zero, L_80165C90
    if (ctx->r2 != 0) {
        // 0x80165C74: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80165C90;
    }
    // 0x80165C74: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80165C78: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80165C7C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165C80: jalr        $t9
    // 0x80165C84: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80165C84: nop

    after_3:
    // 0x80165C88: b           L_80165DD4
    // 0x80165C8C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80165DD4;
    // 0x80165C8C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165C90:
    // 0x80165C90: lw          $v0, 0x4C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X4C);
    // 0x80165C94: lhu         $t4, 0x2($a1)
    ctx->r12 = MEM_HU(ctx->r5, 0X2);
    // 0x80165C98: sw          $zero, 0x1C($a1)
    MEM_W(0X1C, ctx->r5) = 0;
    // 0x80165C9C: addiu       $t3, $zero, 0x80
    ctx->r11 = ADD32(0, 0X80);
    // 0x80165CA0: ori         $t5, $t4, 0x480
    ctx->r13 = ctx->r12 | 0X480;
    // 0x80165CA4: sb          $t3, 0x1F($a1)
    MEM_B(0X1F, ctx->r5) = ctx->r11;
    // 0x80165CA8: sh          $t5, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r13;
    // 0x80165CAC: lwc1        $f4, 0x50($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80165CB0: swc1        $f4, 0x50($a1)
    MEM_W(0X50, ctx->r5) = ctx->f4.u32l;
    // 0x80165CB4: lwc1        $f12, 0x54($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X54);
    // 0x80165CB8: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x80165CBC: jal         0x80165F54
    // 0x80165CC0: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    func_80165F54(rdram, ctx);
        goto after_4;
    // 0x80165CC0: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    after_4:
    // 0x80165CC4: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    // 0x80165CC8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80165CCC: lwc1        $f2, -0x5250($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X5250);
    // 0x80165CD0: swc1        $f0, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f0.u32l;
    // 0x80165CD4: lw          $t6, 0x4C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X4C);
    // 0x80165CD8: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x80165CDC: lui         $t8, 0x600
    ctx->r24 = S32(0X600 << 16);
    // 0x80165CE0: lwc1        $f6, 0x58($t6)
    ctx->f6.u32l = MEM_W(ctx->r14, 0X58);
    // 0x80165CE4: swc1        $f2, 0x68($a1)
    MEM_W(0X68, ctx->r5) = ctx->f2.u32l;
    // 0x80165CE8: swc1        $f2, 0x6C($a1)
    MEM_W(0X6C, ctx->r5) = ctx->f2.u32l;
    // 0x80165CEC: swc1        $f2, 0x70($a1)
    MEM_W(0X70, ctx->r5) = ctx->f2.u32l;
    // 0x80165CF0: swc1        $f6, 0x58($a1)
    MEM_W(0X58, ctx->r5) = ctx->f6.u32l;
    // 0x80165CF4: lw          $t7, -0x5224($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X5224);
    // 0x80165CF8: addiu       $t8, $t8, 0x3EC8
    ctx->r24 = ADD32(ctx->r24, 0X3EC8);
    // 0x80165CFC: sw          $t8, 0x3C($a1)
    MEM_W(0X3C, ctx->r5) = ctx->r24;
    // 0x80165D00: sw          $t7, 0x40($a1)
    MEM_W(0X40, ctx->r5) = ctx->r15;
    // 0x80165D04: jal         0x80146AC0
    // 0x80165D08: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X48);
    func_80146AC0(rdram, ctx);
        goto after_5;
    // 0x80165D08: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X48);
    after_5:
    // 0x80165D0C: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    // 0x80165D10: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x80165D14: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80165D18: sw          $v0, 0x38($a1)
    MEM_W(0X38, ctx->r5) = ctx->r2;
    // 0x80165D1C: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x80165D20: addiu       $v0, $v0, 0x1F90
    ctx->r2 = ADD32(ctx->r2, 0X1F90);
    // 0x80165D24: addiu       $v1, $v1, 0x1F88
    ctx->r3 = ADD32(ctx->r3, 0X1F88);
    // 0x80165D28: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
L_80165D2C:
    // 0x80165D2C: lwc1        $f8, 0x10($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X10);
    // 0x80165D30: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80165D34: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80165D38: swc1        $f8, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f8.u32l;
    // 0x80165D3C: lwc1        $f10, 0x14($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X14);
    // 0x80165D40: bne         $v1, $v0, L_80165D2C
    if (ctx->r3 != ctx->r2) {
        // 0x80165D44: swc1        $f10, 0xC($a0)
        MEM_W(0XC, ctx->r4) = ctx->f10.u32l;
            goto L_80165D2C;
    }
    // 0x80165D44: swc1        $f10, 0xC($a0)
    MEM_W(0XC, ctx->r4) = ctx->f10.u32l;
    // 0x80165D48: sw          $zero, 0x18($a2)
    MEM_W(0X18, ctx->r6) = 0;
    // 0x80165D4C: sh          $zero, 0x1C($a2)
    MEM_H(0X1C, ctx->r6) = 0;
    // 0x80165D50: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80165D54: lh          $t0, -0x5470($t0)
    ctx->r8 = MEM_H(ctx->r8, -0X5470);
    // 0x80165D58: addiu       $at, $zero, 0x2E
    ctx->r1 = ADD32(0, 0X2E);
    // 0x80165D5C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165D60: beq         $t0, $at, L_80165D78
    if (ctx->r8 == ctx->r1) {
        // 0x80165D64: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80165D78;
    }
    // 0x80165D64: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165D68: addiu       $t9, $t9, 0x2410
    ctx->r25 = ADD32(ctx->r25, 0X2410);
    // 0x80165D6C: jalr        $t9
    // 0x80165D70: addiu       $a1, $zero, 0x1C1
    ctx->r5 = ADD32(0, 0X1C1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x80165D70: addiu       $a1, $zero, 0x1C1
    ctx->r5 = ADD32(0, 0X1C1);
    after_6:
    // 0x80165D74: sw          $zero, 0x34($v0)
    MEM_W(0X34, ctx->r2) = 0;
L_80165D78:
    // 0x80165D78: jal         0x80143234
    // 0x80165D7C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80143234(rdram, ctx);
        goto after_7;
    // 0x80165D7C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_7:
    // 0x80165D80: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165D84: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80165D88: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80165D8C: jalr        $t9
    // 0x80165D90: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x80165D90: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_8:
    // 0x80165D94: lh          $t1, 0xE($s0)
    ctx->r9 = MEM_H(ctx->r16, 0XE);
    // 0x80165D98: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80165D9C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165DA0: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x80165DA4: addu        $v0, $s0, $t2
    ctx->r2 = ADD32(ctx->r16, ctx->r10);
    // 0x80165DA8: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x80165DAC: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x80165DB0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80165DB4: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x80165DB8: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x80165DBC: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x80165DC0: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x80165DC4: lw          $t9, 0x1FBC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1FBC);
    // 0x80165DC8: jalr        $t9
    // 0x80165DCC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x80165DCC: nop

    after_9:
    // 0x80165DD0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165DD4:
    // 0x80165DD4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80165DD8: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    // 0x80165DDC: jr          $ra
    // 0x80165DE0: nop

    return;
    // 0x80165DE0: nop

;}
RECOMP_FUNC void func_80165DE4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165DE4: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80165DE8: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x80165DEC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80165DF0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80165DF4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80165DF8: addiu       $at, $zero, 0x29
    ctx->r1 = ADD32(0, 0X29);
    // 0x80165DFC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80165E00: bne         $t6, $at, L_80165E6C
    if (ctx->r14 != ctx->r1) {
        // 0x80165E04: lw          $v0, 0x70($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X70);
            goto L_80165E6C;
    }
    // 0x80165E04: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80165E08: jal         0x8014314C
    // 0x80165E0C: addiu       $a1, $v0, 0x4
    ctx->r5 = ADD32(ctx->r2, 0X4);
    func_8014314C(rdram, ctx);
        goto after_0;
    // 0x80165E0C: addiu       $a1, $v0, 0x4
    ctx->r5 = ADD32(ctx->r2, 0X4);
    after_0:
    // 0x80165E10: beq         $v0, $zero, L_80165E6C
    if (ctx->r2 == 0) {
        // 0x80165E14: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_80165E6C;
    }
    // 0x80165E14: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80165E18: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80165E1C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80165E20: jalr        $t9
    // 0x80165E24: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80165E24: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_1:
    // 0x80165E28: lh          $t7, 0xE($s0)
    ctx->r15 = MEM_H(ctx->r16, 0XE);
    // 0x80165E2C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80165E30: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165E34: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x80165E38: addu        $v0, $s0, $t8
    ctx->r2 = ADD32(ctx->r16, ctx->r24);
    // 0x80165E3C: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80165E40: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80165E44: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80165E48: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80165E4C: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80165E50: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80165E54: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80165E58: lw          $t9, 0x1FBC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1FBC);
    // 0x80165E5C: jalr        $t9
    // 0x80165E60: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80165E60: nop

    after_2:
    // 0x80165E64: b           L_80165ED4
    // 0x80165E68: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80165ED4;
    // 0x80165E68: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165E6C:
    // 0x80165E6C: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    // 0x80165E70: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80165E74: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165E78: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80165E7C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80165E80: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80165E84: sll         $t4, $v1, 1
    ctx->r12 = S32(ctx->r3 << 1);
    // 0x80165E88: addu        $v0, $s0, $t4
    ctx->r2 = ADD32(ctx->r16, ctx->r12);
    // 0x80165E8C: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x80165E90: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x80165E94: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x80165E98: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80165E9C: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x80165EA0: sb          $t6, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r14;
    // 0x80165EA4: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x80165EA8: lw          $t9, 0x1FC8($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X1FC8);
    // 0x80165EAC: jalr        $t9
    // 0x80165EB0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80165EB0: nop

    after_3:
    // 0x80165EB4: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x80165EB8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80165EBC: addiu       $t1, $t0, -0x1
    ctx->r9 = ADD32(ctx->r8, -0X1);
    // 0x80165EC0: jal         0x80165FAC
    // 0x80165EC4: sh          $t1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r9;
    func_80165FAC(rdram, ctx);
        goto after_4;
    // 0x80165EC4: sh          $t1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r9;
    after_4:
    // 0x80165EC8: jal         0x80166130
    // 0x80165ECC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80166130(rdram, ctx);
        goto after_5;
    // 0x80165ECC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x80165ED0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80165ED4:
    // 0x80165ED4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80165ED8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80165EDC: jr          $ra
    // 0x80165EE0: nop

    return;
    // 0x80165EE0: nop

;}
RECOMP_FUNC void func_80165EE4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165EE4: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x80165EE8: lw          $v0, 0x27A8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X27A8);
    // 0x80165EEC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80165EF0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165EF4: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x80165EF8: lwc1        $f4, 0x50($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80165EFC: swc1        $f4, 0x50($v1)
    MEM_W(0X50, ctx->r3) = ctx->f4.u32l;
    // 0x80165F00: lwc1        $f6, 0x58($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X58);
    // 0x80165F04: swc1        $f6, 0x58($v1)
    MEM_W(0X58, ctx->r3) = ctx->f6.u32l;
    // 0x80165F08: lwc1        $f12, 0x54($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X54);
    // 0x80165F0C: jal         0x80165F54
    // 0x80165F10: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    func_80165F54(rdram, ctx);
        goto after_0;
    // 0x80165F10: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    after_0:
    // 0x80165F14: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80165F18: jal         0x80165F78
    // 0x80165F1C: swc1        $f0, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f0.u32l;
    func_80165F78(rdram, ctx);
        goto after_1;
    // 0x80165F1C: swc1        $f0, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f0.u32l;
    after_1:
    // 0x80165F20: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165F24: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80165F28: jr          $ra
    // 0x80165F2C: nop

    return;
    // 0x80165F2C: nop

;}
RECOMP_FUNC void func_80165F30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165F30: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165F34: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165F38: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80165F3C: jalr        $t9
    // 0x80165F40: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80165F40: nop

    after_0:
    // 0x80165F44: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165F48: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80165F4C: jr          $ra
    // 0x80165F50: nop

    return;
    // 0x80165F50: nop

;}
RECOMP_FUNC void func_80165F54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165F54: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80165F58: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165F5C: jal         0x80165F78
    // 0x80165F60: swc1        $f12, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f12.u32l;
    func_80165F78(rdram, ctx);
        goto after_0;
    // 0x80165F60: swc1        $f12, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f12.u32l;
    after_0:
    // 0x80165F64: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80165F68: lwc1        $f4, 0x18($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X18);
    // 0x80165F6C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80165F70: jr          $ra
    // 0x80165F74: add.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f4.fl;
    return;
    // 0x80165F74: add.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f4.fl;
;}
RECOMP_FUNC void func_80165F78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165F78: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80165F7C: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x80165F80: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80165F84: bne         $t6, $at, L_80165F98
    if (ctx->r14 != ctx->r1) {
        // 0x80165F88: lui         $at, 0x444D
        ctx->r1 = S32(0X444D << 16);
            goto L_80165F98;
    }
    // 0x80165F88: lui         $at, 0x444D
    ctx->r1 = S32(0X444D << 16);
    // 0x80165F8C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80165F90: jr          $ra
    // 0x80165F94: nop

    return;
    // 0x80165F94: nop

L_80165F98:
    // 0x80165F98: lui         $at, 0x433E
    ctx->r1 = S32(0X433E << 16);
    // 0x80165F9C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80165FA0: nop

    // 0x80165FA4: jr          $ra
    // 0x80165FA8: nop

    return;
    // 0x80165FA8: nop

;}
RECOMP_FUNC void func_80165FAC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80165FAC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80165FB0: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80165FB4: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80165FB8: lh          $t7, 0x28D0($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X28D0);
    // 0x80165FBC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80165FC0: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x80165FC4: lw          $a2, 0x34($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X34);
    // 0x80165FC8: addiu       $at, $zero, 0x2E
    ctx->r1 = ADD32(0, 0X2E);
    // 0x80165FCC: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80165FD0: bne         $t7, $at, L_80165FF4
    if (ctx->r15 != ctx->r1) {
        // 0x80165FD4: sw          $t6, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r14;
            goto L_80165FF4;
    }
    // 0x80165FD4: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    // 0x80165FD8: lbu         $t8, 0x2B0C($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X2B0C);
    // 0x80165FDC: sb          $t8, 0x28($a2)
    MEM_B(0X28, ctx->r6) = ctx->r24;
    // 0x80165FE0: lbu         $t9, 0x2B0D($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X2B0D);
    // 0x80165FE4: sb          $t9, 0x29($a2)
    MEM_B(0X29, ctx->r6) = ctx->r25;
    // 0x80165FE8: lbu         $t1, 0x2B0E($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X2B0E);
    // 0x80165FEC: b           L_8016600C
    // 0x80165FF0: sb          $t1, 0x2A($a2)
    MEM_B(0X2A, ctx->r6) = ctx->r9;
        goto L_8016600C;
    // 0x80165FF0: sb          $t1, 0x2A($a2)
    MEM_B(0X2A, ctx->r6) = ctx->r9;
L_80165FF4:
    // 0x80165FF4: lbu         $t2, 0x7C($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X7C);
    // 0x80165FF8: sb          $t2, 0x28($a2)
    MEM_B(0X28, ctx->r6) = ctx->r10;
    // 0x80165FFC: lbu         $t3, 0x7D($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X7D);
    // 0x80166000: sb          $t3, 0x29($a2)
    MEM_B(0X29, ctx->r6) = ctx->r11;
    // 0x80166004: lbu         $t4, 0x7E($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X7E);
    // 0x80166008: sb          $t4, 0x2A($a2)
    MEM_B(0X2A, ctx->r6) = ctx->r12;
L_8016600C:
    // 0x8016600C: addiu       $a3, $a1, 0x34
    ctx->r7 = ADD32(ctx->r5, 0X34);
    // 0x80166010: lui         $at, 0x4280
    ctx->r1 = S32(0X4280 << 16);
    // 0x80166014: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x80166018: mtc1        $zero, $f13
    ctx->f_odd[(13 - 1) * 2] = 0;
    // 0x8016601C: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x80166020: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
    // 0x80166024: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    // 0x80166028: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016602C: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
L_80166030:
    // 0x80166030: lwc1        $f4, 0x18($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X18);
    // 0x80166034: lwc1        $f6, 0x8($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X8);
    // 0x80166038: lwc1        $f10, 0x20($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X20);
    // 0x8016603C: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80166040: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80166044: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80166048: swc1        $f8, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f8.u32l;
    // 0x8016604C: lwc1        $f0, 0x18($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X18);
    // 0x80166050: lwc1        $f16, 0xC($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0XC);
    // 0x80166054: cvt.d.s     $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.d = CVT_D_S(ctx->f0.fl);
    // 0x80166058: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x8016605C: c.lt.d      $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f4.d < ctx->f12.d;
    // 0x80166060: swc1        $f18, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f18.u32l;
    // 0x80166064: bc1fl       L_8016607C
    if (!c1cs) {
        // 0x80166068: c.lt.s      $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
            goto L_8016607C;
    }
    goto skip_0;
    // 0x80166068: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
    skip_0:
    // 0x8016606C: add.s       $f6, $f0, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x80166070: b           L_80166090
    // 0x80166074: swc1        $f6, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f6.u32l;
        goto L_80166090;
    // 0x80166074: swc1        $f6, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f6.u32l;
    // 0x80166078: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
L_8016607C:
    // 0x8016607C: nop

    // 0x80166080: bc1fl       L_80166094
    if (!c1cs) {
        // 0x80166084: lwc1        $f0, 0x20($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X20);
            goto L_80166094;
    }
    goto skip_1;
    // 0x80166084: lwc1        $f0, 0x20($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X20);
    skip_1:
    // 0x80166088: sub.s       $f8, $f0, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x8016608C: swc1        $f8, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f8.u32l;
L_80166090:
    // 0x80166090: lwc1        $f0, 0x20($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X20);
L_80166094:
    // 0x80166094: cvt.d.s     $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f10.d = CVT_D_S(ctx->f0.fl);
    // 0x80166098: c.lt.d      $f10, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f10.d < ctx->f12.d;
    // 0x8016609C: nop

    // 0x801660A0: bc1fl       L_801660B8
    if (!c1cs) {
        // 0x801660A4: c.lt.s      $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
            goto L_801660B8;
    }
    goto skip_2;
    // 0x801660A4: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
    skip_2:
    // 0x801660A8: add.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x801660AC: b           L_801660CC
    // 0x801660B0: swc1        $f16, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f16.u32l;
        goto L_801660CC;
    // 0x801660B0: swc1        $f16, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f16.u32l;
    // 0x801660B4: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
L_801660B8:
    // 0x801660B8: nop

    // 0x801660BC: bc1f        L_801660CC
    if (!c1cs) {
        // 0x801660C0: nop
    
            goto L_801660CC;
    }
    // 0x801660C0: nop

    // 0x801660C4: sub.s       $f18, $f0, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x801660C8: swc1        $f18, 0x20($v0)
    MEM_W(0X20, ctx->r2) = ctx->f18.u32l;
L_801660CC:
    // 0x801660CC: bne         $a0, $a1, L_80166030
    if (ctx->r4 != ctx->r5) {
        // 0x801660D0: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_80166030;
    }
    // 0x801660D0: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x801660D4: lw          $t5, 0x18($a3)
    ctx->r13 = MEM_W(ctx->r7, 0X18);
    // 0x801660D8: addiu       $t6, $t5, 0x10
    ctx->r14 = ADD32(ctx->r13, 0X10);
    // 0x801660DC: sw          $t6, 0x18($a3)
    MEM_W(0X18, ctx->r7) = ctx->r14;
    // 0x801660E0: andi        $t8, $t6, 0xFFFF
    ctx->r24 = ctx->r14 & 0XFFFF;
    // 0x801660E4: sw          $t8, 0x18($a3)
    MEM_W(0X18, ctx->r7) = ctx->r24;
    // 0x801660E8: lhu         $a0, 0x1A($a3)
    ctx->r4 = MEM_HU(ctx->r7, 0X1A);
    // 0x801660EC: jal         0x80097330
    // 0x801660F0: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    func_80097330(rdram, ctx);
        goto after_0;
    // 0x801660F0: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_0:
    // 0x801660F4: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x801660F8: bgez        $v0, L_80166108
    if (SIGNED(ctx->r2) >= 0) {
        // 0x801660FC: sra         $t9, $v0, 9
        ctx->r25 = S32(SIGNED(ctx->r2) >> 9);
            goto L_80166108;
    }
    // 0x801660FC: sra         $t9, $v0, 9
    ctx->r25 = S32(SIGNED(ctx->r2) >> 9);
    // 0x80166100: addiu       $at, $v0, 0x1FF
    ctx->r1 = ADD32(ctx->r2, 0X1FF);
    // 0x80166104: sra         $t9, $at, 9
    ctx->r25 = S32(SIGNED(ctx->r1) >> 9);
L_80166108:
    // 0x80166108: addiu       $t1, $t9, 0x80
    ctx->r9 = ADD32(ctx->r25, 0X80);
    // 0x8016610C: sh          $t1, 0x2C($a2)
    MEM_H(0X2C, ctx->r6) = ctx->r9;
    // 0x80166110: jal         0x80146AC0
    // 0x80166114: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    func_80146AC0(rdram, ctx);
        goto after_1;
    // 0x80166114: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_1:
    // 0x80166118: lw          $t2, 0x1C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X1C);
    // 0x8016611C: sw          $v0, 0x38($t2)
    MEM_W(0X38, ctx->r10) = ctx->r2;
    // 0x80166120: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80166124: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80166128: jr          $ra
    // 0x8016612C: nop

    return;
    // 0x8016612C: nop

;}
RECOMP_FUNC void func_80166130(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166130: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80166134: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x80166138: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8016613C: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x80166140: bne         $t6, $at, L_8016615C
    if (ctx->r14 != ctx->r1) {
        // 0x80166144: lui         $at, 0x3FC0
        ctx->r1 = S32(0X3FC0 << 16);
            goto L_8016615C;
    }
    // 0x80166144: lui         $at, 0x3FC0
    ctx->r1 = S32(0X3FC0 << 16);
    // 0x80166148: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8016614C: nop

    // 0x80166150: swc1        $f0, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f0.u32l;
    // 0x80166154: jr          $ra
    // 0x80166158: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    return;
    // 0x80166158: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
L_8016615C:
    // 0x8016615C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80166160: lwc1        $f0, -0x524C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X524C);
    // 0x80166164: swc1        $f0, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f0.u32l;
    // 0x80166168: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x8016616C: jr          $ra
    // 0x80166170: nop

    return;
    // 0x80166170: nop

;}
RECOMP_FUNC void func_80166180(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166180: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80166184: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80166188: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8016618C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80166190: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80166194: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80166198: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8016619C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x801661A0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801661A4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801661A8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801661AC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801661B0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801661B4: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801661B8: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801661BC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801661C0: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801661C4: lw          $t9, 0x212C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X212C);
    // 0x801661C8: jalr        $t9
    // 0x801661CC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801661CC: nop

    after_0:
    // 0x801661D0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801661D4: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801661D8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801661DC: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801661E0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801661E4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801661E8: jr          $ra
    // 0x801661EC: nop

    return;
    // 0x801661EC: nop

;}
RECOMP_FUNC void func_801661F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801661F0: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x801661F4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801661F8: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801661FC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80166200: jal         0x80166DBC
    // 0x80166204: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    func_80166DBC(rdram, ctx);
        goto after_0;
    // 0x80166204: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    after_0:
    // 0x80166208: addiu       $v1, $s0, 0x34
    ctx->r3 = ADD32(ctx->r16, 0X34);
    // 0x8016620C: bne         $v0, $zero, L_8016622C
    if (ctx->r2 != 0) {
        // 0x80166210: sw          $v0, 0x4($v1)
        MEM_W(0X4, ctx->r3) = ctx->r2;
            goto L_8016622C;
    }
    // 0x80166210: sw          $v0, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r2;
    // 0x80166214: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80166218: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016621C: jalr        $t9
    // 0x80166220: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80166220: nop

    after_1:
    // 0x80166224: b           L_8016669C
    // 0x80166228: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016669C;
    // 0x80166228: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016622C:
    // 0x8016622C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166230: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x80166234: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80166238: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x8016623C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80166240: jalr        $t9
    // 0x80166244: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80166244: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    after_2:
    // 0x80166248: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x8016624C: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x80166250: bne         $v0, $zero, L_80166270
    if (ctx->r2 != 0) {
        // 0x80166254: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_80166270;
    }
    // 0x80166254: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x80166258: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x8016625C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166260: jalr        $t9
    // 0x80166264: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80166264: nop

    after_3:
    // 0x80166268: b           L_8016669C
    // 0x8016626C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016669C;
    // 0x8016626C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80166270:
    // 0x80166270: lhu         $t6, 0x2($s1)
    ctx->r14 = MEM_HU(ctx->r17, 0X2);
    // 0x80166274: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80166278: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8016627C: ori         $t7, $t6, 0x900
    ctx->r15 = ctx->r14 | 0X900;
    // 0x80166280: sh          $t7, 0x2($s1)
    MEM_H(0X2, ctx->r17) = ctx->r15;
    // 0x80166284: lw          $t8, 0x2B14($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2B14);
    // 0x80166288: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016628C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80166290: sw          $t8, 0x40($s1)
    MEM_W(0X40, ctx->r17) = ctx->r24;
    // 0x80166294: lw          $t0, 0x4($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X4);
    // 0x80166298: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x8016629C: sw          $t1, 0x3C($s1)
    MEM_W(0X3C, ctx->r17) = ctx->r9;
    // 0x801662A0: lw          $t2, 0x2B0C($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X2B0C);
    // 0x801662A4: sw          $t2, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->r10;
    // 0x801662A8: lw          $t3, 0x7C($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X7C);
    // 0x801662AC: sw          $t3, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r11;
    // 0x801662B0: jal         0x801431CC
    // 0x801662B4: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    sceneStateCopy(rdram, ctx);
        goto after_4;
    // 0x801662B4: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    after_4:
    // 0x801662B8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x801662BC: addiu       $t9, $t9, 0x6504
    ctx->r25 = ADD32(ctx->r25, 0X6504);
    // 0x801662C0: lw          $a0, 0x3C($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X3C);
    // 0x801662C4: lw          $a1, 0x40($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X40);
    // 0x801662C8: jalr        $t9
    // 0x801662CC: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x801662CC: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_5:
    // 0x801662D0: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x801662D4: sw          $v0, 0x74($s1)
    MEM_W(0X74, ctx->r17) = ctx->r2;
    // 0x801662D8: sw          $zero, 0x64($s0)
    MEM_W(0X64, ctx->r16) = 0;
    // 0x801662DC: lw          $t4, 0x4($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X4);
    // 0x801662E0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801662E4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801662E8: lw          $t5, 0x10($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X10);
    // 0x801662EC: addiu       $a2, $zero, 0x20
    ctx->r6 = ADD32(0, 0X20);
    // 0x801662F0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x801662F4: beq         $t5, $zero, L_80166328
    if (ctx->r13 == 0) {
        // 0x801662F8: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80166328;
    }
    // 0x801662F8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801662FC: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x80166300: jalr        $t9
    // 0x80166304: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x80166304: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    after_6:
    // 0x80166308: bne         $v0, $zero, L_80166328
    if (ctx->r2 != 0) {
        // 0x8016630C: lw          $v1, 0x28($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X28);
            goto L_80166328;
    }
    // 0x8016630C: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x80166310: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80166314: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166318: jalr        $t9
    // 0x8016631C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x8016631C: nop

    after_7:
    // 0x80166320: b           L_8016669C
    // 0x80166324: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016669C;
    // 0x80166324: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80166328:
    // 0x80166328: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
    // 0x8016632C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166330: lw          $t7, 0x4($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X4);
    // 0x80166334: sw          $t7, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->r15;
    // 0x80166338: lw          $t0, 0x0($t7)
    ctx->r8 = MEM_W(ctx->r15, 0X0);
    // 0x8016633C: sw          $zero, 0x14($v1)
    MEM_W(0X14, ctx->r3) = 0;
    // 0x80166340: sw          $zero, 0x18($v1)
    MEM_W(0X18, ctx->r3) = 0;
    // 0x80166344: sw          $t0, 0xC($v1)
    MEM_W(0XC, ctx->r3) = ctx->r8;
    // 0x80166348: lh          $t1, 0x5C($s1)
    ctx->r9 = MEM_H(ctx->r17, 0X5C);
    // 0x8016634C: sh          $zero, 0x20($v1)
    MEM_H(0X20, ctx->r3) = 0;
    // 0x80166350: sh          $zero, 0x22($v1)
    MEM_H(0X22, ctx->r3) = 0;
    // 0x80166354: sw          $t1, 0x1C($v1)
    MEM_W(0X1C, ctx->r3) = ctx->r9;
    // 0x80166358: jal         0x80143234
    // 0x8016635C: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    func_80143234(rdram, ctx);
        goto after_8;
    // 0x8016635C: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    after_8:
    // 0x80166360: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80166364: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x80166368: lh          $t2, 0x2874($a0)
    ctx->r10 = MEM_H(ctx->r4, 0X2874);
    // 0x8016636C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80166370: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x80166374: bnel        $v0, $t2, L_80166408
    if (ctx->r2 != ctx->r10) {
        // 0x80166378: lw          $t6, 0x4($v1)
        ctx->r14 = MEM_W(ctx->r3, 0X4);
            goto L_80166408;
    }
    goto skip_0;
    // 0x80166378: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
    skip_0:
    // 0x8016637C: lh          $t3, 0x28D0($a0)
    ctx->r11 = MEM_H(ctx->r4, 0X28D0);
    // 0x80166380: bnel        $v0, $t3, L_80166408
    if (ctx->r2 != ctx->r11) {
        // 0x80166384: lw          $t6, 0x4($v1)
        ctx->r14 = MEM_W(ctx->r3, 0X4);
            goto L_80166408;
    }
    goto skip_1;
    // 0x80166384: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
    skip_1:
    // 0x80166388: lw          $t5, 0x8($v1)
    ctx->r13 = MEM_W(ctx->r3, 0X8);
    // 0x8016638C: lw          $t4, 0x1C($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X1C);
    // 0x80166390: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166394: lw          $t9, 0x0($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X0);
    // 0x80166398: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x8016639C: bgez        $t9, L_801663AC
    if (SIGNED(ctx->r25) >= 0) {
        // 0x801663A0: sra         $t6, $t9, 8
        ctx->r14 = S32(SIGNED(ctx->r25) >> 8);
            goto L_801663AC;
    }
    // 0x801663A0: sra         $t6, $t9, 8
    ctx->r14 = S32(SIGNED(ctx->r25) >> 8);
    // 0x801663A4: addiu       $at, $t9, 0xFF
    ctx->r1 = ADD32(ctx->r25, 0XFF);
    // 0x801663A8: sra         $t6, $at, 8
    ctx->r14 = S32(SIGNED(ctx->r1) >> 8);
L_801663AC:
    // 0x801663AC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801663B0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801663B4: addu        $t8, $t4, $t6
    ctx->r24 = ADD32(ctx->r12, ctx->r14);
    // 0x801663B8: jalr        $t9
    // 0x801663BC: sh          $t8, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r24;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x801663BC: sh          $t8, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r24;
    after_9:
    // 0x801663C0: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x801663C4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801663C8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801663CC: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x801663D0: addu        $v0, $s0, $t1
    ctx->r2 = ADD32(ctx->r16, ctx->r9);
    // 0x801663D4: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801663D8: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x801663DC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801663E0: sll         $t4, $t5, 2
    ctx->r12 = S32(ctx->r13 << 2);
    // 0x801663E4: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x801663E8: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x801663EC: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
    // 0x801663F0: lw          $t9, 0x212C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X212C);
    // 0x801663F4: jalr        $t9
    // 0x801663F8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x801663F8: nop

    after_10:
    // 0x801663FC: b           L_8016669C
    // 0x80166400: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016669C;
    // 0x80166400: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80166404: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
L_80166408:
    // 0x80166408: lhu         $v0, 0x8($t6)
    ctx->r2 = MEM_HU(ctx->r14, 0X8);
    // 0x8016640C: beql        $v0, $zero, L_801664D4
    if (ctx->r2 == 0) {
        // 0x80166410: lw          $t7, 0x4($v1)
        ctx->r15 = MEM_W(ctx->r3, 0X4);
            goto L_801664D4;
    }
    goto skip_2;
    // 0x80166410: lw          $t7, 0x4($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X4);
    skip_2:
    // 0x80166414: beq         $v0, $zero, L_80166444
    if (ctx->r2 == 0) {
        // 0x80166418: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_80166444;
    }
    // 0x80166418: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016641C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166420: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80166424: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80166428: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8016642C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80166430: jalr        $t9
    // 0x80166434: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x80166434: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    after_11:
    // 0x80166438: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x8016643C: b           L_80166444
    // 0x80166440: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
        goto L_80166444;
    // 0x80166440: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
L_80166444:
    // 0x80166444: beq         $a0, $zero, L_801664D0
    if (ctx->r4 == 0) {
        // 0x80166448: addiu       $a1, $s0, 0xE
        ctx->r5 = ADD32(ctx->r16, 0XE);
            goto L_801664D0;
    }
    // 0x80166448: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x8016644C: lw          $t7, 0x1C($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X1C);
    // 0x80166450: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166454: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80166458: addiu       $t0, $t7, 0x1555
    ctx->r8 = ADD32(ctx->r15, 0X1555);
    // 0x8016645C: sh          $t0, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r8;
    // 0x80166460: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166464: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x80166468: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    // 0x8016646C: jalr        $t9
    // 0x80166470: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x80166470: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_12:
    // 0x80166474: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166478: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x8016647C: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x80166480: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80166484: jalr        $t9
    // 0x80166488: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_13;
    // 0x80166488: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_13:
    // 0x8016648C: lh          $t1, 0xE($s0)
    ctx->r9 = MEM_H(ctx->r16, 0XE);
    // 0x80166490: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80166494: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166498: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x8016649C: addu        $v0, $s0, $t2
    ctx->r2 = ADD32(ctx->r16, ctx->r10);
    // 0x801664A0: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x801664A4: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801664A8: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801664AC: sll         $t6, $t4, 2
    ctx->r14 = S32(ctx->r12 << 2);
    // 0x801664B0: addiu       $t5, $t3, 0x1
    ctx->r13 = ADD32(ctx->r11, 0X1);
    // 0x801664B4: sb          $t5, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r13;
    // 0x801664B8: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801664BC: lw          $t9, 0x212C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X212C);
    // 0x801664C0: jalr        $t9
    // 0x801664C4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_14;
    // 0x801664C4: nop

    after_14:
    // 0x801664C8: b           L_8016669C
    // 0x801664CC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016669C;
    // 0x801664CC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801664D0:
    // 0x801664D0: lw          $t7, 0x4($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X4);
L_801664D4:
    // 0x801664D4: lw          $a0, 0xC($t7)
    ctx->r4 = MEM_W(ctx->r15, 0XC);
    // 0x801664D8: beql        $a0, $zero, L_8016657C
    if (ctx->r4 == 0) {
        // 0x801664DC: lw          $t0, 0x8($v1)
        ctx->r8 = MEM_W(ctx->r3, 0X8);
            goto L_8016657C;
    }
    goto skip_3;
    // 0x801664DC: lw          $t0, 0x8($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X8);
    skip_3:
    // 0x801664E0: jal         0x80147718
    // 0x801664E4: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    requestBit_test(rdram, ctx);
        goto after_15;
    // 0x801664E4: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    after_15:
    // 0x801664E8: beq         $v0, $zero, L_80166578
    if (ctx->r2 == 0) {
        // 0x801664EC: lw          $v1, 0x28($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X28);
            goto L_80166578;
    }
    // 0x801664EC: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x801664F0: lw          $t8, 0x1C($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X1C);
    // 0x801664F4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801664F8: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801664FC: addiu       $t1, $t8, 0x1555
    ctx->r9 = ADD32(ctx->r24, 0X1555);
    // 0x80166500: sh          $t1, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r9;
    // 0x80166504: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166508: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x8016650C: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    // 0x80166510: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x80166514: jalr        $t9
    // 0x80166518: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_16;
    // 0x80166518: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_16:
    // 0x8016651C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166520: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x80166524: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x80166528: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x8016652C: jalr        $t9
    // 0x80166530: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_17;
    // 0x80166530: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    after_17:
    // 0x80166534: lh          $t2, 0xE($s0)
    ctx->r10 = MEM_H(ctx->r16, 0XE);
    // 0x80166538: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x8016653C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166540: sll         $t3, $t2, 1
    ctx->r11 = S32(ctx->r10 << 1);
    // 0x80166544: addu        $v0, $s0, $t3
    ctx->r2 = ADD32(ctx->r16, ctx->r11);
    // 0x80166548: lbu         $t6, 0x9($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X9);
    // 0x8016654C: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x80166550: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80166554: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80166558: addiu       $t4, $t5, 0x1
    ctx->r12 = ADD32(ctx->r13, 0X1);
    // 0x8016655C: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x80166560: addu        $t9, $t9, $t7
    ctx->r25 = ADD32(ctx->r25, ctx->r15);
    // 0x80166564: lw          $t9, 0x212C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X212C);
    // 0x80166568: jalr        $t9
    // 0x8016656C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_18;
    // 0x8016656C: nop

    after_18:
    // 0x80166570: b           L_8016669C
    // 0x80166574: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016669C;
    // 0x80166574: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80166578:
    // 0x80166578: lw          $t0, 0x8($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X8);
L_8016657C:
    // 0x8016657C: lw          $t8, 0x1C($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X1C);
    // 0x80166580: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x80166584: bgez        $t1, L_80166594
    if (SIGNED(ctx->r9) >= 0) {
        // 0x80166588: sra         $t2, $t1, 8
        ctx->r10 = S32(SIGNED(ctx->r9) >> 8);
            goto L_80166594;
    }
    // 0x80166588: sra         $t2, $t1, 8
    ctx->r10 = S32(SIGNED(ctx->r9) >> 8);
    // 0x8016658C: addiu       $at, $t1, 0xFF
    ctx->r1 = ADD32(ctx->r9, 0XFF);
    // 0x80166590: sra         $t2, $at, 8
    ctx->r10 = S32(SIGNED(ctx->r1) >> 8);
L_80166594:
    // 0x80166594: addu        $t5, $t8, $t2
    ctx->r13 = ADD32(ctx->r24, ctx->r10);
    // 0x80166598: sh          $t5, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r13;
    // 0x8016659C: lw          $t4, 0x4($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X4);
    // 0x801665A0: lw          $t6, 0x10($t4)
    ctx->r14 = MEM_W(ctx->r12, 0X10);
    // 0x801665A4: beq         $t6, $zero, L_80166648
    if (ctx->r14 == 0) {
        // 0x801665A8: nop
    
            goto L_80166648;
    }
    // 0x801665A8: nop

    // 0x801665AC: lw          $a1, 0x0($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X0);
    // 0x801665B0: lwc1        $f4, 0x50($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X50);
    // 0x801665B4: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x801665B8: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x801665BC: swc1        $f4, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f4.u32l;
    // 0x801665C0: lwc1        $f6, 0x54($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X54);
    // 0x801665C4: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801665C8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801665CC: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x801665D0: sub.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d - ctx->f10.d;
    // 0x801665D4: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x801665D8: swc1        $f18, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f18.u32l;
    // 0x801665DC: lwc1        $f4, 0x58($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X58);
    // 0x801665E0: swc1        $f4, 0xC($a1)
    MEM_W(0XC, ctx->r5) = ctx->f4.u32l;
    // 0x801665E4: lw          $t7, 0x4($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X4);
    // 0x801665E8: lw          $t9, 0x10($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X10);
    // 0x801665EC: lh          $t0, 0x0($t9)
    ctx->r8 = MEM_H(ctx->r25, 0X0);
    // 0x801665F0: sh          $t0, 0x10($a1)
    MEM_H(0X10, ctx->r5) = ctx->r8;
    // 0x801665F4: lw          $t1, 0x4($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X4);
    // 0x801665F8: lw          $t8, 0x10($t1)
    ctx->r24 = MEM_W(ctx->r9, 0X10);
    // 0x801665FC: lhu         $t2, 0x2($t8)
    ctx->r10 = MEM_HU(ctx->r24, 0X2);
    // 0x80166600: sh          $t2, 0x18($a1)
    MEM_H(0X18, ctx->r5) = ctx->r10;
    // 0x80166604: lw          $t3, 0x4($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X4);
    // 0x80166608: lw          $t5, 0x10($t3)
    ctx->r13 = MEM_W(ctx->r11, 0X10);
    // 0x8016660C: lhu         $t4, 0x4($t5)
    ctx->r12 = MEM_HU(ctx->r13, 0X4);
    // 0x80166610: sh          $t4, 0x14($a1)
    MEM_H(0X14, ctx->r5) = ctx->r12;
    // 0x80166614: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
    // 0x80166618: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x8016661C: lhu         $t9, 0x6($t7)
    ctx->r25 = MEM_HU(ctx->r15, 0X6);
    // 0x80166620: jal         0x80188F84
    // 0x80166624: sh          $t9, 0x16($a1)
    MEM_H(0X16, ctx->r5) = ctx->r25;
    interactable_createWithSettings(rdram, ctx);
        goto after_19;
    // 0x80166624: sh          $t9, 0x16($a1)
    MEM_H(0X16, ctx->r5) = ctx->r25;
    after_19:
    // 0x80166628: bne         $v0, $zero, L_80166648
    if (ctx->r2 != 0) {
        // 0x8016662C: nop
    
            goto L_80166648;
    }
    // 0x8016662C: nop

    // 0x80166630: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80166634: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166638: jalr        $t9
    // 0x8016663C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_20;
    // 0x8016663C: nop

    after_20:
    // 0x80166640: b           L_8016669C
    // 0x80166644: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016669C;
    // 0x80166644: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80166648:
    // 0x80166648: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8016664C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80166650: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166654: jalr        $t9
    // 0x80166658: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_21;
    // 0x80166658: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_21:
    // 0x8016665C: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x80166660: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80166664: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166668: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x8016666C: addu        $v0, $s0, $t1
    ctx->r2 = ADD32(ctx->r16, ctx->r9);
    // 0x80166670: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x80166674: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x80166678: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8016667C: sll         $t5, $t3, 2
    ctx->r13 = S32(ctx->r11 << 2);
    // 0x80166680: addiu       $t2, $t8, 0x1
    ctx->r10 = ADD32(ctx->r24, 0X1);
    // 0x80166684: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x80166688: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x8016668C: lw          $t9, 0x212C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X212C);
    // 0x80166690: jalr        $t9
    // 0x80166694: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_22;
    // 0x80166694: nop

    after_22:
    // 0x80166698: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016669C:
    // 0x8016669C: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x801666A0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801666A4: jr          $ra
    // 0x801666A8: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x801666A8: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_801666AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801666AC: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801666B0: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x801666B4: lh          $t6, 0x2874($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X2874);
    // 0x801666B8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801666BC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801666C0: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801666C4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801666C8: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x801666CC: bne         $a1, $t6, L_8016672C
    if (ctx->r5 != ctx->r14) {
        // 0x801666D0: lw          $v1, 0x24($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X24);
            goto L_8016672C;
    }
    // 0x801666D0: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x801666D4: lh          $t7, 0x28D0($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X28D0);
    // 0x801666D8: bnel        $a1, $t7, L_80166730
    if (ctx->r5 != ctx->r15) {
        // 0x801666DC: lh          $t2, 0x0($v1)
        ctx->r10 = MEM_H(ctx->r3, 0X0);
            goto L_80166730;
    }
    goto skip_0;
    // 0x801666DC: lh          $t2, 0x0($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X0);
    skip_0:
    // 0x801666E0: lhu         $t8, 0x18($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X18);
    // 0x801666E4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801666E8: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x801666EC: bne         $t8, $zero, L_8016672C
    if (ctx->r24 != 0) {
        // 0x801666F0: addiu       $a1, $zero, 0x43
        ctx->r5 = ADD32(0, 0X43);
            goto L_8016672C;
    }
    // 0x801666F0: addiu       $a1, $zero, 0x43
    ctx->r5 = ADD32(0, 0X43);
    // 0x801666F4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801666F8: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x801666FC: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x80166700: jalr        $t9
    // 0x80166704: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80166704: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x80166708: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x8016670C: bne         $v0, $zero, L_8016672C
    if (ctx->r2 != 0) {
        // 0x80166710: lw          $a2, 0x28($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X28);
            goto L_8016672C;
    }
    // 0x80166710: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80166714: sh          $zero, 0x56($a2)
    MEM_H(0X56, ctx->r6) = 0;
    // 0x80166718: lh          $t0, 0x0($v1)
    ctx->r8 = MEM_H(ctx->r3, 0X0);
    // 0x8016671C: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x80166720: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x80166724: b           L_8016678C
    // 0x80166728: sh          $t1, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r9;
        goto L_8016678C;
    // 0x80166728: sh          $t1, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r9;
L_8016672C:
    // 0x8016672C: lh          $t2, 0x0($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X0);
L_80166730:
    // 0x80166730: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166734: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80166738: andi        $t3, $t2, 0x7FFF
    ctx->r11 = ctx->r10 & 0X7FFF;
    // 0x8016673C: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
    // 0x80166740: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x80166744: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80166748: jalr        $t9
    // 0x8016674C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8016674C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
    // 0x80166750: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x80166754: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80166758: lh          $t4, 0xE($a0)
    ctx->r12 = MEM_H(ctx->r4, 0XE);
    // 0x8016675C: sll         $t5, $t4, 1
    ctx->r13 = S32(ctx->r12 << 1);
    // 0x80166760: addu        $v0, $a0, $t5
    ctx->r2 = ADD32(ctx->r4, ctx->r13);
    // 0x80166764: lbu         $t8, 0x9($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X9);
    // 0x80166768: lbu         $t6, 0x8($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X8);
    // 0x8016676C: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80166770: sll         $t0, $t8, 2
    ctx->r8 = S32(ctx->r24 << 2);
    // 0x80166774: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80166778: sb          $t7, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r15;
    // 0x8016677C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80166780: lw          $t9, 0x212C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X212C);
    // 0x80166784: jalr        $t9
    // 0x80166788: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80166788: nop

    after_2:
L_8016678C:
    // 0x8016678C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80166790: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80166794: jr          $ra
    // 0x80166798: nop

    return;
    // 0x80166798: nop

;}
RECOMP_FUNC void func_8016679C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016679C: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801667A0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801667A4: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801667A8: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801667AC: lw          $a3, 0x70($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X70);
    // 0x801667B0: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x801667B4: lw          $s1, 0x24($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X24);
    // 0x801667B8: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x801667BC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801667C0: sw          $a3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r7;
    // 0x801667C4: jalr        $t9
    // 0x801667C8: addiu       $a1, $a3, 0x4
    ctx->r5 = ADD32(ctx->r7, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801667C8: addiu       $a1, $a3, 0x4
    ctx->r5 = ADD32(ctx->r7, 0X4);
    after_0:
    // 0x801667CC: beq         $v0, $zero, L_8016682C
    if (ctx->r2 == 0) {
        // 0x801667D0: lw          $a3, 0x38($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X38);
            goto L_8016682C;
    }
    // 0x801667D0: lw          $a3, 0x38($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X38);
    // 0x801667D4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801667D8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801667DC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801667E0: jalr        $t9
    // 0x801667E4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801667E4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_1:
    // 0x801667E8: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x801667EC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801667F0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801667F4: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x801667F8: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x801667FC: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80166800: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x80166804: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80166808: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x8016680C: addiu       $t1, $t8, 0x1
    ctx->r9 = ADD32(ctx->r24, 0X1);
    // 0x80166810: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80166814: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80166818: lw          $t9, 0x212C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X212C);
    // 0x8016681C: jalr        $t9
    // 0x80166820: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80166820: nop

    after_2:
    // 0x80166824: b           L_80166978
    // 0x80166828: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80166978;
    // 0x80166828: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016682C:
    // 0x8016682C: lw          $t4, 0x64($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X64);
    // 0x80166830: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80166834: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80166838: bne         $t4, $at, L_801668F4
    if (ctx->r12 != ctx->r1) {
        // 0x8016683C: addiu       $t0, $t0, -0x7D40
        ctx->r8 = ADD32(ctx->r8, -0X7D40);
            goto L_801668F4;
    }
    // 0x8016683C: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80166840: sw          $zero, 0x64($s0)
    MEM_W(0X64, ctx->r16) = 0;
    // 0x80166844: lh          $t5, 0x2874($t0)
    ctx->r13 = MEM_H(ctx->r8, 0X2874);
    // 0x80166848: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x8016684C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166850: bne         $v0, $t5, L_801668D8
    if (ctx->r2 != ctx->r13) {
        // 0x80166854: addiu       $a1, $s0, 0xE
        ctx->r5 = ADD32(ctx->r16, 0XE);
            goto L_801668D8;
    }
    // 0x80166854: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80166858: lh          $t6, 0x28D0($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X28D0);
    // 0x8016685C: bnel        $v0, $t6, L_801668DC
    if (ctx->r2 != ctx->r14) {
        // 0x80166860: lw          $t8, 0x50($s0)
        ctx->r24 = MEM_W(ctx->r16, 0X50);
            goto L_801668DC;
    }
    goto skip_0;
    // 0x80166860: lw          $t8, 0x50($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X50);
    skip_0:
    // 0x80166864: lhu         $t7, 0x14($a3)
    ctx->r15 = MEM_HU(ctx->r7, 0X14);
    // 0x80166868: addiu       $v0, $s0, 0x34
    ctx->r2 = ADD32(ctx->r16, 0X34);
    // 0x8016686C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166870: sh          $t7, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r15;
    // 0x80166874: lw          $t8, 0x4($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X4);
    // 0x80166878: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x8016687C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80166880: lw          $t1, 0x4($t8)
    ctx->r9 = MEM_W(ctx->r24, 0X4);
    // 0x80166884: sw          $t1, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r9;
    // 0x80166888: lw          $t2, 0x0($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X0);
    // 0x8016688C: sw          $zero, 0x14($v0)
    MEM_W(0X14, ctx->r2) = 0;
    // 0x80166890: sw          $zero, 0x18($v0)
    MEM_W(0X18, ctx->r2) = 0;
    // 0x80166894: sw          $t2, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r10;
    // 0x80166898: lh          $t3, 0x5C($s1)
    ctx->r11 = MEM_H(ctx->r17, 0X5C);
    // 0x8016689C: or          $v1, $t1, $zero
    ctx->r3 = ctx->r9 | 0;
    // 0x801668A0: sw          $t3, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->r11;
    // 0x801668A4: lw          $t4, 0x0($t1)
    ctx->r12 = MEM_W(ctx->r9, 0X0);
    // 0x801668A8: or          $t9, $t3, $zero
    ctx->r25 = ctx->r11 | 0;
    // 0x801668AC: bgez        $t4, L_801668BC
    if (SIGNED(ctx->r12) >= 0) {
        // 0x801668B0: sra         $t5, $t4, 8
        ctx->r13 = S32(SIGNED(ctx->r12) >> 8);
            goto L_801668BC;
    }
    // 0x801668B0: sra         $t5, $t4, 8
    ctx->r13 = S32(SIGNED(ctx->r12) >> 8);
    // 0x801668B4: addiu       $at, $t4, 0xFF
    ctx->r1 = ADD32(ctx->r12, 0XFF);
    // 0x801668B8: sra         $t5, $at, 8
    ctx->r13 = S32(SIGNED(ctx->r1) >> 8);
L_801668BC:
    // 0x801668BC: addu        $t7, $t9, $t5
    ctx->r15 = ADD32(ctx->r25, ctx->r13);
    // 0x801668C0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801668C4: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801668C8: jalr        $t9
    // 0x801668CC: sh          $t7, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801668CC: sh          $t7, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r15;
    after_3:
    // 0x801668D0: b           L_801668F4
    // 0x801668D4: nop

        goto L_801668F4;
    // 0x801668D4: nop

L_801668D8:
    // 0x801668D8: lw          $t8, 0x50($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X50);
L_801668DC:
    // 0x801668DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801668E0: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801668E4: addiu       $t2, $t8, 0x1555
    ctx->r10 = ADD32(ctx->r24, 0X1555);
    // 0x801668E8: sh          $t2, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r10;
    // 0x801668EC: jalr        $t9
    // 0x801668F0: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801668F0: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_4:
L_801668F4:
    // 0x801668F4: lui         $t3, 0x801D
    ctx->r11 = S32(0X801D << 16);
    // 0x801668F8: lw          $t3, -0x5234($t3)
    ctx->r11 = MEM_W(ctx->r11, -0X5234);
    // 0x801668FC: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x80166900: addiu       $v0, $s0, 0x34
    ctx->r2 = ADD32(ctx->r16, 0X34);
    // 0x80166904: sw          $t3, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->r11;
    // 0x80166908: lw          $t4, -0x7CC4($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X7CC4);
    // 0x8016690C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80166910: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80166914: sw          $t4, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r12;
    // 0x80166918: sh          $zero, 0x22($v0)
    MEM_H(0X22, ctx->r2) = 0;
    // 0x8016691C: lh          $a1, 0xE($s0)
    ctx->r5 = MEM_H(ctx->r16, 0XE);
    // 0x80166920: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x80166924: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80166928: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x8016692C: sll         $t5, $a1, 1
    ctx->r13 = S32(ctx->r5 << 1);
    // 0x80166930: addu        $v1, $s0, $t5
    ctx->r3 = ADD32(ctx->r16, ctx->r13);
    // 0x80166934: sh          $a1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r5;
    // 0x80166938: lbu         $t6, 0x8($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X8);
    // 0x8016693C: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80166940: sb          $t7, 0x8($v1)
    MEM_B(0X8, ctx->r3) = ctx->r15;
    // 0x80166944: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x80166948: lbu         $t8, 0x9($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0X9);
    // 0x8016694C: sll         $t1, $t8, 2
    ctx->r9 = S32(ctx->r24 << 2);
    // 0x80166950: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x80166954: lw          $t9, 0x213C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X213C);
    // 0x80166958: jalr        $t9
    // 0x8016695C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x8016695C: nop

    after_5:
    // 0x80166960: lh          $t2, 0xE($s0)
    ctx->r10 = MEM_H(ctx->r16, 0XE);
    // 0x80166964: lw          $v0, 0x24($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X24);
    // 0x80166968: addiu       $t3, $t2, -0x1
    ctx->r11 = ADD32(ctx->r10, -0X1);
    // 0x8016696C: sh          $t3, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r11;
    // 0x80166970: sh          $zero, 0x20($v0)
    MEM_H(0X20, ctx->r2) = 0;
    // 0x80166974: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80166978:
    // 0x80166978: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016697C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80166980: jr          $ra
    // 0x80166984: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x80166984: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_80166988(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166988: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8016698C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80166990: lw          $t6, 0x64($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X64);
    // 0x80166994: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80166998: addiu       $at, $zero, 0x6
    ctx->r1 = ADD32(0, 0X6);
    // 0x8016699C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801669A0: beq         $t6, $at, L_801669B0
    if (ctx->r14 == ctx->r1) {
        // 0x801669A4: sh          $v0, 0x56($a0)
        MEM_H(0X56, ctx->r4) = ctx->r2;
            goto L_801669B0;
    }
    // 0x801669A4: sh          $v0, 0x56($a0)
    MEM_H(0X56, ctx->r4) = ctx->r2;
    // 0x801669A8: lh          $t7, 0x54($a0)
    ctx->r15 = MEM_H(ctx->r4, 0X54);
    // 0x801669AC: bne         $v0, $t7, L_801669C8
    if (ctx->r2 != ctx->r15) {
        // 0x801669B0: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_801669C8;
    }
L_801669B0:
    // 0x801669B0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801669B4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801669B8: sw          $zero, 0x64($a2)
    MEM_W(0X64, ctx->r6) = 0;
    // 0x801669BC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801669C0: jalr        $t9
    // 0x801669C4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801669C4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_801669C8:
    // 0x801669C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801669CC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801669D0: jr          $ra
    // 0x801669D4: nop

    return;
    // 0x801669D4: nop

;}
RECOMP_FUNC void func_801669D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801669D8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801669DC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801669E0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801669E4: lw          $t7, 0x4C($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X4C);
    // 0x801669E8: lw          $t6, 0x48($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X48);
    // 0x801669EC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801669F0: lw          $a2, 0x24($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X24);
    // 0x801669F4: slt         $at, $t6, $t7
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x801669F8: beq         $at, $zero, L_80166A44
    if (ctx->r1 == 0) {
        // 0x801669FC: addiu       $v1, $s0, 0x34
        ctx->r3 = ADD32(ctx->r16, 0X34);
            goto L_80166A44;
    }
    // 0x801669FC: addiu       $v1, $s0, 0x34
    ctx->r3 = ADD32(ctx->r16, 0X34);
    // 0x80166A00: addiu       $v1, $a0, 0x34
    ctx->r3 = ADD32(ctx->r4, 0X34);
    // 0x80166A04: lw          $t0, 0xC($v1)
    ctx->r8 = MEM_W(ctx->r3, 0XC);
    // 0x80166A08: lw          $t1, 0x10($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X10);
    // 0x80166A0C: lw          $t8, 0x14($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X14);
    // 0x80166A10: lw          $t3, 0x1C($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X1C);
    // 0x80166A14: addu        $t2, $t0, $t1
    ctx->r10 = ADD32(ctx->r8, ctx->r9);
    // 0x80166A18: or          $t4, $t2, $zero
    ctx->r12 = ctx->r10 | 0;
    // 0x80166A1C: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x80166A20: sw          $t9, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->r25;
    // 0x80166A24: sw          $t2, 0xC($v1)
    MEM_W(0XC, ctx->r3) = ctx->r10;
    // 0x80166A28: bgez        $t4, L_80166A38
    if (SIGNED(ctx->r12) >= 0) {
        // 0x80166A2C: sra         $t5, $t4, 8
        ctx->r13 = S32(SIGNED(ctx->r12) >> 8);
            goto L_80166A38;
    }
    // 0x80166A2C: sra         $t5, $t4, 8
    ctx->r13 = S32(SIGNED(ctx->r12) >> 8);
    // 0x80166A30: addiu       $at, $t4, 0xFF
    ctx->r1 = ADD32(ctx->r12, 0XFF);
    // 0x80166A34: sra         $t5, $at, 8
    ctx->r13 = S32(SIGNED(ctx->r1) >> 8);
L_80166A38:
    // 0x80166A38: addu        $t7, $t3, $t5
    ctx->r15 = ADD32(ctx->r11, ctx->r13);
    // 0x80166A3C: b           L_80166B88
    // 0x80166A40: sh          $t7, 0x5C($a2)
    MEM_H(0X5C, ctx->r6) = ctx->r15;
        goto L_80166B88;
    // 0x80166A40: sh          $t7, 0x5C($a2)
    MEM_H(0X5C, ctx->r6) = ctx->r15;
L_80166A44:
    // 0x80166A44: lw          $t8, 0x8($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X8);
    // 0x80166A48: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80166A4C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80166A50: lh          $t9, 0x6($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X6);
    // 0x80166A54: bnel        $t9, $at, L_80166AA0
    if (ctx->r25 != ctx->r1) {
        // 0x80166A58: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80166AA0;
    }
    goto skip_0;
    // 0x80166A58: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    skip_0:
    // 0x80166A5C: lw          $v0, -0x5178($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X5178);
    // 0x80166A60: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    // 0x80166A64: beq         $v0, $at, L_80166A9C
    if (ctx->r2 == ctx->r1) {
        // 0x80166A68: addiu       $at, $zero, 0x19
        ctx->r1 = ADD32(0, 0X19);
            goto L_80166A9C;
    }
    // 0x80166A68: addiu       $at, $zero, 0x19
    ctx->r1 = ADD32(0, 0X19);
    // 0x80166A6C: beq         $v0, $at, L_80166A9C
    if (ctx->r2 == ctx->r1) {
        // 0x80166A70: addiu       $at, $zero, 0x36
        ctx->r1 = ADD32(0, 0X36);
            goto L_80166A9C;
    }
    // 0x80166A70: addiu       $at, $zero, 0x36
    ctx->r1 = ADD32(0, 0X36);
    // 0x80166A74: beq         $v0, $at, L_80166A9C
    if (ctx->r2 == ctx->r1) {
        // 0x80166A78: addiu       $at, $zero, 0x37
        ctx->r1 = ADD32(0, 0X37);
            goto L_80166A9C;
    }
    // 0x80166A78: addiu       $at, $zero, 0x37
    ctx->r1 = ADD32(0, 0X37);
    // 0x80166A7C: beq         $v0, $at, L_80166A9C
    if (ctx->r2 == ctx->r1) {
        // 0x80166A80: addiu       $a0, $zero, 0x118
        ctx->r4 = ADD32(0, 0X118);
            goto L_80166A9C;
    }
    // 0x80166A80: addiu       $a0, $zero, 0x118
    ctx->r4 = ADD32(0, 0X118);
    // 0x80166A84: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80166A88: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x80166A8C: addiu       $a1, $a2, 0x50
    ctx->r5 = ADD32(ctx->r6, 0X50);
    // 0x80166A90: jalr        $t9
    // 0x80166A94: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80166A94: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x80166A98: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_80166A9C:
    // 0x80166A9C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
L_80166AA0:
    // 0x80166AA0: jal         0x80166E7C
    // 0x80166AA4: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    func_80166E7C(rdram, ctx);
        goto after_1;
    // 0x80166AA4: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_1:
    // 0x80166AA8: beq         $v0, $zero, L_80166B88
    if (ctx->r2 == 0) {
        // 0x80166AAC: lw          $v1, 0x24($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X24);
            goto L_80166B88;
    }
    // 0x80166AAC: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x80166AB0: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80166AB4: lh          $t0, -0x54CC($t0)
    ctx->r8 = MEM_H(ctx->r8, -0X54CC);
    // 0x80166AB8: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80166ABC: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x80166AC0: bnel        $v0, $t0, L_80166AFC
    if (ctx->r2 != ctx->r8) {
        // 0x80166AC4: lw          $v0, 0x4($v1)
        ctx->r2 = MEM_W(ctx->r3, 0X4);
            goto L_80166AFC;
    }
    goto skip_1;
    // 0x80166AC4: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    skip_1:
    // 0x80166AC8: lh          $t1, -0x5470($t1)
    ctx->r9 = MEM_H(ctx->r9, -0X5470);
    // 0x80166ACC: addiu       $t2, $zero, 0x96
    ctx->r10 = ADD32(0, 0X96);
    // 0x80166AD0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166AD4: bne         $v0, $t1, L_80166AF8
    if (ctx->r2 != ctx->r9) {
        // 0x80166AD8: addiu       $a1, $s0, 0xE
        ctx->r5 = ADD32(ctx->r16, 0XE);
            goto L_80166AF8;
    }
    // 0x80166AD8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80166ADC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166AE0: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80166AE4: sh          $t2, 0x24($v1)
    MEM_H(0X24, ctx->r3) = ctx->r10;
    // 0x80166AE8: jalr        $t9
    // 0x80166AEC: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80166AEC: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    after_2:
    // 0x80166AF0: b           L_80166B8C
    // 0x80166AF4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80166B8C;
    // 0x80166AF4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80166AF8:
    // 0x80166AF8: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
L_80166AFC:
    // 0x80166AFC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80166B00: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166B04: lhu         $a2, 0x8($v0)
    ctx->r6 = MEM_HU(ctx->r2, 0X8);
    // 0x80166B08: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x80166B0C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80166B10: beq         $a2, $zero, L_80166B40
    if (ctx->r6 == 0) {
        // 0x80166B14: or          $a1, $a2, $zero
        ctx->r5 = ctx->r6 | 0;
            goto L_80166B40;
    }
    // 0x80166B14: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x80166B18: jalr        $t9
    // 0x80166B1C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80166B1C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_3:
    // 0x80166B20: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166B24: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80166B28: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166B2C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80166B30: jalr        $t9
    // 0x80166B34: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80166B34: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_4:
    // 0x80166B38: b           L_80166B68
    // 0x80166B3C: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
        goto L_80166B68;
    // 0x80166B3C: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_80166B40:
    // 0x80166B40: lw          $a0, 0xC($v0)
    ctx->r4 = MEM_W(ctx->r2, 0XC);
    // 0x80166B44: jal         0x801476FC
    // 0x80166B48: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    requestBit_set(rdram, ctx);
        goto after_5;
    // 0x80166B48: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_5:
    // 0x80166B4C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166B50: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80166B54: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80166B58: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80166B5C: jalr        $t9
    // 0x80166B60: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x80166B60: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    after_6:
    // 0x80166B64: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_80166B68:
    // 0x80166B68: lw          $t4, 0x4($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X4);
    // 0x80166B6C: lw          $t3, 0x10($t4)
    ctx->r11 = MEM_W(ctx->r12, 0X10);
    // 0x80166B70: beql        $t3, $zero, L_80166B8C
    if (ctx->r11 == 0) {
        // 0x80166B74: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80166B8C;
    }
    goto skip_2;
    // 0x80166B74: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x80166B78: lw          $a0, 0x1C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X1C);
    // 0x80166B7C: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80166B80: jalr        $t9
    // 0x80166B84: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80166B84: nop

    after_7:
L_80166B88:
    // 0x80166B88: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80166B8C:
    // 0x80166B8C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80166B90: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80166B94: jr          $ra
    // 0x80166B98: nop

    return;
    // 0x80166B98: nop

;}
RECOMP_FUNC void func_80166B9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166B9C: jr          $ra
    // 0x80166BA0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x80166BA0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_80166BA4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166BA4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80166BA8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80166BAC: lw          $t6, 0x38($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X38);
    // 0x80166BB0: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80166BB4: lw          $a0, 0xC($t6)
    ctx->r4 = MEM_W(ctx->r14, 0XC);
    // 0x80166BB8: jal         0x80147718
    // 0x80166BBC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    requestBit_test(rdram, ctx);
        goto after_0;
    // 0x80166BBC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x80166BC0: bne         $v0, $zero, L_80166BDC
    if (ctx->r2 != 0) {
        // 0x80166BC4: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_80166BDC;
    }
    // 0x80166BC4: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80166BC8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166BCC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80166BD0: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80166BD4: jalr        $t9
    // 0x80166BD8: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80166BD8: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_80166BDC:
    // 0x80166BDC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80166BE0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80166BE4: jr          $ra
    // 0x80166BE8: nop

    return;
    // 0x80166BE8: nop

;}
RECOMP_FUNC void func_80166BEC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166BEC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80166BF0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80166BF4: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x80166BF8: lw          $a1, 0x70($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X70);
    // 0x80166BFC: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80166C00: lh          $t6, 0x5C($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X5C);
    // 0x80166C04: addiu       $t7, $t6, -0x111
    ctx->r15 = ADD32(ctx->r14, -0X111);
    // 0x80166C08: sh          $t7, 0x5C($v1)
    MEM_H(0X5C, ctx->r3) = ctx->r15;
    // 0x80166C0C: lw          $t9, 0x50($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X50);
    // 0x80166C10: lh          $t8, 0x5C($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X5C);
    // 0x80166C14: addiu       $t0, $t9, -0x1555
    ctx->r8 = ADD32(ctx->r25, -0X1555);
    // 0x80166C18: slt         $at, $t0, $t8
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r24) ? 1 : 0;
    // 0x80166C1C: bnel        $at, $zero, L_80166D44
    if (ctx->r1 != 0) {
        // 0x80166C20: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80166D44;
    }
    goto skip_0;
    // 0x80166C20: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80166C24: lhu         $t1, 0x14($a1)
    ctx->r9 = MEM_HU(ctx->r5, 0X14);
    // 0x80166C28: addiu       $v0, $a0, 0x34
    ctx->r2 = ADD32(ctx->r4, 0X34);
    // 0x80166C2C: sh          $t1, 0x5C($v1)
    MEM_H(0X5C, ctx->r3) = ctx->r9;
    // 0x80166C30: lw          $t2, 0x4($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X4);
    // 0x80166C34: lw          $t3, 0x4($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X4);
    // 0x80166C38: sw          $t3, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r11;
    // 0x80166C3C: lw          $t4, 0x0($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X0);
    // 0x80166C40: sw          $zero, 0x14($v0)
    MEM_W(0X14, ctx->r2) = 0;
    // 0x80166C44: sw          $zero, 0x18($v0)
    MEM_W(0X18, ctx->r2) = 0;
    // 0x80166C48: sw          $t4, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r12;
    // 0x80166C4C: lh          $t5, 0x5C($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X5C);
    // 0x80166C50: or          $a0, $t3, $zero
    ctx->r4 = ctx->r11 | 0;
    // 0x80166C54: sw          $t5, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->r13;
    // 0x80166C58: lw          $t7, 0x0($t3)
    ctx->r15 = MEM_W(ctx->r11, 0X0);
    // 0x80166C5C: or          $t6, $t5, $zero
    ctx->r14 = ctx->r13 | 0;
    // 0x80166C60: bgez        $t7, L_80166C70
    if (SIGNED(ctx->r15) >= 0) {
        // 0x80166C64: sra         $t9, $t7, 8
        ctx->r25 = S32(SIGNED(ctx->r15) >> 8);
            goto L_80166C70;
    }
    // 0x80166C64: sra         $t9, $t7, 8
    ctx->r25 = S32(SIGNED(ctx->r15) >> 8);
    // 0x80166C68: addiu       $at, $t7, 0xFF
    ctx->r1 = ADD32(ctx->r15, 0XFF);
    // 0x80166C6C: sra         $t9, $at, 8
    ctx->r25 = S32(SIGNED(ctx->r1) >> 8);
L_80166C70:
    // 0x80166C70: addu        $t0, $t6, $t9
    ctx->r8 = ADD32(ctx->r14, ctx->r25);
    // 0x80166C74: sh          $t0, 0x5C($v1)
    MEM_H(0X5C, ctx->r3) = ctx->r8;
    // 0x80166C78: lw          $t1, 0x4($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X4);
    // 0x80166C7C: lw          $t2, 0x10($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X10);
    // 0x80166C80: beq         $t2, $zero, L_80166D28
    if (ctx->r10 == 0) {
        // 0x80166C84: nop
    
            goto L_80166D28;
    }
    // 0x80166C84: nop

    // 0x80166C88: lw          $a1, 0x0($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X0);
    // 0x80166C8C: lwc1        $f4, 0x50($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X50);
    // 0x80166C90: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x80166C94: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x80166C98: swc1        $f4, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f4.u32l;
    // 0x80166C9C: lwc1        $f6, 0x54($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X54);
    // 0x80166CA0: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80166CA4: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80166CA8: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x80166CAC: sub.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d - ctx->f10.d;
    // 0x80166CB0: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x80166CB4: swc1        $f18, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f18.u32l;
    // 0x80166CB8: lwc1        $f4, 0x58($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80166CBC: swc1        $f4, 0xC($a1)
    MEM_W(0XC, ctx->r5) = ctx->f4.u32l;
    // 0x80166CC0: lw          $t3, 0x4($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X4);
    // 0x80166CC4: lw          $t4, 0x10($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X10);
    // 0x80166CC8: lh          $t5, 0x0($t4)
    ctx->r13 = MEM_H(ctx->r12, 0X0);
    // 0x80166CCC: sh          $t5, 0x10($a1)
    MEM_H(0X10, ctx->r5) = ctx->r13;
    // 0x80166CD0: lw          $t7, 0x4($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X4);
    // 0x80166CD4: lw          $t6, 0x10($t7)
    ctx->r14 = MEM_W(ctx->r15, 0X10);
    // 0x80166CD8: lhu         $t9, 0x2($t6)
    ctx->r25 = MEM_HU(ctx->r14, 0X2);
    // 0x80166CDC: sh          $t9, 0x18($a1)
    MEM_H(0X18, ctx->r5) = ctx->r25;
    // 0x80166CE0: lw          $t8, 0x4($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X4);
    // 0x80166CE4: lw          $t0, 0x10($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X10);
    // 0x80166CE8: lhu         $t1, 0x4($t0)
    ctx->r9 = MEM_HU(ctx->r8, 0X4);
    // 0x80166CEC: sh          $t1, 0x14($a1)
    MEM_H(0X14, ctx->r5) = ctx->r9;
    // 0x80166CF0: lw          $t2, 0x4($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X4);
    // 0x80166CF4: lw          $t3, 0x10($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X10);
    // 0x80166CF8: lhu         $t4, 0x6($t3)
    ctx->r12 = MEM_HU(ctx->r11, 0X6);
    // 0x80166CFC: sh          $t4, 0x16($a1)
    MEM_H(0X16, ctx->r5) = ctx->r12;
    // 0x80166D00: jal         0x80188F84
    // 0x80166D04: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    interactable_createWithSettings(rdram, ctx);
        goto after_0;
    // 0x80166D04: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_0:
    // 0x80166D08: bne         $v0, $zero, L_80166D28
    if (ctx->r2 != 0) {
        // 0x80166D0C: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_80166D28;
    }
    // 0x80166D0C: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80166D10: lw          $t9, 0x10($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X10);
    // 0x80166D14: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80166D18: jalr        $t9
    // 0x80166D1C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80166D1C: nop

    after_1:
    // 0x80166D20: b           L_80166D44
    // 0x80166D24: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80166D44;
    // 0x80166D24: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80166D28:
    // 0x80166D28: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166D2C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80166D30: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80166D34: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x80166D38: jalr        $t9
    // 0x80166D3C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80166D3C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_2:
    // 0x80166D40: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80166D44:
    // 0x80166D44: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80166D48: jr          $ra
    // 0x80166D4C: nop

    return;
    // 0x80166D4C: nop

;}
RECOMP_FUNC void func_80166D50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166D50: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80166D54: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80166D58: lh          $t6, 0x58($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X58);
    // 0x80166D5C: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80166D60: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80166D64: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x80166D68: sh          $t7, 0x58($a0)
    MEM_H(0X58, ctx->r4) = ctx->r15;
    // 0x80166D6C: lh          $t8, 0x58($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X58);
    // 0x80166D70: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80166D74: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x80166D78: bgtz        $t8, L_80166D88
    if (SIGNED(ctx->r24) > 0) {
        // 0x80166D7C: addiu       $a0, $a0, 0x8
        ctx->r4 = ADD32(ctx->r4, 0X8);
            goto L_80166D88;
    }
    // 0x80166D7C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80166D80: jalr        $t9
    // 0x80166D84: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80166D84: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_0:
L_80166D88:
    // 0x80166D88: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80166D8C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80166D90: jr          $ra
    // 0x80166D94: nop

    return;
    // 0x80166D94: nop

;}
RECOMP_FUNC void func_80166D98(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166D98: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80166D9C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80166DA0: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80166DA4: jalr        $t9
    // 0x80166DA8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80166DA8: nop

    after_0:
    // 0x80166DAC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80166DB0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80166DB4: jr          $ra
    // 0x80166DB8: nop

    return;
    // 0x80166DB8: nop

;}
RECOMP_FUNC void func_80166DBC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166DBC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80166DC0: lh          $v0, -0x5470($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X5470);
    // 0x80166DC4: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x80166DC8: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80166DCC: beql        $v0, $zero, L_80166DF8
    if (ctx->r2 == 0) {
        // 0x80166DD0: lhu         $t6, 0x18($v1)
        ctx->r14 = MEM_HU(ctx->r3, 0X18);
            goto L_80166DF8;
    }
    goto skip_0;
    // 0x80166DD0: lhu         $t6, 0x18($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X18);
    skip_0:
    // 0x80166DD4: beq         $v0, $at, L_80166E14
    if (ctx->r2 == ctx->r1) {
        // 0x80166DD8: addiu       $at, $zero, 0x10
        ctx->r1 = ADD32(0, 0X10);
            goto L_80166E14;
    }
    // 0x80166DD8: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x80166DDC: beq         $v0, $at, L_80166E34
    if (ctx->r2 == ctx->r1) {
        // 0x80166DE0: addiu       $at, $zero, 0x29
        ctx->r1 = ADD32(0, 0X29);
            goto L_80166E34;
    }
    // 0x80166DE0: addiu       $at, $zero, 0x29
    ctx->r1 = ADD32(0, 0X29);
    // 0x80166DE4: beql        $v0, $at, L_80166E58
    if (ctx->r2 == ctx->r1) {
        // 0x80166DE8: lhu         $t5, 0x18($v1)
        ctx->r13 = MEM_HU(ctx->r3, 0X18);
            goto L_80166E58;
    }
    goto skip_1;
    // 0x80166DE8: lhu         $t5, 0x18($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X18);
    skip_1:
    // 0x80166DEC: b           L_80166E74
    // 0x80166DF0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80166E74;
    // 0x80166DF0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80166DF4: lhu         $t6, 0x18($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X18);
L_80166DF8:
    // 0x80166DF8: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80166DFC: addiu       $t8, $t8, 0x2050
    ctx->r24 = ADD32(ctx->r24, 0X2050);
    // 0x80166E00: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80166E04: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80166E08: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80166E0C: jr          $ra
    // 0x80166E10: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    return;
    // 0x80166E10: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
L_80166E14:
    // 0x80166E14: lhu         $t9, 0x18($v1)
    ctx->r25 = MEM_HU(ctx->r3, 0X18);
    // 0x80166E18: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80166E1C: addiu       $t1, $t1, 0x20B4
    ctx->r9 = ADD32(ctx->r9, 0X20B4);
    // 0x80166E20: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80166E24: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80166E28: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x80166E2C: jr          $ra
    // 0x80166E30: addu        $v0, $t0, $t1
    ctx->r2 = ADD32(ctx->r8, ctx->r9);
    return;
    // 0x80166E30: addu        $v0, $t0, $t1
    ctx->r2 = ADD32(ctx->r8, ctx->r9);
L_80166E34:
    // 0x80166E34: lhu         $t2, 0x18($v1)
    ctx->r10 = MEM_HU(ctx->r3, 0X18);
    // 0x80166E38: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80166E3C: addiu       $t4, $t4, 0x20DC
    ctx->r12 = ADD32(ctx->r12, 0X20DC);
    // 0x80166E40: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80166E44: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x80166E48: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x80166E4C: jr          $ra
    // 0x80166E50: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    return;
    // 0x80166E50: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    // 0x80166E54: lhu         $t5, 0x18($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X18);
L_80166E58:
    // 0x80166E58: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80166E5C: addiu       $t7, $t7, 0x2104
    ctx->r15 = ADD32(ctx->r15, 0X2104);
    // 0x80166E60: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x80166E64: addu        $t6, $t6, $t5
    ctx->r14 = ADD32(ctx->r14, ctx->r13);
    // 0x80166E68: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80166E6C: jr          $ra
    // 0x80166E70: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
    return;
    // 0x80166E70: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
L_80166E74:
    // 0x80166E74: jr          $ra
    // 0x80166E78: nop

    return;
    // 0x80166E78: nop

;}
RECOMP_FUNC void func_80166E7C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166E7C: lw          $t6, 0x3C($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X3C);
    // 0x80166E80: lw          $t9, 0x40($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X40);
    // 0x80166E84: lw          $t2, 0x48($a0)
    ctx->r10 = MEM_W(ctx->r4, 0X48);
    // 0x80166E88: addiu       $t7, $t6, 0x8
    ctx->r15 = ADD32(ctx->r14, 0X8);
    // 0x80166E8C: sw          $t7, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r15;
    // 0x80166E90: lh          $t1, 0x4($t7)
    ctx->r9 = MEM_H(ctx->r15, 0X4);
    // 0x80166E94: lw          $t8, 0x0($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X0);
    // 0x80166E98: or          $v0, $t7, $zero
    ctx->r2 = ctx->r15 | 0;
    // 0x80166E9C: subu        $t3, $t1, $t2
    ctx->r11 = SUB32(ctx->r9, ctx->r10);
    // 0x80166EA0: subu        $t0, $t8, $t9
    ctx->r8 = SUB32(ctx->r24, ctx->r25);
    // 0x80166EA4: div         $zero, $t0, $t3
    lo = S32(S64(S32(ctx->r8)) / S64(S32(ctx->r11))); hi = S32(S64(S32(ctx->r8)) % S64(S32(ctx->r11)));
    // 0x80166EA8: mflo        $t4
    ctx->r12 = lo;
    // 0x80166EAC: sw          $t4, 0x44($a0)
    MEM_W(0X44, ctx->r4) = ctx->r12;
    // 0x80166EB0: lh          $t5, 0x4($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X4);
    // 0x80166EB4: bne         $t3, $zero, L_80166EC0
    if (ctx->r11 != 0) {
        // 0x80166EB8: nop
    
            goto L_80166EC0;
    }
    // 0x80166EB8: nop

    // 0x80166EBC: break       7
    do_break(2148953788);
L_80166EC0:
    // 0x80166EC0: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80166EC4: bne         $t3, $at, L_80166ED8
    if (ctx->r11 != ctx->r1) {
        // 0x80166EC8: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80166ED8;
    }
    // 0x80166EC8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80166ECC: bne         $t0, $at, L_80166ED8
    if (ctx->r8 != ctx->r1) {
        // 0x80166ED0: nop
    
            goto L_80166ED8;
    }
    // 0x80166ED0: nop

    // 0x80166ED4: break       6
    do_break(2148953812);
L_80166ED8:
    // 0x80166ED8: sw          $t5, 0x4C($a0)
    MEM_W(0X4C, ctx->r4) = ctx->r13;
    // 0x80166EDC: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80166EE0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80166EE4: bne         $t6, $zero, L_80166F04
    if (ctx->r14 != 0) {
        // 0x80166EE8: nop
    
            goto L_80166F04;
    }
    // 0x80166EE8: nop

    // 0x80166EEC: lw          $t7, 0x3C($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X3C);
    // 0x80166EF0: lh          $t8, 0x4($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X4);
    // 0x80166EF4: bne         $t8, $zero, L_80166F04
    if (ctx->r24 != 0) {
        // 0x80166EF8: nop
    
            goto L_80166F04;
    }
    // 0x80166EF8: nop

    // 0x80166EFC: jr          $ra
    // 0x80166F00: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80166F00: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80166F04:
    // 0x80166F04: jr          $ra
    // 0x80166F08: nop

    return;
    // 0x80166F08: nop

;}
RECOMP_FUNC void func_80166F0C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166F0C: lh          $t6, 0x56($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X56);
    // 0x80166F10: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x80166F14: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80166F18: bnel        $t6, $zero, L_80166F2C
    if (ctx->r14 != 0) {
        // 0x80166F1C: sh          $t7, 0x54($a0)
        MEM_H(0X54, ctx->r4) = ctx->r15;
            goto L_80166F2C;
    }
    goto skip_0;
    // 0x80166F1C: sh          $t7, 0x54($a0)
    MEM_H(0X54, ctx->r4) = ctx->r15;
    skip_0:
    // 0x80166F20: jr          $ra
    // 0x80166F24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80166F24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80166F28: sh          $t7, 0x54($a0)
    MEM_H(0X54, ctx->r4) = ctx->r15;
L_80166F2C:
    // 0x80166F2C: jr          $ra
    // 0x80166F30: nop

    return;
    // 0x80166F30: nop

;}
RECOMP_FUNC void func_80166F40(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166F40: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80166F44: addiu       $v0, $v1, 0x24AC
    ctx->r2 = ADD32(ctx->r3, 0X24AC);
    // 0x80166F48: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80166F4C: lhu         $t6, 0x0($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X0);
    // 0x80166F50: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x80166F54: ori         $a2, $zero, 0xFFFF
    ctx->r6 = 0 | 0XFFFF;
    // 0x80166F58: beq         $a2, $t6, L_80166F8C
    if (ctx->r6 == ctx->r14) {
        // 0x80166F5C: or          $v1, $a0, $zero
        ctx->r3 = ctx->r4 | 0;
            goto L_80166F8C;
    }
    // 0x80166F5C: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80166F60: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80166F64: addiu       $t7, $t7, 0x24AC
    ctx->r15 = ADD32(ctx->r15, 0X24AC);
    // 0x80166F68: lhu         $a1, 0x0($t7)
    ctx->r5 = MEM_HU(ctx->r15, 0X0);
L_80166F6C:
    // 0x80166F6C: bnel        $v1, $a1, L_80166F80
    if (ctx->r3 != ctx->r5) {
        // 0x80166F70: lhu         $a1, 0x2($v0)
        ctx->r5 = MEM_HU(ctx->r2, 0X2);
            goto L_80166F80;
    }
    goto skip_0;
    // 0x80166F70: lhu         $a1, 0x2($v0)
    ctx->r5 = MEM_HU(ctx->r2, 0X2);
    skip_0:
    // 0x80166F74: jr          $ra
    // 0x80166F78: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80166F78: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80166F7C: lhu         $a1, 0x2($v0)
    ctx->r5 = MEM_HU(ctx->r2, 0X2);
L_80166F80:
    // 0x80166F80: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80166F84: bne         $a2, $a1, L_80166F6C
    if (ctx->r6 != ctx->r5) {
        // 0x80166F88: nop
    
            goto L_80166F6C;
    }
    // 0x80166F88: nop

L_80166F8C:
    // 0x80166F8C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80166F90: jr          $ra
    // 0x80166F94: nop

    return;
    // 0x80166F94: nop

;}
RECOMP_FUNC void func_80166F98(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80166F98: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80166F9C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80166FA0: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x80166FA4: lw          $s1, -0x1A20($s1)
    ctx->r17 = MEM_W(ctx->r17, -0X1A20);
    // 0x80166FA8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80166FAC: sw          $zero, -0x1130($at)
    MEM_W(-0X1130, ctx->r1) = 0;
    // 0x80166FB0: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80166FB4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80166FB8: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80166FBC: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80166FC0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80166FC4: sw          $zero, -0x112C($at)
    MEM_W(-0X112C, ctx->r1) = 0;
    // 0x80166FC8: beq         $s1, $zero, L_8016705C
    if (ctx->r17 == 0) {
        // 0x80166FCC: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_8016705C;
    }
    // 0x80166FCC: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80166FD0: lui         $s3, 0x801A
    ctx->r19 = S32(0X801A << 16);
    // 0x80166FD4: addiu       $s3, $s3, -0x6C4
    ctx->r19 = ADD32(ctx->r19, -0X6C4);
    // 0x80166FD8: lw          $v0, 0x4($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X4);
L_80166FDC:
    // 0x80166FDC: beql        $v0, $zero, L_80167050
    if (ctx->r2 == 0) {
        // 0x80166FE0: lw          $s1, 0x0($s1)
        ctx->r17 = MEM_W(ctx->r17, 0X0);
            goto L_80167050;
    }
    goto skip_0;
    // 0x80166FE0: lw          $s1, 0x0($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X0);
    skip_0:
    // 0x80166FE4: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
    // 0x80166FE8: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x80166FEC: slti        $at, $v1, 0x282
    ctx->r1 = SIGNED(ctx->r3) < 0X282 ? 1 : 0;
    // 0x80166FF0: bne         $at, $zero, L_80167000
    if (ctx->r1 != 0) {
        // 0x80166FF4: slti        $at, $v1, 0x285
        ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
            goto L_80167000;
    }
    // 0x80166FF4: slti        $at, $v1, 0x285
    ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
    // 0x80166FF8: bnel        $at, $zero, L_80167050
    if (ctx->r1 != 0) {
        // 0x80166FFC: lw          $s1, 0x0($s1)
        ctx->r17 = MEM_W(ctx->r17, 0X0);
            goto L_80167050;
    }
    goto skip_1;
    // 0x80166FFC: lw          $s1, 0x0($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X0);
    skip_1:
L_80167000:
    // 0x80167000: lw          $s0, 0x24($v0)
    ctx->r16 = MEM_W(ctx->r2, 0X24);
    // 0x80167004: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80167008: beq         $s0, $zero, L_8016704C
    if (ctx->r16 == 0) {
        // 0x8016700C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016704C;
    }
    // 0x8016700C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80167010: jal         0x80167150
    // 0x80167014: sw          $s2, 0x0($s3)
    MEM_W(0X0, ctx->r19) = ctx->r18;
    func_80167150(rdram, ctx);
        goto after_0;
    // 0x80167014: sw          $s2, 0x0($s3)
    MEM_W(0X0, ctx->r19) = ctx->r18;
    after_0:
    // 0x80167018: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016701C: beql        $a0, $zero, L_80167050
    if (ctx->r4 == 0) {
        // 0x80167020: lw          $s1, 0x0($s1)
        ctx->r17 = MEM_W(ctx->r17, 0X0);
            goto L_80167050;
    }
    goto skip_2;
    // 0x80167020: lw          $s1, 0x0($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X0);
    skip_2:
    // 0x80167024: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x80167028: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x8016702C: beq         $t7, $zero, L_80167044
    if (ctx->r15 == 0) {
        // 0x80167030: nop
    
            goto L_80167044;
    }
    // 0x80167030: nop

    // 0x80167034: jal         0x80167078
    // 0x80167038: nop

    func_80167078(rdram, ctx);
        goto after_1;
    // 0x80167038: nop

    after_1:
    // 0x8016703C: b           L_80167050
    // 0x80167040: lw          $s1, 0x0($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X0);
        goto L_80167050;
    // 0x80167040: lw          $s1, 0x0($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X0);
L_80167044:
    // 0x80167044: jal         0x801670E4
    // 0x80167048: nop

    func_801670E4(rdram, ctx);
        goto after_2;
    // 0x80167048: nop

    after_2:
L_8016704C:
    // 0x8016704C: lw          $s1, 0x0($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X0);
L_80167050:
    // 0x80167050: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x80167054: bnel        $s1, $zero, L_80166FDC
    if (ctx->r17 != 0) {
        // 0x80167058: lw          $v0, 0x4($s1)
        ctx->r2 = MEM_W(ctx->r17, 0X4);
            goto L_80166FDC;
    }
    goto skip_3;
    // 0x80167058: lw          $v0, 0x4($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X4);
    skip_3:
L_8016705C:
    // 0x8016705C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80167060: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80167064: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80167068: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8016706C: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80167070: jr          $ra
    // 0x80167074: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80167074: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80167078(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_80167078 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_80167078++;
    if (lod_text_guard_depth_80167078 > 128) {
        if (!lod_text_guard_logged_80167078) {
            lod_text_guard_logged_80167078 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_80167078 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_80167078--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x80167078: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016707C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80167080: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80167084: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80167088: beq         $s0, $zero, L_801670D0
    if (ctx->r16 == 0) {
        // 0x8016708C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_801670D0;
    }
L_8016708C:
    // 0x8016708C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80167090: jal         0x80167150
    // 0x80167094: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_80167150(rdram, ctx);
        goto after_0;
    // 0x80167094: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_0:
    // 0x80167098: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016709C: beql        $a0, $zero, L_801670B0
    if (ctx->r4 == 0) {
        // 0x801670A0: lh          $t6, 0x0($s0)
        ctx->r14 = MEM_H(ctx->r16, 0X0);
            goto L_801670B0;
    }
    goto skip_0;
    // 0x801670A0: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    skip_0:
    // 0x801670A4: jal         0x80167078
    // 0x801670A8: nop

    func_80167078(rdram, ctx);
        goto after_1;
    // 0x801670A8: nop

    after_1:
    // 0x801670AC: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
L_801670B0:
    // 0x801670B0: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x801670B4: bnel        $t7, $zero, L_801670D4
    if (ctx->r15 != 0) {
        // 0x801670B8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_801670D4;
    }
    goto skip_1;
    // 0x801670B8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x801670BC: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x801670C0: beql        $v0, $zero, L_801670D4
    if (ctx->r2 == 0) {
        // 0x801670C4: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_801670D4;
    }
    goto skip_2;
    // 0x801670C4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x801670C8: bne         $v0, $zero, L_8016708C
    if (ctx->r2 != 0) {
        // 0x801670CC: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016708C;
    }
    // 0x801670CC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_801670D0:
    // 0x801670D0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801670D4:
    // 0x801670D4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801670D8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801670DC: jr          $ra
    // 0x801670E0: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_80167078--;
    // --- END PATCH ---
#endif
    return;
    // 0x801670E0: nop

;}
RECOMP_FUNC void func_801670E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_801670E4 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_801670E4++;
    if (lod_text_guard_depth_801670E4 > 128) {
        if (!lod_text_guard_logged_801670E4) {
            lod_text_guard_logged_801670E4 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_801670E4 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_801670E4--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x801670E4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801670E8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801670EC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801670F0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801670F4: beq         $s0, $zero, L_8016713C
    if (ctx->r16 == 0) {
        // 0x801670F8: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016713C;
    }
L_801670F8:
    // 0x801670F8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801670FC: jal         0x80167150
    // 0x80167100: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_80167150(rdram, ctx);
        goto after_0;
    // 0x80167100: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_0:
    // 0x80167104: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x80167108: beql        $a0, $zero, L_8016711C
    if (ctx->r4 == 0) {
        // 0x8016710C: lh          $t6, 0x0($s0)
        ctx->r14 = MEM_H(ctx->r16, 0X0);
            goto L_8016711C;
    }
    goto skip_0;
    // 0x8016710C: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    skip_0:
    // 0x80167110: jal         0x801670E4
    // 0x80167114: nop

    func_801670E4(rdram, ctx);
        goto after_1;
    // 0x80167114: nop

    after_1:
    // 0x80167118: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
L_8016711C:
    // 0x8016711C: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x80167120: bnel        $t7, $zero, L_80167140
    if (ctx->r15 != 0) {
        // 0x80167124: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80167140;
    }
    goto skip_1;
    // 0x80167124: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x80167128: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x8016712C: beql        $v0, $zero, L_80167140
    if (ctx->r2 == 0) {
        // 0x80167130: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80167140;
    }
    goto skip_2;
    // 0x80167130: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x80167134: bne         $v0, $zero, L_801670F8
    if (ctx->r2 != 0) {
        // 0x80167138: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_801670F8;
    }
    // 0x80167138: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016713C:
    // 0x8016713C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80167140:
    // 0x80167140: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80167144: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80167148: jr          $ra
    // 0x8016714C: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_801670E4--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016714C: nop

;}
RECOMP_FUNC void func_80167150(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167150: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x80167154: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80167158: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016715C: lw          $v0, 0x74($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X74);
    // 0x80167160: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80167164: beql        $v0, $zero, L_80167330
    if (ctx->r2 == 0) {
        // 0x80167168: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80167330;
    }
    goto skip_0;
    // 0x80167168: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8016716C: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x80167170: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x80167174: beql        $t7, $zero, L_80167330
    if (ctx->r15 == 0) {
        // 0x80167178: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80167330;
    }
    goto skip_1;
    // 0x80167178: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016717C: lhu         $v1, 0x20($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X20);
    // 0x80167180: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80167184: beql        $v1, $zero, L_80167330
    if (ctx->r3 == 0) {
        // 0x80167188: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80167330;
    }
    goto skip_2;
    // 0x80167188: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016718C: lw          $v0, -0x1134($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X1134);
    // 0x80167190: sll         $t1, $v1, 2
    ctx->r9 = S32(ctx->r3 << 2);
    // 0x80167194: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80167198: lw          $t8, 0xC($v0)
    ctx->r24 = MEM_W(ctx->r2, 0XC);
    // 0x8016719C: addu        $t2, $t8, $t1
    ctx->r10 = ADD32(ctx->r24, ctx->r9);
    // 0x801671A0: lhu         $a0, -0x2($t2)
    ctx->r4 = MEM_HU(ctx->r10, -0X2);
    // 0x801671A4: beq         $a0, $at, L_8016732C
    if (ctx->r4 == ctx->r1) {
        // 0x801671A8: or          $t0, $a0, $zero
        ctx->r8 = ctx->r4 | 0;
            goto L_8016732C;
    }
    // 0x801671A8: or          $t0, $a0, $zero
    ctx->r8 = ctx->r4 | 0;
    // 0x801671AC: lw          $t3, 0x4($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X4);
    // 0x801671B0: sll         $t4, $a0, 2
    ctx->r12 = S32(ctx->r4 << 2);
    // 0x801671B4: subu        $t4, $t4, $a0
    ctx->r12 = SUB32(ctx->r12, ctx->r4);
    // 0x801671B8: sll         $t4, $t4, 4
    ctx->r12 = S32(ctx->r12 << 4);
    // 0x801671BC: addu        $t5, $t3, $t4
    ctx->r13 = ADD32(ctx->r11, ctx->r12);
    // 0x801671C0: bne         $a1, $zero, L_801671FC
    if (ctx->r5 != 0) {
        // 0x801671C4: sw          $t5, 0x5C($sp)
        MEM_W(0X5C, ctx->r29) = ctx->r13;
            goto L_801671FC;
    }
    // 0x801671C4: sw          $t5, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r13;
    // 0x801671C8: lwc1        $f4, 0x50($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X50);
    // 0x801671CC: swc1        $f4, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->f4.u32l;
    // 0x801671D0: lwc1        $f6, 0x54($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X54);
    // 0x801671D4: swc1        $f6, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->f6.u32l;
    // 0x801671D8: lwc1        $f8, 0x58($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X58);
    // 0x801671DC: swc1        $f8, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->f8.u32l;
    // 0x801671E0: lh          $t6, 0x5C($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X5C);
    // 0x801671E4: sh          $t6, 0x36($sp)
    MEM_H(0X36, ctx->r29) = ctx->r14;
    // 0x801671E8: lh          $t7, 0x5E($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X5E);
    // 0x801671EC: sh          $t7, 0x38($sp)
    MEM_H(0X38, ctx->r29) = ctx->r15;
    // 0x801671F0: lh          $t9, 0x60($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X60);
    // 0x801671F4: b           L_80167218
    // 0x801671F8: sh          $t9, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = ctx->r25;
        goto L_80167218;
    // 0x801671F8: sh          $t9, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = ctx->r25;
L_801671FC:
    // 0x801671FC: addiu       $a0, $s0, 0x78
    ctx->r4 = ADD32(ctx->r16, 0X78);
    // 0x80167200: addiu       $a1, $sp, 0x6C
    ctx->r5 = ADD32(ctx->r29, 0X6C);
    // 0x80167204: addiu       $a2, $sp, 0x36
    ctx->r6 = ADD32(ctx->r29, 0X36);
    // 0x80167208: addiu       $a3, $sp, 0x60
    ctx->r7 = ADD32(ctx->r29, 0X60);
    // 0x8016720C: jal         0x80167F54
    // 0x80167210: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    func_80167F54(rdram, ctx);
        goto after_0;
    // 0x80167210: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    after_0:
    // 0x80167214: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
L_80167218:
    // 0x80167218: lh          $t8, 0x36($sp)
    ctx->r24 = MEM_H(ctx->r29, 0X36);
    // 0x8016721C: lh          $t1, 0x3A($sp)
    ctx->r9 = MEM_H(ctx->r29, 0X3A);
    // 0x80167220: addiu       $v1, $sp, 0x6C
    ctx->r3 = ADD32(ctx->r29, 0X6C);
    // 0x80167224: bne         $t8, $zero, L_80167230
    if (ctx->r24 != 0) {
        // 0x80167228: addiu       $v0, $sp, 0x30
        ctx->r2 = ADD32(ctx->r29, 0X30);
            goto L_80167230;
    }
    // 0x80167228: addiu       $v0, $sp, 0x30
    ctx->r2 = ADD32(ctx->r29, 0X30);
    // 0x8016722C: beq         $t1, $zero, L_8016732C
    if (ctx->r9 == 0) {
        // 0x80167230: lui         $at, 0x4120
        ctx->r1 = S32(0X4120 << 16);
            goto L_8016732C;
    }
L_80167230:
    // 0x80167230: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x80167234: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80167238: addiu       $a0, $sp, 0x36
    ctx->r4 = ADD32(ctx->r29, 0X36);
    // 0x8016723C: lwc1        $f14, 0x0($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X0);
    // 0x80167240: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80167244: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x80167248: beql        $v0, $a0, L_80167274
    if (ctx->r2 == ctx->r4) {
        // 0x8016724C: trunc.w.s   $f16, $f18
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
            goto L_80167274;
    }
    goto skip_3;
    // 0x8016724C: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    skip_3:
L_80167250:
    // 0x80167250: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x80167254: lwc1        $f14, 0x4($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X4);
    // 0x80167258: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016725C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80167260: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x80167264: mfc1        $t3, $f16
    ctx->r11 = (int32_t)ctx->f16.u32l;
    // 0x80167268: bne         $v0, $a0, L_80167250
    if (ctx->r2 != ctx->r4) {
        // 0x8016726C: sh          $t3, -0x4($v0)
        MEM_H(-0X4, ctx->r2) = ctx->r11;
            goto L_80167250;
    }
    // 0x8016726C: sh          $t3, -0x4($v0)
    MEM_H(-0X4, ctx->r2) = ctx->r11;
    // 0x80167270: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
L_80167274:
    // 0x80167274: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80167278: mfc1        $t3, $f16
    ctx->r11 = (int32_t)ctx->f16.u32l;
    // 0x8016727C: nop

    // 0x80167280: sh          $t3, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r11;
    // 0x80167284: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x80167288: addiu       $s0, $s0, -0x1130
    ctx->r16 = ADD32(ctx->r16, -0X1130);
    // 0x8016728C: lw          $t5, 0x0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X0);
    // 0x80167290: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x80167294: lw          $t7, -0x1124($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X1124);
    // 0x80167298: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x8016729C: subu        $t6, $t6, $t5
    ctx->r14 = SUB32(ctx->r14, ctx->r13);
    // 0x801672A0: sll         $t6, $t6, 4
    ctx->r14 = S32(ctx->r14 << 4);
    // 0x801672A4: addu        $t6, $t6, $t5
    ctx->r14 = ADD32(ctx->r14, ctx->r13);
    // 0x801672A8: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x801672AC: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x801672B0: sh          $t0, 0x54($sp)
    MEM_H(0X54, ctx->r29) = ctx->r8;
    // 0x801672B4: sh          $t4, 0x56($sp)
    MEM_H(0X56, ctx->r29) = ctx->r12;
    // 0x801672B8: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    // 0x801672BC: addiu       $a2, $sp, 0x30
    ctx->r6 = ADD32(ctx->r29, 0X30);
    // 0x801672C0: jal         0x80167340
    // 0x801672C4: addu        $a0, $t6, $t7
    ctx->r4 = ADD32(ctx->r14, ctx->r15);
    func_80167340(rdram, ctx);
        goto after_1;
    // 0x801672C4: addu        $a0, $t6, $t7
    ctx->r4 = ADD32(ctx->r14, ctx->r15);
    after_1:
    // 0x801672C8: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801672CC: lw          $t1, 0x0($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X0);
    // 0x801672D0: addiu       $v1, $v1, -0x1128
    ctx->r3 = ADD32(ctx->r3, -0X1128);
    // 0x801672D4: lw          $t8, 0x0($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X0);
    // 0x801672D8: lui         $t9, 0x801A
    ctx->r25 = S32(0X801A << 16);
    // 0x801672DC: lw          $t9, -0x6C4($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X6C4);
    // 0x801672E0: sll         $t2, $t1, 3
    ctx->r10 = S32(ctx->r9 << 3);
    // 0x801672E4: addu        $t3, $t8, $t2
    ctx->r11 = ADD32(ctx->r24, ctx->r10);
    // 0x801672E8: sw          $t9, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r25;
    // 0x801672EC: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x801672F0: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x801672F4: lw          $t5, -0x1124($t5)
    ctx->r13 = MEM_W(ctx->r13, -0X1124);
    // 0x801672F8: sll         $t4, $v0, 2
    ctx->r12 = S32(ctx->r2 << 2);
    // 0x801672FC: subu        $t4, $t4, $v0
    ctx->r12 = SUB32(ctx->r12, ctx->r2);
    // 0x80167300: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x80167304: sll         $t4, $t4, 4
    ctx->r12 = S32(ctx->r12 << 4);
    // 0x80167308: addu        $t4, $t4, $v0
    ctx->r12 = ADD32(ctx->r12, ctx->r2);
    // 0x8016730C: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x80167310: sll         $t1, $v0, 3
    ctx->r9 = S32(ctx->r2 << 3);
    // 0x80167314: addu        $t6, $t4, $t5
    ctx->r14 = ADD32(ctx->r12, ctx->r13);
    // 0x80167318: addu        $t8, $t7, $t1
    ctx->r24 = ADD32(ctx->r15, ctx->r9);
    // 0x8016731C: sw          $t6, 0x4($t8)
    MEM_W(0X4, ctx->r24) = ctx->r14;
    // 0x80167320: lw          $t2, 0x0($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X0);
    // 0x80167324: addiu       $t9, $t2, 0x1
    ctx->r25 = ADD32(ctx->r10, 0X1);
    // 0x80167328: sw          $t9, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r25;
L_8016732C:
    // 0x8016732C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80167330:
    // 0x80167330: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80167334: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    // 0x80167338: jr          $ra
    // 0x8016733C: nop

    return;
    // 0x8016733C: nop

;}
RECOMP_FUNC void func_80167340(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167340: addiu       $sp, $sp, -0x3F0
    ctx->r29 = ADD32(ctx->r29, -0X3F0);
    // 0x80167344: sw          $s0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r16;
    // 0x80167348: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x8016734C: sw          $a0, 0x3F0($sp)
    MEM_W(0X3F0, ctx->r29) = ctx->r4;
    // 0x80167350: sw          $a2, 0x3F8($sp)
    MEM_W(0X3F8, ctx->r29) = ctx->r6;
    // 0x80167354: sw          $ra, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r31;
    // 0x80167358: sw          $fp, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r30;
    // 0x8016735C: sw          $s7, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r23;
    // 0x80167360: sw          $s6, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r22;
    // 0x80167364: sw          $s5, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r21;
    // 0x80167368: sw          $s4, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r20;
    // 0x8016736C: sw          $s3, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r19;
    // 0x80167370: sw          $s2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r18;
    // 0x80167374: sw          $s1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r17;
    // 0x80167378: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x8016737C: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80167380: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80167384: addiu       $a2, $sp, 0x3A4
    ctx->r6 = ADD32(ctx->r29, 0X3A4);
    // 0x80167388: addiu       $a0, $zero, 0x7FFF
    ctx->r4 = ADD32(0, 0X7FFF);
    // 0x8016738C: addiu       $a1, $zero, -0x7FFF
    ctx->r5 = ADD32(0, -0X7FFF);
    // 0x80167390: addiu       $v0, $sp, 0x374
    ctx->r2 = ADD32(ctx->r29, 0X374);
L_80167394:
    // 0x80167394: sh          $a0, 0x4($v0)
    MEM_H(0X4, ctx->r2) = ctx->r4;
    // 0x80167398: lh          $v1, 0x4($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X4);
    // 0x8016739C: sh          $a1, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r5;
    // 0x801673A0: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x801673A4: sh          $v1, -0xA($v0)
    MEM_H(-0XA, ctx->r2) = ctx->r3;
    // 0x801673A8: sh          $v1, -0xC($v0)
    MEM_H(-0XC, ctx->r2) = ctx->r3;
    // 0x801673AC: lh          $v1, -0x2($v0)
    ctx->r3 = MEM_H(ctx->r2, -0X2);
    // 0x801673B0: sltu        $at, $v0, $a2
    ctx->r1 = ctx->r2 < ctx->r6 ? 1 : 0;
    // 0x801673B4: sh          $v1, -0x4($v0)
    MEM_H(-0X4, ctx->r2) = ctx->r3;
    // 0x801673B8: bne         $at, $zero, L_80167394
    if (ctx->r1 != 0) {
        // 0x801673BC: sh          $v1, -0x6($v0)
        MEM_H(-0X6, ctx->r2) = ctx->r3;
            goto L_80167394;
    }
    // 0x801673BC: sh          $v1, -0x6($v0)
    MEM_H(-0X6, ctx->r2) = ctx->r3;
    // 0x801673C0: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x801673C4: lw          $t6, -0x112C($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X112C);
    // 0x801673C8: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x801673CC: lw          $t8, -0x1120($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X1120);
    // 0x801673D0: lw          $a1, 0x3F8($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3F8);
    // 0x801673D4: sll         $t7, $t6, 6
    ctx->r15 = S32(ctx->r14 << 6);
    // 0x801673D8: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x801673DC: sw          $t9, 0x338($sp)
    MEM_W(0X338, ctx->r29) = ctx->r25;
    // 0x801673E0: sw          $zero, 0x320($sp)
    MEM_W(0X320, ctx->r29) = 0;
    // 0x801673E4: sw          $zero, 0x32C($sp)
    MEM_W(0X32C, ctx->r29) = 0;
    // 0x801673E8: sw          $zero, 0x328($sp)
    MEM_W(0X328, ctx->r29) = 0;
    // 0x801673EC: sw          $zero, 0x324($sp)
    MEM_W(0X324, ctx->r29) = 0;
    // 0x801673F0: addiu       $a0, $sp, 0x350
    ctx->r4 = ADD32(ctx->r29, 0X350);
    // 0x801673F4: jal         0x801735B8
    // 0x801673F8: addiu       $a1, $a1, 0x6
    ctx->r5 = ADD32(ctx->r5, 0X6);
    func_801735B8(rdram, ctx);
        goto after_0;
    // 0x801673F8: addiu       $a1, $a1, 0x6
    ctx->r5 = ADD32(ctx->r5, 0X6);
    after_0:
    // 0x801673FC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80167400: mtc1        $zero, $f24
    ctx->f24.u32l = 0;
    // 0x80167404: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80167408: lwc1        $f22, -0x51B0($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0X51B0);
    // 0x8016740C: or          $s7, $zero, $zero
    ctx->r23 = 0 | 0;
    // 0x80167410: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    // 0x80167414: addiu       $s6, $sp, 0x324
    ctx->r22 = ADD32(ctx->r29, 0X324);
    // 0x80167418: lw          $v1, 0x320($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X320);
L_8016741C:
    // 0x8016741C: lw          $s5, 0x0($a3)
    ctx->r21 = MEM_W(ctx->r7, 0X0);
    // 0x80167420: beql        $s5, $zero, L_80167514
    if (ctx->r21 == 0) {
        // 0x80167424: addiu       $s7, $s7, 0x4
        ctx->r23 = ADD32(ctx->r23, 0X4);
            goto L_80167514;
    }
    goto skip_0;
    // 0x80167424: addiu       $s7, $s7, 0x4
    ctx->r23 = ADD32(ctx->r23, 0X4);
    skip_0:
    // 0x80167428: lh          $t5, 0x1C($s5)
    ctx->r13 = MEM_H(ctx->r21, 0X1C);
    // 0x8016742C: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80167430: lw          $s3, 0x18($s5)
    ctx->r19 = MEM_W(ctx->r21, 0X18);
    // 0x80167434: blez        $t5, L_80167510
    if (SIGNED(ctx->r13) <= 0) {
        // 0x80167438: sll         $t6, $v1, 2
        ctx->r14 = S32(ctx->r3 << 2);
            goto L_80167510;
    }
    // 0x80167438: sll         $t6, $v1, 2
    ctx->r14 = S32(ctx->r3 << 2);
    // 0x8016743C: sll         $t8, $v1, 2
    ctx->r24 = S32(ctx->r3 << 2);
    // 0x80167440: subu        $t8, $t8, $v1
    ctx->r24 = SUB32(ctx->r24, ctx->r3);
    // 0x80167444: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80167448: addiu       $t7, $sp, 0x11C
    ctx->r15 = ADD32(ctx->r29, 0X11C);
    // 0x8016744C: addiu       $t9, $sp, 0x19C
    ctx->r25 = ADD32(ctx->r29, 0X19C);
    // 0x80167450: addiu       $t5, $sp, 0xFC
    ctx->r13 = ADD32(ctx->r29, 0XFC);
    // 0x80167454: addu        $s4, $v1, $t5
    ctx->r20 = ADD32(ctx->r3, ctx->r13);
    // 0x80167458: addu        $s1, $t8, $t9
    ctx->r17 = ADD32(ctx->r24, ctx->r25);
    // 0x8016745C: addu        $s2, $t6, $t7
    ctx->r18 = ADD32(ctx->r14, ctx->r15);
    // 0x80167460: sw          $v1, 0x320($sp)
    MEM_W(0X320, ctx->r29) = ctx->r3;
    // 0x80167464: sw          $a3, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r7;
L_80167468:
    // 0x80167468: sw          $s3, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r19;
    // 0x8016746C: addiu       $a0, $sp, 0x350
    ctx->r4 = ADD32(ctx->r29, 0X350);
    // 0x80167470: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x80167474: jal         0x80173524
    // 0x80167478: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_80173524(rdram, ctx);
        goto after_1;
    // 0x80167478: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_1:
    // 0x8016747C: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80167480: lw          $v1, 0x320($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X320);
    // 0x80167484: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x80167488: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x8016748C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80167490: bc1fl       L_801674A4
    if (!c1cs) {
        // 0x80167494: mov.s       $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
            goto L_801674A4;
    }
    goto skip_1;
    // 0x80167494: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    skip_1:
    // 0x80167498: b           L_801674A4
    // 0x8016749C: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_801674A4;
    // 0x8016749C: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
    // 0x801674A0: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_801674A4:
    // 0x801674A4: c.lt.s      $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f2.fl < ctx->f22.fl;
    // 0x801674A8: nop

    // 0x801674AC: bc1fl       L_801674C0
    if (!c1cs) {
        // 0x801674B0: c.lt.s      $f24, $f0
        CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f24.fl < ctx->f0.fl;
            goto L_801674C0;
    }
    goto skip_2;
    // 0x801674B0: c.lt.s      $f24, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f24.fl < ctx->f0.fl;
    skip_2:
    // 0x801674B4: b           L_801674D4
    // 0x801674B8: addiu       $fp, $zero, 0x1
    ctx->r30 = ADD32(0, 0X1);
        goto L_801674D4;
    // 0x801674B8: addiu       $fp, $zero, 0x1
    ctx->r30 = ADD32(0, 0X1);
    // 0x801674BC: c.lt.s      $f24, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f24.fl < ctx->f0.fl;
L_801674C0:
    // 0x801674C0: addiu       $fp, $zero, 0x2
    ctx->r30 = ADD32(0, 0X2);
    // 0x801674C4: bc1f        L_801674D4
    if (!c1cs) {
        // 0x801674C8: nop
    
            goto L_801674D4;
    }
    // 0x801674C8: nop

    // 0x801674CC: b           L_801674D4
    // 0x801674D0: or          $fp, $zero, $zero
    ctx->r30 = 0 | 0;
        goto L_801674D4;
    // 0x801674D0: or          $fp, $zero, $zero
    ctx->r30 = 0 | 0;
L_801674D4:
    // 0x801674D4: sll         $t6, $fp, 2
    ctx->r14 = S32(ctx->r30 << 2);
    // 0x801674D8: addu        $v0, $s6, $t6
    ctx->r2 = ADD32(ctx->r22, ctx->r14);
    // 0x801674DC: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x801674E0: sb          $fp, 0x0($s4)
    MEM_B(0X0, ctx->r20) = ctx->r30;
    // 0x801674E4: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x801674E8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801674EC: sw          $t8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r24;
    // 0x801674F0: sw          $v1, 0x320($sp)
    MEM_W(0X320, ctx->r29) = ctx->r3;
    // 0x801674F4: lh          $t9, 0x1C($s5)
    ctx->r25 = MEM_H(ctx->r21, 0X1C);
    // 0x801674F8: addiu       $s3, $s3, 0x40
    ctx->r19 = ADD32(ctx->r19, 0X40);
    // 0x801674FC: addiu       $s1, $s1, 0xC
    ctx->r17 = ADD32(ctx->r17, 0XC);
    // 0x80167500: slt         $at, $s0, $t9
    ctx->r1 = SIGNED(ctx->r16) < SIGNED(ctx->r25) ? 1 : 0;
    // 0x80167504: bne         $at, $zero, L_80167468
    if (ctx->r1 != 0) {
        // 0x80167508: addiu       $s4, $s4, 0x1
        ctx->r20 = ADD32(ctx->r20, 0X1);
            goto L_80167468;
    }
    // 0x80167508: addiu       $s4, $s4, 0x1
    ctx->r20 = ADD32(ctx->r20, 0X1);
    // 0x8016750C: lw          $a3, 0x80($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X80);
L_80167510:
    // 0x80167510: addiu       $s7, $s7, 0x4
    ctx->r23 = ADD32(ctx->r23, 0X4);
L_80167514:
    // 0x80167514: slti        $at, $s7, 0xC
    ctx->r1 = SIGNED(ctx->r23) < 0XC ? 1 : 0;
    // 0x80167518: bne         $at, $zero, L_8016741C
    if (ctx->r1 != 0) {
        // 0x8016751C: addiu       $a3, $a3, 0x4
        ctx->r7 = ADD32(ctx->r7, 0X4);
            goto L_8016741C;
    }
    // 0x8016751C: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x80167520: lw          $t5, 0x338($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X338);
    // 0x80167524: lw          $t4, 0x3F0($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X3F0);
    // 0x80167528: sw          $v1, 0x320($sp)
    MEM_W(0X320, ctx->r29) = ctx->r3;
    // 0x8016752C: sw          $t5, 0x33C($sp)
    MEM_W(0X33C, ctx->r29) = ctx->r13;
    // 0x80167530: sw          $t5, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r13;
    // 0x80167534: lw          $t6, 0x324($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X324);
    // 0x80167538: lw          $t8, 0x33C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X33C);
    // 0x8016753C: or          $a0, $v1, $zero
    ctx->r4 = ctx->r3 | 0;
    // 0x80167540: sll         $t7, $t6, 6
    ctx->r15 = S32(ctx->r14 << 6);
    // 0x80167544: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x80167548: sw          $v0, 0x340($sp)
    MEM_W(0X340, ctx->r29) = ctx->r2;
    // 0x8016754C: sw          $v0, 0x4($t4)
    MEM_W(0X4, ctx->r12) = ctx->r2;
    // 0x80167550: lw          $t9, 0x328($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X328);
    // 0x80167554: lw          $t6, 0x340($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X340);
    // 0x80167558: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016755C: sll         $t5, $t9, 6
    ctx->r13 = S32(ctx->r25 << 6);
    // 0x80167560: addu        $v0, $t5, $t6
    ctx->r2 = ADD32(ctx->r13, ctx->r14);
    // 0x80167564: sw          $v0, 0x344($sp)
    MEM_W(0X344, ctx->r29) = ctx->r2;
    // 0x80167568: sw          $v0, 0x8($t4)
    MEM_W(0X8, ctx->r12) = ctx->r2;
    // 0x8016756C: lw          $t7, -0x112C($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X112C);
    // 0x80167570: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x80167574: blez        $a0, L_80167D14
    if (SIGNED(ctx->r4) <= 0) {
        // 0x80167578: addu        $v1, $t7, $v1
        ctx->r3 = ADD32(ctx->r15, ctx->r3);
            goto L_80167D14;
    }
    // 0x80167578: addu        $v1, $t7, $v1
    ctx->r3 = ADD32(ctx->r15, ctx->r3);
    // 0x8016757C: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x80167580: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x80167584: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80167588: addiu       $t5, $sp, 0x19C
    ctx->r13 = ADD32(ctx->r29, 0X19C);
    // 0x8016758C: addu        $t7, $t6, $t5
    ctx->r15 = ADD32(ctx->r14, ctx->r13);
    // 0x80167590: addiu       $t8, $sp, 0xFC
    ctx->r24 = ADD32(ctx->r29, 0XFC);
    // 0x80167594: addiu       $t9, $sp, 0x11C
    ctx->r25 = ADD32(ctx->r29, 0X11C);
    // 0x80167598: sw          $t9, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r25;
    // 0x8016759C: sw          $t8, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r24;
    // 0x801675A0: sw          $t7, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r15;
    // 0x801675A4: sw          $t5, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r13;
    // 0x801675A8: addiu       $s7, $sp, 0x9C
    ctx->r23 = ADD32(ctx->r29, 0X9C);
    // 0x801675AC: addiu       $s6, $zero, 0xC
    ctx->r22 = ADD32(0, 0XC);
    // 0x801675B0: addiu       $s4, $zero, 0x3
    ctx->r20 = ADD32(0, 0X3);
L_801675B4:
    // 0x801675B4: lw          $t8, 0x84($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X84);
    // 0x801675B8: lw          $t9, 0x80($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X80);
    // 0x801675BC: addiu       $t5, $sp, 0x33C
    ctx->r13 = ADD32(ctx->r29, 0X33C);
    // 0x801675C0: lbu         $fp, 0x0($t8)
    ctx->r30 = MEM_BU(ctx->r24, 0X0);
    // 0x801675C4: lw          $s3, 0x0($t9)
    ctx->r19 = MEM_W(ctx->r25, 0X0);
    // 0x801675C8: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801675CC: sll         $t6, $fp, 2
    ctx->r14 = S32(ctx->r30 << 2);
    // 0x801675D0: addu        $t7, $t6, $t5
    ctx->r15 = ADD32(ctx->r14, ctx->r13);
    // 0x801675D4: sw          $t7, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r15;
    // 0x801675D8: lw          $s5, 0x0($t7)
    ctx->r21 = MEM_W(ctx->r15, 0X0);
    // 0x801675DC: or          $s0, $s3, $zero
    ctx->r16 = ctx->r19 | 0;
    // 0x801675E0: sb          $fp, 0x2A($s5)
    MEM_B(0X2A, ctx->r21) = ctx->r30;
    // 0x801675E4: lbu         $t8, 0x2B($s3)
    ctx->r24 = MEM_BU(ctx->r19, 0X2B);
    // 0x801675E8: or          $s2, $s5, $zero
    ctx->r18 = ctx->r21 | 0;
    // 0x801675EC: sb          $t8, 0x2B($s5)
    MEM_B(0X2B, ctx->r21) = ctx->r24;
    // 0x801675F0: lh          $t9, 0x28($s3)
    ctx->r25 = MEM_H(ctx->r19, 0X28);
    // 0x801675F4: sh          $t9, 0x28($s5)
    MEM_H(0X28, ctx->r21) = ctx->r25;
    // 0x801675F8: lh          $t6, 0x28($s3)
    ctx->r14 = MEM_H(ctx->r19, 0X28);
    // 0x801675FC: blezl       $t6, L_801676A8
    if (SIGNED(ctx->r14) <= 0) {
        // 0x80167600: lw          $a0, 0x7C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X7C);
            goto L_801676A8;
    }
    goto skip_3;
    // 0x80167600: lw          $a0, 0x7C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X7C);
    skip_3:
    // 0x80167604: lh          $t5, 0x10($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X10);
L_80167608:
    // 0x80167608: addiu       $a0, $sp, 0x350
    ctx->r4 = ADD32(ctx->r29, 0X350);
    // 0x8016760C: addiu       $a1, $sp, 0x3B0
    ctx->r5 = ADD32(ctx->r29, 0X3B0);
    // 0x80167610: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x80167614: addiu       $a2, $sp, 0x3A4
    ctx->r6 = ADD32(ctx->r29, 0X3A4);
    // 0x80167618: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016761C: swc1        $f6, 0x3B0($sp)
    MEM_W(0X3B0, ctx->r29) = ctx->f6.u32l;
    // 0x80167620: lh          $t7, 0x12($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X12);
    // 0x80167624: mtc1        $t7, $f8
    ctx->f8.u32l = ctx->r15;
    // 0x80167628: nop

    // 0x8016762C: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80167630: swc1        $f10, 0x3B4($sp)
    MEM_W(0X3B4, ctx->r29) = ctx->f10.u32l;
    // 0x80167634: lh          $t8, 0x14($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X14);
    // 0x80167638: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016763C: nop

    // 0x80167640: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80167644: jal         0x80173524
    // 0x80167648: swc1        $f18, 0x3B8($sp)
    MEM_W(0X3B8, ctx->r29) = ctx->f18.u32l;
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x80167648: swc1        $f18, 0x3B8($sp)
    MEM_W(0X3B8, ctx->r29) = ctx->f18.u32l;
    after_2:
    // 0x8016764C: lwc1        $f4, 0x3A4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X3A4);
    // 0x80167650: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80167654: addiu       $s0, $s0, 0x6
    ctx->r16 = ADD32(ctx->r16, 0X6);
    // 0x80167658: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016765C: addiu       $s2, $s2, 0x6
    ctx->r18 = ADD32(ctx->r18, 0X6);
    // 0x80167660: mfc1        $t6, $f6
    ctx->r14 = (int32_t)ctx->f6.u32l;
    // 0x80167664: nop

    // 0x80167668: sh          $t6, 0xA($s2)
    MEM_H(0XA, ctx->r18) = ctx->r14;
    // 0x8016766C: lwc1        $f8, 0x3A8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X3A8);
    // 0x80167670: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x80167674: mfc1        $t7, $f10
    ctx->r15 = (int32_t)ctx->f10.u32l;
    // 0x80167678: nop

    // 0x8016767C: sh          $t7, 0xC($s2)
    MEM_H(0XC, ctx->r18) = ctx->r15;
    // 0x80167680: lwc1        $f16, 0x3AC($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X3AC);
    // 0x80167684: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x80167688: mfc1        $t9, $f18
    ctx->r25 = (int32_t)ctx->f18.u32l;
    // 0x8016768C: nop

    // 0x80167690: sh          $t9, 0xE($s2)
    MEM_H(0XE, ctx->r18) = ctx->r25;
    // 0x80167694: lh          $t6, 0x28($s3)
    ctx->r14 = MEM_H(ctx->r19, 0X28);
    // 0x80167698: slt         $at, $s1, $t6
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r14) ? 1 : 0;
    // 0x8016769C: bnel        $at, $zero, L_80167608
    if (ctx->r1 != 0) {
        // 0x801676A0: lh          $t5, 0x10($s0)
        ctx->r13 = MEM_H(ctx->r16, 0X10);
            goto L_80167608;
    }
    goto skip_4;
    // 0x801676A0: lh          $t5, 0x10($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X10);
    skip_4:
    // 0x801676A4: lw          $a0, 0x7C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X7C);
L_801676A8:
    // 0x801676A8: or          $a1, $s5, $zero
    ctx->r5 = ctx->r21 | 0;
    // 0x801676AC: jal         0x80001090
    // 0x801676B0: addiu       $a2, $zero, 0xC
    ctx->r6 = ADD32(0, 0XC);
    memory_copy(rdram, ctx);
        goto after_3;
    // 0x801676B0: addiu       $a2, $zero, 0xC
    ctx->r6 = ADD32(0, 0XC);
    after_3:
    // 0x801676B4: lw          $t5, 0x7C($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X7C);
    // 0x801676B8: lwc1        $f6, 0x3A4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3A4);
    // 0x801676BC: lwc1        $f16, 0x3AC($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X3AC);
    // 0x801676C0: lwc1        $f4, 0x0($t5)
    ctx->f4.u32l = MEM_W(ctx->r13, 0X0);
    // 0x801676C4: lwc1        $f10, 0x8($t5)
    ctx->f10.u32l = MEM_W(ctx->r13, 0X8);
    // 0x801676C8: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x801676CC: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801676D0: lwc1        $f6, 0x3A8($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3A8);
    // 0x801676D4: addiu       $a1, $sp, 0x3BC
    ctx->r5 = ADD32(ctx->r29, 0X3BC);
    // 0x801676D8: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x801676DC: lwc1        $f10, 0x4($t5)
    ctx->f10.u32l = MEM_W(ctx->r13, 0X4);
    // 0x801676E0: addiu       $v0, $sp, 0x3C4
    ctx->r2 = ADD32(ctx->r29, 0X3C4);
    // 0x801676E4: addiu       $a2, $sp, 0x3CA
    ctx->r6 = ADD32(ctx->r29, 0X3CA);
    // 0x801676E8: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x801676EC: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x801676F0: add.s       $f8, $f16, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f16.fl + ctx->f4.fl;
    // 0x801676F4: neg.s       $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = -ctx->f8.fl;
    // 0x801676F8: swc1        $f18, 0xC($s5)
    MEM_W(0XC, ctx->r21) = ctx->f18.u32l;
L_801676FC:
    // 0x801676FC: lh          $v1, 0x10($a0)
    ctx->r3 = MEM_H(ctx->r4, 0X10);
    // 0x80167700: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80167704: sltu        $at, $v0, $a2
    ctx->r1 = ctx->r2 < ctx->r6 ? 1 : 0;
    // 0x80167708: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x8016770C: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x80167710: sh          $v1, -0x2($a1)
    MEM_H(-0X2, ctx->r5) = ctx->r3;
    // 0x80167714: bne         $at, $zero, L_801676FC
    if (ctx->r1 != 0) {
        // 0x80167718: sh          $v1, -0x2($v0)
        MEM_H(-0X2, ctx->r2) = ctx->r3;
            goto L_801676FC;
    }
    // 0x80167718: sh          $v1, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r3;
    // 0x8016771C: lh          $t7, 0x28($s5)
    ctx->r15 = MEM_H(ctx->r21, 0X28);
    // 0x80167720: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
    // 0x80167724: addiu       $a2, $s5, 0x6
    ctx->r6 = ADD32(ctx->r21, 0X6);
    // 0x80167728: slti        $at, $t7, 0x2
    ctx->r1 = SIGNED(ctx->r15) < 0X2 ? 1 : 0;
    // 0x8016772C: bne         $at, $zero, L_80167798
    if (ctx->r1 != 0) {
        // 0x80167730: or          $v1, $a2, $zero
        ctx->r3 = ctx->r6 | 0;
            goto L_80167798;
    }
    // 0x80167730: or          $v1, $a2, $zero
    ctx->r3 = ctx->r6 | 0;
L_80167734:
    // 0x80167734: addiu       $a1, $sp, 0x3C4
    ctx->r5 = ADD32(ctx->r29, 0X3C4);
    // 0x80167738: addiu       $a0, $sp, 0x3BC
    ctx->r4 = ADD32(ctx->r29, 0X3BC);
L_8016773C:
    // 0x8016773C: lh          $v0, 0x10($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X10);
    // 0x80167740: lh          $t8, 0x0($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X0);
    // 0x80167744: addiu       $t6, $sp, 0x3C2
    ctx->r14 = ADD32(ctx->r29, 0X3C2);
    // 0x80167748: slt         $at, $v0, $t8
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r24) ? 1 : 0;
    // 0x8016774C: beql        $at, $zero, L_80167760
    if (ctx->r1 == 0) {
        // 0x80167750: lh          $t9, 0x0($a0)
        ctx->r25 = MEM_H(ctx->r4, 0X0);
            goto L_80167760;
    }
    goto skip_5;
    // 0x80167750: lh          $t9, 0x0($a0)
    ctx->r25 = MEM_H(ctx->r4, 0X0);
    skip_5:
    // 0x80167754: sh          $v0, 0x0($a1)
    MEM_H(0X0, ctx->r5) = ctx->r2;
    // 0x80167758: lh          $v0, 0x10($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X10);
    // 0x8016775C: lh          $t9, 0x0($a0)
    ctx->r25 = MEM_H(ctx->r4, 0X0);
L_80167760:
    // 0x80167760: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80167764: slt         $at, $t9, $v0
    ctx->r1 = SIGNED(ctx->r25) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80167768: beql        $at, $zero, L_80167778
    if (ctx->r1 == 0) {
        // 0x8016776C: addiu       $a0, $a0, 0x2
        ctx->r4 = ADD32(ctx->r4, 0X2);
            goto L_80167778;
    }
    goto skip_6;
    // 0x8016776C: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    skip_6:
    // 0x80167770: sh          $v0, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r2;
    // 0x80167774: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
L_80167778:
    // 0x80167778: bne         $a0, $t6, L_8016773C
    if (ctx->r4 != ctx->r14) {
        // 0x8016777C: addiu       $a1, $a1, 0x2
        ctx->r5 = ADD32(ctx->r5, 0X2);
            goto L_8016773C;
    }
    // 0x8016777C: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x80167780: lh          $v0, 0x28($s5)
    ctx->r2 = MEM_H(ctx->r21, 0X28);
    // 0x80167784: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80167788: addiu       $a2, $a2, 0x6
    ctx->r6 = ADD32(ctx->r6, 0X6);
    // 0x8016778C: slt         $at, $s1, $v0
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80167790: bnel        $at, $zero, L_80167734
    if (ctx->r1 != 0) {
        // 0x80167794: or          $v1, $a2, $zero
        ctx->r3 = ctx->r6 | 0;
            goto L_80167734;
    }
    goto skip_7;
    // 0x80167794: or          $v1, $a2, $zero
    ctx->r3 = ctx->r6 | 0;
    skip_7:
L_80167798:
    // 0x80167798: lh          $t5, 0x3C4($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X3C4);
    // 0x8016779C: addiu       $v1, $sp, 0x374
    ctx->r3 = ADD32(ctx->r29, 0X374);
    // 0x801677A0: addiu       $a1, $sp, 0x3C4
    ctx->r5 = ADD32(ctx->r29, 0X3C4);
    // 0x801677A4: sh          $t5, 0x30($s5)
    MEM_H(0X30, ctx->r21) = ctx->r13;
    // 0x801677A8: lh          $t7, 0x3C6($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X3C6);
    // 0x801677AC: addiu       $a0, $sp, 0x3BC
    ctx->r4 = ADD32(ctx->r29, 0X3BC);
    // 0x801677B0: sh          $t7, 0x2C($s5)
    MEM_H(0X2C, ctx->r21) = ctx->r15;
    // 0x801677B4: lh          $t8, 0x3C8($sp)
    ctx->r24 = MEM_H(ctx->r29, 0X3C8);
    // 0x801677B8: sll         $t7, $fp, 2
    ctx->r15 = S32(ctx->r30 << 2);
    // 0x801677BC: subu        $t7, $t7, $fp
    ctx->r15 = SUB32(ctx->r15, ctx->r30);
    // 0x801677C0: sh          $t8, 0x34($s5)
    MEM_H(0X34, ctx->r21) = ctx->r24;
    // 0x801677C4: lh          $t9, 0x3BC($sp)
    ctx->r25 = MEM_H(ctx->r29, 0X3BC);
    // 0x801677C8: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x801677CC: addu        $a3, $t7, $v1
    ctx->r7 = ADD32(ctx->r15, ctx->r3);
    // 0x801677D0: sh          $t9, 0x32($s5)
    MEM_H(0X32, ctx->r21) = ctx->r25;
    // 0x801677D4: lh          $t6, 0x3BE($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X3BE);
    // 0x801677D8: sh          $t6, 0x2E($s5)
    MEM_H(0X2E, ctx->r21) = ctx->r14;
    // 0x801677DC: lh          $t5, 0x3C0($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X3C0);
    // 0x801677E0: sh          $t5, 0x36($s5)
    MEM_H(0X36, ctx->r21) = ctx->r13;
    // 0x801677E4: sw          $a3, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r7;
L_801677E8:
    // 0x801677E8: lh          $v0, 0x0($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X0);
    // 0x801677EC: lh          $t9, 0x0($a3)
    ctx->r25 = MEM_H(ctx->r7, 0X0);
    // 0x801677F0: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x801677F4: addiu       $t8, $sp, 0x37A
    ctx->r24 = ADD32(ctx->r29, 0X37A);
    // 0x801677F8: slt         $at, $v0, $t9
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r25) ? 1 : 0;
    // 0x801677FC: beql        $at, $zero, L_8016780C
    if (ctx->r1 == 0) {
        // 0x80167800: lh          $a2, 0x0($a0)
        ctx->r6 = MEM_H(ctx->r4, 0X0);
            goto L_8016780C;
    }
    goto skip_8;
    // 0x80167800: lh          $a2, 0x0($a0)
    ctx->r6 = MEM_H(ctx->r4, 0X0);
    skip_8:
    // 0x80167804: sh          $v0, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r2;
    // 0x80167808: lh          $a2, 0x0($a0)
    ctx->r6 = MEM_H(ctx->r4, 0X0);
L_8016780C:
    // 0x8016780C: lh          $t6, 0x6($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X6);
    // 0x80167810: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x80167814: slt         $at, $t6, $a2
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x80167818: beql        $at, $zero, L_80167828
    if (ctx->r1 == 0) {
        // 0x8016781C: lh          $t5, 0x24($v1)
        ctx->r13 = MEM_H(ctx->r3, 0X24);
            goto L_80167828;
    }
    goto skip_9;
    // 0x8016781C: lh          $t5, 0x24($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X24);
    skip_9:
    // 0x80167820: sh          $a2, 0x6($a3)
    MEM_H(0X6, ctx->r7) = ctx->r6;
    // 0x80167824: lh          $t5, 0x24($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X24);
L_80167828:
    // 0x80167828: slt         $at, $v0, $t5
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r13) ? 1 : 0;
    // 0x8016782C: beql        $at, $zero, L_8016783C
    if (ctx->r1 == 0) {
        // 0x80167830: lh          $t7, 0x2A($v1)
        ctx->r15 = MEM_H(ctx->r3, 0X2A);
            goto L_8016783C;
    }
    goto skip_10;
    // 0x80167830: lh          $t7, 0x2A($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X2A);
    skip_10:
    // 0x80167834: sh          $v0, 0x24($v1)
    MEM_H(0X24, ctx->r3) = ctx->r2;
    // 0x80167838: lh          $t7, 0x2A($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X2A);
L_8016783C:
    // 0x8016783C: slt         $at, $t7, $a2
    ctx->r1 = SIGNED(ctx->r15) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x80167840: beql        $at, $zero, L_80167850
    if (ctx->r1 == 0) {
        // 0x80167844: addiu       $v1, $v1, 0x2
        ctx->r3 = ADD32(ctx->r3, 0X2);
            goto L_80167850;
    }
    goto skip_11;
    // 0x80167844: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    skip_11:
    // 0x80167848: sh          $a2, 0x2A($v1)
    MEM_H(0X2A, ctx->r3) = ctx->r6;
    // 0x8016784C: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
L_80167850:
    // 0x80167850: bne         $v1, $t8, L_801677E8
    if (ctx->r3 != ctx->r24) {
        // 0x80167854: addiu       $a3, $a3, 0x2
        ctx->r7 = ADD32(ctx->r7, 0X2);
            goto L_801677E8;
    }
    // 0x80167854: addiu       $a3, $a3, 0x2
    ctx->r7 = ADD32(ctx->r7, 0X2);
    // 0x80167858: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016785C: lw          $t9, 0x6C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X6C);
L_80167860:
    // 0x80167860: addiu       $t6, $sp, 0x380
    ctx->r14 = ADD32(ctx->r29, 0X380);
    // 0x80167864: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80167868: bne         $t9, $t6, L_80167888
    if (ctx->r25 != ctx->r14) {
        // 0x8016786C: nop
    
            goto L_80167888;
    }
    // 0x8016786C: nop

    // 0x80167870: bne         $v1, $at, L_801678B8
    if (ctx->r3 != ctx->r1) {
        // 0x80167874: sw          $v1, 0x3DC($sp)
        MEM_W(0X3DC, ctx->r29) = ctx->r3;
            goto L_801678B8;
    }
    // 0x80167874: sw          $v1, 0x3DC($sp)
    MEM_W(0X3DC, ctx->r29) = ctx->r3;
    // 0x80167878: sw          $v1, 0x3DC($sp)
    MEM_W(0X3DC, ctx->r29) = ctx->r3;
    // 0x8016787C: lh          $t5, 0x28($s5)
    ctx->r13 = MEM_H(ctx->r21, 0X28);
    // 0x80167880: bnel        $s4, $t5, L_801678BC
    if (ctx->r20 != ctx->r13) {
        // 0x80167884: lw          $t7, 0x3DC($sp)
        ctx->r15 = MEM_W(ctx->r29, 0X3DC);
            goto L_801678BC;
    }
    goto skip_12;
    // 0x80167884: lw          $t7, 0x3DC($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X3DC);
    skip_12:
L_80167888:
    // 0x80167888: bnel        $v1, $zero, L_801678A8
    if (ctx->r3 != 0) {
        // 0x8016788C: sb          $zero, 0x3F($s5)
        MEM_B(0X3F, ctx->r21) = 0;
            goto L_801678A8;
    }
    goto skip_13;
    // 0x8016788C: sb          $zero, 0x3F($s5)
    MEM_B(0X3F, ctx->r21) = 0;
    skip_13:
    // 0x80167890: sb          $zero, 0x3B($s5)
    MEM_B(0X3B, ctx->r21) = 0;
    // 0x80167894: sb          $zero, 0x3A($s5)
    MEM_B(0X3A, ctx->r21) = 0;
    // 0x80167898: sb          $zero, 0x39($s5)
    MEM_B(0X39, ctx->r21) = 0;
    // 0x8016789C: b           L_80167CB0
    // 0x801678A0: sb          $zero, 0x38($s5)
    MEM_B(0X38, ctx->r21) = 0;
        goto L_80167CB0;
    // 0x801678A0: sb          $zero, 0x38($s5)
    MEM_B(0X38, ctx->r21) = 0;
    // 0x801678A4: sb          $zero, 0x3F($s5)
    MEM_B(0X3F, ctx->r21) = 0;
L_801678A8:
    // 0x801678A8: sb          $zero, 0x3E($s5)
    MEM_B(0X3E, ctx->r21) = 0;
    // 0x801678AC: sb          $zero, 0x3D($s5)
    MEM_B(0X3D, ctx->r21) = 0;
    // 0x801678B0: b           L_80167CB0
    // 0x801678B4: sb          $zero, 0x3C($s5)
    MEM_B(0X3C, ctx->r21) = 0;
        goto L_80167CB0;
    // 0x801678B4: sb          $zero, 0x3C($s5)
    MEM_B(0X3C, ctx->r21) = 0;
L_801678B8:
    // 0x801678B8: lw          $t7, 0x3DC($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X3DC);
L_801678BC:
    // 0x801678BC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801678C0: addiu       $t9, $t9, 0x24B4
    ctx->r25 = ADD32(ctx->r25, 0X24B4);
    // 0x801678C4: sll         $t8, $t7, 4
    ctx->r24 = S32(ctx->r15 << 4);
    // 0x801678C8: addu        $fp, $t8, $t9
    ctx->r30 = ADD32(ctx->r24, ctx->r25);
    // 0x801678CC: or          $a2, $fp, $zero
    ctx->r6 = ctx->r30 | 0;
    // 0x801678D0: addiu       $a1, $sp, 0x9C
    ctx->r5 = ADD32(ctx->r29, 0X9C);
L_801678D4:
    // 0x801678D4: lw          $a3, 0x0($a2)
    ctx->r7 = MEM_W(ctx->r6, 0X0);
    // 0x801678D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801678DC: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x801678E0: sll         $t6, $a3, 2
    ctx->r14 = S32(ctx->r7 << 2);
    // 0x801678E4: subu        $t6, $t6, $a3
    ctx->r14 = SUB32(ctx->r14, ctx->r7);
    // 0x801678E8: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x801678EC: addu        $a0, $s5, $t6
    ctx->r4 = ADD32(ctx->r21, ctx->r14);
    // 0x801678F0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x801678F4: beq         $v0, $s4, L_8016791C
    if (ctx->r2 == ctx->r20) {
        // 0x801678F8: lh          $t5, 0x10($a0)
        ctx->r13 = MEM_H(ctx->r4, 0X10);
            goto L_8016791C;
    }
    // 0x801678F8: lh          $t5, 0x10($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X10);
L_801678FC:
    // 0x801678FC: mtc1        $t5, $f6
    ctx->f6.u32l = ctx->r13;
    // 0x80167900: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80167904: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80167908: cvt.s.w     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016790C: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x80167910: swc1        $f10, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f10.u32l;
    // 0x80167914: bne         $v0, $s4, L_801678FC
    if (ctx->r2 != ctx->r20) {
        // 0x80167918: lh          $t5, 0x10($a0)
        ctx->r13 = MEM_H(ctx->r4, 0X10);
            goto L_801678FC;
    }
    // 0x80167918: lh          $t5, 0x10($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X10);
L_8016791C:
    // 0x8016791C: mtc1        $t5, $f6
    ctx->f6.u32l = ctx->r13;
    // 0x80167920: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80167924: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x80167928: cvt.s.w     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016792C: swc1        $f10, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f10.u32l;
    // 0x80167930: addiu       $a1, $a1, 0xC
    ctx->r5 = ADD32(ctx->r5, 0XC);
    // 0x80167934: addiu       $t7, $sp, 0xC0
    ctx->r15 = ADD32(ctx->r29, 0XC0);
    // 0x80167938: sltu        $at, $a1, $t7
    ctx->r1 = ctx->r5 < ctx->r15 ? 1 : 0;
    // 0x8016793C: bne         $at, $zero, L_801678D4
    if (ctx->r1 != 0) {
        // 0x80167940: addiu       $a2, $a2, 0x4
        ctx->r6 = ADD32(ctx->r6, 0X4);
            goto L_801678D4;
    }
    // 0x80167940: addiu       $a2, $a2, 0x4
    ctx->r6 = ADD32(ctx->r6, 0X4);
    // 0x80167944: lui         $s2, 0x8019
    ctx->r18 = S32(0X8019 << 16);
    // 0x80167948: addiu       $s2, $s2, 0x24B4
    ctx->r18 = ADD32(ctx->r18, 0X24B4);
    // 0x8016794C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80167950: addiu       $s0, $sp, 0xD0
    ctx->r16 = ADD32(ctx->r29, 0XD0);
L_80167954:
    // 0x80167954: addiu       $s3, $s1, 0x1
    ctx->r19 = ADD32(ctx->r17, 0X1);
    // 0x80167958: div         $zero, $s3, $s4
    lo = S32(S64(S32(ctx->r19)) / S64(S32(ctx->r20))); hi = S32(S64(S32(ctx->r19)) % S64(S32(ctx->r20)));
    // 0x8016795C: lw          $v0, 0x0($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X0);
    // 0x80167960: mfhi        $t8
    ctx->r24 = hi;
    // 0x80167964: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80167968: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x8016796C: multu       $v0, $s6
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167970: addu        $v1, $v1, $t9
    ctx->r3 = ADD32(ctx->r3, ctx->r25);
    // 0x80167974: lw          $v1, 0x24B4($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X24B4);
    // 0x80167978: bne         $s4, $zero, L_80167984
    if (ctx->r20 != 0) {
        // 0x8016797C: nop
    
            goto L_80167984;
    }
    // 0x8016797C: nop

    // 0x80167980: break       7
    do_break(2148956544);
L_80167984:
    // 0x80167984: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80167988: bne         $s4, $at, L_8016799C
    if (ctx->r20 != ctx->r1) {
        // 0x8016798C: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_8016799C;
    }
    // 0x8016798C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80167990: bne         $s3, $at, L_8016799C
    if (ctx->r19 != ctx->r1) {
        // 0x80167994: nop
    
            goto L_8016799C;
    }
    // 0x80167994: nop

    // 0x80167998: break       6
    do_break(2148956568);
L_8016799C:
    // 0x8016799C: mflo        $t6
    ctx->r14 = lo;
    // 0x801679A0: addu        $a1, $s7, $t6
    ctx->r5 = ADD32(ctx->r23, ctx->r14);
    // 0x801679A4: addiu       $a0, $sp, 0xC0
    ctx->r4 = ADD32(ctx->r29, 0XC0);
    // 0x801679A8: multu       $v1, $s6
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801679AC: mflo        $t5
    ctx->r13 = lo;
    // 0x801679B0: addu        $a2, $s7, $t5
    ctx->r6 = ADD32(ctx->r23, ctx->r13);
    // 0x801679B4: jal         0x80172DD4
    // 0x801679B8: nop

    func_80172DD4(rdram, ctx);
        goto after_4;
    // 0x801679B8: nop

    after_4:
    // 0x801679BC: lwc1        $f2, 0xC8($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0XC8);
    // 0x801679C0: lwc1        $f14, 0xC0($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0XC0);
    // 0x801679C4: swc1        $f20, 0xC4($sp)
    MEM_W(0XC4, ctx->r29) = ctx->f20.u32l;
    // 0x801679C8: mul.s       $f16, $f2, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801679CC: nop

    // 0x801679D0: mul.s       $f4, $f14, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x801679D4: jal         0x800A01E0
    // 0x801679D8: add.s       $f12, $f16, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f4.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_5;
    // 0x801679D8: add.s       $f12, $f16, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f4.fl;
    after_5:
    // 0x801679DC: slti        $at, $s3, 0x3
    ctx->r1 = SIGNED(ctx->r19) < 0X3 ? 1 : 0;
    // 0x801679E0: or          $s1, $s3, $zero
    ctx->r17 = ctx->r19 | 0;
    // 0x801679E4: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x801679E8: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x801679EC: bne         $at, $zero, L_80167954
    if (ctx->r1 != 0) {
        // 0x801679F0: swc1        $f0, -0x4($s0)
        MEM_W(-0X4, ctx->r16) = ctx->f0.u32l;
            goto L_80167954;
    }
    // 0x801679F0: swc1        $f0, -0x4($s0)
    MEM_W(-0X4, ctx->r16) = ctx->f0.u32l;
    // 0x801679F4: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801679F8: addiu       $a3, $zero, -0x1
    ctx->r7 = ADD32(0, -0X1);
    // 0x801679FC: addiu       $s0, $sp, 0xD0
    ctx->r16 = ADD32(ctx->r29, 0XD0);
L_80167A00:
    // 0x80167A00: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80167A04: c.eq.s      $f20, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f20.fl == ctx->f8.fl;
    // 0x80167A08: nop

    // 0x80167A0C: bc1fl       L_80167A1C
    if (!c1cs) {
        // 0x80167A10: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80167A1C;
    }
    goto skip_14;
    // 0x80167A10: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_14:
    // 0x80167A14: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x80167A18: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_80167A1C:
    // 0x80167A1C: slti        $at, $s1, 0x3
    ctx->r1 = SIGNED(ctx->r17) < 0X3 ? 1 : 0;
    // 0x80167A20: bne         $at, $zero, L_80167A00
    if (ctx->r1 != 0) {
        // 0x80167A24: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80167A00;
    }
    // 0x80167A24: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80167A28: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80167A2C: beq         $a3, $at, L_80167B58
    if (ctx->r7 == ctx->r1) {
        // 0x80167A30: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80167B58;
    }
    // 0x80167A30: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80167A34: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80167A38: addiu       $a3, $zero, -0x1
    ctx->r7 = ADD32(0, -0X1);
    // 0x80167A3C: addiu       $s0, $sp, 0xD0
    ctx->r16 = ADD32(ctx->r29, 0XD0);
L_80167A40:
    // 0x80167A40: lwc1        $f18, 0x0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80167A44: c.eq.s      $f20, $f18
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f20.fl == ctx->f18.fl;
    // 0x80167A48: nop

    // 0x80167A4C: bc1fl       L_80167A5C
    if (!c1cs) {
        // 0x80167A50: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80167A5C;
    }
    goto skip_15;
    // 0x80167A50: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_15:
    // 0x80167A54: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x80167A58: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_80167A5C:
    // 0x80167A5C: bne         $s1, $s4, L_80167A40
    if (ctx->r17 != ctx->r20) {
        // 0x80167A60: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80167A40;
    }
    // 0x80167A60: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80167A64: multu       $a3, $s6
    result = U64(U32(ctx->r7)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167A68: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80167A6C: addiu       $t8, $t8, 0x24D4
    ctx->r24 = ADD32(ctx->r24, 0X24D4);
    // 0x80167A70: mflo        $t7
    ctx->r15 = lo;
    // 0x80167A74: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x80167A78: lw          $a1, 0x4($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X4);
    // 0x80167A7C: lw          $a2, 0x0($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X0);
    // 0x80167A80: lw          $t1, 0x8($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X8);
    // 0x80167A84: multu       $a1, $s6
    result = U64(U32(ctx->r5)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167A88: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
    // 0x80167A8C: mflo        $t9
    ctx->r25 = lo;
    // 0x80167A90: addu        $a0, $s7, $t9
    ctx->r4 = ADD32(ctx->r23, ctx->r25);
    // 0x80167A94: lwc1        $f0, 0x0($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80167A98: multu       $a2, $s6
    result = U64(U32(ctx->r6)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167A9C: mflo        $t6
    ctx->r14 = lo;
    // 0x80167AA0: addu        $v1, $s7, $t6
    ctx->r3 = ADD32(ctx->r23, ctx->r14);
    // 0x80167AA4: lwc1        $f2, 0x0($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X0);
    // 0x80167AA8: c.eq.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl == ctx->f2.fl;
    // 0x80167AAC: nop

    // 0x80167AB0: bc1fl       L_80167AE0
    if (!c1cs) {
        // 0x80167AB4: c.lt.s      $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
            goto L_80167AE0;
    }
    goto skip_16;
    // 0x80167AB4: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    skip_16:
    // 0x80167AB8: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80167ABC: lwc1        $f10, 0x8($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X8);
    // 0x80167AC0: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x80167AC4: nop

    // 0x80167AC8: bc1f        L_80167AF4
    if (!c1cs) {
        // 0x80167ACC: nop
    
            goto L_80167AF4;
    }
    // 0x80167ACC: nop

    // 0x80167AD0: or          $t0, $a1, $zero
    ctx->r8 = ctx->r5 | 0;
    // 0x80167AD4: b           L_80167AF4
    // 0x80167AD8: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
        goto L_80167AF4;
    // 0x80167AD8: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x80167ADC: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
L_80167AE0:
    // 0x80167AE0: nop

    // 0x80167AE4: bc1f        L_80167AF4
    if (!c1cs) {
        // 0x80167AE8: nop
    
            goto L_80167AF4;
    }
    // 0x80167AE8: nop

    // 0x80167AEC: or          $t0, $a1, $zero
    ctx->r8 = ctx->r5 | 0;
    // 0x80167AF0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
L_80167AF4:
    // 0x80167AF4: multu       $t1, $s6
    result = U64(U32(ctx->r9)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167AF8: mflo        $t5
    ctx->r13 = lo;
    // 0x80167AFC: addu        $v0, $s7, $t5
    ctx->r2 = ADD32(ctx->r23, ctx->r13);
    // 0x80167B00: lwc1        $f0, 0x0($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80167B04: c.eq.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl == ctx->f2.fl;
    // 0x80167B08: nop

    // 0x80167B0C: bc1fl       L_80167B40
    if (!c1cs) {
        // 0x80167B10: c.lt.s      $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
            goto L_80167B40;
    }
    goto skip_17;
    // 0x80167B10: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    skip_17:
    // 0x80167B14: lwc1        $f16, 0x8($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80167B18: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x80167B1C: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80167B20: c.lt.s      $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f16.fl < ctx->f4.fl;
    // 0x80167B24: nop

    // 0x80167B28: bc1fl       L_80167C24
    if (!c1cs) {
        // 0x80167B2C: lw          $t5, 0x3DC($sp)
        ctx->r13 = MEM_W(ctx->r29, 0X3DC);
            goto L_80167C24;
    }
    goto skip_18;
    // 0x80167B2C: lw          $t5, 0x3DC($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X3DC);
    skip_18:
    // 0x80167B30: or          $a2, $t1, $zero
    ctx->r6 = ctx->r9 | 0;
    // 0x80167B34: b           L_80167C20
    // 0x80167B38: or          $t1, $v0, $zero
    ctx->r9 = ctx->r2 | 0;
        goto L_80167C20;
    // 0x80167B38: or          $t1, $v0, $zero
    ctx->r9 = ctx->r2 | 0;
    // 0x80167B3C: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
L_80167B40:
    // 0x80167B40: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80167B44: bc1fl       L_80167C24
    if (!c1cs) {
        // 0x80167B48: lw          $t5, 0x3DC($sp)
        ctx->r13 = MEM_W(ctx->r29, 0X3DC);
            goto L_80167C24;
    }
    goto skip_19;
    // 0x80167B48: lw          $t5, 0x3DC($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X3DC);
    skip_19:
    // 0x80167B4C: or          $a2, $t1, $zero
    ctx->r6 = ctx->r9 | 0;
    // 0x80167B50: b           L_80167C20
    // 0x80167B54: or          $t1, $v0, $zero
    ctx->r9 = ctx->r2 | 0;
        goto L_80167C20;
    // 0x80167B54: or          $t1, $v0, $zero
    ctx->r9 = ctx->r2 | 0;
L_80167B58:
    // 0x80167B58: lwc1        $f2, 0xD0($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0XD0);
    // 0x80167B5C: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
    // 0x80167B60: addiu       $s0, $sp, 0xD4
    ctx->r16 = ADD32(ctx->r29, 0XD4);
    // 0x80167B64: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
L_80167B68:
    // 0x80167B68: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80167B6C: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
    // 0x80167B70: nop

    // 0x80167B74: bc1fl       L_80167B88
    if (!c1cs) {
        // 0x80167B78: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80167B88;
    }
    goto skip_20;
    // 0x80167B78: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_20:
    // 0x80167B7C: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x80167B80: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x80167B84: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_80167B88:
    // 0x80167B88: bne         $s1, $s4, L_80167B68
    if (ctx->r17 != ctx->r20) {
        // 0x80167B8C: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80167B68;
    }
    // 0x80167B8C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80167B90: multu       $v0, $s6
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167B94: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80167B98: addiu       $t8, $t8, 0x24F8
    ctx->r24 = ADD32(ctx->r24, 0X24F8);
    // 0x80167B9C: mflo        $t7
    ctx->r15 = lo;
    // 0x80167BA0: addu        $v1, $t7, $t8
    ctx->r3 = ADD32(ctx->r15, ctx->r24);
    // 0x80167BA4: lw          $a1, 0x4($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X4);
    // 0x80167BA8: lw          $t0, 0x0($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X0);
    // 0x80167BAC: lw          $t1, 0x8($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X8);
    // 0x80167BB0: multu       $a1, $s6
    result = U64(U32(ctx->r5)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167BB4: mflo        $t9
    ctx->r25 = lo;
    // 0x80167BB8: addu        $a0, $s7, $t9
    ctx->r4 = ADD32(ctx->r23, ctx->r25);
    // 0x80167BBC: lwc1        $f0, 0x0($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80167BC0: multu       $t0, $s6
    result = U64(U32(ctx->r8)) * U64(U32(ctx->r22)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80167BC4: mflo        $t6
    ctx->r14 = lo;
    // 0x80167BC8: addu        $a3, $s7, $t6
    ctx->r7 = ADD32(ctx->r23, ctx->r14);
    // 0x80167BCC: lwc1        $f2, 0x0($a3)
    ctx->f2.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80167BD0: c.eq.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl == ctx->f2.fl;
    // 0x80167BD4: nop

    // 0x80167BD8: bc1fl       L_80167C0C
    if (!c1cs) {
        // 0x80167BDC: c.lt.s      $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
            goto L_80167C0C;
    }
    goto skip_21;
    // 0x80167BDC: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    skip_21:
    // 0x80167BE0: lwc1        $f8, 0x8($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80167BE4: lwc1        $f18, 0x8($a3)
    ctx->f18.u32l = MEM_W(ctx->r7, 0X8);
    // 0x80167BE8: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80167BEC: c.lt.s      $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f8.fl < ctx->f18.fl;
    // 0x80167BF0: nop

    // 0x80167BF4: bc1fl       L_80167C24
    if (!c1cs) {
        // 0x80167BF8: lw          $t5, 0x3DC($sp)
        ctx->r13 = MEM_W(ctx->r29, 0X3DC);
            goto L_80167C24;
    }
    goto skip_22;
    // 0x80167BF8: lw          $t5, 0x3DC($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X3DC);
    skip_22:
    // 0x80167BFC: or          $t0, $a1, $zero
    ctx->r8 = ctx->r5 | 0;
    // 0x80167C00: b           L_80167C20
    // 0x80167C04: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
        goto L_80167C20;
    // 0x80167C04: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80167C08: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
L_80167C0C:
    // 0x80167C0C: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80167C10: bc1fl       L_80167C24
    if (!c1cs) {
        // 0x80167C14: lw          $t5, 0x3DC($sp)
        ctx->r13 = MEM_W(ctx->r29, 0X3DC);
            goto L_80167C24;
    }
    goto skip_23;
    // 0x80167C14: lw          $t5, 0x3DC($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X3DC);
    skip_23:
    // 0x80167C18: or          $t0, $a1, $zero
    ctx->r8 = ctx->r5 | 0;
    // 0x80167C1C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
L_80167C20:
    // 0x80167C20: lw          $t5, 0x3DC($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X3DC);
L_80167C24:
    // 0x80167C24: sll         $t9, $t0, 2
    ctx->r25 = S32(ctx->r8 << 2);
    // 0x80167C28: sll         $t7, $t0, 2
    ctx->r15 = S32(ctx->r8 << 2);
    // 0x80167C2C: bne         $t5, $zero, L_80167C74
    if (ctx->r13 != 0) {
        // 0x80167C30: addu        $t6, $fp, $t9
        ctx->r14 = ADD32(ctx->r30, ctx->r25);
            goto L_80167C74;
    }
    // 0x80167C30: addu        $t6, $fp, $t9
    ctx->r14 = ADD32(ctx->r30, ctx->r25);
    // 0x80167C34: addu        $t8, $fp, $t7
    ctx->r24 = ADD32(ctx->r30, ctx->r15);
    // 0x80167C38: lw          $t9, 0x0($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X0);
    // 0x80167C3C: sll         $t6, $a1, 2
    ctx->r14 = S32(ctx->r5 << 2);
    // 0x80167C40: addu        $t5, $fp, $t6
    ctx->r13 = ADD32(ctx->r30, ctx->r14);
    // 0x80167C44: sb          $t9, 0x38($s5)
    MEM_B(0X38, ctx->r21) = ctx->r25;
    // 0x80167C48: lw          $t7, 0x0($t5)
    ctx->r15 = MEM_W(ctx->r13, 0X0);
    // 0x80167C4C: sll         $t8, $a2, 2
    ctx->r24 = S32(ctx->r6 << 2);
    // 0x80167C50: addu        $t9, $fp, $t8
    ctx->r25 = ADD32(ctx->r30, ctx->r24);
    // 0x80167C54: sb          $t7, 0x39($s5)
    MEM_B(0X39, ctx->r21) = ctx->r15;
    // 0x80167C58: lw          $t6, 0x0($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X0);
    // 0x80167C5C: sll         $t5, $t1, 2
    ctx->r13 = S32(ctx->r9 << 2);
    // 0x80167C60: addu        $t7, $fp, $t5
    ctx->r15 = ADD32(ctx->r30, ctx->r13);
    // 0x80167C64: sb          $t6, 0x3A($s5)
    MEM_B(0X3A, ctx->r21) = ctx->r14;
    // 0x80167C68: lw          $t8, 0x0($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X0);
    // 0x80167C6C: b           L_80167CAC
    // 0x80167C70: sb          $t8, 0x3B($s5)
    MEM_B(0X3B, ctx->r21) = ctx->r24;
        goto L_80167CAC;
    // 0x80167C70: sb          $t8, 0x3B($s5)
    MEM_B(0X3B, ctx->r21) = ctx->r24;
L_80167C74:
    // 0x80167C74: lw          $t5, 0x0($t6)
    ctx->r13 = MEM_W(ctx->r14, 0X0);
    // 0x80167C78: sll         $t7, $a1, 2
    ctx->r15 = S32(ctx->r5 << 2);
    // 0x80167C7C: addu        $t8, $fp, $t7
    ctx->r24 = ADD32(ctx->r30, ctx->r15);
    // 0x80167C80: sb          $t5, 0x3C($s5)
    MEM_B(0X3C, ctx->r21) = ctx->r13;
    // 0x80167C84: lw          $t9, 0x0($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X0);
    // 0x80167C88: sll         $t6, $a2, 2
    ctx->r14 = S32(ctx->r6 << 2);
    // 0x80167C8C: addu        $t5, $fp, $t6
    ctx->r13 = ADD32(ctx->r30, ctx->r14);
    // 0x80167C90: sb          $t9, 0x3D($s5)
    MEM_B(0X3D, ctx->r21) = ctx->r25;
    // 0x80167C94: lw          $t7, 0x0($t5)
    ctx->r15 = MEM_W(ctx->r13, 0X0);
    // 0x80167C98: sll         $t8, $t1, 2
    ctx->r24 = S32(ctx->r9 << 2);
    // 0x80167C9C: addu        $t9, $fp, $t8
    ctx->r25 = ADD32(ctx->r30, ctx->r24);
    // 0x80167CA0: sb          $t7, 0x3E($s5)
    MEM_B(0X3E, ctx->r21) = ctx->r15;
    // 0x80167CA4: lw          $t6, 0x0($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X0);
    // 0x80167CA8: sb          $t6, 0x3F($s5)
    MEM_B(0X3F, ctx->r21) = ctx->r14;
L_80167CAC:
    // 0x80167CAC: lw          $v1, 0x3DC($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X3DC);
L_80167CB0:
    // 0x80167CB0: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80167CB4: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80167CB8: bnel        $v1, $at, L_80167860
    if (ctx->r3 != ctx->r1) {
        // 0x80167CBC: lw          $t9, 0x6C($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X6C);
            goto L_80167860;
    }
    goto skip_24;
    // 0x80167CBC: lw          $t9, 0x6C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X6C);
    skip_24:
    // 0x80167CC0: lw          $t5, 0x78($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X78);
    // 0x80167CC4: lw          $v0, 0x320($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X320);
    // 0x80167CC8: lw          $t7, 0x0($t5)
    ctx->r15 = MEM_W(ctx->r13, 0X0);
    // 0x80167CCC: addiu       $t8, $t7, 0x40
    ctx->r24 = ADD32(ctx->r15, 0X40);
    // 0x80167CD0: sw          $t8, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->r24;
    // 0x80167CD4: lw          $t9, 0x84($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X84);
    // 0x80167CD8: lw          $t5, 0x7C($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X7C);
    // 0x80167CDC: lw          $t7, 0x80($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X80);
    // 0x80167CE0: addiu       $t6, $t9, 0x1
    ctx->r14 = ADD32(ctx->r25, 0X1);
    // 0x80167CE4: sw          $t6, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r14;
    // 0x80167CE8: lw          $t6, 0x58($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X58);
    // 0x80167CEC: addiu       $t9, $t5, 0xC
    ctx->r25 = ADD32(ctx->r13, 0XC);
    // 0x80167CF0: addiu       $t8, $t7, 0x4
    ctx->r24 = ADD32(ctx->r15, 0X4);
    // 0x80167CF4: sw          $t8, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r24;
    // 0x80167CF8: bne         $t9, $t6, L_801675B4
    if (ctx->r25 != ctx->r14) {
        // 0x80167CFC: sw          $t9, 0x7C($sp)
        MEM_W(0X7C, ctx->r29) = ctx->r25;
            goto L_801675B4;
    }
    // 0x80167CFC: sw          $t9, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r25;
    // 0x80167D00: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x80167D04: lw          $t7, -0x112C($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X112C);
    // 0x80167D08: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x80167D0C: lw          $t4, 0x3F0($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X3F0);
    // 0x80167D10: addu        $v1, $t7, $v0
    ctx->r3 = ADD32(ctx->r15, ctx->r2);
L_80167D14:
    // 0x80167D14: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80167D18: addiu       $s4, $zero, 0x3
    ctx->r20 = ADD32(0, 0X3);
    // 0x80167D1C: sw          $v1, -0x112C($at)
    MEM_W(-0X112C, ctx->r1) = ctx->r3;
    // 0x80167D20: or          $a1, $t4, $zero
    ctx->r5 = ctx->r12 | 0;
    // 0x80167D24: addiu       $t1, $t4, 0xC
    ctx->r9 = ADD32(ctx->r12, 0XC);
    // 0x80167D28: addiu       $t2, $sp, 0x324
    ctx->r10 = ADD32(ctx->r29, 0X324);
    // 0x80167D2C: or          $a0, $t4, $zero
    ctx->r4 = ctx->r12 | 0;
    // 0x80167D30: addiu       $t3, $sp, 0x374
    ctx->r11 = ADD32(ctx->r29, 0X374);
L_80167D34:
    // 0x80167D34: sb          $t0, 0x2A($a1)
    MEM_B(0X2A, ctx->r5) = ctx->r8;
    // 0x80167D38: lw          $t8, 0x0($t2)
    ctx->r24 = MEM_W(ctx->r10, 0X0);
    // 0x80167D3C: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80167D40: or          $a2, $t3, $zero
    ctx->r6 = ctx->r11 | 0;
    // 0x80167D44: sh          $t8, 0x28($a1)
    MEM_H(0X28, ctx->r5) = ctx->r24;
    // 0x80167D48: lw          $t5, 0x0($a0)
    ctx->r13 = MEM_W(ctx->r4, 0X0);
    // 0x80167D4C: or          $a3, $t1, $zero
    ctx->r7 = ctx->r9 | 0;
    // 0x80167D50: sw          $t5, 0x24($a1)
    MEM_W(0X24, ctx->r5) = ctx->r13;
L_80167D54:
    // 0x80167D54: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80167D58: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80167D5C: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
    // 0x80167D60: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80167D64: beq         $s1, $s4, L_80167D8C
    if (ctx->r17 == ctx->r20) {
        // 0x80167D68: lh          $t9, 0x0($v0)
        ctx->r25 = MEM_H(ctx->r2, 0X0);
            goto L_80167D8C;
    }
    // 0x80167D68: lh          $t9, 0x0($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X0);
L_80167D6C:
    // 0x80167D6C: mtc1        $t9, $f24
    ctx->f24.u32l = ctx->r25;
    // 0x80167D70: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80167D74: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80167D78: cvt.s.w     $f24, $f24
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    ctx->f24.fl = CVT_S_W(ctx->f24.u32l);
    // 0x80167D7C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80167D80: swc1        $f24, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f24.u32l;
    // 0x80167D84: bne         $s1, $s4, L_80167D6C
    if (ctx->r17 != ctx->r20) {
        // 0x80167D88: lh          $t9, 0x0($v0)
        ctx->r25 = MEM_H(ctx->r2, 0X0);
            goto L_80167D6C;
    }
    // 0x80167D88: lh          $t9, 0x0($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X0);
L_80167D8C:
    // 0x80167D8C: mtc1        $t9, $f24
    ctx->f24.u32l = ctx->r25;
    // 0x80167D90: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80167D94: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80167D98: cvt.s.w     $f24, $f24
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    ctx->f24.fl = CVT_S_W(ctx->f24.u32l);
    // 0x80167D9C: swc1        $f24, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f24.u32l;
    // 0x80167DA0: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80167DA4: slti        $at, $s0, 0x2
    ctx->r1 = SIGNED(ctx->r16) < 0X2 ? 1 : 0;
    // 0x80167DA8: addiu       $a2, $a2, 0x6
    ctx->r6 = ADD32(ctx->r6, 0X6);
    // 0x80167DAC: bne         $at, $zero, L_80167D54
    if (ctx->r1 != 0) {
        // 0x80167DB0: addiu       $a3, $a3, 0xC
        ctx->r7 = ADD32(ctx->r7, 0XC);
            goto L_80167D54;
    }
    // 0x80167DB0: addiu       $a3, $a3, 0xC
    ctx->r7 = ADD32(ctx->r7, 0XC);
    // 0x80167DB4: addiu       $t0, $t0, 0x1
    ctx->r8 = ADD32(ctx->r8, 0X1);
    // 0x80167DB8: slti        $at, $t0, 0x3
    ctx->r1 = SIGNED(ctx->r8) < 0X3 ? 1 : 0;
    // 0x80167DBC: addiu       $a1, $a1, 0x20
    ctx->r5 = ADD32(ctx->r5, 0X20);
    // 0x80167DC0: addiu       $t1, $t1, 0x20
    ctx->r9 = ADD32(ctx->r9, 0X20);
    // 0x80167DC4: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x80167DC8: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80167DCC: bne         $at, $zero, L_80167D34
    if (ctx->r1 != 0) {
        // 0x80167DD0: addiu       $t3, $t3, 0xC
        ctx->r11 = ADD32(ctx->r11, 0XC);
            goto L_80167D34;
    }
    // 0x80167DD0: addiu       $t3, $t3, 0xC
    ctx->r11 = ADD32(ctx->r11, 0XC);
    // 0x80167DD4: lw          $a2, 0x3F0($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X3F0);
    // 0x80167DD8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80167DDC: addiu       $v1, $zero, 0x60
    ctx->r3 = ADD32(0, 0X60);
    // 0x80167DE0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x80167DE4: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
L_80167DE8:
    // 0x80167DE8: lh          $t6, 0x28($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X28);
    // 0x80167DEC: addiu       $v0, $v0, 0x20
    ctx->r2 = ADD32(ctx->r2, 0X20);
    // 0x80167DF0: addiu       $t7, $a1, 0xC
    ctx->r15 = ADD32(ctx->r5, 0XC);
    // 0x80167DF4: beql        $t6, $zero, L_80167E08
    if (ctx->r14 == 0) {
        // 0x80167DF8: sw          $zero, 0x6C($a0)
        MEM_W(0X6C, ctx->r4) = 0;
            goto L_80167E08;
    }
    goto skip_25;
    // 0x80167DF8: sw          $zero, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = 0;
    skip_25:
    // 0x80167DFC: b           L_80167E08
    // 0x80167E00: sw          $t7, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->r15;
        goto L_80167E08;
    // 0x80167E00: sw          $t7, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->r15;
    // 0x80167E04: sw          $zero, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = 0;
L_80167E08:
    // 0x80167E08: addiu       $a1, $a1, 0x20
    ctx->r5 = ADD32(ctx->r5, 0X20);
    // 0x80167E0C: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80167E10: bne         $v0, $v1, L_80167DE8
    if (ctx->r2 != ctx->r3) {
        // 0x80167E14: sw          $zero, 0x74($a0)
        MEM_W(0X74, ctx->r4) = 0;
            goto L_80167DE8;
    }
    // 0x80167E14: sw          $zero, 0x74($a0)
    MEM_W(0X74, ctx->r4) = 0;
    // 0x80167E18: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x80167E1C: addiu       $a0, $sp, 0x374
    ctx->r4 = ADD32(ctx->r29, 0X374);
    // 0x80167E20: addiu       $a3, $sp, 0x380
    ctx->r7 = ADD32(ctx->r29, 0X380);
L_80167E24:
    // 0x80167E24: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80167E28: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80167E2C: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80167E30: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80167E34: beq         $s1, $s4, L_80167E5C
    if (ctx->r17 == ctx->r20) {
        // 0x80167E38: lh          $t8, 0x24($v1)
        ctx->r24 = MEM_H(ctx->r3, 0X24);
            goto L_80167E5C;
    }
    // 0x80167E38: lh          $t8, 0x24($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X24);
L_80167E3C:
    // 0x80167E3C: mtc1        $t8, $f24
    ctx->f24.u32l = ctx->r24;
    // 0x80167E40: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80167E44: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80167E48: cvt.s.w     $f24, $f24
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    ctx->f24.fl = CVT_S_W(ctx->f24.u32l);
    // 0x80167E4C: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80167E50: swc1        $f24, 0x80($v0)
    MEM_W(0X80, ctx->r2) = ctx->f24.u32l;
    // 0x80167E54: bne         $s1, $s4, L_80167E3C
    if (ctx->r17 != ctx->r20) {
        // 0x80167E58: lh          $t8, 0x24($v1)
        ctx->r24 = MEM_H(ctx->r3, 0X24);
            goto L_80167E3C;
    }
    // 0x80167E58: lh          $t8, 0x24($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X24);
L_80167E5C:
    // 0x80167E5C: mtc1        $t8, $f24
    ctx->f24.u32l = ctx->r24;
    // 0x80167E60: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80167E64: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80167E68: cvt.s.w     $f24, $f24
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    ctx->f24.fl = CVT_S_W(ctx->f24.u32l);
    // 0x80167E6C: swc1        $f24, 0x80($v0)
    MEM_W(0X80, ctx->r2) = ctx->f24.u32l;
    // 0x80167E70: addiu       $a0, $a0, 0x6
    ctx->r4 = ADD32(ctx->r4, 0X6);
    // 0x80167E74: bne         $a0, $a3, L_80167E24
    if (ctx->r4 != ctx->r7) {
        // 0x80167E78: addiu       $a1, $a1, 0xC
        ctx->r5 = ADD32(ctx->r5, 0XC);
            goto L_80167E24;
    }
    // 0x80167E78: addiu       $a1, $a1, 0xC
    ctx->r5 = ADD32(ctx->r5, 0XC);
    // 0x80167E7C: lw          $v0, 0x3F8($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X3F8);
    // 0x80167E80: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x80167E84: lh          $t5, 0x0($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X0);
    // 0x80167E88: sh          $t5, 0x9C($a2)
    MEM_H(0X9C, ctx->r6) = ctx->r13;
    // 0x80167E8C: lh          $t9, 0x2($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X2);
    // 0x80167E90: sh          $t9, 0x9E($a2)
    MEM_H(0X9E, ctx->r6) = ctx->r25;
    // 0x80167E94: lh          $t6, 0x4($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X4);
    // 0x80167E98: sh          $zero, 0xA2($a2)
    MEM_H(0XA2, ctx->r6) = 0;
    // 0x80167E9C: sh          $zero, 0xA4($a2)
    MEM_H(0XA4, ctx->r6) = 0;
    // 0x80167EA0: sh          $zero, 0xA6($a2)
    MEM_H(0XA6, ctx->r6) = 0;
    // 0x80167EA4: sh          $t6, 0xA0($a2)
    MEM_H(0XA0, ctx->r6) = ctx->r14;
    // 0x80167EA8: lh          $t7, 0x24($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X24);
    // 0x80167EAC: sh          $t8, 0xC2($a2)
    MEM_H(0XC2, ctx->r6) = ctx->r24;
    // 0x80167EB0: sh          $t7, 0xC0($a2)
    MEM_H(0XC0, ctx->r6) = ctx->r15;
    // 0x80167EB4: lw          $ra, 0x54($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X54);
    // 0x80167EB8: lw          $fp, 0x50($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X50);
    // 0x80167EBC: lw          $s7, 0x4C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X4C);
    // 0x80167EC0: lw          $s6, 0x48($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X48);
    // 0x80167EC4: lw          $s5, 0x44($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X44);
    // 0x80167EC8: lw          $s4, 0x40($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X40);
    // 0x80167ECC: lw          $s3, 0x3C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X3C);
    // 0x80167ED0: lw          $s2, 0x38($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X38);
    // 0x80167ED4: lw          $s1, 0x34($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X34);
    // 0x80167ED8: lw          $s0, 0x30($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X30);
    // 0x80167EDC: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x80167EE0: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x80167EE4: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80167EE8: jr          $ra
    // 0x80167EEC: addiu       $sp, $sp, 0x3F0
    ctx->r29 = ADD32(ctx->r29, 0X3F0);
    return;
    // 0x80167EEC: addiu       $sp, $sp, 0x3F0
    ctx->r29 = ADD32(ctx->r29, 0X3F0);
;}
RECOMP_FUNC void func_80167EF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167EF0: jr          $ra
    // 0x80167EF4: nop

    return;
    // 0x80167EF4: nop

    // 0x80167EF8: jr          $ra
    // 0x80167EFC: nop

    return;
    // 0x80167EFC: nop

    // 0x80167F00: jr          $ra
    // 0x80167F04: nop

    return;
    // 0x80167F04: nop

    // 0x80167F08: jr          $ra
    // 0x80167F0C: nop

    return;
    // 0x80167F0C: nop

    // 0x80167F10: jr          $ra
    // 0x80167F14: nop

    return;
    // 0x80167F14: nop

;}
