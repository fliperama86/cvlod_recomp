#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

#if LOD_ENABLE_NI0E_TRACE
#include <stdbool.h>
#include <stdio.h>

extern uint32_t lod_current_map_overlay_rom(void);

static inline bool lod_ni0e_pause_addr_ok(uint32_t addr, uint32_t size) {
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return phys <= 0x800000u && size <= 0x800000u - phys;
}

static inline gpr lod_ni0e_pause_addr_gpr(uint32_t addr) {
    return (gpr)(int32_t)addr;
}

#define LOD_NI0E_PAUSE_MEM_H(offset, addr) MEM_H((offset), lod_ni0e_pause_addr_gpr((addr)))
#define LOD_NI0E_PAUSE_MEM_HU(offset, addr) MEM_HU((offset), lod_ni0e_pause_addr_gpr((addr)))
#define LOD_NI0E_PAUSE_MEM_W(offset, addr) MEM_W((offset), lod_ni0e_pause_addr_gpr((addr)))

// Issue #27/#31 unified 0x0E hypothesis: pause-gate probe for the maps
// implicated in both issues. See docs/issue27-31-ni0e-findings.md.
static bool lod_ni0e_pause_focus_map(uint32_t rom) {
    return rom == 0x007A2D70u || rom == 0x007932D0u || rom == 0x007D4420u ||
           rom == 0x007D3C90u;
}

static void lod_ni0e_pause_gate_probe(uint8_t* rdram, uint32_t obj) {
    const uint32_t map_rom = lod_current_map_overlay_rom();
    if (!lod_ni0e_pause_focus_map(map_rom)) {
        return;
    }

    static uint32_t call_count = 0;
    call_count++;
    if (!(call_count <= 40 || (call_count % 300) == 0)) {
        return;
    }

    if (obj == 0 || !lod_ni0e_pause_addr_ok(obj, 0x38)) {
        fprintf(stderr,
                "[NI0E_TRACE] pause-gate #%u map_rom=0x%08X obj=0x%08X invalid\n",
                call_count, map_rom, obj);
        return;
    }

    const uint32_t mgr_data = (uint32_t)LOD_NI0E_PAUSE_MEM_W(0x34, obj);
    int32_t countdown = 0;
    uint32_t frame_counter = 0;
    uint32_t request_latch = 0;
    if (mgr_data != 0 && lod_ni0e_pause_addr_ok(mgr_data, 0x1AA)) {
        countdown = (int32_t)LOD_NI0E_PAUSE_MEM_H(0x1A4, mgr_data);
        frame_counter = (uint32_t)LOD_NI0E_PAUSE_MEM_HU(0x1A6, mgr_data);
        request_latch = (uint32_t)LOD_NI0E_PAUSE_MEM_HU(0x1A8, mgr_data);
    }

    const uint32_t sys_base = 0x801C82C0u;
    const uint32_t pressed = (uint32_t)LOD_NI0E_PAUSE_MEM_HU(0x538, sys_base);
    const uint32_t fade_mode = (uint32_t)LOD_NI0E_PAUSE_MEM_HU(0x8E, sys_base);
    const uint32_t fade_counter = (uint32_t)LOD_NI0E_PAUSE_MEM_HU(0x94, sys_base);
    const uint32_t fade_duration = (uint32_t)LOD_NI0E_PAUSE_MEM_HU(0x96, sys_base);

    fprintf(stderr,
            "[NI0E_TRACE] pause-gate #%u map_rom=0x%08X obj=0x%08X mgrData=0x%08X "
            "countdown=%d frame=%u latch=%u pressed=0x%04X fade_mode=0x%04X "
            "fade_counter=%u fade_duration=%u\n",
            call_count, map_rom, obj, mgr_data, countdown, frame_counter,
            request_latch, pressed, fade_mode, fade_counter, fade_duration);
}
#endif  // LOD_ENABLE_NI0E_TRACE

RECOMP_FUNC void ni_ovl_047_func_0F002F10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_pause_gate_probe(rdram, (uint32_t)ctx->r4);
#endif
    // 0x0F002F10: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x0F002F14: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F002F18: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F002F1C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F002F20: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F002F24: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F002F28: lh          $t6, 0x1A8($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X1A8);
    // 0x0F002F2C: addiu       $t9, $t9, 0x5BC
    ctx->r25 = ADD32(ctx->r25, 0X5BC);
    // 0x0F002F30: beql        $t6, $zero, L_0F002F70
    if (ctx->r14 == 0) {
        // 0x0F002F34: addiu       $v0, $v1, 0xDC
        ctx->r2 = ADD32(ctx->r3, 0XDC);
            goto L_0F002F70;
    }
    goto skip_0;
    // 0x0F002F34: addiu       $v0, $v1, 0xDC
    ctx->r2 = ADD32(ctx->r3, 0XDC);
    skip_0:
    // 0x0F002F38: jalr        $t9
    // 0x0F002F3C: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F002F3C: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    after_0:
    // 0x0F002F40: bne         $v0, $zero, L_0F002F6C
    if (ctx->r2 != 0) {
        // 0x0F002F44: lw          $v1, 0x3C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X3C);
            goto L_0F002F6C;
    }
    // 0x0F002F44: lw          $v1, 0x3C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X3C);
    // 0x0F002F48: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F002F4C: addiu       $t9, $t9, 0x1E64
    ctx->r25 = ADD32(ctx->r25, 0X1E64);
    // 0x0F002F50: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F002F54: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F002F58: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x0F002F5C: jalr        $t9
    // 0x0F002F60: addiu       $a3, $zero, 0x9
    ctx->r7 = ADD32(0, 0X9);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F002F60: addiu       $a3, $zero, 0x9
    ctx->r7 = ADD32(0, 0X9);
    after_1:
    // 0x0F002F64: b           L_0F003070
    // 0x0F002F68: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F003070;
    // 0x0F002F68: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F002F6C:
    // 0x0F002F6C: addiu       $v0, $v1, 0xDC
    ctx->r2 = ADD32(ctx->r3, 0XDC);
L_0F002F70:
    // 0x0F002F70: lh          $t7, 0xCC($v0)
    ctx->r15 = MEM_H(ctx->r2, 0XCC);
    // 0x0F002F74: bnel        $t7, $zero, L_0F002FD0
    if (ctx->r15 != 0) {
        // 0x0F002F78: lh          $t3, 0xCA($v0)
        ctx->r11 = MEM_H(ctx->r2, 0XCA);
            goto L_0F002FD0;
    }
    goto skip_1;
    // 0x0F002F78: lh          $t3, 0xCA($v0)
    ctx->r11 = MEM_H(ctx->r2, 0XCA);
    skip_1:
    // 0x0F002F7C: lh          $t8, 0xC8($v0)
    ctx->r24 = MEM_H(ctx->r2, 0XC8);
    // 0x0F002F80: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x0F002F84: slti        $at, $t8, -0xE
    ctx->r1 = SIGNED(ctx->r24) < -0XE ? 1 : 0;
    // 0x0F002F88: beql        $at, $zero, L_0F002FD0
    if (ctx->r1 == 0) {
        // 0x0F002F8C: lh          $t3, 0xCA($v0)
        ctx->r11 = MEM_H(ctx->r2, 0XCA);
            goto L_0F002FD0;
    }
    goto skip_2;
    // 0x0F002F8C: lh          $t3, 0xCA($v0)
    ctx->r11 = MEM_H(ctx->r2, 0XCA);
    skip_2:
    // 0x0F002F90: lhu         $t0, -0x7808($t0)
    ctx->r8 = MEM_HU(ctx->r8, -0X7808);
    // 0x0F002F94: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x0F002F98: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x0F002F9C: andi        $t1, $t0, 0x1080
    ctx->r9 = ctx->r8 & 0X1080;
    // 0x0F002FA0: beq         $t1, $zero, L_0F002FCC
    if (ctx->r9 == 0) {
        // 0x0F002FA4: addiu       $a1, $zero, 0xA
        ctx->r5 = ADD32(0, 0XA);
            goto L_0F002FCC;
    }
    // 0x0F002FA4: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
    // 0x0F002FA8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F002FAC: sh          $t2, 0xCC($v0)
    MEM_H(0XCC, ctx->r2) = ctx->r10;
    // 0x0F002FB0: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x0F002FB4: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    // 0x0F002FB8: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F002FBC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F002FC0: jalr        $t9
    // 0x0F002FC4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F002FC4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_2:
    // 0x0F002FC8: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
L_0F002FCC:
    // 0x0F002FCC: lh          $t3, 0xCA($v0)
    ctx->r11 = MEM_H(ctx->r2, 0XCA);
L_0F002FD0:
    // 0x0F002FD0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F002FD4: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x0F002FD8: sh          $t4, 0xCA($v0)
    MEM_H(0XCA, ctx->r2) = ctx->r12;
    // 0x0F002FDC: jal         0x0F000FE4
    // 0x0F002FE0: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    ni_ovl_047_func_0F000FE4(rdram, ctx);
        goto after_3;
    // 0x0F002FE0: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    after_3:
    // 0x0F002FE4: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x0F002FE8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F002FEC: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x0F002FF0: lh          $t5, 0xC8($v0)
    ctx->r13 = MEM_H(ctx->r2, 0XC8);
    // 0x0F002FF4: addiu       $a0, $zero, 0x4000
    ctx->r4 = ADD32(0, 0X4000);
    // 0x0F002FF8: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
    // 0x0F002FFC: addiu       $t6, $t5, -0x1
    ctx->r14 = ADD32(ctx->r13, -0X1);
    // 0x0F003000: sh          $t6, 0xC8($v0)
    MEM_H(0XC8, ctx->r2) = ctx->r14;
    // 0x0F003004: lh          $t7, 0xC8($v0)
    ctx->r15 = MEM_H(ctx->r2, 0XC8);
    // 0x0F003008: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F00300C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F003010: bnel        $t7, $zero, L_0F00302C
    if (ctx->r15 != 0) {
        // 0x0F003014: lw          $v1, 0xB8($v0)
        ctx->r3 = MEM_W(ctx->r2, 0XB8);
            goto L_0F00302C;
    }
    goto skip_3;
    // 0x0F003014: lw          $v1, 0xB8($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XB8);
    skip_3:
    // 0x0F003018: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F00301C: jalr        $t9
    // 0x0F003020: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F003020: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    after_4:
    // 0x0F003024: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x0F003028: lw          $v1, 0xB8($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XB8);
L_0F00302C:
    // 0x0F00302C: beql        $v1, $zero, L_0F003070
    if (ctx->r3 == 0) {
        // 0x0F003030: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_0F003070;
    }
    goto skip_4;
    // 0x0F003030: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_4:
    // 0x0F003034: lbu         $t8, 0xB($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0XB);
    // 0x0F003038: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x0F00303C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F003040: bne         $t8, $at, L_0F00306C
    if (ctx->r24 != ctx->r1) {
        // 0x0F003044: lui         $at, 0x802F
        ctx->r1 = S32(0X802F << 16);
            goto L_0F00306C;
    }
    // 0x0F003044: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x0F003048: jal         0x0F0022BC
    // 0x0F00304C: sh          $zero, -0x46A0($at)
    MEM_H(-0X46A0, ctx->r1) = 0;
    ni_ovl_047_func_0F0022BC(rdram, ctx);
        goto after_5;
    // 0x0F00304C: sh          $zero, -0x46A0($at)
    MEM_H(-0X46A0, ctx->r1) = 0;
    after_5:
    // 0x0F003050: bnel        $v0, $zero, L_0F003064
    if (ctx->r2 != 0) {
        // 0x0F003054: addiu       $t0, $zero, 0x1
        ctx->r8 = ADD32(0, 0X1);
            goto L_0F003064;
    }
    goto skip_5;
    // 0x0F003054: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    skip_5:
    // 0x0F003058: jal         0x0F001558
    // 0x0F00305C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    ni_ovl_047_func_0F001558(rdram, ctx);
        goto after_6;
    // 0x0F00305C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_6:
    // 0x0F003060: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
L_0F003064:
    // 0x0F003064: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x0F003068: sh          $t0, -0x46A0($at)
    MEM_H(-0X46A0, ctx->r1) = ctx->r8;
L_0F00306C:
    // 0x0F00306C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F003070:
    // 0x0F003070: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F003074: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x0F003078: jr          $ra
    // 0x0F00307C: nop

    return;
    // 0x0F00307C: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003080(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003080: jr          $ra
    // 0x0F003084: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x0F003084: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void ni_ovl_047_func_0F003088(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003088: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F00308C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003090: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F003094: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F003098: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F00309C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F0030A0: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F0030A4: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F0030A8: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F0030AC: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F0030B0: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F0030B4: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F0030B8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F0030BC: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F0030C0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F0030C4: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F0030C8: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F0030CC: lw          $t9, 0x58F8($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X58F8);
    // 0x0F0030D0: jalr        $t9
    // 0x0F0030D4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0030D4: nop

    after_0:
    // 0x0F0030D8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F0030DC: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F0030E0: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F0030E4: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F0030E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F0030EC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0030F0: jr          $ra
    // 0x0F0030F4: nop

    return;
    // 0x0F0030F4: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F0030F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0030F8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x0F0030FC: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F003100: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F003104: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003108: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F00310C: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x0F003110: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F003114: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x0F003118: jalr        $t9
    // 0x0F00311C: addiu       $a1, $zero, 0x93
    ctx->r5 = ADD32(0, 0X93);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00311C: addiu       $a1, $zero, 0x93
    ctx->r5 = ADD32(0, 0X93);
    after_0:
    // 0x0F003120: jal         0x0F0031B0
    // 0x0F003124: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    ni_ovl_047_func_0F0031B0(rdram, ctx);
        goto after_1;
    // 0x0F003124: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x0F003128: jal         0x0F0032A8
    // 0x0F00312C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    ni_ovl_047_func_0F0032A8(rdram, ctx);
        goto after_2;
    // 0x0F00312C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x0F003130: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003134: addiu       $t9, $t9, 0x1E64
    ctx->r25 = ADD32(ctx->r25, 0X1E64);
    // 0x0F003138: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F00313C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F003140: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F003144: jalr        $t9
    // 0x0F003148: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F003148: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
    after_3:
    // 0x0F00314C: jal         0x0F001558
    // 0x0F003150: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    ni_ovl_047_func_0F001558(rdram, ctx);
        goto after_4;
    // 0x0F003150: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_4:
    // 0x0F003154: lui         $s0, 0x8009
    ctx->r16 = S32(0X8009 << 16);
    // 0x0F003158: addiu       $s0, $s0, 0x10DC
    ctx->r16 = ADD32(ctx->r16, 0X10DC);
    // 0x0F00315C: jalr        $s0
    // 0x0F003160: addiu       $a0, $zero, 0x7E
    ctx->r4 = ADD32(0, 0X7E);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_5;
    // 0x0F003160: addiu       $a0, $zero, 0x7E
    ctx->r4 = ADD32(0, 0X7E);
    after_5:
    // 0x0F003164: jalr        $s0
    // 0x0F003168: addiu       $a0, $zero, 0x371
    ctx->r4 = ADD32(0, 0X371);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_6;
    // 0x0F003168: addiu       $a0, $zero, 0x371
    ctx->r4 = ADD32(0, 0X371);
    after_6:
    // 0x0F00316C: jalr        $s0
    // 0x0F003170: addiu       $a0, $zero, 0x372
    ctx->r4 = ADD32(0, 0X372);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_7;
    // 0x0F003170: addiu       $a0, $zero, 0x372
    ctx->r4 = ADD32(0, 0X372);
    after_7:
    // 0x0F003174: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F003178: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x0F00317C: addiu       $a0, $zero, 0x4000
    ctx->r4 = ADD32(0, 0X4000);
    // 0x0F003180: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
    // 0x0F003184: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F003188: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F00318C: jalr        $t9
    // 0x0F003190: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F003190: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_8:
    // 0x0F003194: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x0F003198: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F00319C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x0F0031A0: jr          $ra
    // 0x0F0031A4: nop

    return;
    // 0x0F0031A4: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F0031A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0031A8: jr          $ra
    // 0x0F0031AC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x0F0031AC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void ni_ovl_047_func_0F0031B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0031B0: lui         $v1, 0x802F
    ctx->r3 = S32(0X802F << 16);
    // 0x0F0031B4: addiu       $v1, $v1, -0x46A0
    ctx->r3 = ADD32(ctx->r3, -0X46A0);
    // 0x0F0031B8: lh          $t7, 0x0($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X0);
    // 0x0F0031BC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F0031C0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F0031C4: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x0F0031C8: slti        $at, $t7, 0x2
    ctx->r1 = SIGNED(ctx->r15) < 0X2 ? 1 : 0;
    // 0x0F0031CC: beq         $at, $zero, L_0F003290
    if (ctx->r1 == 0) {
        // 0x0F0031D0: sw          $t6, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r14;
            goto L_0F003290;
    }
    // 0x0F0031D0: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    // 0x0F0031D4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x0F0031D8: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x0F0031DC: lw          $t9, 0x2908($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X2908);
    // 0x0F0031E0: addiu       $at, $zero, -0x41
    ctx->r1 = ADD32(0, -0X41);
    // 0x0F0031E4: addiu       $t8, $zero, 0x2
    ctx->r24 = ADD32(0, 0X2);
    // 0x0F0031E8: and         $t0, $t9, $at
    ctx->r8 = ctx->r25 & ctx->r1;
    // 0x0F0031EC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F0031F0: addiu       $t9, $t9, -0x5590
    ctx->r25 = ADD32(ctx->r25, -0X5590);
    // 0x0F0031F4: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
    // 0x0F0031F8: sw          $t0, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r8;
    // 0x0F0031FC: jalr        $t9
    // 0x0F003200: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F003200: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x0F003204: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F003208: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
    // 0x0F00320C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003210: addiu       $t9, $t9, 0x2410
    ctx->r25 = ADD32(ctx->r25, 0X2410);
    // 0x0F003214: lw          $v1, 0x60($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X60);
    // 0x0F003218: beq         $v1, $zero, L_0F003234
    if (ctx->r3 == 0) {
        // 0x0F00321C: nop
    
            goto L_0F003234;
    }
    // 0x0F00321C: nop

    // 0x0F003220: lbu         $t1, 0x0($v1)
    ctx->r9 = MEM_BU(ctx->r3, 0X0);
    // 0x0F003224: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F003228: andi        $t2, $t1, 0xFFBF
    ctx->r10 = ctx->r9 & 0XFFBF;
    // 0x0F00322C: sb          $t2, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r10;
    // 0x0F003230: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
L_0F003234:
    // 0x0F003234: jalr        $t9
    // 0x0F003238: addiu       $a1, $zero, 0x21A0
    ctx->r5 = ADD32(0, 0X21A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F003238: addiu       $a1, $zero, 0x21A0
    ctx->r5 = ADD32(0, 0X21A0);
    after_1:
    // 0x0F00323C: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x0F003240: addiu       $t5, $zero, 0x2710
    ctx->r13 = ADD32(0, 0X2710);
    // 0x0F003244: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F003248: sw          $v0, 0x198($v1)
    MEM_W(0X198, ctx->r3) = ctx->r2;
    // 0x0F00324C: sw          $zero, 0x38($v0)
    MEM_W(0X38, ctx->r2) = 0;
    // 0x0F003250: lw          $t4, 0x198($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X198);
    // 0x0F003254: addiu       $v1, $v1, 0xDC
    ctx->r3 = ADD32(ctx->r3, 0XDC);
    // 0x0F003258: addiu       $t3, $v1, 0xB4
    ctx->r11 = ADD32(ctx->r3, 0XB4);
    // 0x0F00325C: sw          $t3, 0x40($t4)
    MEM_W(0X40, ctx->r12) = ctx->r11;
    // 0x0F003260: lw          $t6, 0xBC($v1)
    ctx->r14 = MEM_W(ctx->r3, 0XBC);
    // 0x0F003264: addiu       $at, $zero, 0x18
    ctx->r1 = ADD32(0, 0X18);
    // 0x0F003268: lui         $t9, 0x802E
    ctx->r25 = S32(0X802E << 16);
    // 0x0F00326C: sw          $t5, 0x44($t6)
    MEM_W(0X44, ctx->r14) = ctx->r13;
    // 0x0F003270: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x0F003274: addiu       $t9, $t9, 0x6A14
    ctx->r25 = ADD32(ctx->r25, 0X6A14);
    // 0x0F003278: lw          $t7, 0x34($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X34);
    // 0x0F00327C: lw          $t8, 0x4($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X4);
    // 0x0F003280: bnel        $t8, $at, L_0F003294
    if (ctx->r24 != ctx->r1) {
        // 0x0F003284: lw          $t0, 0x1C($sp)
        ctx->r8 = MEM_W(ctx->r29, 0X1C);
            goto L_0F003294;
    }
    goto skip_0;
    // 0x0F003284: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x0F003288: jalr        $t9
    // 0x0F00328C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00328C: nop

    after_2:
L_0F003290:
    // 0x0F003290: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
L_0F003294:
    // 0x0F003294: sh          $zero, 0x1A8($t0)
    MEM_H(0X1A8, ctx->r8) = 0;
    // 0x0F003298: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F00329C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F0032A0: jr          $ra
    // 0x0F0032A4: nop

    return;
    // 0x0F0032A4: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F0032A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0032A8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x0F0032AC: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x0F0032B0: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0032B4: lwc1        $f4, 0x59CC($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X59CC);
    // 0x0F0032B8: lw          $t6, 0x2968($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2968);
    // 0x0F0032BC: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F0032C0: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0032C4: swc1        $f4, 0x14($t6)
    MEM_W(0X14, ctx->r14) = ctx->f4.u32l;
    // 0x0F0032C8: lw          $t7, 0x2968($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X2968);
    // 0x0F0032CC: lw          $t8, 0x2960($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X2960);
    // 0x0F0032D0: addiu       $t5, $zero, -0x40E5
    ctx->r13 = ADD32(0, -0X40E5);
    // 0x0F0032D4: lwc1        $f6, 0x14($t7)
    ctx->f6.u32l = MEM_W(ctx->r15, 0X14);
    // 0x0F0032D8: addiu       $t7, $zero, -0x4939
    ctx->r15 = ADD32(0, -0X4939);
    // 0x0F0032DC: swc1        $f6, 0x50($t8)
    MEM_W(0X50, ctx->r24) = ctx->f6.u32l;
    // 0x0F0032E0: lw          $t9, 0x2968($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X2968);
    // 0x0F0032E4: lwc1        $f8, 0x59D0($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X59D0);
    // 0x0F0032E8: lui         $at, 0xC3BF
    ctx->r1 = S32(0XC3BF << 16);
    // 0x0F0032EC: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F0032F0: swc1        $f8, 0x18($t9)
    MEM_W(0X18, ctx->r25) = ctx->f8.u32l;
    // 0x0F0032F4: lw          $t0, 0x2968($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X2968);
    // 0x0F0032F8: lw          $t1, 0x2960($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X2960);
    // 0x0F0032FC: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F003300: lwc1        $f10, 0x18($t0)
    ctx->f10.u32l = MEM_W(ctx->r8, 0X18);
    // 0x0F003304: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F003308: swc1        $f10, 0x54($t1)
    MEM_W(0X54, ctx->r9) = ctx->f10.u32l;
    // 0x0F00330C: lw          $t2, 0x2968($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X2968);
    // 0x0F003310: swc1        $f16, 0x1C($t2)
    MEM_W(0X1C, ctx->r10) = ctx->f16.u32l;
    // 0x0F003314: lw          $t3, 0x2968($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X2968);
    // 0x0F003318: lw          $t4, 0x2960($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X2960);
    // 0x0F00331C: lwc1        $f18, 0x1C($t3)
    ctx->f18.u32l = MEM_W(ctx->r11, 0X1C);
    // 0x0F003320: swc1        $f18, 0x58($t4)
    MEM_W(0X58, ctx->r12) = ctx->f18.u32l;
    // 0x0F003324: lw          $t6, 0x2960($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2960);
    // 0x0F003328: sh          $t5, 0x5E($t6)
    MEM_H(0X5E, ctx->r14) = ctx->r13;
    // 0x0F00332C: lwc1        $f4, 0x59D4($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X59D4);
    // 0x0F003330: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F003334: swc1        $f6, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f6.u32l;
    // 0x0F003338: swc1        $f4, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f4.u32l;
    // 0x0F00333C: lwc1        $f8, 0x59D8($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X59D8);
    // 0x0F003340: sh          $t7, 0x5E($v0)
    MEM_H(0X5E, ctx->r2) = ctx->r15;
    // 0x0F003344: jr          $ra
    // 0x0F003348: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    return;
    // 0x0F003348: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
;}
RECOMP_FUNC void ni_ovl_047_func_0F00334C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F00334C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F003350: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003354: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F003358: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F00335C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F003360: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F003364: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F003368: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F00336C: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F003370: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F003374: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F003378: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F00337C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F003380: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F003384: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F003388: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F00338C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F003390: lw          $t9, 0x5900($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X5900);
    // 0x0F003394: jalr        $t9
    // 0x0F003398: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F003398: nop

    after_0:
    // 0x0F00339C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F0033A0: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F0033A4: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F0033A8: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F0033AC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F0033B0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0033B4: jr          $ra
    // 0x0F0033B8: nop

    return;
    // 0x0F0033B8: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F0033BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0033BC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F0033C0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F0033C4: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F0033C8: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x0F0033CC: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0033D0: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F0033D4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x0F0033D8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x0F0033DC: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x0F0033E0: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x0F0033E4: lw          $t8, -0x5170($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X5170);
    // 0x0F0033E8: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x0F0033EC: beq         $t9, $zero, L_0F003408
    if (ctx->r25 == 0) {
        // 0x0F0033F0: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_0F003408;
    }
    // 0x0F0033F0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0033F4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0033F8: jalr        $t9
    // 0x0F0033FC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0033FC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x0F003400: jal         0x0F003418
    // 0x0F003404: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    ni_ovl_047_func_0F003418(rdram, ctx);
        goto after_1;
    // 0x0F003404: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    after_1:
