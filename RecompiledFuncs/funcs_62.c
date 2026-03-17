#include "recomp.h"
#include "funcs.h"
#include <stdio.h>

RECOMP_FUNC void func_80179FD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80179FD0: addiu       $sp, $sp, -0xD0
    ctx->r29 = ADD32(ctx->r29, -0XD0);
    // 0x80179FD4: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80179FD8: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x80179FDC: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80179FE0: sw          $a1, 0xD4($sp)
    MEM_W(0XD4, ctx->r29) = ctx->r5;
    // 0x80179FE4: sw          $a2, 0xD8($sp)
    MEM_W(0XD8, ctx->r29) = ctx->r6;
    // 0x80179FE8: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80179FEC: addiu       $a0, $sp, 0xC0
    ctx->r4 = ADD32(ctx->r29, 0XC0);
    // 0x80179FF0: addiu       $a2, $v0, 0x18
    ctx->r6 = ADD32(ctx->r2, 0X18);
    // 0x80179FF4: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x80179FF8: sw          $a2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r6;
    // 0x80179FFC: jal         0x80013CF8
    // 0x8017A000: sw          $v0, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->r2;
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x8017A000: sw          $v0, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->r2;
    after_0:
    // 0x8017A004: lw          $a1, 0xCC($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XCC);
    // 0x8017A008: lw          $a2, 0x2C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X2C);
    // 0x8017A00C: addiu       $a0, $sp, 0xB4
    ctx->r4 = ADD32(ctx->r29, 0XB4);
    // 0x8017A010: jal         0x80013CC4
    // 0x8017A014: addiu       $a1, $a1, 0x24
    ctx->r5 = ADD32(ctx->r5, 0X24);
    func_80013CC4(rdram, ctx);
        goto after_1;
    // 0x8017A014: addiu       $a1, $a1, 0x24
    ctx->r5 = ADD32(ctx->r5, 0X24);
    after_1:
    // 0x8017A018: addiu       $a0, $sp, 0xB4
    ctx->r4 = ADD32(ctx->r29, 0XB4);
    // 0x8017A01C: jal         0x80013DE4
    // 0x8017A020: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013DE4(rdram, ctx);
        goto after_2;
    // 0x8017A020: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_2:
    // 0x8017A024: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x8017A028: addiu       $a1, $a1, -0x3068
    ctx->r5 = ADD32(ctx->r5, -0X3068);
    // 0x8017A02C: addiu       $a0, $sp, 0xA8
    ctx->r4 = ADD32(ctx->r29, 0XA8);
    // 0x8017A030: jal         0x80013E6C
    // 0x8017A034: addiu       $a2, $sp, 0xB4
    ctx->r6 = ADD32(ctx->r29, 0XB4);
    func_80013E6C(rdram, ctx);
        goto after_3;
    // 0x8017A034: addiu       $a2, $sp, 0xB4
    ctx->r6 = ADD32(ctx->r29, 0XB4);
    after_3:
    // 0x8017A038: lw          $t6, 0xCC($sp)
    ctx->r14 = MEM_W(ctx->r29, 0XCC);
    // 0x8017A03C: addiu       $a0, $sp, 0x9C
    ctx->r4 = ADD32(ctx->r29, 0X9C);
    // 0x8017A040: lw          $a1, 0xD4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XD4);
    // 0x8017A044: lw          $t7, 0xC($t6)
    ctx->r15 = MEM_W(ctx->r14, 0XC);
    // 0x8017A048: addiu       $a2, $sp, 0xC0
    ctx->r6 = ADD32(ctx->r29, 0XC0);
    // 0x8017A04C: lw          $v0, 0x3C($t7)
    ctx->r2 = MEM_W(ctx->r15, 0X3C);
    // 0x8017A050: lwc1        $f4, 0xC($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0XC);
    // 0x8017A054: lwc1        $f20, 0x8($v0)
    ctx->f20.u32l = MEM_W(ctx->r2, 0X8);
    // 0x8017A058: swc1        $f4, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->f4.u32l;
    // 0x8017A05C: lwc1        $f6, 0x0($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8017A060: jal         0x80013CC4
    // 0x8017A064: swc1        $f6, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->f6.u32l;
    func_80013CC4(rdram, ctx);
        goto after_4;
    // 0x8017A064: swc1        $f6, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->f6.u32l;
    after_4:
    // 0x8017A068: addiu       $a0, $sp, 0x64
    ctx->r4 = ADD32(ctx->r29, 0X64);
    // 0x8017A06C: jal         0x80013CF8
    // 0x8017A070: addiu       $a1, $sp, 0xB4
    ctx->r5 = ADD32(ctx->r29, 0XB4);
    func_80013CF8(rdram, ctx);
        goto after_5;
    // 0x8017A070: addiu       $a1, $sp, 0xB4
    ctx->r5 = ADD32(ctx->r29, 0XB4);
    after_5:
    // 0x8017A074: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x8017A078: addiu       $a0, $sp, 0x64
    ctx->r4 = ADD32(ctx->r29, 0X64);
    // 0x8017A07C: jal         0x80013BD0
    // 0x8017A080: swc1        $f22, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f22.u32l;
    func_80013BD0(rdram, ctx);
        goto after_6;
    // 0x8017A080: swc1        $f22, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f22.u32l;
    after_6:
    // 0x8017A084: lwc1        $f12, 0xB8($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0XB8);
    // 0x8017A088: jal         0x80004C68
    // 0x8017A08C: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    func_80004C68(rdram, ctx);
        goto after_7;
    // 0x8017A08C: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    after_7:
    // 0x8017A090: lwc1        $f12, 0xBC($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0XBC);
    // 0x8017A094: sw          $v0, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->r2;
    // 0x8017A098: lwc1        $f14, 0xB4($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0XB4);
    // 0x8017A09C: jal         0x80004C68
    // 0x8017A0A0: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_80004C68(rdram, ctx);
        goto after_8;
    // 0x8017A0A0: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_8:
    // 0x8017A0A4: addiu       $a0, $sp, 0x9C
    ctx->r4 = ADD32(ctx->r29, 0X9C);
    // 0x8017A0A8: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x8017A0AC: addiu       $a2, $a2, -0x3068
    ctx->r6 = ADD32(ctx->r6, -0X3068);
    // 0x8017A0B0: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017A0B4: jal         0x800140E4
    // 0x8017A0B8: negu        $a3, $v0
    ctx->r7 = SUB32(0, ctx->r2);
    func_800140E4(rdram, ctx);
        goto after_9;
    // 0x8017A0B8: negu        $a3, $v0
    ctx->r7 = SUB32(0, ctx->r2);
    after_9:
    // 0x8017A0BC: lw          $a3, 0x8C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X8C);
    // 0x8017A0C0: addiu       $a1, $sp, 0x9C
    ctx->r5 = ADD32(ctx->r29, 0X9C);
    // 0x8017A0C4: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x8017A0C8: addiu       $a2, $a2, -0x305C
    ctx->r6 = ADD32(ctx->r6, -0X305C);
    // 0x8017A0CC: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x8017A0D0: jal         0x800140E4
    // 0x8017A0D4: negu        $a3, $a3
    ctx->r7 = SUB32(0, ctx->r7);
    func_800140E4(rdram, ctx);
        goto after_10;
    // 0x8017A0D4: negu        $a3, $a3
    ctx->r7 = SUB32(0, ctx->r7);
    after_10:
    // 0x8017A0D8: lwc1        $f0, 0xD8($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0XD8);
    // 0x8017A0DC: lwc1        $f14, 0x9C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x8017A0E0: add.s       $f8, $f14, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f14.fl + ctx->f0.fl;
    // 0x8017A0E4: c.le.s      $f8, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f8.fl <= ctx->f20.fl;
    // 0x8017A0E8: nop

    // 0x8017A0EC: bc1fl       L_8017A100
    if (!c1cs) {
        // 0x8017A0F0: sub.s       $f10, $f14, $f0
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f14.fl - ctx->f0.fl;
            goto L_8017A100;
    }
    goto skip_0;
    // 0x8017A0F0: sub.s       $f10, $f14, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f14.fl - ctx->f0.fl;
    skip_0:
    // 0x8017A0F4: b           L_8017A5E8
    // 0x8017A0F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017A5E8;
    // 0x8017A0F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017A0FC: sub.s       $f10, $f14, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f14.fl - ctx->f0.fl;
L_8017A100:
    // 0x8017A100: lwc1        $f4, 0x94($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X94);
    // 0x8017A104: lwc1        $f6, 0x90($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X90);
    // 0x8017A108: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x8017A10C: c.le.s      $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f4.fl <= ctx->f10.fl;
    // 0x8017A110: nop

    // 0x8017A114: bc1fl       L_8017A128
    if (!c1cs) {
        // 0x8017A118: mtc1        $at, $f8
        ctx->f8.u32l = ctx->r1;
            goto L_8017A128;
    }
    goto skip_1;
    // 0x8017A118: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    skip_1:
    // 0x8017A11C: b           L_8017A5E8
    // 0x8017A120: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017A5E8;
    // 0x8017A120: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017A124: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
L_8017A128:
    // 0x8017A128: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A12C: lwc1        $f4, -0x3F50($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X3F50);
    // 0x8017A130: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8017A134: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x8017A138: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x8017A13C: mul.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8017A140: cfc1        $t9, $FpcCsr
    ctx->r25 = get_cop1_cs();
    // 0x8017A144: ctc1        $a0, $FpcCsr
    set_cop1_cs(ctx->r4);
    // 0x8017A148: nop

    // 0x8017A14C: cvt.w.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.u32l = CVT_W_S(ctx->f6.fl);
    // 0x8017A150: cfc1        $a0, $FpcCsr
    ctx->r4 = get_cop1_cs();
    // 0x8017A154: nop

    // 0x8017A158: andi        $a0, $a0, 0x78
    ctx->r4 = ctx->r4 & 0X78;
    // 0x8017A15C: beql        $a0, $zero, L_8017A1AC
    if (ctx->r4 == 0) {
        // 0x8017A160: mfc1        $a0, $f8
        ctx->r4 = (int32_t)ctx->f8.u32l;
            goto L_8017A1AC;
    }
    goto skip_2;
    // 0x8017A160: mfc1        $a0, $f8
    ctx->r4 = (int32_t)ctx->f8.u32l;
    skip_2:
    // 0x8017A164: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8017A168: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x8017A16C: sub.s       $f8, $f6, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x8017A170: ctc1        $a0, $FpcCsr
    set_cop1_cs(ctx->r4);
    // 0x8017A174: nop

    // 0x8017A178: cvt.w.s     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.u32l = CVT_W_S(ctx->f8.fl);
    // 0x8017A17C: cfc1        $a0, $FpcCsr
    ctx->r4 = get_cop1_cs();
    // 0x8017A180: nop

    // 0x8017A184: andi        $a0, $a0, 0x78
    ctx->r4 = ctx->r4 & 0X78;
    // 0x8017A188: bne         $a0, $zero, L_8017A1A0
    if (ctx->r4 != 0) {
        // 0x8017A18C: nop
    
            goto L_8017A1A0;
    }
    // 0x8017A18C: nop

    // 0x8017A190: mfc1        $a0, $f8
    ctx->r4 = (int32_t)ctx->f8.u32l;
    // 0x8017A194: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x8017A198: b           L_8017A1B8
    // 0x8017A19C: or          $a0, $a0, $at
    ctx->r4 = ctx->r4 | ctx->r1;
        goto L_8017A1B8;
    // 0x8017A19C: or          $a0, $a0, $at
    ctx->r4 = ctx->r4 | ctx->r1;
L_8017A1A0:
    // 0x8017A1A0: b           L_8017A1B8
    // 0x8017A1A4: addiu       $a0, $zero, -0x1
    ctx->r4 = ADD32(0, -0X1);
        goto L_8017A1B8;
    // 0x8017A1A4: addiu       $a0, $zero, -0x1
    ctx->r4 = ADD32(0, -0X1);
    // 0x8017A1A8: mfc1        $a0, $f8
    ctx->r4 = (int32_t)ctx->f8.u32l;
L_8017A1AC:
    // 0x8017A1AC: nop

    // 0x8017A1B0: bltz        $a0, L_8017A1A0
    if (SIGNED(ctx->r4) < 0) {
        // 0x8017A1B4: nop
    
            goto L_8017A1A0;
    }
    // 0x8017A1B4: nop

L_8017A1B8:
    // 0x8017A1B8: ctc1        $t9, $FpcCsr
    set_cop1_cs(ctx->r25);
    // 0x8017A1BC: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x8017A1C0: jal         0x80097330
    // 0x8017A1C4: sw          $a0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r4;
    func_80097330(rdram, ctx);
        goto after_11;
    // 0x8017A1C4: sw          $a0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r4;
    after_11:
    // 0x8017A1C8: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x8017A1CC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A1D0: lwc1        $f6, -0x3F4C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3F4C);
    // 0x8017A1D4: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8017A1D8: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    // 0x8017A1DC: div.s       $f20, $f4, $f6
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f20.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8017A1E0: jal         0x800A3A50
    // 0x8017A1E4: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    func_800A3A50(rdram, ctx);
        goto after_12;
    // 0x8017A1E4: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    after_12:
    // 0x8017A1E8: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x8017A1EC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A1F0: lwc1        $f4, -0x3F48($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X3F48);
    // 0x8017A1F4: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8017A1F8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A1FC: lwc1        $f8, -0x3F44($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X3F44);
    // 0x8017A200: addiu       $a0, $sp, 0x58
    ctx->r4 = ADD32(ctx->r29, 0X58);
    // 0x8017A204: swc1        $f22, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f22.u32l;
    // 0x8017A208: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017A20C: div.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = DIV_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8017A210: swc1        $f20, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f20.u32l;
    // 0x8017A214: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8017A218: jal         0x80013DE4
    // 0x8017A21C: swc1        $f10, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f10.u32l;
    func_80013DE4(rdram, ctx);
        goto after_13;
    // 0x8017A21C: swc1        $f10, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f10.u32l;
    after_13:
    // 0x8017A220: lwc1        $f14, 0x9C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x8017A224: lwc1        $f18, 0x58($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X58);
    // 0x8017A228: lwc1        $f16, 0xA0($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0XA0);
    // 0x8017A22C: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8017A230: mul.s       $f6, $f18, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A234: lwc1        $f20, 0x5C($sp)
    ctx->f20.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8017A238: mul.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8017A23C: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8017A240: add.s       $f6, $f22, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f22.fl + ctx->f10.fl;
    // 0x8017A244: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8017A248: add.s       $f8, $f6, $f22
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f22.fl;
    // 0x8017A24C: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8017A250: nop

    // 0x8017A254: bc1f        L_8017A27C
    if (!c1cs) {
        // 0x8017A258: nop
    
            goto L_8017A27C;
    }
    // 0x8017A258: nop

    // 0x8017A25C: mul.s       $f6, $f18, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A260: nop

    // 0x8017A264: mul.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8017A268: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8017A26C: add.s       $f2, $f22, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f22.fl + ctx->f10.fl;
    // 0x8017A270: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
    // 0x8017A274: b           L_8017A294
    // 0x8017A278: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
        goto L_8017A294;
    // 0x8017A278: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
L_8017A27C:
    // 0x8017A27C: mul.s       $f4, $f18, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A280: nop

    // 0x8017A284: mul.s       $f6, $f20, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f20.fl, ctx->f16.fl);
    // 0x8017A288: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017A28C: add.s       $f2, $f22, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = ctx->f22.fl + ctx->f8.fl;
    // 0x8017A290: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
L_8017A294:
    // 0x8017A294: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8017A298: swc1        $f18, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f18.u32l;
    // 0x8017A29C: lwc1        $f4, 0xD8($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XD8);
    // 0x8017A2A0: c.lt.s      $f10, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f10.fl < ctx->f2.fl;
    // 0x8017A2A4: nop

    // 0x8017A2A8: bc1f        L_8017A2C8
    if (!c1cs) {
        // 0x8017A2AC: nop
    
            goto L_8017A2C8;
    }
    // 0x8017A2AC: nop

    // 0x8017A2B0: c.le.s      $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f4.fl <= ctx->f12.fl;
    // 0x8017A2B4: nop

    // 0x8017A2B8: bc1f        L_8017A2C8
    if (!c1cs) {
        // 0x8017A2BC: nop
    
            goto L_8017A2C8;
    }
    // 0x8017A2BC: nop

    // 0x8017A2C0: b           L_8017A5E8
    // 0x8017A2C4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017A5E8;
    // 0x8017A2C4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017A2C8:
    // 0x8017A2C8: jal         0x80097330
    // 0x8017A2CC: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    func_80097330(rdram, ctx);
        goto after_14;
    // 0x8017A2CC: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    after_14:
    // 0x8017A2D0: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x8017A2D4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A2D8: lwc1        $f10, -0x3F40($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3F40);
    // 0x8017A2DC: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8017A2E0: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    // 0x8017A2E4: div.s       $f20, $f8, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f20.fl = DIV_S(ctx->f8.fl, ctx->f10.fl);
    // 0x8017A2E8: jal         0x800A3A50
    // 0x8017A2EC: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    func_800A3A50(rdram, ctx);
        goto after_15;
    // 0x8017A2EC: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    after_15:
    // 0x8017A2F0: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x8017A2F4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A2F8: lwc1        $f8, -0x3F3C($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X3F3C);
    // 0x8017A2FC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8017A300: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A304: addiu       $a0, $sp, 0x4C
    ctx->r4 = ADD32(ctx->r29, 0X4C);
    // 0x8017A308: swc1        $f22, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f22.u32l;
    // 0x8017A30C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017A310: swc1        $f20, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f20.u32l;
    // 0x8017A314: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8017A318: lwc1        $f6, -0x3F38($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3F38);
    // 0x8017A31C: neg.s       $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = -ctx->f10.fl;
    // 0x8017A320: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8017A324: jal         0x80013DE4
    // 0x8017A328: swc1        $f8, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f8.u32l;
    func_80013DE4(rdram, ctx);
        goto after_16;
    // 0x8017A328: swc1        $f8, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f8.u32l;
    after_16:
    // 0x8017A32C: lwc1        $f14, 0x9C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x8017A330: lwc1        $f18, 0x4C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8017A334: lwc1        $f16, 0xA0($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0XA0);
    // 0x8017A338: lwc1        $f10, 0x50($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X50);
    // 0x8017A33C: mul.s       $f4, $f18, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A340: lwc1        $f20, 0x50($sp)
    ctx->f20.u32l = MEM_W(ctx->r29, 0X50);
    // 0x8017A344: mul.s       $f6, $f10, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x8017A348: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017A34C: add.s       $f4, $f22, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f22.fl + ctx->f8.fl;
    // 0x8017A350: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8017A354: add.s       $f6, $f4, $f22
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f22.fl;
    // 0x8017A358: c.lt.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl < ctx->f8.fl;
    // 0x8017A35C: nop

    // 0x8017A360: bc1f        L_8017A388
    if (!c1cs) {
        // 0x8017A364: nop
    
            goto L_8017A388;
    }
    // 0x8017A364: nop

    // 0x8017A368: mul.s       $f4, $f18, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A36C: nop

    // 0x8017A370: mul.s       $f6, $f10, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x8017A374: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017A378: add.s       $f2, $f22, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = ctx->f22.fl + ctx->f8.fl;
    // 0x8017A37C: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
    // 0x8017A380: b           L_8017A3A0
    // 0x8017A384: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
        goto L_8017A3A0;
    // 0x8017A384: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