L_0F003408:
    // 0x0F003408: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F00340C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F003410: jr          $ra
    // 0x0F003414: nop

    return;
    // 0x0F003414: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003418(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003418: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x0F00341C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F003420: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F003424: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F003428: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F00342C: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x0F003430: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F003434: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F003438: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x0F00343C: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x0F003440: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x0F003444: lw          $t8, -0x5170($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X5170);
    // 0x0F003448: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x0F00344C: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x0F003450: beq         $t9, $zero, L_0F003494
    if (ctx->r25 == 0) {
        // 0x0F003454: lui         $t9, 0x8004
        ctx->r25 = S32(0X8004 << 16);
            goto L_0F003494;
    }
    // 0x0F003454: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F003458: addiu       $t9, $t9, 0x7954
    ctx->r25 = ADD32(ctx->r25, 0X7954);
    // 0x0F00345C: jalr        $t9
    // 0x0F003460: sw          $a0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F003460: sw          $a0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r4;
    after_0:
    // 0x0F003464: beq         $v0, $zero, L_0F0034EC
    if (ctx->r2 == 0) {
        // 0x0F003468: lw          $a0, 0x2C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X2C);
            goto L_0F0034EC;
    }
    // 0x0F003468: lw          $a0, 0x2C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X2C);
    // 0x0F00346C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F003470: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F003474: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x0F003478: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F00347C: addiu       $a2, $zero, 0x6
    ctx->r6 = ADD32(0, 0X6);
    // 0x0F003480: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F003484: jalr        $t9
    // 0x0F003488: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F003488: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x0F00348C: b           L_0F0034F0
    // 0x0F003490: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F0034F0;
    // 0x0F003490: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F003494:
    // 0x0F003494: jal         0x0F0031B0
    // 0x0F003498: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    ni_ovl_047_func_0F0031B0(rdram, ctx);
        goto after_2;
    // 0x0F003498: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x0F00349C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F0034A0: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x0F0034A4: addiu       $at, $zero, 0x18
    ctx->r1 = ADD32(0, 0X18);
    // 0x0F0034A8: lui         $t9, 0x802E
    ctx->r25 = S32(0X802E << 16);
    // 0x0F0034AC: lw          $t0, 0x34($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X34);
    // 0x0F0034B0: addiu       $t9, $t9, 0x6A14
    ctx->r25 = ADD32(ctx->r25, 0X6A14);
    // 0x0F0034B4: lw          $t1, 0x4($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X4);
    // 0x0F0034B8: bne         $t1, $at, L_0F0034C8
    if (ctx->r9 != ctx->r1) {
        // 0x0F0034BC: nop
    
            goto L_0F0034C8;
    }
    // 0x0F0034BC: nop

    // 0x0F0034C0: jalr        $t9
    // 0x0F0034C4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F0034C4: nop

    after_3:
L_0F0034C8:
    // 0x0F0034C8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0034CC: addiu       $t9, $t9, 0x1E64
    ctx->r25 = ADD32(ctx->r25, 0X1E64);
    // 0x0F0034D0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0034D4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F0034D8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F0034DC: jalr        $t9
    // 0x0F0034E0: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F0034E0: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
    after_4:
    // 0x0F0034E4: jal         0x0F001558
    // 0x0F0034E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    ni_ovl_047_func_0F001558(rdram, ctx);
        goto after_5;
    // 0x0F0034E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
L_0F0034EC:
    // 0x0F0034EC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F0034F0:
    // 0x0F0034F0: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F0034F4: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x0F0034F8: jr          $ra
    // 0x0F0034FC: nop

    return;
    // 0x0F0034FC: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003500(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003500: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x0F003504: addiu       $t6, $zero, 0x2
    ctx->r14 = ADD32(0, 0X2);
    // 0x0F003508: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x0F00350C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x0F003510: sh          $t6, -0x46A0($at)
    MEM_H(-0X46A0, ctx->r1) = ctx->r14;
    // 0x0F003514: lw          $t8, 0x2908($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2908);
    // 0x0F003518: addiu       $at, $zero, -0x41
    ctx->r1 = ADD32(0, -0X41);
    // 0x0F00351C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F003520: and         $t9, $t8, $at
    ctx->r25 = ctx->r24 & ctx->r1;
    // 0x0F003524: sw          $t9, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r25;
    // 0x0F003528: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00352C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003530: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F003534: addiu       $t7, $zero, 0x48
    ctx->r15 = ADD32(0, 0X48);
    // 0x0F003538: addiu       $t9, $t9, 0x1E64
    ctx->r25 = ADD32(ctx->r25, 0X1E64);
    // 0x0F00353C: or          $t0, $a0, $zero
    ctx->r8 = ctx->r4 | 0;
    // 0x0F003540: sw          $t7, 0x2BC8($v0)
    MEM_W(0X2BC8, ctx->r2) = ctx->r15;
    // 0x0F003544: addiu       $a1, $t0, 0xE
    ctx->r5 = ADD32(ctx->r8, 0XE);
    // 0x0F003548: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x0F00354C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F003550: jalr        $t9
    // 0x0F003554: addiu       $a3, $zero, 0x5
    ctx->r7 = ADD32(0, 0X5);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F003554: addiu       $a3, $zero, 0x5
    ctx->r7 = ADD32(0, 0X5);
    after_0:
    // 0x0F003558: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F00355C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F003560: jr          $ra
    // 0x0F003564: nop

    return;
    // 0x0F003564: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003568(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003568: addiu       $sp, $sp, -0x90
    ctx->r29 = ADD32(ctx->r29, -0X90);
    // 0x0F00356C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003570: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x0F003574: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F003578: lui         $t8, 0x802F
    ctx->r24 = S32(0X802F << 16);
    // 0x0F00357C: sw          $t6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r14;
    // 0x0F003580: lw          $t7, 0x198($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X198);
    // 0x0F003584: beql        $t7, $zero, L_0F00364C
    if (ctx->r15 == 0) {
        // 0x0F003588: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F00364C;
    }
    goto skip_0;
    // 0x0F003588: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x0F00358C: lh          $t8, -0x46A0($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X46A0);
    // 0x0F003590: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F003594: slti        $at, $t8, 0x2
    ctx->r1 = SIGNED(ctx->r24) < 0X2 ? 1 : 0;
    // 0x0F003598: bnel        $at, $zero, L_0F00364C
    if (ctx->r1 != 0) {
        // 0x0F00359C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F00364C;
    }
    goto skip_1;
    // 0x0F00359C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x0F0035A0: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x0F0035A4: addiu       $a3, $v1, 0xDC
    ctx->r7 = ADD32(ctx->r3, 0XDC);
    // 0x0F0035A8: lw          $v0, 0xBC($a3)
    ctx->r2 = MEM_W(ctx->r7, 0XBC);
    // 0x0F0035AC: lw          $t9, 0x34($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X34);
    // 0x0F0035B0: addiu       $at, $zero, 0x18
    ctx->r1 = ADD32(0, 0X18);
    // 0x0F0035B4: addiu       $a2, $sp, 0x38
    ctx->r6 = ADD32(ctx->r29, 0X38);
    // 0x0F0035B8: lw          $t0, 0x4($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X4);
    // 0x0F0035BC: lui         $t9, 0x802E
    ctx->r25 = S32(0X802E << 16);
    // 0x0F0035C0: addiu       $t9, $t9, 0x6B0C
    ctx->r25 = ADD32(ctx->r25, 0X6B0C);
    // 0x0F0035C4: bne         $t0, $at, L_0F0035DC
    if (ctx->r8 != ctx->r1) {
        // 0x0F0035C8: lw          $a1, 0x28($v0)
        ctx->r5 = MEM_W(ctx->r2, 0X28);
            goto L_0F0035DC;
    }
    // 0x0F0035C8: lw          $a1, 0x28($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X28);
    // 0x0F0035CC: beq         $a1, $zero, L_0F003648
    if (ctx->r5 == 0) {
        // 0x0F0035D0: addiu       $t1, $zero, 0x1
        ctx->r9 = ADD32(0, 0X1);
            goto L_0F003648;
    }
    // 0x0F0035D0: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x0F0035D4: b           L_0F003648
    // 0x0F0035D8: sw          $t1, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->r9;
        goto L_0F003648;
    // 0x0F0035D8: sw          $t1, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->r9;
L_0F0035DC:
    // 0x0F0035DC: beql        $a1, $zero, L_0F0035F4
    if (ctx->r5 == 0) {
        // 0x0F0035E0: addiu       $a1, $sp, 0x44
        ctx->r5 = ADD32(ctx->r29, 0X44);
            goto L_0F0035F4;
    }
    goto skip_2;
    // 0x0F0035E0: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    skip_2:
    // 0x0F0035E4: sw          $zero, 0x54($v0)
    MEM_W(0X54, ctx->r2) = 0;
    // 0x0F0035E8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F0035EC: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x0F0035F0: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
L_0F0035F4:
    // 0x0F0035F4: jalr        $t9
    // 0x0F0035F8: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0035F8: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    after_0:
    // 0x0F0035FC: lui         $v1, 0x8004
    ctx->r3 = S32(0X8004 << 16);
    // 0x0F003600: addiu       $v1, $v1, 0x6D28
    ctx->r3 = ADD32(ctx->r3, 0X6D28);
    // 0x0F003604: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    // 0x0F003608: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x0F00360C: jalr        $v1
    // 0x0F003610: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_1;
    // 0x0F003610: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    after_1:
    // 0x0F003614: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x0F003618: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x0F00361C: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x0F003620: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F003624: sh          $v0, 0x56($sp)
    MEM_H(0X56, ctx->r29) = ctx->r2;
    // 0x0F003628: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    // 0x0F00362C: jalr        $v1
    // 0x0F003630: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_2;
    // 0x0F003630: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    after_2:
    // 0x0F003634: lw          $t4, 0x24($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X24);
    // 0x0F003638: lh          $t2, 0x56($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X56);
    // 0x0F00363C: lw          $t5, 0xBC($t4)
    ctx->r13 = MEM_W(ctx->r12, 0XBC);
    // 0x0F003640: subu        $t3, $v0, $t2
    ctx->r11 = SUB32(ctx->r2, ctx->r10);
    // 0x0F003644: sh          $t3, 0x50($t5)
    MEM_H(0X50, ctx->r13) = ctx->r11;
L_0F003648:
    // 0x0F003648: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F00364C:
    // 0x0F00364C: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
    // 0x0F003650: jr          $ra
    // 0x0F003654: nop

    return;
    // 0x0F003654: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003658(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003658: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F00365C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003660: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F003664: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x0F003668: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x0F00366C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F003670: jalr        $t9
    // 0x0F003674: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F003674: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    after_0:
    // 0x0F003678: beq         $v0, $zero, L_0F003694
    if (ctx->r2 == 0) {
        // 0x0F00367C: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_0F003694;
    }
    // 0x0F00367C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F003680: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F003684: jalr        $t9
    // 0x0F003688: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F003688: nop

    after_1:
    // 0x0F00368C: b           L_0F0036F0
    // 0x0F003690: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_0F0036F0;
    // 0x0F003690: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F003694:
    // 0x0F003694: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F003698: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F00369C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F0036A0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F0036A4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F0036A8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F0036AC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F0036B0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F0036B4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F0036B8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F0036BC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F0036C0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F0036C4: lbu         $t0, 0x9($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X9);
    // 0x0F0036C8: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x0F0036CC: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x0F0036D0: lw          $t9, 0x5908($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X5908);
    // 0x0F0036D4: jalr        $t9
    // 0x0F0036D8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0036D8: nop

    after_2:
    // 0x0F0036DC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F0036E0: lh          $t2, 0xE($a0)
    ctx->r10 = MEM_H(ctx->r4, 0XE);
    // 0x0F0036E4: addiu       $t3, $t2, -0x1
    ctx->r11 = ADD32(ctx->r10, -0X1);
    // 0x0F0036E8: sh          $t3, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r11;
    // 0x0F0036EC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F0036F0:
    // 0x0F0036F0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0036F4: jr          $ra
    // 0x0F0036F8: nop

    return;
    // 0x0F0036F8: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F0036FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0036FC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F003700: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003704: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F003708: addiu       $a1, $zero, 0xE0
    ctx->r5 = ADD32(0, 0XE0);
    // 0x0F00370C: lui         $a2, 0x3F80
    ctx->r6 = S32(0X3F80 << 16);
    // 0x0F003710: jal         0x0F000F80
    // 0x0F003714: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    ni_ovl_047_func_0F000F80(rdram, ctx);
        goto after_0;
    // 0x0F003714: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x0F003718: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x0F00371C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003720: addiu       $t9, $t9, 0x1E64
    ctx->r25 = ADD32(ctx->r25, 0X1E64);
    // 0x0F003724: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x0F003728: addiu       $a3, $zero, 0xB
    ctx->r7 = ADD32(0, 0XB);
    // 0x0F00372C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x0F003730: jalr        $t9
    // 0x0F003734: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F003734: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x0F003738: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F00373C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F003740: jr          $ra
    // 0x0F003744: nop

    return;
    // 0x0F003744: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003748(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003748: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F00374C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003750: jal         0x0F000FE4
    // 0x0F003754: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    ni_ovl_047_func_0F000FE4(rdram, ctx);
        goto after_0;
    // 0x0F003754: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x0F003758: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x0F00375C: lw          $v0, 0x4C($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4C);
    // 0x0F003760: beql        $v0, $zero, L_0F003788
    if (ctx->r2 == 0) {
        // 0x0F003764: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F003788;
    }
    goto skip_0;
    // 0x0F003764: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x0F003768: lw          $t6, 0x3C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X3C);
    // 0x0F00376C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003770: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F003774: beq         $t6, $zero, L_0F003784
    if (ctx->r14 == 0) {
        // 0x0F003778: addiu       $a0, $a2, 0x8
        ctx->r4 = ADD32(ctx->r6, 0X8);
            goto L_0F003784;
    }
    // 0x0F003778: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F00377C: jalr        $t9
    // 0x0F003780: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F003780: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_0F003784:
    // 0x0F003784: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F003788:
    // 0x0F003788: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F00378C: jr          $ra
    // 0x0F003790: nop

    return;
    // 0x0F003790: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003794(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003794: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F003798: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F00379C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x0F0037A0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x0F0037A4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F0037A8: jal         0x0F000FE4
    // 0x0F0037AC: lw          $s1, 0x70($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X70);
    ni_ovl_047_func_0F000FE4(rdram, ctx);
        goto after_0;
    // 0x0F0037AC: lw          $s1, 0x70($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X70);
    after_0:
    // 0x0F0037B0: lhu         $v1, 0x14($s1)
    ctx->r3 = MEM_HU(ctx->r17, 0X14);
    // 0x0F0037B4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x0F0037B8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0037BC: beq         $v1, $zero, L_0F0037E0
    if (ctx->r3 == 0) {
        // 0x0F0037C0: or          $a1, $v1, $zero
        ctx->r5 = ctx->r3 | 0;
            goto L_0F0037E0;
    }
    // 0x0F0037C0: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    // 0x0F0037C4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F0037C8: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x0F0037CC: jalr        $t9
    // 0x0F0037D0: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0037D0: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_1:
    // 0x0F0037D4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x0F0037D8: b           L_0F0037E0
    // 0x0F0037DC: lhu         $v1, 0x14($s1)
    ctx->r3 = MEM_HU(ctx->r17, 0X14);
        goto L_0F0037E0;
    // 0x0F0037DC: lhu         $v1, 0x14($s1)
    ctx->r3 = MEM_HU(ctx->r17, 0X14);
L_0F0037E0:
    // 0x0F0037E0: beq         $a0, $zero, L_0F003850
    if (ctx->r4 == 0) {
        // 0x0F0037E4: addiu       $at, $zero, 0x2A4
        ctx->r1 = ADD32(0, 0X2A4);
            goto L_0F003850;
    }
    // 0x0F0037E4: addiu       $at, $zero, 0x2A4
    ctx->r1 = ADD32(0, 0X2A4);
    // 0x0F0037E8: addiu       $at, $zero, 0x2A4
    ctx->r1 = ADD32(0, 0X2A4);
    // 0x0F0037EC: bne         $v1, $at, L_0F00380C
    if (ctx->r3 != ctx->r1) {
        // 0x0F0037F0: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_0F00380C;
    }
    // 0x0F0037F0: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x0F0037F4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F0037F8: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F0037FC: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    // 0x0F003800: jalr        $t9
    // 0x0F003804: addiu       $a1, $zero, 0x11
    ctx->r5 = ADD32(0, 0X11);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F003804: addiu       $a1, $zero, 0x11
    ctx->r5 = ADD32(0, 0X11);
    after_2:
    // 0x0F003808: lhu         $v0, 0x14($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X14);
L_0F00380C:
    // 0x0F00380C: addiu       $at, $zero, 0x2A6
    ctx->r1 = ADD32(0, 0X2A6);
    // 0x0F003810: bne         $v0, $at, L_0F00382C
    if (ctx->r2 != ctx->r1) {
        // 0x0F003814: addiu       $a1, $zero, 0x17
        ctx->r5 = ADD32(0, 0X17);
            goto L_0F00382C;
    }
    // 0x0F003814: addiu       $a1, $zero, 0x17
    ctx->r5 = ADD32(0, 0X17);
    // 0x0F003818: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F00381C: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F003820: jalr        $t9
    // 0x0F003824: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F003824: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    after_3:
    // 0x0F003828: lhu         $v0, 0x14($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X14);
L_0F00382C:
    // 0x0F00382C: addiu       $at, $zero, 0x2A8
    ctx->r1 = ADD32(0, 0X2A8);
    // 0x0F003830: bne         $v0, $at, L_0F0038AC
    if (ctx->r2 != ctx->r1) {
        // 0x0F003834: addiu       $a1, $zero, 0xC
        ctx->r5 = ADD32(0, 0XC);
            goto L_0F0038AC;
    }
    // 0x0F003834: addiu       $a1, $zero, 0xC
    ctx->r5 = ADD32(0, 0XC);
    // 0x0F003838: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F00383C: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F003840: jalr        $t9
    // 0x0F003844: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F003844: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    after_4:
    // 0x0F003848: b           L_0F0038AC
    // 0x0F00384C: nop

        goto L_0F0038AC;
    // 0x0F00384C: nop

L_0F003850:
    // 0x0F003850: bne         $v1, $at, L_0F003870
    if (ctx->r3 != ctx->r1) {
        // 0x0F003854: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_0F003870;
    }
    // 0x0F003854: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x0F003858: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F00385C: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F003860: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    // 0x0F003864: jalr        $t9
    // 0x0F003868: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F003868: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    after_5:
    // 0x0F00386C: lhu         $v0, 0x14($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X14);
L_0F003870:
    // 0x0F003870: addiu       $at, $zero, 0x2A6
    ctx->r1 = ADD32(0, 0X2A6);
    // 0x0F003874: bne         $v0, $at, L_0F003890
    if (ctx->r2 != ctx->r1) {
        // 0x0F003878: addiu       $a1, $zero, 0x16
        ctx->r5 = ADD32(0, 0X16);
            goto L_0F003890;
    }
    // 0x0F003878: addiu       $a1, $zero, 0x16
    ctx->r5 = ADD32(0, 0X16);
    // 0x0F00387C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F003880: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F003884: jalr        $t9
    // 0x0F003888: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F003888: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    after_6:
    // 0x0F00388C: lhu         $v0, 0x14($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X14);
L_0F003890:
    // 0x0F003890: addiu       $at, $zero, 0x2A8
    ctx->r1 = ADD32(0, 0X2A8);
    // 0x0F003894: bne         $v0, $at, L_0F0038AC
    if (ctx->r2 != ctx->r1) {
        // 0x0F003898: addiu       $a1, $zero, 0xB
        ctx->r5 = ADD32(0, 0XB);
            goto L_0F0038AC;
    }
    // 0x0F003898: addiu       $a1, $zero, 0xB
    ctx->r5 = ADD32(0, 0XB);
    // 0x0F00389C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F0038A0: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F0038A4: jalr        $t9
    // 0x0F0038A8: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F0038A8: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    after_7:
L_0F0038AC:
    // 0x0F0038AC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0038B0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0038B4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0038B8: jalr        $t9
    // 0x0F0038BC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F0038BC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_8:
    // 0x0F0038C0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x0F0038C4: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x0F0038C8: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x0F0038CC: jr          $ra
    // 0x0F0038D0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x0F0038D0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void ni_ovl_047_func_0F0038D4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0038D4: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x0F0038D8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F0038DC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F0038E0: lw          $s0, 0x24($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X24);
    // 0x0F0038E4: jal         0x0F000FE4
    // 0x0F0038E8: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    ni_ovl_047_func_0F000FE4(rdram, ctx);
        goto after_0;
    // 0x0F0038E8: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    after_0:
    // 0x0F0038EC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F0038F0: lw          $a0, -0x53E0($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E0);
    // 0x0F0038F4: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0038F8: addiu       $t9, $t9, 0x6D28
    ctx->r25 = ADD32(ctx->r25, 0X6D28);
    // 0x0F0038FC: addiu       $a1, $s0, 0x50
    ctx->r5 = ADD32(ctx->r16, 0X50);
    // 0x0F003900: jalr        $t9
    // 0x0F003904: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F003904: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    after_1:
    // 0x0F003908: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F00390C: sh          $v0, 0x26($sp)
    MEM_H(0X26, ctx->r29) = ctx->r2;
    // 0x0F003910: addiu       $t9, $t9, 0x6CE0
    ctx->r25 = ADD32(ctx->r25, 0X6CE0);
    // 0x0F003914: sll         $a0, $v0, 16
    ctx->r4 = S32(ctx->r2 << 16);
    // 0x0F003918: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x0F00391C: jalr        $t9
    // 0x0F003920: lh          $a1, 0x5E($s0)
    ctx->r5 = MEM_H(ctx->r16, 0X5E);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F003920: lh          $a1, 0x5E($s0)
    ctx->r5 = MEM_H(ctx->r16, 0X5E);
    after_2:
    // 0x0F003924: slti        $at, $v0, 0x401
    ctx->r1 = SIGNED(ctx->r2) < 0X401 ? 1 : 0;
    // 0x0F003928: beq         $at, $zero, L_0F003944
    if (ctx->r1 == 0) {
        // 0x0F00392C: lw          $v0, 0x28($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X28);
            goto L_0F003944;
    }
    // 0x0F00392C: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x0F003930: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003934: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F003938: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x0F00393C: jalr        $t9
    // 0x0F003940: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F003940: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_3:
L_0F003944:
    // 0x0F003944: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F003948: addiu       $t9, $t9, 0x6BF0
    ctx->r25 = ADD32(ctx->r25, 0X6BF0);
    // 0x0F00394C: lh          $a0, 0x26($sp)
    ctx->r4 = MEM_H(ctx->r29, 0X26);
    // 0x0F003950: lh          $a1, 0x5E($s0)
    ctx->r5 = MEM_H(ctx->r16, 0X5E);
    // 0x0F003954: jalr        $t9
    // 0x0F003958: addiu       $a2, $zero, 0x400
    ctx->r6 = ADD32(0, 0X400);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F003958: addiu       $a2, $zero, 0x400
    ctx->r6 = ADD32(0, 0X400);
    after_4:
    // 0x0F00395C: sh          $v0, 0x5E($s0)
    MEM_H(0X5E, ctx->r16) = ctx->r2;
    // 0x0F003960: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x0F003964: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F003968: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x0F00396C: jr          $ra
    // 0x0F003970: nop

    return;
    // 0x0F003970: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003974(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003974: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F003978: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F00397C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F003980: lw          $s0, 0x70($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X70);
    // 0x0F003984: jal         0x0F000FE4
    // 0x0F003988: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    ni_ovl_047_func_0F000FE4(rdram, ctx);
        goto after_0;
    // 0x0F003988: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    after_0:
    // 0x0F00398C: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    // 0x0F003990: lw          $v0, 0x4C($t6)
    ctx->r2 = MEM_W(ctx->r14, 0X4C);
    // 0x0F003994: lw          $t7, 0x3C($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X3C);
    // 0x0F003998: bnel        $t7, $zero, L_0F003A60
    if (ctx->r15 != 0) {
        // 0x0F00399C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_0F003A60;
    }
    goto skip_0;
    // 0x0F00399C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x0F0039A0: lhu         $v0, 0x14($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X14);
    // 0x0F0039A4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F0039A8: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x0F0039AC: beq         $v0, $zero, L_0F0039CC
    if (ctx->r2 == 0) {
        // 0x0F0039B0: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_0F0039CC;
    }
    // 0x0F0039B0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x0F0039B4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0039B8: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x0F0039BC: jalr        $t9
    // 0x0F0039C0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0039C0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_1:
    // 0x0F0039C4: b           L_0F0039CC
    // 0x0F0039C8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_0F0039CC;
    // 0x0F0039C8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_0F0039CC:
    // 0x0F0039CC: bnel        $v1, $zero, L_0F003A44
    if (ctx->r3 != 0) {
        // 0x0F0039D0: lw          $t8, 0x20($sp)
        ctx->r24 = MEM_W(ctx->r29, 0X20);
            goto L_0F003A44;
    }
    goto skip_1;
    // 0x0F0039D0: lw          $t8, 0x20($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X20);
    skip_1:
    // 0x0F0039D4: lhu         $a1, 0x14($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X14);
    // 0x0F0039D8: addiu       $at, $zero, 0x2A4
    ctx->r1 = ADD32(0, 0X2A4);
    // 0x0F0039DC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0039E0: bne         $a1, $at, L_0F0039F4
    if (ctx->r5 != ctx->r1) {
        // 0x0F0039E4: addiu       $t9, $t9, 0x66F8
        ctx->r25 = ADD32(ctx->r25, 0X66F8);
            goto L_0F0039F4;
    }
    // 0x0F0039E4: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F0039E8: jalr        $t9
    // 0x0F0039EC: addiu       $a0, $zero, 0x41C
    ctx->r4 = ADD32(0, 0X41C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0039EC: addiu       $a0, $zero, 0x41C
    ctx->r4 = ADD32(0, 0X41C);
    after_2:
    // 0x0F0039F0: lhu         $a1, 0x14($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X14);
L_0F0039F4:
    // 0x0F0039F4: addiu       $at, $zero, 0x2A6
    ctx->r1 = ADD32(0, 0X2A6);
    // 0x0F0039F8: bne         $a1, $at, L_0F003A10
    if (ctx->r5 != ctx->r1) {
        // 0x0F0039FC: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_0F003A10;
    }
    // 0x0F0039FC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F003A00: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F003A04: jalr        $t9
    // 0x0F003A08: addiu       $a0, $zero, 0x1F3
    ctx->r4 = ADD32(0, 0X1F3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F003A08: addiu       $a0, $zero, 0x1F3
    ctx->r4 = ADD32(0, 0X1F3);
    after_3:
    // 0x0F003A0C: lhu         $a1, 0x14($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X14);
L_0F003A10:
    // 0x0F003A10: addiu       $at, $zero, 0x2A8
    ctx->r1 = ADD32(0, 0X2A8);
    // 0x0F003A14: bne         $a1, $at, L_0F003A2C
    if (ctx->r5 != ctx->r1) {
        // 0x0F003A18: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_0F003A2C;
    }
    // 0x0F003A18: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F003A1C: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F003A20: jalr        $t9
    // 0x0F003A24: addiu       $a0, $zero, 0x41E
    ctx->r4 = ADD32(0, 0X41E);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F003A24: addiu       $a0, $zero, 0x41E
    ctx->r4 = ADD32(0, 0X41E);
    after_4:
    // 0x0F003A28: lhu         $a1, 0x14($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X14);
L_0F003A2C:
    // 0x0F003A2C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003A30: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x0F003A34: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F003A38: jalr        $t9
    // 0x0F003A3C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F003A3C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_5:
    // 0x0F003A40: lw          $t8, 0x20($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X20);
L_0F003A44:
    // 0x0F003A44: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F003A48: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x0F003A4C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F003A50: addiu       $a0, $t8, 0x8
    ctx->r4 = ADD32(ctx->r24, 0X8);
    // 0x0F003A54: jalr        $t9
    // 0x0F003A58: addiu       $a1, $t8, 0xE
    ctx->r5 = ADD32(ctx->r24, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F003A58: addiu       $a1, $t8, 0xE
    ctx->r5 = ADD32(ctx->r24, 0XE);
    after_6:
    // 0x0F003A5C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F003A60:
    // 0x0F003A60: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F003A64: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F003A68: jr          $ra
    // 0x0F003A6C: nop

    return;
    // 0x0F003A6C: nop

;}
RECOMP_FUNC void ni_ovl_047_func_0F003A70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F003A70: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x0F003A74: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F003A78: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x0F003A7C: lw          $a2, 0x24($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X24);
    // 0x0F003A80: lw          $t6, 0x194($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X194);
    // 0x0F003A84: addiu       $a3, $v0, 0xDC
    ctx->r7 = ADD32(ctx->r2, 0XDC);
    // 0x0F003A88: beql        $t6, $zero, L_0F003BAC
    if (ctx->r14 == 0) {
        // 0x0F003A8C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F003BAC;
    }
    goto skip_0;
    // 0x0F003A8C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x0F003A90: lw          $t7, 0xB8($a3)
    ctx->r15 = MEM_W(ctx->r7, 0XB8);
    // 0x0F003A94: lui         $v0, 0x8004
    ctx->r2 = S32(0X8004 << 16);
    // 0x0F003A98: addiu       $v0, $v0, 0x6E98
    ctx->r2 = ADD32(ctx->r2, 0X6E98);
    // 0x0F003A9C: lw          $v1, 0x24($t7)
    ctx->r3 = MEM_W(ctx->r15, 0X24);
    // 0x0F003AA0: addiu       $a0, $a2, 0x50
    ctx->r4 = ADD32(ctx->r6, 0X50);
    // 0x0F003AA4: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x0F003AA8: beql        $v1, $zero, L_0F003BAC
    if (ctx->r3 == 0) {
        // 0x0F003AAC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F003BAC;
    }
    goto skip_1;
    // 0x0F003AAC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x0F003AB0: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x0F003AB4: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x0F003AB8: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    // 0x0F003ABC: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x0F003AC0: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    // 0x0F003AC4: jalr        $v0
    // 0x0F003AC8: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_0;
    // 0x0F003AC8: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_0:
    // 0x0F003ACC: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x0F003AD0: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F003AD4: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x0F003AD8: lw          $v1, 0x3C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X3C);
    // 0x0F003ADC: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x0F003AE0: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x0F003AE4: bc1tl       L_0F003BAC
    if (c1cs) {
        // 0x0F003AE8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F003BAC;
    }
    goto skip_2;
    // 0x0F003AE8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x0F003AEC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F003AF0: addiu       $a0, $v1, 0x50
    ctx->r4 = ADD32(ctx->r3, 0X50);
    // 0x0F003AF4: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x0F003AF8: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x0F003AFC: nop

    // 0x0F003B00: bc1tl       L_0F003BAC
    if (c1cs) {
        // 0x0F003B04: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F003BAC;
    }
    goto skip_3;
    // 0x0F003B04: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_3:
    // 0x0F003B08: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x0F003B0C: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    // 0x0F003B10: jalr        $v0
    // 0x0F003B14: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_1;
    // 0x0F003B14: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_1:
    // 0x0F003B18: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x0F003B1C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F003B20: lui         $v1, 0x8004
    ctx->r3 = S32(0X8004 << 16);
    // 0x0F003B24: addiu       $v1, $v1, 0x6D28
    ctx->r3 = ADD32(ctx->r3, 0X6D28);
    // 0x0F003B28: c.lt.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl < ctx->f0.fl;
    // 0x0F003B2C: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x0F003B30: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x0F003B34: bc1tl       L_0F003BAC
    if (c1cs) {
        // 0x0F003B38: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F003BAC;
    }
    goto skip_4;
    // 0x0F003B38: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_4:
    // 0x0F003B3C: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x0F003B40: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    // 0x0F003B44: jalr        $v1
    // 0x0F003B48: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_2;
    // 0x0F003B48: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_2:
    // 0x0F003B4C: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x0F003B50: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x0F003B54: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x0F003B58: sh          $v0, 0x4E($sp)
    MEM_H(0X4E, ctx->r29) = ctx->r2;
    // 0x0F003B5C: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x0F003B60: jalr        $v1
    // 0x0F003B64: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_3;
    // 0x0F003B64: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_3:
    // 0x0F003B68: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F003B6C: addiu       $t9, $t9, 0x6CE0
    ctx->r25 = ADD32(ctx->r25, 0X6CE0);
    // 0x0F003B70: sll         $a1, $v0, 16
    ctx->r5 = S32(ctx->r2 << 16);
    // 0x0F003B74: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x0F003B78: jalr        $t9
    // 0x0F003B7C: lh          $a0, 0x4E($sp)
    ctx->r4 = MEM_H(ctx->r29, 0X4E);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F003B7C: lh          $a0, 0x4E($sp)
    ctx->r4 = MEM_H(ctx->r29, 0X4E);
    after_4:
    // 0x0F003B80: slti        $at, $v0, 0x4001
    ctx->r1 = SIGNED(ctx->r2) < 0X4001 ? 1 : 0;
    // 0x0F003B84: beq         $at, $zero, L_0F003BA8
    if (ctx->r1 == 0) {
        // 0x0F003B88: addiu       $a0, $zero, 0x362
        ctx->r4 = ADD32(0, 0X362);
            goto L_0F003BA8;
    }
    // 0x0F003B88: addiu       $a0, $zero, 0x362
    ctx->r4 = ADD32(0, 0X362);
    // 0x0F003B8C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F003B90: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x0F003B94: jalr        $t9
    // 0x0F003B98: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F003B98: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    after_5:
    // 0x0F003B9C: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
    // 0x0F003BA0: addiu       $t8, $zero, 0x63
    ctx->r24 = ADD32(0, 0X63);
    // 0x0F003BA4: sh          $t8, 0xD0($t0)
    MEM_H(0XD0, ctx->r8) = ctx->r24;
L_0F003BA8:
    // 0x0F003BA8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F003BAC:
    // 0x0F003BAC: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    // 0x0F003BB0: jr          $ra
    // 0x0F003BB4: nop

    return;
    // 0x0F003BB4: nop

;}
RECOMP_FUNC void ni_ovl_048_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0x3ED8($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3ED8);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_048_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00008C: addiu       $a2, $zero, 0x14C
    ctx->r6 = ADD32(0, 0X14C);
    // 0x0F000090: jalr        $t9
    // 0x0F000094: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000094: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x0F000098: bne         $v0, $zero, L_0F0000B8
    if (ctx->r2 != 0) {
        // 0x0F00009C: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_0F0000B8;
    }
    // 0x0F00009C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0000A0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x0F0000A4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0000A8: jalr        $t9
    // 0x0F0000AC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000AC: nop

    after_1:
    // 0x0F0000B0: b           L_0F0000D8
    // 0x0F0000B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F0000D8;
    // 0x0F0000B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000B8:
    // 0x0F0000B8: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x0F0000BC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000C0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0000C4: ori         $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 | 0X1000;
    // 0x0F0000C8: sh          $t7, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r15;
    // 0x0F0000CC: jalr        $t9
    // 0x0F0000D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0000D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x0F0000D4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000D8:
    // 0x0F0000D8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F0000DC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F0000E0: jr          $ra
    // 0x0F0000E4: nop

    return;
    // 0x0F0000E4: nop

;}
RECOMP_FUNC void ni_ovl_048_func_0F0000E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0000E8: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x0F0000EC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F0000F0: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x0F0000F4: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x0F0000F8: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x0F0000FC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000100: addiu       $t9, $t9, 0x34AC
    ctx->r25 = ADD32(ctx->r25, 0X34AC);
    // 0x0F000104: sw          $t6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r14;
    // 0x0F000108: lw          $t7, 0x70($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X70);
    // 0x0F00010C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F000110: jalr        $t9
    // 0x0F000114: sw          $t7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000114: sw          $t7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r15;
    after_0:
    // 0x0F000118: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00011C: sw          $v0, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->r2;
    // 0x0F000120: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x0F000124: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F000128: addiu       $a2, $a2, 0x600
    ctx->r6 = ADD32(ctx->r6, 0X600);
    // 0x0F00012C: lw          $a1, 0x24($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X24);
    // 0x0F000130: jalr        $t9
    // 0x0F000134: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000134: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_1:
    // 0x0F000138: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x0F00013C: addiu       $t9, $t9, -0x6C40
    ctx->r25 = ADD32(ctx->r25, -0X6C40);
    // 0x0F000140: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000144: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000148: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x0F00014C: jalr        $t9
    // 0x0F000150: addiu       $a0, $a0, 0x3EA8
    ctx->r4 = ADD32(ctx->r4, 0X3EA8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000150: addiu       $a0, $a0, 0x3EA8
    ctx->r4 = ADD32(ctx->r4, 0X3EA8);
    after_2:
    // 0x0F000154: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x0F000158: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
    // 0x0F00015C: lw          $t4, 0x14($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X14);
    // 0x0F000160: addiu       $at, $zero, -0x7E00
    ctx->r1 = ADD32(0, -0X7E00);
    // 0x0F000164: or          $t0, $t8, $at
    ctx->r8 = ctx->r24 | ctx->r1;
    // 0x0F000168: ori         $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 | 0X1000;
    // 0x0F00016C: sw          $v0, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r2;
    // 0x0F000170: sh          $t0, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r8;
    // 0x0F000174: sh          $t2, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r10;
    // 0x0F000178: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F00017C: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000180: lui         $t3, 0x600
    ctx->r11 = S32(0X600 << 16);
    // 0x0F000184: lw          $t6, 0x10($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X10);
    // 0x0F000188: ldc1        $f0, 0x3F28($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X3F28);
    // 0x0F00018C: addiu       $t3, $t3, 0x5B68
    ctx->r11 = ADD32(ctx->r11, 0X5B68);
    // 0x0F000190: sw          $t3, 0x34($t6)
    MEM_W(0X34, ctx->r14) = ctx->r11;
    // 0x0F000194: lw          $t9, 0x14($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X14);
    // 0x0F000198: lui         $t7, 0x600
    ctx->r15 = S32(0X600 << 16);
    // 0x0F00019C: addiu       $t7, $t7, 0x5D88
    ctx->r15 = ADD32(ctx->r15, 0X5D88);
    // 0x0F0001A0: lw          $t8, 0x14($t9)
    ctx->r24 = MEM_W(ctx->r25, 0X14);
    // 0x0F0001A4: lui         $v1, 0x8000
    ctx->r3 = S32(0X8000 << 16);
    // 0x0F0001A8: addiu       $v1, $v1, 0x2410
    ctx->r3 = ADD32(ctx->r3, 0X2410);
    // 0x0F0001AC: lw          $t0, 0x10($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X10);
    // 0x0F0001B0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F0001B4: addiu       $a1, $zero, 0x22A7
    ctx->r5 = ADD32(0, 0X22A7);
    // 0x0F0001B8: lw          $t1, 0x10($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X10);
    // 0x0F0001BC: sw          $t7, 0x34($t1)
    MEM_W(0X34, ctx->r9) = ctx->r15;
    // 0x0F0001C0: lw          $t2, 0x14($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X14);
    // 0x0F0001C4: lw          $t4, 0x14($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X14);
    // 0x0F0001C8: lw          $t5, 0x10($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X10);
    // 0x0F0001CC: lw          $t3, 0x10($t5)
    ctx->r11 = MEM_W(ctx->r13, 0X10);
    // 0x0F0001D0: lw          $v0, 0x10($t3)
    ctx->r2 = MEM_W(ctx->r11, 0X10);
    // 0x0F0001D4: lwc1        $f4, 0x68($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X68);
    // 0x0F0001D8: lwc1        $f16, 0x6C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X6C);
    // 0x0F0001DC: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F0001E0: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x0F0001E4: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x0F0001E8: mul.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x0F0001EC: ori         $t9, $t6, 0x8
    ctx->r25 = ctx->r14 | 0X8;
    // 0x0F0001F0: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
    // 0x0F0001F4: mul.d       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f0.d);
    // 0x0F0001F8: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x0F0001FC: lwc1        $f8, 0x70($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X70);
    // 0x0F000200: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F000204: swc1        $f10, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f10.u32l;
    // 0x0F000208: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x0F00020C: swc1        $f6, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f6.u32l;
    // 0x0F000210: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x0F000214: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x0F000218: swc1        $f18, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f18.u32l;
    // 0x0F00021C: jalr        $v1
    // 0x0F000220: sw          $v1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_3;
    // 0x0F000220: sw          $v1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r3;
    after_3:
    // 0x0F000224: lw          $v1, 0x30($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X30);
    // 0x0F000228: lw          $t8, 0x24($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X24);
    // 0x0F00022C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000230: addiu       $a1, $zero, 0xFF
    ctx->r5 = ADD32(0, 0XFF);
    // 0x0F000234: jalr        $v1
    // 0x0F000238: sw          $t8, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r24;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_4;
    // 0x0F000238: sw          $t8, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r24;
    after_4:
    // 0x0F00023C: beq         $v0, $zero, L_0F00027C
    if (ctx->r2 == 0) {
        // 0x0F000240: sw          $v0, 0x38($s1)
        MEM_W(0X38, ctx->r17) = ctx->r2;
            goto L_0F00027C;
    }
    // 0x0F000240: sw          $v0, 0x38($s1)
    MEM_W(0X38, ctx->r17) = ctx->r2;
    // 0x0F000244: lw          $t0, 0x58($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X58);
    // 0x0F000248: lui         $at, 0x200
    ctx->r1 = S32(0X200 << 16);
    // 0x0F00024C: ori         $at, $at, 0x808
    ctx->r1 = ctx->r1 | 0X808;
    // 0x0F000250: or          $t7, $t0, $at
    ctx->r15 = ctx->r8 | ctx->r1;
    // 0x0F000254: sw          $t7, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->r15;
    // 0x0F000258: lw          $t2, 0x38($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X38);
    // 0x0F00025C: lui         $t1, 0x2
    ctx->r9 = S32(0X2 << 16);
    // 0x0F000260: ori         $t1, $t1, 0x102
    ctx->r9 = ctx->r9 | 0X102;
    // 0x0F000264: sw          $t1, 0x4C($t2)
    MEM_W(0X4C, ctx->r10) = ctx->r9;
    // 0x0F000268: lw          $t5, 0x38($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X38);
    // 0x0F00026C: lui         $t4, 0x7850
    ctx->r12 = S32(0X7850 << 16);
    // 0x0F000270: ori         $t4, $t4, 0x6438
    ctx->r12 = ctx->r12 | 0X6438;
    // 0x0F000274: b           L_0F000294
    // 0x0F000278: sw          $t4, 0x50($t5)
    MEM_W(0X50, ctx->r13) = ctx->r12;
        goto L_0F000294;
    // 0x0F000278: sw          $t4, 0x50($t5)
    MEM_W(0X50, ctx->r13) = ctx->r12;
L_0F00027C:
    // 0x0F00027C: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x0F000280: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000284: jalr        $t9
    // 0x0F000288: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000288: nop

    after_5:
    // 0x0F00028C: b           L_0F000400
    // 0x0F000290: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F000400;
    // 0x0F000290: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F000294:
    // 0x0F000294: lw          $v0, 0x38($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X38);
    // 0x0F000298: lui         $a1, 0x3FC0
    ctx->r5 = S32(0X3FC0 << 16);
    // 0x0F00029C: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x0F0002A0: beq         $v0, $zero, L_0F0002C8
    if (ctx->r2 == 0) {
        // 0x0F0002A4: or          $a3, $s1, $zero
        ctx->r7 = ctx->r17 | 0;
            goto L_0F0002C8;
    }
    // 0x0F0002A4: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x0F0002A8: lw          $t3, 0x14($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X14);
    // 0x0F0002AC: lw          $t6, 0x14($t3)
    ctx->r14 = MEM_W(ctx->r11, 0X14);
    // 0x0F0002B0: sw          $t6, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r14;
    // 0x0F0002B4: lw          $t8, 0x14($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X14);
    // 0x0F0002B8: lw          $t1, 0x38($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X38);
    // 0x0F0002BC: lw          $t0, 0x14($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X14);
    // 0x0F0002C0: lw          $t7, 0x14($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X14);
    // 0x0F0002C4: sw          $t7, 0x40($t1)
    MEM_W(0X40, ctx->r9) = ctx->r15;
L_0F0002C8:
    // 0x0F0002C8: lw          $t2, 0x14($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X14);
    // 0x0F0002CC: lui         $t9, 0x8007
    ctx->r25 = S32(0X8007 << 16);
    // 0x0F0002D0: addiu       $t9, $t9, -0x7BE0
    ctx->r25 = ADD32(ctx->r25, -0X7BE0);
    // 0x0F0002D4: jalr        $t9
    // 0x0F0002D8: lw          $a0, 0x14($t2)
    ctx->r4 = MEM_W(ctx->r10, 0X14);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F0002D8: lw          $a0, 0x14($t2)
    ctx->r4 = MEM_W(ctx->r10, 0X14);
    after_6:
    // 0x0F0002DC: lw          $v1, 0x4C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X4C);
    // 0x0F0002E0: lui         $t4, 0xF00
    ctx->r12 = S32(0XF00 << 16);
    // 0x0F0002E4: addiu       $t4, $t4, 0x3DCC
    ctx->r12 = ADD32(ctx->r12, 0X3DCC);
    // 0x0F0002E8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0002EC: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x0F0002F0: sw          $v0, 0xAC($v1)
    MEM_W(0XAC, ctx->r3) = ctx->r2;
    // 0x0F0002F4: sw          $t4, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r12;
    // 0x0F0002F8: addiu       $a0, $v1, 0x4
    ctx->r4 = ADD32(ctx->r3, 0X4);
    // 0x0F0002FC: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F000300: sw          $a0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r4;
    // 0x0F000304: sw          $v1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r3;
    // 0x0F000308: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F00030C: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F000310: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F000314: jalr        $t9
    // 0x0F000318: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000318: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_7:
    // 0x0F00031C: lw          $v1, 0x30($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X30);
    // 0x0F000320: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F000324: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000328: lbu         $t3, 0x90($v1)
    ctx->r11 = MEM_BU(ctx->r3, 0X90);
    // 0x0F00032C: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000330: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F000334: ori         $t6, $t3, 0x80
    ctx->r14 = ctx->r11 | 0X80;
    // 0x0F000338: sb          $t6, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r14;
    // 0x0F00033C: addiu       $t8, $zero, 0x4
    ctx->r24 = ADD32(0, 0X4);
    // 0x0F000340: lw          $a1, 0x0($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X0);
    // 0x0F000344: sw          $t8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r24;
    // 0x0F000348: lw          $a0, 0x2C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X2C);
    // 0x0F00034C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000350: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F000354: jalr        $t9
    // 0x0F000358: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F000358: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_8:
    // 0x0F00035C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000360: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F000364: lw          $a0, 0x2C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000368: jalr        $t9
    // 0x0F00036C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F00036C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_9:
    // 0x0F000370: lw          $v1, 0x30($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X30);
    // 0x0F000374: lw          $a0, 0x3C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3C);
    // 0x0F000378: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00037C: lbu         $t7, 0x90($v1)
    ctx->r15 = MEM_BU(ctx->r3, 0X90);
    // 0x0F000380: sw          $v0, 0x84($v1)
    MEM_W(0X84, ctx->r3) = ctx->r2;
    // 0x0F000384: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000388: ori         $t1, $t7, 0x8
    ctx->r9 = ctx->r15 | 0X8;
    // 0x0F00038C: sb          $t1, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r9;
    // 0x0F000390: lw          $t2, 0x4C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X4C);
    // 0x0F000394: addiu       $t9, $t9, 0x5668
    ctx->r25 = ADD32(ctx->r25, 0X5668);
    // 0x0F000398: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00039C: lw          $t4, 0x0($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X0);
    // 0x0F0003A0: sw          $t4, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->r12;
    // 0x0F0003A4: lwc1        $f6, 0x3F30($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X3F30);
    // 0x0F0003A8: lui         $at, 0x41A0
    ctx->r1 = S32(0X41A0 << 16);
    // 0x0F0003AC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0003B0: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x0F0003B4: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0003B8: swc1        $f6, 0xC4($v1)
    MEM_W(0XC4, ctx->r3) = ctx->f6.u32l;
    // 0x0F0003BC: swc1        $f8, 0xCC($v1)
    MEM_W(0XCC, ctx->r3) = ctx->f8.u32l;
    // 0x0F0003C0: beq         $a0, $zero, L_0F0003E0
    if (ctx->r4 == 0) {
        // 0x0F0003C4: swc1        $f10, 0xC8($v1)
        MEM_W(0XC8, ctx->r3) = ctx->f10.u32l;
            goto L_0F0003E0;
    }
    // 0x0F0003C4: swc1        $f10, 0xC8($v1)
    MEM_W(0XC8, ctx->r3) = ctx->f10.u32l;
    // 0x0F0003C8: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x0F0003CC: swc1        $f16, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->f16.u32l;
    // 0x0F0003D0: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x0F0003D4: swc1        $f18, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->f18.u32l;
    // 0x0F0003D8: lwc1        $f4, 0xC($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0XC);
    // 0x0F0003DC: swc1        $f4, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->f4.u32l;
L_0F0003E0:
    // 0x0F0003E0: jalr        $t9
    // 0x0F0003E4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x0F0003E4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_10:
    // 0x0F0003E8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0003EC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0003F0: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x0F0003F4: jalr        $t9
    // 0x0F0003F8: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x0F0003F8: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_11:
    // 0x0F0003FC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F000400:
    // 0x0F000400: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000404: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x0F000408: jr          $ra
    // 0x0F00040C: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x0F00040C: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void ni_ovl_048_func_0F000410(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000410: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000414: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000418: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F00041C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F000420: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x0F000424: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F000428: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F00042C: addiu       $t0, $zero, 0xFF
    ctx->r8 = ADD32(0, 0XFF);
    // 0x0F000430: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x0F000434: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x0F000438: lw          $t8, -0x5170($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X5170);
    // 0x0F00043C: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F000440: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x0F000444: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x0F000448: beq         $t9, $zero, L_0F000494
    if (ctx->r25 == 0) {
        // 0x0F00044C: lui         $t9, 0x8004
        ctx->r25 = S32(0X8004 << 16);
            goto L_0F000494;
    }
    // 0x0F00044C: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000450: addiu       $t9, $t9, 0x7954
    ctx->r25 = ADD32(ctx->r25, 0X7954);
    // 0x0F000454: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x0F000458: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x0F00045C: jalr        $t9
    // 0x0F000460: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000460: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    after_0:
    // 0x0F000464: beq         $v0, $zero, L_0F0004A4
    if (ctx->r2 == 0) {
        // 0x0F000468: lw          $a0, 0x24($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X24);
            goto L_0F0004A4;
    }
    // 0x0F000468: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x0F00046C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000470: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F000474: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x0F000478: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F00047C: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F000480: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F000484: jalr        $t9
    // 0x0F000488: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000488: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x0F00048C: b           L_0F0004A8
    // 0x0F000490: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F0004A8;
    // 0x0F000490: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000494:
    // 0x0F000494: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000498: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F00049C: jalr        $t9
    // 0x0F0004A0: sb          $t0, 0x1B($v0)
    MEM_B(0X1B, ctx->r2) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0004A0: sb          $t0, 0x1B($v0)
    MEM_B(0X1B, ctx->r2) = ctx->r8;
    after_2:
L_0F0004A4:
    // 0x0F0004A4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0004A8:
    // 0x0F0004A8: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F0004AC: jr          $ra
    // 0x0F0004B0: nop

    return;
    // 0x0F0004B0: nop

;}
RECOMP_FUNC void ni_ovl_048_func_0F0004B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0004B4: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x0F0004B8: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F0004BC: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F0004C0: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F0004C4: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F0004C8: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x0F0004CC: lw          $s2, 0x34($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X34);
    // 0x0F0004D0: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F0004D4: jal         0x0F0005EC
    // 0x0F0004D8: sw          $t6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r14;
    static_102_0F0005EC(rdram, ctx);
        goto after_0;
    // 0x0F0004D8: sw          $t6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r14;
    after_0:
    // 0x0F0004DC: addiu       $s0, $s2, 0x8
    ctx->r16 = ADD32(ctx->r18, 0X8);
    // 0x0F0004E0: lbu         $t8, 0x90($s0)
    ctx->r24 = MEM_BU(ctx->r16, 0X90);
    // 0x0F0004E4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0004E8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0004EC: ori         $t9, $t8, 0x80
    ctx->r25 = ctx->r24 | 0X80;
    // 0x0F0004F0: sb          $t9, 0x90($s0)
    MEM_B(0X90, ctx->r16) = ctx->r25;
    // 0x0F0004F4: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0004F8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x0F0004FC: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F000500: addiu       $a0, $s0, 0x4
    ctx->r4 = ADD32(ctx->r16, 0X4);
    // 0x0F000504: addiu       $t0, $zero, 0x4
    ctx->r8 = ADD32(0, 0X4);
    // 0x0F000508: sw          $t0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r8;
    // 0x0F00050C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x0F000510: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000514: addiu       $a3, $zero, 0xEF
    ctx->r7 = ADD32(0, 0XEF);
    // 0x0F000518: jalr        $t9
    // 0x0F00051C: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F00051C: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_1:
    // 0x0F000520: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000524: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F000528: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x0F00052C: jalr        $t9
    // 0x0F000530: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000530: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    after_2:
    // 0x0F000534: lbu         $t2, 0x90($s0)
    ctx->r10 = MEM_BU(ctx->r16, 0X90);
    // 0x0F000538: sw          $v0, 0x84($s0)
    MEM_W(0X84, ctx->r16) = ctx->r2;
    // 0x0F00053C: addiu       $v1, $s2, 0xDC
    ctx->r3 = ADD32(ctx->r18, 0XDC);
    // 0x0F000540: ori         $t3, $t2, 0x8
    ctx->r11 = ctx->r10 | 0X8;
    // 0x0F000544: sb          $t3, 0x90($s0)
    MEM_B(0X90, ctx->r16) = ctx->r11;
    // 0x0F000548: lw          $t4, 0x0($s2)
    ctx->r12 = MEM_W(ctx->r18, 0X0);
    // 0x0F00054C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000550: addiu       $t9, $t9, -0x6164
    ctx->r25 = ADD32(ctx->r25, -0X6164);
    // 0x0F000554: sw          $t4, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r12;
    // 0x0F000558: lw          $t5, 0x40($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X40);
    // 0x0F00055C: addiu       $a1, $v1, 0x8
    ctx->r5 = ADD32(ctx->r3, 0X8);
    // 0x0F000560: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000564: lw          $t6, 0x14($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X14);
    // 0x0F000568: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x0F00056C: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x0F000570: lw          $a0, 0x14($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X14);
    // 0x0F000574: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x0F000578: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x0F00057C: jalr        $t9
    // 0x0F000580: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000580: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    after_3:
    // 0x0F000584: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x0F000588: addiu       $at, $zero, -0x80D
    ctx->r1 = ADD32(0, -0X80D);
    // 0x0F00058C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000590: swc1        $f0, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->f0.u32l;
    // 0x0F000594: lw          $v0, 0x38($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X38);
    // 0x0F000598: addiu       $t9, $t9, -0x7FC4
    ctx->r25 = ADD32(ctx->r25, -0X7FC4);
    // 0x0F00059C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x0F0005A0: lw          $t8, 0x58($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X58);
    // 0x0F0005A4: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x0F0005A8: addiu       $a0, $v1, 0x1C
    ctx->r4 = ADD32(ctx->r3, 0X1C);
    // 0x0F0005AC: and         $t0, $t8, $at
    ctx->r8 = ctx->r24 & ctx->r1;
    // 0x0F0005B0: sw          $t0, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->r8;
    // 0x0F0005B4: lw          $t2, 0x38($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X38);
    // 0x0F0005B8: lw          $t1, 0x34($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X34);
    // 0x0F0005BC: jalr        $t9
    // 0x0F0005C0: sw          $t1, 0x34($t2)
    MEM_W(0X34, ctx->r10) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F0005C0: sw          $t1, 0x34($t2)
    MEM_W(0X34, ctx->r10) = ctx->r9;
    after_4:
    // 0x0F0005C4: lw          $t3, 0x40($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X40);
    // 0x0F0005C8: sh          $v0, 0x5E($t3)
    MEM_H(0X5E, ctx->r11) = ctx->r2;
    // 0x0F0005CC: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0005D0: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F0005D4: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F0005D8: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F0005DC: jr          $ra
    // 0x0F0005E0: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x0F0005E0: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void ni_ovl_048_func_0F0005E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0005E4: jr          $ra
    // 0x0F0005E8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x0F0005E8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x0F0005EC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x0F0005F0: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x0F0005F4: swc1        $f0, 0xF8($v0)
    MEM_W(0XF8, ctx->r2) = ctx->f0.u32l;
    // 0x0F0005F8: jr          $ra
    // 0x0F0005FC: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
    return;
    // 0x0F0005FC: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
;}
RECOMP_FUNC void ni_ovl_049_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0x3EA8($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3EA8);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_049_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00008C: addiu       $a2, $zero, 0x14C
    ctx->r6 = ADD32(0, 0X14C);
    // 0x0F000090: jalr        $t9
    // 0x0F000094: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000094: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x0F000098: bne         $v0, $zero, L_0F0000B8
    if (ctx->r2 != 0) {
        // 0x0F00009C: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_0F0000B8;
    }
    // 0x0F00009C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0000A0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x0F0000A4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0000A8: jalr        $t9
    // 0x0F0000AC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000AC: nop

    after_1:
    // 0x0F0000B0: b           L_0F0000D8
    // 0x0F0000B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F0000D8;
    // 0x0F0000B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000B8:
    // 0x0F0000B8: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x0F0000BC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000C0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0000C4: ori         $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 | 0X1000;
    // 0x0F0000C8: sh          $t7, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r15;
    // 0x0F0000CC: jalr        $t9
    // 0x0F0000D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0000D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x0F0000D4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000D8:
    // 0x0F0000D8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F0000DC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F0000E0: jr          $ra
    // 0x0F0000E4: nop

    return;
    // 0x0F0000E4: nop

;}
RECOMP_FUNC void ni_ovl_049_func_0F0000E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0000E8: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x0F0000EC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F0000F0: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x0F0000F4: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x0F0000F8: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x0F0000FC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000100: addiu       $t9, $t9, 0x34AC
    ctx->r25 = ADD32(ctx->r25, 0X34AC);
    // 0x0F000104: sw          $t6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r14;
    // 0x0F000108: lw          $t7, 0x70($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X70);
    // 0x0F00010C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F000110: jalr        $t9
    // 0x0F000114: sw          $t7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000114: sw          $t7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r15;
    after_0:
    // 0x0F000118: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00011C: sw          $v0, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->r2;
    // 0x0F000120: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x0F000124: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F000128: addiu       $a2, $a2, 0x5E0
    ctx->r6 = ADD32(ctx->r6, 0X5E0);
    // 0x0F00012C: lw          $a1, 0x24($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X24);
    // 0x0F000130: jalr        $t9
    // 0x0F000134: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000134: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_1:
    // 0x0F000138: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x0F00013C: addiu       $t9, $t9, -0x6C40
    ctx->r25 = ADD32(ctx->r25, -0X6C40);
    // 0x0F000140: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000144: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000148: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x0F00014C: jalr        $t9
    // 0x0F000150: addiu       $a0, $a0, 0x3E78
    ctx->r4 = ADD32(ctx->r4, 0X3E78);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000150: addiu       $a0, $a0, 0x3E78
    ctx->r4 = ADD32(ctx->r4, 0X3E78);
    after_2:
    // 0x0F000154: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x0F000158: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
    // 0x0F00015C: lw          $t4, 0x14($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X14);
    // 0x0F000160: addiu       $at, $zero, -0x7E00
    ctx->r1 = ADD32(0, -0X7E00);
    // 0x0F000164: or          $t0, $t8, $at
    ctx->r8 = ctx->r24 | ctx->r1;
    // 0x0F000168: ori         $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 | 0X1000;
    // 0x0F00016C: sw          $v0, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r2;
    // 0x0F000170: sh          $t0, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r8;
    // 0x0F000174: sh          $t2, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r10;
    // 0x0F000178: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F00017C: lui         $t3, 0x600
    ctx->r11 = S32(0X600 << 16);
    // 0x0F000180: addiu       $t3, $t3, 0x6898
    ctx->r11 = ADD32(ctx->r11, 0X6898);
    // 0x0F000184: lw          $t6, 0x10($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X10);
    // 0x0F000188: lui         $t7, 0x600
    ctx->r15 = S32(0X600 << 16);
    // 0x0F00018C: addiu       $t7, $t7, 0x6AB8
    ctx->r15 = ADD32(ctx->r15, 0X6AB8);
    // 0x0F000190: sw          $t3, 0x34($t6)
    MEM_W(0X34, ctx->r14) = ctx->r11;
    // 0x0F000194: lw          $t9, 0x14($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X14);
    // 0x0F000198: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F00019C: addiu       $a1, $zero, 0xFF
    ctx->r5 = ADD32(0, 0XFF);
    // 0x0F0001A0: lw          $t8, 0x14($t9)
    ctx->r24 = MEM_W(ctx->r25, 0X14);
    // 0x0F0001A4: lw          $t0, 0x10($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X10);
    // 0x0F0001A8: lw          $t1, 0x10($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X10);
    // 0x0F0001AC: sw          $t7, 0x34($t1)
    MEM_W(0X34, ctx->r9) = ctx->r15;
    // 0x0F0001B0: lw          $t2, 0x14($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X14);
    // 0x0F0001B4: lw          $t4, 0x14($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X14);
    // 0x0F0001B8: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F0001BC: lw          $t3, 0x10($t5)
    ctx->r11 = MEM_W(ctx->r13, 0X10);
    // 0x0F0001C0: lw          $t6, 0x10($t3)
    ctx->r14 = MEM_W(ctx->r11, 0X10);
    // 0x0F0001C4: sw          $zero, 0x38($t6)
    MEM_W(0X38, ctx->r14) = 0;
    // 0x0F0001C8: lw          $t9, 0x14($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X14);
    // 0x0F0001CC: lw          $t8, 0x14($t9)
    ctx->r24 = MEM_W(ctx->r25, 0X14);
    // 0x0F0001D0: lw          $t0, 0x14($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X14);
    // 0x0F0001D4: lw          $t7, 0x10($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X10);
    // 0x0F0001D8: lw          $t1, 0x10($t7)
    ctx->r9 = MEM_W(ctx->r15, 0X10);
    // 0x0F0001DC: lw          $t2, 0x14($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X14);
    // 0x0F0001E0: lw          $t4, 0x14($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X14);
    // 0x0F0001E4: sw          $zero, 0x38($t4)
    MEM_W(0X38, ctx->r12) = 0;
    // 0x0F0001E8: lw          $t5, 0x14($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X14);
    // 0x0F0001EC: lw          $t3, 0x14($t5)
    ctx->r11 = MEM_W(ctx->r13, 0X14);
    // 0x0F0001F0: lw          $t6, 0x14($t3)
    ctx->r14 = MEM_W(ctx->r11, 0X14);
    // 0x0F0001F4: lw          $t9, 0x10($t6)
    ctx->r25 = MEM_W(ctx->r14, 0X10);
    // 0x0F0001F8: sw          $zero, 0x38($t9)
    MEM_W(0X38, ctx->r25) = 0;
    // 0x0F0001FC: lw          $t8, 0x14($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X14);
    // 0x0F000200: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000204: addiu       $t9, $t9, 0x2410
    ctx->r25 = ADD32(ctx->r25, 0X2410);
    // 0x0F000208: lw          $t0, 0x14($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X14);
    // 0x0F00020C: lw          $t7, 0x14($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X14);
    // 0x0F000210: lw          $t1, 0x10($t7)
    ctx->r9 = MEM_W(ctx->r15, 0X10);
    // 0x0F000214: lw          $t2, 0x14($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X14);
    // 0x0F000218: lw          $t4, 0x14($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X14);
    // 0x0F00021C: jalr        $t9
    // 0x0F000220: sw          $zero, 0x38($t4)
    MEM_W(0X38, ctx->r12) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000220: sw          $zero, 0x38($t4)
    MEM_W(0X38, ctx->r12) = 0;
    after_3:
    // 0x0F000224: beq         $v0, $zero, L_0F000264
    if (ctx->r2 == 0) {
        // 0x0F000228: sw          $v0, 0x38($s1)
        MEM_W(0X38, ctx->r17) = ctx->r2;
            goto L_0F000264;
    }
    // 0x0F000228: sw          $v0, 0x38($s1)
    MEM_W(0X38, ctx->r17) = ctx->r2;
    // 0x0F00022C: lw          $t5, 0x58($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X58);
    // 0x0F000230: lui         $at, 0x200
    ctx->r1 = S32(0X200 << 16);
    // 0x0F000234: ori         $at, $at, 0x808
    ctx->r1 = ctx->r1 | 0X808;
    // 0x0F000238: or          $t3, $t5, $at
    ctx->r11 = ctx->r13 | ctx->r1;
    // 0x0F00023C: sw          $t3, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->r11;
    // 0x0F000240: lw          $t8, 0x38($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X38);
    // 0x0F000244: lui         $t6, 0x2
    ctx->r14 = S32(0X2 << 16);
    // 0x0F000248: ori         $t6, $t6, 0x102
    ctx->r14 = ctx->r14 | 0X102;
    // 0x0F00024C: sw          $t6, 0x4C($t8)
    MEM_W(0X4C, ctx->r24) = ctx->r14;
    // 0x0F000250: lw          $t7, 0x38($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X38);
    // 0x0F000254: lui         $t0, 0x7850
    ctx->r8 = S32(0X7850 << 16);
    // 0x0F000258: ori         $t0, $t0, 0x6438
    ctx->r8 = ctx->r8 | 0X6438;
    // 0x0F00025C: b           L_0F00027C
    // 0x0F000260: sw          $t0, 0x50($t7)
    MEM_W(0X50, ctx->r15) = ctx->r8;
        goto L_0F00027C;
    // 0x0F000260: sw          $t0, 0x50($t7)
    MEM_W(0X50, ctx->r15) = ctx->r8;
L_0F000264:
    // 0x0F000264: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x0F000268: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F00026C: jalr        $t9
    // 0x0F000270: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000270: nop

    after_4:
    // 0x0F000274: b           L_0F0003E8
    // 0x0F000278: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F0003E8;
    // 0x0F000278: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F00027C:
    // 0x0F00027C: lw          $v0, 0x38($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X38);
    // 0x0F000280: lui         $a1, 0x3FC0
    ctx->r5 = S32(0X3FC0 << 16);
    // 0x0F000284: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x0F000288: beq         $v0, $zero, L_0F0002B0
    if (ctx->r2 == 0) {
        // 0x0F00028C: or          $a3, $s1, $zero
        ctx->r7 = ctx->r17 | 0;
            goto L_0F0002B0;
    }
    // 0x0F00028C: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x0F000290: lw          $t1, 0x14($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X14);
    // 0x0F000294: lw          $t2, 0x14($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X14);
    // 0x0F000298: sw          $t2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r10;
    // 0x0F00029C: lw          $t4, 0x14($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X14);
    // 0x0F0002A0: lw          $t6, 0x38($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X38);
    // 0x0F0002A4: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F0002A8: lw          $t3, 0x14($t5)
    ctx->r11 = MEM_W(ctx->r13, 0X14);
    // 0x0F0002AC: sw          $t3, 0x40($t6)
    MEM_W(0X40, ctx->r14) = ctx->r11;
L_0F0002B0:
    // 0x0F0002B0: lw          $t8, 0x14($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X14);
    // 0x0F0002B4: lui         $t9, 0x8007
    ctx->r25 = S32(0X8007 << 16);
    // 0x0F0002B8: addiu       $t9, $t9, -0x7BE0
    ctx->r25 = ADD32(ctx->r25, -0X7BE0);
    // 0x0F0002BC: jalr        $t9
    // 0x0F0002C0: lw          $a0, 0x14($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X14);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F0002C0: lw          $a0, 0x14($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X14);
    after_5:
    // 0x0F0002C4: lw          $v1, 0x44($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X44);
    // 0x0F0002C8: lui         $t0, 0xF00
    ctx->r8 = S32(0XF00 << 16);
    // 0x0F0002CC: addiu       $t0, $t0, 0x3DA0
    ctx->r8 = ADD32(ctx->r8, 0X3DA0);
    // 0x0F0002D0: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0002D4: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x0F0002D8: sw          $v0, 0xAC($v1)
    MEM_W(0XAC, ctx->r3) = ctx->r2;
    // 0x0F0002DC: sw          $t0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r8;
    // 0x0F0002E0: addiu       $a0, $v1, 0x4
    ctx->r4 = ADD32(ctx->r3, 0X4);
    // 0x0F0002E4: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F0002E8: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x0F0002EC: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    // 0x0F0002F0: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F0002F4: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F0002F8: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F0002FC: jalr        $t9
    // 0x0F000300: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000300: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_6:
    // 0x0F000304: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000308: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F00030C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000310: lbu         $t1, 0x90($v1)
    ctx->r9 = MEM_BU(ctx->r3, 0X90);
    // 0x0F000314: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000318: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F00031C: ori         $t2, $t1, 0x80
    ctx->r10 = ctx->r9 | 0X80;
    // 0x0F000320: sb          $t2, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r10;
    // 0x0F000324: addiu       $t4, $zero, 0x4
    ctx->r12 = ADD32(0, 0X4);
    // 0x0F000328: lw          $a1, 0x0($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X0);
    // 0x0F00032C: sw          $t4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r12;
    // 0x0F000330: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F000334: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000338: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F00033C: jalr        $t9
    // 0x0F000340: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000340: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_7:
    // 0x0F000344: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000348: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F00034C: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F000350: jalr        $t9
    // 0x0F000354: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F000354: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_8:
    // 0x0F000358: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x0F00035C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x0F000360: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000364: lbu         $t3, 0x90($v1)
    ctx->r11 = MEM_BU(ctx->r3, 0X90);
    // 0x0F000368: sw          $v0, 0x84($v1)
    MEM_W(0X84, ctx->r3) = ctx->r2;
    // 0x0F00036C: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000370: ori         $t6, $t3, 0x8
    ctx->r14 = ctx->r11 | 0X8;
    // 0x0F000374: sb          $t6, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r14;
    // 0x0F000378: lw          $t8, 0x44($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X44);
    // 0x0F00037C: addiu       $t9, $t9, 0x5668
    ctx->r25 = ADD32(ctx->r25, 0X5668);
    // 0x0F000380: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000384: lw          $t0, 0x0($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X0);
    // 0x0F000388: sw          $t0, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->r8;
    // 0x0F00038C: lwc1        $f6, 0x3EF8($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X3EF8);
    // 0x0F000390: lui         $at, 0x41A0
    ctx->r1 = S32(0X41A0 << 16);
    // 0x0F000394: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000398: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x0F00039C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0003A0: swc1        $f6, 0xC4($v1)
    MEM_W(0XC4, ctx->r3) = ctx->f6.u32l;
    // 0x0F0003A4: swc1        $f8, 0xCC($v1)
    MEM_W(0XCC, ctx->r3) = ctx->f8.u32l;
    // 0x0F0003A8: beq         $a0, $zero, L_0F0003C8
    if (ctx->r4 == 0) {
        // 0x0F0003AC: swc1        $f10, 0xC8($v1)
        MEM_W(0XC8, ctx->r3) = ctx->f10.u32l;
            goto L_0F0003C8;
    }
    // 0x0F0003AC: swc1        $f10, 0xC8($v1)
    MEM_W(0XC8, ctx->r3) = ctx->f10.u32l;
    // 0x0F0003B0: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x0F0003B4: swc1        $f16, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->f16.u32l;
    // 0x0F0003B8: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x0F0003BC: swc1        $f18, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->f18.u32l;
    // 0x0F0003C0: lwc1        $f4, 0xC($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0XC);
    // 0x0F0003C4: swc1        $f4, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->f4.u32l;
L_0F0003C8:
    // 0x0F0003C8: jalr        $t9
    // 0x0F0003CC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F0003CC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_9:
    // 0x0F0003D0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0003D4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0003D8: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x0F0003DC: jalr        $t9
    // 0x0F0003E0: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x0F0003E0: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_10:
    // 0x0F0003E4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F0003E8:
    // 0x0F0003E8: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x0F0003EC: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x0F0003F0: jr          $ra
    // 0x0F0003F4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x0F0003F4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void ni_ovl_049_func_0F0003F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0003F8: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F0003FC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000400: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F000404: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F000408: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x0F00040C: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F000410: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F000414: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x0F000418: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x0F00041C: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x0F000420: lw          $t8, -0x5170($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X5170);
    // 0x0F000424: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F000428: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x0F00042C: beq         $t9, $zero, L_0F000474
    if (ctx->r25 == 0) {
        // 0x0F000430: lui         $t9, 0x8004
        ctx->r25 = S32(0X8004 << 16);
            goto L_0F000474;
    }
    // 0x0F000430: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000434: addiu       $t9, $t9, 0x7954
    ctx->r25 = ADD32(ctx->r25, 0X7954);
    // 0x0F000438: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x0F00043C: jalr        $t9
    // 0x0F000440: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000440: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    after_0:
    // 0x0F000444: beq         $v0, $zero, L_0F000484
    if (ctx->r2 == 0) {
        // 0x0F000448: lw          $a0, 0x24($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X24);
            goto L_0F000484;
    }
    // 0x0F000448: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x0F00044C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000450: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F000454: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x0F000458: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F00045C: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F000460: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F000464: jalr        $t9
    // 0x0F000468: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000468: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x0F00046C: b           L_0F000488
    // 0x0F000470: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F000488;
    // 0x0F000470: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000474:
    // 0x0F000474: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000478: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F00047C: jalr        $t9
    // 0x0F000480: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000480: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_2:
L_0F000484:
    // 0x0F000484: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000488:
    // 0x0F000488: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F00048C: jr          $ra
    // 0x0F000490: nop

    return;
    // 0x0F000490: nop