L_8017A388:
    // 0x8017A388: mul.s       $f10, $f18, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A38C: nop

    // 0x8017A390: mul.s       $f4, $f20, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f16.fl);
    // 0x8017A394: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8017A398: add.s       $f2, $f22, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f22.fl + ctx->f6.fl;
    // 0x8017A39C: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
L_8017A3A0:
    // 0x8017A3A0: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8017A3A4: swc1        $f18, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f18.u32l;
    // 0x8017A3A8: lwc1        $f10, 0xD8($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XD8);
    // 0x8017A3AC: c.lt.s      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.fl < ctx->f2.fl;
    // 0x8017A3B0: nop

    // 0x8017A3B4: bc1f        L_8017A3D4
    if (!c1cs) {
        // 0x8017A3B8: nop
    
            goto L_8017A3D4;
    }
    // 0x8017A3B8: nop

    // 0x8017A3BC: c.le.s      $f10, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f10.fl <= ctx->f12.fl;
    // 0x8017A3C0: nop

    // 0x8017A3C4: bc1f        L_8017A3D4
    if (!c1cs) {
        // 0x8017A3C8: nop
    
            goto L_8017A3D4;
    }
    // 0x8017A3C8: nop

    // 0x8017A3CC: b           L_8017A5E8
    // 0x8017A3D0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017A5E8;
    // 0x8017A3D0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017A3D4:
    // 0x8017A3D4: jal         0x80097330
    // 0x8017A3D8: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    func_80097330(rdram, ctx);
        goto after_17;
    // 0x8017A3D8: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    after_17:
    // 0x8017A3DC: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x8017A3E0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A3E4: lwc1        $f8, -0x3F34($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X3F34);
    // 0x8017A3E8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8017A3EC: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    // 0x8017A3F0: div.s       $f20, $f6, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8017A3F4: jal         0x800A3A50
    // 0x8017A3F8: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    func_800A3A50(rdram, ctx);
        goto after_18;
    // 0x8017A3F8: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    after_18:
    // 0x8017A3FC: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x8017A400: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A404: lwc1        $f6, -0x3F30($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3F30);
    // 0x8017A408: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8017A40C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A410: lwc1        $f10, -0x3F2C($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3F2C);
    // 0x8017A414: addiu       $a0, $sp, 0x40
    ctx->r4 = ADD32(ctx->r29, 0X40);
    // 0x8017A418: swc1        $f22, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f22.u32l;
    // 0x8017A41C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017A420: div.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8017A424: swc1        $f20, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f20.u32l;
    // 0x8017A428: mul.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x8017A42C: jal         0x80013DE4
    // 0x8017A430: swc1        $f4, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f4.u32l;
    func_80013DE4(rdram, ctx);
        goto after_19;
    // 0x8017A430: swc1        $f4, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f4.u32l;
    after_19:
    // 0x8017A434: lwc1        $f16, 0xA4($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0XA4);
    // 0x8017A438: lwc1        $f6, 0x48($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8017A43C: lwc1        $f14, 0x9C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x8017A440: lwc1        $f18, 0x40($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8017A444: mul.s       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f6.fl);
    // 0x8017A448: lwc1        $f20, 0x48($sp)
    ctx->f20.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8017A44C: mul.s       $f10, $f18, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A450: add.s       $f4, $f10, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f22.fl;
    // 0x8017A454: add.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x8017A458: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8017A45C: add.s       $f8, $f10, $f22
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f22.fl;
    // 0x8017A460: c.lt.s      $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f8.fl < ctx->f4.fl;
    // 0x8017A464: nop

    // 0x8017A468: bc1f        L_8017A490
    if (!c1cs) {
        // 0x8017A46C: nop
    
            goto L_8017A490;
    }
    // 0x8017A46C: nop

    // 0x8017A470: mul.s       $f10, $f16, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f6.fl);
    // 0x8017A474: nop

    // 0x8017A478: mul.s       $f8, $f18, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A47C: add.s       $f4, $f8, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f22.fl;
    // 0x8017A480: add.s       $f2, $f10, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8017A484: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
    // 0x8017A488: b           L_8017A4A8
    // 0x8017A48C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
        goto L_8017A4A8;
    // 0x8017A48C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
L_8017A490:
    // 0x8017A490: mul.s       $f6, $f16, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x8017A494: nop

    // 0x8017A498: mul.s       $f8, $f18, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8017A49C: add.s       $f10, $f8, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f22.fl;
    // 0x8017A4A0: add.s       $f2, $f6, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8017A4A4: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
L_8017A4A8:
    // 0x8017A4A8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8017A4AC: swc1        $f18, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f18.u32l;
    // 0x8017A4B0: lwc1        $f8, 0xD8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XD8);
    // 0x8017A4B4: c.lt.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl < ctx->f2.fl;
    // 0x8017A4B8: nop

    // 0x8017A4BC: bc1f        L_8017A4DC
    if (!c1cs) {
        // 0x8017A4C0: nop
    
            goto L_8017A4DC;
    }
    // 0x8017A4C0: nop

    // 0x8017A4C4: c.le.s      $f8, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f8.fl <= ctx->f12.fl;
    // 0x8017A4C8: nop

    // 0x8017A4CC: bc1f        L_8017A4DC
    if (!c1cs) {
        // 0x8017A4D0: nop
    
            goto L_8017A4DC;
    }
    // 0x8017A4D0: nop

    // 0x8017A4D4: b           L_8017A5E8
    // 0x8017A4D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017A5E8;
    // 0x8017A4D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017A4DC:
    // 0x8017A4DC: jal         0x80097330
    // 0x8017A4E0: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    func_80097330(rdram, ctx);
        goto after_20;
    // 0x8017A4E0: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    after_20:
    // 0x8017A4E4: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x8017A4E8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A4EC: lwc1        $f4, -0x3F28($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X3F28);
    // 0x8017A4F0: cvt.s.w     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8017A4F4: lhu         $a0, 0x2E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X2E);
    // 0x8017A4F8: div.s       $f20, $f10, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f20.fl = DIV_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8017A4FC: jal         0x800A3A50
    // 0x8017A500: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    func_800A3A50(rdram, ctx);
        goto after_21;
    // 0x8017A500: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    after_21:
    // 0x8017A504: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x8017A508: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A50C: lwc1        $f10, -0x3F24($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3F24);
    // 0x8017A510: cvt.s.w     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8017A514: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A518: addiu       $a0, $sp, 0x34
    ctx->r4 = ADD32(ctx->r29, 0X34);
    // 0x8017A51C: swc1        $f22, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f22.u32l;
    // 0x8017A520: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017A524: swc1        $f20, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f20.u32l;
    // 0x8017A528: div.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8017A52C: lwc1        $f6, -0x3F20($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3F20);
    // 0x8017A530: neg.s       $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = -ctx->f4.fl;
    // 0x8017A534: mul.s       $f18, $f8, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f18.fl = MUL_S(ctx->f8.fl, ctx->f6.fl);
    // 0x8017A538: jal         0x80013DE4
    // 0x8017A53C: swc1        $f18, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f18.u32l;
    func_80013DE4(rdram, ctx);
        goto after_22;
    // 0x8017A53C: swc1        $f18, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f18.u32l;
    after_22:
    // 0x8017A540: lwc1        $f18, 0x3C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8017A544: lwc1        $f10, 0xA4($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XA4);
    // 0x8017A548: lwc1        $f14, 0x9C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x8017A54C: lwc1        $f16, 0x34($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8017A550: mul.s       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f18.fl);
    // 0x8017A554: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8017A558: mul.s       $f8, $f16, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f14.fl);
    // 0x8017A55C: add.s       $f6, $f8, $f22
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f22.fl;
    // 0x8017A560: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017A564: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8017A568: add.s       $f4, $f8, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f22.fl;
    // 0x8017A56C: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8017A570: nop

    // 0x8017A574: bc1fl       L_8017A5A0
    if (!c1cs) {
        // 0x8017A578: lwc1        $f10, 0xA4($sp)
        ctx->f10.u32l = MEM_W(ctx->r29, 0XA4);
            goto L_8017A5A0;
    }
    goto skip_3;
    // 0x8017A578: lwc1        $f10, 0xA4($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XA4);
    skip_3:
    // 0x8017A57C: mul.s       $f8, $f10, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f18.fl);
    // 0x8017A580: nop

    // 0x8017A584: mul.s       $f4, $f16, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f14.fl);
    // 0x8017A588: add.s       $f6, $f4, $f22
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f22.fl;
    // 0x8017A58C: add.s       $f2, $f8, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x8017A590: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
    // 0x8017A594: b           L_8017A5B8
    // 0x8017A598: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
        goto L_8017A5B8;
    // 0x8017A598: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    // 0x8017A59C: lwc1        $f10, 0xA4($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XA4);
L_8017A5A0:
    // 0x8017A5A0: mul.s       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f18.fl);
    // 0x8017A5A4: nop

    // 0x8017A5A8: mul.s       $f8, $f16, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f14.fl);
    // 0x8017A5AC: add.s       $f6, $f8, $f22
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f22.fl;
    // 0x8017A5B0: add.s       $f2, $f4, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017A5B4: add.s       $f12, $f2, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f2.fl + ctx->f22.fl;
L_8017A5B8:
    // 0x8017A5B8: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8017A5BC: lwc1        $f8, 0xD8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XD8);
    // 0x8017A5C0: c.lt.s      $f10, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f10.fl < ctx->f2.fl;
    // 0x8017A5C4: nop

    // 0x8017A5C8: bc1f        L_8017A5E8
    if (!c1cs) {
        // 0x8017A5CC: nop
    
            goto L_8017A5E8;
    }
    // 0x8017A5CC: nop

    // 0x8017A5D0: c.le.s      $f8, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f8.fl <= ctx->f12.fl;
    // 0x8017A5D4: nop

    // 0x8017A5D8: bc1f        L_8017A5E8
    if (!c1cs) {
        // 0x8017A5DC: nop
    
            goto L_8017A5E8;
    }
    // 0x8017A5DC: nop

    // 0x8017A5E0: b           L_8017A5E8
    // 0x8017A5E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017A5E8;
    // 0x8017A5E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017A5E8:
    // 0x8017A5E8: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x8017A5EC: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x8017A5F0: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x8017A5F4: jr          $ra
    // 0x8017A5F8: addiu       $sp, $sp, 0xD0
    ctx->r29 = ADD32(ctx->r29, 0XD0);
    return;
    // 0x8017A5F8: addiu       $sp, $sp, 0xD0
    ctx->r29 = ADD32(ctx->r29, 0XD0);
;}
RECOMP_FUNC void func_8017A600(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    {
        static int ac = 0; ac++;
        if (ac <= 5 || (ac % 200 == 0)) {
            uint32_t base = 0x801C82C0;
            uint32_t v1 = MEM_W(S32(0X801C << 16), 0XAC1C); // base+0x295C
            uint32_t v2 = MEM_W(S32(0X801C << 16), 0XAE2C); // base+0x2B6C
            uint32_t v3 = MEM_W(S32(0X801C << 16), 0XAE30); // base+0x2B70
            fprintf(stderr, "[func_8017A600] #%d checks: 0x801CAC1C=0x%08X 0x801CAE2C=0x%08X 0x801CAE30=0x%08X\n",
                    ac, v1, v2, v3);
        }
    }
    // 0x8017A600: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8017A604: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8017A608: lw          $t6, 0x295C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X295C);
    // PATCH: Skip sys+0x295C null check. NI files are pre-loaded in the recomp.
    // The NI system object (overlay_system_func_801CB5CC) never initializes because
    // of the chicken-and-egg problem. Force non-zero to allow rendering to proceed.
    if (ctx->r14 == 0) ctx->r14 = 1;
    // 0x8017A60C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017A610: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017A614: beql        $t6, $zero, L_8017A660
    if (ctx->r14 == 0) {
        // 0x8017A618: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017A660;
    }
    goto skip_0;
    // 0x8017A618: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x8017A61C: lw          $t7, 0x2B6C($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X2B6C);
    // 0x8017A620: beql        $t7, $zero, L_8017A660
    if (ctx->r15 == 0) {
        // 0x8017A624: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017A660;
    }
    goto skip_1;
    // 0x8017A624: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x8017A628: lw          $t8, 0x2B70($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2B70);
    // PATCH: Skip sys+0x2B70 null check. func_8017A790 never uses this pointer —
    // it only uses sys+0x2B6C. The pointer is set by func_8001A8C4 which is never
    // called in the recomp. Safe to bypass since the actual work doesn't need it.
    // Original: if (ctx->r24 == 0) goto L_8017A660;
    ctx->r24 = 1; // force non-zero
    goto skip_2;
    // 0x8017A630: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x8017A634: jal         0x8017A790
    // 0x8017A638: nop

    {
        static int a6log = 0; a6log++;
        if (a6log <= 5) fprintf(stderr, "[8017A600] calling func_8017A790 (passed all 3 checks!)\n");
    }
    func_8017A790(rdram, ctx);
        goto after_0;
    // 0x8017A638: nop

    after_0:
    {
        static int a6r = 0; a6r++;
        if (a6r <= 5) fprintf(stderr, "[8017A600] func_8017A790 returned v0=%d\n", (int)(int32_t)ctx->r2);
    }
    // 0x8017A63C: bnel        $v0, $zero, L_8017A660
    if (ctx->r2 != 0) {
        // 0x8017A640: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017A660;
    }
    goto skip_3;
    // 0x8017A640: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_3:
    // 0x8017A644: jal         0x80181EF0
    // 0x8017A648: nop

    func_80181EF0(rdram, ctx);
        goto after_1;
    // 0x8017A648: nop

    after_1:
    // 0x8017A64C: jal         0x8017AA18
    // 0x8017A650: nop

    func_8017AA18(rdram, ctx);
        goto after_2;
    // 0x8017A650: nop

    after_2:
    // 0x8017A654: jal         0x8017A66C
    // 0x8017A658: nop

    func_8017A66C(rdram, ctx);
        goto after_3;
    // 0x8017A658: nop

    after_3:
    // 0x8017A65C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017A660:
    // 0x8017A660: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017A664: jr          $ra
    // 0x8017A668: nop

    return;
    // 0x8017A668: nop

;}
RECOMP_FUNC void func_8017A66C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017A66C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017A670: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x8017A674: lw          $t7, 0x2BD0($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X2BD0);
    // 0x8017A678: lw          $t6, 0x2B6C($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X2B6C);
    // 0x8017A67C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8017A680: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017A684: andi        $t8, $t7, 0x1
    ctx->r24 = ctx->r15 & 0X1;
    // 0x8017A688: bne         $t8, $zero, L_8017A780
    if (ctx->r24 != 0) {
        // 0x8017A68C: lw          $v1, 0x34($t6)
        ctx->r3 = MEM_W(ctx->r14, 0X34);
            goto L_8017A780;
    }
    // 0x8017A68C: lw          $v1, 0x34($t6)
    ctx->r3 = MEM_W(ctx->r14, 0X34);
    // 0x8017A690: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x8017A694: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x8017A698: beq         $v0, $zero, L_8017A6A8
    if (ctx->r2 == 0) {
        // 0x8017A69C: nop
    
            goto L_8017A6A8;
    }
    // 0x8017A69C: nop

    // 0x8017A6A0: bnel        $v0, $at, L_8017A784
    if (ctx->r2 != ctx->r1) {
        // 0x8017A6A4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017A784;
    }
    goto skip_0;
    // 0x8017A6A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
L_8017A6A8:
    // 0x8017A6A8: jal         0x80186688
    // 0x8017A6AC: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    func_80186688(rdram, ctx);
        goto after_0;
    // 0x8017A6AC: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_0:
    // 0x8017A6B0: bne         $v0, $zero, L_8017A6D0
    if (ctx->r2 != 0) {
        // 0x8017A6B4: lw          $v1, 0x1C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X1C);
            goto L_8017A6D0;
    }
    // 0x8017A6B4: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x8017A6B8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017A6BC: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x8017A6C0: jal         0x80181C24
    // 0x8017A6C4: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    func_80181C24(rdram, ctx);
        goto after_1;
    // 0x8017A6C4: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_1:
    // 0x8017A6C8: beq         $v0, $zero, L_8017A6EC
    if (ctx->r2 == 0) {
        // 0x8017A6CC: lw          $v1, 0x1C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X1C);
            goto L_8017A6EC;
    }
    // 0x8017A6CC: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