;}
RECOMP_FUNC void ni_ovl_049_func_0F000494(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000494: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x0F000498: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F00049C: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F0004A0: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F0004A4: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F0004A8: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x0F0004AC: lw          $s2, 0x34($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X34);
    // 0x0F0004B0: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F0004B4: jal         0x0F0005CC
    // 0x0F0004B8: sw          $t6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r14;
    static_103_0F0005CC(rdram, ctx);
        goto after_0;
    // 0x0F0004B8: sw          $t6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r14;
    after_0:
    // 0x0F0004BC: addiu       $s0, $s2, 0x8
    ctx->r16 = ADD32(ctx->r18, 0X8);
    // 0x0F0004C0: lbu         $t8, 0x90($s0)
    ctx->r24 = MEM_BU(ctx->r16, 0X90);
    // 0x0F0004C4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0004C8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0004CC: ori         $t9, $t8, 0x80
    ctx->r25 = ctx->r24 | 0X80;
    // 0x0F0004D0: sb          $t9, 0x90($s0)
    MEM_B(0X90, ctx->r16) = ctx->r25;
    // 0x0F0004D4: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0004D8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x0F0004DC: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F0004E0: addiu       $a0, $s0, 0x4
    ctx->r4 = ADD32(ctx->r16, 0X4);
    // 0x0F0004E4: addiu       $t0, $zero, 0x4
    ctx->r8 = ADD32(0, 0X4);
    // 0x0F0004E8: sw          $t0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r8;
    // 0x0F0004EC: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x0F0004F0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F0004F4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F0004F8: jalr        $t9
    // 0x0F0004FC: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0004FC: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_1:
    // 0x0F000500: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000504: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F000508: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x0F00050C: jalr        $t9
    // 0x0F000510: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000510: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    after_2:
    // 0x0F000514: lbu         $t2, 0x90($s0)
    ctx->r10 = MEM_BU(ctx->r16, 0X90);
    // 0x0F000518: sw          $v0, 0x84($s0)
    MEM_W(0X84, ctx->r16) = ctx->r2;
    // 0x0F00051C: addiu       $v1, $s2, 0xDC
    ctx->r3 = ADD32(ctx->r18, 0XDC);
    // 0x0F000520: ori         $t3, $t2, 0x8
    ctx->r11 = ctx->r10 | 0X8;
    // 0x0F000524: sb          $t3, 0x90($s0)
    MEM_B(0X90, ctx->r16) = ctx->r11;
    // 0x0F000528: lw          $t4, 0x0($s2)
    ctx->r12 = MEM_W(ctx->r18, 0X0);
    // 0x0F00052C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000530: addiu       $t9, $t9, -0x6164
    ctx->r25 = ADD32(ctx->r25, -0X6164);
    // 0x0F000534: sw          $t4, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r12;
    // 0x0F000538: lw          $t5, 0x40($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X40);
    // 0x0F00053C: addiu       $a1, $v1, 0x8
    ctx->r5 = ADD32(ctx->r3, 0X8);
    // 0x0F000540: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000544: lw          $t6, 0x14($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X14);
    // 0x0F000548: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x0F00054C: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x0F000550: lw          $a0, 0x14($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X14);
    // 0x0F000554: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x0F000558: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x0F00055C: jalr        $t9
    // 0x0F000560: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000560: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    after_3:
    // 0x0F000564: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x0F000568: addiu       $at, $zero, -0x80D
    ctx->r1 = ADD32(0, -0X80D);
    // 0x0F00056C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000570: swc1        $f0, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->f0.u32l;
    // 0x0F000574: lw          $v0, 0x38($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X38);
    // 0x0F000578: addiu       $t9, $t9, -0x7FC4
    ctx->r25 = ADD32(ctx->r25, -0X7FC4);
    // 0x0F00057C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x0F000580: lw          $t8, 0x58($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X58);
    // 0x0F000584: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x0F000588: addiu       $a0, $v1, 0x1C
    ctx->r4 = ADD32(ctx->r3, 0X1C);
    // 0x0F00058C: and         $t0, $t8, $at
    ctx->r8 = ctx->r24 & ctx->r1;
    // 0x0F000590: sw          $t0, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->r8;
    // 0x0F000594: lw          $t2, 0x38($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X38);
    // 0x0F000598: lw          $t1, 0x34($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X34);
    // 0x0F00059C: jalr        $t9
    // 0x0F0005A0: sw          $t1, 0x34($t2)
    MEM_W(0X34, ctx->r10) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F0005A0: sw          $t1, 0x34($t2)
    MEM_W(0X34, ctx->r10) = ctx->r9;
    after_4:
    // 0x0F0005A4: lw          $t3, 0x40($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X40);
    // 0x0F0005A8: sh          $v0, 0x5E($t3)
    MEM_H(0X5E, ctx->r11) = ctx->r2;
    // 0x0F0005AC: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0005B0: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F0005B4: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F0005B8: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F0005BC: jr          $ra
    // 0x0F0005C0: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x0F0005C0: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void ni_ovl_049_func_0F0005C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0005C4: jr          $ra
    // 0x0F0005C8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x0F0005C8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x0F0005CC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x0F0005D0: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x0F0005D4: swc1        $f0, 0xF8($v0)
    MEM_W(0XF8, ctx->r2) = ctx->f0.u32l;
    // 0x0F0005D8: jr          $ra
    // 0x0F0005DC: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
    return;
    // 0x0F0005DC: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
;}
RECOMP_FUNC void ni_ovl_050_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0xD90($t9)
    ctx->r25 = MEM_W(ctx->r25, 0XD90);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_050_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00008C: addiu       $a2, $zero, 0x14C
    ctx->r6 = ADD32(0, 0X14C);
    // 0x0F000090: jalr        $t9
    // 0x0F000094: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000094: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x0F000098: bne         $v0, $zero, L_0F0000B8
    if (ctx->r2 != 0) {
        // 0x0F00009C: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_0F0000B8;
    }
    // 0x0F00009C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0000A0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x0F0000A4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0000A8: jalr        $t9
    // 0x0F0000AC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000AC: nop

    after_1:
    // 0x0F0000B0: b           L_0F0000D8
    // 0x0F0000B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F0000D8;
    // 0x0F0000B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000B8:
    // 0x0F0000B8: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x0F0000BC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000C0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0000C4: ori         $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 | 0X1000;
    // 0x0F0000C8: sh          $t7, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r15;
    // 0x0F0000CC: jalr        $t9
    // 0x0F0000D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0000D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x0F0000D4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000D8:
    // 0x0F0000D8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F0000DC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F0000E0: jr          $ra
    // 0x0F0000E4: nop

    return;
    // 0x0F0000E4: nop

;}
RECOMP_FUNC void ni_ovl_050_func_0F0000E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0000E8: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x0F0000EC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F0000F0: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x0F0000F4: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x0F0000F8: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x0F0000FC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000100: addiu       $t9, $t9, 0x34AC
    ctx->r25 = ADD32(ctx->r25, 0X34AC);
    // 0x0F000104: sw          $t6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r14;
    // 0x0F000108: lw          $t7, 0x70($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X70);
    // 0x0F00010C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000110: jalr        $t9
    // 0x0F000114: sw          $t7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000114: sw          $t7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r15;
    after_0:
    // 0x0F000118: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00011C: sw          $v0, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->r2;
    // 0x0F000120: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x0F000124: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F000128: addiu       $a2, $a2, 0x580
    ctx->r6 = ADD32(ctx->r6, 0X580);
    // 0x0F00012C: lw          $a1, 0x24($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X24);
    // 0x0F000130: jalr        $t9
    // 0x0F000134: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000134: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_1:
    // 0x0F000138: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x0F00013C: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x0F000140: lhu         $t1, 0x2($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000144: lw          $t4, 0x14($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X14);
    // 0x0F000148: addiu       $at, $zero, -0x7E00
    ctx->r1 = ADD32(0, -0X7E00);
    // 0x0F00014C: or          $t0, $t8, $at
    ctx->r8 = ctx->r24 | ctx->r1;
    // 0x0F000150: ori         $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 | 0X1000;
    // 0x0F000154: sh          $t0, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r8;
    // 0x0F000158: sh          $t2, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r10;
    // 0x0F00015C: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F000160: lui         $t3, 0x600
    ctx->r11 = S32(0X600 << 16);
    // 0x0F000164: addiu       $t3, $t3, 0x6AA8
    ctx->r11 = ADD32(ctx->r11, 0X6AA8);
    // 0x0F000168: lw          $t6, 0x10($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X10);
    // 0x0F00016C: lui         $t7, 0x600
    ctx->r15 = S32(0X600 << 16);
    // 0x0F000170: addiu       $t7, $t7, 0x6CC8
    ctx->r15 = ADD32(ctx->r15, 0X6CC8);
    // 0x0F000174: sw          $t3, 0x34($t6)
    MEM_W(0X34, ctx->r14) = ctx->r11;
    // 0x0F000178: lw          $t9, 0x14($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X14);
    // 0x0F00017C: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000180: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x0F000184: lw          $t8, 0x14($t9)
    ctx->r24 = MEM_W(ctx->r25, 0X14);
    // 0x0F000188: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x0F00018C: addiu       $t9, $t9, -0x6C40
    ctx->r25 = ADD32(ctx->r25, -0X6C40);
    // 0x0F000190: lw          $t0, 0x10($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X10);
    // 0x0F000194: addiu       $a0, $a0, 0xD58
    ctx->r4 = ADD32(ctx->r4, 0XD58);
    // 0x0F000198: lw          $t1, 0x10($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X10);
    // 0x0F00019C: jalr        $t9
    // 0x0F0001A0: sw          $t7, 0x34($t1)
    MEM_W(0X34, ctx->r9) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0001A0: sw          $t7, 0x34($t1)
    MEM_W(0X34, ctx->r9) = ctx->r15;
    after_2:
    // 0x0F0001A4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0001A8: addiu       $t9, $t9, 0x2410
    ctx->r25 = ADD32(ctx->r25, 0X2410);
    // 0x0F0001AC: sw          $v0, 0x3C($s1)
    MEM_W(0X3C, ctx->r17) = ctx->r2;
    // 0x0F0001B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001B4: jalr        $t9
    // 0x0F0001B8: addiu       $a1, $zero, 0xFF
    ctx->r5 = ADD32(0, 0XFF);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F0001B8: addiu       $a1, $zero, 0xFF
    ctx->r5 = ADD32(0, 0XFF);
    after_3:
    // 0x0F0001BC: beq         $v0, $zero, L_0F0001FC
    if (ctx->r2 == 0) {
        // 0x0F0001C0: sw          $v0, 0x38($s0)
        MEM_W(0X38, ctx->r16) = ctx->r2;
            goto L_0F0001FC;
    }
    // 0x0F0001C0: sw          $v0, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->r2;
    // 0x0F0001C4: lw          $t2, 0x58($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X58);
    // 0x0F0001C8: lui         $at, 0x200
    ctx->r1 = S32(0X200 << 16);
    // 0x0F0001CC: ori         $at, $at, 0x808
    ctx->r1 = ctx->r1 | 0X808;
    // 0x0F0001D0: or          $t4, $t2, $at
    ctx->r12 = ctx->r10 | ctx->r1;
    // 0x0F0001D4: sw          $t4, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->r12;
    // 0x0F0001D8: lw          $t3, 0x38($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X38);
    // 0x0F0001DC: lui         $t5, 0x2
    ctx->r13 = S32(0X2 << 16);
    // 0x0F0001E0: ori         $t5, $t5, 0x102
    ctx->r13 = ctx->r13 | 0X102;
    // 0x0F0001E4: sw          $t5, 0x4C($t3)
    MEM_W(0X4C, ctx->r11) = ctx->r13;
    // 0x0F0001E8: lw          $t8, 0x38($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X38);
    // 0x0F0001EC: lui         $t6, 0x7850
    ctx->r14 = S32(0X7850 << 16);
    // 0x0F0001F0: ori         $t6, $t6, 0x6438
    ctx->r14 = ctx->r14 | 0X6438;
    // 0x0F0001F4: b           L_0F000214
    // 0x0F0001F8: sw          $t6, 0x50($t8)
    MEM_W(0X50, ctx->r24) = ctx->r14;
        goto L_0F000214;
    // 0x0F0001F8: sw          $t6, 0x50($t8)
    MEM_W(0X50, ctx->r24) = ctx->r14;
L_0F0001FC:
    // 0x0F0001FC: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x0F000200: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000204: jalr        $t9
    // 0x0F000208: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000208: nop

    after_4:
    // 0x0F00020C: b           L_0F000380
    // 0x0F000210: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F000380;
    // 0x0F000210: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F000214:
    // 0x0F000214: lw          $v0, 0x38($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X38);
    // 0x0F000218: lui         $a1, 0x3FC0
    ctx->r5 = S32(0X3FC0 << 16);
    // 0x0F00021C: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x0F000220: beq         $v0, $zero, L_0F000248
    if (ctx->r2 == 0) {
        // 0x0F000224: or          $a3, $s0, $zero
        ctx->r7 = ctx->r16 | 0;
            goto L_0F000248;
    }
    // 0x0F000224: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    // 0x0F000228: lw          $t0, 0x14($s1)
    ctx->r8 = MEM_W(ctx->r17, 0X14);
    // 0x0F00022C: lw          $t7, 0x14($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X14);
    // 0x0F000230: sw          $t7, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r15;
    // 0x0F000234: lw          $t1, 0x14($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X14);
    // 0x0F000238: lw          $t5, 0x38($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X38);
    // 0x0F00023C: lw          $t2, 0x14($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X14);
    // 0x0F000240: lw          $t4, 0x14($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X14);
    // 0x0F000244: sw          $t4, 0x40($t5)
    MEM_W(0X40, ctx->r13) = ctx->r12;
L_0F000248:
    // 0x0F000248: lw          $t3, 0x14($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X14);
    // 0x0F00024C: lui         $t9, 0x8007
    ctx->r25 = S32(0X8007 << 16);
    // 0x0F000250: addiu       $t9, $t9, -0x7BE0
    ctx->r25 = ADD32(ctx->r25, -0X7BE0);
    // 0x0F000254: jalr        $t9
    // 0x0F000258: lw          $a0, 0x14($t3)
    ctx->r4 = MEM_W(ctx->r11, 0X14);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000258: lw          $a0, 0x14($t3)
    ctx->r4 = MEM_W(ctx->r11, 0X14);
    after_5:
    // 0x0F00025C: lw          $v1, 0x44($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X44);
    // 0x0F000260: lui         $t6, 0xF00
    ctx->r14 = S32(0XF00 << 16);
    // 0x0F000264: addiu       $t6, $t6, 0xC80
    ctx->r14 = ADD32(ctx->r14, 0XC80);
    // 0x0F000268: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F00026C: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x0F000270: sw          $v0, 0xAC($v1)
    MEM_W(0XAC, ctx->r3) = ctx->r2;
    // 0x0F000274: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x0F000278: addiu       $a0, $v1, 0x4
    ctx->r4 = ADD32(ctx->r3, 0X4);
    // 0x0F00027C: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F000280: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x0F000284: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    // 0x0F000288: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F00028C: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F000290: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F000294: jalr        $t9
    // 0x0F000298: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000298: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_6:
    // 0x0F00029C: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0002A0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0002A4: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0002A8: lbu         $t0, 0x90($v1)
    ctx->r8 = MEM_BU(ctx->r3, 0X90);
    // 0x0F0002AC: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0002B0: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F0002B4: ori         $t7, $t0, 0x80
    ctx->r15 = ctx->r8 | 0X80;
    // 0x0F0002B8: sb          $t7, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r15;
    // 0x0F0002BC: addiu       $t1, $zero, 0x4
    ctx->r9 = ADD32(0, 0X4);
    // 0x0F0002C0: lw          $a1, 0x0($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X0);
    // 0x0F0002C4: sw          $t1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r9;
    // 0x0F0002C8: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F0002CC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F0002D0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F0002D4: jalr        $t9
    // 0x0F0002D8: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F0002D8: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_7:
    // 0x0F0002DC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0002E0: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F0002E4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F0002E8: jalr        $t9
    // 0x0F0002EC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F0002EC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_8:
    // 0x0F0002F0: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0002F4: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x0F0002F8: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0002FC: lbu         $t4, 0x90($v1)
    ctx->r12 = MEM_BU(ctx->r3, 0X90);
    // 0x0F000300: sw          $v0, 0x84($v1)
    MEM_W(0X84, ctx->r3) = ctx->r2;
    // 0x0F000304: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000308: ori         $t5, $t4, 0x8
    ctx->r13 = ctx->r12 | 0X8;
    // 0x0F00030C: sb          $t5, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r13;
    // 0x0F000310: lw          $t3, 0x44($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X44);
    // 0x0F000314: addiu       $t9, $t9, 0x5668
    ctx->r25 = ADD32(ctx->r25, 0X5668);
    // 0x0F000318: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00031C: lw          $t6, 0x0($t3)
    ctx->r14 = MEM_W(ctx->r11, 0X0);
    // 0x0F000320: sw          $t6, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r14;
    // 0x0F000324: lwc1        $f6, 0xDE8($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0XDE8);
    // 0x0F000328: lui         $at, 0x41A0
    ctx->r1 = S32(0X41A0 << 16);
    // 0x0F00032C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000330: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x0F000334: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000338: swc1        $f6, 0xC4($v1)
    MEM_W(0XC4, ctx->r3) = ctx->f6.u32l;
    // 0x0F00033C: swc1        $f8, 0xCC($v1)
    MEM_W(0XCC, ctx->r3) = ctx->f8.u32l;
    // 0x0F000340: beq         $a0, $zero, L_0F000360
    if (ctx->r4 == 0) {
        // 0x0F000344: swc1        $f10, 0xC8($v1)
        MEM_W(0XC8, ctx->r3) = ctx->f10.u32l;
            goto L_0F000360;
    }
    // 0x0F000344: swc1        $f10, 0xC8($v1)
    MEM_W(0XC8, ctx->r3) = ctx->f10.u32l;
    // 0x0F000348: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x0F00034C: swc1        $f16, 0x50($s1)
    MEM_W(0X50, ctx->r17) = ctx->f16.u32l;
    // 0x0F000350: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x0F000354: swc1        $f18, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->f18.u32l;
    // 0x0F000358: lwc1        $f4, 0xC($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0XC);
    // 0x0F00035C: swc1        $f4, 0x58($s1)
    MEM_W(0X58, ctx->r17) = ctx->f4.u32l;
L_0F000360:
    // 0x0F000360: jalr        $t9
    // 0x0F000364: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F000364: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_9:
    // 0x0F000368: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00036C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000370: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000374: jalr        $t9
    // 0x0F000378: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x0F000378: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_10:
    // 0x0F00037C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F000380:
    // 0x0F000380: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000384: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x0F000388: jr          $ra
    // 0x0F00038C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x0F00038C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void ni_ovl_050_func_0F000390(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000390: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000394: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000398: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F00039C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F0003A0: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x0F0003A4: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F0003A8: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0003AC: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x0F0003B0: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x0F0003B4: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x0F0003B8: lw          $t8, -0x5170($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X5170);
    // 0x0F0003BC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F0003C0: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x0F0003C4: beq         $t9, $zero, L_0F00040C
    if (ctx->r25 == 0) {
        // 0x0F0003C8: lui         $t9, 0x8004
        ctx->r25 = S32(0X8004 << 16);
            goto L_0F00040C;
    }
    // 0x0F0003C8: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0003CC: addiu       $t9, $t9, 0x7954
    ctx->r25 = ADD32(ctx->r25, 0X7954);
    // 0x0F0003D0: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x0F0003D4: jalr        $t9
    // 0x0F0003D8: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0003D8: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    after_0:
    // 0x0F0003DC: beq         $v0, $zero, L_0F00041C
    if (ctx->r2 == 0) {
        // 0x0F0003E0: lw          $a0, 0x24($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X24);
            goto L_0F00041C;
    }
    // 0x0F0003E0: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x0F0003E4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0003E8: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F0003EC: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x0F0003F0: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F0003F4: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F0003F8: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F0003FC: jalr        $t9
    // 0x0F000400: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000400: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x0F000404: b           L_0F000420
    // 0x0F000408: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F000420;
    // 0x0F000408: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F00040C:
    // 0x0F00040C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000410: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000414: jalr        $t9
    // 0x0F000418: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000418: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_2:
L_0F00041C:
    // 0x0F00041C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000420:
    // 0x0F000420: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F000424: jr          $ra
    // 0x0F000428: nop

    return;
    // 0x0F000428: nop

;}
RECOMP_FUNC void ni_ovl_050_func_0F00042C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F00042C: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x0F000430: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F000434: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F000438: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F00043C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000440: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x0F000444: lw          $s2, 0x34($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X34);
    // 0x0F000448: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F00044C: jal         0x0F000564
    // 0x0F000450: sw          $t6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r14;
    static_104_0F000564(rdram, ctx);
        goto after_0;
    // 0x0F000450: sw          $t6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r14;
    after_0:
    // 0x0F000454: addiu       $s0, $s2, 0x8
    ctx->r16 = ADD32(ctx->r18, 0X8);
    // 0x0F000458: lbu         $t8, 0x90($s0)
    ctx->r24 = MEM_BU(ctx->r16, 0X90);
    // 0x0F00045C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F000460: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000464: ori         $t9, $t8, 0x80
    ctx->r25 = ctx->r24 | 0X80;
    // 0x0F000468: sb          $t9, 0x90($s0)
    MEM_B(0X90, ctx->r16) = ctx->r25;
    // 0x0F00046C: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000470: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x0F000474: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F000478: addiu       $a0, $s0, 0x4
    ctx->r4 = ADD32(ctx->r16, 0X4);
    // 0x0F00047C: addiu       $t0, $zero, 0x4
    ctx->r8 = ADD32(0, 0X4);
    // 0x0F000480: sw          $t0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r8;
    // 0x0F000484: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x0F000488: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F00048C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F000490: jalr        $t9
    // 0x0F000494: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000494: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_1:
    // 0x0F000498: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F00049C: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F0004A0: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x0F0004A4: jalr        $t9
    // 0x0F0004A8: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0004A8: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    after_2:
    // 0x0F0004AC: lbu         $t2, 0x90($s0)
    ctx->r10 = MEM_BU(ctx->r16, 0X90);
    // 0x0F0004B0: sw          $v0, 0x84($s0)
    MEM_W(0X84, ctx->r16) = ctx->r2;
    // 0x0F0004B4: addiu       $v1, $s2, 0xDC
    ctx->r3 = ADD32(ctx->r18, 0XDC);
    // 0x0F0004B8: ori         $t3, $t2, 0x8
    ctx->r11 = ctx->r10 | 0X8;
    // 0x0F0004BC: sb          $t3, 0x90($s0)
    MEM_B(0X90, ctx->r16) = ctx->r11;
    // 0x0F0004C0: lw          $t4, 0x0($s2)
    ctx->r12 = MEM_W(ctx->r18, 0X0);
    // 0x0F0004C4: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F0004C8: addiu       $t9, $t9, -0x6164
    ctx->r25 = ADD32(ctx->r25, -0X6164);
    // 0x0F0004CC: sw          $t4, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r12;
    // 0x0F0004D0: lw          $t5, 0x40($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X40);
    // 0x0F0004D4: addiu       $a1, $v1, 0x8
    ctx->r5 = ADD32(ctx->r3, 0X8);
    // 0x0F0004D8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F0004DC: lw          $t6, 0x14($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X14);
    // 0x0F0004E0: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x0F0004E4: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x0F0004E8: lw          $a0, 0x14($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X14);
    // 0x0F0004EC: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x0F0004F0: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x0F0004F4: jalr        $t9
    // 0x0F0004F8: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F0004F8: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    after_3:
    // 0x0F0004FC: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x0F000500: addiu       $at, $zero, -0x80D
    ctx->r1 = ADD32(0, -0X80D);
    // 0x0F000504: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000508: swc1        $f0, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->f0.u32l;
    // 0x0F00050C: lw          $v0, 0x38($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X38);
    // 0x0F000510: addiu       $t9, $t9, -0x7FC4
    ctx->r25 = ADD32(ctx->r25, -0X7FC4);
    // 0x0F000514: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x0F000518: lw          $t8, 0x58($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X58);
    // 0x0F00051C: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x0F000520: addiu       $a0, $v1, 0x1C
    ctx->r4 = ADD32(ctx->r3, 0X1C);
    // 0x0F000524: and         $t0, $t8, $at
    ctx->r8 = ctx->r24 & ctx->r1;
    // 0x0F000528: sw          $t0, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->r8;
    // 0x0F00052C: lw          $t2, 0x38($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X38);
    // 0x0F000530: lw          $t1, 0x34($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X34);
    // 0x0F000534: jalr        $t9
    // 0x0F000538: sw          $t1, 0x34($t2)
    MEM_W(0X34, ctx->r10) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000538: sw          $t1, 0x34($t2)
    MEM_W(0X34, ctx->r10) = ctx->r9;
    after_4:
    // 0x0F00053C: lw          $t3, 0x40($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X40);
    // 0x0F000540: sh          $v0, 0x5E($t3)
    MEM_H(0X5E, ctx->r11) = ctx->r2;
    // 0x0F000544: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000548: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F00054C: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F000550: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F000554: jr          $ra
    // 0x0F000558: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x0F000558: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void ni_ovl_050_func_0F00055C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F00055C: jr          $ra
    // 0x0F000560: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x0F000560: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x0F000564: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x0F000568: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x0F00056C: swc1        $f0, 0xF8($v0)
    MEM_W(0XF8, ctx->r2) = ctx->f0.u32l;
    // 0x0F000570: jr          $ra
    // 0x0F000574: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
    return;
    // 0x0F000574: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