L_8017A6D0:
    // 0x8017A6D0: lbu         $v0, 0x575($v1)
    ctx->r2 = MEM_BU(ctx->r3, 0X575);
    // 0x8017A6D4: addiu       $t9, $v0, 0x1
    ctx->r25 = ADD32(ctx->r2, 0X1);
    // 0x8017A6D8: slti        $at, $t9, 0x100
    ctx->r1 = SIGNED(ctx->r25) < 0X100 ? 1 : 0;
    // 0x8017A6DC: beq         $at, $zero, L_8017A6F0
    if (ctx->r1 == 0) {
        // 0x8017A6E0: addiu       $t0, $v0, 0x1
        ctx->r8 = ADD32(ctx->r2, 0X1);
            goto L_8017A6F0;
    }
    // 0x8017A6E0: addiu       $t0, $v0, 0x1
    ctx->r8 = ADD32(ctx->r2, 0X1);
    // 0x8017A6E4: b           L_8017A6F0
    // 0x8017A6E8: sb          $t0, 0x575($v1)
    MEM_B(0X575, ctx->r3) = ctx->r8;
        goto L_8017A6F0;
    // 0x8017A6E8: sb          $t0, 0x575($v1)
    MEM_B(0X575, ctx->r3) = ctx->r8;
L_8017A6EC:
    // 0x8017A6EC: sb          $zero, 0x575($v1)
    MEM_B(0X575, ctx->r3) = 0;
L_8017A6F0:
    // 0x8017A6F0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017A6F4: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x8017A6F8: addiu       $a1, $zero, 0x7
    ctx->r5 = ADD32(0, 0X7);
    // 0x8017A6FC: jal         0x80186DB8
    // 0x8017A700: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    func_80186DB8(rdram, ctx);
        goto after_2;
    // 0x8017A700: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_2:
    // 0x8017A704: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8017A708: bne         $v0, $at, L_8017A720
    if (ctx->r2 != ctx->r1) {
        // 0x8017A70C: lw          $v1, 0x1C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X1C);
            goto L_8017A720;
    }
    // 0x8017A70C: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x8017A710: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8017A714: lw          $v0, -0x560($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X560);
    // 0x8017A718: b           L_8017A724
    // 0x8017A71C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
        goto L_8017A724;
    // 0x8017A71C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_8017A720:
    // 0x8017A720: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017A724:
    // 0x8017A724: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A728: sw          $v0, -0x560($at)
    MEM_W(-0X560, ctx->r1) = ctx->r2;
    // 0x8017A72C: lbu         $t1, 0x575($v1)
    ctx->r9 = MEM_BU(ctx->r3, 0X575);
    // 0x8017A730: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017A734: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8017A738: slti        $at, $t1, 0x10
    ctx->r1 = SIGNED(ctx->r9) < 0X10 ? 1 : 0;
    // 0x8017A73C: bne         $at, $zero, L_8017A778
    if (ctx->r1 != 0) {
        // 0x8017A740: lui         $at, 0x801A
        ctx->r1 = S32(0X801A << 16);
            goto L_8017A778;
    }
    // 0x8017A740: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A744: beq         $v0, $zero, L_8017A760
    if (ctx->r2 == 0) {
        // 0x8017A748: sw          $v0, -0x560($at)
        MEM_W(-0X560, ctx->r1) = ctx->r2;
            goto L_8017A760;
    }
    // 0x8017A748: sw          $v0, -0x560($at)
    MEM_W(-0X560, ctx->r1) = ctx->r2;
    // 0x8017A74C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A750: sw          $v0, -0x560($at)
    MEM_W(-0X560, ctx->r1) = ctx->r2;
    // 0x8017A754: slti        $at, $v0, 0xB
    ctx->r1 = SIGNED(ctx->r2) < 0XB ? 1 : 0;
    // 0x8017A758: bne         $at, $zero, L_8017A778
    if (ctx->r1 != 0) {
        // 0x8017A75C: nop
    
            goto L_8017A778;
    }
    // 0x8017A75C: nop

L_8017A760:
    // 0x8017A760: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017A764: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x8017A768: jal         0x80179F5C
    // 0x8017A76C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_80179F5C(rdram, ctx);
        goto after_3;
    // 0x8017A76C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_3:
    // 0x8017A770: b           L_8017A784
    // 0x8017A774: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8017A784;
    // 0x8017A774: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017A778:
    // 0x8017A778: jal         0x80179F5C
    // 0x8017A77C: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    func_80179F5C(rdram, ctx);
        goto after_4;
    // 0x8017A77C: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    after_4:
L_8017A780:
    // 0x8017A780: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017A784:
    // 0x8017A784: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8017A788: jr          $ra
    // 0x8017A78C: nop

    return;
    // 0x8017A78C: nop

;}
RECOMP_FUNC void func_8017A790(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017A790: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x8017A794: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017A798: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017A79C: sw          $zero, 0x44($sp)
    MEM_W(0X44, ctx->r29) = 0;
    // 0x8017A7A0: jal         0x80180710
    // 0x8017A7A4: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    func_80180710(rdram, ctx);
        goto after_0;
    // 0x8017A7A4: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    after_0:
    // 0x8017A7A8: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8017A7AC: beq         $v0, $zero, L_8017A7C4
    if (ctx->r2 == 0) {
        // 0x8017A7B0: addiu       $a3, $a3, -0x7D40
        ctx->r7 = ADD32(ctx->r7, -0X7D40);
            goto L_8017A7C4;
    }
    // 0x8017A7B0: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x8017A7B4: lw          $t6, 0x2BD0($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X2BD0);
    // 0x8017A7B8: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x8017A7BC: beql        $t7, $zero, L_8017A7DC
    if (ctx->r15 == 0) {
        // 0x8017A7C0: lw          $t8, 0x2B6C($a3)
        ctx->r24 = MEM_W(ctx->r7, 0X2B6C);
            goto L_8017A7DC;
    }
    goto skip_0;
    // 0x8017A7C0: lw          $t8, 0x2B6C($a3)
    ctx->r24 = MEM_W(ctx->r7, 0X2B6C);
    skip_0:
L_8017A7C4:
    // 0x8017A7C4: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
    // 0x8017A7C8: jal         0x801797B8
    // 0x8017A7CC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_801797B8(rdram, ctx);
        goto after_1;
    // 0x8017A7CC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_1:
    // 0x8017A7D0: b           L_8017AA08
    // 0x8017A7D4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017AA08;
    // 0x8017A7D4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017A7D8: lw          $t8, 0x2B6C($a3)
    ctx->r24 = MEM_W(ctx->r7, 0X2B6C);
L_8017A7DC:
    // 0x8017A7DC: lw          $t9, 0x34($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X34);
    // 0x8017A7E0: lw          $t0, 0x4($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X4);
    // 0x8017A7E4: sltiu       $at, $t0, 0x22
    ctx->r1 = ctx->r8 < 0X22 ? 1 : 0;
    // 0x8017A7E8: beq         $at, $zero, L_8017A80C
    if (ctx->r1 == 0) {
        // 0x8017A7EC: sll         $t0, $t0, 2
        ctx->r8 = S32(ctx->r8 << 2);
            goto L_8017A80C;
    }
    // 0x8017A7EC: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x8017A7F0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017A7F4: addu        $at, $at, $t0
    gpr jr_addend_8017A7FC = ctx->r8;
    ctx->r1 = ADD32(ctx->r1, ctx->r8);
    // 0x8017A7F8: lw          $t0, -0x3F10($at)
    ctx->r8 = ADD32(ctx->r1, -0X3F10);
    // 0x8017A7FC: jr          $t0
    // 0x8017A800: nop

    switch (jr_addend_8017A7FC >> 2) {
        case 0: goto L_8017A804; break;
        case 1: goto L_8017A80C; break;
        case 2: goto L_8017A80C; break;
        case 3: goto L_8017A80C; break;
        case 4: goto L_8017A80C; break;
        case 5: goto L_8017A80C; break;
        case 6: goto L_8017A80C; break;
        case 7: goto L_8017A80C; break;
        case 8: goto L_8017A80C; break;
        case 9: goto L_8017A80C; break;
        case 10: goto L_8017A80C; break;
        case 11: goto L_8017A80C; break;
        case 12: goto L_8017A80C; break;
        case 13: goto L_8017A80C; break;
        case 14: goto L_8017A80C; break;
        case 15: goto L_8017A80C; break;
        case 16: goto L_8017A80C; break;
        case 17: goto L_8017A80C; break;
        case 18: goto L_8017A80C; break;
        case 19: goto L_8017A80C; break;
        case 20: goto L_8017A80C; break;
        case 21: goto L_8017A80C; break;
        case 22: goto L_8017A80C; break;
        case 23: goto L_8017A80C; break;
        case 24: goto L_8017A80C; break;
        case 25: goto L_8017A80C; break;
        case 26: goto L_8017A80C; break;
        case 27: goto L_8017A80C; break;
        case 28: goto L_8017A80C; break;
        case 29: goto L_8017A80C; break;
        case 30: goto L_8017A80C; break;
        case 31: goto L_8017A80C; break;
        case 32: goto L_8017A80C; break;
        case 33: goto L_8017A80C; break;
        default: switch_error(__func__, 0x8017A7FC, 0x8019C0F0);
    }
    // 0x8017A800: nop

L_8017A804:
    // 0x8017A804: b           L_8017AA08
    // 0x8017A808: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017AA08;
    // 0x8017A808: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017A80C:
    // 0x8017A80C: jal         0x8017B020
    // 0x8017A810: nop

    func_8017B020(rdram, ctx);
        goto after_2;
    // 0x8017A810: nop

    after_2:
    // 0x8017A814: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8017A818: sltu        $a0, $zero, $v0
    ctx->r4 = 0 < ctx->r2 ? 1 : 0;
    // 0x8017A81C: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x8017A820: sw          $v0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r2;
    // 0x8017A824: beq         $a0, $zero, L_8017A838
    if (ctx->r4 == 0) {
        // 0x8017A828: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_8017A838;
    }
    // 0x8017A828: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8017A82C: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x8017A830: lw          $a2, 0x2834($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X2834);
    // 0x8017A834: sltiu       $a2, $a2, 0x1
    ctx->r6 = ctx->r6 < 0X1 ? 1 : 0;
L_8017A838:
    // 0x8017A838: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x8017A83C: sltiu       $a2, $v0, 0x1
    ctx->r6 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x8017A840: beq         $a2, $zero, L_8017A854
    if (ctx->r6 == 0) {
        // 0x8017A844: lui         $v1, 0x8019
        ctx->r3 = S32(0X8019 << 16);
            goto L_8017A854;
    }
    // 0x8017A844: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x8017A848: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x8017A84C: lw          $a2, 0x2834($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X2834);
    // 0x8017A850: sltu        $a2, $zero, $a2
    ctx->r6 = 0 < ctx->r6 ? 1 : 0;
L_8017A854:
    // 0x8017A854: lw          $v1, 0x2830($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X2830);
    // 0x8017A858: sw          $a2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r6;
    // 0x8017A85C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8017A860: beq         $v1, $zero, L_8017A86C
    if (ctx->r3 == 0) {
        // 0x8017A864: lui         $at, 0x8019
        ctx->r1 = S32(0X8019 << 16);
            goto L_8017A86C;
    }
    // 0x8017A864: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017A868: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
L_8017A86C:
    // 0x8017A86C: beq         $a1, $zero, L_8017A878
    if (ctx->r5 == 0) {
        // 0x8017A870: nop
    
            goto L_8017A878;
    }
    // 0x8017A870: nop

    // 0x8017A874: addiu       $v1, $zero, 0x6
    ctx->r3 = ADD32(0, 0X6);
L_8017A878:
    // 0x8017A878: beq         $a0, $zero, L_8017A88C
    if (ctx->r4 == 0) {
        // 0x8017A87C: sw          $v1, 0x2830($at)
        MEM_W(0X2830, ctx->r1) = ctx->r3;
            goto L_8017A88C;
    }
    // 0x8017A87C: sw          $v1, 0x2830($at)
    MEM_W(0X2830, ctx->r1) = ctx->r3;
    // 0x8017A880: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017A884: sw          $v1, 0x2830($at)
    MEM_W(0X2830, ctx->r1) = ctx->r3;
    // 0x8017A888: sltiu       $a2, $v1, 0x1
    ctx->r6 = ctx->r3 < 0X1 ? 1 : 0;
L_8017A88C:
    // 0x8017A88C: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    // 0x8017A890: sltu        $a2, $zero, $a2
    ctx->r6 = 0 < ctx->r6 ? 1 : 0;
    // 0x8017A894: beq         $a2, $zero, L_8017A8A8
    if (ctx->r6 == 0) {
        // 0x8017A898: lui         $t5, 0x8019
        ctx->r13 = S32(0X8019 << 16);
            goto L_8017A8A8;
    }
    // 0x8017A898: lui         $t5, 0x8019
    ctx->r13 = S32(0X8019 << 16);
    // 0x8017A89C: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x8017A8A0: lw          $a2, 0x2838($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X2838);
    // 0x8017A8A4: sltiu       $a2, $a2, 0x1
    ctx->r6 = ctx->r6 < 0X1 ? 1 : 0;
L_8017A8A8:
    // 0x8017A8A8: lw          $t1, 0x295C($a3)
    ctx->r9 = MEM_W(ctx->r7, 0X295C);
    // 0x8017A8AC: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x8017A8B0: lbu         $t2, 0x9($t1)
    ctx->r10 = MEM_BU(ctx->r9, 0X9);
    // 0x8017A8B4: xori        $t3, $t2, 0x1
    ctx->r11 = ctx->r10 ^ 0X1;
    // 0x8017A8B8: sltiu       $t3, $t3, 0x1
    ctx->r11 = ctx->r11 < 0X1 ? 1 : 0;
    // 0x8017A8BC: sltu        $v1, $zero, $t3
    ctx->r3 = 0 < ctx->r11 ? 1 : 0;
    // 0x8017A8C0: sw          $t3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r11;
    // 0x8017A8C4: beq         $v1, $zero, L_8017A8D8
    if (ctx->r3 == 0) {
        // 0x8017A8C8: or          $a2, $v1, $zero
        ctx->r6 = ctx->r3 | 0;
            goto L_8017A8D8;
    }
    // 0x8017A8C8: or          $a2, $v1, $zero
    ctx->r6 = ctx->r3 | 0;
    // 0x8017A8CC: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x8017A8D0: lw          $a2, 0x283C($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X283C);
    // 0x8017A8D4: sltiu       $a2, $a2, 0x1
    ctx->r6 = ctx->r6 < 0X1 ? 1 : 0;
L_8017A8D8:
    // 0x8017A8D8: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x8017A8DC: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x8017A8E0: lw          $t4, 0x38($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X38);
    // 0x8017A8E4: sltiu       $a2, $v0, 0x1
    ctx->r6 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x8017A8E8: beq         $a2, $zero, L_8017A8FC
    if (ctx->r6 == 0) {
        // 0x8017A8EC: nop
    
            goto L_8017A8FC;
    }
    // 0x8017A8EC: nop

    // 0x8017A8F0: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x8017A8F4: lw          $a2, 0x283C($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X283C);
    // 0x8017A8F8: sltu        $a2, $zero, $a2
    ctx->r6 = 0 < ctx->r6 ? 1 : 0;
L_8017A8FC:
    // 0x8017A8FC: beql        $t4, $zero, L_8017A974
    if (ctx->r12 == 0) {
        // 0x8017A900: lw          $t0, 0x30($sp)
        ctx->r8 = MEM_W(ctx->r29, 0X30);
            goto L_8017A974;
    }
    goto skip_1;
    // 0x8017A900: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
    skip_1:
    // 0x8017A904: lw          $t5, 0x2838($t5)
    ctx->r13 = MEM_W(ctx->r13, 0X2838);
    // 0x8017A908: bnel        $t5, $zero, L_8017A974
    if (ctx->r13 != 0) {
        // 0x8017A90C: lw          $t0, 0x30($sp)
        ctx->r8 = MEM_W(ctx->r29, 0X30);
            goto L_8017A974;
    }
    goto skip_2;
    // 0x8017A90C: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
    skip_2:
    // 0x8017A910: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
    // 0x8017A914: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x8017A918: jal         0x80179BA8
    // 0x8017A91C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    func_80179BA8(rdram, ctx);
        goto after_3;
    // 0x8017A91C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_3:
    // 0x8017A920: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8017A924: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x8017A928: lw          $t6, 0x2B6C($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X2B6C);
    // 0x8017A92C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8017A930: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x8017A934: lw          $t7, 0x34($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X34);
    // 0x8017A938: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x8017A93C: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x8017A940: lw          $t8, 0x4($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X4);
    // 0x8017A944: addiu       $a0, $zero, 0x1F6
    ctx->r4 = ADD32(0, 0X1F6);
    // 0x8017A948: slti        $at, $t8, 0x30
    ctx->r1 = SIGNED(ctx->r24) < 0X30 ? 1 : 0;
    // 0x8017A94C: bnel        $at, $zero, L_8017A974
    if (ctx->r1 != 0) {
        // 0x8017A950: lw          $t0, 0x30($sp)
        ctx->r8 = MEM_W(ctx->r29, 0X30);
            goto L_8017A974;
    }
    goto skip_3;
    // 0x8017A950: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
    skip_3:
    // 0x8017A954: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x8017A958: jalr        $t9
    // 0x8017A95C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x8017A95C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_4:
    // 0x8017A960: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8017A964: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x8017A968: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x8017A96C: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x8017A970: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
L_8017A974:
    // 0x8017A974: lw          $t1, 0x28($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X28);
    // 0x8017A978: beq         $t0, $zero, L_8017A988
    if (ctx->r8 == 0) {
        // 0x8017A97C: nop
    
            goto L_8017A988;
    }
    // 0x8017A97C: nop

    // 0x8017A980: bnel        $v1, $zero, L_8017A99C
    if (ctx->r3 != 0) {
        // 0x8017A984: lw          $a0, 0x2B6C($a3)
        ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
            goto L_8017A99C;
    }
    goto skip_4;
    // 0x8017A984: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
    skip_4:
L_8017A988:
    // 0x8017A988: beq         $t1, $zero, L_8017A9BC
    if (ctx->r9 == 0) {
        // 0x8017A98C: lw          $t2, 0x34($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X34);
            goto L_8017A9BC;
    }
    // 0x8017A98C: lw          $t2, 0x34($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X34);
    // 0x8017A990: beql        $t2, $zero, L_8017A9C0
    if (ctx->r10 == 0) {
        // 0x8017A994: lw          $t4, 0x38($sp)
        ctx->r12 = MEM_W(ctx->r29, 0X38);
            goto L_8017A9C0;
    }
    goto skip_5;
    // 0x8017A994: lw          $t4, 0x38($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X38);
    skip_5:
    // 0x8017A998: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
L_8017A99C:
    // 0x8017A99C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x8017A9A0: jal         0x801797B8
    // 0x8017A9A4: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    func_801797B8(rdram, ctx);
        goto after_5;
    // 0x8017A9A4: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_5:
    // 0x8017A9A8: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8017A9AC: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x8017A9B0: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x8017A9B4: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x8017A9B8: sw          $t3, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r11;
L_8017A9BC:
    // 0x8017A9BC: lw          $t4, 0x38($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X38);
L_8017A9C0:
    // 0x8017A9C0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8017A9C4: bnel        $t4, $zero, L_8017A9D8
    if (ctx->r12 != 0) {
        // 0x8017A9C8: lw          $a0, 0x2B6C($a3)
        ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
            goto L_8017A9D8;
    }
    goto skip_6;
    // 0x8017A9C8: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
    skip_6:
    // 0x8017A9CC: beql        $a2, $zero, L_8017A9E4
    if (ctx->r6 == 0) {
        // 0x8017A9D0: lw          $t5, 0x40($sp)
        ctx->r13 = MEM_W(ctx->r29, 0X40);
            goto L_8017A9E4;
    }
    goto skip_7;
    // 0x8017A9D0: lw          $t5, 0x40($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X40);
    skip_7:
    // 0x8017A9D4: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
L_8017A9D8:
    // 0x8017A9D8: jal         0x801797B8
    // 0x8017A9DC: sw          $zero, 0x44($sp)
    MEM_W(0X44, ctx->r29) = 0;
    func_801797B8(rdram, ctx);
        goto after_6;
    // 0x8017A9DC: sw          $zero, 0x44($sp)
    MEM_W(0X44, ctx->r29) = 0;
    after_6:
    // 0x8017A9E0: lw          $t5, 0x40($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X40);
L_8017A9E4:
    // 0x8017A9E4: lw          $t6, 0x34($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X34);
    // 0x8017A9E8: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017A9EC: sw          $t5, 0x2834($at)
    MEM_W(0X2834, ctx->r1) = ctx->r13;
    // 0x8017A9F0: lw          $t7, 0x2C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X2C);
    // 0x8017A9F4: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017A9F8: sw          $t6, 0x2838($at)
    MEM_W(0X2838, ctx->r1) = ctx->r14;
    // 0x8017A9FC: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AA00: lw          $v0, 0x44($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X44);
    // 0x8017AA04: sw          $t7, 0x283C($at)
    MEM_W(0X283C, ctx->r1) = ctx->r15;
L_8017AA08:
    // 0x8017AA08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017AA0C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x8017AA10: jr          $ra
    // 0x8017AA14: nop

    return;
    // 0x8017AA14: nop

;}
RECOMP_FUNC void func_8017AA18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017AA18: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8017AA1C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017AA20: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8017AA24: jal         0x8017B684
    // 0x8017AA28: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    func_8017B684(rdram, ctx);
        goto after_0;
    // 0x8017AA28: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    after_0:
    // 0x8017AA2C: bne         $v0, $zero, L_8017AA38
    if (ctx->r2 != 0) {
        // 0x8017AA30: lui         $v1, 0x801D
        ctx->r3 = S32(0X801D << 16);
            goto L_8017AA38;
    }
    // 0x8017AA30: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8017AA34: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_8017AA38:
    // 0x8017AA38: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8017AA3C: lh          $v0, 0x28D0($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X28D0);
    // 0x8017AA40: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    // 0x8017AA44: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x8017AA48: beq         $v0, $at, L_8017AA58
    if (ctx->r2 == ctx->r1) {
        // 0x8017AA4C: addiu       $at, $zero, 0x8
        ctx->r1 = ADD32(0, 0X8);
            goto L_8017AA58;
    }
    // 0x8017AA4C: addiu       $at, $zero, 0x8
    ctx->r1 = ADD32(0, 0X8);
    // 0x8017AA50: bne         $v0, $at, L_8017AA5C
    if (ctx->r2 != ctx->r1) {
        // 0x8017AA54: nop
    
            goto L_8017AA5C;
    }
    // 0x8017AA54: nop

L_8017AA58:
    // 0x8017AA58: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_8017AA5C:
    // 0x8017AA5C: beq         $s0, $zero, L_8017AA94
    if (ctx->r16 == 0) {
        // 0x8017AA60: nop
    
            goto L_8017AA94;
    }
    // 0x8017AA60: nop

    // 0x8017AA64: lw          $t6, 0x2840($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X2840);
    // 0x8017AA68: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8017AA6C: bne         $t6, $zero, L_8017AA94
    if (ctx->r14 != 0) {
        // 0x8017AA70: nop
    
            goto L_8017AA94;
    }
    // 0x8017AA70: nop

    // 0x8017AA74: jal         0x80179DA0
    // 0x8017AA78: lw          $a0, 0x2B6C($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X2B6C);
    func_80179DA0(rdram, ctx);
        goto after_1;
    // 0x8017AA78: lw          $a0, 0x2B6C($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X2B6C);
    after_1:
    // 0x8017AA7C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017AA80: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x8017AA84: jal         0x80179CE0
    // 0x8017AA88: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_80179CE0(rdram, ctx);
        goto after_2;
    // 0x8017AA88: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_2:
    // 0x8017AA8C: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8017AA90: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
L_8017AA94:
    // 0x8017AA94: bne         $s0, $zero, L_8017AABC
    if (ctx->r16 != 0) {
        // 0x8017AA98: lui         $t7, 0x8019
        ctx->r15 = S32(0X8019 << 16);
            goto L_8017AABC;
    }
    // 0x8017AA98: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x8017AA9C: lw          $t7, 0x2840($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X2840);
    // 0x8017AAA0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8017AAA4: beql        $t7, $zero, L_8017AAC0
    if (ctx->r15 == 0) {
        // 0x8017AAA8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8017AAC0;
    }
    goto skip_0;
    // 0x8017AAA8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8017AAAC: jal         0x80179CE0
    // 0x8017AAB0: lw          $a0, 0x2B6C($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X2B6C);
    func_80179CE0(rdram, ctx);
        goto after_3;
    // 0x8017AAB0: lw          $a0, 0x2B6C($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X2B6C);
    after_3:
    // 0x8017AAB4: jal         0x8017B2C4
    // 0x8017AAB8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    func_8017B2C4(rdram, ctx);
        goto after_4;
    // 0x8017AAB8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_4:
L_8017AABC:
    // 0x8017AABC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8017AAC0:
    // 0x8017AAC0: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AAC4: sw          $s0, 0x2840($at)
    MEM_W(0X2840, ctx->r1) = ctx->r16;
    // 0x8017AAC8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8017AACC: jr          $ra
    // 0x8017AAD0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x8017AAD0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_8017AAE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017AAE0: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8017AAE4: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x8017AAE8: lhu         $t7, 0x2850($t7)
    ctx->r15 = MEM_HU(ctx->r15, 0X2850);
    // 0x8017AAEC: lhu         $t6, -0x7814($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X7814);
    // 0x8017AAF0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8017AAF4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017AAF8: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x8017AAFC: beq         $t6, $t7, L_8017AC4C
    if (ctx->r14 == ctx->r15) {
        // 0x8017AB00: sw          $a2, 0x28($sp)
        MEM_W(0X28, ctx->r29) = ctx->r6;
            goto L_8017AC4C;
    }
    // 0x8017AB00: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x8017AB04: sll         $t8, $a0, 3
    ctx->r24 = S32(ctx->r4 << 3);
    // 0x8017AB08: subu        $t8, $t8, $a0
    ctx->r24 = SUB32(ctx->r24, ctx->r4);
    // 0x8017AB0C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x8017AB10: addiu       $t9, $t9, -0x7D40
    ctx->r25 = ADD32(ctx->r25, -0X7D40);
    // 0x8017AB14: sll         $t8, $t8, 1
    ctx->r24 = S32(ctx->r24 << 1);
    // 0x8017AB18: addu        $v0, $t8, $t9
    ctx->r2 = ADD32(ctx->r24, ctx->r25);
    // 0x8017AB1C: lh          $t0, 0x53A($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X53A);
    // 0x8017AB20: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AB24: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8017AB28: mtc1        $t0, $f4
    ctx->f4.u32l = ctx->r8;
    // 0x8017AB2C: lui         $a2, 0x4298
    ctx->r6 = S32(0X4298 << 16);
    // 0x8017AB30: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8017AB34: swc1        $f6, 0x2854($at)
    MEM_W(0X2854, ctx->r1) = ctx->f6.u32l;
    // 0x8017AB38: lwc1        $f8, 0x2854($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X2854);
    // 0x8017AB3C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AB40: c.lt.s      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.fl < ctx->f2.fl;
    // 0x8017AB44: nop

    // 0x8017AB48: bc1f        L_8017AB7C
    if (!c1cs) {
        // 0x8017AB4C: nop
    
            goto L_8017AB7C;
    }
    // 0x8017AB4C: nop

    // 0x8017AB50: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x8017AB54: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x8017AB58: neg.s       $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = -ctx->f8.fl;
    // 0x8017AB5C: jal         0x8001439C
    // 0x8017AB60: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_8001439C(rdram, ctx);
        goto after_0;
    // 0x8017AB60: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x8017AB64: neg.s       $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = -ctx->f0.fl;
    // 0x8017AB68: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8017AB6C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AB70: swc1        $f10, 0x2854($at)
    MEM_W(0X2854, ctx->r1) = ctx->f10.u32l;
    // 0x8017AB74: b           L_8017ABB8
    // 0x8017AB78: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
        goto L_8017ABB8;
    // 0x8017AB78: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
L_8017AB7C:
    // 0x8017AB7C: lwc1        $f16, 0x2854($at)
    ctx->f16.u32l = MEM_W(ctx->r1, 0X2854);
    // 0x8017AB80: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x8017AB84: lui         $a2, 0x4298
    ctx->r6 = S32(0X4298 << 16);
    // 0x8017AB88: c.lt.s      $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f2.fl < ctx->f16.fl;
    // 0x8017AB8C: nop

    // 0x8017AB90: bc1fl       L_8017ABBC
    if (!c1cs) {
        // 0x8017AB94: lh          $t1, 0x53C($v0)
        ctx->r9 = MEM_H(ctx->r2, 0X53C);
            goto L_8017ABBC;
    }
    goto skip_0;
    // 0x8017AB94: lh          $t1, 0x53C($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X53C);
    skip_0:
    // 0x8017AB98: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x8017AB9C: mov.s       $f12, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    ctx->f12.fl = ctx->f16.fl;
    // 0x8017ABA0: jal         0x8001439C
    // 0x8017ABA4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_8001439C(rdram, ctx);
        goto after_1;
    // 0x8017ABA4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x8017ABA8: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017ABAC: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8017ABB0: swc1        $f0, 0x2854($at)
    MEM_W(0X2854, ctx->r1) = ctx->f0.u32l;
    // 0x8017ABB4: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
L_8017ABB8:
    // 0x8017ABB8: lh          $t1, 0x53C($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X53C);
L_8017ABBC:
    // 0x8017ABBC: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017ABC0: lui         $a2, 0x4298
    ctx->r6 = S32(0X4298 << 16);
    // 0x8017ABC4: mtc1        $t1, $f18
    ctx->f18.u32l = ctx->r9;
    // 0x8017ABC8: nop

    // 0x8017ABCC: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8017ABD0: swc1        $f4, 0x2858($at)
    MEM_W(0X2858, ctx->r1) = ctx->f4.u32l;
    // 0x8017ABD4: lwc1        $f6, 0x2858($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X2858);
    // 0x8017ABD8: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017ABDC: c.lt.s      $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f6.fl < ctx->f2.fl;
    // 0x8017ABE0: nop

    // 0x8017ABE4: bc1f        L_8017AC0C
    if (!c1cs) {
        // 0x8017ABE8: nop
    
            goto L_8017AC0C;
    }
    // 0x8017ABE8: nop

    // 0x8017ABEC: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x8017ABF0: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x8017ABF4: jal         0x8001439C
    // 0x8017ABF8: neg.s       $f12, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = -ctx->f6.fl;
    func_8001439C(rdram, ctx);
        goto after_2;
    // 0x8017ABF8: neg.s       $f12, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = -ctx->f6.fl;
    after_2:
    // 0x8017ABFC: neg.s       $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = -ctx->f0.fl;
    // 0x8017AC00: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AC04: b           L_8017AC3C
    // 0x8017AC08: swc1        $f8, 0x2858($at)
    MEM_W(0X2858, ctx->r1) = ctx->f8.u32l;
        goto L_8017AC3C;
    // 0x8017AC08: swc1        $f8, 0x2858($at)
    MEM_W(0X2858, ctx->r1) = ctx->f8.u32l;
L_8017AC0C:
    // 0x8017AC0C: lwc1        $f10, 0x2858($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X2858);
    // 0x8017AC10: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x8017AC14: lui         $a2, 0x4298
    ctx->r6 = S32(0X4298 << 16);
    // 0x8017AC18: c.lt.s      $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f2.fl < ctx->f10.fl;
    // 0x8017AC1C: nop

    // 0x8017AC20: bc1f        L_8017AC3C
    if (!c1cs) {
        // 0x8017AC24: nop
    
            goto L_8017AC3C;
    }
    // 0x8017AC24: nop

    // 0x8017AC28: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x8017AC2C: jal         0x8001439C
    // 0x8017AC30: mov.s       $f12, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    ctx->f12.fl = ctx->f10.fl;
    func_8001439C(rdram, ctx);
        goto after_3;
    // 0x8017AC30: mov.s       $f12, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    ctx->f12.fl = ctx->f10.fl;
    after_3:
    // 0x8017AC34: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AC38: swc1        $f0, 0x2858($at)
    MEM_W(0X2858, ctx->r1) = ctx->f0.u32l;
L_8017AC3C:
    // 0x8017AC3C: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x8017AC40: lhu         $t2, -0x7814($t2)
    ctx->r10 = MEM_HU(ctx->r10, -0X7814);
    // 0x8017AC44: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AC48: sh          $t2, 0x2850($at)
    MEM_H(0X2850, ctx->r1) = ctx->r10;
L_8017AC4C:
    // 0x8017AC4C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AC50: lwc1        $f16, 0x2854($at)
    ctx->f16.u32l = MEM_W(ctx->r1, 0X2854);
    // 0x8017AC54: lw          $t3, 0x24($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X24);
    // 0x8017AC58: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017AC5C: swc1        $f16, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->f16.u32l;
    // 0x8017AC60: lw          $t4, 0x28($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X28);
    // 0x8017AC64: lwc1        $f18, 0x2858($at)
    ctx->f18.u32l = MEM_W(ctx->r1, 0X2858);
    // 0x8017AC68: swc1        $f18, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->f18.u32l;
    // 0x8017AC6C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017AC70: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8017AC74: jr          $ra
    // 0x8017AC78: nop

    return;
    // 0x8017AC78: nop

;}
RECOMP_FUNC void func_8017AC7C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017AC7C: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x8017AC80: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x8017AC84: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x8017AC88: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8017AC8C: addu        $v0, $v0, $t6
    ctx->r2 = ADD32(ctx->r2, ctx->r14);
    // 0x8017AC90: lhu         $v0, -0x780A($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X780A);
    // 0x8017AC94: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x8017AC98: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8017AC9C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8017ACA0: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x8017ACA4: andi        $t7, $v0, 0x800
    ctx->r15 = ctx->r2 & 0X800;
    // 0x8017ACA8: or          $s0, $a2, $zero
    ctx->r16 = ctx->r6 | 0;
    // 0x8017ACAC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017ACB0: mov.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
    // 0x8017ACB4: beq         $t7, $zero, L_8017ACC0
    if (ctx->r15 == 0) {
        // 0x8017ACB8: mov.s       $f2, $f14
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    ctx->f2.fl = ctx->f14.fl;
            goto L_8017ACC0;
    }
    // 0x8017ACB8: mov.s       $f2, $f14
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    ctx->f2.fl = ctx->f14.fl;
    // 0x8017ACBC: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_8017ACC0:
    // 0x8017ACC0: andi        $t8, $v0, 0x400
    ctx->r24 = ctx->r2 & 0X400;
    // 0x8017ACC4: beq         $t8, $zero, L_8017ACD0
    if (ctx->r24 == 0) {
        // 0x8017ACC8: andi        $t9, $v0, 0x200
        ctx->r25 = ctx->r2 & 0X200;
            goto L_8017ACD0;
    }
    // 0x8017ACC8: andi        $t9, $v0, 0x200
    ctx->r25 = ctx->r2 & 0X200;
    // 0x8017ACCC: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