;}
RECOMP_FUNC void ni_ovl_051_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0x3C98($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3C98);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000074: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000078: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F00007C: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x0F000080: lui         $t0, 0x8000
    ctx->r8 = S32(0X8000 << 16);
    // 0x0F000084: addiu       $t0, $t0, 0x2808
    ctx->r8 = ADD32(ctx->r8, 0X2808);
    // 0x0F000088: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F00008C: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    // 0x0F000090: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000094: addiu       $a2, $zero, 0x10C
    ctx->r6 = ADD32(0, 0X10C);
    // 0x0F000098: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F00009C: jalr        $t0
    // 0x0F0000A0: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r8)(rdram, ctx);
        goto after_0;
    // 0x0F0000A0: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    after_0:
    // 0x0F0000A4: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F0000A8: bne         $v0, $zero, L_0F0000C8
    if (ctx->r2 != 0) {
        // 0x0F0000AC: lw          $t0, 0x24($sp)
        ctx->r8 = MEM_W(ctx->r29, 0X24);
            goto L_0F0000C8;
    }
    // 0x0F0000AC: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
    // 0x0F0000B0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x0F0000B4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0000B8: jalr        $t9
    // 0x0F0000BC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000BC: nop

    after_1:
    // 0x0F0000C0: b           L_0F000184
    // 0x0F0000C4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F000184;
    // 0x0F0000C4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000C8:
    // 0x0F0000C8: beql        $v1, $zero, L_0F000164
    if (ctx->r3 == 0) {
        // 0x0F0000CC: lhu         $t1, 0x2($s0)
        ctx->r9 = MEM_HU(ctx->r16, 0X2);
            goto L_0F000164;
    }
    goto skip_0;
    // 0x0F0000CC: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
    skip_0:
    // 0x0F0000D0: lhu         $t6, 0x18($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X18);
    // 0x0F0000D4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0000D8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F0000DC: slti        $at, $t6, 0x2
    ctx->r1 = SIGNED(ctx->r14) < 0X2 ? 1 : 0;
    // 0x0F0000E0: bne         $at, $zero, L_0F000160
    if (ctx->r1 != 0) {
        // 0x0F0000E4: addiu       $a2, $zero, 0x20
        ctx->r6 = ADD32(0, 0X20);
            goto L_0F000160;
    }
    // 0x0F0000E4: addiu       $a2, $zero, 0x20
    ctx->r6 = ADD32(0, 0X20);
    // 0x0F0000E8: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    // 0x0F0000EC: jalr        $t0
    // 0x0F0000F0: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r8)(rdram, ctx);
        goto after_2;
    // 0x0F0000F0: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    after_2:
    // 0x0F0000F4: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F0000F8: bne         $v0, $zero, L_0F000118
    if (ctx->r2 != 0) {
        // 0x0F0000FC: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_0F000118;
    }
    // 0x0F0000FC: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x0F000100: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x0F000104: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000108: jalr        $t9
    // 0x0F00010C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F00010C: nop

    after_3:
    // 0x0F000110: b           L_0F000184
    // 0x0F000114: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F000184;
    // 0x0F000114: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000118:
    // 0x0F000118: lwc1        $f4, 0x4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X4);
    // 0x0F00011C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F000120: addiu       $v0, $zero, 0xA
    ctx->r2 = ADD32(0, 0XA);
    // 0x0F000124: swc1        $f4, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f4.u32l;
    // 0x0F000128: lwc1        $f6, 0x8($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X8);
    // 0x0F00012C: addiu       $t7, $zero, 0x27
    ctx->r15 = ADD32(0, 0X27);
    // 0x0F000130: addiu       $t8, $zero, 0x7A
    ctx->r24 = ADD32(0, 0X7A);
    // 0x0F000134: swc1        $f6, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f6.u32l;
    // 0x0F000138: lwc1        $f8, 0xC($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0XC);
    // 0x0F00013C: addiu       $t9, $t9, -0x707C
    ctx->r25 = ADD32(ctx->r25, -0X707C);
    // 0x0F000140: sh          $t7, 0x10($a1)
    MEM_H(0X10, ctx->r5) = ctx->r15;
    // 0x0F000144: sh          $t8, 0x18($a1)
    MEM_H(0X18, ctx->r5) = ctx->r24;
    // 0x0F000148: sh          $v0, 0x14($a1)
    MEM_H(0X14, ctx->r5) = ctx->r2;
    // 0x0F00014C: sh          $v0, 0x16($a1)
    MEM_H(0X16, ctx->r5) = ctx->r2;
    // 0x0F000150: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000154: jalr        $t9
    // 0x0F000158: swc1        $f8, 0xC($a1)
    MEM_W(0XC, ctx->r5) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000158: swc1        $f8, 0xC($a1)
    MEM_W(0XC, ctx->r5) = ctx->f8.u32l;
    after_4:
    // 0x0F00015C: sw          $v0, 0x4C($s0)
    MEM_W(0X4C, ctx->r16) = ctx->r2;
L_0F000160:
    // 0x0F000160: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
L_0F000164:
    // 0x0F000164: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000168: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F00016C: ori         $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 | 0X1000;
    // 0x0F000170: sh          $t2, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r10;
    // 0x0F000174: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000178: jalr        $t9
    // 0x0F00017C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F00017C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_5:
    // 0x0F000180: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000184:
    // 0x0F000184: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000188: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F00018C: jr          $ra
    // 0x0F000190: nop

    return;
    // 0x0F000190: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000194(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000194: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x0F000198: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F00019C: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x0F0001A0: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x0F0001A4: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x0F0001A8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0001AC: addiu       $t9, $t9, 0x34AC
    ctx->r25 = ADD32(ctx->r25, 0X34AC);
    // 0x0F0001B0: sw          $t6, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r14;
    // 0x0F0001B4: lw          $t7, 0x70($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X70);
    // 0x0F0001B8: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F0001BC: jalr        $t9
    // 0x0F0001C0: sw          $t7, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0001C0: sw          $t7, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r15;
    after_0:
    // 0x0F0001C4: sw          $v0, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->r2;
    // 0x0F0001C8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0001CC: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x0F0001D0: lw          $a1, 0x24($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X24);
    // 0x0F0001D4: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F0001D8: addiu       $a2, $a2, 0x1700
    ctx->r6 = ADD32(ctx->r6, 0X1700);
    // 0x0F0001DC: sw          $v0, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r2;
    // 0x0F0001E0: jalr        $t9
    // 0x0F0001E4: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0001E4: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_1:
    // 0x0F0001E8: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x0F0001EC: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x0F0001F0: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0001F4: lw          $v1, 0x5C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X5C);
    // 0x0F0001F8: lw          $t4, 0x14($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X14);
    // 0x0F0001FC: ori         $t1, $t8, 0x200
    ctx->r9 = ctx->r24 | 0X200;
    // 0x0F000200: ori         $t3, $t2, 0x1000
    ctx->r11 = ctx->r10 | 0X1000;
    // 0x0F000204: sh          $t1, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r9;
    // 0x0F000208: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x0F00020C: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F000210: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000214: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000218: lw          $t6, 0x10($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X10);
    // 0x0F00021C: ldc1        $f0, 0x3D40($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X3D40);
    // 0x0F000220: lui         $a2, 0x800A
    ctx->r6 = S32(0X800A << 16);
    // 0x0F000224: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x0F000228: addiu       $a2, $a2, -0x6C40
    ctx->r6 = ADD32(ctx->r6, -0X6C40);
    // 0x0F00022C: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000230: lw          $v0, 0x10($t7)
    ctx->r2 = MEM_W(ctx->r15, 0X10);
    // 0x0F000234: addiu       $a1, $v1, 0xDC
    ctx->r5 = ADD32(ctx->r3, 0XDC);
    // 0x0F000238: addiu       $a0, $a0, 0x3C68
    ctx->r4 = ADD32(ctx->r4, 0X3C68);
    // 0x0F00023C: lh          $t9, 0x0($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X0);
    // 0x0F000240: ori         $t8, $t9, 0x8
    ctx->r24 = ctx->r25 | 0X8;
    // 0x0F000244: sh          $t8, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r24;
    // 0x0F000248: lw          $t1, 0x14($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X14);
    // 0x0F00024C: lw          $t2, 0x14($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X14);
    // 0x0F000250: lw          $t3, 0x10($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X10);
    // 0x0F000254: lw          $t4, 0x10($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X10);
    // 0x0F000258: lw          $v0, 0x10($t4)
    ctx->r2 = MEM_W(ctx->r12, 0X10);
    // 0x0F00025C: lwc1        $f4, 0x68($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X68);
    // 0x0F000260: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x0F000264: mul.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x0F000268: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x0F00026C: swc1        $f10, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f10.u32l;
    // 0x0F000270: lw          $t5, 0x14($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X14);
    // 0x0F000274: lw          $t6, 0x14($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X14);
    // 0x0F000278: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x0F00027C: lw          $t9, 0x10($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X10);
    // 0x0F000280: lw          $v0, 0x10($t9)
    ctx->r2 = MEM_W(ctx->r25, 0X10);
    // 0x0F000284: lwc1        $f16, 0x6C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X6C);
    // 0x0F000288: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x0F00028C: mul.d       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f0.d);
    // 0x0F000290: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F000294: swc1        $f6, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f6.u32l;
    // 0x0F000298: lw          $t8, 0x14($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X14);
    // 0x0F00029C: lw          $t1, 0x14($t8)
    ctx->r9 = MEM_W(ctx->r24, 0X14);
    // 0x0F0002A0: lw          $t2, 0x10($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X10);
    // 0x0F0002A4: lw          $t3, 0x10($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X10);
    // 0x0F0002A8: lw          $v0, 0x10($t3)
    ctx->r2 = MEM_W(ctx->r11, 0X10);
    // 0x0F0002AC: lwc1        $f8, 0x70($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X70);
    // 0x0F0002B0: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x0F0002B4: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x0F0002B8: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x0F0002BC: swc1        $f18, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f18.u32l;
    // 0x0F0002C0: sw          $a2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r6;
    // 0x0F0002C4: jalr        $a2
    // 0x0F0002C8: sw          $a1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r5;
    LOOKUP_FUNC(ctx->r6)(rdram, ctx);
        goto after_2;
    // 0x0F0002C8: sw          $a1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r5;
    after_2:
    // 0x0F0002CC: lui         $v1, 0x8001
    ctx->r3 = S32(0X8001 << 16);
    // 0x0F0002D0: sw          $v0, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->r2;
    // 0x0F0002D4: addiu       $v1, $v1, 0x36C4
    ctx->r3 = ADD32(ctx->r3, 0X36C4);
    // 0x0F0002D8: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x0F0002DC: lw          $a0, 0x64($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X64);
    // 0x0F0002E0: lui         $a1, 0xC
    ctx->r5 = S32(0XC << 16);
    // 0x0F0002E4: jalr        $v1
    // 0x0F0002E8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_3;
    // 0x0F0002E8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_3:
    // 0x0F0002EC: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F0002F0: lui         $a1, 0x20
    ctx->r5 = S32(0X20 << 16);
    // 0x0F0002F4: ori         $a1, $a1, 0x1
    ctx->r5 = ctx->r5 | 0X1;
    // 0x0F0002F8: lw          $a0, 0x64($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X64);
    // 0x0F0002FC: jalr        $v1
    // 0x0F000300: addiu       $a2, $s0, 0x50
    ctx->r6 = ADD32(ctx->r16, 0X50);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_4;
    // 0x0F000300: addiu       $a2, $s0, 0x50
    ctx->r6 = ADD32(ctx->r16, 0X50);
    after_4:
    // 0x0F000304: lui         $v1, 0x8000
    ctx->r3 = S32(0X8000 << 16);
    // 0x0F000308: addiu       $v1, $v1, 0x2410
    ctx->r3 = ADD32(ctx->r3, 0X2410);
    // 0x0F00030C: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x0F000310: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000314: jalr        $v1
    // 0x0F000318: addiu       $a1, $zero, 0x22A3
    ctx->r5 = ADD32(0, 0X22A3);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_5;
    // 0x0F000318: addiu       $a1, $zero, 0x22A3
    ctx->r5 = ADD32(0, 0X22A3);
    after_5:
    // 0x0F00031C: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F000320: lw          $t4, 0x24($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X24);
    // 0x0F000324: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000328: addiu       $a1, $zero, 0xFF
    ctx->r5 = ADD32(0, 0XFF);
    // 0x0F00032C: jalr        $v1
    // 0x0F000330: sw          $t4, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r12;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_6;
    // 0x0F000330: sw          $t4, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r12;
    after_6:
    // 0x0F000334: beq         $v0, $zero, L_0F000374
    if (ctx->r2 == 0) {
        // 0x0F000338: sw          $v0, 0x3C($s1)
        MEM_W(0X3C, ctx->r17) = ctx->r2;
            goto L_0F000374;
    }
    // 0x0F000338: sw          $v0, 0x3C($s1)
    MEM_W(0X3C, ctx->r17) = ctx->r2;
    // 0x0F00033C: lw          $t5, 0x58($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X58);
    // 0x0F000340: lui         $at, 0x200
    ctx->r1 = S32(0X200 << 16);
    // 0x0F000344: ori         $at, $at, 0x808
    ctx->r1 = ctx->r1 | 0X808;
    // 0x0F000348: or          $t6, $t5, $at
    ctx->r14 = ctx->r13 | ctx->r1;
    // 0x0F00034C: sw          $t6, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->r14;
    // 0x0F000350: lw          $t9, 0x3C($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X3C);
    // 0x0F000354: lui         $t7, 0x2
    ctx->r15 = S32(0X2 << 16);
    // 0x0F000358: ori         $t7, $t7, 0x102
    ctx->r15 = ctx->r15 | 0X102;
    // 0x0F00035C: sw          $t7, 0x4C($t9)
    MEM_W(0X4C, ctx->r25) = ctx->r15;
    // 0x0F000360: lw          $t1, 0x3C($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X3C);
    // 0x0F000364: lui         $t8, 0x7850
    ctx->r24 = S32(0X7850 << 16);
    // 0x0F000368: ori         $t8, $t8, 0x6438
    ctx->r24 = ctx->r24 | 0X6438;
    // 0x0F00036C: b           L_0F00038C
    // 0x0F000370: sw          $t8, 0x50($t1)
    MEM_W(0X50, ctx->r9) = ctx->r24;
        goto L_0F00038C;
    // 0x0F000370: sw          $t8, 0x50($t1)
    MEM_W(0X50, ctx->r9) = ctx->r24;
L_0F000374:
    // 0x0F000374: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x0F000378: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F00037C: jalr        $t9
    // 0x0F000380: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000380: nop

    after_7:
    // 0x0F000384: b           L_0F000858
    // 0x0F000388: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F000858;
    // 0x0F000388: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F00038C:
    // 0x0F00038C: lw          $v0, 0x3C($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X3C);
    // 0x0F000390: lui         $a1, 0x3FC0
    ctx->r5 = S32(0X3FC0 << 16);
    // 0x0F000394: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x0F000398: beq         $v0, $zero, L_0F0003C0
    if (ctx->r2 == 0) {
        // 0x0F00039C: or          $a3, $s1, $zero
        ctx->r7 = ctx->r17 | 0;
            goto L_0F0003C0;
    }
    // 0x0F00039C: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x0F0003A0: lw          $t2, 0x14($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X14);
    // 0x0F0003A4: lw          $t3, 0x14($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X14);
    // 0x0F0003A8: sw          $t3, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r11;
    // 0x0F0003AC: lw          $t4, 0x14($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X14);
    // 0x0F0003B0: lw          $t7, 0x3C($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X3C);
    // 0x0F0003B4: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F0003B8: lw          $t6, 0x14($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X14);
    // 0x0F0003BC: sw          $t6, 0x40($t7)
    MEM_W(0X40, ctx->r15) = ctx->r14;
L_0F0003C0:
    // 0x0F0003C0: lw          $t8, 0x14($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X14);
    // 0x0F0003C4: lui         $t9, 0x8007
    ctx->r25 = S32(0X8007 << 16);
    // 0x0F0003C8: addiu       $t9, $t9, -0x7BE0
    ctx->r25 = ADD32(ctx->r25, -0X7BE0);
    // 0x0F0003CC: jalr        $t9
    // 0x0F0003D0: lw          $a0, 0x14($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X14);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F0003D0: lw          $a0, 0x14($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X14);
    after_8:
    // 0x0F0003D4: lw          $v1, 0x5C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X5C);
    // 0x0F0003D8: lui         $t2, 0xF00
    ctx->r10 = S32(0XF00 << 16);
    // 0x0F0003DC: addiu       $t2, $t2, 0xE54
    ctx->r10 = ADD32(ctx->r10, 0XE54);
    // 0x0F0003E0: sw          $v0, 0xB4($v1)
    MEM_W(0XB4, ctx->r3) = ctx->r2;
    // 0x0F0003E4: lw          $t1, 0x10($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X10);
    // 0x0F0003E8: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0003EC: addiu       $t9, $t9, 0x524
    ctx->r25 = ADD32(ctx->r25, 0X524);
    // 0x0F0003F0: sw          $t1, 0xA8($v1)
    MEM_W(0XA8, ctx->r3) = ctx->r9;
    // 0x0F0003F4: sw          $t2, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r10;
    // 0x0F0003F8: lw          $a0, 0x24($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X24);
    // 0x0F0003FC: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x0F000400: jalr        $t9
    // 0x0F000404: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F000404: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    after_9:
    // 0x0F000408: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F00040C: bne         $v0, $zero, L_0F00042C
    if (ctx->r2 != 0) {
        // 0x0F000410: or          $a3, $v0, $zero
        ctx->r7 = ctx->r2 | 0;
            goto L_0F00042C;
    }
    // 0x0F000410: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x0F000414: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x0F000418: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F00041C: jalr        $t9
    // 0x0F000420: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x0F000420: nop

    after_10:
    // 0x0F000424: b           L_0F000858
    // 0x0F000428: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F000858;
    // 0x0F000428: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F00042C:
    // 0x0F00042C: lw          $t3, 0x5C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X5C);
    // 0x0F000430: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F000434: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000438: sw          $a3, 0x4($t3)
    MEM_W(0X4, ctx->r11) = ctx->r7;
    // 0x0F00043C: lhu         $t4, 0xA($a3)
    ctx->r12 = MEM_HU(ctx->r7, 0XA);
    // 0x0F000440: sh          $t8, 0x4($a3)
    MEM_H(0X4, ctx->r7) = ctx->r24;
    // 0x0F000444: andi        $t6, $t4, 0xF9FF
    ctx->r14 = ctx->r12 & 0XF9FF;
    // 0x0F000448: ori         $t1, $t6, 0x200
    ctx->r9 = ctx->r14 | 0X200;
    // 0x0F00044C: sh          $t6, 0xA($a3)
    MEM_H(0XA, ctx->r7) = ctx->r14;
    // 0x0F000450: andi        $t9, $t1, 0xFFFC
    ctx->r25 = ctx->r9 & 0XFFFC;
    // 0x0F000454: sh          $t1, 0xA($a3)
    MEM_H(0XA, ctx->r7) = ctx->r9;
    // 0x0F000458: sh          $t9, 0xA($a3)
    MEM_H(0XA, ctx->r7) = ctx->r25;
    // 0x0F00045C: ori         $t3, $t9, 0x2
    ctx->r11 = ctx->r25 | 0X2;
    // 0x0F000460: sh          $t3, 0xA($a3)
    MEM_H(0XA, ctx->r7) = ctx->r11;
    // 0x0F000464: lw          $t4, 0x14($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X14);
    // 0x0F000468: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F00046C: addiu       $t9, $t9, 0x628
    ctx->r25 = ADD32(ctx->r25, 0X628);
    // 0x0F000470: lw          $a0, 0x14($t4)
    ctx->r4 = MEM_W(ctx->r12, 0X14);
    // 0x0F000474: sw          $a3, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r7;
    // 0x0F000478: jalr        $t9
    // 0x0F00047C: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x0F00047C: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    after_11:
    // 0x0F000480: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F000484: lw          $a3, 0x44($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X44);
    // 0x0F000488: bne         $v0, $zero, L_0F0004A8
    if (ctx->r2 != 0) {
        // 0x0F00048C: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_0F0004A8;
    }
    // 0x0F00048C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x0F000490: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x0F000494: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000498: jalr        $t9
    // 0x0F00049C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x0F00049C: nop

    after_12:
    // 0x0F0004A0: b           L_0F000858
    // 0x0F0004A4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F000858;
    // 0x0F0004A4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F0004A8:
    // 0x0F0004A8: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0004AC: addiu       $t9, $t9, 0x844
    ctx->r25 = ADD32(ctx->r25, 0X844);
    // 0x0F0004B0: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x0F0004B4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x0F0004B8: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x0F0004BC: jalr        $t9
    // 0x0F0004C0: sw          $a1, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r5;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_13;
    // 0x0F0004C0: sw          $a1, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r5;
    after_13:
    // 0x0F0004C4: lui         $at, 0x4220
    ctx->r1 = S32(0X4220 << 16);
    // 0x0F0004C8: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F0004CC: lw          $t0, 0x40($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X40);
    // 0x0F0004D0: lw          $t5, 0x30($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X30);
    // 0x0F0004D4: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0004D8: lui         $at, 0x430C
    ctx->r1 = S32(0X430C << 16);
    // 0x0F0004DC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0004E0: lui         $at, 0x428C
    ctx->r1 = S32(0X428C << 16);
    // 0x0F0004E4: sw          $t0, 0x2C($t5)
    MEM_W(0X2C, ctx->r13) = ctx->r8;
    // 0x0F0004E8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0004EC: addiu       $t6, $zero, 0x4000
    ctx->r14 = ADD32(0, 0X4000);
    // 0x0F0004F0: lui         $t7, 0xF00
    ctx->r15 = S32(0XF00 << 16);
    // 0x0F0004F4: addiu       $t7, $t7, 0x3B8C
    ctx->r15 = ADD32(ctx->r15, 0X3B8C);
    // 0x0F0004F8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0004FC: sh          $t6, 0x16($t0)
    MEM_H(0X16, ctx->r8) = ctx->r14;
    // 0x0F000500: swc1        $f4, 0x24($t0)
    MEM_W(0X24, ctx->r8) = ctx->f4.u32l;
    // 0x0F000504: swc1        $f6, 0x28($t0)
    MEM_W(0X28, ctx->r8) = ctx->f6.u32l;
    // 0x0F000508: swc1        $f8, 0x18($t0)
    MEM_W(0X18, ctx->r8) = ctx->f8.u32l;
    // 0x0F00050C: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F000510: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x0F000514: addiu       $a0, $v1, 0x4
    ctx->r4 = ADD32(ctx->r3, 0X4);
    // 0x0F000518: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x0F00051C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F000520: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F000524: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F000528: jalr        $t9
    // 0x0F00052C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_14;
    // 0x0F00052C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_14:
    // 0x0F000530: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x0F000534: lh          $t8, -0x5470($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X5470);
    // 0x0F000538: addiu       $at, $zero, 0x28
    ctx->r1 = ADD32(0, 0X28);
    // 0x0F00053C: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F000540: bne         $t8, $at, L_0F0005C0
    if (ctx->r24 != ctx->r1) {
        // 0x0F000544: or          $a2, $zero, $zero
        ctx->r6 = 0 | 0;
            goto L_0F0005C0;
    }
    // 0x0F000544: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000548: lbu         $t2, 0x90($v1)
    ctx->r10 = MEM_BU(ctx->r3, 0X90);
    // 0x0F00054C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F000550: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000554: ori         $t3, $t2, 0x80
    ctx->r11 = ctx->r10 | 0X80;
    // 0x0F000558: sb          $t3, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r11;
    // 0x0F00055C: lw          $a1, 0x0($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X0);
    // 0x0F000560: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000564: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F000568: addiu       $t4, $zero, 0x4
    ctx->r12 = ADD32(0, 0X4);
    // 0x0F00056C: sw          $t4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r12;
    // 0x0F000570: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x0F000574: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F000578: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F00057C: addiu       $a3, $zero, 0xD7
    ctx->r7 = ADD32(0, 0XD7);
    // 0x0F000580: jalr        $t9
    // 0x0F000584: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_15;
    // 0x0F000584: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    after_15:
    // 0x0F000588: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F00058C: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F000590: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F000594: jalr        $t9
    // 0x0F000598: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_16;
    // 0x0F000598: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_16:
    // 0x0F00059C: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F0005A0: lbu         $t6, 0x90($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X90);
    // 0x0F0005A4: sw          $v0, 0x84($v1)
    MEM_W(0X84, ctx->r3) = ctx->r2;
    // 0x0F0005A8: ori         $t7, $t6, 0x8
    ctx->r15 = ctx->r14 | 0X8;
    // 0x0F0005AC: sb          $t7, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r15;
    // 0x0F0005B0: lw          $t8, 0x5C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X5C);
    // 0x0F0005B4: lw          $t1, 0x0($t8)
    ctx->r9 = MEM_W(ctx->r24, 0X0);
    // 0x0F0005B8: b           L_0F000630
    // 0x0F0005BC: sw          $t1, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->r9;
        goto L_0F000630;
    // 0x0F0005BC: sw          $t1, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->r9;