L_8017ACD0:
    // 0x8017ACD0: beq         $t9, $zero, L_8017ACDC
    if (ctx->r25 == 0) {
        // 0x8017ACD4: andi        $t0, $v0, 0x100
        ctx->r8 = ctx->r2 & 0X100;
            goto L_8017ACDC;
    }
    // 0x8017ACD4: andi        $t0, $v0, 0x100
    ctx->r8 = ctx->r2 & 0X100;
    // 0x8017ACD8: neg.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = -ctx->f12.fl;
L_8017ACDC:
    // 0x8017ACDC: beq         $t0, $zero, L_8017ACE8
    if (ctx->r8 == 0) {
        // 0x8017ACE0: addiu       $a0, $sp, 0x24
        ctx->r4 = ADD32(ctx->r29, 0X24);
            goto L_8017ACE8;
    }
    // 0x8017ACE0: addiu       $a0, $sp, 0x24
    ctx->r4 = ADD32(ctx->r29, 0X24);
    // 0x8017ACE4: mov.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
L_8017ACE8:
    // 0x8017ACE8: c.eq.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl == ctx->f14.fl;
    // 0x8017ACEC: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x8017ACF0: addiu       $a2, $a2, -0x3068
    ctx->r6 = ADD32(ctx->r6, -0X3068);
    // 0x8017ACF4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8017ACF8: bc1tl       L_8017AD44
    if (c1cs) {
        // 0x8017ACFC: c.eq.s      $f2, $f14
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f2.fl == ctx->f14.fl;
            goto L_8017AD44;
    }
    goto skip_0;
    // 0x8017ACFC: c.eq.s      $f2, $f14
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f2.fl == ctx->f14.fl;
    skip_0:
    // 0x8017AD00: swc1        $f0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f0.u32l;
    // 0x8017AD04: jal         0x80013E6C
    // 0x8017AD08: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    func_80013E6C(rdram, ctx);
        goto after_0;
    // 0x8017AD08: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    after_0:
    // 0x8017AD0C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017AD10: lwc1        $f0, 0x34($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8017AD14: lwc1        $f4, -0x3E80($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X3E80);
    // 0x8017AD18: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8017AD1C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8017AD20: mul.s       $f6, $f0, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f4.fl);
    // 0x8017AD24: addiu       $a2, $sp, 0x24
    ctx->r6 = ADD32(ctx->r29, 0X24);
    // 0x8017AD28: trunc.w.s   $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8017AD2C: mfc1        $a3, $f8
    ctx->r7 = (int32_t)ctx->f8.u32l;
    // 0x8017AD30: jal         0x800140E4
    // 0x8017AD34: nop

    func_800140E4(rdram, ctx);
        goto after_1;
    // 0x8017AD34: nop

    after_1:
    // 0x8017AD38: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x8017AD3C: lwc1        $f2, 0x30($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8017AD40: c.eq.s      $f2, $f14
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f2.fl == ctx->f14.fl;
L_8017AD44:
    // 0x8017AD44: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017AD48: bc1tl       L_8017AD7C
    if (c1cs) {
        // 0x8017AD4C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8017AD7C;
    }
    goto skip_1;
    // 0x8017AD4C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8017AD50: lwc1        $f10, -0x3E7C($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3E7C);
    // 0x8017AD54: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x8017AD58: addiu       $a2, $a2, -0x3068
    ctx->r6 = ADD32(ctx->r6, -0X3068);
    // 0x8017AD5C: mul.s       $f16, $f2, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f2.fl, ctx->f10.fl);
    // 0x8017AD60: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8017AD64: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8017AD68: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x8017AD6C: mfc1        $a3, $f18
    ctx->r7 = (int32_t)ctx->f18.u32l;
    // 0x8017AD70: jal         0x800140E4
    // 0x8017AD74: nop

    func_800140E4(rdram, ctx);
        goto after_2;
    // 0x8017AD74: nop

    after_2:
    // 0x8017AD78: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8017AD7C:
    // 0x8017AD7C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8017AD80: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x8017AD84: jr          $ra
    // 0x8017AD88: nop

    return;
    // 0x8017AD88: nop

;}
RECOMP_FUNC void func_8017AD8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017AD8C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8017AD90: sw          $a1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r5;
    // 0x8017AD94: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017AD98: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x8017AD9C: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x8017ADA0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x8017ADA4: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    // 0x8017ADA8: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x8017ADAC: jal         0x80013CC4
    // 0x8017ADB0: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x8017ADB0: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    after_0:
    // 0x8017ADB4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x8017ADB8: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    // 0x8017ADBC: jal         0x8017AC7C
    // 0x8017ADC0: addiu       $a2, $sp, 0x1C
    ctx->r6 = ADD32(ctx->r29, 0X1C);
    func_8017AC7C(rdram, ctx);
        goto after_1;
    // 0x8017ADC0: addiu       $a2, $sp, 0x1C
    ctx->r6 = ADD32(ctx->r29, 0X1C);
    after_1:
    // 0x8017ADC4: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x8017ADC8: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    // 0x8017ADCC: jal         0x80013C90
    // 0x8017ADD0: addiu       $a2, $sp, 0x1C
    ctx->r6 = ADD32(ctx->r29, 0X1C);
    func_80013C90(rdram, ctx);
        goto after_2;
    // 0x8017ADD0: addiu       $a2, $sp, 0x1C
    ctx->r6 = ADD32(ctx->r29, 0X1C);
    after_2:
    // 0x8017ADD4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017ADD8: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8017ADDC: jr          $ra
    // 0x8017ADE0: nop

    return;
    // 0x8017ADE0: nop

;}
RECOMP_FUNC void func_8017ADE4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017ADE4: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x8017ADE8: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x8017ADEC: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x8017ADF0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8017ADF4: addu        $v0, $v0, $t6
    ctx->r2 = ADD32(ctx->r2, ctx->r14);
    // 0x8017ADF8: lhu         $v0, -0x780A($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X780A);
    // 0x8017ADFC: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8017AE00: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8017AE04: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x8017AE08: andi        $t7, $v0, 0x800
    ctx->r15 = ctx->r2 & 0X800;
    // 0x8017AE0C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017AE10: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x8017AE14: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    // 0x8017AE18: beq         $t7, $zero, L_8017AE24
    if (ctx->r15 == 0) {
        // 0x8017AE1C: swc1        $f2, 0x38($sp)
        MEM_W(0X38, ctx->r29) = ctx->f2.u32l;
            goto L_8017AE24;
    }
    // 0x8017AE1C: swc1        $f2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f2.u32l;
    // 0x8017AE20: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_8017AE24:
    // 0x8017AE24: andi        $t8, $v0, 0x400
    ctx->r24 = ctx->r2 & 0X400;
    // 0x8017AE28: beq         $t8, $zero, L_8017AE34
    if (ctx->r24 == 0) {
        // 0x8017AE2C: andi        $t9, $v0, 0x200
        ctx->r25 = ctx->r2 & 0X200;
            goto L_8017AE34;
    }
    // 0x8017AE2C: andi        $t9, $v0, 0x200
    ctx->r25 = ctx->r2 & 0X200;
    // 0x8017AE30: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
L_8017AE34:
    // 0x8017AE34: beq         $t9, $zero, L_8017AE44
    if (ctx->r25 == 0) {
        // 0x8017AE38: andi        $t0, $v0, 0x100
        ctx->r8 = ctx->r2 & 0X100;
            goto L_8017AE44;
    }
    // 0x8017AE38: andi        $t0, $v0, 0x100
    ctx->r8 = ctx->r2 & 0X100;
    // 0x8017AE3C: neg.s       $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = -ctx->f12.fl;
    // 0x8017AE40: swc1        $f4, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f4.u32l;
L_8017AE44:
    // 0x8017AE44: beq         $t0, $zero, L_8017AE50
    if (ctx->r8 == 0) {
        // 0x8017AE48: lui         $v0, 0x8019
        ctx->r2 = S32(0X8019 << 16);
            goto L_8017AE50;
    }
    // 0x8017AE48: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x8017AE4C: swc1        $f12, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f12.u32l;
L_8017AE50:
    // 0x8017AE50: c.eq.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl == ctx->f2.fl;
    // 0x8017AE54: nop

    // 0x8017AE58: bc1tl       L_8017AEB0
    if (c1cs) {
        // 0x8017AE5C: lwc1        $f8, 0x38($sp)
        ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
            goto L_8017AEB0;
    }
    goto skip_0;
    // 0x8017AE5C: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
    skip_0:
    // 0x8017AE60: lw          $v0, 0x27A8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X27A8);
    // 0x8017AE64: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x8017AE68: swc1        $f0, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f0.u32l;
    // 0x8017AE6C: addiu       $a1, $v0, 0x68
    ctx->r5 = ADD32(ctx->r2, 0X68);
    // 0x8017AE70: jal         0x80013CC4
    // 0x8017AE74: addiu       $a2, $v0, 0x50
    ctx->r6 = ADD32(ctx->r2, 0X50);
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x8017AE74: addiu       $a2, $v0, 0x50
    ctx->r6 = ADD32(ctx->r2, 0X50);
    after_0:
    // 0x8017AE78: lwc1        $f0, 0x3C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8017AE7C: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8017AE80: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x8017AE84: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x8017AE88: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017AE8C: jal         0x80013C10
    // 0x8017AE90: swc1        $f6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f6.u32l;
    func_80013C10(rdram, ctx);
        goto after_1;
    // 0x8017AE90: swc1        $f6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f6.u32l;
    after_1:
    // 0x8017AE94: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X48);
    // 0x8017AE98: addiu       $a2, $sp, 0x2C
    ctx->r6 = ADD32(ctx->r29, 0X2C);
    // 0x8017AE9C: jal         0x80013C90
    // 0x8017AEA0: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013C90(rdram, ctx);
        goto after_2;
    // 0x8017AEA0: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_2:
    // 0x8017AEA4: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8017AEA8: nop

    // 0x8017AEAC: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
L_8017AEB0:
    // 0x8017AEB0: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x8017AEB4: c.eq.s      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.fl == ctx->f2.fl;
    // 0x8017AEB8: nop

    // 0x8017AEBC: bc1tl       L_8017AF10
    if (c1cs) {
        // 0x8017AEC0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017AF10;
    }
    goto skip_1;
    // 0x8017AEC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x8017AEC4: lw          $v0, 0x27A8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X27A8);
    // 0x8017AEC8: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    // 0x8017AECC: addiu       $a1, $v0, 0x68
    ctx->r5 = ADD32(ctx->r2, 0X68);
    // 0x8017AED0: jal         0x80013CC4
    // 0x8017AED4: addiu       $a2, $v0, 0x50
    ctx->r6 = ADD32(ctx->r2, 0X50);
    func_80013CC4(rdram, ctx);
        goto after_3;
    // 0x8017AED4: addiu       $a2, $v0, 0x50
    ctx->r6 = ADD32(ctx->r2, 0X50);
    after_3:
    // 0x8017AED8: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    // 0x8017AEDC: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x8017AEE0: addiu       $a2, $a2, -0x3068
    ctx->r6 = ADD32(ctx->r6, -0X3068);
    // 0x8017AEE4: jal         0x80013E6C
    // 0x8017AEE8: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013E6C(rdram, ctx);
        goto after_4;
    // 0x8017AEE8: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_4:
    // 0x8017AEEC: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    // 0x8017AEF0: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017AEF4: jal         0x80013C10
    // 0x8017AEF8: lw          $a2, 0x38($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X38);
    func_80013C10(rdram, ctx);
        goto after_5;
    // 0x8017AEF8: lw          $a2, 0x38($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X38);
    after_5:
    // 0x8017AEFC: lw          $a1, 0x48($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X48);
    // 0x8017AF00: addiu       $a2, $sp, 0x20
    ctx->r6 = ADD32(ctx->r29, 0X20);
    // 0x8017AF04: jal         0x80013C90
    // 0x8017AF08: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80013C90(rdram, ctx);
        goto after_6;
    // 0x8017AF08: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_6:
    // 0x8017AF0C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017AF10:
    // 0x8017AF10: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8017AF14: jr          $ra
    // 0x8017AF18: nop

    return;
    // 0x8017AF18: nop

;}
RECOMP_FUNC void func_8017AF1C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017AF1C: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x8017AF20: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x8017AF24: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x8017AF28: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8017AF2C: addu        $v0, $v0, $t6
    ctx->r2 = ADD32(ctx->r2, ctx->r14);
    // 0x8017AF30: lhu         $v0, -0x780A($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X780A);
    // 0x8017AF34: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x8017AF38: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8017AF3C: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x8017AF40: andi        $t7, $v0, 0x800
    ctx->r15 = ctx->r2 & 0X800;
    // 0x8017AF44: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017AF48: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8017AF4C: or          $a3, $a2, $zero
    ctx->r7 = ctx->r6 | 0;
    // 0x8017AF50: mov.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
    // 0x8017AF54: beq         $t7, $zero, L_8017AF60
    if (ctx->r15 == 0) {
        // 0x8017AF58: mov.s       $f2, $f14
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    ctx->f2.fl = ctx->f14.fl;
            goto L_8017AF60;
    }
    // 0x8017AF58: mov.s       $f2, $f14
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    ctx->f2.fl = ctx->f14.fl;
    // 0x8017AF5C: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_8017AF60:
    // 0x8017AF60: andi        $t8, $v0, 0x400
    ctx->r24 = ctx->r2 & 0X400;
    // 0x8017AF64: beq         $t8, $zero, L_8017AF70
    if (ctx->r24 == 0) {
        // 0x8017AF68: andi        $t9, $v0, 0x200
        ctx->r25 = ctx->r2 & 0X200;
            goto L_8017AF70;
    }
    // 0x8017AF68: andi        $t9, $v0, 0x200
    ctx->r25 = ctx->r2 & 0X200;
    // 0x8017AF6C: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
L_8017AF70:
    // 0x8017AF70: beq         $t9, $zero, L_8017AF7C
    if (ctx->r25 == 0) {
        // 0x8017AF74: andi        $t0, $v0, 0x100
        ctx->r8 = ctx->r2 & 0X100;
            goto L_8017AF7C;
    }
    // 0x8017AF74: andi        $t0, $v0, 0x100
    ctx->r8 = ctx->r2 & 0X100;
    // 0x8017AF78: neg.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = -ctx->f12.fl;
L_8017AF7C:
    // 0x8017AF7C: beq         $t0, $zero, L_8017AF88
    if (ctx->r8 == 0) {
        // 0x8017AF80: addiu       $s0, $sp, 0x24
        ctx->r16 = ADD32(ctx->r29, 0X24);
            goto L_8017AF88;
    }
    // 0x8017AF80: addiu       $s0, $sp, 0x24
    ctx->r16 = ADD32(ctx->r29, 0X24);
    // 0x8017AF84: mov.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
L_8017AF88:
    // 0x8017AF88: c.eq.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl == ctx->f14.fl;
    // 0x8017AF8C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8017AF90: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x8017AF94: bc1tl       L_8017AFAC
    if (c1cs) {
        // 0x8017AF98: c.eq.s      $f2, $f14
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f2.fl == ctx->f14.fl;
            goto L_8017AFAC;
    }
    goto skip_0;
    // 0x8017AF98: c.eq.s      $f2, $f14
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f2.fl == ctx->f14.fl;
    skip_0:
    // 0x8017AF9C: lwc1        $f4, 0x4($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X4);
    // 0x8017AFA0: add.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f0.fl;
    // 0x8017AFA4: swc1        $f6, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f6.u32l;
    // 0x8017AFA8: c.eq.s      $f2, $f14
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f2.fl == ctx->f14.fl;
L_8017AFAC:
    // 0x8017AFAC: nop

    // 0x8017AFB0: bc1tl       L_8017B010
    if (c1cs) {
        // 0x8017AFB4: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8017B010;
    }
    goto skip_1;
    // 0x8017AFB4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8017AFB8: lw          $v0, 0x27A8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X27A8);
    // 0x8017AFBC: sw          $a3, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r7;
    // 0x8017AFC0: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    // 0x8017AFC4: addiu       $a1, $v0, 0x68
    ctx->r5 = ADD32(ctx->r2, 0X68);
    // 0x8017AFC8: jal         0x80013CC4
    // 0x8017AFCC: addiu       $a2, $v0, 0x50
    ctx->r6 = ADD32(ctx->r2, 0X50);
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x8017AFCC: addiu       $a2, $v0, 0x50
    ctx->r6 = ADD32(ctx->r2, 0X50);
    after_0:
    // 0x8017AFD0: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x8017AFD4: addiu       $a2, $a2, -0x3068
    ctx->r6 = ADD32(ctx->r6, -0X3068);
    // 0x8017AFD8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8017AFDC: jal         0x80013E6C
    // 0x8017AFE0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80013E6C(rdram, ctx);
        goto after_1;
    // 0x8017AFE0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x8017AFE4: lwc1        $f2, 0x30($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8017AFE8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8017AFEC: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8017AFF0: mfc1        $a2, $f2
    ctx->r6 = (int32_t)ctx->f2.u32l;
    // 0x8017AFF4: jal         0x80013C10
    // 0x8017AFF8: nop

    func_80013C10(rdram, ctx);
        goto after_2;
    // 0x8017AFF8: nop

    after_2:
    // 0x8017AFFC: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    // 0x8017B000: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x8017B004: jal         0x80013C90
    // 0x8017B008: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80013C90(rdram, ctx);
        goto after_3;
    // 0x8017B008: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_3:
    // 0x8017B00C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8017B010:
    // 0x8017B010: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8017B014: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x8017B018: jr          $ra
    // 0x8017B01C: nop

    return;
    // 0x8017B01C: nop

;}
RECOMP_FUNC void func_8017B020(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B020: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017B024: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x8017B028: lh          $v0, 0x286C($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X286C);
    // 0x8017B02C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8017B030: beq         $v0, $zero, L_8017B048
    if (ctx->r2 == 0) {
        // 0x8017B034: nop
    
            goto L_8017B048;
    }
    // 0x8017B034: nop

    // 0x8017B038: beq         $v0, $at, L_8017B050
    if (ctx->r2 == ctx->r1) {
        // 0x8017B03C: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_8017B050;
    }
    // 0x8017B03C: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8017B040: beq         $v0, $at, L_8017B058
    if (ctx->r2 == ctx->r1) {
        // 0x8017B044: addiu       $v1, $zero, 0x8
        ctx->r3 = ADD32(0, 0X8);
            goto L_8017B058;
    }
    // 0x8017B044: addiu       $v1, $zero, 0x8
    ctx->r3 = ADD32(0, 0X8);
L_8017B048:
    // 0x8017B048: b           L_8017B058
    // 0x8017B04C: addiu       $v1, $zero, 0x8
    ctx->r3 = ADD32(0, 0X8);
        goto L_8017B058;
    // 0x8017B04C: addiu       $v1, $zero, 0x8
    ctx->r3 = ADD32(0, 0X8);
L_8017B050:
    // 0x8017B050: b           L_8017B058
    // 0x8017B054: addiu       $v1, $zero, 0x2000
    ctx->r3 = ADD32(0, 0X2000);
        goto L_8017B058;
    // 0x8017B054: addiu       $v1, $zero, 0x2000
    ctx->r3 = ADD32(0, 0X2000);
L_8017B058:
    // 0x8017B058: lhu         $t6, 0x536($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X536);
    // 0x8017B05C: jr          $ra
    // 0x8017B060: and         $v0, $t6, $v1
    ctx->r2 = ctx->r14 & ctx->r3;
    return;
    // 0x8017B060: and         $v0, $t6, $v1
    ctx->r2 = ctx->r14 & ctx->r3;
;}
RECOMP_FUNC void func_8017B064(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B064: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017B068: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x8017B06C: lh          $v0, 0x286C($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X286C);
    // 0x8017B070: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8017B074: beq         $v0, $zero, L_8017B08C
    if (ctx->r2 == 0) {
        // 0x8017B078: nop
    
            goto L_8017B08C;
    }
    // 0x8017B078: nop

    // 0x8017B07C: beq         $v0, $at, L_8017B094
    if (ctx->r2 == ctx->r1) {
        // 0x8017B080: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_8017B094;
    }
    // 0x8017B080: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8017B084: beq         $v0, $at, L_8017B09C
    if (ctx->r2 == ctx->r1) {
        // 0x8017B088: addiu       $v1, $zero, 0x4
        ctx->r3 = ADD32(0, 0X4);
            goto L_8017B09C;
    }
    // 0x8017B088: addiu       $v1, $zero, 0x4
    ctx->r3 = ADD32(0, 0X4);
L_8017B08C:
    // 0x8017B08C: b           L_8017B09C
    // 0x8017B090: addiu       $v1, $zero, 0x10
    ctx->r3 = ADD32(0, 0X10);
        goto L_8017B09C;
    // 0x8017B090: addiu       $v1, $zero, 0x10
    ctx->r3 = ADD32(0, 0X10);
L_8017B094:
    // 0x8017B094: b           L_8017B09C
    // 0x8017B098: addiu       $v1, $zero, 0x10
    ctx->r3 = ADD32(0, 0X10);
        goto L_8017B09C;
    // 0x8017B098: addiu       $v1, $zero, 0x10
    ctx->r3 = ADD32(0, 0X10);
L_8017B09C:
    // 0x8017B09C: lhu         $t6, 0x536($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X536);
    // 0x8017B0A0: jr          $ra
    // 0x8017B0A4: and         $v0, $t6, $v1
    ctx->r2 = ctx->r14 & ctx->r3;
    return;
    // 0x8017B0A4: and         $v0, $t6, $v1
    ctx->r2 = ctx->r14 & ctx->r3;
;}
RECOMP_FUNC void func_8017B0A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B0A8: beql        $a1, $zero, L_8017B0C4
    if (ctx->r5 == 0) {
        // 0x8017B0AC: lh          $t8, 0x0($a0)
        ctx->r24 = MEM_H(ctx->r4, 0X0);
            goto L_8017B0C4;
    }
    goto skip_0;
    // 0x8017B0AC: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
    skip_0:
    // 0x8017B0B0: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x8017B0B4: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x8017B0B8: jr          $ra
    // 0x8017B0BC: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    return;
    // 0x8017B0BC: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    // 0x8017B0C0: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
L_8017B0C4:
    // 0x8017B0C4: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x8017B0C8: or          $t9, $t8, $at
    ctx->r25 = ctx->r24 | ctx->r1;
    // 0x8017B0CC: sh          $t9, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r25;
    // 0x8017B0D0: jr          $ra
    // 0x8017B0D4: nop

    return;
    // 0x8017B0D4: nop

;}
RECOMP_FUNC void func_8017B0D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B0D8: beql        $a1, $zero, L_8017B0FC
    if (ctx->r5 == 0) {
        // 0x8017B0DC: lh          $t8, 0x0($a0)
        ctx->r24 = MEM_H(ctx->r4, 0X0);
            goto L_8017B0FC;
    }
    goto skip_0;
    // 0x8017B0DC: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
    skip_0:
    // 0x8017B0E0: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x8017B0E4: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x8017B0E8: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x8017B0EC: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x8017B0F0: jr          $ra
    // 0x8017B0F4: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    return;
    // 0x8017B0F4: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    // 0x8017B0F8: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
L_8017B0FC:
    // 0x8017B0FC: ori         $t9, $t8, 0x8000
    ctx->r25 = ctx->r24 | 0X8000;
    // 0x8017B100: sh          $t9, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r25;
    // 0x8017B104: jr          $ra
    // 0x8017B108: nop

    return;
    // 0x8017B108: nop

;}
RECOMP_FUNC void func_8017B10C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B10C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8017B110: jr          $ra
    // 0x8017B114: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x8017B114: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
;}
RECOMP_FUNC void func_8017B118(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B118: lh          $v1, 0x0($a0)
    ctx->r3 = MEM_H(ctx->r4, 0X0);
    // 0x8017B11C: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x8017B120: xori        $v0, $v1, 0x1B8
    ctx->r2 = ctx->r3 ^ 0X1B8;
    // 0x8017B124: jr          $ra
    // 0x8017B128: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    return;
    // 0x8017B128: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
;}
RECOMP_FUNC void func_8017B12C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B12C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8017B130: jr          $ra
    // 0x8017B134: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x8017B134: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
;}
RECOMP_FUNC void func_8017B138(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B138: beq         $a0, $zero, L_8017B148
    if (ctx->r4 == 0) {
        // 0x8017B13C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8017B148;
    }
    // 0x8017B13C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017B140: jr          $ra
    // 0x8017B144: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    return;
    // 0x8017B144: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
L_8017B148:
    // 0x8017B148: jr          $ra
    // 0x8017B14C: nop

    return;
    // 0x8017B14C: nop

;}
RECOMP_FUNC void func_8017B150(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B150: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x8017B154: lw          $a2, -0x53E4($a2)
    ctx->r6 = MEM_W(ctx->r6, -0X53E4);
    // 0x8017B158: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B15C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B160: lh          $t6, 0x0($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X0);
    // 0x8017B164: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017B168: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017B16C: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x8017B170: bne         $t7, $zero, L_8017B188
    if (ctx->r15 != 0) {
        // 0x8017B174: nop
    
            goto L_8017B188;
    }
    // 0x8017B174: nop

    // 0x8017B178: jal         0x80044388
    // 0x8017B17C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    func_80044388(rdram, ctx);
        goto after_0;
    // 0x8017B17C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_0:
    // 0x8017B180: b           L_8017B188
    // 0x8017B184: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
        goto L_8017B188;
    // 0x8017B184: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_8017B188:
    // 0x8017B188: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B18C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B190: jr          $ra
    // 0x8017B194: nop

    return;
    // 0x8017B194: nop

;}
RECOMP_FUNC void func_8017B198(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B198: jr          $ra
    // 0x8017B19C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x8017B19C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8017B1A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B1A0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8017B1A4: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8017B1A8: lw          $t6, -0x53E0($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53E0);
    // 0x8017B1AC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B1B0: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x8017B1B4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8017B1B8: beq         $a1, $at, L_8017B1DC
    if (ctx->r5 == ctx->r1) {
        // 0x8017B1BC: lh          $v0, 0x5E($t6)
        ctx->r2 = MEM_H(ctx->r14, 0X5E);
            goto L_8017B1DC;
    }
    // 0x8017B1BC: lh          $v0, 0x5E($t6)
    ctx->r2 = MEM_H(ctx->r14, 0X5E);
    // 0x8017B1C0: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8017B1C4: beq         $a1, $at, L_8017B208
    if (ctx->r5 == ctx->r1) {
        // 0x8017B1C8: addiu       $at, $zero, 0x3
        ctx->r1 = ADD32(0, 0X3);
            goto L_8017B208;
    }
    // 0x8017B1C8: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x8017B1CC: beql        $a1, $at, L_8017B238
    if (ctx->r5 == ctx->r1) {
        // 0x8017B1D0: mtc1        $v0, $f16
        ctx->f16.u32l = ctx->r2;
            goto L_8017B238;
    }
    goto skip_0;
    // 0x8017B1D0: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    skip_0:
    // 0x8017B1D4: b           L_8017B260
    // 0x8017B1D8: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
        goto L_8017B260;
    // 0x8017B1D8: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
L_8017B1DC:
    // 0x8017B1DC: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x8017B1E0: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x8017B1E4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8017B1E8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8017B1EC: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x8017B1F0: trunc.w.s   $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8017B1F4: mfc1        $v0, $f16
    ctx->r2 = (int32_t)ctx->f16.u32l;
    // 0x8017B1F8: nop

    // 0x8017B1FC: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x8017B200: b           L_8017B25C
    // 0x8017B204: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
        goto L_8017B25C;
    // 0x8017B204: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
L_8017B208:
    // 0x8017B208: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x8017B20C: lui         $at, 0x4680
    ctx->r1 = S32(0X4680 << 16);
    // 0x8017B210: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8017B214: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8017B218: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017B21C: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8017B220: mfc1        $v0, $f10
    ctx->r2 = (int32_t)ctx->f10.u32l;
    // 0x8017B224: nop

    // 0x8017B228: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x8017B22C: b           L_8017B25C
    // 0x8017B230: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
        goto L_8017B25C;
    // 0x8017B230: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x8017B234: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
L_8017B238:
    // 0x8017B238: lui         $at, 0x4680
    ctx->r1 = S32(0X4680 << 16);
    // 0x8017B23C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8017B240: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8017B244: sub.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x8017B248: trunc.w.s   $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8017B24C: mfc1        $v0, $f8
    ctx->r2 = (int32_t)ctx->f8.u32l;
    // 0x8017B250: nop

    // 0x8017B254: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x8017B258: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
L_8017B25C:
    // 0x8017B25C: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
L_8017B260:
    // 0x8017B260: jal         0x800A3A50
    // 0x8017B264: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_800A3A50(rdram, ctx);
        goto after_0;
    // 0x8017B264: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x8017B268: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8017B26C: jal         0x80097330
    // 0x8017B270: sh          $v0, 0x1C($sp)
    MEM_H(0X1C, ctx->r29) = ctx->r2;
    func_80097330(rdram, ctx);
        goto after_1;
    // 0x8017B270: sh          $v0, 0x1C($sp)
    MEM_H(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x8017B274: lh          $t0, 0x1C($sp)
    ctx->r8 = MEM_H(ctx->r29, 0X1C);
    // 0x8017B278: negu        $t1, $v0
    ctx->r9 = SUB32(0, ctx->r2);
    // 0x8017B27C: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x8017B280: mtc1        $t0, $f10
    ctx->f10.u32l = ctx->r8;
    // 0x8017B284: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x8017B288: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8017B28C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8017B290: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x8017B294: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x8017B298: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8017B29C: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8017B2A0: div.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8017B2A4: mfc1        $a3, $f8
    ctx->r7 = (int32_t)ctx->f8.u32l;
    // 0x8017B2A8: mfc1        $a1, $f18
    ctx->r5 = (int32_t)ctx->f18.u32l;
    // 0x8017B2AC: jal         0x80013F30
    // 0x8017B2B0: nop

    func_80013F30(rdram, ctx);
        goto after_2;
    // 0x8017B2B0: nop

    after_2:
    // 0x8017B2B4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B2B8: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8017B2BC: jr          $ra
    // 0x8017B2C0: nop

    return;
    // 0x8017B2C0: nop

;}
RECOMP_FUNC void func_8017B2C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B2C4: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8017B2C8: lw          $t6, -0x53E4($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53E4);
    // 0x8017B2CC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B2D0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B2D4: lw          $a2, 0x24($t6)
    ctx->r6 = MEM_W(ctx->r14, 0X24);
    // 0x8017B2D8: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017B2DC: beql        $a2, $zero, L_8017B2F0
    if (ctx->r6 == 0) {
        // 0x8017B2E0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017B2F0;
    }
    goto skip_0;
    // 0x8017B2E0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x8017B2E4: jal         0x8017B0A8
    // 0x8017B2E8: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    func_8017B0A8(rdram, ctx);
        goto after_0;
    // 0x8017B2E8: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_0:
    // 0x8017B2EC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017B2F0:
    // 0x8017B2F0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B2F4: jr          $ra
    // 0x8017B2F8: nop

    return;
    // 0x8017B2F8: nop

;}
RECOMP_FUNC void func_8017B2FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B2FC: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8017B300: lw          $t6, -0x53E4($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53E4);
    // 0x8017B304: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017B308: lw          $v1, 0x24($t6)
    ctx->r3 = MEM_W(ctx->r14, 0X24);
    // 0x8017B30C: beq         $v1, $zero, L_8017B328
    if (ctx->r3 == 0) {
        // 0x8017B310: nop
    
            goto L_8017B328;
    }
    // 0x8017B310: nop

    // 0x8017B314: lh          $v0, 0x0($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X0);
    // 0x8017B318: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x8017B31C: and         $v0, $v0, $at
    ctx->r2 = ctx->r2 & ctx->r1;
    // 0x8017B320: jr          $ra
    // 0x8017B324: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    return;
    // 0x8017B324: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
L_8017B328:
    // 0x8017B328: jr          $ra
    // 0x8017B32C: nop

    return;
    // 0x8017B32C: nop

;}
RECOMP_FUNC void func_8017B330(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B330: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8017B334: jr          $ra
    // 0x8017B338: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x8017B338: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
;}
RECOMP_FUNC void func_8017B33C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B33C: lui         $at, 0xBF80
    ctx->r1 = S32(0XBF80 << 16);
    // 0x8017B340: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8017B344: jr          $ra
    // 0x8017B348: nop

    return;
    // 0x8017B348: nop

;}
RECOMP_FUNC void func_8017B34C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B34C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8017B350: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B354: jal         0x8017B33C
    // 0x8017B358: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    func_8017B33C(rdram, ctx);
        goto after_0;
    // 0x8017B358: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    after_0:
    // 0x8017B35C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8017B360: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x8017B364: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x8017B368: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x8017B36C: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x8017B370: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017B374: bc1f        L_8017B3A8
    if (!c1cs) {
        // 0x8017B378: nop
    
            goto L_8017B3A8;
    }
    // 0x8017B378: nop

    // 0x8017B37C: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x8017B380: swc1        $f2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f2.u32l;
    // 0x8017B384: jal         0x80013CF8
    // 0x8017B388: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_1;
    // 0x8017B388: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_1:
    // 0x8017B38C: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    // 0x8017B390: lwc1        $f2, 0x1C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x8017B394: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8017B398: lwc1        $f6, 0x4($t6)
    ctx->f6.u32l = MEM_W(ctx->r14, 0X4);
    // 0x8017B39C: sub.s       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f2.fl;
    // 0x8017B3A0: b           L_8017B3A8
    // 0x8017B3A4: swc1        $f8, 0x4($t6)
    MEM_W(0X4, ctx->r14) = ctx->f8.u32l;
        goto L_8017B3A8;
    // 0x8017B3A4: swc1        $f8, 0x4($t6)
    MEM_W(0X4, ctx->r14) = ctx->f8.u32l;
L_8017B3A8:
    // 0x8017B3A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B3AC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8017B3B0: jr          $ra
    // 0x8017B3B4: nop

    return;
    // 0x8017B3B4: nop

;}
RECOMP_FUNC void func_8017B3B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B3B8: jr          $ra
    // 0x8017B3BC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x8017B3BC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