L_0F0005C0:
    // 0x0F0005C0: lbu         $t3, 0x90($v1)
    ctx->r11 = MEM_BU(ctx->r3, 0X90);
    // 0x0F0005C4: lw          $a1, 0x0($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X0);
    // 0x0F0005C8: addiu       $t9, $zero, 0x4
    ctx->r25 = ADD32(0, 0X4);
    // 0x0F0005CC: ori         $t4, $t3, 0x80
    ctx->r12 = ctx->r11 | 0X80;
    // 0x0F0005D0: sb          $t4, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r12;
    // 0x0F0005D4: sw          $t9, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r25;
    // 0x0F0005D8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0005DC: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F0005E0: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0005E4: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F0005E8: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x0F0005EC: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F0005F0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F0005F4: jalr        $t9
    // 0x0F0005F8: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_17;
    // 0x0F0005F8: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    after_17:
    // 0x0F0005FC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000600: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F000604: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x0F000608: jalr        $t9
    // 0x0F00060C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_18;
    // 0x0F00060C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_18:
    // 0x0F000610: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F000614: lbu         $t6, 0x90($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X90);
    // 0x0F000618: sw          $v0, 0x84($v1)
    MEM_W(0X84, ctx->r3) = ctx->r2;
    // 0x0F00061C: ori         $t7, $t6, 0x8
    ctx->r15 = ctx->r14 | 0X8;
    // 0x0F000620: sb          $t7, 0x90($v1)
    MEM_B(0X90, ctx->r3) = ctx->r15;
    // 0x0F000624: lw          $t1, 0x5C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X5C);
    // 0x0F000628: lw          $t8, 0x0($t1)
    ctx->r24 = MEM_W(ctx->r9, 0X0);
    // 0x0F00062C: sw          $t8, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r24;
L_0F000630:
    // 0x0F000630: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000634: lwc1        $f18, 0x3D48($at)
    ctx->f18.u32l = MEM_W(ctx->r1, 0X3D48);
    // 0x0F000638: lui         $at, 0x41A0
    ctx->r1 = S32(0X41A0 << 16);
    // 0x0F00063C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000640: lw          $a1, 0x4C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4C);
    // 0x0F000644: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x0F000648: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F00064C: lui         $t2, 0x601
    ctx->r10 = S32(0X601 << 16);
    // 0x0F000650: addiu       $t2, $t2, -0x6820
    ctx->r10 = ADD32(ctx->r10, -0X6820);
    // 0x0F000654: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F000658: or          $t3, $t2, $at
    ctx->r11 = ctx->r10 | ctx->r1;
    // 0x0F00065C: swc1        $f18, 0xC4($v1)
    MEM_W(0XC4, ctx->r3) = ctx->f18.u32l;
    // 0x0F000660: swc1        $f4, 0xCC($v1)
    MEM_W(0XCC, ctx->r3) = ctx->f4.u32l;
    // 0x0F000664: swc1        $f6, 0xC8($v1)
    MEM_W(0XC8, ctx->r3) = ctx->f6.u32l;
    // 0x0F000668: beq         $a1, $zero, L_0F000690
    if (ctx->r5 == 0) {
        // 0x0F00066C: sw          $t3, 0x3C($s0)
        MEM_W(0X3C, ctx->r16) = ctx->r11;
            goto L_0F000690;
    }
    // 0x0F00066C: sw          $t3, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r11;
    // 0x0F000670: lwc1        $f8, 0x4($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X4);
    // 0x0F000674: swc1        $f8, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->f8.u32l;
    // 0x0F000678: lwc1        $f10, 0x8($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X8);
    // 0x0F00067C: swc1        $f10, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->f10.u32l;
    // 0x0F000680: lwc1        $f16, 0xC($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0XC);
    // 0x0F000684: swc1        $f16, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->f16.u32l;
    // 0x0F000688: b           L_0F000694
    // 0x0F00068C: lhu         $v1, 0x18($a1)
    ctx->r3 = MEM_HU(ctx->r5, 0X18);
        goto L_0F000694;
    // 0x0F00068C: lhu         $v1, 0x18($a1)
    ctx->r3 = MEM_HU(ctx->r5, 0X18);
L_0F000690:
    // 0x0F000690: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_0F000694:
    // 0x0F000694: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x0F000698: lh          $t4, -0x5470($t4)
    ctx->r12 = MEM_H(ctx->r12, -0X5470);
    // 0x0F00069C: addiu       $at, $zero, 0x28
    ctx->r1 = ADD32(0, 0X28);
    // 0x0F0006A0: bnel        $t4, $at, L_0F0006F8
    if (ctx->r12 != ctx->r1) {
        // 0x0F0006A4: slti        $at, $v1, 0x2
        ctx->r1 = SIGNED(ctx->r3) < 0X2 ? 1 : 0;
            goto L_0F0006F8;
    }
    goto skip_0;
    // 0x0F0006A4: slti        $at, $v1, 0x2
    ctx->r1 = SIGNED(ctx->r3) < 0X2 ? 1 : 0;
    skip_0:
    // 0x0F0006A8: lw          $t9, 0x2C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0006AC: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F0006B0: addiu       $a0, $a0, 0x3C68
    ctx->r4 = ADD32(ctx->r4, 0X3C68);
    // 0x0F0006B4: jalr        $t9
    // 0x0F0006B8: sw          $v1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_19;
    // 0x0F0006B8: sw          $v1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r3;
    after_19:
    // 0x0F0006BC: lui         $at, 0xC2F0
    ctx->r1 = S32(0XC2F0 << 16);
    // 0x0F0006C0: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F0006C4: lui         $at, 0x4382
    ctx->r1 = S32(0X4382 << 16);
    // 0x0F0006C8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0006CC: lw          $v1, 0x48($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X48);
    // 0x0F0006D0: sw          $v0, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->r2;
    // 0x0F0006D4: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0006D8: swc1        $f18, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->f18.u32l;
    // 0x0F0006DC: swc1        $f4, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->f4.u32l;
    // 0x0F0006E0: lwc1        $f6, 0x3D4C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X3D4C);
    // 0x0F0006E4: addiu       $t5, $zero, -0x8000
    ctx->r13 = ADD32(0, -0X8000);
    // 0x0F0006E8: sh          $t5, 0x5E($s0)
    MEM_H(0X5E, ctx->r16) = ctx->r13;
    // 0x0F0006EC: swc1        $f6, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->f6.u32l;
    // 0x0F0006F0: lw          $a1, 0x4C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4C);
    // 0x0F0006F4: slti        $at, $v1, 0x2
    ctx->r1 = SIGNED(ctx->r3) < 0X2 ? 1 : 0;
L_0F0006F8:
    // 0x0F0006F8: beql        $at, $zero, L_0F000754
    if (ctx->r1 == 0) {
        // 0x0F0006FC: lwc1        $f8, 0x68($s0)
        ctx->f8.u32l = MEM_W(ctx->r16, 0X68);
            goto L_0F000754;
    }
    goto skip_1;
    // 0x0F0006FC: lwc1        $f8, 0x68($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X68);
    skip_1:
    // 0x0F000700: lwc1        $f8, 0x68($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X68);
    // 0x0F000704: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000708: ldc1        $f0, 0x3D50($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X3D50);
    // 0x0F00070C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x0F000710: lwc1        $f4, 0x6C($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X6C);
    // 0x0F000714: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x0F000718: sh          $v1, 0x28($s0)
    MEM_H(0X28, ctx->r16) = ctx->r3;
    // 0x0F00071C: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x0F000720: sh          $v1, 0x2A($s0)
    MEM_H(0X2A, ctx->r16) = ctx->r3;
    // 0x0F000724: mul.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x0F000728: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x0F00072C: lwc1        $f16, 0x70($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X70);
    // 0x0F000730: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x0F000734: swc1        $f18, 0x68($s0)
    MEM_W(0X68, ctx->r16) = ctx->f18.u32l;
    // 0x0F000738: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x0F00073C: swc1        $f10, 0x6C($s0)
    MEM_W(0X6C, ctx->r16) = ctx->f10.u32l;
    // 0x0F000740: mul.d       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f0.d);
    // 0x0F000744: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F000748: b           L_0F000810
    // 0x0F00074C: swc1        $f6, 0x70($s0)
    MEM_W(0X70, ctx->r16) = ctx->f6.u32l;
        goto L_0F000810;
    // 0x0F00074C: swc1        $f6, 0x70($s0)
    MEM_W(0X70, ctx->r16) = ctx->f6.u32l;
    // 0x0F000750: lwc1        $f8, 0x68($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X68);
L_0F000754:
    // 0x0F000754: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000758: ldc1        $f0, 0x3D58($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X3D58);
    // 0x0F00075C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x0F000760: lwc1        $f4, 0x6C($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X6C);
    // 0x0F000764: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x0F000768: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00076C: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x0F000770: ldc1        $f2, 0x3D60($at)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r1, 0X3D60);
    // 0x0F000774: mul.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x0F000778: lw          $t6, 0x14($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X14);
    // 0x0F00077C: addiu       $t4, $zero, 0x2
    ctx->r12 = ADD32(0, 0X2);
    // 0x0F000780: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x0F000784: lwc1        $f16, 0x70($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X70);
    // 0x0F000788: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x0F00078C: swc1        $f18, 0x68($s0)
    MEM_W(0X68, ctx->r16) = ctx->f18.u32l;
    // 0x0F000790: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x0F000794: swc1        $f10, 0x6C($s0)
    MEM_W(0X6C, ctx->r16) = ctx->f10.u32l;
    // 0x0F000798: mul.d       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f0.d);
    // 0x0F00079C: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F0007A0: swc1        $f6, 0x70($s0)
    MEM_W(0X70, ctx->r16) = ctx->f6.u32l;
    // 0x0F0007A4: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x0F0007A8: lw          $v0, 0x14($t7)
    ctx->r2 = MEM_W(ctx->r15, 0X14);
    // 0x0F0007AC: lwc1        $f8, 0x68($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X68);
    // 0x0F0007B0: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x0F0007B4: mul.d       $f16, $f10, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f2.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f2.d);
    // 0x0F0007B8: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x0F0007BC: swc1        $f18, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f18.u32l;
    // 0x0F0007C0: lw          $t8, 0x14($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X14);
    // 0x0F0007C4: lw          $t1, 0x14($t8)
    ctx->r9 = MEM_W(ctx->r24, 0X14);
    // 0x0F0007C8: lw          $v0, 0x14($t1)
    ctx->r2 = MEM_W(ctx->r9, 0X14);
    // 0x0F0007CC: lwc1        $f4, 0x6C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X6C);
    // 0x0F0007D0: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x0F0007D4: mul.d       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f2.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f2.d);
    // 0x0F0007D8: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x0F0007DC: swc1        $f10, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f10.u32l;
    // 0x0F0007E0: lw          $t2, 0x14($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X14);
    // 0x0F0007E4: lw          $t3, 0x14($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X14);
    // 0x0F0007E8: lw          $v0, 0x14($t3)
    ctx->r2 = MEM_W(ctx->r11, 0X14);
    // 0x0F0007EC: lwc1        $f16, 0x70($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X70);
    // 0x0F0007F0: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x0F0007F4: mul.d       $f4, $f18, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f2.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f2.d);
    // 0x0F0007F8: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F0007FC: swc1        $f6, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f6.u32l;
    // 0x0F000800: sh          $t4, 0x28($s0)
    MEM_H(0X28, ctx->r16) = ctx->r12;
    // 0x0F000804: sh          $v1, 0x2A($s0)
    MEM_H(0X2A, ctx->r16) = ctx->r3;
    // 0x0F000808: lhu         $t9, 0x16($a1)
    ctx->r25 = MEM_HU(ctx->r5, 0X16);
    // 0x0F00080C: sh          $t9, 0x5E($s0)
    MEM_H(0X5E, ctx->r16) = ctx->r25;
L_0F000810:
    // 0x0F000810: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x0F000814: addiu       $t9, $t9, 0x3234
    ctx->r25 = ADD32(ctx->r25, 0X3234);
    // 0x0F000818: jalr        $t9
    // 0x0F00081C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_20;
    // 0x0F00081C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_20:
    // 0x0F000820: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000824: addiu       $t9, $t9, 0x5668
    ctx->r25 = ADD32(ctx->r25, 0X5668);
    // 0x0F000828: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F00082C: jalr        $t9
    // 0x0F000830: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_21;
    // 0x0F000830: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_21:
    // 0x0F000834: lw          $t6, 0x30($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X30);
    // 0x0F000838: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00083C: addiu       $t5, $zero, 0x4E
    ctx->r13 = ADD32(0, 0X4E);
    // 0x0F000840: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000844: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x0F000848: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    // 0x0F00084C: jalr        $t9
    // 0x0F000850: sw          $t5, 0x28($t6)
    MEM_W(0X28, ctx->r14) = ctx->r13;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_22;
    // 0x0F000850: sw          $t5, 0x28($t6)
    MEM_W(0X28, ctx->r14) = ctx->r13;
    after_22:
    // 0x0F000854: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F000858:
    // 0x0F000858: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x0F00085C: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x0F000860: jr          $ra
    // 0x0F000864: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x0F000864: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void ni_ovl_051_func_0F000868(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000868: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F00086C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000870: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F000874: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x0F000878: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F00087C: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F000880: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x0F000884: addiu       $at, $zero, 0x28
    ctx->r1 = ADD32(0, 0X28);
    // 0x0F000888: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x0F00088C: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x0F000890: lh          $t8, 0x28D0($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X28D0);
    // 0x0F000894: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F000898: bnel        $t8, $at, L_0F0008B4
    if (ctx->r24 != ctx->r1) {
        // 0x0F00089C: lw          $t0, 0x2BD0($a1)
        ctx->r8 = MEM_W(ctx->r5, 0X2BD0);
            goto L_0F0008B4;
    }
    goto skip_0;
    // 0x0F00089C: lw          $t0, 0x2BD0($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X2BD0);
    skip_0:
    // 0x0F0008A0: lw          $t9, 0x2BC8($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X2BC8);
    // 0x0F0008A4: addiu       $at, $zero, 0x4E
    ctx->r1 = ADD32(0, 0X4E);
    // 0x0F0008A8: bnel        $t9, $at, L_0F00091C
    if (ctx->r25 != ctx->r1) {
        // 0x0F0008AC: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_0F00091C;
    }
    goto skip_1;
    // 0x0F0008AC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x0F0008B0: lw          $t0, 0x2BD0($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X2BD0);
L_0F0008B4:
    // 0x0F0008B4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0008B8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0008BC: andi        $t1, $t0, 0x1
    ctx->r9 = ctx->r8 & 0X1;
    // 0x0F0008C0: beq         $t1, $zero, L_0F000910
    if (ctx->r9 == 0) {
        // 0x0F0008C4: addiu       $a0, $a2, 0x8
        ctx->r4 = ADD32(ctx->r6, 0X8);
            goto L_0F000910;
    }
    // 0x0F0008C4: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F0008C8: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F0008CC: addiu       $t9, $t9, 0x7954
    ctx->r25 = ADD32(ctx->r25, 0X7954);
    // 0x0F0008D0: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x0F0008D4: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x0F0008D8: jalr        $t9
    // 0x0F0008DC: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0008DC: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    after_0:
    // 0x0F0008E0: beq         $v0, $zero, L_0F000918
    if (ctx->r2 == 0) {
        // 0x0F0008E4: lw          $a0, 0x24($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X24);
            goto L_0F000918;
    }
    // 0x0F0008E4: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x0F0008E8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0008EC: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F0008F0: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x0F0008F4: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x0F0008F8: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F0008FC: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F000900: jalr        $t9
    // 0x0F000904: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000904: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x0F000908: b           L_0F00091C
    // 0x0F00090C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F00091C;
    // 0x0F00090C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000910:
    // 0x0F000910: jalr        $t9
    // 0x0F000914: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000914: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_2:
L_0F000918:
    // 0x0F000918: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F00091C:
    // 0x0F00091C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F000920: jr          $ra
    // 0x0F000924: nop

    return;
    // 0x0F000924: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000928(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000928: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x0F00092C: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x0F000930: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000934: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x0F000938: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x0F00093C: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F000940: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F000944: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000948: lw          $s4, 0x24($a0)
    ctx->r20 = MEM_W(ctx->r4, 0X24);
    // 0x0F00094C: jal         0x0F000EB8
    // 0x0F000950: lw          $s2, 0x34($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X34);
    static_105_0F000EB8(rdram, ctx);
        goto after_0;
    // 0x0F000950: lw          $s2, 0x34($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X34);
    after_0:
    // 0x0F000954: addiu       $s1, $s2, 0x8
    ctx->r17 = ADD32(ctx->r18, 0X8);
    // 0x0F000958: lbu         $t7, 0x90($s1)
    ctx->r15 = MEM_BU(ctx->r17, 0X90);
    // 0x0F00095C: addiu       $s3, $s2, 0xDC
    ctx->r19 = ADD32(ctx->r18, 0XDC);
    // 0x0F000960: lw          $a1, 0x0($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X0);
    // 0x0F000964: ori         $t8, $t7, 0x80
    ctx->r24 = ctx->r15 | 0X80;
    // 0x0F000968: sb          $t8, 0x90($s1)
    MEM_B(0X90, ctx->r17) = ctx->r24;
    // 0x0F00096C: lw          $a3, 0x28($s3)
    ctx->r7 = MEM_W(ctx->r19, 0X28);
    // 0x0F000970: addiu       $t9, $zero, 0x4
    ctx->r25 = ADD32(0, 0X4);
    // 0x0F000974: sw          $t9, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r25;
    // 0x0F000978: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F00097C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000980: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000984: addiu       $t9, $t9, 0x7620
    ctx->r25 = ADD32(ctx->r25, 0X7620);
    // 0x0F000988: addiu       $a0, $s1, 0x4
    ctx->r4 = ADD32(ctx->r17, 0X4);
    // 0x0F00098C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x0F000990: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000994: jalr        $t9
    // 0x0F000998: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000998: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_1:
    // 0x0F00099C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0009A0: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F0009A4: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x0F0009A8: jalr        $t9
    // 0x0F0009AC: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0009AC: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    after_2:
    // 0x0F0009B0: lbu         $t1, 0x90($s1)
    ctx->r9 = MEM_BU(ctx->r17, 0X90);
    // 0x0F0009B4: sw          $v0, 0x84($s1)
    MEM_W(0X84, ctx->r17) = ctx->r2;
    // 0x0F0009B8: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F0009BC: ori         $t2, $t1, 0x8
    ctx->r10 = ctx->r9 | 0X8;
    // 0x0F0009C0: sb          $t2, 0x90($s1)
    MEM_B(0X90, ctx->r17) = ctx->r10;
    // 0x0F0009C4: lw          $t3, 0x0($s2)
    ctx->r11 = MEM_W(ctx->r18, 0X0);
    // 0x0F0009C8: addiu       $t9, $t9, -0x6164
    ctx->r25 = ADD32(ctx->r25, -0X6164);
    // 0x0F0009CC: addiu       $a1, $s3, 0x8
    ctx->r5 = ADD32(ctx->r19, 0X8);
    // 0x0F0009D0: sw          $t3, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r11;
    // 0x0F0009D4: lw          $t4, 0x14($s4)
    ctx->r12 = MEM_W(ctx->r20, 0X14);
    // 0x0F0009D8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F0009DC: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    // 0x0F0009E0: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x0F0009E4: lw          $a0, 0x14($t5)
    ctx->r4 = MEM_W(ctx->r13, 0X14);
    // 0x0F0009E8: jalr        $t9
    // 0x0F0009EC: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F0009EC: addiu       $a0, $a0, 0xA8
    ctx->r4 = ADD32(ctx->r4, 0XA8);
    after_3:
    // 0x0F0009F0: swc1        $f0, 0x14($s3)
    MEM_W(0X14, ctx->r19) = ctx->f0.u32l;
    // 0x0F0009F4: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    // 0x0F0009F8: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F0009FC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000A00: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000A04: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000A08: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000A0C: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F000A10: addu        $v0, $s0, $t6
    ctx->r2 = ADD32(ctx->r16, ctx->r14);
    // 0x0F000A14: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x0F000A18: lbu         $t0, 0x9($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000A1C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000A20: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x0F000A24: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F000A28: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000A2C: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x0F000A30: lw          $t9, 0x3CAC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3CAC);
    // 0x0F000A34: jalr        $t9
    // 0x0F000A38: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000A38: nop

    after_4:
    // 0x0F000A3C: lh          $t2, 0xE($s0)
    ctx->r10 = MEM_H(ctx->r16, 0XE);
    // 0x0F000A40: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000A44: addiu       $t9, $t9, -0x7FC4
    ctx->r25 = ADD32(ctx->r25, -0X7FC4);
    // 0x0F000A48: addiu       $t3, $t2, -0x1
    ctx->r11 = ADD32(ctx->r10, -0X1);
    // 0x0F000A4C: sh          $t3, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r11;
    // 0x0F000A50: addiu       $a0, $s3, 0x1C
    ctx->r4 = ADD32(ctx->r19, 0X1C);
    // 0x0F000A54: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x0F000A58: jalr        $t9
    // 0x0F000A5C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000A5C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_5:
    // 0x0F000A60: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x0F000A64: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x0F000A68: sh          $v0, 0x5E($s4)
    MEM_H(0X5E, ctx->r20) = ctx->r2;
    // 0x0F000A6C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000A70: jalr        $t9
    // 0x0F000A74: addiu       $a1, $s4, 0x50
    ctx->r5 = ADD32(ctx->r20, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000A74: addiu       $a1, $s4, 0x50
    ctx->r5 = ADD32(ctx->r20, 0X50);
    after_6:
    // 0x0F000A78: beql        $v0, $zero, L_0F000A94
    if (ctx->r2 == 0) {
        // 0x0F000A7C: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_0F000A94;
    }
    goto skip_0;
    // 0x0F000A7C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_0:
    // 0x0F000A80: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x0F000A84: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000A88: jalr        $t9
    // 0x0F000A8C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000A8C: nop

    after_7:
    // 0x0F000A90: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
L_0F000A94:
    // 0x0F000A94: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F000A98: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F000A9C: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F000AA0: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000AA4: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x0F000AA8: jr          $ra
    // 0x0F000AAC: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    return;
    // 0x0F000AAC: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
;}
RECOMP_FUNC void ni_ovl_051_func_0F000AB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000AB0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000AB4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000AB8: lw          $v0, 0x4C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4C);
    // 0x0F000ABC: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F000AC0: beql        $v0, $zero, L_0F000AE8
    if (ctx->r2 == 0) {
        // 0x0F000AC4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F000AE8;
    }
    goto skip_0;
    // 0x0F000AC4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x0F000AC8: lw          $t6, 0x3C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X3C);
    // 0x0F000ACC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000AD0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000AD4: beq         $t6, $zero, L_0F000AE4
    if (ctx->r14 == 0) {
        // 0x0F000AD8: addiu       $a0, $a0, 0x8
        ctx->r4 = ADD32(ctx->r4, 0X8);
            goto L_0F000AE4;
    }
    // 0x0F000AD8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x0F000ADC: jalr        $t9
    // 0x0F000AE0: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000AE0: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_0F000AE4:
    // 0x0F000AE4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F000AE8:
    // 0x0F000AE8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000AEC: jr          $ra
    // 0x0F000AF0: nop

    return;
    // 0x0F000AF0: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000AF4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000AF4: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000AF8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000AFC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F000B00: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x0F000B04: lw          $a2, 0x70($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X70);
    // 0x0F000B08: addiu       $t6, $zero, 0x4D
    ctx->r14 = ADD32(0, 0X4D);
    // 0x0F000B0C: sw          $t6, 0x104($v0)
    MEM_W(0X104, ctx->r2) = ctx->r14;
    // 0x0F000B10: lhu         $v1, 0x14($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X14);
    // 0x0F000B14: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000B18: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x0F000B1C: beq         $v1, $zero, L_0F000B4C
    if (ctx->r3 == 0) {
        // 0x0F000B20: or          $a1, $v1, $zero
        ctx->r5 = ctx->r3 | 0;
            goto L_0F000B4C;
    }
    // 0x0F000B20: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    // 0x0F000B24: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000B28: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x0F000B2C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F000B30: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x0F000B34: jalr        $t9
    // 0x0F000B38: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000B38: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    after_0:
    // 0x0F000B3C: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x0F000B40: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x0F000B44: b           L_0F000B4C
    // 0x0F000B48: lhu         $v1, 0x14($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X14);
        goto L_0F000B4C;
    // 0x0F000B48: lhu         $v1, 0x14($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X14);
L_0F000B4C:
    // 0x0F000B4C: beq         $a0, $zero, L_0F000BCC
    if (ctx->r4 == 0) {
        // 0x0F000B50: addiu       $at, $zero, 0x2A5
        ctx->r1 = ADD32(0, 0X2A5);
            goto L_0F000BCC;
    }
    // 0x0F000B50: addiu       $at, $zero, 0x2A5
    ctx->r1 = ADD32(0, 0X2A5);
    // 0x0F000B54: addiu       $at, $zero, 0x2A5
    ctx->r1 = ADD32(0, 0X2A5);
    // 0x0F000B58: bne         $v1, $at, L_0F000B80
    if (ctx->r3 != ctx->r1) {
        // 0x0F000B5C: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_0F000B80;
    }
    // 0x0F000B5C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x0F000B60: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F000B64: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F000B68: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    // 0x0F000B6C: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    // 0x0F000B70: jalr        $t9
    // 0x0F000B74: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000B74: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
    after_1:
    // 0x0F000B78: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x0F000B7C: lhu         $v0, 0x14($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X14);