;}
RECOMP_FUNC void func_8017B3C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B3C0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B3C4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B3C8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017B3CC: jal         0x8017B138
    // 0x8017B3D0: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    func_8017B138(rdram, ctx);
        goto after_0;
    // 0x8017B3D0: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    after_0:
    // 0x8017B3D4: beq         $v0, $zero, L_8017B3F0
    if (ctx->r2 == 0) {
        // 0x8017B3D8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017B3F0;
    }
    // 0x8017B3D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B3DC: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x8017B3E0: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x8017B3E4: and         $v0, $v0, $at
    ctx->r2 = ctx->r2 & ctx->r1;
    // 0x8017B3E8: b           L_8017B3F4
    // 0x8017B3EC: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
        goto L_8017B3F4;
    // 0x8017B3EC: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_8017B3F0:
    // 0x8017B3F0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017B3F4:
    // 0x8017B3F4: jr          $ra
    // 0x8017B3F8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x8017B3F8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8017B3FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B3FC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B400: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B404: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017B408: jal         0x8017B138
    // 0x8017B40C: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    func_8017B138(rdram, ctx);
        goto after_0;
    // 0x8017B40C: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    after_0:
    // 0x8017B410: beq         $v0, $zero, L_8017B428
    if (ctx->r2 == 0) {
        // 0x8017B414: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017B428;
    }
    // 0x8017B414: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B418: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x8017B41C: andi        $v0, $v0, 0x2000
    ctx->r2 = ctx->r2 & 0X2000;
    // 0x8017B420: b           L_8017B42C
    // 0x8017B424: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
        goto L_8017B42C;
    // 0x8017B424: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_8017B428:
    // 0x8017B428: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B42C:
    // 0x8017B42C: jr          $ra
    // 0x8017B430: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x8017B430: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8017B434(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B434: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B438: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B43C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8017B440: jal         0x8017B138
    // 0x8017B444: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    func_8017B138(rdram, ctx);
        goto after_0;
    // 0x8017B444: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    after_0:
    // 0x8017B448: beq         $v0, $zero, L_8017B464
    if (ctx->r2 == 0) {
        // 0x8017B44C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017B464;
    }
    // 0x8017B44C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B450: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x8017B454: lui         $at, 0x20
    ctx->r1 = S32(0X20 << 16);
    // 0x8017B458: and         $v0, $v0, $at
    ctx->r2 = ctx->r2 & ctx->r1;
    // 0x8017B45C: b           L_8017B468
    // 0x8017B460: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
        goto L_8017B468;
    // 0x8017B460: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_8017B464:
    // 0x8017B464: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017B468:
    // 0x8017B468: jr          $ra
    // 0x8017B46C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x8017B46C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8017B470(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B470: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8017B474: jr          $ra
    // 0x8017B478: lh          $v0, -0x54CC($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X54CC);
    return;
    // 0x8017B478: lh          $v0, -0x54CC($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X54CC);
;}
RECOMP_FUNC void func_8017B47C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B47C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8017B480: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B484: jal         0x80020E50
    // 0x8017B488: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_80020E50(rdram, ctx);
        goto after_0;
    // 0x8017B488: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8017B48C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x8017B490: jal         0x80020E50
    // 0x8017B494: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    func_80020E50(rdram, ctx);
        goto after_1;
    // 0x8017B494: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    after_1:
    // 0x8017B498: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    // 0x8017B49C: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x8017B4A0: beql        $a1, $zero, L_8017B4E4
    if (ctx->r5 == 0) {
        // 0x8017B4A4: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_8017B4E4;
    }
    goto skip_0;
    // 0x8017B4A4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_0:
    // 0x8017B4A8: beql        $v0, $zero, L_8017B4E4
    if (ctx->r2 == 0) {
        // 0x8017B4AC: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_8017B4E4;
    }
    goto skip_1;
    // 0x8017B4AC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_1:
    // 0x8017B4B0: jal         0x80013CC4
    // 0x8017B4B4: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    func_80013CC4(rdram, ctx);
        goto after_2;
    // 0x8017B4B4: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    after_2:
    // 0x8017B4B8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8017B4BC: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    // 0x8017B4C0: jal         0x80013BD0
    // 0x8017B4C4: swc1        $f4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f4.u32l;
    func_80013BD0(rdram, ctx);
        goto after_3;
    // 0x8017B4C4: swc1        $f4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f4.u32l;
    after_3:
    // 0x8017B4C8: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x8017B4CC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8017B4D0: nop

    // 0x8017B4D4: mul.s       $f0, $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x8017B4D8: b           L_8017B4EC
    // 0x8017B4DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8017B4EC;
    // 0x8017B4DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B4E0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
L_8017B4E4:
    // 0x8017B4E4: nop

    // 0x8017B4E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017B4EC:
    // 0x8017B4EC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8017B4F0: jr          $ra
    // 0x8017B4F4: nop

    return;
    // 0x8017B4F4: nop

;}
RECOMP_FUNC void func_8017B4F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B4F8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8017B4FC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B500: jal         0x80020E50
    // 0x8017B504: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_80020E50(rdram, ctx);
        goto after_0;
    // 0x8017B504: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8017B508: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x8017B50C: jal         0x80020E50
    // 0x8017B510: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_80020E50(rdram, ctx);
        goto after_1;
    // 0x8017B510: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x8017B514: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x8017B518: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x8017B51C: beql        $a2, $zero, L_8017B550
    if (ctx->r6 == 0) {
        // 0x8017B520: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_8017B550;
    }
    goto skip_0;
    // 0x8017B520: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_0:
    // 0x8017B524: beql        $v0, $zero, L_8017B550
    if (ctx->r2 == 0) {
        // 0x8017B528: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_8017B550;
    }
    goto skip_1;
    // 0x8017B528: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_1:
    // 0x8017B52C: jal         0x80013B9C
    // 0x8017B530: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    func_80013B9C(rdram, ctx);
        goto after_2;
    // 0x8017B530: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_2:
    // 0x8017B534: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x8017B538: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8017B53C: nop

    // 0x8017B540: mul.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f4.fl);
    // 0x8017B544: b           L_8017B558
    // 0x8017B548: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8017B558;
    // 0x8017B548: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B54C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
L_8017B550:
    // 0x8017B550: nop

    // 0x8017B554: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017B558:
    // 0x8017B558: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8017B55C: jr          $ra
    // 0x8017B560: nop

    return;
    // 0x8017B560: nop

;}
RECOMP_FUNC void func_8017B564(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B564: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x8017B568: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x8017B56C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B570: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B574: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8017B578: jal         0x80013CF8
    // 0x8017B57C: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x8017B57C: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_0:
    // 0x8017B580: jal         0x8017B470
    // 0x8017B584: nop

    func_8017B470(rdram, ctx);
        goto after_1;
    // 0x8017B584: nop

    after_1:
    // 0x8017B588: beq         $v0, $zero, L_8017B5C0
    if (ctx->r2 == 0) {
        // 0x8017B58C: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_8017B5C0;
    }
    // 0x8017B58C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8017B590: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8017B594: bnel        $v0, $at, L_8017B5C4
    if (ctx->r2 != ctx->r1) {
        // 0x8017B598: lwc1        $f18, 0x4($a0)
        ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
            goto L_8017B5C4;
    }
    goto skip_0;
    // 0x8017B598: lwc1        $f18, 0x4($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
    skip_0:
    // 0x8017B59C: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8017B5A0: lui         $at, 0x402C
    ctx->r1 = S32(0X402C << 16);
    // 0x8017B5A4: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x8017B5A8: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8017B5AC: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x8017B5B0: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x8017B5B4: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x8017B5B8: b           L_8017B5E0
    // 0x8017B5BC: swc1        $f16, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f16.u32l;
        goto L_8017B5E0;
    // 0x8017B5BC: swc1        $f16, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f16.u32l;
L_8017B5C0:
    // 0x8017B5C0: lwc1        $f18, 0x4($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
L_8017B5C4:
    // 0x8017B5C4: lui         $at, 0x4032
    ctx->r1 = S32(0X4032 << 16);
    // 0x8017B5C8: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x8017B5CC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8017B5D0: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x8017B5D4: add.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f4.d + ctx->f6.d;
    // 0x8017B5D8: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x8017B5DC: swc1        $f10, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f10.u32l;
L_8017B5E0:
    // 0x8017B5E0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B5E4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B5E8: jr          $ra
    // 0x8017B5EC: nop

    return;
    // 0x8017B5EC: nop

;}
RECOMP_FUNC void func_8017B5F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B5F0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B5F4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B5F8: jal         0x8017B10C
    // 0x8017B5FC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_8017B10C(rdram, ctx);
        goto after_0;
    // 0x8017B5FC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x8017B600: beql        $v0, $zero, L_8017B61C
    if (ctx->r2 == 0) {
        // 0x8017B604: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8017B61C;
    }
    goto skip_0;
    // 0x8017B604: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x8017B608: jal         0x8017B138
    // 0x8017B60C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    func_8017B138(rdram, ctx);
        goto after_1;
    // 0x8017B60C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    after_1:
    // 0x8017B610: b           L_8017B620
    // 0x8017B614: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8017B620;
    // 0x8017B614: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B618: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B61C:
    // 0x8017B61C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017B620:
    // 0x8017B620: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B624: jr          $ra
    // 0x8017B628: nop

    return;
    // 0x8017B628: nop

;}
RECOMP_FUNC void func_8017B62C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B62C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B630: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B634: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8017B638: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x8017B63C: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x8017B640: bnel        $t7, $zero, L_8017B674
    if (ctx->r15 != 0) {
        // 0x8017B644: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8017B674;
    }
    goto skip_0;
    // 0x8017B644: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x8017B648: jal         0x8017B10C
    // 0x8017B64C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_8017B10C(rdram, ctx);
        goto after_0;
    // 0x8017B64C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x8017B650: beq         $v0, $zero, L_8017B670
    if (ctx->r2 == 0) {
        // 0x8017B654: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_8017B670;
    }
    // 0x8017B654: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8017B658: jal         0x80044388
    // 0x8017B65C: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    func_80044388(rdram, ctx);
        goto after_1;
    // 0x8017B65C: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    after_1:
    // 0x8017B660: beql        $v0, $zero, L_8017B674
    if (ctx->r2 == 0) {
        // 0x8017B664: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8017B674;
    }
    goto skip_1;
    // 0x8017B664: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x8017B668: b           L_8017B674
    // 0x8017B66C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8017B674;
    // 0x8017B66C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017B670:
    // 0x8017B670: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B674:
    // 0x8017B674: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B678: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B67C: jr          $ra
    // 0x8017B680: nop

    return;
    // 0x8017B680: nop

;}
RECOMP_FUNC void func_8017B684(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B684: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8017B688: lw          $t6, -0x53E4($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53E4);
    // 0x8017B68C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B690: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B694: bne         $t6, $zero, L_8017B6A4
    if (ctx->r14 != 0) {
        // 0x8017B698: nop
    
            goto L_8017B6A4;
    }
    // 0x8017B698: nop

    // 0x8017B69C: b           L_8017B6C4
    // 0x8017B6A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B6C4;
    // 0x8017B6A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B6A4:
    // 0x8017B6A4: jal         0x8017B434
    // 0x8017B6A8: nop

    func_8017B434(rdram, ctx);
        goto after_0;
    // 0x8017B6A8: nop

    after_0:
    // 0x8017B6AC: bne         $v0, $zero, L_8017B6BC
    if (ctx->r2 != 0) {
        // 0x8017B6B0: nop
    
            goto L_8017B6BC;
    }
    // 0x8017B6B0: nop

    // 0x8017B6B4: b           L_8017B6C4
    // 0x8017B6B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B6C4;
    // 0x8017B6B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B6BC:
    // 0x8017B6BC: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8017B6C0: lw          $v0, -0x2E60($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X2E60);
L_8017B6C4:
    // 0x8017B6C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B6C8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B6CC: jr          $ra
    // 0x8017B6D0: nop

    return;
    // 0x8017B6D0: nop

;}
RECOMP_FUNC void func_8017B6D4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B6D4: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x8017B6D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017B6DC: beq         $v1, $zero, L_8017B6F0
    if (ctx->r3 == 0) {
        // 0x8017B6E0: nop
    
            goto L_8017B6F0;
    }
    // 0x8017B6E0: nop

    // 0x8017B6E4: lh          $v0, 0x5E($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X5E);
    // 0x8017B6E8: jr          $ra
    // 0x8017B6EC: addiu       $v0, $v0, 0x4000
    ctx->r2 = ADD32(ctx->r2, 0X4000);
    return;
    // 0x8017B6EC: addiu       $v0, $v0, 0x4000
    ctx->r2 = ADD32(ctx->r2, 0X4000);
L_8017B6F0:
    // 0x8017B6F0: jr          $ra
    // 0x8017B6F4: nop

    return;
    // 0x8017B6F4: nop

;}
RECOMP_FUNC void func_8017B6F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B6F8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B6FC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B700: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x8017B704: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x8017B708: beql        $v0, $zero, L_8017B724
    if (ctx->r2 == 0) {
        // 0x8017B70C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8017B724;
    }
    goto skip_0;
    // 0x8017B70C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x8017B710: jal         0x80013CF8
    // 0x8017B714: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x8017B714: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    after_0:
    // 0x8017B718: b           L_8017B724
    // 0x8017B71C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8017B724;
    // 0x8017B71C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8017B720: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B724:
    // 0x8017B724: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B728: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B72C: jr          $ra
    // 0x8017B730: nop

    return;
    // 0x8017B730: nop

;}
RECOMP_FUNC void func_8017B734(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B734: lw          $v0, 0x68($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X68);
    // 0x8017B738: beql        $v0, $zero, L_8017B754
    if (ctx->r2 == 0) {
        // 0x8017B73C: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_8017B754;
    }
    goto skip_0;
    // 0x8017B73C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_0:
    // 0x8017B740: lh          $t6, 0x1C($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X1C);
    // 0x8017B744: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x8017B748: jr          $ra
    // 0x8017B74C: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    return;
    // 0x8017B74C: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8017B750: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
L_8017B754:
    // 0x8017B754: nop

    // 0x8017B758: jr          $ra
    // 0x8017B75C: nop

    return;
    // 0x8017B75C: nop

;}
RECOMP_FUNC void func_8017B760(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B760: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8017B764: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B768: jal         0x8017B6D4
    // 0x8017B76C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_8017B6D4(rdram, ctx);
        goto after_0;
    // 0x8017B76C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_0:
    // 0x8017B770: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x8017B774: jal         0x800A3A50
    // 0x8017B778: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    func_800A3A50(rdram, ctx);
        goto after_1;
    // 0x8017B778: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    after_1:
    // 0x8017B77C: sh          $v0, 0x18($sp)
    MEM_H(0X18, ctx->r29) = ctx->r2;
    // 0x8017B780: jal         0x80097330
    // 0x8017B784: lhu         $a0, 0x1E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1E);
    func_80097330(rdram, ctx);
        goto after_2;
    // 0x8017B784: lhu         $a0, 0x1E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1E);
    after_2:
    // 0x8017B788: lh          $t6, 0x18($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X18);
    // 0x8017B78C: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x8017B790: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x8017B794: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x8017B798: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8017B79C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8017B7A0: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x8017B7A4: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x8017B7A8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8017B7AC: div.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8017B7B0: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8017B7B4: mfc1        $a3, $f18
    ctx->r7 = (int32_t)ctx->f18.u32l;
    // 0x8017B7B8: mfc1        $a1, $f8
    ctx->r5 = (int32_t)ctx->f8.u32l;
    // 0x8017B7BC: jal         0x80013F30
    // 0x8017B7C0: nop

    func_80013F30(rdram, ctx);
        goto after_3;
    // 0x8017B7C0: nop

    after_3:
    // 0x8017B7C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B7C8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8017B7CC: jr          $ra
    // 0x8017B7D0: nop

    return;
    // 0x8017B7D0: nop

;}
RECOMP_FUNC void func_8017B7D4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B7D4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B7D8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B7DC: jal         0x8017B62C
    // 0x8017B7E0: lw          $a0, 0x18($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X18);
    func_8017B62C(rdram, ctx);
        goto after_0;
    // 0x8017B7E0: lw          $a0, 0x18($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X18);
    after_0:
    // 0x8017B7E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B7E8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B7EC: jr          $ra
    // 0x8017B7F0: nop

    return;
    // 0x8017B7F0: nop

;}
RECOMP_FUNC void func_8017B7F4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B7F4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B7F8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B7FC: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x8017B800: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017B804: andi        $t6, $v0, 0x2
    ctx->r14 = ctx->r2 & 0X2;
    // 0x8017B808: bne         $t6, $zero, L_8017B818
    if (ctx->r14 != 0) {
        // 0x8017B80C: andi        $t7, $v0, 0x10
        ctx->r15 = ctx->r2 & 0X10;
            goto L_8017B818;
    }
    // 0x8017B80C: andi        $t7, $v0, 0x10
    ctx->r15 = ctx->r2 & 0X10;
    // 0x8017B810: b           L_8017B854
    // 0x8017B814: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B854;
    // 0x8017B814: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B818:
    // 0x8017B818: beq         $t7, $zero, L_8017B828
    if (ctx->r15 == 0) {
        // 0x8017B81C: nop
    
            goto L_8017B828;
    }
    // 0x8017B81C: nop

    // 0x8017B820: b           L_8017B854
    // 0x8017B824: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B854;
    // 0x8017B824: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B828:
    // 0x8017B828: jal         0x8017B5F0
    // 0x8017B82C: lw          $a0, 0x18($a1)
    ctx->r4 = MEM_W(ctx->r5, 0X18);
    func_8017B5F0(rdram, ctx);
        goto after_0;
    // 0x8017B82C: lw          $a0, 0x18($a1)
    ctx->r4 = MEM_W(ctx->r5, 0X18);
    after_0:
    // 0x8017B830: beql        $v0, $zero, L_8017B854
    if (ctx->r2 == 0) {
        // 0x8017B834: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_8017B854;
    }
    goto skip_0;
    // 0x8017B834: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_0:
    // 0x8017B838: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x8017B83C: sll         $t9, $t8, 0
    ctx->r25 = S32(ctx->r24 << 0);
    // 0x8017B840: bgezl       $t9, L_8017B854
    if (SIGNED(ctx->r25) >= 0) {
        // 0x8017B844: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_8017B854;
    }
    goto skip_1;
    // 0x8017B844: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_1:
    // 0x8017B848: b           L_8017B854
    // 0x8017B84C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B854;
    // 0x8017B84C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8017B850: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017B854:
    // 0x8017B854: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B858: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B85C: jr          $ra
    // 0x8017B860: nop

    return;
    // 0x8017B860: nop

;}
RECOMP_FUNC void func_8017B864(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B864: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B868: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B86C: lwc1        $f12, 0x8($a0)
    ctx->f12.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8017B870: lwc1        $f14, 0x0($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8017B874: jal         0x80004C68
    // 0x8017B878: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_80004C68(rdram, ctx);
        goto after_0;
    // 0x8017B878: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_0:
    // 0x8017B87C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B880: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B884: jr          $ra
    // 0x8017B888: nop

    return;
    // 0x8017B888: nop

;}
RECOMP_FUNC void func_8017B88C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B88C: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x8017B890: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017B894: addiu       $t6, $sp, 0x2C
    ctx->r14 = ADD32(ctx->r29, 0X2C);
    // 0x8017B898: lui         $a2, 0x3C23
    ctx->r6 = S32(0X3C23 << 16);
    // 0x8017B89C: lui         $a3, 0xBC23
    ctx->r7 = S32(0XBC23 << 16);
    // 0x8017B8A0: sw          $a0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r4;
    // 0x8017B8A4: sw          $a1, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r5;
    // 0x8017B8A8: ori         $a3, $a3, 0xD70A
    ctx->r7 = ctx->r7 | 0XD70A;
    // 0x8017B8AC: ori         $a2, $a2, 0xD70A
    ctx->r6 = ctx->r6 | 0XD70A;
    // 0x8017B8B0: jal         0x800401F0
    // 0x8017B8B4: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    func_800401F0(rdram, ctx);
        goto after_0;
    // 0x8017B8B4: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    after_0:
    // 0x8017B8B8: sll         $t7, $v0, 1
    ctx->r15 = S32(ctx->r2 << 1);
    // 0x8017B8BC: bgez        $t7, L_8017B8CC
    if (SIGNED(ctx->r15) >= 0) {
        // 0x8017B8C0: lw          $a0, 0x30($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X30);
            goto L_8017B8CC;
    }
    // 0x8017B8C0: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x8017B8C4: b           L_8017B918
    // 0x8017B8C8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B918;
    // 0x8017B8C8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B8CC:
    // 0x8017B8CC: jal         0x8017B928
    // 0x8017B8D0: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    func_8017B928(rdram, ctx);
        goto after_1;
    // 0x8017B8D0: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    after_1:
    // 0x8017B8D4: bne         $v0, $zero, L_8017B8E4
    if (ctx->r2 != 0) {
        // 0x8017B8D8: lw          $a0, 0x60($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X60);
            goto L_8017B8E4;
    }
    // 0x8017B8D8: lw          $a0, 0x60($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X60);
    // 0x8017B8DC: b           L_8017B918
    // 0x8017B8E0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B918;
    // 0x8017B8E0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B8E4:
    // 0x8017B8E4: jal         0x80013B9C
    // 0x8017B8E8: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    func_80013B9C(rdram, ctx);
        goto after_2;
    // 0x8017B8E8: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    after_2:
    // 0x8017B8EC: swc1        $f0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f0.u32l;
    // 0x8017B8F0: jal         0x80013BD0
    // 0x8017B8F4: lw          $a0, 0x64($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X64);
    func_80013BD0(rdram, ctx);
        goto after_3;
    // 0x8017B8F4: lw          $a0, 0x64($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X64);
    after_3:
    // 0x8017B8F8: lwc1        $f4, 0x24($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X24);
    // 0x8017B8FC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8017B900: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x8017B904: nop

    // 0x8017B908: bc1f        L_8017B918
    if (!c1cs) {
        // 0x8017B90C: nop
    
            goto L_8017B918;
    }
    // 0x8017B90C: nop

    // 0x8017B910: b           L_8017B918
    // 0x8017B914: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B918;
    // 0x8017B914: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B918:
    // 0x8017B918: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8017B91C: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    // 0x8017B920: jr          $ra
    // 0x8017B924: nop

    return;
    // 0x8017B924: nop

;}
RECOMP_FUNC void func_8017B928(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B928: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017B92C: beq         $a0, $zero, L_8017B968
    if (ctx->r4 == 0) {
        // 0x8017B930: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_8017B968;
    }
    // 0x8017B930: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017B934: lhu         $v0, 0x0($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X0);
    // 0x8017B938: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x8017B93C: andi        $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 & 0XFF;
    // 0x8017B940: beq         $v0, $at, L_8017B960
    if (ctx->r2 == ctx->r1) {
        // 0x8017B944: addiu       $at, $zero, 0x29
        ctx->r1 = ADD32(0, 0X29);
            goto L_8017B960;
    }
    // 0x8017B944: addiu       $at, $zero, 0x29
    ctx->r1 = ADD32(0, 0X29);
    // 0x8017B948: beq         $v0, $at, L_8017B960
    if (ctx->r2 == ctx->r1) {
        // 0x8017B94C: addiu       $at, $zero, 0x2A
        ctx->r1 = ADD32(0, 0X2A);
            goto L_8017B960;
    }
    // 0x8017B94C: addiu       $at, $zero, 0x2A
    ctx->r1 = ADD32(0, 0X2A);
    // 0x8017B950: beq         $v0, $at, L_8017B960
    if (ctx->r2 == ctx->r1) {
        // 0x8017B954: addiu       $at, $zero, 0x2B
        ctx->r1 = ADD32(0, 0X2B);
            goto L_8017B960;
    }
    // 0x8017B954: addiu       $at, $zero, 0x2B
    ctx->r1 = ADD32(0, 0X2B);
    // 0x8017B958: bne         $v0, $at, L_8017B970
    if (ctx->r2 != ctx->r1) {
        // 0x8017B95C: nop
    
            goto L_8017B970;
    }
    // 0x8017B95C: nop

L_8017B960:
    // 0x8017B960: b           L_8017B97C
    // 0x8017B964: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B97C;
    // 0x8017B964: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B968:
    // 0x8017B968: b           L_8017B97C
    // 0x8017B96C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8017B97C;
    // 0x8017B96C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017B970:
    // 0x8017B970: jal         0x80001FD0
    // 0x8017B974: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80001FD0(rdram, ctx);
        goto after_0;
    // 0x8017B974: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_0:
    // 0x8017B978: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017B97C:
    // 0x8017B97C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017B980: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017B984: jr          $ra
    // 0x8017B988: nop

    return;
    // 0x8017B988: nop

;}
RECOMP_FUNC void func_8017B98C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017B98C: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x8017B990: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017B994: lwc1        $f0, -0x3E78($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X3E78);
    // 0x8017B998: sw          $a0, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r4;
    // 0x8017B99C: sw          $a1, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r5;
    // 0x8017B9A0: sw          $a2, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r6;
    // 0x8017B9A4: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x8017B9A8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017B9AC: addiu       $t6, $sp, 0x3C
    ctx->r14 = ADD32(ctx->r29, 0X3C);
    // 0x8017B9B0: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x8017B9B4: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x8017B9B8: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x8017B9BC: jal         0x800403C0
    // 0x8017B9C0: lw          $a3, 0x70($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X70);
    func_800403C0(rdram, ctx);
        goto after_0;
    // 0x8017B9C0: lw          $a3, 0x70($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X70);
    after_0:
    // 0x8017B9C4: sll         $t7, $v0, 1
    ctx->r15 = S32(ctx->r2 << 1);
    // 0x8017B9C8: bltz        $t7, L_8017BA90
    if (SIGNED(ctx->r15) < 0) {
        // 0x8017B9CC: lui         $t9, 0x8019
        ctx->r25 = S32(0X8019 << 16);
            goto L_8017BA90;
    }
    // 0x8017B9CC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x8017B9D0: addiu       $t9, $t9, 0x285C
    ctx->r25 = ADD32(ctx->r25, 0X285C);
    // 0x8017B9D4: lw          $t1, 0x0($t9)
    ctx->r9 = MEM_W(ctx->r25, 0X0);
    // 0x8017B9D8: addiu       $t8, $sp, 0x2C
    ctx->r24 = ADD32(ctx->r29, 0X2C);
    // 0x8017B9DC: lw          $t0, 0x4($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X4);
    // 0x8017B9E0: sw          $t1, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->r9;
    // 0x8017B9E4: lw          $t1, 0x8($t9)
    ctx->r9 = MEM_W(ctx->r25, 0X8);
    // 0x8017B9E8: sw          $t0, 0x4($t8)
    MEM_W(0X4, ctx->r24) = ctx->r8;
    // 0x8017B9EC: sw          $t1, 0x8($t8)
    MEM_W(0X8, ctx->r24) = ctx->r9;
    // 0x8017B9F0: lw          $a1, 0x44($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X44);
    // 0x8017B9F4: jal         0x8017B928
    // 0x8017B9F8: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X40);
    func_8017B928(rdram, ctx);
        goto after_1;
    // 0x8017B9F8: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X40);
    after_1:
    // 0x8017B9FC: beq         $v0, $zero, L_8017BA90
    if (ctx->r2 == 0) {
        // 0x8017BA00: lwc1        $f4, 0x70($sp)
        ctx->f4.u32l = MEM_W(ctx->r29, 0X70);
            goto L_8017BA90;
    }
    // 0x8017BA00: lwc1        $f4, 0x70($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X70);
    // 0x8017BA04: lwc1        $f6, 0x48($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8017BA08: lwc1        $f10, 0x4C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8017BA0C: lwc1        $f8, 0x2C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8017BA10: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8017BA14: lwc1        $f6, 0x50($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X50);
    // 0x8017BA18: lwc1        $f4, 0x34($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8017BA1C: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X54);
    // 0x8017BA20: mul.s       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x8017BA24: lw          $a1, 0x58($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X58);
    // 0x8017BA28: mul.s       $f10, $f6, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8017BA2C: add.s       $f18, $f8, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f8.fl + ctx->f16.fl;
    // 0x8017BA30: add.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x8017BA34: swc1        $f18, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f18.u32l;
    // 0x8017BA38: jal         0x8017B928
    // 0x8017BA3C: swc1        $f8, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f8.u32l;
    func_8017B928(rdram, ctx);
        goto after_2;
    // 0x8017BA3C: swc1        $f8, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f8.u32l;
    after_2:
    // 0x8017BA40: beq         $v0, $zero, L_8017BA80
    if (ctx->r2 == 0) {
        // 0x8017BA44: addiu       $a1, $sp, 0x2C
        ctx->r5 = ADD32(ctx->r29, 0X2C);
            goto L_8017BA80;
    }
    // 0x8017BA44: addiu       $a1, $sp, 0x2C
    ctx->r5 = ADD32(ctx->r29, 0X2C);
    // 0x8017BA48: lwc1        $f16, 0x70($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X70);
    // 0x8017BA4C: lwc1        $f18, 0x5C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8017BA50: lwc1        $f4, 0x60($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X60);
    // 0x8017BA54: lwc1        $f6, 0x2C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8017BA58: sub.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8017BA5C: lwc1        $f18, 0x64($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X64);
    // 0x8017BA60: lwc1        $f16, 0x34($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8017BA64: mul.s       $f10, $f4, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8017BA68: nop

    // 0x8017BA6C: mul.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x8017BA70: add.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8017BA74: add.s       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f16.fl + ctx->f4.fl;
    // 0x8017BA78: swc1        $f8, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f8.u32l;
    // 0x8017BA7C: swc1        $f6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f6.u32l;
L_8017BA80:
    // 0x8017BA80: jal         0x80013CF8
    // 0x8017BA84: lw          $a0, 0x68($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X68);
    func_80013CF8(rdram, ctx);
        goto after_3;
    // 0x8017BA84: lw          $a0, 0x68($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X68);
    after_3:
    // 0x8017BA88: b           L_8017BA94
    // 0x8017BA8C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8017BA94;
    // 0x8017BA8C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017BA90:
    // 0x8017BA90: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8017BA94:
    // 0x8017BA94: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8017BA98: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    // 0x8017BA9C: jr          $ra
    // 0x8017BAA0: nop

    return;
    // 0x8017BAA0: nop

;}
RECOMP_FUNC void func_8017BAB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017BAB0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017BAB4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017BAB8: lw          $v0, 0x7C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X7C);
    // 0x8017BABC: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8017BAC0: bnel        $v0, $at, L_8017BADC
    if (ctx->r2 != ctx->r1) {
        // 0x8017BAC4: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_8017BADC;
    }
    goto skip_0;
    // 0x8017BAC4: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    skip_0:
    // 0x8017BAC8: jal         0x8017BB18
    // 0x8017BACC: nop

    func_8017BB18(rdram, ctx);
        goto after_0;
    // 0x8017BACC: nop

    after_0:
    // 0x8017BAD0: b           L_8017BB0C
    // 0x8017BAD4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8017BB0C;
    // 0x8017BAD4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017BAD8: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
L_8017BADC:
    // 0x8017BADC: bnel        $v0, $at, L_8017BAF8
    if (ctx->r2 != ctx->r1) {
        // 0x8017BAE0: addiu       $at, $zero, 0x3
        ctx->r1 = ADD32(0, 0X3);
            goto L_8017BAF8;
    }
    goto skip_1;
    // 0x8017BAE0: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    skip_1:
    // 0x8017BAE4: jal         0x8017BB80
    // 0x8017BAE8: nop

    func_8017BB80(rdram, ctx);
        goto after_1;
    // 0x8017BAE8: nop

    after_1:
    // 0x8017BAEC: b           L_8017BB0C
    // 0x8017BAF0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8017BB0C;
    // 0x8017BAF0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017BAF4: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
L_8017BAF8:
    // 0x8017BAF8: bnel        $v0, $at, L_8017BB0C
    if (ctx->r2 != ctx->r1) {
        // 0x8017BAFC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017BB0C;
    }
    goto skip_2;
    // 0x8017BAFC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x8017BB00: jal         0x8017BBEC
    // 0x8017BB04: nop

    func_8017BBEC(rdram, ctx);
        goto after_2;
    // 0x8017BB04: nop

    after_2:
    // 0x8017BB08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017BB0C:
    // 0x8017BB0C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017BB10: jr          $ra
    // 0x8017BB14: nop

    return;
    // 0x8017BB14: nop

;}
RECOMP_FUNC void func_8017BB18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017BB18: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x8017BB1C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8017BB20: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017BB24: lwc1        $f6, -0x3E70($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3E70);
    // 0x8017BB28: lwc1        $f4, 0x68($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X68);
    // 0x8017BB2C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017BB30: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8017BB34: swc1        $f8, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f8.u32l;
    // 0x8017BB38: lwc1        $f10, 0x68($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X68);
    // 0x8017BB3C: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x8017BB40: nop

    // 0x8017BB44: bc1fl       L_8017BB54
    if (!c1cs) {
        // 0x8017BB48: lwc1        $f16, 0x6C($a0)
        ctx->f16.u32l = MEM_W(ctx->r4, 0X6C);
            goto L_8017BB54;
    }
    goto skip_0;
    // 0x8017BB48: lwc1        $f16, 0x6C($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X6C);
    skip_0:
    // 0x8017BB4C: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
    // 0x8017BB50: lwc1        $f16, 0x6C($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X6C);
L_8017BB54:
    // 0x8017BB54: lwc1        $f18, -0x3E6C($at)
    ctx->f18.u32l = MEM_W(ctx->r1, -0X3E6C);
    // 0x8017BB58: add.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x8017BB5C: swc1        $f4, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f4.u32l;
    // 0x8017BB60: lwc1        $f6, 0x6C($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X6C);
    // 0x8017BB64: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x8017BB68: nop

    // 0x8017BB6C: bc1f        L_8017BB78
    if (!c1cs) {
        // 0x8017BB70: nop
    
            goto L_8017BB78;
    }
    // 0x8017BB70: nop

    // 0x8017BB74: swc1        $f0, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f0.u32l;
L_8017BB78:
    // 0x8017BB78: jr          $ra
    // 0x8017BB7C: nop

    return;
    // 0x8017BB7C: nop

;}
RECOMP_FUNC void func_8017BB80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017BB80: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017BB84: lwc1        $f0, -0x3E68($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X3E68);
    // 0x8017BB88: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017BB8C: lwc1        $f6, -0x3E64($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3E64);
    // 0x8017BB90: lwc1        $f4, 0x68($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X68);
    // 0x8017BB94: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017BB98: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017BB9C: swc1        $f8, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f8.u32l;
    // 0x8017BBA0: lwc1        $f10, 0x68($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X68);
    // 0x8017BBA4: c.lt.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl < ctx->f10.fl;
    // 0x8017BBA8: nop

    // 0x8017BBAC: bc1fl       L_8017BBBC
    if (!c1cs) {
        // 0x8017BBB0: lwc1        $f16, 0x6C($a0)
        ctx->f16.u32l = MEM_W(ctx->r4, 0X6C);
            goto L_8017BBBC;
    }
    goto skip_0;
    // 0x8017BBB0: lwc1        $f16, 0x6C($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X6C);
    skip_0:
    // 0x8017BBB4: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
    // 0x8017BBB8: lwc1        $f16, 0x6C($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X6C);
L_8017BBBC:
    // 0x8017BBBC: lwc1        $f18, -0x3E60($at)
    ctx->f18.u32l = MEM_W(ctx->r1, -0X3E60);
    // 0x8017BBC0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8017BBC4: sub.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8017BBC8: swc1        $f4, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f4.u32l;
    // 0x8017BBCC: lwc1        $f6, 0x6C($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X6C);
    // 0x8017BBD0: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x8017BBD4: nop

    // 0x8017BBD8: bc1f        L_8017BBE4
    if (!c1cs) {
        // 0x8017BBDC: nop
    
            goto L_8017BBE4;
    }
    // 0x8017BBDC: nop

    // 0x8017BBE0: swc1        $f0, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f0.u32l;
L_8017BBE4:
    // 0x8017BBE4: jr          $ra
    // 0x8017BBE8: nop

    return;
    // 0x8017BBE8: nop

;}