L_0F000B80:
    // 0x0F000B80: addiu       $at, $zero, 0x2A7
    ctx->r1 = ADD32(0, 0X2A7);
    // 0x0F000B84: bne         $v0, $at, L_0F000BA8
    if (ctx->r2 != ctx->r1) {
        // 0x0F000B88: addiu       $a1, $zero, 0x2
        ctx->r5 = ADD32(0, 0X2);
            goto L_0F000BA8;
    }
    // 0x0F000B88: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x0F000B8C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F000B90: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F000B94: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    // 0x0F000B98: jalr        $t9
    // 0x0F000B9C: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000B9C: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    after_2:
    // 0x0F000BA0: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x0F000BA4: lhu         $v0, 0x14($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X14);
L_0F000BA8:
    // 0x0F000BA8: addiu       $at, $zero, 0x2A9
    ctx->r1 = ADD32(0, 0X2A9);
    // 0x0F000BAC: bne         $v0, $at, L_0F000C38
    if (ctx->r2 != ctx->r1) {
        // 0x0F000BB0: addiu       $a1, $zero, 0x3
        ctx->r5 = ADD32(0, 0X3);
            goto L_0F000C38;
    }
    // 0x0F000BB0: addiu       $a1, $zero, 0x3
    ctx->r5 = ADD32(0, 0X3);
    // 0x0F000BB4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F000BB8: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F000BBC: jalr        $t9
    // 0x0F000BC0: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000BC0: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    after_3:
    // 0x0F000BC4: b           L_0F000C38
    // 0x0F000BC8: nop

        goto L_0F000C38;
    // 0x0F000BC8: nop

L_0F000BCC:
    // 0x0F000BCC: bne         $v1, $at, L_0F000BF4
    if (ctx->r3 != ctx->r1) {
        // 0x0F000BD0: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_0F000BF4;
    }
    // 0x0F000BD0: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x0F000BD4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F000BD8: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F000BDC: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    // 0x0F000BE0: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    // 0x0F000BE4: jalr        $t9
    // 0x0F000BE8: addiu       $a1, $zero, 0x9
    ctx->r5 = ADD32(0, 0X9);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000BE8: addiu       $a1, $zero, 0x9
    ctx->r5 = ADD32(0, 0X9);
    after_4:
    // 0x0F000BEC: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x0F000BF0: lhu         $v0, 0x14($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X14);
L_0F000BF4:
    // 0x0F000BF4: addiu       $at, $zero, 0x2A7
    ctx->r1 = ADD32(0, 0X2A7);
    // 0x0F000BF8: bne         $v0, $at, L_0F000C1C
    if (ctx->r2 != ctx->r1) {
        // 0x0F000BFC: addiu       $a1, $zero, 0x1
        ctx->r5 = ADD32(0, 0X1);
            goto L_0F000C1C;
    }
    // 0x0F000BFC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000C00: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F000C04: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F000C08: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    // 0x0F000C0C: jalr        $t9
    // 0x0F000C10: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000C10: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    after_5:
    // 0x0F000C14: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x0F000C18: lhu         $v0, 0x14($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X14);
L_0F000C1C:
    // 0x0F000C1C: addiu       $at, $zero, 0x2A9
    ctx->r1 = ADD32(0, 0X2A9);
    // 0x0F000C20: bne         $v0, $at, L_0F000C38
    if (ctx->r2 != ctx->r1) {
        // 0x0F000C24: addiu       $a1, $zero, 0x2
        ctx->r5 = ADD32(0, 0X2);
            goto L_0F000C38;
    }
    // 0x0F000C24: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x0F000C28: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x0F000C2C: addiu       $t9, $t9, -0x6FE4
    ctx->r25 = ADD32(ctx->r25, -0X6FE4);
    // 0x0F000C30: jalr        $t9
    // 0x0F000C34: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000C34: lw          $a0, 0x4C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4C);
    after_6:
L_0F000C38:
    // 0x0F000C38: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000C3C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000C40: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000C44: jalr        $t9
    // 0x0F000C48: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000C48: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_7:
    // 0x0F000C4C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000C50: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000C54: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F000C58: jr          $ra
    // 0x0F000C5C: nop

    return;
    // 0x0F000C5C: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000C60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000C60: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x0F000C64: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000C68: lw          $a1, 0x24($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X24);
    // 0x0F000C6C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000C70: addiu       $t9, $t9, -0x6274
    ctx->r25 = ADD32(ctx->r25, -0X6274);
    // 0x0F000C74: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x0F000C78: jalr        $t9
    // 0x0F000C7C: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000C7C: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    after_0:
    // 0x0F000C80: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x0F000C84: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x0F000C88: lh          $a3, 0x5E($a1)
    ctx->r7 = MEM_H(ctx->r5, 0X5E);
    // 0x0F000C8C: subu        $v1, $a3, $v0
    ctx->r3 = SUB32(ctx->r7, ctx->r2);
    // 0x0F000C90: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000C94: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000C98: slti        $at, $v1, -0x27F
    ctx->r1 = SIGNED(ctx->r3) < -0X27F ? 1 : 0;
    // 0x0F000C9C: bne         $at, $zero, L_0F000CB4
    if (ctx->r1 != 0) {
        // 0x0F000CA0: slti        $at, $v1, 0x280
        ctx->r1 = SIGNED(ctx->r3) < 0X280 ? 1 : 0;
            goto L_0F000CB4;
    }
    // 0x0F000CA0: slti        $at, $v1, 0x280
    ctx->r1 = SIGNED(ctx->r3) < 0X280 ? 1 : 0;
    // 0x0F000CA4: beq         $at, $zero, L_0F000CB4
    if (ctx->r1 == 0) {
        // 0x0F000CA8: addiu       $a0, $a2, 0x8
        ctx->r4 = ADD32(ctx->r6, 0X8);
            goto L_0F000CB4;
    }
    // 0x0F000CA8: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F000CAC: b           L_0F000D28
    // 0x0F000CB0: sh          $v0, 0x5E($a1)
    MEM_H(0X5E, ctx->r5) = ctx->r2;
        goto L_0F000D28;
    // 0x0F000CB0: sh          $v0, 0x5E($a1)
    MEM_H(0X5E, ctx->r5) = ctx->r2;
L_0F000CB4:
    // 0x0F000CB4: mtc1        $v1, $f4
    ctx->f4.u32l = ctx->r3;
    // 0x0F000CB8: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x0F000CBC: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x0F000CC0: cvt.d.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
    // 0x0F000CC4: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x0F000CC8: nop

    // 0x0F000CCC: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x0F000CD0: trunc.w.d   $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.u32l = TRUNC_W_D(ctx->f10.d);
    // 0x0F000CD4: mfc1        $v0, $f16
    ctx->r2 = (int32_t)ctx->f16.u32l;
    // 0x0F000CD8: nop

    // 0x0F000CDC: sll         $a0, $v0, 16
    ctx->r4 = S32(ctx->r2 << 16);
    // 0x0F000CE0: bgez        $v1, L_0F000D04
    if (SIGNED(ctx->r3) >= 0) {
        // 0x0F000CE4: sra         $a0, $a0, 16
        ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
            goto L_0F000D04;
    }
    // 0x0F000CE4: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x0F000CE8: sll         $t7, $v0, 16
    ctx->r15 = S32(ctx->r2 << 16);
    // 0x0F000CEC: sra         $t8, $t7, 16
    ctx->r24 = S32(SIGNED(ctx->r15) >> 16);
    // 0x0F000CF0: slti        $at, $t8, -0x500
    ctx->r1 = SIGNED(ctx->r24) < -0X500 ? 1 : 0;
    // 0x0F000CF4: beql        $at, $zero, L_0F000D20
    if (ctx->r1 == 0) {
        // 0x0F000CF8: subu        $t2, $a3, $a0
        ctx->r10 = SUB32(ctx->r7, ctx->r4);
            goto L_0F000D20;
    }
    goto skip_0;
    // 0x0F000CF8: subu        $t2, $a3, $a0
    ctx->r10 = SUB32(ctx->r7, ctx->r4);
    skip_0:
    // 0x0F000CFC: b           L_0F000D1C
    // 0x0F000D00: addiu       $a0, $zero, -0x500
    ctx->r4 = ADD32(0, -0X500);
        goto L_0F000D1C;
    // 0x0F000D00: addiu       $a0, $zero, -0x500
    ctx->r4 = ADD32(0, -0X500);
L_0F000D04:
    // 0x0F000D04: sll         $t0, $v0, 16
    ctx->r8 = S32(ctx->r2 << 16);
    // 0x0F000D08: sra         $t1, $t0, 16
    ctx->r9 = S32(SIGNED(ctx->r8) >> 16);
    // 0x0F000D0C: slti        $at, $t1, 0x501
    ctx->r1 = SIGNED(ctx->r9) < 0X501 ? 1 : 0;
    // 0x0F000D10: bnel        $at, $zero, L_0F000D20
    if (ctx->r1 != 0) {
        // 0x0F000D14: subu        $t2, $a3, $a0
        ctx->r10 = SUB32(ctx->r7, ctx->r4);
            goto L_0F000D20;
    }
    goto skip_1;
    // 0x0F000D14: subu        $t2, $a3, $a0
    ctx->r10 = SUB32(ctx->r7, ctx->r4);
    skip_1:
    // 0x0F000D18: addiu       $a0, $zero, 0x500
    ctx->r4 = ADD32(0, 0X500);
L_0F000D1C:
    // 0x0F000D1C: subu        $t2, $a3, $a0
    ctx->r10 = SUB32(ctx->r7, ctx->r4);
L_0F000D20:
    // 0x0F000D20: b           L_0F000D38
    // 0x0F000D24: sh          $t2, 0x5E($a1)
    MEM_H(0X5E, ctx->r5) = ctx->r10;
        goto L_0F000D38;
    // 0x0F000D24: sh          $t2, 0x5E($a1)
    MEM_H(0X5E, ctx->r5) = ctx->r10;
L_0F000D28:
    // 0x0F000D28: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000D2C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000D30: jalr        $t9
    // 0x0F000D34: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000D34: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_0F000D38:
    // 0x0F000D38: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000D3C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x0F000D40: jr          $ra
    // 0x0F000D44: nop

    return;
    // 0x0F000D44: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000D48(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000D48: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x0F000D4C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000D50: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x0F000D54: lw          $v0, 0x4C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4C);
    // 0x0F000D58: lw          $a2, 0x70($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X70);
    // 0x0F000D5C: lw          $t7, 0x3C($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X3C);
    // 0x0F000D60: bnel        $t7, $zero, L_0F000E48
    if (ctx->r15 != 0) {
        // 0x0F000D64: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F000E48;
    }
    goto skip_0;
    // 0x0F000D64: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x0F000D68: lhu         $v0, 0x14($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X14);
    // 0x0F000D6C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F000D70: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x0F000D74: beq         $v0, $zero, L_0F000D9C
    if (ctx->r2 == 0) {
        // 0x0F000D78: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_0F000D9C;
    }
    // 0x0F000D78: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x0F000D7C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000D80: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x0F000D84: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x0F000D88: jalr        $t9
    // 0x0F000D8C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000D8C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x0F000D90: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x0F000D94: b           L_0F000D9C
    // 0x0F000D98: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_0F000D9C;
    // 0x0F000D98: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_0F000D9C:
    // 0x0F000D9C: bnel        $v1, $zero, L_0F000E2C
    if (ctx->r3 != 0) {
        // 0x0F000DA0: lw          $t8, 0x30($sp)
        ctx->r24 = MEM_W(ctx->r29, 0X30);
            goto L_0F000E2C;
    }
    goto skip_1;
    // 0x0F000DA0: lw          $t8, 0x30($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X30);
    skip_1:
    // 0x0F000DA4: lhu         $a1, 0x14($a2)
    ctx->r5 = MEM_HU(ctx->r6, 0X14);
    // 0x0F000DA8: addiu       $at, $zero, 0x2A5
    ctx->r1 = ADD32(0, 0X2A5);
    // 0x0F000DAC: addiu       $a0, $zero, 0x41F
    ctx->r4 = ADD32(0, 0X41F);
    // 0x0F000DB0: bne         $a1, $at, L_0F000DCC
    if (ctx->r5 != ctx->r1) {
        // 0x0F000DB4: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_0F000DCC;
    }
    // 0x0F000DB4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000DB8: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F000DBC: jalr        $t9
    // 0x0F000DC0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000DC0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_1:
    // 0x0F000DC4: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x0F000DC8: lhu         $a1, 0x14($a2)
    ctx->r5 = MEM_HU(ctx->r6, 0X14);
L_0F000DCC:
    // 0x0F000DCC: addiu       $at, $zero, 0x2A7
    ctx->r1 = ADD32(0, 0X2A7);
    // 0x0F000DD0: bne         $a1, $at, L_0F000DF0
    if (ctx->r5 != ctx->r1) {
        // 0x0F000DD4: addiu       $a0, $zero, 0x41D
        ctx->r4 = ADD32(0, 0X41D);
            goto L_0F000DF0;
    }
    // 0x0F000DD4: addiu       $a0, $zero, 0x41D
    ctx->r4 = ADD32(0, 0X41D);
    // 0x0F000DD8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000DDC: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F000DE0: jalr        $t9
    // 0x0F000DE4: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000DE4: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_2:
    // 0x0F000DE8: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x0F000DEC: lhu         $a1, 0x14($a2)
    ctx->r5 = MEM_HU(ctx->r6, 0X14);
L_0F000DF0:
    // 0x0F000DF0: addiu       $at, $zero, 0x2A9
    ctx->r1 = ADD32(0, 0X2A9);
    // 0x0F000DF4: bne         $a1, $at, L_0F000E14
    if (ctx->r5 != ctx->r1) {
        // 0x0F000DF8: addiu       $a0, $zero, 0x420
        ctx->r4 = ADD32(0, 0X420);
            goto L_0F000E14;
    }
    // 0x0F000DF8: addiu       $a0, $zero, 0x420
    ctx->r4 = ADD32(0, 0X420);
    // 0x0F000DFC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000E00: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F000E04: jalr        $t9
    // 0x0F000E08: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000E08: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_3:
    // 0x0F000E0C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x0F000E10: lhu         $a1, 0x14($a2)
    ctx->r5 = MEM_HU(ctx->r6, 0X14);
L_0F000E14:
    // 0x0F000E14: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000E18: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x0F000E1C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F000E20: jalr        $t9
    // 0x0F000E24: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000E24: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_4:
    // 0x0F000E28: lw          $t8, 0x30($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X30);
L_0F000E2C:
    // 0x0F000E2C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000E30: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x0F000E34: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000E38: addiu       $a0, $t8, 0x8
    ctx->r4 = ADD32(ctx->r24, 0X8);
    // 0x0F000E3C: jalr        $t9
    // 0x0F000E40: addiu       $a1, $t8, 0xE
    ctx->r5 = ADD32(ctx->r24, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000E40: addiu       $a1, $t8, 0xE
    ctx->r5 = ADD32(ctx->r24, 0XE);
    after_5:
    // 0x0F000E44: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F000E48:
    // 0x0F000E48: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x0F000E4C: jr          $ra
    // 0x0F000E50: nop

    return;
    // 0x0F000E50: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000E54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000E54: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F000E58: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000E5C: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x0F000E60: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000E64: addiu       $t9, $t9, 0x5C5C
    ctx->r25 = ADD32(ctx->r25, 0X5C5C);
    // 0x0F000E68: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x0F000E6C: jalr        $t9
    // 0x0F000E70: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000E70: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    after_0:
    // 0x0F000E74: lw          $t7, 0x1C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000E78: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x0F000E7C: addiu       $t9, $t9, 0x938
    ctx->r25 = ADD32(ctx->r25, 0X938);
    // 0x0F000E80: lw          $a0, 0x4($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X4);
    // 0x0F000E84: beql        $a0, $zero, L_0F000E98
    if (ctx->r4 == 0) {
        // 0x0F000E88: lw          $t8, 0x1C($sp)
        ctx->r24 = MEM_W(ctx->r29, 0X1C);
            goto L_0F000E98;
    }
    goto skip_0;
    // 0x0F000E88: lw          $t8, 0x1C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x0F000E8C: jalr        $t9
    // 0x0F000E90: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000E90: nop

    after_1:
    // 0x0F000E94: lw          $t8, 0x1C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X1C);
L_0F000E98:
    // 0x0F000E98: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x0F000E9C: lw          $t9, 0xA8($t8)
    ctx->r25 = MEM_W(ctx->r24, 0XA8);
    // 0x0F000EA0: jalr        $t9
    // 0x0F000EA4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000EA4: nop

    after_2:
    // 0x0F000EA8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000EAC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F000EB0: jr          $ra
    // 0x0F000EB4: nop

    return;
    // 0x0F000EB4: nop

    // 0x0F000EB8: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x0F000EBC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x0F000EC0: swc1        $f0, 0xF8($v0)
    MEM_W(0XF8, ctx->r2) = ctx->f0.u32l;
    // 0x0F000EC4: jr          $ra
    // 0x0F000EC8: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
    return;
    // 0x0F000EC8: swc1        $f0, 0x100($v0)
    MEM_W(0X100, ctx->r2) = ctx->f0.u32l;
;}
RECOMP_FUNC void ni_ovl_051_func_0F000ED0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000ED0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000ED4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000ED8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F000EDC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000EE0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000EE4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000EE8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F000EEC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000EF0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000EF4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000EF8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F000EFC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000F00: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000F04: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000F08: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F000F0C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000F10: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000F14: lw          $t9, 0x3FA8($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3FA8);
    // 0x0F000F18: jalr        $t9
    // 0x0F000F1C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000F1C: nop

    after_0:
    // 0x0F000F20: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000F24: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000F28: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F000F2C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000F30: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000F34: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000F38: jr          $ra
    // 0x0F000F3C: nop

    return;
    // 0x0F000F3C: nop

;}
RECOMP_FUNC void ni_ovl_051_func_0F000F40(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000F40: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x0F000F44: lui         $t7, 0xF00
    ctx->r15 = S32(0XF00 << 16);
    // 0x0F000F48: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x0F000F4C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x0F000F50: addiu       $t7, $t7, 0x3FBC
    ctx->r15 = ADD32(ctx->r15, 0X3FBC);
    // 0x0F000F54: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x0F000F58: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F000F5C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F000F60: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x0F000F64: addiu       $t0, $t7, 0x24
    ctx->r8 = ADD32(ctx->r15, 0X24);
    // 0x0F000F68: addiu       $t6, $sp, 0x48
    ctx->r14 = ADD32(ctx->r29, 0X48);
L_0F000F6C:
    // 0x0F000F6C: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x0F000F70: addiu       $t7, $t7, 0xC
    ctx->r15 = ADD32(ctx->r15, 0XC);
    // 0x0F000F74: addiu       $t6, $t6, 0xC
    ctx->r14 = ADD32(ctx->r14, 0XC);
    // 0x0F000F78: sw          $t9, -0xC($t6)
    MEM_W(-0XC, ctx->r14) = ctx->r25;
    // 0x0F000F7C: lw          $t8, -0x8($t7)
    ctx->r24 = MEM_W(ctx->r15, -0X8);
    // 0x0F000F80: sw          $t8, -0x8($t6)
    MEM_W(-0X8, ctx->r14) = ctx->r24;
    // 0x0F000F84: lw          $t9, -0x4($t7)
    ctx->r25 = MEM_W(ctx->r15, -0X4);
    // 0x0F000F88: bne         $t7, $t0, L_0F000F6C
    if (ctx->r15 != ctx->r8) {
        // 0x0F000F8C: sw          $t9, -0x4($t6)
        MEM_W(-0X4, ctx->r14) = ctx->r25;
            goto L_0F000F6C;
    }
    // 0x0F000F8C: sw          $t9, -0x4($t6)
    MEM_W(-0X4, ctx->r14) = ctx->r25;
    // 0x0F000F90: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x0F000F94: lui         $at, 0x4500
    ctx->r1 = S32(0X4500 << 16);
    // 0x0F000F98: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000F9C: sw          $t9, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->r25;
    // 0x0F000FA0: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x0F000FA4: lwc1        $f4, 0x4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X4);
    // 0x0F000FA8: lw          $s0, 0x64($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X64);
    // 0x0F000FAC: lui         $t3, 0xF00
    ctx->r11 = S32(0XF00 << 16);
    // 0x0F000FB0: div.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F000FB4: lui         $t4, 0xF00
    ctx->r12 = S32(0XF00 << 16);
    // 0x0F000FB8: addiu       $t3, $t3, 0x3D70
    ctx->r11 = ADD32(ctx->r11, 0X3D70);
    // 0x0F000FBC: addiu       $t4, $t4, 0x3DE8
    ctx->r12 = ADD32(ctx->r12, 0X3DE8);
    // 0x0F000FC0: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x0F000FC4: lui         $t0, 0x601
    ctx->r8 = S32(0X601 << 16);
    // 0x0F000FC8: addiu       $t0, $t0, -0x7E00
    ctx->r8 = ADD32(ctx->r8, -0X7E00);
    // 0x0F000FCC: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x0F000FD0: and         $t7, $t0, $at
    ctx->r15 = ctx->r8 & ctx->r1;
    // 0x0F000FD4: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F000FD8: addiu       $t9, $t9, 0x3178
    ctx->r25 = ADD32(ctx->r25, 0X3178);
    // 0x0F000FDC: addiu       $t6, $zero, 0x50
    ctx->r14 = ADD32(0, 0X50);
    // 0x0F000FE0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000FE4: addiu       $a3, $zero, 0x4E0
    ctx->r7 = ADD32(0, 0X4E0);
    // 0x0F000FE8: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x0F000FEC: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x0F000FF0: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x0F000FF4: mfc1        $t2, $f18
    ctx->r10 = (int32_t)ctx->f18.u32l;
    // 0x0F000FF8: nop

    // 0x0F000FFC: sh          $t2, 0x66($sp)
    MEM_H(0X66, ctx->r29) = ctx->r10;
    // 0x0F001000: sw          $t3, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r11;
    // 0x0F001004: sw          $t4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r12;
    // 0x0F001008: lw          $t5, 0x7D8($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X7D8);
    // 0x0F00100C: addu        $a2, $t5, $t7
    ctx->r6 = ADD32(ctx->r13, ctx->r15);
    // 0x0F001010: sw          $a2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r6;
    // 0x0F001014: jalr        $t9
    // 0x0F001018: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F001018: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    after_0:
    // 0x0F00101C: bne         $v0, $zero, L_0F00103C
    if (ctx->r2 != 0) {
        // 0x0F001020: addiu       $a0, $s0, 0x20
        ctx->r4 = ADD32(ctx->r16, 0X20);
            goto L_0F00103C;
    }
    // 0x0F001020: addiu       $a0, $s0, 0x20
    ctx->r4 = ADD32(ctx->r16, 0X20);
    // 0x0F001024: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x0F001028: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F00102C: jalr        $t9
    // 0x0F001030: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F001030: nop

    after_1:
    // 0x0F001034: b           L_0F001064
    // 0x0F001038: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_0F001064;
    // 0x0F001038: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F00103C:
    // 0x0F00103C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F001040: addiu       $t9, $t9, 0x3794
    ctx->r25 = ADD32(ctx->r25, 0X3794);
    // 0x0F001044: jalr        $t9
    // 0x0F001048: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F001048: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    after_2:
    // 0x0F00104C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F001050: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F001054: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x0F001058: jalr        $t9
    // 0x0F00105C: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F00105C: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_3:
    // 0x0F001060: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F001064:
    // 0x0F001064: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x0F001068: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x0F00106C: jr          $ra
    // 0x0F001070: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x0F001070: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void ni_ovl_051_func_0F001074(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F001074: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F001078: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F00107C: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x0F001080: lw          $t6, 0x24($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X24);
    // 0x0F001084: lw          $a0, 0x64($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X64);
    // 0x0F001088: addiu       $a1, $zero, 0x1B
    ctx->r5 = ADD32(0, 0X1B);
    // 0x0F00108C: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x0F001090: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x0F001094: lw          $t9, 0x10($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X10);
    // 0x0F001098: lw          $t0, 0x10($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X10);
    // 0x0F00109C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x0F0010A0: addiu       $t9, $t9, 0x41B8
    ctx->r25 = ADD32(ctx->r25, 0X41B8);
    // 0x0F0010A4: lw          $a2, 0x10($t0)
    ctx->r6 = MEM_W(ctx->r8, 0X10);
    // 0x0F0010A8: jalr        $t9
    // 0x0F0010AC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0010AC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_0:
    // 0x0F0010B0: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x0F0010B4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0010B8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0010BC: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x0F0010C0: jalr        $t9
    // 0x0F0010C4: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0010C4: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_1:
    // 0x0F0010C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F0010CC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0010D0: jr          $ra
    // 0x0F0010D4: nop

    return;
    // 0x0F0010D4: nop

;}
