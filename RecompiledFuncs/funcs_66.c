#include "recomp.h"
#include "funcs.h"
#include <stdio.h>
#include "lod_symbols.h"
#ifndef LOD_ENABLE_INTERACT_TRACE
#define LOD_ENABLE_INTERACT_TRACE 0
#endif

#if LOD_ENABLE_INTERACT_TRACE
extern uint32_t lod_current_map_overlay_rom(void);

static uint32_t lod_interact_trace_scan_counter = 0;
static uint32_t lod_interact_trace_gate_counter = 0;

static int lod_interact_trace_is_tower(void) {
    return lod_current_map_overlay_rom() == 0x007D9790;
}

static int lod_interact_trace_enabled(void) {
    const uint32_t map = lod_current_map_overlay_rom();
    return map != 0;
}

static float lod_interact_trace_f32(uint32_t raw) {
    union {
        uint32_t u;
        float f;
    } v;
    v.u = raw;
    return v.f;
}

static uint64_t lod_interact_trace_guest_addr(uint32_t addr) {
    return (uint64_t)(int64_t)(int32_t)addr;
}

static int lod_interact_trace_rdram_offset(uint64_t guest, int32_t offset, uint32_t size, uint32_t xor_mask, uint64_t* out_offset) {
    const uint64_t base = 0xFFFFFFFF80000000ull;
    const uint64_t limit = 0x00800000ull;
    const int64_t signed_addr = (int64_t)(int32_t)((uint32_t)guest) + (int64_t)offset;
    const uint64_t addr = (uint64_t)signed_addr;
    const uint64_t xaddr = addr ^ (uint64_t)xor_mask;
    if (addr < base || xaddr < base) {
        return 0;
    }
    const uint64_t rdram_offset = xaddr - base;
    if (rdram_offset > limit || size > limit - rdram_offset) {
        return 0;
    }
    *out_offset = rdram_offset;
    return 1;
}

static int lod_interact_trace_can_read(uint64_t guest, int32_t offset, uint32_t size) {
    uint64_t ignored = 0;
    return lod_interact_trace_rdram_offset(guest, offset, size, size == 2 ? 2 : 0, &ignored);
}

static uint32_t lod_interact_trace_read_w(uint8_t* rdram, uint64_t guest, int32_t offset, uint32_t default_value) {
    uint64_t rdram_offset = 0;
    if (!lod_interact_trace_rdram_offset(guest, offset, 4, 0, &rdram_offset)) {
        return default_value;
    }
    return (uint32_t)(*(int32_t*)(rdram + rdram_offset));
}

static uint16_t lod_interact_trace_read_hu(uint8_t* rdram, uint64_t guest, int32_t offset, uint16_t default_value) {
    uint64_t rdram_offset = 0;
    if (!lod_interact_trace_rdram_offset(guest, offset, 2, 2, &rdram_offset)) {
        return default_value;
    }
    return *(uint16_t*)(rdram + rdram_offset);
}

static int16_t lod_interact_trace_read_h(uint8_t* rdram, uint64_t guest, int32_t offset, int16_t default_value) {
    uint64_t rdram_offset = 0;
    if (!lod_interact_trace_rdram_offset(guest, offset, 2, 2, &rdram_offset)) {
        return default_value;
    }
    return *(int16_t*)(rdram + rdram_offset);
}

static uint8_t lod_interact_trace_read_bu(uint8_t* rdram, uint64_t guest, int32_t offset, uint8_t default_value) {
    uint64_t rdram_offset = 0;
    if (!lod_interact_trace_rdram_offset(guest, offset, 1, 3, &rdram_offset)) {
        return default_value;
    }
    return *(uint8_t*)(rdram + rdram_offset);
}

static const char* lod_interact_trace_kind(uint32_t type, uint32_t item) {
    if (type == 1 && item == 1) {
        return "save-crystal";
    }
    if (type == 1) {
        return "item";
    }
    if (type == 2) {
        return "text";
    }
    return "unknown";
}

static void lod_interact_trace_textbox_fields(uint8_t* rdram,
                                              uint32_t textbox,
                                              uint32_t* flags,
                                              uint32_t* prev_option,
                                              uint32_t* option,
                                              uint32_t* display_time,
                                              uint32_t* window_flags,
                                              uint32_t* lens) {
    if (textbox == 0 || !lod_interact_trace_can_read(lod_interact_trace_guest_addr(textbox), 0, 4)) {
        *flags = 0xFFFFFFFFu;
        *prev_option = 0xFFu;
        *option = 0xFFu;
        *display_time = 0xFFu;
        *window_flags = 0xFFFFFFFFu;
        *lens = 0;
        return;
    }
    const uint64_t tb = lod_interact_trace_guest_addr(textbox);
    *flags = lod_interact_trace_read_w(rdram, tb, 0x00, 0xFFFFFFFFu);
    *prev_option = lod_interact_trace_read_bu(rdram, tb, 0x2D, 0xFFu);
    *option = lod_interact_trace_read_bu(rdram, tb, 0x2E, 0xFFu);
    *display_time = lod_interact_trace_read_bu(rdram, tb, 0x3A, 0xFFu);
    *window_flags = lod_interact_trace_read_w(rdram, tb, 0x54, 0xFFFFFFFFu);
    *lens = lod_interact_trace_read_w(rdram, tb, 0x5C, 0);
}

static void lod_interact_trace_object(uint8_t* rdram, const char* tag, uint64_t obj, uint32_t result, uint32_t scan_id) {
    if (obj == 0) {
        fprintf(stderr, "[INTERACT_TRACE] %s scan=%u obj=NULL result=%u map=0x%08X\n",
                tag, scan_id, result, lod_current_map_overlay_rom());
        return;
    }
    if (!lod_interact_trace_can_read(obj, 0, 2)) {
        fprintf(stderr, "[INTERACT_TRACE] %s scan=%u obj=0x%08X unreadable result=%u map=0x%08X\n",
                tag, scan_id, (uint32_t)obj, result, lod_current_map_overlay_rom());
        return;
    }

    const uint32_t idx = lod_interact_trace_read_hu(rdram, obj, 0x38, 0xFFFF);
    const uint64_t cfg = (idx < 0x80) ? lod_interact_trace_guest_addr(0x80196778u + idx * 0x14u) : 0;
    const uint32_t cfg0 = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x00, 0xFFFF) : 0xFFFF;
    const uint32_t cfg2 = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x02, 0xFFFF) : 0xFFFF;
    const uint32_t cfg4 = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x04, 0xFFFF) : 0xFFFF;
    const uint32_t cfg6 = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x06, 0xFFFF) : 0xFFFF;
    const uint32_t cfg8 = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x08, 0xFFFF) : 0xFFFF;
    const uint32_t cfgA = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x0A, 0xFFFF) : 0xFFFF;
    const uint32_t cfgC = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x0C, 0xFFFF) : 0xFFFF;
    const uint32_t cfgE = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x0E, 0xFFFF) : 0xFFFF;
    const uint32_t cfg10 = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x10, 0xFFFF) : 0xFFFF;
    const uint32_t cfg12 = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x12, 0xFFFF) : 0xFFFF;
    const uint64_t player = lod_interact_trace_guest_addr(lod_interact_trace_read_w(rdram, lod_interact_trace_guest_addr(0x801CAC20u), 0, 0));
    const float actor_x = lod_interact_trace_f32(lod_interact_trace_read_w(rdram, obj, 0x64, 0));
    const float actor_y = lod_interact_trace_f32(lod_interact_trace_read_w(rdram, obj, 0x68, 0));
    const float actor_z = lod_interact_trace_f32(lod_interact_trace_read_w(rdram, obj, 0x40, 0));
    const float player_x = player != 0 ? lod_interact_trace_f32(lod_interact_trace_read_w(rdram, player, 0x50, 0)) : 0.0f;
    const float player_y = player != 0 ? lod_interact_trace_f32(lod_interact_trace_read_w(rdram, player, 0x54, 0)) : 0.0f;
    const float player_z = player != 0 ? lod_interact_trace_f32(lod_interact_trace_read_w(rdram, player, 0x58, 0)) : 0.0f;
    const uint32_t player_yaw = player != 0 ? lod_interact_trace_read_hu(rdram, player, 0x5E, 0) : 0;
    const int16_t contpak_file_no = lod_interact_trace_read_h(rdram, lod_interact_trace_guest_addr(0x801CAE1Cu), 0, -32768);

    fprintf(stderr,
            "[INTERACT_TRACE] %s scan=%u result=%u map=0x%08X obj=0x%08X id=0x%04X idx=%u flag3C=%u settings=0x%08X cfg=(%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X) actor_xyz=(%.2f,%.2f,%.2f) player=0x%08X player_xyz=(%.2f,%.2f,%.2f) delta=(%.2f,%.2f,%.2f) trigger=%u trigXZ=(%d,%d) yaw=0x%04X contpak=%d\n",
            tag,
            scan_id,
            result,
            lod_current_map_overlay_rom(),
            (uint32_t)obj,
            lod_interact_trace_read_hu(rdram, obj, 0x00, 0xFFFF),
            idx,
            lod_interact_trace_read_w(rdram, obj, 0x3C, 0xFFFFFFFFu),
            lod_interact_trace_read_w(rdram, obj, 0x70, 0),
            cfg0,
            cfg2,
            cfg4,
            cfg6,
            cfg8,
            cfgA,
            cfgC,
            cfgE,
            cfg10,
            cfg12,
            actor_x,
            actor_y,
            actor_z,
            (uint32_t)player,
            player_x,
            player_y,
            player_z,
            player_x - actor_x,
            player_y - actor_y,
            player_z - actor_z,
            cfg10,
            lod_interact_trace_read_h(rdram, obj, 0x5C, 0),
            lod_interact_trace_read_h(rdram, obj, 0x5E, 0),
            player_yaw,
            contpak_file_no);
}

static void lod_interact_trace_state(uint8_t* rdram, const char* tag, uint64_t obj) {
    if (!lod_interact_trace_enabled()) {
        return;
    }
    if (obj == 0 || !lod_interact_trace_can_read(obj, 0, 2)) {
        fprintf(stderr, "[INTERACT_STATE] %s map=0x%08X obj=0x%08X unreadable\n",
                tag, lod_current_map_overlay_rom(), (uint32_t)obj);
        return;
    }
    const uint32_t idx = lod_interact_trace_read_hu(rdram, obj, 0x38, 0xFFFF);
    const uint64_t cfg = (idx < 0x80) ? lod_interact_trace_guest_addr(0x80196778u + idx * 0x14u) : 0;
    const uint32_t type = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x00, 0xFFFF) : 0xFFFF;
    const uint32_t item = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x02, 0xFFFF) : 0xFFFF;
    const uint32_t textbox = lod_interact_trace_read_w(rdram, obj, 0x60, 0);
    uint32_t tb_flags = 0, tb_prev = 0, tb_option = 0, tb_display = 0, tb_window = 0, tb_lens = 0;
    lod_interact_trace_textbox_fields(rdram, textbox, &tb_flags, &tb_prev, &tb_option, &tb_display, &tb_window, &tb_lens);
    const uint64_t cont0 = lod_interact_trace_guest_addr(0x801C87F4u);
    const uint32_t cont_held = lod_interact_trace_read_hu(rdram, cont0, 0x02, 0);
    const uint32_t cont_pressed = lod_interact_trace_read_hu(rdram, cont0, 0x04, 0);
    const int16_t cont_joy_x = lod_interact_trace_read_h(rdram, cont0, 0x06, 0);
    const int16_t cont_joy_y = lod_interact_trace_read_h(rdram, cont0, 0x08, 0);
    fprintf(stderr,
            "[INTERACT_STATE] %s map=0x%08X obj=0x%08X id=0x%04X idx=%u kind=%s type=%u item=0x%04X flag3C=%u func0E=%d textbox60=0x%08X field04=%d field52=%d event50=0x%04X model24=0x%08X tb=(flags=0x%08X prev=%u opt=%u display=%u window=0x%08X lens=0x%08X) input=(held=0x%04X pressed=0x%04X joy=%d,%d)\n",
            tag,
            lod_current_map_overlay_rom(),
            (uint32_t)obj,
            lod_interact_trace_read_hu(rdram, obj, 0x00, 0xFFFF),
            idx,
            lod_interact_trace_kind(type, item),
            type,
            item,
            lod_interact_trace_read_w(rdram, obj, 0x3C, 0xFFFFFFFFu),
            lod_interact_trace_read_h(rdram, obj, 0x0E, 0),
            textbox,
            lod_interact_trace_read_h(rdram, obj, 0x04, 0),
            lod_interact_trace_read_h(rdram, obj, 0x52, 0),
            lod_interact_trace_read_hu(rdram, obj, 0x50, 0xFFFF),
            lod_interact_trace_read_w(rdram, obj, 0x24, 0),
            tb_flags,
            tb_prev,
            tb_option,
            tb_display,
            tb_window,
            tb_lens,
            cont_held,
            cont_pressed,
            cont_joy_x,
            cont_joy_y);
}

static void lod_interact_trace_choice(uint8_t* rdram, const char* tag, uint64_t obj, uint32_t choice_result) {
    if (!lod_interact_trace_enabled()) {
        return;
    }
    if (obj == 0 || !lod_interact_trace_can_read(obj, 0, 2)) {
        fprintf(stderr, "[INTERACT_CHOICE] %s map=0x%08X obj=0x%08X choice=%u unreadable\n",
                tag, lod_current_map_overlay_rom(), (uint32_t)obj, choice_result);
        return;
    }
    const uint32_t idx = lod_interact_trace_read_hu(rdram, obj, 0x38, 0xFFFF);
    const uint64_t cfg = (idx < 0x80) ? lod_interact_trace_guest_addr(0x80196778u + idx * 0x14u) : 0;
    const uint32_t type = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x00, 0xFFFF) : 0xFFFF;
    const uint32_t item = cfg != 0 ? lod_interact_trace_read_hu(rdram, cfg, 0x02, 0xFFFF) : 0xFFFF;
    const uint32_t textbox = lod_interact_trace_read_w(rdram, obj, 0x60, 0);
    uint32_t tb_flags = 0, tb_prev = 0, tb_option = 0, tb_display = 0, tb_window = 0, tb_lens = 0;
    lod_interact_trace_textbox_fields(rdram, textbox, &tb_flags, &tb_prev, &tb_option, &tb_display, &tb_window, &tb_lens);
    const uint64_t cont0 = lod_interact_trace_guest_addr(0x801C87F4u);
    fprintf(stderr,
            "[INTERACT_CHOICE] %s map=0x%08X obj=0x%08X idx=%u kind=%s type=%u item=0x%04X choice=%u field52=%d textbox60=0x%08X tb=(flags=0x%08X prev=%u opt=%u display=%u window=0x%08X lens=0x%08X) input=(held=0x%04X pressed=0x%04X joy=%d,%d)\n",
            tag,
            lod_current_map_overlay_rom(),
            (uint32_t)obj,
            idx,
            lod_interact_trace_kind(type, item),
            type,
            item,
            choice_result,
            lod_interact_trace_read_h(rdram, obj, 0x52, 0),
            textbox,
            tb_flags,
            tb_prev,
            tb_option,
            tb_display,
            tb_window,
            tb_lens,
            lod_interact_trace_read_hu(rdram, cont0, 0x02, 0),
            lod_interact_trace_read_hu(rdram, cont0, 0x04, 0),
            lod_interact_trace_read_h(rdram, cont0, 0x06, 0),
            lod_interact_trace_read_h(rdram, cont0, 0x08, 0));
}
#endif

#if LOD_ENABLE_NI0E_TRACE
#include <stdbool.h>

// Round 4: draw-dependency probe for Interactable_Init (0x80186E60), the
// state-0 handler that Interactable_Entrypoint (0x80186DF0) dispatches to.
// Round 3 found the live dispatch-table word for object id 0x027 in the
// focus maps is 0x80186DF0 (Interactable_Entrypoint), not the ROM's
// 0x0E000000 NI-overlay target; these 22 objects have collision but render
// nothing. Reading Interactable_Init shows it allocates the object's scene
// glow/display node via sceneLookup(kind=0x210, parent) (0x80186F7C) and
// stores the result at obj+0x24 (the "figures[0]" slot per the
// game-engine-architecture memory notes). sceneLookup
// (docs/issue27-31-ni0e-findings.md / Issue #27 notes) silently returns NULL
// on scene-node-pool exhaustion or an invalid parent -- exactly the kind of
// silent failure that would explain "collision but renders nothing". This
// probe logs the outcome once per distinct object address (cap 32); never
// alters control flow or registers.
extern uint32_t lod_current_map_overlay_rom(void);

static inline bool lod_ni0e_interact_addr_ok(uint32_t addr, uint32_t size) {
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return addr != 0 && phys <= 0x800000u && size <= 0x800000u - phys;
}

static inline gpr lod_ni0e_interact_addr_gpr(uint32_t addr) {
    return (gpr)(int32_t)addr;
}

static bool lod_ni0e_interact_focus_map(void) {
    const uint32_t rom = lod_current_map_overlay_rom();
    return rom == 0x007A2D70u || rom == 0x007932D0u || rom == 0x007D4420u ||
           rom == 0x007D3C90u;
}

static uint32_t lod_ni0e_interact_seen[32];
static int lod_ni0e_interact_seen_count = 0;

static bool lod_ni0e_interact_obj_is_new(uint32_t obj) {
    for (int i = 0; i < lod_ni0e_interact_seen_count; i++) {
        if (lod_ni0e_interact_seen[i] == obj) {
            return false;
        }
    }
    if (lod_ni0e_interact_seen_count < 32) {
        lod_ni0e_interact_seen[lod_ni0e_interact_seen_count++] = obj;
        return true;
    }
    return false;
}

static void lod_ni0e_interact_node_probe(uint8_t* rdram, uint32_t obj, uint32_t parent,
                                          uint32_t node) {
    if (!lod_ni0e_interact_focus_map() || !lod_ni0e_interact_addr_ok(obj, 0x74)) {
        return;
    }
    const uint32_t id = (uint32_t)MEM_HU(0x0, lod_ni0e_interact_addr_gpr(obj)) & 0x7FFu;
    if (id != 0x027u || !lod_ni0e_interact_obj_is_new(obj)) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] interact-node obj=0x%08X id=0x%03X kind=0x210 parent=0x%08X "
            "node=0x%08X %s\n",
            obj, id, parent, node,
            node == 0 ? "(sceneLookup FAILED - NULL node)" : "(ok)");
}
#endif  // LOD_ENABLE_NI0E_TRACE

RECOMP_FUNC void func_801855FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801855FC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80185600: bne         $a0, $zero, L_80185610
    if (ctx->r4 != 0) {
        // 0x80185604: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_80185610;
    }
    // 0x80185604: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80185608: b           L_80185728
    // 0x8018560C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80185728;
    // 0x8018560C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80185610:
    // 0x80185610: lhu         $v1, 0x0($a0)
    ctx->r3 = MEM_HU(ctx->r4, 0X0);
    // 0x80185614: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x80185618: andi        $v1, $v1, 0xFF
    ctx->r3 = ctx->r3 & 0XFF;
    // 0x8018561C: andi        $v1, $v1, 0xFFFF
    ctx->r3 = ctx->r3 & 0XFFFF;
    // 0x80185620: beq         $v1, $at, L_80185640
    if (ctx->r3 == ctx->r1) {
        // 0x80185624: addiu       $at, $zero, 0x29
        ctx->r1 = ADD32(0, 0X29);
            goto L_80185640;
    }
    // 0x80185624: addiu       $at, $zero, 0x29
    ctx->r1 = ADD32(0, 0X29);
    // 0x80185628: beq         $v1, $at, L_80185640
    if (ctx->r3 == ctx->r1) {
        // 0x8018562C: addiu       $at, $zero, 0x2A
        ctx->r1 = ADD32(0, 0X2A);
            goto L_80185640;
    }
    // 0x8018562C: addiu       $at, $zero, 0x2A
    ctx->r1 = ADD32(0, 0X2A);
    // 0x80185630: beq         $v1, $at, L_80185640
    if (ctx->r3 == ctx->r1) {
        // 0x80185634: addiu       $at, $zero, 0x2B
        ctx->r1 = ADD32(0, 0X2B);
            goto L_80185640;
    }
    // 0x80185634: addiu       $at, $zero, 0x2B
    ctx->r1 = ADD32(0, 0X2B);
    // 0x80185638: bne         $v1, $at, L_80185648
    if (ctx->r3 != ctx->r1) {
        // 0x8018563C: nop
    
            goto L_80185648;
    }
    // 0x8018563C: nop

L_80185640:
    // 0x80185640: b           L_80185728
    // 0x80185644: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80185728;
    // 0x80185644: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80185648:
    // 0x80185648: beql        $a1, $zero, L_80185728
    if (ctx->r5 == 0) {
        // 0x8018564C: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80185728;
    }
    goto skip_0;
    // 0x8018564C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_0:
    // 0x80185650: beq         $a1, $zero, L_80185680
    if (ctx->r5 == 0) {
        // 0x80185654: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80185680;
    }
    // 0x80185654: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80185658: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x8018565C: jal         0x80001FD0
    // 0x80185660: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    cmdNodeTable_classify(rdram, ctx);
        goto after_0;
    // 0x80185660: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x80185664: beq         $v0, $zero, L_80185674
    if (ctx->r2 == 0) {
        // 0x80185668: lw          $a1, 0x1C($sp)
        ctx->r5 = MEM_W(ctx->r29, 0X1C);
            goto L_80185674;
    }
    // 0x80185668: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x8018566C: b           L_80185680
    // 0x80185670: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80185680;
    // 0x80185670: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80185674:
    // 0x80185674: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
    // 0x80185678: b           L_80185680
    // 0x8018567C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
        goto L_80185680;
    // 0x8018567C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80185680:
    // 0x80185680: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80185684: bnel        $v0, $at, L_80185728
    if (ctx->r2 != ctx->r1) {
        // 0x80185688: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80185728;
    }
    goto skip_1;
    // 0x80185688: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_1:
    // 0x8018568C: lh          $v0, 0x0($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X0);
    // 0x80185690: andi        $v0, $v0, 0x7FF
    ctx->r2 = ctx->r2 & 0X7FF;
    // 0x80185694: slti        $at, $v0, 0x201
    ctx->r1 = SIGNED(ctx->r2) < 0X201 ? 1 : 0;
    // 0x80185698: bne         $at, $zero, L_801856C0
    if (ctx->r1 != 0) {
        // 0x8018569C: addiu       $at, $zero, 0x207
        ctx->r1 = ADD32(0, 0X207);
            goto L_801856C0;
    }
    // 0x8018569C: addiu       $at, $zero, 0x207
    ctx->r1 = ADD32(0, 0X207);
    // 0x801856A0: beq         $v0, $at, L_8018571C
    if (ctx->r2 == ctx->r1) {
        // 0x801856A4: addiu       $at, $zero, 0x239
        ctx->r1 = ADD32(0, 0X239);
            goto L_8018571C;
    }
    // 0x801856A4: addiu       $at, $zero, 0x239
    ctx->r1 = ADD32(0, 0X239);
    // 0x801856A8: beq         $v0, $at, L_8018571C
    if (ctx->r2 == ctx->r1) {
        // 0x801856AC: addiu       $at, $zero, 0x277
        ctx->r1 = ADD32(0, 0X277);
            goto L_8018571C;
    }
    // 0x801856AC: addiu       $at, $zero, 0x277
    ctx->r1 = ADD32(0, 0X277);
    // 0x801856B0: beq         $v0, $at, L_8018571C
    if (ctx->r2 == ctx->r1) {
        // 0x801856B4: nop
    
            goto L_8018571C;
    }
    // 0x801856B4: nop

    // 0x801856B8: b           L_80185728
    // 0x801856BC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80185728;
    // 0x801856BC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801856C0:
    // 0x801856C0: slti        $at, $v0, 0x1FF
    ctx->r1 = SIGNED(ctx->r2) < 0X1FF ? 1 : 0;
    // 0x801856C4: bne         $at, $zero, L_801856DC
    if (ctx->r1 != 0) {
        // 0x801856C8: addiu       $at, $zero, 0x200
        ctx->r1 = ADD32(0, 0X200);
            goto L_801856DC;
    }
    // 0x801856C8: addiu       $at, $zero, 0x200
    ctx->r1 = ADD32(0, 0X200);
    // 0x801856CC: beq         $v0, $at, L_8018571C
    if (ctx->r2 == ctx->r1) {
        // 0x801856D0: nop
    
            goto L_8018571C;
    }
    // 0x801856D0: nop

    // 0x801856D4: b           L_80185728
    // 0x801856D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80185728;
    // 0x801856D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801856DC:
    // 0x801856DC: slti        $at, $v0, 0x1CA
    ctx->r1 = SIGNED(ctx->r2) < 0X1CA ? 1 : 0;
    // 0x801856E0: bne         $at, $zero, L_801856FC
    if (ctx->r1 != 0) {
        // 0x801856E4: addiu       $t6, $v0, -0x1BC
        ctx->r14 = ADD32(ctx->r2, -0X1BC);
            goto L_801856FC;
    }
    // 0x801856E4: addiu       $t6, $v0, -0x1BC
    ctx->r14 = ADD32(ctx->r2, -0X1BC);
    // 0x801856E8: addiu       $at, $zero, 0x1FE
    ctx->r1 = ADD32(0, 0X1FE);
    // 0x801856EC: beq         $v0, $at, L_8018571C
    if (ctx->r2 == ctx->r1) {
        // 0x801856F0: nop
    
            goto L_8018571C;
    }
    // 0x801856F0: nop

    // 0x801856F4: b           L_80185728
    // 0x801856F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80185728;
    // 0x801856F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801856FC:
    // 0x801856FC: sltiu       $at, $t6, 0xE
    ctx->r1 = ctx->r14 < 0XE ? 1 : 0;
    // 0x80185700: beq         $at, $zero, L_80185724
    if (ctx->r1 == 0) {
        // 0x80185704: sll         $t6, $t6, 2
        ctx->r14 = S32(ctx->r14 << 2);
            goto L_80185724;
    }
    // 0x80185704: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80185708: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018570C: addu        $at, $at, $t6
    gpr jr_addend_80185714 = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x80185710: lw          $t6, -0x312C($at)
    ctx->r14 = ADD32(ctx->r1, -0X312C);
    // 0x80185714: jr          $t6
    // 0x80185718: nop

    switch (jr_addend_80185714 >> 2) {
        case 0: goto L_8018571C; break;
        case 1: goto L_80185724; break;
        case 2: goto L_8018571C; break;
        case 3: goto L_8018571C; break;
        case 4: goto L_80185724; break;
        case 5: goto L_80185724; break;
        case 6: goto L_8018571C; break;
        case 7: goto L_80185724; break;
        case 8: goto L_8018571C; break;
        case 9: goto L_8018571C; break;
        case 10: goto L_80185724; break;
        case 11: goto L_8018571C; break;
        case 12: goto L_80185724; break;
        case 13: goto L_8018571C; break;
        default: switch_error(__func__, 0x80185714, 0x8019CED4);
    }
    // 0x80185718: nop

L_8018571C:
    // 0x8018571C: b           L_80185728
    // 0x80185720: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80185728;
    // 0x80185720: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80185724:
    // 0x80185724: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80185728:
    // 0x80185728: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8018572C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80185730: jr          $ra
    // 0x80185734: nop

    return;
    // 0x80185734: nop

;}
RECOMP_FUNC void func_80185738(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80185738: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8018573C: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80185740: lbu         $t6, 0x56F($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X56F);
    // 0x80185744: or          $t7, $t6, $a1
    ctx->r15 = ctx->r14 | ctx->r5;
    // 0x80185748: jr          $ra
    // 0x8018574C: sb          $t7, 0x56F($v0)
    MEM_B(0X56F, ctx->r2) = ctx->r15;
    return;
    // 0x8018574C: sb          $t7, 0x56F($v0)
    MEM_B(0X56F, ctx->r2) = ctx->r15;
;}
RECOMP_FUNC void func_80185750(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80185750: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80185754: jr          $ra
    // 0x80185758: sb          $zero, 0x56F($v0)
    MEM_B(0X56F, ctx->r2) = 0;
    return;
    // 0x80185758: sb          $zero, 0x56F($v0)
    MEM_B(0X56F, ctx->r2) = 0;
;}
RECOMP_FUNC void func_8018575C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018575C: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80185760: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80185764: andi        $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 & 0XFF;
    // 0x80185768: lbu         $t6, 0x56F($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X56F);
    // 0x8018576C: and         $v0, $t6, $a1
    ctx->r2 = ctx->r14 & ctx->r5;
    // 0x80185770: jr          $ra
    // 0x80185774: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
    return;
    // 0x80185774: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
;}
RECOMP_FUNC void func_80185778(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80185778: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8018577C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80185780: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80185784: swc1        $f0, 0x6554($at)
    MEM_W(0X6554, ctx->r1) = ctx->f0.u32l;
    // 0x80185788: swc1        $f0, 0x6550($at)
    MEM_W(0X6550, ctx->r1) = ctx->f0.u32l;
    // 0x8018578C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80185790: swc1        $f0, 0x654C($at)
    MEM_W(0X654C, ctx->r1) = ctx->f0.u32l;
    // 0x80185794: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80185798: swc1        $f0, 0x6560($at)
    MEM_W(0X6560, ctx->r1) = ctx->f0.u32l;
    // 0x8018579C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801857A0: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x801857A4: addiu       $v0, $v0, 0x65C4
    ctx->r2 = ADD32(ctx->r2, 0X65C4);
    // 0x801857A8: addiu       $v1, $v1, 0x6564
    ctx->r3 = ADD32(ctx->r3, 0X6564);
    // 0x801857AC: swc1        $f0, 0x655C($at)
    MEM_W(0X655C, ctx->r1) = ctx->f0.u32l;
    // 0x801857B0: swc1        $f0, 0x6558($at)
    MEM_W(0X6558, ctx->r1) = ctx->f0.u32l;
L_801857B4:
    // 0x801857B4: addiu       $v1, $v1, 0x30
    ctx->r3 = ADD32(ctx->r3, 0X30);
    // 0x801857B8: swc1        $f0, -0x1C($v1)
    MEM_W(-0X1C, ctx->r3) = ctx->f0.u32l;
    // 0x801857BC: swc1        $f0, -0x20($v1)
    MEM_W(-0X20, ctx->r3) = ctx->f0.u32l;
    // 0x801857C0: swc1        $f0, -0x24($v1)
    MEM_W(-0X24, ctx->r3) = ctx->f0.u32l;
    // 0x801857C4: swc1        $f0, -0x10($v1)
    MEM_W(-0X10, ctx->r3) = ctx->f0.u32l;
    // 0x801857C8: swc1        $f0, -0x14($v1)
    MEM_W(-0X14, ctx->r3) = ctx->f0.u32l;
    // 0x801857CC: swc1        $f0, -0x18($v1)
    MEM_W(-0X18, ctx->r3) = ctx->f0.u32l;
    // 0x801857D0: swc1        $f0, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f0.u32l;
    // 0x801857D4: swc1        $f0, -0x8($v1)
    MEM_W(-0X8, ctx->r3) = ctx->f0.u32l;
    // 0x801857D8: swc1        $f0, -0xC($v1)
    MEM_W(-0XC, ctx->r3) = ctx->f0.u32l;
    // 0x801857DC: swc1        $f0, -0x28($v1)
    MEM_W(-0X28, ctx->r3) = ctx->f0.u32l;
    // 0x801857E0: swc1        $f0, -0x2C($v1)
    MEM_W(-0X2C, ctx->r3) = ctx->f0.u32l;
    // 0x801857E4: bne         $v1, $v0, L_801857B4
    if (ctx->r3 != ctx->r2) {
        // 0x801857E8: swc1        $f0, -0x30($v1)
        MEM_W(-0X30, ctx->r3) = ctx->f0.u32l;
            goto L_801857B4;
    }
    // 0x801857E8: swc1        $f0, -0x30($v1)
    MEM_W(-0X30, ctx->r3) = ctx->f0.u32l;
    // 0x801857EC: jr          $ra
    // 0x801857F0: nop

    return;
    // 0x801857F0: nop

;}
RECOMP_FUNC void func_80185820(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80185820: addiu       $sp, $sp, -0xE80
    ctx->r29 = ADD32(ctx->r29, -0XE80);
    // 0x80185824: sw          $a2, 0xE88($sp)
    MEM_W(0XE88, ctx->r29) = ctx->r6;
    // 0x80185828: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8018582C: sw          $ra, 0x274($sp)
    MEM_W(0X274, ctx->r29) = ctx->r31;
    // 0x80185830: sw          $s0, 0x270($sp)
    MEM_W(0X270, ctx->r29) = ctx->r16;
    // 0x80185834: sw          $a0, 0xE80($sp)
    MEM_W(0XE80, ctx->r29) = ctx->r4;
    // 0x80185838: beq         $a0, $zero, L_80185DF0
    if (ctx->r4 == 0) {
        // 0x8018583C: sw          $a3, 0xE8C($sp)
        MEM_W(0XE8C, ctx->r29) = ctx->r7;
            goto L_80185DF0;
    }
    // 0x8018583C: sw          $a3, 0xE8C($sp)
    MEM_W(0XE8C, ctx->r29) = ctx->r7;
    // 0x80185840: beq         $a1, $zero, L_80185DF0
    if (ctx->r5 == 0) {
        // 0x80185844: lw          $t6, 0xEA0($sp)
        ctx->r14 = MEM_W(ctx->r29, 0XEA0);
            goto L_80185DF0;
    }
    // 0x80185844: lw          $t6, 0xEA0($sp)
    ctx->r14 = MEM_W(ctx->r29, 0XEA0);
    // 0x80185848: beq         $t6, $zero, L_80185DF0
    if (ctx->r14 == 0) {
        // 0x8018584C: addiu       $a0, $sp, 0x2BC
        ctx->r4 = ADD32(ctx->r29, 0X2BC);
            goto L_80185DF0;
    }
    // 0x8018584C: addiu       $a0, $sp, 0x2BC
    ctx->r4 = ADD32(ctx->r29, 0X2BC);
    // 0x80185850: sw          $a1, 0xE84($sp)
    MEM_W(0XE84, ctx->r29) = ctx->r5;
    // 0x80185854: jal         0x80013CC4
    // 0x80185858: sw          $a2, 0xE80($sp)
    MEM_W(0XE80, ctx->r29) = ctx->r6;
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x80185858: sw          $a2, 0xE80($sp)
    MEM_W(0XE80, ctx->r29) = ctx->r6;
    after_0:
    // 0x8018585C: lwc1        $f4, 0x2C0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X2C0);
    // 0x80185860: lwc1        $f6, 0xE90($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XE90);
    // 0x80185864: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80185868: addiu       $a1, $a1, -0x3068
    ctx->r5 = ADD32(ctx->r5, -0X3068);
    // 0x8018586C: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80185870: addiu       $a0, $sp, 0x298
    ctx->r4 = ADD32(ctx->r29, 0X298);
    // 0x80185874: addiu       $a2, $sp, 0x2BC
    ctx->r6 = ADD32(ctx->r29, 0X2BC);
    // 0x80185878: jal         0x80013E6C
    // 0x8018587C: swc1        $f8, 0x2C0($sp)
    MEM_W(0X2C0, ctx->r29) = ctx->f8.u32l;
    vec3f_crossProduct(rdram, ctx);
        goto after_1;
    // 0x8018587C: swc1        $f8, 0x2C0($sp)
    MEM_W(0X2C0, ctx->r29) = ctx->f8.u32l;
    after_1:
    // 0x80185880: addiu       $a0, $sp, 0x280
    ctx->r4 = ADD32(ctx->r29, 0X280);
    // 0x80185884: addiu       $a1, $sp, 0x298
    ctx->r5 = ADD32(ctx->r29, 0X298);
    // 0x80185888: jal         0x80013C10
    // 0x8018588C: lw          $a2, 0xE88($sp)
    ctx->r6 = MEM_W(ctx->r29, 0XE88);
    func_80013C10(rdram, ctx);
        goto after_2;
    // 0x8018588C: lw          $a2, 0xE88($sp)
    ctx->r6 = MEM_W(ctx->r29, 0XE88);
    after_2:
    // 0x80185890: addiu       $s0, $sp, 0x778
    ctx->r16 = ADD32(ctx->r29, 0X778);
    // 0x80185894: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x80185898: lw          $a0, 0xE80($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XE80);
    // 0x8018589C: addiu       $a1, $sp, 0x2BC
    ctx->r5 = ADD32(ctx->r29, 0X2BC);
    // 0x801858A0: jal         0x8003E2F0
    // 0x801858A4: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    func_8003E2F0(rdram, ctx);
        goto after_3;
    // 0x801858A4: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    after_3:
    // 0x801858A8: or          $t9, $s0, $zero
    ctx->r25 = ctx->r16 | 0;
    // 0x801858AC: or          $t0, $sp, $zero
    ctx->r8 = ctx->r29 | 0;
    // 0x801858B0: addiu       $t1, $s0, 0x258
    ctx->r9 = ADD32(ctx->r16, 0X258);
L_801858B4:
    // 0x801858B4: lw          $t8, 0x0($t9)
    ctx->r24 = MEM_W(ctx->r25, 0X0);
    // 0x801858B8: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x801858BC: addiu       $t0, $t0, 0xC
    ctx->r8 = ADD32(ctx->r8, 0XC);
    // 0x801858C0: sw          $t8, -0xC($t0)
    MEM_W(-0XC, ctx->r8) = ctx->r24;
    // 0x801858C4: lw          $t7, -0x8($t9)
    ctx->r15 = MEM_W(ctx->r25, -0X8);
    // 0x801858C8: sw          $t7, -0x8($t0)
    MEM_W(-0X8, ctx->r8) = ctx->r15;
    // 0x801858CC: lw          $t8, -0x4($t9)
    ctx->r24 = MEM_W(ctx->r25, -0X4);
    // 0x801858D0: bne         $t9, $t1, L_801858B4
    if (ctx->r25 != ctx->r9) {
        // 0x801858D4: sw          $t8, -0x4($t0)
        MEM_W(-0X4, ctx->r8) = ctx->r24;
            goto L_801858B4;
    }
    // 0x801858D4: sw          $t8, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->r24;
    // 0x801858D8: lwc1        $f10, 0xE9C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XE9C);
    // 0x801858DC: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x801858E0: addiu       $t3, $t3, 0x6648
    ctx->r11 = ADD32(ctx->r11, 0X6648);
    // 0x801858E4: addiu       $t2, $sp, 0x2BC
    ctx->r10 = ADD32(ctx->r29, 0X2BC);
    // 0x801858E8: sw          $t2, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r10;
    // 0x801858EC: sw          $t3, 0x260($sp)
    MEM_W(0X260, ctx->r29) = ctx->r11;
    // 0x801858F0: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x801858F4: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x801858F8: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x801858FC: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    // 0x80185900: jal         0x80185E04
    // 0x80185904: swc1        $f10, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f10.u32l;
    func_80185E04(rdram, ctx);
        goto after_4;
    // 0x80185904: swc1        $f10, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f10.u32l;
    after_4:
    // 0x80185908: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x8018590C: lw          $a1, 0xE80($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE80);
    // 0x80185910: jal         0x80013C90
    // 0x80185914: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    func_80013C90(rdram, ctx);
        goto after_5;
    // 0x80185914: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    after_5:
    // 0x80185918: lwc1        $f16, 0x2B4($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X2B4);
    // 0x8018591C: lwc1        $f18, 0xE8C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XE8C);
    // 0x80185920: addiu       $s0, $sp, 0x2A4
    ctx->r16 = ADD32(ctx->r29, 0X2A4);
    // 0x80185924: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185928: sub.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8018592C: addiu       $a1, $sp, 0x298
    ctx->r5 = ADD32(ctx->r29, 0X298);
    // 0x80185930: lw          $a2, 0xE94($sp)
    ctx->r6 = MEM_W(ctx->r29, 0XE94);
    // 0x80185934: jal         0x80013C10
    // 0x80185938: swc1        $f4, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f4.u32l;
    func_80013C10(rdram, ctx);
        goto after_6;
    // 0x80185938: swc1        $f4, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f4.u32l;
    after_6:
    // 0x8018593C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185940: lw          $a1, 0xE84($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE84);
    // 0x80185944: jal         0x80013C90
    // 0x80185948: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_80013C90(rdram, ctx);
        goto after_7;
    // 0x80185948: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_7:
    // 0x8018594C: lwc1        $f6, 0xE90($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XE90);
    // 0x80185950: lwc1        $f8, 0xE98($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XE98);
    // 0x80185954: lwc1        $f10, 0x2A8($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X2A8);
    // 0x80185958: addiu       $a0, $sp, 0x28C
    ctx->r4 = ADD32(ctx->r29, 0X28C);
    // 0x8018595C: sub.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80185960: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80185964: addiu       $a2, $sp, 0x2B0
    ctx->r6 = ADD32(ctx->r29, 0X2B0);
    // 0x80185968: add.s       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f0.fl;
    // 0x8018596C: swc1        $f0, 0x27C($sp)
    MEM_W(0X27C, ctx->r29) = ctx->f0.u32l;
    // 0x80185970: jal         0x80013CC4
    // 0x80185974: swc1        $f16, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f16.u32l;
    func_80013CC4(rdram, ctx);
        goto after_8;
    // 0x80185974: swc1        $f16, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f16.u32l;
    after_8:
    // 0x80185978: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x8018597C: addiu       $a1, $sp, 0x28C
    ctx->r5 = ADD32(ctx->r29, 0X28C);
    // 0x80185980: addiu       $a2, $sp, 0xC28
    ctx->r6 = ADD32(ctx->r29, 0XC28);
    // 0x80185984: jal         0x8003E2F0
    // 0x80185988: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    func_8003E2F0(rdram, ctx);
        goto after_9;
    // 0x80185988: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    after_9:
    // 0x8018598C: addiu       $t4, $sp, 0xC28
    ctx->r12 = ADD32(ctx->r29, 0XC28);
    // 0x80185990: addiu       $t9, $t4, 0x258
    ctx->r25 = ADD32(ctx->r12, 0X258);
    // 0x80185994: or          $t1, $sp, $zero
    ctx->r9 = ctx->r29 | 0;
L_80185998:
    // 0x80185998: lw          $t6, 0x0($t4)
    ctx->r14 = MEM_W(ctx->r12, 0X0);
    // 0x8018599C: addiu       $t4, $t4, 0xC
    ctx->r12 = ADD32(ctx->r12, 0XC);
    // 0x801859A0: addiu       $t1, $t1, 0xC
    ctx->r9 = ADD32(ctx->r9, 0XC);
    // 0x801859A4: sw          $t6, -0xC($t1)
    MEM_W(-0XC, ctx->r9) = ctx->r14;
    // 0x801859A8: lw          $t5, -0x8($t4)
    ctx->r13 = MEM_W(ctx->r12, -0X8);
    // 0x801859AC: sw          $t5, -0x8($t1)
    MEM_W(-0X8, ctx->r9) = ctx->r13;
    // 0x801859B0: lw          $t6, -0x4($t4)
    ctx->r14 = MEM_W(ctx->r12, -0X4);
    // 0x801859B4: bne         $t4, $t9, L_80185998
    if (ctx->r12 != ctx->r25) {
        // 0x801859B8: sw          $t6, -0x4($t1)
        MEM_W(-0X4, ctx->r9) = ctx->r14;
            goto L_80185998;
    }
    // 0x801859B8: sw          $t6, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->r14;
    // 0x801859BC: lwc1        $f18, 0xE9C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XE9C);
    // 0x801859C0: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x801859C4: addiu       $t7, $t7, 0x65E0
    ctx->r15 = ADD32(ctx->r15, 0X65E0);
    // 0x801859C8: addiu       $t0, $sp, 0x2BC
    ctx->r8 = ADD32(ctx->r29, 0X2BC);
    // 0x801859CC: sw          $t0, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r8;
    // 0x801859D0: sw          $t7, 0x260($sp)
    MEM_W(0X260, ctx->r29) = ctx->r15;
    // 0x801859D4: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x801859D8: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x801859DC: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x801859E0: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    // 0x801859E4: jal         0x80185E04
    // 0x801859E8: swc1        $f18, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f18.u32l;
    func_80185E04(rdram, ctx);
        goto after_10;
    // 0x801859E8: swc1        $f18, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f18.u32l;
    after_10:
    // 0x801859EC: addiu       $t8, $sp, 0xC28
    ctx->r24 = ADD32(ctx->r29, 0XC28);
    // 0x801859F0: addiu       $t4, $t8, 0x258
    ctx->r12 = ADD32(ctx->r24, 0X258);
    // 0x801859F4: or          $t9, $sp, $zero
    ctx->r25 = ctx->r29 | 0;
L_801859F8:
    // 0x801859F8: lw          $t3, 0x0($t8)
    ctx->r11 = MEM_W(ctx->r24, 0X0);
    // 0x801859FC: addiu       $t8, $t8, 0xC
    ctx->r24 = ADD32(ctx->r24, 0XC);
    // 0x80185A00: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x80185A04: sw          $t3, -0xC($t9)
    MEM_W(-0XC, ctx->r25) = ctx->r11;
    // 0x80185A08: lw          $t2, -0x8($t8)
    ctx->r10 = MEM_W(ctx->r24, -0X8);
    // 0x80185A0C: sw          $t2, -0x8($t9)
    MEM_W(-0X8, ctx->r25) = ctx->r10;
    // 0x80185A10: lw          $t3, -0x4($t8)
    ctx->r11 = MEM_W(ctx->r24, -0X4);
    // 0x80185A14: bne         $t8, $t4, L_801859F8
    if (ctx->r24 != ctx->r12) {
        // 0x80185A18: sw          $t3, -0x4($t9)
        MEM_W(-0X4, ctx->r25) = ctx->r11;
            goto L_801859F8;
    }
    // 0x80185A18: sw          $t3, -0x4($t9)
    MEM_W(-0X4, ctx->r25) = ctx->r11;
    // 0x80185A1C: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80185A20: addiu       $t1, $t1, 0x65E0
    ctx->r9 = ADD32(ctx->r9, 0X65E0);
    // 0x80185A24: sw          $t1, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r9;
    // 0x80185A28: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x80185A2C: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80185A30: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x80185A34: jal         0x80185EF8
    // 0x80185A38: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    func_80185EF8(rdram, ctx);
        goto after_11;
    // 0x80185A38: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    after_11:
    // 0x80185A3C: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x80185A40: lw          $a1, 0xE80($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE80);
    // 0x80185A44: jal         0x80013CC4
    // 0x80185A48: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    func_80013CC4(rdram, ctx);
        goto after_12;
    // 0x80185A48: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    after_12:
    // 0x80185A4C: lwc1        $f4, 0x2B4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X2B4);
    // 0x80185A50: lwc1        $f6, 0xE8C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XE8C);
    // 0x80185A54: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185A58: addiu       $a1, $sp, 0x298
    ctx->r5 = ADD32(ctx->r29, 0X298);
    // 0x80185A5C: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80185A60: lw          $a2, 0xE94($sp)
    ctx->r6 = MEM_W(ctx->r29, 0XE94);
    // 0x80185A64: jal         0x80013C10
    // 0x80185A68: swc1        $f8, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f8.u32l;
    func_80013C10(rdram, ctx);
        goto after_13;
    // 0x80185A68: swc1        $f8, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f8.u32l;
    after_13:
    // 0x80185A6C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185A70: lw          $a1, 0xE84($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE84);
    // 0x80185A74: jal         0x80013CC4
    // 0x80185A78: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_80013CC4(rdram, ctx);
        goto after_14;
    // 0x80185A78: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_14:
    // 0x80185A7C: lwc1        $f10, 0x2A8($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X2A8);
    // 0x80185A80: lwc1        $f16, 0x27C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X27C);
    // 0x80185A84: addiu       $a0, $sp, 0x28C
    ctx->r4 = ADD32(ctx->r29, 0X28C);
    // 0x80185A88: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80185A8C: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x80185A90: addiu       $a2, $sp, 0x2B0
    ctx->r6 = ADD32(ctx->r29, 0X2B0);
    // 0x80185A94: jal         0x80013CC4
    // 0x80185A98: swc1        $f18, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f18.u32l;
    func_80013CC4(rdram, ctx);
        goto after_15;
    // 0x80185A98: swc1        $f18, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f18.u32l;
    after_15:
    // 0x80185A9C: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x80185AA0: addiu       $a1, $sp, 0x28C
    ctx->r5 = ADD32(ctx->r29, 0X28C);
    // 0x80185AA4: addiu       $a2, $sp, 0x9D0
    ctx->r6 = ADD32(ctx->r29, 0X9D0);
    // 0x80185AA8: jal         0x8003E2F0
    // 0x80185AAC: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    func_8003E2F0(rdram, ctx);
        goto after_16;
    // 0x80185AAC: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    after_16:
    // 0x80185AB0: addiu       $t5, $sp, 0x9D0
    ctx->r13 = ADD32(ctx->r29, 0X9D0);
    // 0x80185AB4: addiu       $t4, $t5, 0x258
    ctx->r12 = ADD32(ctx->r13, 0X258);
    // 0x80185AB8: or          $t7, $sp, $zero
    ctx->r15 = ctx->r29 | 0;
L_80185ABC:
    // 0x80185ABC: lw          $t0, 0x0($t5)
    ctx->r8 = MEM_W(ctx->r13, 0X0);
    // 0x80185AC0: addiu       $t5, $t5, 0xC
    ctx->r13 = ADD32(ctx->r13, 0XC);
    // 0x80185AC4: addiu       $t7, $t7, 0xC
    ctx->r15 = ADD32(ctx->r15, 0XC);
    // 0x80185AC8: sw          $t0, -0xC($t7)
    MEM_W(-0XC, ctx->r15) = ctx->r8;
    // 0x80185ACC: lw          $t6, -0x8($t5)
    ctx->r14 = MEM_W(ctx->r13, -0X8);
    // 0x80185AD0: sw          $t6, -0x8($t7)
    MEM_W(-0X8, ctx->r15) = ctx->r14;
    // 0x80185AD4: lw          $t0, -0x4($t5)
    ctx->r8 = MEM_W(ctx->r13, -0X4);
    // 0x80185AD8: bne         $t5, $t4, L_80185ABC
    if (ctx->r13 != ctx->r12) {
        // 0x80185ADC: sw          $t0, -0x4($t7)
        MEM_W(-0X4, ctx->r15) = ctx->r8;
            goto L_80185ABC;
    }
    // 0x80185ADC: sw          $t0, -0x4($t7)
    MEM_W(-0X4, ctx->r15) = ctx->r8;
    // 0x80185AE0: lwc1        $f4, 0xE9C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XE9C);
    // 0x80185AE4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80185AE8: addiu       $t9, $t9, 0x6614
    ctx->r25 = ADD32(ctx->r25, 0X6614);
    // 0x80185AEC: addiu       $t8, $sp, 0x2BC
    ctx->r24 = ADD32(ctx->r29, 0X2BC);
    // 0x80185AF0: sw          $t8, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r24;
    // 0x80185AF4: sw          $t9, 0x260($sp)
    MEM_W(0X260, ctx->r29) = ctx->r25;
    // 0x80185AF8: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x80185AFC: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80185B00: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x80185B04: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    // 0x80185B08: jal         0x80185E04
    // 0x80185B0C: swc1        $f4, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f4.u32l;
    func_80185E04(rdram, ctx);
        goto after_17;
    // 0x80185B0C: swc1        $f4, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f4.u32l;
    after_17:
    // 0x80185B10: addiu       $t2, $sp, 0x9D0
    ctx->r10 = ADD32(ctx->r29, 0X9D0);
    // 0x80185B14: addiu       $t5, $t2, 0x258
    ctx->r13 = ADD32(ctx->r10, 0X258);
    // 0x80185B18: or          $t4, $sp, $zero
    ctx->r12 = ctx->r29 | 0;
L_80185B1C:
    // 0x80185B1C: lw          $t1, 0x0($t2)
    ctx->r9 = MEM_W(ctx->r10, 0X0);
    // 0x80185B20: addiu       $t2, $t2, 0xC
    ctx->r10 = ADD32(ctx->r10, 0XC);
    // 0x80185B24: addiu       $t4, $t4, 0xC
    ctx->r12 = ADD32(ctx->r12, 0XC);
    // 0x80185B28: sw          $t1, -0xC($t4)
    MEM_W(-0XC, ctx->r12) = ctx->r9;
    // 0x80185B2C: lw          $t3, -0x8($t2)
    ctx->r11 = MEM_W(ctx->r10, -0X8);
    // 0x80185B30: sw          $t3, -0x8($t4)
    MEM_W(-0X8, ctx->r12) = ctx->r11;
    // 0x80185B34: lw          $t1, -0x4($t2)
    ctx->r9 = MEM_W(ctx->r10, -0X4);
    // 0x80185B38: bne         $t2, $t5, L_80185B1C
    if (ctx->r10 != ctx->r13) {
        // 0x80185B3C: sw          $t1, -0x4($t4)
        MEM_W(-0X4, ctx->r12) = ctx->r9;
            goto L_80185B1C;
    }
    // 0x80185B3C: sw          $t1, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->r9;
    // 0x80185B40: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80185B44: addiu       $t7, $t7, 0x6614
    ctx->r15 = ADD32(ctx->r15, 0X6614);
    // 0x80185B48: sw          $t7, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r15;
    // 0x80185B4C: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x80185B50: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80185B54: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x80185B58: jal         0x80185EF8
    // 0x80185B5C: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    func_80185EF8(rdram, ctx);
        goto after_18;
    // 0x80185B5C: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    after_18:
    // 0x80185B60: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x80185B64: lw          $a1, 0xE80($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE80);
    // 0x80185B68: jal         0x80013C90
    // 0x80185B6C: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    func_80013C90(rdram, ctx);
        goto after_19;
    // 0x80185B6C: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    after_19:
    // 0x80185B70: lwc1        $f6, 0x2B4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2B4);
    // 0x80185B74: lwc1        $f8, 0xE8C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XE8C);
    // 0x80185B78: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185B7C: addiu       $a1, $sp, 0x298
    ctx->r5 = ADD32(ctx->r29, 0X298);
    // 0x80185B80: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80185B84: lw          $a2, 0xE94($sp)
    ctx->r6 = MEM_W(ctx->r29, 0XE94);
    // 0x80185B88: jal         0x80013C10
    // 0x80185B8C: swc1        $f10, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f10.u32l;
    func_80013C10(rdram, ctx);
        goto after_20;
    // 0x80185B8C: swc1        $f10, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f10.u32l;
    after_20:
    // 0x80185B90: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185B94: lw          $a1, 0xE84($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE84);
    // 0x80185B98: jal         0x80013C90
    // 0x80185B9C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_80013C90(rdram, ctx);
        goto after_21;
    // 0x80185B9C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_21:
    // 0x80185BA0: lwc1        $f16, 0xE90($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0XE90);
    // 0x80185BA4: lwc1        $f18, 0xE98($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XE98);
    // 0x80185BA8: lwc1        $f4, 0x2A8($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X2A8);
    // 0x80185BAC: addiu       $a0, $sp, 0x28C
    ctx->r4 = ADD32(ctx->r29, 0X28C);
    // 0x80185BB0: add.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x80185BB4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80185BB8: addiu       $a2, $sp, 0x2B0
    ctx->r6 = ADD32(ctx->r29, 0X2B0);
    // 0x80185BBC: add.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f0.fl;
    // 0x80185BC0: swc1        $f0, 0x27C($sp)
    MEM_W(0X27C, ctx->r29) = ctx->f0.u32l;
    // 0x80185BC4: jal         0x80013CC4
    // 0x80185BC8: swc1        $f6, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f6.u32l;
    func_80013CC4(rdram, ctx);
        goto after_22;
    // 0x80185BC8: swc1        $f6, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f6.u32l;
    after_22:
    // 0x80185BCC: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x80185BD0: addiu       $a1, $sp, 0x28C
    ctx->r5 = ADD32(ctx->r29, 0X28C);
    // 0x80185BD4: addiu       $a2, $sp, 0x520
    ctx->r6 = ADD32(ctx->r29, 0X520);
    // 0x80185BD8: jal         0x8003E2F0
    // 0x80185BDC: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    func_8003E2F0(rdram, ctx);
        goto after_23;
    // 0x80185BDC: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    after_23:
    // 0x80185BE0: addiu       $t6, $sp, 0x520
    ctx->r14 = ADD32(ctx->r29, 0X520);
    // 0x80185BE4: addiu       $t5, $t6, 0x258
    ctx->r13 = ADD32(ctx->r14, 0X258);
    // 0x80185BE8: or          $t9, $sp, $zero
    ctx->r25 = ctx->r29 | 0;
L_80185BEC:
    // 0x80185BEC: lw          $t8, 0x0($t6)
    ctx->r24 = MEM_W(ctx->r14, 0X0);
    // 0x80185BF0: addiu       $t6, $t6, 0xC
    ctx->r14 = ADD32(ctx->r14, 0XC);
    // 0x80185BF4: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x80185BF8: sw          $t8, -0xC($t9)
    MEM_W(-0XC, ctx->r25) = ctx->r24;
    // 0x80185BFC: lw          $t0, -0x8($t6)
    ctx->r8 = MEM_W(ctx->r14, -0X8);
    // 0x80185C00: sw          $t0, -0x8($t9)
    MEM_W(-0X8, ctx->r25) = ctx->r8;
    // 0x80185C04: lw          $t8, -0x4($t6)
    ctx->r24 = MEM_W(ctx->r14, -0X4);
    // 0x80185C08: bne         $t6, $t5, L_80185BEC
    if (ctx->r14 != ctx->r13) {
        // 0x80185C0C: sw          $t8, -0x4($t9)
        MEM_W(-0X4, ctx->r25) = ctx->r24;
            goto L_80185BEC;
    }
    // 0x80185C0C: sw          $t8, -0x4($t9)
    MEM_W(-0X4, ctx->r25) = ctx->r24;
    // 0x80185C10: lwc1        $f8, 0xE9C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XE9C);
    // 0x80185C14: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80185C18: addiu       $t4, $t4, 0x667C
    ctx->r12 = ADD32(ctx->r12, 0X667C);
    // 0x80185C1C: addiu       $t2, $sp, 0x2BC
    ctx->r10 = ADD32(ctx->r29, 0X2BC);
    // 0x80185C20: sw          $t2, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r10;
    // 0x80185C24: sw          $t4, 0x260($sp)
    MEM_W(0X260, ctx->r29) = ctx->r12;
    // 0x80185C28: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x80185C2C: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80185C30: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x80185C34: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    // 0x80185C38: jal         0x80185E04
    // 0x80185C3C: swc1        $f8, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f8.u32l;
    func_80185E04(rdram, ctx);
        goto after_24;
    // 0x80185C3C: swc1        $f8, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f8.u32l;
    after_24:
    // 0x80185C40: addiu       $t3, $sp, 0x520
    ctx->r11 = ADD32(ctx->r29, 0X520);
    // 0x80185C44: addiu       $t6, $t3, 0x258
    ctx->r14 = ADD32(ctx->r11, 0X258);
    // 0x80185C48: or          $t5, $sp, $zero
    ctx->r13 = ctx->r29 | 0;
L_80185C4C:
    // 0x80185C4C: lw          $t7, 0x0($t3)
    ctx->r15 = MEM_W(ctx->r11, 0X0);
    // 0x80185C50: addiu       $t3, $t3, 0xC
    ctx->r11 = ADD32(ctx->r11, 0XC);
    // 0x80185C54: addiu       $t5, $t5, 0xC
    ctx->r13 = ADD32(ctx->r13, 0XC);
    // 0x80185C58: sw          $t7, -0xC($t5)
    MEM_W(-0XC, ctx->r13) = ctx->r15;
    // 0x80185C5C: lw          $t1, -0x8($t3)
    ctx->r9 = MEM_W(ctx->r11, -0X8);
    // 0x80185C60: sw          $t1, -0x8($t5)
    MEM_W(-0X8, ctx->r13) = ctx->r9;
    // 0x80185C64: lw          $t7, -0x4($t3)
    ctx->r15 = MEM_W(ctx->r11, -0X4);
    // 0x80185C68: bne         $t3, $t6, L_80185C4C
    if (ctx->r11 != ctx->r14) {
        // 0x80185C6C: sw          $t7, -0x4($t5)
        MEM_W(-0X4, ctx->r13) = ctx->r15;
            goto L_80185C4C;
    }
    // 0x80185C6C: sw          $t7, -0x4($t5)
    MEM_W(-0X4, ctx->r13) = ctx->r15;
    // 0x80185C70: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80185C74: addiu       $t9, $t9, 0x667C
    ctx->r25 = ADD32(ctx->r25, 0X667C);
    // 0x80185C78: sw          $t9, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r25;
    // 0x80185C7C: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x80185C80: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80185C84: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x80185C88: jal         0x80185EF8
    // 0x80185C8C: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    func_80185EF8(rdram, ctx);
        goto after_25;
    // 0x80185C8C: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    after_25:
    // 0x80185C90: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x80185C94: lw          $a1, 0xE80($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE80);
    // 0x80185C98: jal         0x80013CC4
    // 0x80185C9C: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    func_80013CC4(rdram, ctx);
        goto after_26;
    // 0x80185C9C: addiu       $a2, $sp, 0x280
    ctx->r6 = ADD32(ctx->r29, 0X280);
    after_26:
    // 0x80185CA0: lwc1        $f10, 0x2B4($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X2B4);
    // 0x80185CA4: lwc1        $f16, 0xE8C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0XE8C);
    // 0x80185CA8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185CAC: addiu       $a1, $sp, 0x298
    ctx->r5 = ADD32(ctx->r29, 0X298);
    // 0x80185CB0: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x80185CB4: lw          $a2, 0xE94($sp)
    ctx->r6 = MEM_W(ctx->r29, 0XE94);
    // 0x80185CB8: jal         0x80013C10
    // 0x80185CBC: swc1        $f18, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f18.u32l;
    func_80013C10(rdram, ctx);
        goto after_27;
    // 0x80185CBC: swc1        $f18, 0x2B4($sp)
    MEM_W(0X2B4, ctx->r29) = ctx->f18.u32l;
    after_27:
    // 0x80185CC0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185CC4: lw          $a1, 0xE84($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XE84);
    // 0x80185CC8: jal         0x80013CC4
    // 0x80185CCC: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_80013CC4(rdram, ctx);
        goto after_28;
    // 0x80185CCC: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_28:
    // 0x80185CD0: lwc1        $f4, 0x2A8($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X2A8);
    // 0x80185CD4: lwc1        $f6, 0x27C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X27C);
    // 0x80185CD8: addiu       $a0, $sp, 0x28C
    ctx->r4 = ADD32(ctx->r29, 0X28C);
    // 0x80185CDC: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80185CE0: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80185CE4: addiu       $a2, $sp, 0x2B0
    ctx->r6 = ADD32(ctx->r29, 0X2B0);
    // 0x80185CE8: jal         0x80013CC4
    // 0x80185CEC: swc1        $f8, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f8.u32l;
    func_80013CC4(rdram, ctx);
        goto after_29;
    // 0x80185CEC: swc1        $f8, 0x2A8($sp)
    MEM_W(0X2A8, ctx->r29) = ctx->f8.u32l;
    after_29:
    // 0x80185CF0: addiu       $s0, $sp, 0x2C8
    ctx->r16 = ADD32(ctx->r29, 0X2C8);
    // 0x80185CF4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x80185CF8: addiu       $a0, $sp, 0x2B0
    ctx->r4 = ADD32(ctx->r29, 0X2B0);
    // 0x80185CFC: addiu       $a1, $sp, 0x28C
    ctx->r5 = ADD32(ctx->r29, 0X28C);
    // 0x80185D00: jal         0x8003E2F0
    // 0x80185D04: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    func_8003E2F0(rdram, ctx);
        goto after_30;
    // 0x80185D04: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    after_30:
    // 0x80185D08: or          $t2, $s0, $zero
    ctx->r10 = ctx->r16 | 0;
    // 0x80185D0C: or          $t4, $sp, $zero
    ctx->r12 = ctx->r29 | 0;
    // 0x80185D10: addiu       $t6, $s0, 0x258
    ctx->r14 = ADD32(ctx->r16, 0X258);
L_80185D14:
    // 0x80185D14: lw          $t8, 0x0($t2)
    ctx->r24 = MEM_W(ctx->r10, 0X0);
    // 0x80185D18: addiu       $t2, $t2, 0xC
    ctx->r10 = ADD32(ctx->r10, 0XC);
    // 0x80185D1C: addiu       $t4, $t4, 0xC
    ctx->r12 = ADD32(ctx->r12, 0XC);
    // 0x80185D20: sw          $t8, -0xC($t4)
    MEM_W(-0XC, ctx->r12) = ctx->r24;
    // 0x80185D24: lw          $t0, -0x8($t2)
    ctx->r8 = MEM_W(ctx->r10, -0X8);
    // 0x80185D28: sw          $t0, -0x8($t4)
    MEM_W(-0X8, ctx->r12) = ctx->r8;
    // 0x80185D2C: lw          $t8, -0x4($t2)
    ctx->r24 = MEM_W(ctx->r10, -0X4);
    // 0x80185D30: bne         $t2, $t6, L_80185D14
    if (ctx->r10 != ctx->r14) {
        // 0x80185D34: sw          $t8, -0x4($t4)
        MEM_W(-0X4, ctx->r12) = ctx->r24;
            goto L_80185D14;
    }
    // 0x80185D34: sw          $t8, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->r24;
    // 0x80185D38: lwc1        $f10, 0xE9C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XE9C);
    // 0x80185D3C: lui         $t5, 0x8019
    ctx->r13 = S32(0X8019 << 16);
    // 0x80185D40: addiu       $t5, $t5, 0x66B0
    ctx->r13 = ADD32(ctx->r13, 0X66B0);
    // 0x80185D44: addiu       $t3, $sp, 0x2BC
    ctx->r11 = ADD32(ctx->r29, 0X2BC);
    // 0x80185D48: sw          $t3, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r11;
    // 0x80185D4C: sw          $t5, 0x260($sp)
    MEM_W(0X260, ctx->r29) = ctx->r13;
    // 0x80185D50: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x80185D54: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80185D58: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x80185D5C: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    // 0x80185D60: jal         0x80185E04
    // 0x80185D64: swc1        $f10, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f10.u32l;
    func_80185E04(rdram, ctx);
        goto after_31;
    // 0x80185D64: swc1        $f10, 0x25C($sp)
    MEM_W(0X25C, ctx->r29) = ctx->f10.u32l;
    after_31:
    // 0x80185D68: or          $t9, $s0, $zero
    ctx->r25 = ctx->r16 | 0;
    // 0x80185D6C: or          $t6, $sp, $zero
    ctx->r14 = ctx->r29 | 0;
    // 0x80185D70: addiu       $t2, $s0, 0x258
    ctx->r10 = ADD32(ctx->r16, 0X258);
L_80185D74:
    // 0x80185D74: lw          $t7, 0x0($t9)
    ctx->r15 = MEM_W(ctx->r25, 0X0);
    // 0x80185D78: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x80185D7C: addiu       $t6, $t6, 0xC
    ctx->r14 = ADD32(ctx->r14, 0XC);
    // 0x80185D80: sw          $t7, -0xC($t6)
    MEM_W(-0XC, ctx->r14) = ctx->r15;
    // 0x80185D84: lw          $t1, -0x8($t9)
    ctx->r9 = MEM_W(ctx->r25, -0X8);
    // 0x80185D88: sw          $t1, -0x8($t6)
    MEM_W(-0X8, ctx->r14) = ctx->r9;
    // 0x80185D8C: lw          $t7, -0x4($t9)
    ctx->r15 = MEM_W(ctx->r25, -0X4);
    // 0x80185D90: bne         $t9, $t2, L_80185D74
    if (ctx->r25 != ctx->r10) {
        // 0x80185D94: sw          $t7, -0x4($t6)
        MEM_W(-0X4, ctx->r14) = ctx->r15;
            goto L_80185D74;
    }
    // 0x80185D94: sw          $t7, -0x4($t6)
    MEM_W(-0X4, ctx->r14) = ctx->r15;
    // 0x80185D98: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80185D9C: addiu       $t4, $t4, 0x66B0
    ctx->r12 = ADD32(ctx->r12, 0X66B0);
    // 0x80185DA0: sw          $t4, 0x258($sp)
    MEM_W(0X258, ctx->r29) = ctx->r12;
    // 0x80185DA4: lw          $a0, 0x0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X0);
    // 0x80185DA8: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80185DAC: lw          $a2, 0x8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X8);
    // 0x80185DB0: jal         0x80185EF8
    // 0x80185DB4: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    func_80185EF8(rdram, ctx);
        goto after_32;
    // 0x80185DB4: lw          $a3, 0xC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC);
    after_32:
    // 0x80185DB8: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x80185DBC: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80185DC0: addiu       $t8, $t8, 0x66B0
    ctx->r24 = ADD32(ctx->r24, 0X66B0);
    // 0x80185DC4: addiu       $t0, $t0, 0x667C
    ctx->r8 = ADD32(ctx->r8, 0X667C);
    // 0x80185DC8: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80185DCC: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80185DD0: lui         $a3, 0x8019
    ctx->r7 = S32(0X8019 << 16);
    // 0x80185DD4: addiu       $a3, $a3, 0x6648
    ctx->r7 = ADD32(ctx->r7, 0X6648);
    // 0x80185DD8: addiu       $a2, $a2, 0x6614
    ctx->r6 = ADD32(ctx->r6, 0X6614);
    // 0x80185DDC: addiu       $a1, $a1, 0x65E0
    ctx->r5 = ADD32(ctx->r5, 0X65E0);
    // 0x80185DE0: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    // 0x80185DE4: sw          $t8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r24;
    // 0x80185DE8: jal         0x80186038
    // 0x80185DEC: lw          $a0, 0xEA0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XEA0);
    func_80186038(rdram, ctx);
        goto after_33;
    // 0x80185DEC: lw          $a0, 0xEA0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XEA0);
    after_33:
L_80185DF0:
    // 0x80185DF0: lw          $ra, 0x274($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X274);
    // 0x80185DF4: lw          $s0, 0x270($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X270);
    // 0x80185DF8: addiu       $sp, $sp, 0xE80
    ctx->r29 = ADD32(ctx->r29, 0XE80);
    // 0x80185DFC: jr          $ra
    // 0x80185E00: nop

    return;
    // 0x80185E00: nop

;}
RECOMP_FUNC void func_80185E04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80185E04: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80185E08: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80185E0C: sw          $fp, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r30;
    // 0x80185E10: sw          $s7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r23;
    // 0x80185E14: sw          $s6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r22;
    // 0x80185E18: sw          $s5, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r21;
    // 0x80185E1C: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x80185E20: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x80185E24: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x80185E28: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x80185E2C: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80185E30: sw          $ra, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r31;
    // 0x80185E34: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x80185E38: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x80185E3C: sw          $a1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r5;
    // 0x80185E40: sw          $a2, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r6;
    // 0x80185E44: sw          $a3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r7;
    // 0x80185E48: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80185E4C: lw          $s2, 0x2A8($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X2A8);
    // 0x80185E50: addiu       $s3, $sp, 0x48
    ctx->r19 = ADD32(ctx->r29, 0X48);
    // 0x80185E54: lw          $s4, 0x2A0($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X2A0);
    // 0x80185E58: lui         $s5, 0x2000
    ctx->r21 = S32(0X2000 << 16);
    // 0x80185E5C: addiu       $s6, $sp, 0x48
    ctx->r22 = ADD32(ctx->r29, 0X48);
    // 0x80185E60: addiu       $s7, $zero, 0x60
    ctx->r23 = ADD32(0, 0X60);
    // 0x80185E64: addiu       $fp, $zero, 0x3
    ctx->r30 = ADD32(0, 0X3);
L_80185E68:
    // 0x80185E68: lw          $t6, 0x0($s3)
    ctx->r14 = MEM_W(ctx->r19, 0X0);
    // 0x80185E6C: and         $t7, $t6, $s5
    ctx->r15 = ctx->r14 & ctx->r21;
    // 0x80185E70: beql        $t7, $zero, L_80185EB4
    if (ctx->r15 == 0) {
        // 0x80185E74: swc1        $f20, 0x0($s2)
        MEM_W(0X0, ctx->r18) = ctx->f20.u32l;
            goto L_80185EB4;
    }
    goto skip_0;
    // 0x80185E74: swc1        $f20, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->f20.u32l;
    skip_0:
    // 0x80185E78: multu       $s1, $s7
    result = U64(U32(ctx->r17)) * U64(U32(ctx->r23)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80185E7C: mflo        $t8
    ctx->r24 = lo;
    // 0x80185E80: addu        $s0, $s6, $t8
    ctx->r16 = ADD32(ctx->r22, ctx->r24);
    // 0x80185E84: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
    // 0x80185E88: jal         0x801855FC
    // 0x80185E8C: lw          $a1, 0x18($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X18);
    func_801855FC(rdram, ctx);
        goto after_0;
    // 0x80185E8C: lw          $a1, 0x18($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X18);
    after_0:
    // 0x80185E90: beql        $v0, $zero, L_80185EB4
    if (ctx->r2 == 0) {
        // 0x80185E94: swc1        $f20, 0x0($s2)
        MEM_W(0X0, ctx->r18) = ctx->f20.u32l;
            goto L_80185EB4;
    }
    goto skip_1;
    // 0x80185E94: swc1        $f20, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->f20.u32l;
    skip_1:
    // 0x80185E98: jal         0x80013BD0
    // 0x80185E9C: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    func_80013BD0(rdram, ctx);
        goto after_1;
    // 0x80185E9C: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    after_1:
    // 0x80185EA0: lwc1        $f4, 0x14($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X14);
    // 0x80185EA4: div.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = DIV_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80185EA8: b           L_80185EB4
    // 0x80185EAC: swc1        $f6, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->f6.u32l;
        goto L_80185EB4;
    // 0x80185EAC: swc1        $f6, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->f6.u32l;
    // 0x80185EB0: swc1        $f20, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->f20.u32l;
L_80185EB4:
    // 0x80185EB4: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80185EB8: addiu       $s3, $s3, 0x4
    ctx->r19 = ADD32(ctx->r19, 0X4);
    // 0x80185EBC: bne         $s1, $fp, L_80185E68
    if (ctx->r17 != ctx->r30) {
        // 0x80185EC0: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_80185E68;
    }
    // 0x80185EC0: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x80185EC4: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    // 0x80185EC8: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80185ECC: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80185ED0: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80185ED4: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x80185ED8: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x80185EDC: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x80185EE0: lw          $s5, 0x34($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X34);
    // 0x80185EE4: lw          $s6, 0x38($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X38);
    // 0x80185EE8: lw          $s7, 0x3C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X3C);
    // 0x80185EEC: lw          $fp, 0x40($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X40);
    // 0x80185EF0: jr          $ra
    // 0x80185EF4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x80185EF4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_80185EF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80185EF8: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80185EFC: sw          $s4, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r20;
    // 0x80185F00: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80185F04: sw          $fp, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r30;
    // 0x80185F08: sw          $s7, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r23;
    // 0x80185F0C: sw          $s6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r22;
    // 0x80185F10: sw          $s5, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r21;
    // 0x80185F14: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x80185F18: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80185F1C: lui         $s4, 0x800C
    ctx->r20 = S32(0X800C << 16);
    // 0x80185F20: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80185F24: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80185F28: sw          $ra, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r31;
    // 0x80185F2C: sw          $s3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r19;
    // 0x80185F30: sw          $s1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r17;
    // 0x80185F34: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x80185F38: sw          $a0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r4;
    // 0x80185F3C: sw          $a1, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r5;
    // 0x80185F40: sw          $a2, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r6;
    // 0x80185F44: sw          $a3, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r7;
    // 0x80185F48: lwc1        $f22, -0x30F0($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0X30F0);
    // 0x80185F4C: addiu       $s4, $s4, -0x3068
    ctx->r20 = ADD32(ctx->r20, -0X3068);
    // 0x80185F50: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80185F54: addiu       $s5, $sp, 0x50
    ctx->r21 = ADD32(ctx->r29, 0X50);
    // 0x80185F58: lw          $s6, 0x2A8($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2A8);
    // 0x80185F5C: lui         $s7, 0x2000
    ctx->r23 = S32(0X2000 << 16);
    // 0x80185F60: addiu       $fp, $sp, 0x50
    ctx->r30 = ADD32(ctx->r29, 0X50);
L_80185F64:
    // 0x80185F64: lw          $t6, 0x0($s5)
    ctx->r14 = MEM_W(ctx->r21, 0X0);
    // 0x80185F68: sll         $t8, $s2, 2
    ctx->r24 = S32(ctx->r18 << 2);
    // 0x80185F6C: subu        $t8, $t8, $s2
    ctx->r24 = SUB32(ctx->r24, ctx->r18);
    // 0x80185F70: and         $t7, $t6, $s7
    ctx->r15 = ctx->r14 & ctx->r23;
    // 0x80185F74: beq         $t7, $zero, L_80185FF0
    if (ctx->r15 == 0) {
        // 0x80185F78: sll         $t8, $t8, 5
        ctx->r24 = S32(ctx->r24 << 5);
            goto L_80185FF0;
    }
    // 0x80185F78: sll         $t8, $t8, 5
    ctx->r24 = S32(ctx->r24 << 5);
    // 0x80185F7C: addu        $s0, $fp, $t8
    ctx->r16 = ADD32(ctx->r30, ctx->r24);
    // 0x80185F80: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
    // 0x80185F84: jal         0x801855FC
    // 0x80185F88: lw          $a1, 0x18($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X18);
    func_801855FC(rdram, ctx);
        goto after_0;
    // 0x80185F88: lw          $a1, 0x18($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X18);
    after_0:
    // 0x80185F8C: beq         $v0, $zero, L_80185FF0
    if (ctx->r2 == 0) {
        // 0x80185F90: or          $a0, $s4, $zero
        ctx->r4 = ctx->r20 | 0;
            goto L_80185FF0;
    }
    // 0x80185F90: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80185F94: addiu       $s1, $s0, 0x1C
    ctx->r17 = ADD32(ctx->r16, 0X1C);
    // 0x80185F98: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80185F9C: jal         0x80013E3C
    // 0x80185FA0: addu        $s3, $s6, $s2
    ctx->r19 = ADD32(ctx->r22, ctx->r18);
    func_80013E3C(rdram, ctx);
        goto after_1;
    // 0x80185FA0: addu        $s3, $s6, $s2
    ctx->r19 = ADD32(ctx->r22, ctx->r18);
    after_1:
    // 0x80185FA4: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x80185FA8: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80185FAC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80185FB0: bc1f        L_80185FC8
    if (!c1cs) {
        // 0x80185FB4: nop
    
            goto L_80185FC8;
    }
    // 0x80185FB4: nop

    // 0x80185FB8: jal         0x80013E3C
    // 0x80185FBC: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    func_80013E3C(rdram, ctx);
        goto after_2;
    // 0x80185FBC: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    after_2:
    // 0x80185FC0: b           L_80185FD4
    // 0x80185FC4: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_80185FD4;
    // 0x80185FC4: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_80185FC8:
    // 0x80185FC8: jal         0x80013E3C
    // 0x80185FCC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80013E3C(rdram, ctx);
        goto after_3;
    // 0x80185FCC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_3:
    // 0x80185FD0: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_80185FD4:
    // 0x80185FD4: c.le.s      $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f22.fl <= ctx->f2.fl;
    // 0x80185FD8: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x80185FDC: bc1fl       L_80185FF0
    if (!c1cs) {
        // 0x80185FE0: sb          $zero, 0xC($s3)
        MEM_B(0XC, ctx->r19) = 0;
            goto L_80185FF0;
    }
    goto skip_0;
    // 0x80185FE0: sb          $zero, 0xC($s3)
    MEM_B(0XC, ctx->r19) = 0;
    skip_0:
    // 0x80185FE4: b           L_80185FF0
    // 0x80185FE8: sb          $t9, 0xC($s3)
    MEM_B(0XC, ctx->r19) = ctx->r25;
        goto L_80185FF0;
    // 0x80185FE8: sb          $t9, 0xC($s3)
    MEM_B(0XC, ctx->r19) = ctx->r25;
    // 0x80185FEC: sb          $zero, 0xC($s3)
    MEM_B(0XC, ctx->r19) = 0;
L_80185FF0:
    // 0x80185FF0: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x80185FF4: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x80185FF8: bne         $s2, $at, L_80185F64
    if (ctx->r18 != ctx->r1) {
        // 0x80185FFC: addiu       $s5, $s5, 0x4
        ctx->r21 = ADD32(ctx->r21, 0X4);
            goto L_80185F64;
    }
    // 0x80185FFC: addiu       $s5, $s5, 0x4
    ctx->r21 = ADD32(ctx->r21, 0X4);
    // 0x80186000: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
    // 0x80186004: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80186008: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x8018600C: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x80186010: lw          $s1, 0x2C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X2C);
    // 0x80186014: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x80186018: lw          $s3, 0x34($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X34);
    // 0x8018601C: lw          $s4, 0x38($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X38);
    // 0x80186020: lw          $s5, 0x3C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X3C);
    // 0x80186024: lw          $s6, 0x40($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X40);
    // 0x80186028: lw          $s7, 0x44($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X44);
    // 0x8018602C: lw          $fp, 0x48($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X48);
    // 0x80186030: jr          $ra
    // 0x80186034: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x80186034: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_80186038(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186038: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x8018603C: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80186040: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80186044: sw          $a0, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r4;
    // 0x80186048: addiu       $t7, $t7, 0x66E4
    ctx->r15 = ADD32(ctx->r15, 0X66E4);
    // 0x8018604C: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x80186050: addiu       $t6, $sp, 0x38
    ctx->r14 = ADD32(ctx->r29, 0X38);
    // 0x80186054: lw          $t8, 0x4($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X4);
    // 0x80186058: sw          $t9, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->r25;
    // 0x8018605C: lw          $t9, 0x8($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X8);
    // 0x80186060: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80186064: addiu       $t1, $t1, 0x66F0
    ctx->r9 = ADD32(ctx->r9, 0X66F0);
    // 0x80186068: sw          $t8, 0x4($t6)
    MEM_W(0X4, ctx->r14) = ctx->r24;
    // 0x8018606C: sw          $t9, 0x8($t6)
    MEM_W(0X8, ctx->r14) = ctx->r25;
    // 0x80186070: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x80186074: addiu       $t0, $sp, 0x2C
    ctx->r8 = ADD32(ctx->r29, 0X2C);
    // 0x80186078: lw          $t2, 0x4($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X4);
    // 0x8018607C: sw          $t3, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r11;
    // 0x80186080: lw          $t3, 0x8($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X8);
    // 0x80186084: sw          $t2, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->r10;
    // 0x80186088: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8018608C: sw          $t3, 0x8($t0)
    MEM_W(0X8, ctx->r8) = ctx->r11;
    // 0x80186090: lwc1        $f18, 0x0($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80186094: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80186098: addiu       $v1, $a1, 0x4
    ctx->r3 = ADD32(ctx->r5, 0X4);
L_8018609C:
    // 0x8018609C: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    // 0x801860A0: nop

    // 0x801860A4: bc1fl       L_801860B8
    if (!c1cs) {
        // 0x801860A8: lwc1        $f0, 0x0($v1)
        ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
            goto L_801860B8;
    }
    goto skip_0;
    // 0x801860A8: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    skip_0:
    // 0x801860AC: b           L_801860E8
    // 0x801860B0: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
        goto L_801860E8;
    // 0x801860B0: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x801860B4: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
L_801860B8:
    // 0x801860B8: c.eq.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl == ctx->f0.fl;
    // 0x801860BC: nop

    // 0x801860C0: bc1tl       L_801860EC
    if (c1cs) {
        // 0x801860C4: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_801860EC;
    }
    goto skip_1;
    // 0x801860C4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_1:
    // 0x801860C8: c.lt.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl < ctx->f18.fl;
    // 0x801860CC: addu        $t4, $a1, $v0
    ctx->r12 = ADD32(ctx->r5, ctx->r2);
    // 0x801860D0: bc1fl       L_801860EC
    if (!c1cs) {
        // 0x801860D4: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_801860EC;
    }
    goto skip_2;
    // 0x801860D4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_2:
    // 0x801860D8: lbu         $t5, 0xC($t4)
    ctx->r13 = MEM_BU(ctx->r12, 0XC);
    // 0x801860DC: bnel        $t5, $zero, L_801860EC
    if (ctx->r13 != 0) {
        // 0x801860E0: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_801860EC;
    }
    goto skip_3;
    // 0x801860E0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_3:
    // 0x801860E4: mov.s       $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    ctx->f18.fl = ctx->f0.fl;
L_801860E8:
    // 0x801860E8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_801860EC:
    // 0x801860EC: slti        $at, $v0, 0x3
    ctx->r1 = SIGNED(ctx->r2) < 0X3 ? 1 : 0;
    // 0x801860F0: bne         $at, $zero, L_8018609C
    if (ctx->r1 != 0) {
        // 0x801860F4: addiu       $v1, $v1, 0x4
        ctx->r3 = ADD32(ctx->r3, 0X4);
            goto L_8018609C;
    }
    // 0x801860F4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801860F8: lw          $a0, 0x78($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X78);
    // 0x801860FC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80186100: lwc1        $f16, 0x0($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80186104: addiu       $v1, $a0, 0x4
    ctx->r3 = ADD32(ctx->r4, 0X4);
L_80186108:
    // 0x80186108: c.eq.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl == ctx->f2.fl;
    // 0x8018610C: nop

    // 0x80186110: bc1fl       L_80186124
    if (!c1cs) {
        // 0x80186114: lwc1        $f0, 0x0($v1)
        ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
            goto L_80186124;
    }
    goto skip_4;
    // 0x80186114: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    skip_4:
    // 0x80186118: b           L_80186154
    // 0x8018611C: lwc1        $f16, 0x0($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X0);
        goto L_80186154;
    // 0x8018611C: lwc1        $f16, 0x0($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X0);
    // 0x80186120: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
L_80186124:
    // 0x80186124: c.eq.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl == ctx->f0.fl;
    // 0x80186128: nop

    // 0x8018612C: bc1tl       L_80186158
    if (c1cs) {
        // 0x80186130: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80186158;
    }
    goto skip_5;
    // 0x80186130: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_5:
    // 0x80186134: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x80186138: addu        $t6, $a0, $v0
    ctx->r14 = ADD32(ctx->r4, ctx->r2);
    // 0x8018613C: bc1fl       L_80186158
    if (!c1cs) {
        // 0x80186140: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80186158;
    }
    goto skip_6;
    // 0x80186140: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_6:
    // 0x80186144: lbu         $t7, 0xC($t6)
    ctx->r15 = MEM_BU(ctx->r14, 0XC);
    // 0x80186148: bnel        $t7, $zero, L_80186158
    if (ctx->r15 != 0) {
        // 0x8018614C: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80186158;
    }
    goto skip_7;
    // 0x8018614C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_7:
    // 0x80186150: mov.s       $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    ctx->f16.fl = ctx->f0.fl;
L_80186154:
    // 0x80186154: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_80186158:
    // 0x80186158: slti        $at, $v0, 0x3
    ctx->r1 = SIGNED(ctx->r2) < 0X3 ? 1 : 0;
    // 0x8018615C: bne         $at, $zero, L_80186108
    if (ctx->r1 != 0) {
        // 0x80186160: addiu       $v1, $v1, 0x4
        ctx->r3 = ADD32(ctx->r3, 0X4);
            goto L_80186108;
    }
    // 0x80186160: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80186164: lwc1        $f14, 0x0($a2)
    ctx->f14.u32l = MEM_W(ctx->r6, 0X0);
    // 0x80186168: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8018616C: addiu       $v1, $a2, 0x4
    ctx->r3 = ADD32(ctx->r6, 0X4);
L_80186170:
    // 0x80186170: c.eq.s      $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f14.fl == ctx->f2.fl;
    // 0x80186174: nop

    // 0x80186178: bc1fl       L_8018618C
    if (!c1cs) {
        // 0x8018617C: lwc1        $f0, 0x0($v1)
        ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
            goto L_8018618C;
    }
    goto skip_8;
    // 0x8018617C: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    skip_8:
    // 0x80186180: b           L_801861BC
    // 0x80186184: lwc1        $f14, 0x0($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X0);
        goto L_801861BC;
    // 0x80186184: lwc1        $f14, 0x0($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X0);
    // 0x80186188: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
L_8018618C:
    // 0x8018618C: c.eq.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl == ctx->f0.fl;
    // 0x80186190: nop

    // 0x80186194: bc1tl       L_801861C0
    if (c1cs) {
        // 0x80186198: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_801861C0;
    }
    goto skip_9;
    // 0x80186198: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_9:
    // 0x8018619C: c.lt.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl < ctx->f14.fl;
    // 0x801861A0: addu        $t8, $a2, $v0
    ctx->r24 = ADD32(ctx->r6, ctx->r2);
    // 0x801861A4: bc1fl       L_801861C0
    if (!c1cs) {
        // 0x801861A8: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_801861C0;
    }
    goto skip_10;
    // 0x801861A8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_10:
    // 0x801861AC: lbu         $t9, 0xC($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0XC);
    // 0x801861B0: bnel        $t9, $zero, L_801861C0
    if (ctx->r25 != 0) {
        // 0x801861B4: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_801861C0;
    }
    goto skip_11;
    // 0x801861B4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_11:
    // 0x801861B8: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
L_801861BC:
    // 0x801861BC: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_801861C0:
    // 0x801861C0: slti        $at, $v0, 0x3
    ctx->r1 = SIGNED(ctx->r2) < 0X3 ? 1 : 0;
    // 0x801861C4: bne         $at, $zero, L_80186170
    if (ctx->r1 != 0) {
        // 0x801861C8: addiu       $v1, $v1, 0x4
        ctx->r3 = ADD32(ctx->r3, 0X4);
            goto L_80186170;
    }
    // 0x801861C8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801861CC: lw          $a0, 0x7C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X7C);
    // 0x801861D0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801861D4: addiu       $a1, $zero, 0x3
    ctx->r5 = ADD32(0, 0X3);
    // 0x801861D8: lwc1        $f12, 0x0($a0)
    ctx->f12.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801861DC: addiu       $v1, $a0, 0x4
    ctx->r3 = ADD32(ctx->r4, 0X4);
L_801861E0:
    // 0x801861E0: c.eq.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl == ctx->f2.fl;
    // 0x801861E4: nop

    // 0x801861E8: bc1fl       L_801861FC
    if (!c1cs) {
        // 0x801861EC: lwc1        $f0, 0x0($v1)
        ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
            goto L_801861FC;
    }
    goto skip_12;
    // 0x801861EC: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    skip_12:
    // 0x801861F0: b           L_8018622C
    // 0x801861F4: lwc1        $f12, 0x0($v1)
    ctx->f12.u32l = MEM_W(ctx->r3, 0X0);
        goto L_8018622C;
    // 0x801861F4: lwc1        $f12, 0x0($v1)
    ctx->f12.u32l = MEM_W(ctx->r3, 0X0);
    // 0x801861F8: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
L_801861FC:
    // 0x801861FC: c.eq.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl == ctx->f0.fl;
    // 0x80186200: nop

    // 0x80186204: bc1tl       L_80186230
    if (c1cs) {
        // 0x80186208: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80186230;
    }
    goto skip_13;
    // 0x80186208: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_13:
    // 0x8018620C: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80186210: addu        $t0, $a0, $v0
    ctx->r8 = ADD32(ctx->r4, ctx->r2);
    // 0x80186214: bc1fl       L_80186230
    if (!c1cs) {
        // 0x80186218: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80186230;
    }
    goto skip_14;
    // 0x80186218: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_14:
    // 0x8018621C: lbu         $t1, 0xC($t0)
    ctx->r9 = MEM_BU(ctx->r8, 0XC);
    // 0x80186220: bnel        $t1, $zero, L_80186230
    if (ctx->r9 != 0) {
        // 0x80186224: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80186230;
    }
    goto skip_15;
    // 0x80186224: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_15:
    // 0x80186228: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
L_8018622C:
    // 0x8018622C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_80186230:
    // 0x80186230: bne         $v0, $a1, L_801861E0
    if (ctx->r2 != ctx->r5) {
        // 0x80186234: addiu       $v1, $v1, 0x4
        ctx->r3 = ADD32(ctx->r3, 0X4);
            goto L_801861E0;
    }
    // 0x80186234: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80186238: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    // 0x8018623C: nop

    // 0x80186240: bc1fl       L_801862A4
    if (!c1cs) {
        // 0x80186244: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_801862A4;
    }
    goto skip_16;
    // 0x80186244: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_16:
    // 0x80186248: c.eq.s      $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f14.fl == ctx->f2.fl;
    // 0x8018624C: nop

    // 0x80186250: bc1fl       L_801862A4
    if (!c1cs) {
        // 0x80186254: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_801862A4;
    }
    goto skip_17;
    // 0x80186254: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_17:
    // 0x80186258: c.eq.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl == ctx->f2.fl;
    // 0x8018625C: nop

    // 0x80186260: bc1fl       L_801862A4
    if (!c1cs) {
        // 0x80186264: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_801862A4;
    }
    goto skip_18;
    // 0x80186264: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_18:
    // 0x80186268: c.eq.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl == ctx->f2.fl;
    // 0x8018626C: lw          $t2, 0x68($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X68);
    // 0x80186270: bc1fl       L_801862A4
    if (!c1cs) {
        // 0x80186274: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_801862A4;
    }
    goto skip_19;
    // 0x80186274: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_19:
    // 0x80186278: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8018627C: lui         $t3, 0x801D
    ctx->r11 = S32(0X801D << 16);
    // 0x80186280: swc1        $f4, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f4.u32l;
    // 0x80186284: lw          $t3, -0x51D4($t3)
    ctx->r11 = MEM_W(ctx->r11, -0X51D4);
    // 0x80186288: lw          $a0, 0x34($t3)
    ctx->r4 = MEM_W(ctx->r11, 0X34);
    // 0x8018628C: jal         0x8017FDE0
    // 0x80186290: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    func_8017FDE0(rdram, ctx);
        goto after_0;
    // 0x80186290: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    after_0:
    // 0x80186294: lw          $t4, 0x68($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X68);
    // 0x80186298: b           L_80186408
    // 0x8018629C: swc1        $f0, 0x4($t4)
    MEM_W(0X4, ctx->r12) = ctx->f0.u32l;
        goto L_80186408;
    // 0x8018629C: swc1        $f0, 0x4($t4)
    MEM_W(0X4, ctx->r12) = ctx->f0.u32l;
    // 0x801862A0: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
L_801862A4:
    // 0x801862A4: nop

    // 0x801862A8: bc1tl       L_8018630C
    if (c1cs) {
        // 0x801862AC: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_8018630C;
    }
    goto skip_20;
    // 0x801862AC: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_20:
    // 0x801862B0: c.eq.s      $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f14.fl == ctx->f2.fl;
    // 0x801862B4: nop

    // 0x801862B8: bc1tl       L_8018630C
    if (c1cs) {
        // 0x801862BC: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_8018630C;
    }
    goto skip_21;
    // 0x801862BC: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_21:
    // 0x801862C0: c.eq.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl == ctx->f2.fl;
    // 0x801862C4: nop

    // 0x801862C8: bc1fl       L_8018630C
    if (!c1cs) {
        // 0x801862CC: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_8018630C;
    }
    goto skip_22;
    // 0x801862CC: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_22:
    // 0x801862D0: c.eq.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl == ctx->f2.fl;
    // 0x801862D4: lw          $t5, 0x68($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X68);
    // 0x801862D8: bc1fl       L_8018630C
    if (!c1cs) {
        // 0x801862DC: c.eq.s      $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
            goto L_8018630C;
    }
    goto skip_23;
    // 0x801862DC: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
    skip_23:
    // 0x801862E0: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x801862E4: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801862E8: swc1        $f6, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->f6.u32l;
    // 0x801862EC: lw          $t6, -0x51D4($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X51D4);
    // 0x801862F0: lw          $a0, 0x34($t6)
    ctx->r4 = MEM_W(ctx->r14, 0X34);
    // 0x801862F4: jal         0x8017FDE0
    // 0x801862F8: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    func_8017FDE0(rdram, ctx);
        goto after_1;
    // 0x801862F8: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    after_1:
    // 0x801862FC: lw          $t7, 0x68($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X68);
    // 0x80186300: b           L_80186408
    // 0x80186304: swc1        $f0, 0x4($t7)
    MEM_W(0X4, ctx->r15) = ctx->f0.u32l;
        goto L_80186408;
    // 0x80186304: swc1        $f0, 0x4($t7)
    MEM_W(0X4, ctx->r15) = ctx->f0.u32l;
    // 0x80186308: c.eq.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl == ctx->f2.fl;
L_8018630C:
    // 0x8018630C: nop

    // 0x80186310: bc1fl       L_80186374
    if (!c1cs) {
        // 0x80186314: add.s       $f10, $f18, $f16
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_80186374;
    }
    goto skip_24;
    // 0x80186314: add.s       $f10, $f18, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
    skip_24:
    // 0x80186318: c.eq.s      $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f14.fl == ctx->f2.fl;
    // 0x8018631C: nop

    // 0x80186320: bc1fl       L_80186374
    if (!c1cs) {
        // 0x80186324: add.s       $f10, $f18, $f16
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_80186374;
    }
    goto skip_25;
    // 0x80186324: add.s       $f10, $f18, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
    skip_25:
    // 0x80186328: c.eq.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl == ctx->f2.fl;
    // 0x8018632C: nop

    // 0x80186330: bc1tl       L_80186374
    if (c1cs) {
        // 0x80186334: add.s       $f10, $f18, $f16
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_80186374;
    }
    goto skip_26;
    // 0x80186334: add.s       $f10, $f18, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
    skip_26:
    // 0x80186338: c.eq.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl == ctx->f2.fl;
    // 0x8018633C: lw          $t8, 0x68($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X68);
    // 0x80186340: bc1tl       L_80186374
    if (c1cs) {
        // 0x80186344: add.s       $f10, $f18, $f16
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_80186374;
    }
    goto skip_27;
    // 0x80186344: add.s       $f10, $f18, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
    skip_27:
    // 0x80186348: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8018634C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x80186350: swc1        $f8, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->f8.u32l;
    // 0x80186354: lw          $t9, -0x51D4($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X51D4);
    // 0x80186358: lw          $a0, 0x34($t9)
    ctx->r4 = MEM_W(ctx->r25, 0X34);
    // 0x8018635C: jal         0x8017FDE0
    // 0x80186360: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    func_8017FDE0(rdram, ctx);
        goto after_2;
    // 0x80186360: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    after_2:
    // 0x80186364: lw          $t0, 0x68($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X68);
    // 0x80186368: b           L_80186408
    // 0x8018636C: swc1        $f0, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->f0.u32l;
        goto L_80186408;
    // 0x8018636C: swc1        $f0, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->f0.u32l;
    // 0x80186370: add.s       $f10, $f18, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f16.fl;
L_80186374:
    // 0x80186374: add.s       $f4, $f14, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f14.fl + ctx->f12.fl;
    // 0x80186378: sub.s       $f0, $f10, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x8018637C: swc1        $f0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f0.u32l;
    // 0x80186380: lwc1        $f6, 0x4($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X4);
    // 0x80186384: c.eq.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl == ctx->f6.fl;
    // 0x80186388: nop

    // 0x8018638C: bc1tl       L_801863A0
    if (c1cs) {
        // 0x80186390: lwc1        $f10, 0x28($sp)
        ctx->f10.u32l = MEM_W(ctx->r29, 0X28);
            goto L_801863A0;
    }
    goto skip_28;
    // 0x80186390: lwc1        $f10, 0x28($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X28);
    skip_28:
    // 0x80186394: add.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f0.fl + ctx->f0.fl;
    // 0x80186398: swc1        $f8, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f8.u32l;
    // 0x8018639C: lwc1        $f10, 0x28($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X28);
L_801863A0:
    // 0x801863A0: lw          $t1, 0x68($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X68);
    // 0x801863A4: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801863A8: swc1        $f10, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->f10.u32l;
    // 0x801863AC: lw          $t2, -0x51D4($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X51D4);
    // 0x801863B0: lw          $a0, 0x34($t2)
    ctx->r4 = MEM_W(ctx->r10, 0X34);
    // 0x801863B4: jal         0x8017FDE0
    // 0x801863B8: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    func_8017FDE0(rdram, ctx);
        goto after_3;
    // 0x801863B8: addiu       $a0, $a0, 0x9C
    ctx->r4 = ADD32(ctx->r4, 0X9C);
    after_3:
    // 0x801863BC: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x801863C0: lw          $t3, 0x27A8($t3)
    ctx->r11 = MEM_W(ctx->r11, 0X27A8);
    // 0x801863C4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801863C8: ldc1        $f8, -0x30E8($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X30E8);
    // 0x801863CC: lw          $t4, 0x3C($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X3C);
    // 0x801863D0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801863D4: lw          $t5, 0x68($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X68);
    // 0x801863D8: lwc1        $f4, 0x0($t4)
    ctx->f4.u32l = MEM_W(ctx->r12, 0X0);
    // 0x801863DC: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801863E0: ldc1        $f4, -0x30E0($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X30E0);
    // 0x801863E4: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x801863E8: lwc1        $f8, 0x28($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X28);
    // 0x801863EC: div.d       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = DIV_D(ctx->f10.d, ctx->f4.d);
    // 0x801863F0: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x801863F4: cvt.d.s     $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f8.d = CVT_D_S(ctx->f0.fl);
    // 0x801863F8: mul.d       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f10.d); 
    ctx->f4.d = MUL_D(ctx->f6.d, ctx->f10.d);
    // 0x801863FC: add.d       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f8.d); 
    ctx->f6.d = ctx->f4.d + ctx->f8.d;
    // 0x80186400: cvt.s.d     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f10.fl = CVT_S_D(ctx->f6.d);
    // 0x80186404: swc1        $f10, 0x4($t5)
    MEM_W(0X4, ctx->r13) = ctx->f10.u32l;
L_80186408:
    // 0x80186408: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8018640C: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    // 0x80186410: jr          $ra
    // 0x80186414: nop

    return;
    // 0x80186414: nop

    // 0x80186418: jr          $ra
    // 0x8018641C: nop

    return;
    // 0x8018641C: nop

;}
RECOMP_FUNC void func_80186420(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186420: addiu       $sp, $sp, -0x108
    ctx->r29 = ADD32(ctx->r29, -0X108);
    // 0x80186424: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x80186428: addiu       $v0, $sp, 0xD0
    ctx->r2 = ADD32(ctx->r29, 0XD0);
    // 0x8018642C: addiu       $t6, $t6, 0x66FC
    ctx->r14 = ADD32(ctx->r14, 0X66FC);
    // 0x80186430: addiu       $v1, $sp, 0x0
    ctx->r3 = ADD32(ctx->r29, 0X0);
    // 0x80186434: addiu       $a0, $sp, 0x34
    ctx->r4 = ADD32(ctx->r29, 0X34);
    // 0x80186438: addiu       $a1, $sp, 0x68
    ctx->r5 = ADD32(ctx->r29, 0X68);
    // 0x8018643C: addiu       $a2, $sp, 0x9C
    ctx->r6 = ADD32(ctx->r29, 0X9C);
    // 0x80186440: addiu       $t0, $t6, 0x30
    ctx->r8 = ADD32(ctx->r14, 0X30);
    // 0x80186444: or          $t9, $v0, $zero
    ctx->r25 = ctx->r2 | 0;
L_80186448:
    // 0x80186448: lw          $t8, 0x0($t6)
    ctx->r24 = MEM_W(ctx->r14, 0X0);
    // 0x8018644C: addiu       $t6, $t6, 0xC
    ctx->r14 = ADD32(ctx->r14, 0XC);
    // 0x80186450: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x80186454: sw          $t8, -0xC($t9)
    MEM_W(-0XC, ctx->r25) = ctx->r24;
    // 0x80186458: lw          $t7, -0x8($t6)
    ctx->r15 = MEM_W(ctx->r14, -0X8);
    // 0x8018645C: sw          $t7, -0x8($t9)
    MEM_W(-0X8, ctx->r25) = ctx->r15;
    // 0x80186460: lw          $t8, -0x4($t6)
    ctx->r24 = MEM_W(ctx->r14, -0X4);
    // 0x80186464: bne         $t6, $t0, L_80186448
    if (ctx->r14 != ctx->r8) {
        // 0x80186468: sw          $t8, -0x4($t9)
        MEM_W(-0X4, ctx->r25) = ctx->r24;
            goto L_80186448;
    }
    // 0x80186468: sw          $t8, -0x4($t9)
    MEM_W(-0X4, ctx->r25) = ctx->r24;
    // 0x8018646C: lw          $t8, 0x0($t6)
    ctx->r24 = MEM_W(ctx->r14, 0X0);
    // 0x80186470: or          $t3, $v0, $zero
    ctx->r11 = ctx->r2 | 0;
    // 0x80186474: or          $t4, $v1, $zero
    ctx->r12 = ctx->r3 | 0;
    // 0x80186478: addiu       $t5, $v0, 0x30
    ctx->r13 = ADD32(ctx->r2, 0X30);
    // 0x8018647C: sw          $t8, 0x0($t9)
    MEM_W(0X0, ctx->r25) = ctx->r24;
L_80186480:
    // 0x80186480: lw          $t2, 0x0($t3)
    ctx->r10 = MEM_W(ctx->r11, 0X0);
    // 0x80186484: addiu       $t3, $t3, 0xC
    ctx->r11 = ADD32(ctx->r11, 0XC);
    // 0x80186488: addiu       $t4, $t4, 0xC
    ctx->r12 = ADD32(ctx->r12, 0XC);
    // 0x8018648C: sw          $t2, -0xC($t4)
    MEM_W(-0XC, ctx->r12) = ctx->r10;
    // 0x80186490: lw          $t1, -0x8($t3)
    ctx->r9 = MEM_W(ctx->r11, -0X8);
    // 0x80186494: sw          $t1, -0x8($t4)
    MEM_W(-0X8, ctx->r12) = ctx->r9;
    // 0x80186498: lw          $t2, -0x4($t3)
    ctx->r10 = MEM_W(ctx->r11, -0X4);
    // 0x8018649C: bne         $t3, $t5, L_80186480
    if (ctx->r11 != ctx->r13) {
        // 0x801864A0: sw          $t2, -0x4($t4)
        MEM_W(-0X4, ctx->r12) = ctx->r10;
            goto L_80186480;
    }
    // 0x801864A0: sw          $t2, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->r10;
    // 0x801864A4: lw          $t2, 0x0($t3)
    ctx->r10 = MEM_W(ctx->r11, 0X0);
    // 0x801864A8: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x801864AC: addiu       $t0, $t0, 0x66B0
    ctx->r8 = ADD32(ctx->r8, 0X66B0);
    // 0x801864B0: or          $t7, $v1, $zero
    ctx->r15 = ctx->r3 | 0;
    // 0x801864B4: addiu       $t8, $v1, 0x30
    ctx->r24 = ADD32(ctx->r3, 0X30);
    // 0x801864B8: sw          $t2, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r10;
L_801864BC:
    // 0x801864BC: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x801864C0: addiu       $t7, $t7, 0xC
    ctx->r15 = ADD32(ctx->r15, 0XC);
    // 0x801864C4: addiu       $t0, $t0, 0xC
    ctx->r8 = ADD32(ctx->r8, 0XC);
    // 0x801864C8: sw          $t9, -0xC($t0)
    MEM_W(-0XC, ctx->r8) = ctx->r25;
    // 0x801864CC: lw          $t6, -0x8($t7)
    ctx->r14 = MEM_W(ctx->r15, -0X8);
    // 0x801864D0: sw          $t6, -0x8($t0)
    MEM_W(-0X8, ctx->r8) = ctx->r14;
    // 0x801864D4: lw          $t9, -0x4($t7)
    ctx->r25 = MEM_W(ctx->r15, -0X4);
    // 0x801864D8: bne         $t7, $t8, L_801864BC
    if (ctx->r15 != ctx->r24) {
        // 0x801864DC: sw          $t9, -0x4($t0)
        MEM_W(-0X4, ctx->r8) = ctx->r25;
            goto L_801864BC;
    }
    // 0x801864DC: sw          $t9, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->r25;
    // 0x801864E0: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x801864E4: or          $t4, $v1, $zero
    ctx->r12 = ctx->r3 | 0;
    // 0x801864E8: or          $t1, $a0, $zero
    ctx->r9 = ctx->r4 | 0;
    // 0x801864EC: addiu       $t2, $v1, 0x30
    ctx->r10 = ADD32(ctx->r3, 0X30);
    // 0x801864F0: sw          $t9, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r25;
L_801864F4:
    // 0x801864F4: lw          $t3, 0x0($t4)
    ctx->r11 = MEM_W(ctx->r12, 0X0);
    // 0x801864F8: addiu       $t4, $t4, 0xC
    ctx->r12 = ADD32(ctx->r12, 0XC);
    // 0x801864FC: addiu       $t1, $t1, 0xC
    ctx->r9 = ADD32(ctx->r9, 0XC);
    // 0x80186500: sw          $t3, -0xC($t1)
    MEM_W(-0XC, ctx->r9) = ctx->r11;
    // 0x80186504: lw          $t5, -0x8($t4)
    ctx->r13 = MEM_W(ctx->r12, -0X8);
    // 0x80186508: sw          $t5, -0x8($t1)
    MEM_W(-0X8, ctx->r9) = ctx->r13;
    // 0x8018650C: lw          $t3, -0x4($t4)
    ctx->r11 = MEM_W(ctx->r12, -0X4);
    // 0x80186510: bne         $t4, $t2, L_801864F4
    if (ctx->r12 != ctx->r10) {
        // 0x80186514: sw          $t3, -0x4($t1)
        MEM_W(-0X4, ctx->r9) = ctx->r11;
            goto L_801864F4;
    }
    // 0x80186514: sw          $t3, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->r11;
    // 0x80186518: lw          $t3, 0x0($t4)
    ctx->r11 = MEM_W(ctx->r12, 0X0);
    // 0x8018651C: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80186520: addiu       $t8, $t8, 0x667C
    ctx->r24 = ADD32(ctx->r24, 0X667C);
    // 0x80186524: or          $t6, $a0, $zero
    ctx->r14 = ctx->r4 | 0;
    // 0x80186528: addiu       $t9, $a0, 0x30
    ctx->r25 = ADD32(ctx->r4, 0X30);
    // 0x8018652C: sw          $t3, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r11;
L_80186530:
    // 0x80186530: lw          $t0, 0x0($t6)
    ctx->r8 = MEM_W(ctx->r14, 0X0);
    // 0x80186534: addiu       $t6, $t6, 0xC
    ctx->r14 = ADD32(ctx->r14, 0XC);
    // 0x80186538: addiu       $t8, $t8, 0xC
    ctx->r24 = ADD32(ctx->r24, 0XC);
    // 0x8018653C: sw          $t0, -0xC($t8)
    MEM_W(-0XC, ctx->r24) = ctx->r8;
    // 0x80186540: lw          $t7, -0x8($t6)
    ctx->r15 = MEM_W(ctx->r14, -0X8);
    // 0x80186544: sw          $t7, -0x8($t8)
    MEM_W(-0X8, ctx->r24) = ctx->r15;
    // 0x80186548: lw          $t0, -0x4($t6)
    ctx->r8 = MEM_W(ctx->r14, -0X4);
    // 0x8018654C: bne         $t6, $t9, L_80186530
    if (ctx->r14 != ctx->r25) {
        // 0x80186550: sw          $t0, -0x4($t8)
        MEM_W(-0X4, ctx->r24) = ctx->r8;
            goto L_80186530;
    }
    // 0x80186550: sw          $t0, -0x4($t8)
    MEM_W(-0X4, ctx->r24) = ctx->r8;
    // 0x80186554: lw          $t0, 0x0($t6)
    ctx->r8 = MEM_W(ctx->r14, 0X0);
    // 0x80186558: or          $t1, $a0, $zero
    ctx->r9 = ctx->r4 | 0;
    // 0x8018655C: or          $t5, $a1, $zero
    ctx->r13 = ctx->r5 | 0;
    // 0x80186560: addiu       $t3, $a0, 0x30
    ctx->r11 = ADD32(ctx->r4, 0X30);
    // 0x80186564: sw          $t0, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->r8;
L_80186568:
    // 0x80186568: lw          $t4, 0x0($t1)
    ctx->r12 = MEM_W(ctx->r9, 0X0);
    // 0x8018656C: addiu       $t1, $t1, 0xC
    ctx->r9 = ADD32(ctx->r9, 0XC);
    // 0x80186570: addiu       $t5, $t5, 0xC
    ctx->r13 = ADD32(ctx->r13, 0XC);
    // 0x80186574: sw          $t4, -0xC($t5)
    MEM_W(-0XC, ctx->r13) = ctx->r12;
    // 0x80186578: lw          $t2, -0x8($t1)
    ctx->r10 = MEM_W(ctx->r9, -0X8);
    // 0x8018657C: sw          $t2, -0x8($t5)
    MEM_W(-0X8, ctx->r13) = ctx->r10;
    // 0x80186580: lw          $t4, -0x4($t1)
    ctx->r12 = MEM_W(ctx->r9, -0X4);
    // 0x80186584: bne         $t1, $t3, L_80186568
    if (ctx->r9 != ctx->r11) {
        // 0x80186588: sw          $t4, -0x4($t5)
        MEM_W(-0X4, ctx->r13) = ctx->r12;
            goto L_80186568;
    }
    // 0x80186588: sw          $t4, -0x4($t5)
    MEM_W(-0X4, ctx->r13) = ctx->r12;
    // 0x8018658C: lw          $t4, 0x0($t1)
    ctx->r12 = MEM_W(ctx->r9, 0X0);
    // 0x80186590: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80186594: addiu       $t9, $t9, 0x6648
    ctx->r25 = ADD32(ctx->r25, 0X6648);
    // 0x80186598: or          $t7, $a1, $zero
    ctx->r15 = ctx->r5 | 0;
    // 0x8018659C: addiu       $t0, $a1, 0x30
    ctx->r8 = ADD32(ctx->r5, 0X30);
    // 0x801865A0: sw          $t4, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->r12;
L_801865A4:
    // 0x801865A4: lw          $t8, 0x0($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X0);
    // 0x801865A8: addiu       $t7, $t7, 0xC
    ctx->r15 = ADD32(ctx->r15, 0XC);
    // 0x801865AC: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x801865B0: sw          $t8, -0xC($t9)
    MEM_W(-0XC, ctx->r25) = ctx->r24;
    // 0x801865B4: lw          $t6, -0x8($t7)
    ctx->r14 = MEM_W(ctx->r15, -0X8);
    // 0x801865B8: sw          $t6, -0x8($t9)
    MEM_W(-0X8, ctx->r25) = ctx->r14;
    // 0x801865BC: lw          $t8, -0x4($t7)
    ctx->r24 = MEM_W(ctx->r15, -0X4);
    // 0x801865C0: bne         $t7, $t0, L_801865A4
    if (ctx->r15 != ctx->r8) {
        // 0x801865C4: sw          $t8, -0x4($t9)
        MEM_W(-0X4, ctx->r25) = ctx->r24;
            goto L_801865A4;
    }
    // 0x801865C4: sw          $t8, -0x4($t9)
    MEM_W(-0X4, ctx->r25) = ctx->r24;
    // 0x801865C8: lw          $t8, 0x0($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X0);
    // 0x801865CC: or          $t5, $a1, $zero
    ctx->r13 = ctx->r5 | 0;
    // 0x801865D0: or          $t2, $a2, $zero
    ctx->r10 = ctx->r6 | 0;
    // 0x801865D4: addiu       $t4, $a1, 0x30
    ctx->r12 = ADD32(ctx->r5, 0X30);
    // 0x801865D8: sw          $t8, 0x0($t9)
    MEM_W(0X0, ctx->r25) = ctx->r24;
L_801865DC:
    // 0x801865DC: lw          $t1, 0x0($t5)
    ctx->r9 = MEM_W(ctx->r13, 0X0);
    // 0x801865E0: addiu       $t5, $t5, 0xC
    ctx->r13 = ADD32(ctx->r13, 0XC);
    // 0x801865E4: addiu       $t2, $t2, 0xC
    ctx->r10 = ADD32(ctx->r10, 0XC);
    // 0x801865E8: sw          $t1, -0xC($t2)
    MEM_W(-0XC, ctx->r10) = ctx->r9;
    // 0x801865EC: lw          $t3, -0x8($t5)
    ctx->r11 = MEM_W(ctx->r13, -0X8);
    // 0x801865F0: sw          $t3, -0x8($t2)
    MEM_W(-0X8, ctx->r10) = ctx->r11;
    // 0x801865F4: lw          $t1, -0x4($t5)
    ctx->r9 = MEM_W(ctx->r13, -0X4);
    // 0x801865F8: bne         $t5, $t4, L_801865DC
    if (ctx->r13 != ctx->r12) {
        // 0x801865FC: sw          $t1, -0x4($t2)
        MEM_W(-0X4, ctx->r10) = ctx->r9;
            goto L_801865DC;
    }
    // 0x801865FC: sw          $t1, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->r9;
    // 0x80186600: lw          $t1, 0x0($t5)
    ctx->r9 = MEM_W(ctx->r13, 0X0);
    // 0x80186604: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x80186608: addiu       $t0, $t0, 0x6614
    ctx->r8 = ADD32(ctx->r8, 0X6614);
    // 0x8018660C: or          $t6, $a2, $zero
    ctx->r14 = ctx->r6 | 0;
    // 0x80186610: addiu       $t8, $a2, 0x30
    ctx->r24 = ADD32(ctx->r6, 0X30);
    // 0x80186614: sw          $t1, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->r9;
L_80186618:
    // 0x80186618: lw          $t9, 0x0($t6)
    ctx->r25 = MEM_W(ctx->r14, 0X0);
    // 0x8018661C: addiu       $t6, $t6, 0xC
    ctx->r14 = ADD32(ctx->r14, 0XC);
    // 0x80186620: addiu       $t0, $t0, 0xC
    ctx->r8 = ADD32(ctx->r8, 0XC);
    // 0x80186624: sw          $t9, -0xC($t0)
    MEM_W(-0XC, ctx->r8) = ctx->r25;
    // 0x80186628: lw          $t7, -0x8($t6)
    ctx->r15 = MEM_W(ctx->r14, -0X8);
    // 0x8018662C: sw          $t7, -0x8($t0)
    MEM_W(-0X8, ctx->r8) = ctx->r15;
    // 0x80186630: lw          $t9, -0x4($t6)
    ctx->r25 = MEM_W(ctx->r14, -0X4);
    // 0x80186634: bne         $t6, $t8, L_80186618
    if (ctx->r14 != ctx->r24) {
        // 0x80186638: sw          $t9, -0x4($t0)
        MEM_W(-0X4, ctx->r8) = ctx->r25;
            goto L_80186618;
    }
    // 0x80186638: sw          $t9, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->r25;
    // 0x8018663C: lw          $t9, 0x0($t6)
    ctx->r25 = MEM_W(ctx->r14, 0X0);
    // 0x80186640: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80186644: addiu       $t4, $t4, 0x65E0
    ctx->r12 = ADD32(ctx->r12, 0X65E0);
    // 0x80186648: or          $t3, $a2, $zero
    ctx->r11 = ctx->r6 | 0;
    // 0x8018664C: addiu       $t1, $a2, 0x30
    ctx->r9 = ADD32(ctx->r6, 0X30);
    // 0x80186650: sw          $t9, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r25;
L_80186654:
    // 0x80186654: lw          $t2, 0x0($t3)
    ctx->r10 = MEM_W(ctx->r11, 0X0);
    // 0x80186658: addiu       $t3, $t3, 0xC
    ctx->r11 = ADD32(ctx->r11, 0XC);
    // 0x8018665C: addiu       $t4, $t4, 0xC
    ctx->r12 = ADD32(ctx->r12, 0XC);
    // 0x80186660: sw          $t2, -0xC($t4)
    MEM_W(-0XC, ctx->r12) = ctx->r10;
    // 0x80186664: lw          $t5, -0x8($t3)
    ctx->r13 = MEM_W(ctx->r11, -0X8);
    // 0x80186668: sw          $t5, -0x8($t4)
    MEM_W(-0X8, ctx->r12) = ctx->r13;
    // 0x8018666C: lw          $t2, -0x4($t3)
    ctx->r10 = MEM_W(ctx->r11, -0X4);
    // 0x80186670: bne         $t3, $t1, L_80186654
    if (ctx->r11 != ctx->r9) {
        // 0x80186674: sw          $t2, -0x4($t4)
        MEM_W(-0X4, ctx->r12) = ctx->r10;
            goto L_80186654;
    }
    // 0x80186674: sw          $t2, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->r10;
    // 0x80186678: lw          $t2, 0x0($t3)
    ctx->r10 = MEM_W(ctx->r11, 0X0);
    // 0x8018667C: addiu       $sp, $sp, 0x108
    ctx->r29 = ADD32(ctx->r29, 0X108);
    // 0x80186680: jr          $ra
    // 0x80186684: sw          $t2, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r10;
    return;
    // 0x80186684: sw          $t2, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r10;
;}
RECOMP_FUNC void func_80186688(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186688: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x8018668C: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80186690: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80186694: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80186698: addiu       $v1, $v1, 0x65EC
    ctx->r3 = ADD32(ctx->r3, 0X65EC);
    // 0x8018669C: addiu       $a0, $a0, 0x65E0
    ctx->r4 = ADD32(ctx->r4, 0X65E0);
    // 0x801866A0: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
L_801866A4:
    // 0x801866A4: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x801866A8: sltu        $at, $a0, $v1
    ctx->r1 = ctx->r4 < ctx->r3 ? 1 : 0;
    // 0x801866AC: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x801866B0: nop

    // 0x801866B4: bc1t        L_801866C0
    if (c1cs) {
        // 0x801866B8: nop
    
            goto L_801866C0;
    }
    // 0x801866B8: nop

    // 0x801866BC: ori         $v0, $v0, 0x1
    ctx->r2 = ctx->r2 | 0X1;
L_801866C0:
    // 0x801866C0: bnel        $at, $zero, L_801866A4
    if (ctx->r1 != 0) {
        // 0x801866C4: lwc1        $f4, 0x0($a0)
        ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
            goto L_801866A4;
    }
    goto skip_0;
    // 0x801866C4: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    skip_0:
    // 0x801866C8: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x801866CC: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x801866D0: addiu       $v1, $v1, 0x6620
    ctx->r3 = ADD32(ctx->r3, 0X6620);
    // 0x801866D4: addiu       $a0, $a0, 0x6614
    ctx->r4 = ADD32(ctx->r4, 0X6614);
    // 0x801866D8: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
L_801866DC:
    // 0x801866DC: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x801866E0: sltu        $at, $a0, $v1
    ctx->r1 = ctx->r4 < ctx->r3 ? 1 : 0;
    // 0x801866E4: c.eq.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl == ctx->f6.fl;
    // 0x801866E8: nop

    // 0x801866EC: bc1t        L_801866F8
    if (c1cs) {
        // 0x801866F0: nop
    
            goto L_801866F8;
    }
    // 0x801866F0: nop

    // 0x801866F4: ori         $v0, $v0, 0x2
    ctx->r2 = ctx->r2 | 0X2;
L_801866F8:
    // 0x801866F8: bnel        $at, $zero, L_801866DC
    if (ctx->r1 != 0) {
        // 0x801866FC: lwc1        $f6, 0x0($a0)
        ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
            goto L_801866DC;
    }
    goto skip_1;
    // 0x801866FC: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    skip_1:
    // 0x80186700: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80186704: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80186708: addiu       $v1, $v1, 0x6654
    ctx->r3 = ADD32(ctx->r3, 0X6654);
    // 0x8018670C: addiu       $a0, $a0, 0x6648
    ctx->r4 = ADD32(ctx->r4, 0X6648);
    // 0x80186710: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
L_80186714:
    // 0x80186714: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80186718: sltu        $at, $a0, $v1
    ctx->r1 = ctx->r4 < ctx->r3 ? 1 : 0;
    // 0x8018671C: c.eq.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl == ctx->f8.fl;
    // 0x80186720: nop

    // 0x80186724: bc1t        L_80186730
    if (c1cs) {
        // 0x80186728: nop
    
            goto L_80186730;
    }
    // 0x80186728: nop

    // 0x8018672C: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
L_80186730:
    // 0x80186730: bnel        $at, $zero, L_80186714
    if (ctx->r1 != 0) {
        // 0x80186734: lwc1        $f8, 0x0($a0)
        ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
            goto L_80186714;
    }
    goto skip_2;
    // 0x80186734: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    skip_2:
    // 0x80186738: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x8018673C: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80186740: addiu       $v1, $v1, 0x6688
    ctx->r3 = ADD32(ctx->r3, 0X6688);
    // 0x80186744: addiu       $a0, $a0, 0x667C
    ctx->r4 = ADD32(ctx->r4, 0X667C);
    // 0x80186748: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
L_8018674C:
    // 0x8018674C: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80186750: sltu        $at, $a0, $v1
    ctx->r1 = ctx->r4 < ctx->r3 ? 1 : 0;
    // 0x80186754: c.eq.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl == ctx->f10.fl;
    // 0x80186758: nop

    // 0x8018675C: bc1t        L_80186768
    if (c1cs) {
        // 0x80186760: nop
    
            goto L_80186768;
    }
    // 0x80186760: nop

    // 0x80186764: ori         $v0, $v0, 0x8
    ctx->r2 = ctx->r2 | 0X8;
L_80186768:
    // 0x80186768: bnel        $at, $zero, L_8018674C
    if (ctx->r1 != 0) {
        // 0x8018676C: lwc1        $f10, 0x0($a0)
        ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
            goto L_8018674C;
    }
    goto skip_3;
    // 0x8018676C: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
    skip_3:
    // 0x80186770: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80186774: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80186778: addiu       $v1, $v1, 0x66BC
    ctx->r3 = ADD32(ctx->r3, 0X66BC);
    // 0x8018677C: addiu       $a0, $a0, 0x66B0
    ctx->r4 = ADD32(ctx->r4, 0X66B0);
    // 0x80186780: lwc1        $f16, 0x0($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X0);
L_80186784:
    // 0x80186784: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80186788: c.eq.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl == ctx->f16.fl;
    // 0x8018678C: nop

    // 0x80186790: bc1t        L_8018679C
    if (c1cs) {
        // 0x80186794: nop
    
            goto L_8018679C;
    }
    // 0x80186794: nop

    // 0x80186798: ori         $v0, $v0, 0x10
    ctx->r2 = ctx->r2 | 0X10;
L_8018679C:
    // 0x8018679C: bnel        $a0, $v1, L_80186784
    if (ctx->r4 != ctx->r3) {
        // 0x801867A0: lwc1        $f16, 0x0($a0)
        ctx->f16.u32l = MEM_W(ctx->r4, 0X0);
            goto L_80186784;
    }
    goto skip_4;
    // 0x801867A0: lwc1        $f16, 0x0($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X0);
    skip_4:
    // 0x801867A4: addiu       $at, $zero, 0x1F
    ctx->r1 = ADD32(0, 0X1F);
    // 0x801867A8: bnel        $v0, $at, L_801867BC
    if (ctx->r2 != ctx->r1) {
        // 0x801867AC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_801867BC;
    }
    goto skip_5;
    // 0x801867AC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_5:
    // 0x801867B0: jr          $ra
    // 0x801867B4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x801867B4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801867B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801867BC:
    // 0x801867BC: jr          $ra
    // 0x801867C0: nop

    return;
    // 0x801867C0: nop

;}
RECOMP_FUNC void func_801867C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801867C4: addiu       $sp, $sp, -0x2E8
    ctx->r29 = ADD32(ctx->r29, -0X2E8);
    // 0x801867C8: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x801867CC: sw          $s7, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r23;
    // 0x801867D0: sw          $s6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r22;
    // 0x801867D4: sw          $s5, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r21;
    // 0x801867D8: sw          $s4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r20;
    // 0x801867DC: sw          $s3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r19;
    // 0x801867E0: sw          $s2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r18;
    // 0x801867E4: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x801867E8: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x801867EC: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x801867F0: sw          $a1, 0x2EC($sp)
    MEM_W(0X2EC, ctx->r29) = ctx->r5;
    // 0x801867F4: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x801867F8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801867FC: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x80186800: or          $s7, $a3, $zero
    ctx->r23 = ctx->r7 | 0;
    // 0x80186804: lwc1        $f20, -0x30D8($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0X30D8);
    // 0x80186808: lw          $a2, 0x2EC($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X2EC);
    // 0x8018680C: addiu       $a0, $sp, 0x80
    ctx->r4 = ADD32(ctx->r29, 0X80);
    // 0x80186810: jal         0x80013CC4
    // 0x80186814: sw          $t6, 0x2E4($sp)
    MEM_W(0X2E4, ctx->r29) = ctx->r14;
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x80186814: sw          $t6, 0x2E4($sp)
    MEM_W(0X2E4, ctx->r29) = ctx->r14;
    after_0:
    // 0x80186818: addiu       $s3, $sp, 0x8C
    ctx->r19 = ADD32(ctx->r29, 0X8C);
    // 0x8018681C: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x80186820: lw          $a0, 0x2EC($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X2EC);
    // 0x80186824: addiu       $a1, $sp, 0x80
    ctx->r5 = ADD32(ctx->r29, 0X80);
    // 0x80186828: jal         0x8003E2F0
    // 0x8018682C: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    func_8003E2F0(rdram, ctx);
        goto after_1;
    // 0x8018682C: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    after_1:
    // 0x80186830: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80186834: addiu       $s2, $sp, 0x8C
    ctx->r18 = ADD32(ctx->r29, 0X8C);
    // 0x80186838: addiu       $s6, $zero, 0x3
    ctx->r22 = ADD32(0, 0X3);
    // 0x8018683C: addiu       $s5, $zero, 0x60
    ctx->r21 = ADD32(0, 0X60);
    // 0x80186840: lui         $s4, 0x2000
    ctx->r20 = S32(0X2000 << 16);
L_80186844:
    // 0x80186844: lw          $t7, 0x0($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X0);
    // 0x80186848: and         $t8, $t7, $s4
    ctx->r24 = ctx->r15 & ctx->r20;
    // 0x8018684C: beql        $t8, $zero, L_8018689C
    if (ctx->r24 == 0) {
        // 0x80186850: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_8018689C;
    }
    goto skip_0;
    // 0x80186850: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_0:
    // 0x80186854: multu       $s1, $s5
    result = U64(U32(ctx->r17)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80186858: mflo        $t9
    ctx->r25 = lo;
    // 0x8018685C: addu        $s0, $s3, $t9
    ctx->r16 = ADD32(ctx->r19, ctx->r25);
    // 0x80186860: lw          $a0, 0xC($s0)
    ctx->r4 = MEM_W(ctx->r16, 0XC);
    // 0x80186864: jal         0x801855FC
    // 0x80186868: lw          $a1, 0x18($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X18);
    func_801855FC(rdram, ctx);
        goto after_2;
    // 0x80186868: lw          $a1, 0x18($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X18);
    after_2:
    // 0x8018686C: beql        $v0, $zero, L_8018689C
    if (ctx->r2 == 0) {
        // 0x80186870: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_8018689C;
    }
    goto skip_1;
    // 0x80186870: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_1:
    // 0x80186874: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x80186878: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x8018687C: addiu       $a1, $s0, 0x48
    ctx->r5 = ADD32(ctx->r16, 0X48);
    // 0x80186880: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x80186884: nop

    // 0x80186888: bc1fl       L_8018689C
    if (!c1cs) {
        // 0x8018688C: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_8018689C;
    }
    goto skip_2;
    // 0x8018688C: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_2:
    // 0x80186890: jal         0x80013CF8
    // 0x80186894: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
    func_80013CF8(rdram, ctx);
        goto after_3;
    // 0x80186894: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
    after_3:
    // 0x80186898: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_8018689C:
    // 0x8018689C: bne         $s1, $s6, L_80186844
    if (ctx->r17 != ctx->r22) {
        // 0x801868A0: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_80186844;
    }
    // 0x801868A0: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x801868A4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801868A8: lwc1        $f4, -0x30D4($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X30D4);
    // 0x801868AC: lw          $t0, 0x2E4($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X2E4);
    // 0x801868B0: c.eq.s      $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f20.fl == ctx->f4.fl;
    // 0x801868B4: nop

    // 0x801868B8: bc1fl       L_801868D0
    if (!c1cs) {
        // 0x801868BC: lw          $t1, 0xC($t0)
        ctx->r9 = MEM_W(ctx->r8, 0XC);
            goto L_801868D0;
    }
    goto skip_3;
    // 0x801868BC: lw          $t1, 0xC($t0)
    ctx->r9 = MEM_W(ctx->r8, 0XC);
    skip_3:
    // 0x801868C0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801868C4: b           L_80186998
    // 0x801868C8: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
        goto L_80186998;
    // 0x801868C8: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x801868CC: lw          $t1, 0xC($t0)
    ctx->r9 = MEM_W(ctx->r8, 0XC);
L_801868D0:
    // 0x801868D0: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x801868D4: addiu       $a3, $sp, 0x74
    ctx->r7 = ADD32(ctx->r29, 0X74);
    // 0x801868D8: lw          $v0, 0x3C($t1)
    ctx->r2 = MEM_W(ctx->r9, 0X3C);
    // 0x801868DC: lwc1        $f12, 0x8($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801868E0: jal         0x801854D4
    // 0x801868E4: lwc1        $f14, 0x0($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X0);
    func_801854D4(rdram, ctx);
        goto after_4;
    // 0x801868E4: lwc1        $f14, 0x0($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X0);
    after_4:
    // 0x801868E8: lwc1        $f6, 0x74($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X74);
    // 0x801868EC: addiu       $s0, $sp, 0x50
    ctx->r16 = ADD32(ctx->r29, 0X50);
    // 0x801868F0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801868F4: c.lt.s      $f6, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f6.fl < ctx->f20.fl;
    // 0x801868F8: lw          $a1, 0x2EC($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2EC);
    // 0x801868FC: bc1f        L_80186978
    if (!c1cs) {
        // 0x80186900: nop
    
            goto L_80186978;
    }
    // 0x80186900: nop

    // 0x80186904: addiu       $s0, $sp, 0x68
    ctx->r16 = ADD32(ctx->r29, 0X68);
    // 0x80186908: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8018690C: jal         0x80013DE4
    // 0x80186910: addiu       $a1, $sp, 0x80
    ctx->r5 = ADD32(ctx->r29, 0X80);
    func_80013DE4(rdram, ctx);
        goto after_5;
    // 0x80186910: addiu       $a1, $sp, 0x80
    ctx->r5 = ADD32(ctx->r29, 0X80);
    after_5:
    // 0x80186914: lwc1        $f8, 0x74($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X74);
    // 0x80186918: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018691C: ldc1        $f16, -0x30D0($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, -0X30D0);
    // 0x80186920: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80186924: addiu       $s1, $sp, 0x5C
    ctx->r17 = ADD32(ctx->r29, 0X5C);
    // 0x80186928: add.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = ctx->f10.d + ctx->f16.d;
    // 0x8018692C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80186930: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80186934: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x80186938: mfc1        $a2, $f4
    ctx->r6 = (int32_t)ctx->f4.u32l;
    // 0x8018693C: jal         0x80013D58
    // 0x80186940: nop

    func_80013D58(rdram, ctx);
        goto after_6;
    // 0x80186940: nop

    after_6:
    // 0x80186944: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x80186948: or          $a1, $s7, $zero
    ctx->r5 = ctx->r23 | 0;
    // 0x8018694C: jal         0x80013CC4
    // 0x80186950: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_80013CC4(rdram, ctx);
        goto after_7;
    // 0x80186950: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_7:
    // 0x80186954: lwc1        $f6, 0x74($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X74);
    // 0x80186958: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018695C: ldc1        $f10, -0x30C8($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X30C8);
    // 0x80186960: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x80186964: cvt.d.s     $f18, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f18.d = CVT_D_S(ctx->f20.fl);
    // 0x80186968: add.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d + ctx->f10.d;
    // 0x8018696C: sub.d       $f4, $f18, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f16.d); 
    ctx->f4.d = ctx->f18.d - ctx->f16.d;
    // 0x80186970: b           L_80186990
    // 0x80186974: cvt.s.d     $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f20.fl = CVT_S_D(ctx->f4.d);
        goto L_80186990;
    // 0x80186974: cvt.s.d     $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f20.fl = CVT_S_D(ctx->f4.d);
L_80186978:
    // 0x80186978: jal         0x80013CC4
    // 0x8018697C: or          $a2, $s7, $zero
    ctx->r6 = ctx->r23 | 0;
    func_80013CC4(rdram, ctx);
        goto after_8;
    // 0x8018697C: or          $a2, $s7, $zero
    ctx->r6 = ctx->r23 | 0;
    after_8:
    // 0x80186980: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x80186984: lw          $a1, 0x2EC($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2EC);
    // 0x80186988: jal         0x80013C90
    // 0x8018698C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_80013C90(rdram, ctx);
        goto after_9;
    // 0x8018698C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_9:
L_80186990:
    // 0x80186990: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x80186994: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_80186998:
    // 0x80186998: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x8018699C: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x801869A0: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x801869A4: lw          $s2, 0x24($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X24);
    // 0x801869A8: lw          $s3, 0x28($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X28);
    // 0x801869AC: lw          $s4, 0x2C($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X2C);
    // 0x801869B0: lw          $s5, 0x30($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X30);
    // 0x801869B4: lw          $s6, 0x34($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X34);
    // 0x801869B8: lw          $s7, 0x38($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X38);
    // 0x801869BC: jr          $ra
    // 0x801869C0: addiu       $sp, $sp, 0x2E8
    ctx->r29 = ADD32(ctx->r29, 0X2E8);
    return;
    // 0x801869C0: addiu       $sp, $sp, 0x2E8
    ctx->r29 = ADD32(ctx->r29, 0X2E8);
;}
RECOMP_FUNC void func_801869C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801869C4: sub.s       $f4, $f12, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = ctx->f12.fl - ctx->f14.fl;
    // 0x801869C8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801869CC: ldc1        $f6, -0x30C0($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, -0X30C0);
    // 0x801869D0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801869D4: cvt.d.s     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f0.d = CVT_D_S(ctx->f4.fl);
    // 0x801869D8: c.lt.d      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.d < ctx->f0.d;
    // 0x801869DC: nop

    // 0x801869E0: bc1f        L_80186A00
    if (!c1cs) {
        // 0x801869E4: nop
    
            goto L_80186A00;
    }
    // 0x801869E4: nop

    // 0x801869E8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801869EC: ldc1        $f10, -0x30B8($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X30B8);
    // 0x801869F0: cvt.d.s     $f8, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f8.d = CVT_D_S(ctx->f12.fl);
    // 0x801869F4: sub.d       $f18, $f8, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = ctx->f8.d - ctx->f10.d;
    // 0x801869F8: b           L_80186A28
    // 0x801869FC: cvt.s.d     $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f12.fl = CVT_S_D(ctx->f18.d);
        goto L_80186A28;
    // 0x801869FC: cvt.s.d     $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f12.fl = CVT_S_D(ctx->f18.d);
L_80186A00:
    // 0x80186A00: ldc1        $f4, -0x30B0($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X30B0);
    // 0x80186A04: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80186A08: c.lt.d      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.d < ctx->f4.d;
    // 0x80186A0C: nop

    // 0x80186A10: bc1fl       L_80186A2C
    if (!c1cs) {
        // 0x80186A14: c.lt.s      $f14, $f12
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f14.fl < ctx->f12.fl;
            goto L_80186A2C;
    }
    goto skip_0;
    // 0x80186A14: c.lt.s      $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f14.fl < ctx->f12.fl;
    skip_0:
    // 0x80186A18: ldc1        $f8, -0x30A8($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X30A8);
    // 0x80186A1C: cvt.d.s     $f6, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f6.d = CVT_D_S(ctx->f12.fl);
    // 0x80186A20: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x80186A24: cvt.s.d     $f12, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f12.fl = CVT_S_D(ctx->f10.d);
L_80186A28:
    // 0x80186A28: c.lt.s      $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f14.fl < ctx->f12.fl;
L_80186A2C:
    // 0x80186A2C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80186A30: bc1fl       L_80186A48
    if (!c1cs) {
        // 0x80186A34: sub.s       $f0, $f14, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f12.fl;
            goto L_80186A48;
    }
    goto skip_1;
    // 0x80186A34: sub.s       $f0, $f14, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f12.fl;
    skip_1:
    // 0x80186A38: sub.s       $f0, $f14, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x80186A3C: b           L_80186A48
    // 0x80186A40: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
        goto L_80186A48;
    // 0x80186A40: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x80186A44: sub.s       $f0, $f14, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f12.fl;
L_80186A48:
    // 0x80186A48: ldc1        $f4, -0x30A0($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X30A0);
    // 0x80186A4C: cvt.d.s     $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f18.d = CVT_D_S(ctx->f0.fl);
    // 0x80186A50: c.lt.s      $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f14.fl < ctx->f12.fl;
    // 0x80186A54: mul.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x80186A58: lui         $at, 0xBF80
    ctx->r1 = S32(0XBF80 << 16);
    // 0x80186A5C: cvt.s.d     $f16, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f16.fl = CVT_S_D(ctx->f6.d);
    // 0x80186A60: bc1f        L_80186A78
    if (!c1cs) {
        // 0x80186A64: mov.s       $f2, $f16
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    ctx->f2.fl = ctx->f16.fl;
            goto L_80186A78;
    }
    // 0x80186A64: mov.s       $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    ctx->f2.fl = ctx->f16.fl;
    // 0x80186A68: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80186A6C: nop

    // 0x80186A70: mul.s       $f2, $f16, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x80186A74: nop

L_80186A78:
    // 0x80186A78: jr          $ra
    // 0x80186A7C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    return;
    // 0x80186A7C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
;}
RECOMP_FUNC void func_80186A80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186A80: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80186A84: lw          $t7, 0x48($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X48);
    // 0x80186A88: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80186A8C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x80186A90: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x80186A94: sw          $a2, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r6;
    // 0x80186A98: sw          $a3, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r7;
    // 0x80186A9C: beq         $t7, $zero, L_80186BE0
    if (ctx->r15 == 0) {
        // 0x80186AA0: lw          $v0, 0x34($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X34);
            goto L_80186BE0;
    }
    // 0x80186AA0: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80186AA4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80186AA8: lwc1        $f6, -0x3098($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3098);
    // 0x80186AAC: lwc1        $f4, 0x40($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80186AB0: addiu       $a0, $v0, 0x9C
    ctx->r4 = ADD32(ctx->r2, 0X9C);
    // 0x80186AB4: c.eq.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl == ctx->f6.fl;
    // 0x80186AB8: nop

    // 0x80186ABC: bc1tl       L_80186BE4
    if (c1cs) {
        // 0x80186AC0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80186BE4;
    }
    goto skip_0;
    // 0x80186AC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80186AC4: jal         0x8017FDE0
    // 0x80186AC8: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    func_8017FDE0(rdram, ctx);
        goto after_0;
    // 0x80186AC8: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    after_0:
    // 0x80186ACC: swc1        $f0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f0.u32l;
    // 0x80186AD0: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
    // 0x80186AD4: jal         0x801869C4
    // 0x80186AD8: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
    func_801869C4(rdram, ctx);
        goto after_1;
    // 0x80186AD8: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
    after_1:
    // 0x80186ADC: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80186AE0: jal         0x8017FE00
    // 0x80186AE4: swc1        $f0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f0.u32l;
    func_8017FE00(rdram, ctx);
        goto after_2;
    // 0x80186AE4: swc1        $f0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f0.u32l;
    after_2:
    // 0x80186AE8: lwc1        $f12, 0x28($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80186AEC: lwc1        $f8, 0x30($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80186AF0: lw          $t8, 0x48($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X48);
    // 0x80186AF4: lwc1        $f4, 0x40($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80186AF8: add.s       $f2, $f8, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = ctx->f8.fl + ctx->f12.fl;
    // 0x80186AFC: lwc1        $f10, 0x0($t8)
    ctx->f10.u32l = MEM_W(ctx->r24, 0X0);
    // 0x80186B00: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80186B04: add.s       $f16, $f2, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f2.fl + ctx->f10.fl;
    // 0x80186B08: add.s       $f18, $f0, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f0.fl + ctx->f16.fl;
    // 0x80186B0C: c.lt.s      $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f18.fl < ctx->f4.fl;
    // 0x80186B10: nop

    // 0x80186B14: bc1fl       L_80186B58
    if (!c1cs) {
        // 0x80186B18: swc1        $f2, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->f2.u32l;
            goto L_80186B58;
    }
    goto skip_1;
    // 0x80186B18: swc1        $f2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f2.u32l;
    skip_1:
    // 0x80186B1C: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80186B20: swc1        $f2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f2.u32l;
    // 0x80186B24: jal         0x8017FE00
    // 0x80186B28: swc1        $f12, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f12.u32l;
    func_8017FE00(rdram, ctx);
        goto after_3;
    // 0x80186B28: swc1        $f12, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f12.u32l;
    after_3:
    // 0x80186B2C: lw          $v0, 0x48($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X48);
    // 0x80186B30: lwc1        $f2, 0x18($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X18);
    // 0x80186B34: lwc1        $f16, 0x40($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80186B38: lwc1        $f6, 0x0($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80186B3C: lwc1        $f12, 0x28($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80186B40: add.s       $f8, $f2, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f2.fl + ctx->f6.fl;
    // 0x80186B44: add.s       $f10, $f0, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f0.fl + ctx->f8.fl;
    // 0x80186B48: sub.s       $f14, $f10, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x80186B4C: b           L_80186B80
    // 0x80186B50: neg.s       $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = -ctx->f14.fl;
        goto L_80186B80;
    // 0x80186B50: neg.s       $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = -ctx->f14.fl;
    // 0x80186B54: swc1        $f2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f2.u32l;
L_80186B58:
    // 0x80186B58: jal         0x8017FE00
    // 0x80186B5C: swc1        $f12, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f12.u32l;
    func_8017FE00(rdram, ctx);
        goto after_4;
    // 0x80186B5C: swc1        $f12, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f12.u32l;
    after_4:
    // 0x80186B60: lw          $v0, 0x48($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X48);
    // 0x80186B64: lwc1        $f2, 0x18($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X18);
    // 0x80186B68: lwc1        $f8, 0x40($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80186B6C: lwc1        $f18, 0x0($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80186B70: lwc1        $f12, 0x28($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80186B74: add.s       $f4, $f2, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f2.fl + ctx->f18.fl;
    // 0x80186B78: add.s       $f6, $f0, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f0.fl + ctx->f4.fl;
    // 0x80186B7C: sub.s       $f14, $f6, $f8
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f14.fl = ctx->f6.fl - ctx->f8.fl;
L_80186B80:
    // 0x80186B80: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80186B84: lwc1        $f10, 0x40($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80186B88: lwc1        $f16, 0x30($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80186B8C: c.lt.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl < ctx->f2.fl;
    // 0x80186B90: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x80186B94: bc1fl       L_80186BA8
    if (!c1cs) {
        // 0x80186B98: mov.s       $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
            goto L_80186BA8;
    }
    goto skip_2;
    // 0x80186B98: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    skip_2:
    // 0x80186B9C: b           L_80186BA8
    // 0x80186BA0: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
        goto L_80186BA8;
    // 0x80186BA0: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
    // 0x80186BA4: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_80186BA8:
    // 0x80186BA8: c.lt.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl < ctx->f0.fl;
    // 0x80186BAC: nop

    // 0x80186BB0: bc1fl       L_80186BC8
    if (!c1cs) {
        // 0x80186BB4: swc1        $f12, 0x0($v0)
        MEM_W(0X0, ctx->r2) = ctx->f12.u32l;
            goto L_80186BC8;
    }
    goto skip_3;
    // 0x80186BB4: swc1        $f12, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f12.u32l;
    skip_3:
    // 0x80186BB8: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x80186BBC: b           L_80186BC8
    // 0x80186BC0: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
        goto L_80186BC8;
    // 0x80186BC0: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x80186BC4: swc1        $f12, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f12.u32l;
L_80186BC8:
    // 0x80186BC8: c.eq.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl == ctx->f2.fl;
    // 0x80186BCC: nop

    // 0x80186BD0: bc1tl       L_80186BE4
    if (c1cs) {
        // 0x80186BD4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80186BE4;
    }
    goto skip_4;
    // 0x80186BD4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_4:
    // 0x80186BD8: jal         0x80186D94
    // 0x80186BDC: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    func_80186D94(rdram, ctx);
        goto after_5;
    // 0x80186BDC: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    after_5:
L_80186BE0:
    // 0x80186BE0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80186BE4:
    // 0x80186BE4: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80186BE8: jr          $ra
    // 0x80186BEC: nop

    return;
    // 0x80186BEC: nop

;}
RECOMP_FUNC void func_80186BF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186BF0: addiu       $sp, $sp, -0x78
    ctx->r29 = ADD32(ctx->r29, -0X78);
    // 0x80186BF4: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80186BF8: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80186BFC: lhu         $t7, 0x52C($v1)
    ctx->r15 = MEM_HU(ctx->r3, 0X52C);
    // 0x80186C00: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80186C04: sw          $s0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r16;
    // 0x80186C08: sw          $a0, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r4;
    // 0x80186C0C: sw          $a1, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r5;
    // 0x80186C10: sw          $a2, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r6;
    // 0x80186C14: andi        $t8, $t7, 0x3
    ctx->r24 = ctx->r15 & 0X3;
    // 0x80186C18: bne         $t8, $zero, L_80186D1C
    if (ctx->r24 != 0) {
        // 0x80186C1C: lw          $s0, 0x34($a0)
        ctx->r16 = MEM_W(ctx->r4, 0X34);
            goto L_80186D1C;
    }
    // 0x80186C1C: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x80186C20: lw          $t9, 0x2908($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X2908);
    // 0x80186C24: sll         $t0, $t9, 3
    ctx->r8 = S32(ctx->r25 << 3);
    // 0x80186C28: bgezl       $t0, L_80186D60
    if (SIGNED(ctx->r8) >= 0) {
        // 0x80186C2C: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80186D60;
    }
    goto skip_0;
    // 0x80186C2C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_0:
    // 0x80186C30: lw          $t1, 0x98($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X98);
    // 0x80186C34: srl         $t2, $t1, 31
    ctx->r10 = S32(U32(ctx->r9) >> 31);
    // 0x80186C38: bnel        $t2, $zero, L_80186D60
    if (ctx->r10 != 0) {
        // 0x80186C3C: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80186D60;
    }
    goto skip_1;
    // 0x80186C3C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_1:
    // 0x80186C40: lw          $t3, 0x0($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X0);
    // 0x80186C44: bnel        $t3, $zero, L_80186D60
    if (ctx->r11 != 0) {
        // 0x80186C48: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80186D60;
    }
    goto skip_2;
    // 0x80186C48: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_2:
    // 0x80186C4C: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x80186C50: addiu       $at, $zero, 0x30
    ctx->r1 = ADD32(0, 0X30);
    // 0x80186C54: beq         $v0, $at, L_80186C64
    if (ctx->r2 == ctx->r1) {
        // 0x80186C58: addiu       $at, $zero, 0x31
        ctx->r1 = ADD32(0, 0X31);
            goto L_80186C64;
    }
    // 0x80186C58: addiu       $at, $zero, 0x31
    ctx->r1 = ADD32(0, 0X31);
    // 0x80186C5C: bnel        $v0, $at, L_80186D60
    if (ctx->r2 != ctx->r1) {
        // 0x80186C60: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80186D60;
    }
    goto skip_3;
    // 0x80186C60: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_3:
L_80186C64:
    // 0x80186C64: jal         0x80186DAC
    // 0x80186C68: lw          $a0, 0x78($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X78);
    func_80186DAC(rdram, ctx);
        goto after_0;
    // 0x80186C68: lw          $a0, 0x78($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X78);
    after_0:
    // 0x80186C6C: addiu       $a0, $sp, 0x68
    ctx->r4 = ADD32(ctx->r29, 0X68);
    // 0x80186C70: addiu       $a1, $s0, 0x18
    ctx->r5 = ADD32(ctx->r16, 0X18);
    // 0x80186C74: jal         0x80013C90
    // 0x80186C78: addiu       $a2, $s0, 0x48
    ctx->r6 = ADD32(ctx->r16, 0X48);
    func_80013C90(rdram, ctx);
        goto after_1;
    // 0x80186C78: addiu       $a2, $s0, 0x48
    ctx->r6 = ADD32(ctx->r16, 0X48);
    after_1:
    // 0x80186C7C: addiu       $a0, $sp, 0x5C
    ctx->r4 = ADD32(ctx->r29, 0X5C);
    // 0x80186C80: addiu       $a1, $sp, 0x68
    ctx->r5 = ADD32(ctx->r29, 0X68);
    // 0x80186C84: jal         0x80013CC4
    // 0x80186C88: addiu       $a2, $s0, 0x9C
    ctx->r6 = ADD32(ctx->r16, 0X9C);
    func_80013CC4(rdram, ctx);
        goto after_2;
    // 0x80186C88: addiu       $a2, $s0, 0x9C
    ctx->r6 = ADD32(ctx->r16, 0X9C);
    after_2:
    // 0x80186C8C: jal         0x80013BD0
    // 0x80186C90: addiu       $a0, $sp, 0x5C
    ctx->r4 = ADD32(ctx->r29, 0X5C);
    func_80013BD0(rdram, ctx);
        goto after_3;
    // 0x80186C90: addiu       $a0, $sp, 0x5C
    ctx->r4 = ADD32(ctx->r29, 0X5C);
    after_3:
    // 0x80186C94: lw          $t4, 0xC($s0)
    ctx->r12 = MEM_W(ctx->r16, 0XC);
    // 0x80186C98: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80186C9C: addiu       $a3, $sp, 0x54
    ctx->r7 = ADD32(ctx->r29, 0X54);
    // 0x80186CA0: lw          $v0, 0x3C($t4)
    ctx->r2 = MEM_W(ctx->r12, 0X3C);
    // 0x80186CA4: lwc1        $f12, 0x8($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80186CA8: jal         0x801854D4
    // 0x80186CAC: lwc1        $f14, 0x0($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X0);
    func_801854D4(rdram, ctx);
        goto after_4;
    // 0x80186CAC: lwc1        $f14, 0x0($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X0);
    after_4:
    // 0x80186CB0: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x80186CB4: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x80186CB8: swc1        $f0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f0.u32l;
    // 0x80186CBC: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    // 0x80186CC0: jal         0x80013CF8
    // 0x80186CC4: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_5;
    // 0x80186CC4: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_5:
    // 0x80186CC8: lui         $at, 0x4140
    ctx->r1 = S32(0X4140 << 16);
    // 0x80186CCC: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80186CD0: lui         $at, 0x4060
    ctx->r1 = S32(0X4060 << 16);
    // 0x80186CD4: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80186CD8: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x80186CDC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80186CE0: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    // 0x80186CE4: swc1        $f6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f6.u32l;
    // 0x80186CE8: swc1        $f8, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f8.u32l;
    // 0x80186CEC: lw          $t5, 0xC($s0)
    ctx->r13 = MEM_W(ctx->r16, 0XC);
    // 0x80186CF0: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80186CF4: addiu       $t7, $t7, 0x6730
    ctx->r15 = ADD32(ctx->r15, 0X6730);
    // 0x80186CF8: lw          $t6, 0x3C($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X3C);
    // 0x80186CFC: addiu       $a0, $sp, 0x68
    ctx->r4 = ADD32(ctx->r29, 0X68);
    // 0x80186D00: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    // 0x80186D04: lwc1        $f10, 0x8($t6)
    ctx->f10.u32l = MEM_W(ctx->r14, 0X8);
    // 0x80186D08: sw          $t7, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r15;
    // 0x80186D0C: lw          $a2, 0x54($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X54);
    // 0x80186D10: lw          $a3, 0x50($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X50);
    // 0x80186D14: jal         0x80185820
    // 0x80186D18: swc1        $f10, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f10.u32l;
    func_80185820(rdram, ctx);
        goto after_6;
    // 0x80186D18: swc1        $f10, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f10.u32l;
    after_6:
L_80186D1C:
    // 0x80186D1C: lw          $t8, 0x80($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X80);
    // 0x80186D20: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80186D24: addiu       $t9, $t9, 0x6730
    ctx->r25 = ADD32(ctx->r25, 0X6730);
    // 0x80186D28: bnel        $t8, $zero, L_80186D60
    if (ctx->r24 != 0) {
        // 0x80186D2C: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80186D60;
    }
    goto skip_4;
    // 0x80186D2C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_4:
    // 0x80186D30: lw          $t1, 0x0($t9)
    ctx->r9 = MEM_W(ctx->r25, 0X0);
    // 0x80186D34: lw          $t2, 0x7C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X7C);
    // 0x80186D38: lw          $a0, 0x78($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X78);
    // 0x80186D3C: sw          $t1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r9;
    // 0x80186D40: lw          $a2, 0x4($t9)
    ctx->r6 = MEM_W(ctx->r25, 0X4);
    // 0x80186D44: lw          $a1, 0x4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4);
    // 0x80186D48: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80186D4C: lw          $a3, 0x8($t9)
    ctx->r7 = MEM_W(ctx->r25, 0X8);
    // 0x80186D50: sw          $t2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r10;
    // 0x80186D54: jal         0x80186A80
    // 0x80186D58: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    func_80186A80(rdram, ctx);
        goto after_7;
    // 0x80186D58: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    after_7:
    // 0x80186D5C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
L_80186D60:
    // 0x80186D60: lw          $s0, 0x30($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X30);
    // 0x80186D64: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
    // 0x80186D68: jr          $ra
    // 0x80186D6C: nop

    return;
    // 0x80186D6C: nop

;}
RECOMP_FUNC void func_80186D70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186D70: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80186D74: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x80186D78: addiu       $v0, $v0, 0x6730
    ctx->r2 = ADD32(ctx->r2, 0X6730);
    // 0x80186D7C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80186D80: swc1        $f0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f0.u32l;
    // 0x80186D84: lwc1        $f4, -0x3094($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X3094);
    // 0x80186D88: swc1        $f0, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f0.u32l;
    // 0x80186D8C: jr          $ra
    // 0x80186D90: swc1        $f4, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f4.u32l;
    return;
    // 0x80186D90: swc1        $f4, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f4.u32l;
;}
RECOMP_FUNC void func_80186D94(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186D94: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80186D98: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80186D9C: lbu         $t6, 0x576($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X576);
    // 0x80186DA0: or          $t7, $t6, $a1
    ctx->r15 = ctx->r14 | ctx->r5;
    // 0x80186DA4: jr          $ra
    // 0x80186DA8: sb          $t7, 0x576($v0)
    MEM_B(0X576, ctx->r2) = ctx->r15;
    return;
    // 0x80186DA8: sb          $t7, 0x576($v0)
    MEM_B(0X576, ctx->r2) = ctx->r15;
;}
RECOMP_FUNC void func_80186DAC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186DAC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80186DB0: jr          $ra
    // 0x80186DB4: sb          $zero, 0x576($v0)
    MEM_B(0X576, ctx->r2) = 0;
    return;
    // 0x80186DB4: sb          $zero, 0x576($v0)
    MEM_B(0X576, ctx->r2) = 0;
;}
RECOMP_FUNC void func_80186DB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186DB8: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80186DBC: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80186DC0: andi        $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 & 0XFF;
    // 0x80186DC4: lbu         $t6, 0x576($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X576);
    // 0x80186DC8: and         $v0, $t6, $a1
    ctx->r2 = ctx->r14 & ctx->r5;
    // 0x80186DCC: jr          $ra
    // 0x80186DD0: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
    return;
    // 0x80186DD0: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
;}
RECOMP_FUNC void Interactable_Entrypoint(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80186DF0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80186DF4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80186DF8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80186DFC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80186E00: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80186E04: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80186E08: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80186E0C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80186E10: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80186E14: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80186E18: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80186E1C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80186E20: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80186E24: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80186E28: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80186E2C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80186E30: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80186E34: lw          $t9, 0x7110($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7110);
    // 0x80186E38: jalr        $t9
    // 0x80186E3C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80186E3C: nop

    after_0:
    // 0x80186E40: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80186E44: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80186E48: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80186E4C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80186E50: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80186E54: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80186E58: jr          $ra
    // 0x80186E5C: nop

    return;
    // 0x80186E5C: nop

;}
RECOMP_FUNC void Interactable_Init(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_NI0E_TRACE
    uint32_t ni0e_interact_parent = 0;
#endif
    // 0x80186E60: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80186E64: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80186E68: lw          $a3, 0x70($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X70);
    // 0x80186E6C: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80186E70: jal         0x80143234
    // 0x80186E74: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    func_80143234(rdram, ctx);
        goto after_0;
    // 0x80186E74: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_0:
    // 0x80186E78: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80186E7C: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x80186E80: beql        $a3, $zero, L_80186F40
    if (ctx->r7 == 0) {
        // 0x80186E84: lhu         $t5, 0x38($a2)
        ctx->r13 = MEM_HU(ctx->r6, 0X38);
            goto L_80186F40;
    }
    goto skip_0;
    // 0x80186E84: lhu         $t5, 0x38($a2)
    ctx->r13 = MEM_HU(ctx->r6, 0X38);
    skip_0:
    // 0x80186E88: lhu         $t6, 0x18($a3)
    ctx->r14 = MEM_HU(ctx->r7, 0X18);
    // 0x80186E8C: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x80186E90: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x80186E94: sh          $t7, 0x38($a2)
    MEM_H(0X38, ctx->r6) = ctx->r15;
    // 0x80186E98: lhu         $t8, 0x14($a3)
    ctx->r24 = MEM_HU(ctx->r7, 0X14);
    // 0x80186E9C: andi        $v0, $t7, 0xFFFF
    ctx->r2 = ctx->r15 & 0XFFFF;
    // 0x80186EA0: addiu       $t1, $v0, 0x1
    ctx->r9 = ADD32(ctx->r2, 0X1);
    // 0x80186EA4: sw          $t8, 0x58($a2)
    MEM_W(0X58, ctx->r6) = ctx->r24;
    // 0x80186EA8: lhu         $t9, 0x16($a3)
    ctx->r25 = MEM_HU(ctx->r7, 0X16);
    // 0x80186EAC: slti        $at, $t1, 0x31
    ctx->r1 = SIGNED(ctx->r9) < 0X31 ? 1 : 0;
    // 0x80186EB0: beq         $at, $zero, L_80186F48
    if (ctx->r1 == 0) {
        // 0x80186EB4: sh          $t9, 0x50($a2)
        MEM_H(0X50, ctx->r6) = ctx->r25;
            goto L_80186F48;
    }
    // 0x80186EB4: sh          $t9, 0x50($a2)
    MEM_H(0X50, ctx->r6) = ctx->r25;
    // 0x80186EB8: sll         $t2, $v0, 2
    ctx->r10 = S32(ctx->r2 << 2);
    // 0x80186EBC: addu        $t2, $t2, $v0
    ctx->r10 = ADD32(ctx->r10, ctx->r2);
    // 0x80186EC0: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80186EC4: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x80186EC8: lhu         $t3, 0x677A($t3)
    ctx->r11 = MEM_HU(ctx->r11, 0X677A);
    // 0x80186ECC: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80186ED0: beq         $t3, $at, L_80186F48
    if (ctx->r11 == ctx->r1) {
        // 0x80186ED4: nop
    
            goto L_80186F48;
    }
    // 0x80186ED4: nop

    // 0x80186ED8: lhu         $t4, 0x14($a3)
    ctx->r12 = MEM_HU(ctx->r7, 0X14);
    // 0x80186EDC: beq         $t4, $zero, L_80186F48
    if (ctx->r12 == 0) {
        // 0x80186EE0: nop
    
            goto L_80186F48;
    }
    // 0x80186EE0: nop

    // 0x80186EE4: beq         $t8, $zero, L_80186F1C
    if (ctx->r24 == 0) {
        // 0x80186EE8: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_80186F1C;
    }
    // 0x80186EE8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80186EEC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80186EF0: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80186EF4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80186EF8: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80186EFC: or          $a1, $t8, $zero
    ctx->r5 = ctx->r24 | 0;
    // 0x80186F00: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x80186F04: jalr        $t9
    // 0x80186F08: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80186F08: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_1:
    // 0x80186F0C: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x80186F10: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80186F14: b           L_80186F1C
    // 0x80186F18: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80186F1C;
    // 0x80186F18: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80186F1C:
    // 0x80186F1C: beq         $v1, $zero, L_80186F48
    if (ctx->r3 == 0) {
        // 0x80186F20: nop
    
            goto L_80186F48;
    }
    // 0x80186F20: nop

    // 0x80186F24: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    // 0x80186F28: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80186F2C: jalr        $t9
    // 0x80186F30: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80186F30: nop

    after_2:
    // 0x80186F34: b           L_80187558
    // 0x80186F38: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80187558;
    // 0x80186F38: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80186F3C: lhu         $t5, 0x38($a2)
    ctx->r13 = MEM_HU(ctx->r6, 0X38);
L_80186F40:
    // 0x80186F40: addiu       $t6, $t5, -0x1
    ctx->r14 = ADD32(ctx->r13, -0X1);
    // 0x80186F44: sh          $t6, 0x38($a2)
    MEM_H(0X38, ctx->r6) = ctx->r14;
L_80186F48:
    // 0x80186F48: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x80186F4C: lw          $t7, -0x53D8($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X53D8);
    // 0x80186F50: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80186F54: beql        $t7, $zero, L_80187558
    if (ctx->r15 == 0) {
        // 0x80186F58: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80187558;
    }
    goto skip_1;
    // 0x80186F58: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80186F5C: lhu         $v1, 0x38($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X38);
    // 0x80186F60: addiu       $a0, $v1, 0x1
    ctx->r4 = ADD32(ctx->r3, 0X1);
    // 0x80186F64: slti        $at, $a0, 0x31
    ctx->r1 = SIGNED(ctx->r4) < 0X31 ? 1 : 0;
    // 0x80186F68: beql        $at, $zero, L_80187450
    if (ctx->r1 == 0) {
        // 0x80186F6C: slti        $at, $a0, 0x31
        ctx->r1 = SIGNED(ctx->r4) < 0X31 ? 1 : 0;
            goto L_80187450;
    }
    goto skip_2;
    // 0x80186F6C: slti        $at, $a0, 0x31
    ctx->r1 = SIGNED(ctx->r4) < 0X31 ? 1 : 0;
    skip_2:
    // 0x80186F70: addiu       $a0, $zero, 0x210
    ctx->r4 = ADD32(0, 0X210);
    // 0x80186F74: lw          $a1, -0x1A08($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A08);
#if LOD_ENABLE_NI0E_TRACE
    ni0e_interact_parent = (uint32_t)ctx->r5;
#endif
    // 0x80186F78: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x80186F7C: jal         0x80005A30
    // 0x80186F80: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    sceneLookup(rdram, ctx);
        goto after_3;
    // 0x80186F80: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_3:
    // 0x80186F84: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x80186F88: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80186F8C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80186F90: beq         $v0, $zero, L_80187554
    if (ctx->r2 == 0) {
        // 0x80186F94: sw          $v0, 0x24($a2)
        MEM_W(0X24, ctx->r6) = ctx->r2;
#if LOD_ENABLE_NI0E_TRACE
        lod_ni0e_interact_node_probe(rdram, (uint32_t)ctx->r6, ni0e_interact_parent,
                                      (uint32_t)ctx->r2);
#endif
            goto L_80187554;
    }
    // 0x80186F94: sw          $v0, 0x24($a2)
    MEM_W(0X24, ctx->r6) = ctx->r2;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_interact_node_probe(rdram, (uint32_t)ctx->r6, ni0e_interact_parent,
                                  (uint32_t)ctx->r2);
#endif
    // 0x80186F98: beq         $a3, $zero, L_80186FD8
    if (ctx->r7 == 0) {
        // 0x80186F9C: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_80186FD8;
    }
    // 0x80186F9C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80186FA0: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    // 0x80186FA4: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x80186FA8: jal         0x801431AC
    // 0x80186FAC: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    func_801431AC(rdram, ctx);
        goto after_4;
    // 0x80186FAC: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_4:
    // 0x80186FB0: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x80186FB4: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x80186FB8: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80186FBC: lwc1        $f4, 0x50($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X50);
    // 0x80186FC0: swc1        $f4, 0x64($a2)
    MEM_W(0X64, ctx->r6) = ctx->f4.u32l;
    // 0x80186FC4: lwc1        $f6, 0x54($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X54);
    // 0x80186FC8: swc1        $f6, 0x68($a2)
    MEM_W(0X68, ctx->r6) = ctx->f6.u32l;
    // 0x80186FCC: lwc1        $f8, 0x58($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X58);
    // 0x80186FD0: b           L_80186FF0
    // 0x80186FD4: swc1        $f8, 0x40($a2)
    MEM_W(0X40, ctx->r6) = ctx->f8.u32l;
        goto L_80186FF0;
    // 0x80186FD4: swc1        $f8, 0x40($a2)
    MEM_W(0X40, ctx->r6) = ctx->f8.u32l;
L_80186FD8:
    // 0x80186FD8: lwc1        $f10, 0x64($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X64);
    // 0x80186FDC: swc1        $f10, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f10.u32l;
    // 0x80186FE0: lwc1        $f16, 0x68($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X68);
    // 0x80186FE4: swc1        $f16, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f16.u32l;
    // 0x80186FE8: lwc1        $f18, 0x40($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X40);
    // 0x80186FEC: swc1        $f18, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f18.u32l;
L_80186FF0:
    // 0x80186FF0: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x80186FF4: lw          $t8, -0x5228($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X5228);
    // 0x80186FF8: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80186FFC: sw          $t8, 0x40($a1)
    MEM_W(0X40, ctx->r5) = ctx->r24;
    // 0x80187000: lhu         $t1, 0x38($a2)
    ctx->r9 = MEM_HU(ctx->r6, 0X38);
    // 0x80187004: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    // 0x80187008: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x8018700C: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80187010: addu        $t2, $t2, $t1
    ctx->r10 = ADD32(ctx->r10, ctx->r9);
    // 0x80187014: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80187018: addu        $a0, $a0, $t2
    ctx->r4 = ADD32(ctx->r4, ctx->r10);
    // 0x8018701C: lhu         $a0, 0x677A($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X677A);
    // 0x80187020: jal         0x8008F5E0
    // 0x80187024: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    func_8008F5E0(rdram, ctx);
        goto after_5;
    // 0x80187024: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    after_5:
    // 0x80187028: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x8018702C: lbu         $t3, 0xD($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0XD);
    // 0x80187030: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x80187034: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80187038: sh          $t3, 0x56($a2)
    MEM_H(0X56, ctx->r6) = ctx->r11;
    // 0x8018703C: lbu         $t4, 0x8($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X8);
    // 0x80187040: addiu       $at, $zero, 0xFF
    ctx->r1 = ADD32(0, 0XFF);
    // 0x80187044: beql        $t4, $at, L_80187074
    if (ctx->r12 == ctx->r1) {
        // 0x80187048: lw          $t8, 0x0($v0)
        ctx->r24 = MEM_W(ctx->r2, 0X0);
            goto L_80187074;
    }
    goto skip_3;
    // 0x80187048: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    skip_3:
    // 0x8018704C: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X0);
    // 0x80187050: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80187054: or          $t5, $t9, $at
    ctx->r13 = ctx->r25 | ctx->r1;
    // 0x80187058: sw          $t5, 0x3C($a1)
    MEM_W(0X3C, ctx->r5) = ctx->r13;
    // 0x8018705C: lbu         $t6, 0x8($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X8);
    // 0x80187060: sh          $t6, 0x28($a1)
    MEM_H(0X28, ctx->r5) = ctx->r14;
    // 0x80187064: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x80187068: b           L_80187078
    // 0x8018706C: sh          $t7, 0x2A($a1)
    MEM_H(0X2A, ctx->r5) = ctx->r15;
        goto L_80187078;
    // 0x8018706C: sh          $t7, 0x2A($a1)
    MEM_H(0X2A, ctx->r5) = ctx->r15;
    // 0x80187070: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
L_80187074:
    // 0x80187074: sw          $t8, 0x3C($a1)
    MEM_W(0X3C, ctx->r5) = ctx->r24;
L_80187078:
    // 0x80187078: lhu         $t1, 0x38($a2)
    ctx->r9 = MEM_HU(ctx->r6, 0X38);
    // 0x8018707C: addiu       $a0, $zero, 0x14
    ctx->r4 = ADD32(0, 0X14);
    // 0x80187080: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x80187084: multu       $t1, $a0
    result = U64(U32(ctx->r9)) * U64(U32(ctx->r4)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80187088: addiu       $t0, $t0, 0x6778
    ctx->r8 = ADD32(ctx->r8, 0X6778);
    // 0x8018708C: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x80187090: mflo        $t2
    ctx->r10 = lo;
    // 0x80187094: addu        $t3, $t0, $t2
    ctx->r11 = ADD32(ctx->r8, ctx->r10);
    // 0x80187098: lhu         $t4, 0x2($t3)
    ctx->r12 = MEM_HU(ctx->r11, 0X2);
    // 0x8018709C: beql        $t4, $at, L_801870C0
    if (ctx->r12 == ctx->r1) {
        // 0x801870A0: lhu         $v1, 0x50($a2)
        ctx->r3 = MEM_HU(ctx->r6, 0X50);
            goto L_801870C0;
    }
    goto skip_4;
    // 0x801870A0: lhu         $v1, 0x50($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X50);
    skip_4:
    // 0x801870A4: lhu         $v1, 0x50($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X50);
    // 0x801870A8: andi        $t9, $v1, 0x1000
    ctx->r25 = ctx->r3 & 0X1000;
    // 0x801870AC: bne         $t9, $zero, L_801870BC
    if (ctx->r25 != 0) {
        // 0x801870B0: andi        $t5, $v1, 0x800
        ctx->r13 = ctx->r3 & 0X800;
            goto L_801870BC;
    }
    // 0x801870B0: andi        $t5, $v1, 0x800
    ctx->r13 = ctx->r3 & 0X800;
    // 0x801870B4: beq         $t5, $zero, L_801870C8
    if (ctx->r13 == 0) {
        // 0x801870B8: lui         $t6, 0x601
        ctx->r14 = S32(0X601 << 16);
            goto L_801870C8;
    }
    // 0x801870B8: lui         $t6, 0x601
    ctx->r14 = S32(0X601 << 16);
L_801870BC:
    // 0x801870BC: lhu         $v1, 0x50($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X50);
L_801870C0:
    // 0x801870C0: b           L_801870D8
    // 0x801870C4: andi        $v1, $v1, 0x800
    ctx->r3 = ctx->r3 & 0X800;
        goto L_801870D8;
    // 0x801870C4: andi        $v1, $v1, 0x800
    ctx->r3 = ctx->r3 & 0X800;
L_801870C8:
    // 0x801870C8: addiu       $t6, $t6, -0x7428
    ctx->r14 = ADD32(ctx->r14, -0X7428);
    // 0x801870CC: sw          $t6, 0x38($a1)
    MEM_W(0X38, ctx->r5) = ctx->r14;
    // 0x801870D0: lhu         $v1, 0x50($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X50);
    // 0x801870D4: andi        $v1, $v1, 0x800
    ctx->r3 = ctx->r3 & 0X800;
L_801870D8:
    // 0x801870D8: beql        $v1, $zero, L_801870F4
    if (ctx->r3 == 0) {
        // 0x801870DC: lhu         $v1, 0x56($a2)
        ctx->r3 = MEM_HU(ctx->r6, 0X56);
            goto L_801870F4;
    }
    goto skip_5;
    // 0x801870DC: lhu         $v1, 0x56($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X56);
    skip_5:
    // 0x801870E0: lh          $t7, 0x0($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X0);
    // 0x801870E4: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x801870E8: or          $t8, $t7, $at
    ctx->r24 = ctx->r15 | ctx->r1;
    // 0x801870EC: sh          $t8, 0x0($a1)
    MEM_H(0X0, ctx->r5) = ctx->r24;
    // 0x801870F0: lhu         $v1, 0x56($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X56);
L_801870F4:
    // 0x801870F4: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    // 0x801870F8: andi        $t1, $v1, 0x1
    ctx->r9 = ctx->r3 & 0X1;
    // 0x801870FC: beq         $t1, $zero, L_80187114
    if (ctx->r9 == 0) {
        // 0x80187100: andi        $t4, $v1, 0x80
        ctx->r12 = ctx->r3 & 0X80;
            goto L_80187114;
    }
    // 0x80187100: andi        $t4, $v1, 0x80
    ctx->r12 = ctx->r3 & 0X80;
    // 0x80187104: lhu         $t2, 0x2($a1)
    ctx->r10 = MEM_HU(ctx->r5, 0X2);
    // 0x80187108: ori         $t3, $t2, 0x800
    ctx->r11 = ctx->r10 | 0X800;
    // 0x8018710C: b           L_80187138
    // 0x80187110: sh          $t3, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r11;
        goto L_80187138;
    // 0x80187110: sh          $t3, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r11;
L_80187114:
    // 0x80187114: beql        $t4, $zero, L_80187130
    if (ctx->r12 == 0) {
        // 0x80187118: lhu         $t6, 0x2($a1)
        ctx->r14 = MEM_HU(ctx->r5, 0X2);
            goto L_80187130;
    }
    goto skip_6;
    // 0x80187118: lhu         $t6, 0x2($a1)
    ctx->r14 = MEM_HU(ctx->r5, 0X2);
    skip_6:
    // 0x8018711C: lhu         $t9, 0x2($a1)
    ctx->r25 = MEM_HU(ctx->r5, 0X2);
    // 0x80187120: ori         $t5, $t9, 0x800
    ctx->r13 = ctx->r25 | 0X800;
    // 0x80187124: b           L_80187138
    // 0x80187128: sh          $t5, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r13;
        goto L_80187138;
    // 0x80187128: sh          $t5, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r13;
    // 0x8018712C: lhu         $t6, 0x2($a1)
    ctx->r14 = MEM_HU(ctx->r5, 0X2);
L_80187130:
    // 0x80187130: ori         $t7, $t6, 0x840
    ctx->r15 = ctx->r14 | 0X840;
    // 0x80187134: sh          $t7, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r15;
L_80187138:
    // 0x80187138: sw          $t8, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->r24;
    // 0x8018713C: lhu         $t1, 0x38($a2)
    ctx->r9 = MEM_HU(ctx->r6, 0X38);
    // 0x80187140: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80187144: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x80187148: multu       $t1, $a0
    result = U64(U32(ctx->r9)) * U64(U32(ctx->r4)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8018714C: mflo        $t2
    ctx->r10 = lo;
    // 0x80187150: addu        $t3, $t0, $t2
    ctx->r11 = ADD32(ctx->r8, ctx->r10);
    // 0x80187154: lhu         $t4, 0x2($t3)
    ctx->r12 = MEM_HU(ctx->r11, 0X2);
    // 0x80187158: bnel        $t4, $at, L_80187178
    if (ctx->r12 != ctx->r1) {
        // 0x8018715C: lbu         $t6, 0xA($v0)
        ctx->r14 = MEM_BU(ctx->r2, 0XA);
            goto L_80187178;
    }
    goto skip_7;
    // 0x8018715C: lbu         $t6, 0xA($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0XA);
    skip_7:
    // 0x80187160: lh          $t9, -0x51E4($t9)
    ctx->r25 = MEM_H(ctx->r25, -0X51E4);
    // 0x80187164: addiu       $t5, $zero, -0xC0
    ctx->r13 = ADD32(0, -0XC0);
    // 0x80187168: bgezl       $t9, L_80187178
    if (SIGNED(ctx->r25) >= 0) {
        // 0x8018716C: lbu         $t6, 0xA($v0)
        ctx->r14 = MEM_BU(ctx->r2, 0XA);
            goto L_80187178;
    }
    goto skip_8;
    // 0x8018716C: lbu         $t6, 0xA($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0XA);
    skip_8:
    // 0x80187170: sw          $t5, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->r13;
    // 0x80187174: lbu         $t6, 0xA($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0XA);
L_80187178:
    // 0x80187178: lwc1        $f4, 0x68($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X68);
    // 0x8018717C: lwc1        $f10, 0x6C($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X6C);
    // 0x80187180: sb          $t6, 0x1B($a1)
    MEM_B(0X1B, ctx->r5) = ctx->r14;
    // 0x80187184: lwc1        $f6, 0x4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80187188: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8018718C: lwc1        $f4, 0x70($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X70);
    // 0x80187190: swc1        $f8, 0x68($a1)
    MEM_W(0X68, ctx->r5) = ctx->f8.u32l;
    // 0x80187194: lwc1        $f16, 0x4($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80187198: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x8018719C: swc1        $f18, 0x6C($a1)
    MEM_W(0X6C, ctx->r5) = ctx->f18.u32l;
    // 0x801871A0: lwc1        $f6, 0x4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801871A4: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801871A8: swc1        $f8, 0x70($a1)
    MEM_W(0X70, ctx->r5) = ctx->f8.u32l;
    // 0x801871AC: lhu         $t7, 0x38($a2)
    ctx->r15 = MEM_HU(ctx->r6, 0X38);
    // 0x801871B0: multu       $t7, $a0
    result = U64(U32(ctx->r15)) * U64(U32(ctx->r4)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801871B4: mflo        $t8
    ctx->r24 = lo;
    // 0x801871B8: addu        $t1, $t0, $t8
    ctx->r9 = ADD32(ctx->r8, ctx->r24);
    // 0x801871BC: lhu         $t2, 0x2($t1)
    ctx->r10 = MEM_HU(ctx->r9, 0X2);
    // 0x801871C0: addiu       $t3, $t2, -0xF
    ctx->r11 = ADD32(ctx->r10, -0XF);
    // 0x801871C4: sltiu       $at, $t3, 0xC
    ctx->r1 = ctx->r11 < 0XC ? 1 : 0;
    // 0x801871C8: beq         $at, $zero, L_80187334
    if (ctx->r1 == 0) {
        // 0x801871CC: sll         $t3, $t3, 2
        ctx->r11 = S32(ctx->r11 << 2);
            goto L_80187334;
    }
    // 0x801871CC: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x801871D0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801871D4: addu        $at, $at, $t3
    gpr jr_addend_801871DC = ctx->r11;
    ctx->r1 = ADD32(ctx->r1, ctx->r11);
    // 0x801871D8: lw          $t3, -0x3090($at)
    ctx->r11 = ADD32(ctx->r1, -0X3090);
    // 0x801871DC: jr          $t3
    // 0x801871E0: nop

    switch (jr_addend_801871DC >> 2) {
        case 0: goto L_801871E4; break;
        case 1: goto L_801871E4; break;
        case 2: goto L_80187334; break;
        case 3: goto L_80187334; break;
        case 4: goto L_80187334; break;
        case 5: goto L_80187334; break;
        case 6: goto L_80187334; break;
        case 7: goto L_801872E4; break;
        case 8: goto L_80187334; break;
        case 9: goto L_80187238; break;
        case 10: goto L_80187238; break;
        case 11: goto L_8018728C; break;
        default: switch_error(__func__, 0x801871DC, 0x8019CF70);
    }
    // 0x801871E0: nop

L_801871E4:
    // 0x801871E4: lwc1        $f10, 0x54($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
    // 0x801871E8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801871EC: ldc1        $f0, -0x3060($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, -0X3060);
    // 0x801871F0: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x801871F4: add.d       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = ctx->f16.d + ctx->f0.d;
    // 0x801871F8: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x801871FC: swc1        $f4, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f4.u32l;
    // 0x80187200: lwc1        $f6, 0x68($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X68);
    // 0x80187204: lwc1        $f18, 0x34($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X34);
    // 0x80187208: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x8018720C: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x80187210: add.d       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f0.d); 
    ctx->f10.d = ctx->f8.d + ctx->f0.d;
    // 0x80187214: add.d       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f4.d + ctx->f0.d;
    // 0x80187218: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x8018721C: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x80187220: swc1        $f16, 0x68($a2)
    MEM_W(0X68, ctx->r6) = ctx->f16.u32l;
    // 0x80187224: swc1        $f8, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f8.u32l;
    // 0x80187228: lh          $t4, 0x60($a1)
    ctx->r12 = MEM_H(ctx->r5, 0X60);
    // 0x8018722C: addiu       $t9, $t4, 0x1000
    ctx->r25 = ADD32(ctx->r12, 0X1000);
    // 0x80187230: b           L_80187334
    // 0x80187234: sh          $t9, 0x60($a1)
    MEM_H(0X60, ctx->r5) = ctx->r25;
        goto L_80187334;
    // 0x80187234: sh          $t9, 0x60($a1)
    MEM_H(0X60, ctx->r5) = ctx->r25;
L_80187238:
    // 0x80187238: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018723C: ldc1        $f0, -0x3058($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, -0X3058);
    // 0x80187240: lwc1        $f10, 0x54($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
    // 0x80187244: lui         $at, 0x400C
    ctx->r1 = S32(0X400C << 16);
    // 0x80187248: mtc1        $at, $f3
    ctx->f_odd[(3 - 1) * 2] = ctx->r1;
    // 0x8018724C: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80187250: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x80187254: add.d       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f2.d); 
    ctx->f18.d = ctx->f16.d + ctx->f2.d;
    // 0x80187258: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x8018725C: swc1        $f4, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f4.u32l;
    // 0x80187260: lwc1        $f6, 0x68($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X68);
    // 0x80187264: lwc1        $f18, 0x34($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X34);
    // 0x80187268: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x8018726C: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x80187270: add.d       $f10, $f8, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f2.d); 
    ctx->f10.d = ctx->f8.d + ctx->f2.d;
    // 0x80187274: add.d       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f4.d + ctx->f0.d;
    // 0x80187278: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x8018727C: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x80187280: swc1        $f16, 0x68($a2)
    MEM_W(0X68, ctx->r6) = ctx->f16.u32l;
    // 0x80187284: b           L_80187334
    // 0x80187288: swc1        $f8, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f8.u32l;
        goto L_80187334;
    // 0x80187288: swc1        $f8, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f8.u32l;
L_8018728C:
    // 0x8018728C: lwc1        $f10, 0x54($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
    // 0x80187290: lui         $at, 0x3FF8
    ctx->r1 = S32(0X3FF8 << 16);
    // 0x80187294: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    // 0x80187298: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8018729C: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x801872A0: lh          $t5, 0x60($a1)
    ctx->r13 = MEM_H(ctx->r5, 0X60);
    // 0x801872A4: add.d       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = ctx->f16.d + ctx->f0.d;
    // 0x801872A8: addiu       $t6, $t5, 0x1000
    ctx->r14 = ADD32(ctx->r13, 0X1000);
    // 0x801872AC: sh          $t6, 0x60($a1)
    MEM_H(0X60, ctx->r5) = ctx->r14;
    // 0x801872B0: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x801872B4: swc1        $f4, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f4.u32l;
    // 0x801872B8: lwc1        $f6, 0x68($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X68);
    // 0x801872BC: lwc1        $f18, 0x34($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X34);
    // 0x801872C0: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x801872C4: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x801872C8: add.d       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f0.d); 
    ctx->f10.d = ctx->f8.d + ctx->f0.d;
    // 0x801872CC: add.d       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f4.d + ctx->f0.d;
    // 0x801872D0: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x801872D4: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x801872D8: swc1        $f16, 0x68($a2)
    MEM_W(0X68, ctx->r6) = ctx->f16.u32l;
    // 0x801872DC: b           L_80187334
    // 0x801872E0: swc1        $f8, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f8.u32l;
        goto L_80187334;
    // 0x801872E0: swc1        $f8, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f8.u32l;
L_801872E4:
    // 0x801872E4: lwc1        $f10, 0x54($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
    // 0x801872E8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801872EC: ldc1        $f0, -0x3050($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, -0X3050);
    // 0x801872F0: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x801872F4: lh          $t7, 0x60($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X60);
    // 0x801872F8: add.d       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = ctx->f16.d + ctx->f0.d;
    // 0x801872FC: addiu       $t8, $t7, 0x1800
    ctx->r24 = ADD32(ctx->r15, 0X1800);
    // 0x80187300: sh          $t8, 0x60($a1)
    MEM_H(0X60, ctx->r5) = ctx->r24;
    // 0x80187304: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x80187308: swc1        $f4, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f4.u32l;
    // 0x8018730C: lwc1        $f6, 0x68($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X68);
    // 0x80187310: lwc1        $f18, 0x34($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X34);
    // 0x80187314: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x80187318: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x8018731C: add.d       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f0.d); 
    ctx->f10.d = ctx->f8.d + ctx->f0.d;
    // 0x80187320: add.d       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f4.d + ctx->f0.d;
    // 0x80187324: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x80187328: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x8018732C: swc1        $f16, 0x68($a2)
    MEM_W(0X68, ctx->r6) = ctx->f16.u32l;
    // 0x80187330: swc1        $f8, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f8.u32l;
L_80187334:
    // 0x80187334: lwc1        $f10, 0x54($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
    // 0x80187338: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018733C: ldc1        $f18, -0x3048($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, -0X3048);
    // 0x80187340: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x80187344: add.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f16.d + ctx->f18.d;
    // 0x80187348: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x8018734C: swc1        $f6, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f6.u32l;
    // 0x80187350: lhu         $t1, 0x38($a2)
    ctx->r9 = MEM_HU(ctx->r6, 0X38);
    // 0x80187354: lhu         $t9, 0x50($a2)
    ctx->r25 = MEM_HU(ctx->r6, 0X50);
    // 0x80187358: multu       $t1, $a0
    result = U64(U32(ctx->r9)) * U64(U32(ctx->r4)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8018735C: andi        $t5, $t9, 0x400
    ctx->r13 = ctx->r25 & 0X400;
    // 0x80187360: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187364: addiu       $t9, $t9, 0x47C0
    ctx->r25 = ADD32(ctx->r25, 0X47C0);
    // 0x80187368: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    // 0x8018736C: mflo        $t2
    ctx->r10 = lo;
    // 0x80187370: addu        $t3, $t0, $t2
    ctx->r11 = ADD32(ctx->r8, ctx->r10);
    // 0x80187374: lhu         $t4, 0x10($t3)
    ctx->r12 = MEM_HU(ctx->r11, 0X10);
    // 0x80187378: bne         $t5, $zero, L_80187440
    if (ctx->r13 != 0) {
        // 0x8018737C: sh          $t4, 0x5C($a2)
        MEM_H(0X5C, ctx->r6) = ctx->r12;
            goto L_80187440;
    }
    // 0x8018737C: sh          $t4, 0x5C($a2)
    MEM_H(0X5C, ctx->r6) = ctx->r12;
    // 0x80187380: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x80187384: jalr        $t9
    // 0x80187388: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x80187388: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_6:
    // 0x8018738C: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x80187390: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80187394: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80187398: sh          $v0, 0x4C($a2)
    MEM_H(0X4C, ctx->r6) = ctx->r2;
    // 0x8018739C: lwc1        $f8, -0x7D38($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x801873A0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801873A4: lwc1        $f10, -0x3040($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3040);
    // 0x801873A8: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x801873AC: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x801873B0: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x801873B4: cfc1        $t6, $FpcCsr
    ctx->r14 = get_cop1_cs();
    // 0x801873B8: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x801873BC: nop

    // 0x801873C0: cvt.w.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = CVT_W_S(ctx->f16.fl);
    // 0x801873C4: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x801873C8: nop

    // 0x801873CC: andi        $t7, $t7, 0x78
    ctx->r15 = ctx->r15 & 0X78;
    // 0x801873D0: beql        $t7, $zero, L_80187420
    if (ctx->r15 == 0) {
        // 0x801873D4: mfc1        $t7, $f18
        ctx->r15 = (int32_t)ctx->f18.u32l;
            goto L_80187420;
    }
    goto skip_9;
    // 0x801873D4: mfc1        $t7, $f18
    ctx->r15 = (int32_t)ctx->f18.u32l;
    skip_9:
    // 0x801873D8: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801873DC: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x801873E0: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x801873E4: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x801873E8: nop

    // 0x801873EC: cvt.w.s     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.u32l = CVT_W_S(ctx->f18.fl);
    // 0x801873F0: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x801873F4: nop

    // 0x801873F8: andi        $t7, $t7, 0x78
    ctx->r15 = ctx->r15 & 0X78;
    // 0x801873FC: bne         $t7, $zero, L_80187414
    if (ctx->r15 != 0) {
        // 0x80187400: nop
    
            goto L_80187414;
    }
    // 0x80187400: nop

    // 0x80187404: mfc1        $t7, $f18
    ctx->r15 = (int32_t)ctx->f18.u32l;
    // 0x80187408: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x8018740C: b           L_8018742C
    // 0x80187410: or          $t7, $t7, $at
    ctx->r15 = ctx->r15 | ctx->r1;
        goto L_8018742C;
    // 0x80187410: or          $t7, $t7, $at
    ctx->r15 = ctx->r15 | ctx->r1;
L_80187414:
    // 0x80187414: b           L_8018742C
    // 0x80187418: addiu       $t7, $zero, -0x1
    ctx->r15 = ADD32(0, -0X1);
        goto L_8018742C;
    // 0x80187418: addiu       $t7, $zero, -0x1
    ctx->r15 = ADD32(0, -0X1);
    // 0x8018741C: mfc1        $t7, $f18
    ctx->r15 = (int32_t)ctx->f18.u32l;
L_80187420:
    // 0x80187420: nop

    // 0x80187424: bltz        $t7, L_80187414
    if (SIGNED(ctx->r15) < 0) {
        // 0x80187428: nop
    
            goto L_80187414;
    }
    // 0x80187428: nop

L_8018742C:
    // 0x8018742C: lhu         $v1, 0x38($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X38);
    // 0x80187430: ctc1        $t6, $FpcCsr
    set_cop1_cs(ctx->r14);
    // 0x80187434: sh          $t7, 0x4E($a2)
    MEM_H(0X4E, ctx->r6) = ctx->r15;
    // 0x80187438: b           L_8018744C
    // 0x8018743C: addiu       $a0, $v1, 0x1
    ctx->r4 = ADD32(ctx->r3, 0X1);
        goto L_8018744C;
    // 0x8018743C: addiu       $a0, $v1, 0x1
    ctx->r4 = ADD32(ctx->r3, 0X1);
L_80187440:
    // 0x80187440: lhu         $v1, 0x38($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X38);
    // 0x80187444: sw          $zero, 0x60($a2)
    MEM_W(0X60, ctx->r6) = 0;
    // 0x80187448: addiu       $a0, $v1, 0x1
    ctx->r4 = ADD32(ctx->r3, 0X1);
L_8018744C:
    // 0x8018744C: slti        $at, $a0, 0x31
    ctx->r1 = SIGNED(ctx->r4) < 0X31 ? 1 : 0;
L_80187450:
    // 0x80187450: bne         $at, $zero, L_80187538
    if (ctx->r1 != 0) {
        // 0x80187454: lui         $t3, 0x8019
        ctx->r11 = S32(0X8019 << 16);
            goto L_80187538;
    }
    // 0x80187454: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x80187458: beq         $a3, $zero, L_8018748C
    if (ctx->r7 == 0) {
        // 0x8018745C: addiu       $t3, $t3, 0x6778
        ctx->r11 = ADD32(ctx->r11, 0X6778);
            goto L_8018748C;
    }
    // 0x8018745C: addiu       $t3, $t3, 0x6778
    ctx->r11 = ADD32(ctx->r11, 0X6778);
    // 0x80187460: lwc1        $f4, 0x4($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X4);
    // 0x80187464: lhu         $v1, 0x38($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X38);
    // 0x80187468: swc1        $f4, 0x64($a2)
    MEM_W(0X64, ctx->r6) = ctx->f4.u32l;
    // 0x8018746C: lwc1        $f6, 0x8($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X8);
    // 0x80187470: swc1        $f6, 0x68($a2)
    MEM_W(0X68, ctx->r6) = ctx->f6.u32l;
    // 0x80187474: lwc1        $f8, 0xC($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0XC);
    // 0x80187478: swc1        $f8, 0x40($a2)
    MEM_W(0X40, ctx->r6) = ctx->f8.u32l;
    // 0x8018747C: lhu         $t8, 0x14($a3)
    ctx->r24 = MEM_HU(ctx->r7, 0X14);
    // 0x80187480: sh          $t8, 0x5C($a2)
    MEM_H(0X5C, ctx->r6) = ctx->r24;
    // 0x80187484: lhu         $t1, 0x16($a3)
    ctx->r9 = MEM_HU(ctx->r7, 0X16);
    // 0x80187488: sh          $t1, 0x5E($a2)
    MEM_H(0X5E, ctx->r6) = ctx->r9;
L_8018748C:
    // 0x8018748C: sll         $t2, $v1, 2
    ctx->r10 = S32(ctx->r3 << 2);
    // 0x80187490: addu        $t2, $t2, $v1
    ctx->r10 = ADD32(ctx->r10, ctx->r3);
    // 0x80187494: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80187498: addu        $v0, $t2, $t3
    ctx->r2 = ADD32(ctx->r10, ctx->r11);
    // 0x8018749C: lhu         $a0, -0x308($v0)
    ctx->r4 = MEM_HU(ctx->r2, -0X308);
    // 0x801874A0: andi        $t4, $a0, 0x100
    ctx->r12 = ctx->r4 & 0X100;
    // 0x801874A4: beql        $t4, $zero, L_80187528
    if (ctx->r12 == 0) {
        // 0x801874A8: andi        $t1, $a0, 0x8000
        ctx->r9 = ctx->r4 & 0X8000;
            goto L_80187528;
    }
    goto skip_10;
    // 0x801874A8: andi        $t1, $a0, 0x8000
    ctx->r9 = ctx->r4 & 0X8000;
    skip_10:
    // 0x801874AC: lw          $a0, 0x14($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X14);
    // 0x801874B0: beql        $a0, $zero, L_80187510
    if (ctx->r4 == 0) {
        // 0x801874B4: lw          $t9, 0x10($a2)
        ctx->r25 = MEM_W(ctx->r6, 0X10);
            goto L_80187510;
    }
    goto skip_11;
    // 0x801874B4: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    skip_11:
    // 0x801874B8: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x801874BC: lhu         $t9, -0x2FE($v0)
    ctx->r25 = MEM_HU(ctx->r2, -0X2FE);
    // 0x801874C0: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x801874C4: lhu         $t5, 0x18($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X18);
    // 0x801874C8: bnel        $t5, $t9, L_801874F8
    if (ctx->r13 != ctx->r25) {
        // 0x801874CC: lw          $t9, 0x10($a2)
        ctx->r25 = MEM_W(ctx->r6, 0X10);
            goto L_801874F8;
    }
    goto skip_12;
    // 0x801874CC: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    skip_12:
    // 0x801874D0: lhu         $t6, 0x38($a2)
    ctx->r14 = MEM_HU(ctx->r6, 0X38);
    // 0x801874D4: addiu       $t8, $t8, 0x6778
    ctx->r24 = ADD32(ctx->r24, 0X6778);
    // 0x801874D8: sw          $a0, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->r4;
    // 0x801874DC: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x801874E0: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x801874E4: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x801874E8: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x801874EC: b           L_80187524
    // 0x801874F0: lhu         $a0, -0x308($v0)
    ctx->r4 = MEM_HU(ctx->r2, -0X308);
        goto L_80187524;
    // 0x801874F0: lhu         $a0, -0x308($v0)
    ctx->r4 = MEM_HU(ctx->r2, -0X308);
    // 0x801874F4: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
L_801874F8:
    // 0x801874F8: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x801874FC: jalr        $t9
    // 0x80187500: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80187500: nop

    after_7:
    // 0x80187504: b           L_80187558
    // 0x80187508: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80187558;
    // 0x80187508: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8018750C: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
L_80187510:
    // 0x80187510: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80187514: jalr        $t9
    // 0x80187518: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x80187518: nop

    after_8:
    // 0x8018751C: b           L_80187558
    // 0x80187520: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80187558;
    // 0x80187520: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187524:
    // 0x80187524: andi        $t1, $a0, 0x8000
    ctx->r9 = ctx->r4 & 0X8000;
L_80187528:
    // 0x80187528: beq         $t1, $zero, L_80187538
    if (ctx->r9 == 0) {
        // 0x8018752C: nop
    
            goto L_80187538;
    }
    // 0x8018752C: nop

    // 0x80187530: lhu         $t2, -0x30A($v0)
    ctx->r10 = MEM_HU(ctx->r2, -0X30A);
    // 0x80187534: sh          $t2, 0x3A($a2)
    MEM_H(0X3A, ctx->r6) = ctx->r10;
L_80187538:
    // 0x80187538: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018753C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80187540: sh          $zero, 0x4($a2)
    MEM_H(0X4, ctx->r6) = 0;
    // 0x80187544: sh          $zero, 0x46($a2)
    MEM_H(0X46, ctx->r6) = 0;
    // 0x80187548: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x8018754C: jalr        $t9
    // 0x80187550: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x80187550: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_9:
L_80187554:
    // 0x80187554: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187558:
    // 0x80187558: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8018755C: jr          $ra
    // 0x80187560: nop

    return;
    // 0x80187560: nop

;}
RECOMP_FUNC void Interactable_Main(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80187564: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80187568: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8018756C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80187570: lhu         $t6, 0x38($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X38);
    // 0x80187574: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80187578: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x8018757C: slti        $at, $t7, 0x31
    ctx->r1 = SIGNED(ctx->r15) < 0X31 ? 1 : 0;
    // 0x80187580: beql        $at, $zero, L_80187A68
    if (ctx->r1 == 0) {
        // 0x80187584: lw          $t7, 0x3C($s0)
        ctx->r15 = MEM_W(ctx->r16, 0X3C);
            goto L_80187A68;
    }
    goto skip_0;
    // 0x80187584: lw          $t7, 0x3C($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X3C);
    skip_0:
    // 0x80187588: lw          $a2, 0x24($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X24);
    // 0x8018758C: addiu       $a1, $a2, 0x50
    ctx->r5 = ADD32(ctx->r6, 0X50);
    // 0x80187590: jal         0x8014314C
    // 0x80187594: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    func_8014314C(rdram, ctx);
        goto after_0;
    // 0x80187594: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    after_0:
    // 0x80187598: beq         $v0, $zero, L_80187610
    if (ctx->r2 == 0) {
        // 0x8018759C: lw          $a2, 0x34($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X34);
            goto L_80187610;
    }
    // 0x8018759C: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x801875A0: lh          $t8, 0x0($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X0);
    // 0x801875A4: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x801875A8: or          $t9, $t8, $at
    ctx->r25 = ctx->r24 | ctx->r1;
    // 0x801875AC: sh          $t9, 0x0($a2)
    MEM_H(0X0, ctx->r6) = ctx->r25;
    // 0x801875B0: lhu         $t0, 0x4A($s0)
    ctx->r8 = MEM_HU(ctx->r16, 0X4A);
    // 0x801875B4: beql        $t0, $zero, L_801875FC
    if (ctx->r8 == 0) {
        // 0x801875B8: lw          $t9, 0x10($s0)
        ctx->r25 = MEM_W(ctx->r16, 0X10);
            goto L_801875FC;
    }
    goto skip_1;
    // 0x801875B8: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    skip_1:
    // 0x801875BC: lhu         $t1, 0x50($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X50);
    // 0x801875C0: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801875C4: addiu       $t9, $t9, -0x2F10
    ctx->r25 = ADD32(ctx->r25, -0X2F10);
    // 0x801875C8: andi        $t2, $t1, 0x400
    ctx->r10 = ctx->r9 & 0X400;
    // 0x801875CC: bnel        $t2, $zero, L_801875FC
    if (ctx->r10 != 0) {
        // 0x801875D0: lw          $t9, 0x10($s0)
        ctx->r25 = MEM_W(ctx->r16, 0X10);
            goto L_801875FC;
    }
    goto skip_2;
    // 0x801875D0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    skip_2:
    // 0x801875D4: jalr        $t9
    // 0x801875D8: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801875D8: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    after_1:
    // 0x801875DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801875E0: addiu       $t9, $t9, 0x47C0
    ctx->r25 = ADD32(ctx->r25, 0X47C0);
    // 0x801875E4: sh          $zero, 0x4A($s0)
    MEM_H(0X4A, ctx->r16) = 0;
    // 0x801875E8: sw          $zero, 0x60($s0)
    MEM_W(0X60, ctx->r16) = 0;
    // 0x801875EC: jalr        $t9
    // 0x801875F0: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801875F0: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    after_2:
    // 0x801875F4: sh          $v0, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r2;
    // 0x801875F8: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
L_801875FC:
    // 0x801875FC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80187600: jalr        $t9
    // 0x80187604: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80187604: nop

    after_3:
    // 0x80187608: b           L_80187C68
    // 0x8018760C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80187C68;
    // 0x8018760C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80187610:
    // 0x80187610: lhu         $v0, 0x50($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X50);
    // 0x80187614: andi        $t3, $v0, 0x800
    ctx->r11 = ctx->r2 & 0X800;
    // 0x80187618: bnel        $t3, $zero, L_80187634
    if (ctx->r11 != 0) {
        // 0x8018761C: andi        $t6, $v0, 0x400
        ctx->r14 = ctx->r2 & 0X400;
            goto L_80187634;
    }
    goto skip_3;
    // 0x8018761C: andi        $t6, $v0, 0x400
    ctx->r14 = ctx->r2 & 0X400;
    skip_3:
    // 0x80187620: lh          $t4, 0x0($a2)
    ctx->r12 = MEM_H(ctx->r6, 0X0);
    // 0x80187624: andi        $t5, $t4, 0x7FFF
    ctx->r13 = ctx->r12 & 0X7FFF;
    // 0x80187628: sh          $t5, 0x0($a2)
    MEM_H(0X0, ctx->r6) = ctx->r13;
    // 0x8018762C: lhu         $v0, 0x50($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X50);
    // 0x80187630: andi        $t6, $v0, 0x400
    ctx->r14 = ctx->r2 & 0X400;
L_80187634:
    // 0x80187634: bnel        $t6, $zero, L_80187760
    if (ctx->r14 != 0) {
        // 0x80187638: lhu         $t8, 0x56($s0)
        ctx->r24 = MEM_HU(ctx->r16, 0X56);
            goto L_80187760;
    }
    goto skip_4;
    // 0x80187638: lhu         $t8, 0x56($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X56);
    skip_4:
    // 0x8018763C: lhu         $v1, 0x48($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0X48);
    // 0x80187640: lhu         $t7, 0x4C($s0)
    ctx->r15 = MEM_HU(ctx->r16, 0X4C);
    // 0x80187644: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80187648: slt         $at, $t7, $v1
    ctx->r1 = SIGNED(ctx->r15) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x8018764C: beql        $at, $zero, L_801876A0
    if (ctx->r1 == 0) {
        // 0x80187650: lhu         $t9, 0x4E($s0)
        ctx->r25 = MEM_HU(ctx->r16, 0X4E);
            goto L_801876A0;
    }
    goto skip_5;
    // 0x80187650: lhu         $t9, 0x4E($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X4E);
    skip_5:
    // 0x80187654: lhu         $t8, 0x4A($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X4A);
    // 0x80187658: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8018765C: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x80187660: bnel        $t8, $zero, L_801876A0
    if (ctx->r24 != 0) {
        // 0x80187664: lhu         $t9, 0x4E($s0)
        ctx->r25 = MEM_HU(ctx->r16, 0X4E);
            goto L_801876A0;
    }
    goto skip_6;
    // 0x80187664: lhu         $t9, 0x4E($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X4E);
    skip_6:
    // 0x80187668: lw          $t0, 0x2BD0($a3)
    ctx->r8 = MEM_W(ctx->r7, 0X2BD0);
    // 0x8018766C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80187670: andi        $t1, $t0, 0x1
    ctx->r9 = ctx->r8 & 0X1;
    // 0x80187674: bnel        $t1, $zero, L_80187760
    if (ctx->r9 != 0) {
        // 0x80187678: lhu         $t8, 0x56($s0)
        ctx->r24 = MEM_HU(ctx->r16, 0X56);
            goto L_80187760;
    }
    goto skip_7;
    // 0x80187678: lhu         $t8, 0x56($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X56);
    skip_7:
    // 0x8018767C: jal         0x801885B8
    // 0x80187680: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    func_801885B8(rdram, ctx);
        goto after_4;
    // 0x80187680: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    after_4:
    // 0x80187684: beq         $v0, $zero, L_8018775C
    if (ctx->r2 == 0) {
        // 0x80187688: lw          $a2, 0x34($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X34);
            goto L_8018775C;
    }
    // 0x80187688: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x8018768C: addiu       $t2, $zero, 0x2
    ctx->r10 = ADD32(0, 0X2);
    // 0x80187690: sh          $t2, 0x4A($s0)
    MEM_H(0X4A, ctx->r16) = ctx->r10;
    // 0x80187694: b           L_8018775C
    // 0x80187698: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
        goto L_8018775C;
    // 0x80187698: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
    // 0x8018769C: lhu         $t9, 0x4E($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X4E);
L_801876A0:
    // 0x801876A0: slt         $at, $t9, $v0
    ctx->r1 = SIGNED(ctx->r25) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x801876A4: beq         $at, $zero, L_801876F4
    if (ctx->r1 == 0) {
        // 0x801876A8: nop
    
            goto L_801876F4;
    }
    // 0x801876A8: nop

    // 0x801876AC: lhu         $t3, 0x4A($s0)
    ctx->r11 = MEM_HU(ctx->r16, 0X4A);
    // 0x801876B0: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x801876B4: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801876B8: bne         $t3, $at, L_801876F4
    if (ctx->r11 != ctx->r1) {
        // 0x801876BC: addiu       $t9, $t9, -0x2F10
        ctx->r25 = ADD32(ctx->r25, -0X2F10);
            goto L_801876F4;
    }
    // 0x801876BC: addiu       $t9, $t9, -0x2F10
    ctx->r25 = ADD32(ctx->r25, -0X2F10);
    // 0x801876C0: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    // 0x801876C4: jalr        $t9
    // 0x801876C8: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x801876C8: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    after_5:
    // 0x801876CC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801876D0: addiu       $t9, $t9, 0x47C0
    ctx->r25 = ADD32(ctx->r25, 0X47C0);
    // 0x801876D4: sh          $zero, 0x4A($s0)
    MEM_H(0X4A, ctx->r16) = 0;
    // 0x801876D8: sw          $zero, 0x60($s0)
    MEM_W(0X60, ctx->r16) = 0;
    // 0x801876DC: jalr        $t9
    // 0x801876E0: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x801876E0: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    after_6:
    // 0x801876E4: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x801876E8: sh          $v0, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r2;
    // 0x801876EC: b           L_8018775C
    // 0x801876F0: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
        goto L_8018775C;
    // 0x801876F0: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
L_801876F4:
    // 0x801876F4: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801876F8: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x801876FC: lw          $t4, 0x2BD0($a3)
    ctx->r12 = MEM_W(ctx->r7, 0X2BD0);
    // 0x80187700: addiu       $t7, $v1, 0x1
    ctx->r15 = ADD32(ctx->r3, 0X1);
    // 0x80187704: andi        $t5, $t4, 0x1
    ctx->r13 = ctx->r12 & 0X1;
    // 0x80187708: beql        $t5, $zero, L_8018775C
    if (ctx->r13 == 0) {
        // 0x8018770C: sh          $t7, 0x48($s0)
        MEM_H(0X48, ctx->r16) = ctx->r15;
            goto L_8018775C;
    }
    goto skip_8;
    // 0x8018770C: sh          $t7, 0x48($s0)
    MEM_H(0X48, ctx->r16) = ctx->r15;
    skip_8:
    // 0x80187710: lhu         $t6, 0x4A($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X4A);
    // 0x80187714: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x80187718: addiu       $t9, $t9, -0x2F10
    ctx->r25 = ADD32(ctx->r25, -0X2F10);
    // 0x8018771C: beql        $t6, $zero, L_8018775C
    if (ctx->r14 == 0) {
        // 0x80187720: sh          $t7, 0x48($s0)
        MEM_H(0X48, ctx->r16) = ctx->r15;
            goto L_8018775C;
    }
    goto skip_9;
    // 0x80187720: sh          $t7, 0x48($s0)
    MEM_H(0X48, ctx->r16) = ctx->r15;
    skip_9:
    // 0x80187724: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    // 0x80187728: jalr        $t9
    // 0x8018772C: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x8018772C: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    after_7:
    // 0x80187730: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187734: addiu       $t9, $t9, 0x47C0
    ctx->r25 = ADD32(ctx->r25, 0X47C0);
    // 0x80187738: sh          $zero, 0x4A($s0)
    MEM_H(0X4A, ctx->r16) = 0;
    // 0x8018773C: sw          $zero, 0x60($s0)
    MEM_W(0X60, ctx->r16) = 0;
    // 0x80187740: jalr        $t9
    // 0x80187744: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x80187744: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    after_8:
    // 0x80187748: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x8018774C: sh          $v0, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r2;
    // 0x80187750: b           L_8018775C
    // 0x80187754: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
        goto L_8018775C;
    // 0x80187754: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
    // 0x80187758: sh          $t7, 0x48($s0)
    MEM_H(0X48, ctx->r16) = ctx->r15;
L_8018775C:
    // 0x8018775C: lhu         $t8, 0x56($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X56);
L_80187760:
    // 0x80187760: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x80187764: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x80187768: andi        $t0, $t8, 0x1
    ctx->r8 = ctx->r24 & 0X1;
    // 0x8018776C: beql        $t0, $zero, L_801877C0
    if (ctx->r8 == 0) {
        // 0x80187770: lhu         $t9, 0x50($s0)
        ctx->r25 = MEM_HU(ctx->r16, 0X50);
            goto L_801877C0;
    }
    goto skip_10;
    // 0x80187770: lhu         $t9, 0x50($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X50);
    skip_10:
    // 0x80187774: lhu         $t1, 0x38($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X38);
    // 0x80187778: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x8018777C: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80187780: addu        $t2, $t2, $t1
    ctx->r10 = ADD32(ctx->r10, ctx->r9);
    // 0x80187784: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80187788: addu        $v0, $v0, $t2
    ctx->r2 = ADD32(ctx->r2, ctx->r10);
    // 0x8018778C: lhu         $v0, 0x677A($v0)
    ctx->r2 = MEM_HU(ctx->r2, 0X677A);
    // 0x80187790: blez        $v0, L_801877B0
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80187794: slti        $at, $v0, 0x7
        ctx->r1 = SIGNED(ctx->r2) < 0X7 ? 1 : 0;
            goto L_801877B0;
    }
    // 0x80187794: slti        $at, $v0, 0x7
    ctx->r1 = SIGNED(ctx->r2) < 0X7 ? 1 : 0;
    // 0x80187798: beql        $at, $zero, L_801877B4
    if (ctx->r1 == 0) {
        // 0x8018779C: lh          $t5, 0x5E($a2)
        ctx->r13 = MEM_H(ctx->r6, 0X5E);
            goto L_801877B4;
    }
    goto skip_11;
    // 0x8018779C: lh          $t5, 0x5E($a2)
    ctx->r13 = MEM_H(ctx->r6, 0X5E);
    skip_11:
    // 0x801877A0: lh          $t3, 0x5E($a2)
    ctx->r11 = MEM_H(ctx->r6, 0X5E);
    // 0x801877A4: addiu       $t4, $t3, 0x400
    ctx->r12 = ADD32(ctx->r11, 0X400);
    // 0x801877A8: b           L_801877BC
    // 0x801877AC: sh          $t4, 0x5E($a2)
    MEM_H(0X5E, ctx->r6) = ctx->r12;
        goto L_801877BC;
    // 0x801877AC: sh          $t4, 0x5E($a2)
    MEM_H(0X5E, ctx->r6) = ctx->r12;
L_801877B0:
    // 0x801877B0: lh          $t5, 0x5E($a2)
    ctx->r13 = MEM_H(ctx->r6, 0X5E);
L_801877B4:
    // 0x801877B4: addiu       $t6, $t5, 0x800
    ctx->r14 = ADD32(ctx->r13, 0X800);
    // 0x801877B8: sh          $t6, 0x5E($a2)
    MEM_H(0X5E, ctx->r6) = ctx->r14;
L_801877BC:
    // 0x801877BC: lhu         $t9, 0x50($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X50);
L_801877C0:
    // 0x801877C0: andi        $t7, $t9, 0x1
    ctx->r15 = ctx->r25 & 0X1;
    // 0x801877C4: beql        $t7, $zero, L_8018797C
    if (ctx->r15 == 0) {
        // 0x801877C8: lhu         $t6, 0x50($s0)
        ctx->r14 = MEM_HU(ctx->r16, 0X50);
            goto L_8018797C;
    }
    goto skip_12;
    // 0x801877C8: lhu         $t6, 0x50($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X50);
    skip_12:
    // 0x801877CC: lhu         $t8, 0x44($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X44);
    // 0x801877D0: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x801877D4: bnel        $t8, $zero, L_8018797C
    if (ctx->r24 != 0) {
        // 0x801877D8: lhu         $t6, 0x50($s0)
        ctx->r14 = MEM_HU(ctx->r16, 0X50);
            goto L_8018797C;
    }
    goto skip_13;
    // 0x801877D8: lhu         $t6, 0x50($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X50);
    skip_13:
    // 0x801877DC: lwc1        $f4, 0x8($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X8);
    // 0x801877E0: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801877E4: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x801877E8: lhu         $v0, 0x4($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X4);
    // 0x801877EC: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801877F0: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x801877F4: addiu       $t2, $v0, 0x1
    ctx->r10 = ADD32(ctx->r2, 0X1);
    // 0x801877F8: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    // 0x801877FC: ctc1        $t1, $FpcCsr
    set_cop1_cs(ctx->r9);
    // 0x80187800: nop

    // 0x80187804: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x80187808: cfc1        $t1, $FpcCsr
    ctx->r9 = get_cop1_cs();
    // 0x8018780C: nop

    // 0x80187810: andi        $t1, $t1, 0x78
    ctx->r9 = ctx->r9 & 0X78;
    // 0x80187814: beql        $t1, $zero, L_80187864
    if (ctx->r9 == 0) {
        // 0x80187818: mfc1        $t1, $f10
        ctx->r9 = (int32_t)ctx->f10.u32l;
            goto L_80187864;
    }
    goto skip_14;
    // 0x80187818: mfc1        $t1, $f10
    ctx->r9 = (int32_t)ctx->f10.u32l;
    skip_14:
    // 0x8018781C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80187820: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x80187824: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80187828: ctc1        $t1, $FpcCsr
    set_cop1_cs(ctx->r9);
    // 0x8018782C: nop

    // 0x80187830: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x80187834: cfc1        $t1, $FpcCsr
    ctx->r9 = get_cop1_cs();
    // 0x80187838: nop

    // 0x8018783C: andi        $t1, $t1, 0x78
    ctx->r9 = ctx->r9 & 0X78;
    // 0x80187840: bne         $t1, $zero, L_80187858
    if (ctx->r9 != 0) {
        // 0x80187844: nop
    
            goto L_80187858;
    }
    // 0x80187844: nop

    // 0x80187848: mfc1        $t1, $f10
    ctx->r9 = (int32_t)ctx->f10.u32l;
    // 0x8018784C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80187850: b           L_80187870
    // 0x80187854: or          $t1, $t1, $at
    ctx->r9 = ctx->r9 | ctx->r1;
        goto L_80187870;
    // 0x80187854: or          $t1, $t1, $at
    ctx->r9 = ctx->r9 | ctx->r1;
L_80187858:
    // 0x80187858: b           L_80187870
    // 0x8018785C: addiu       $t1, $zero, -0x1
    ctx->r9 = ADD32(0, -0X1);
        goto L_80187870;
    // 0x8018785C: addiu       $t1, $zero, -0x1
    ctx->r9 = ADD32(0, -0X1);
    // 0x80187860: mfc1        $t1, $f10
    ctx->r9 = (int32_t)ctx->f10.u32l;
L_80187864:
    // 0x80187864: nop

    // 0x80187868: bltz        $t1, L_80187858
    if (SIGNED(ctx->r9) < 0) {
        // 0x8018786C: nop
    
            goto L_80187858;
    }
    // 0x8018786C: nop

L_80187870:
    // 0x80187870: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x80187874: sltu        $v1, $t1, $v0
    ctx->r3 = ctx->r9 < ctx->r2 ? 1 : 0;
    // 0x80187878: beq         $v1, $zero, L_80187978
    if (ctx->r3 == 0) {
        // 0x8018787C: sh          $t2, 0x4($s0)
        MEM_H(0X4, ctx->r16) = ctx->r10;
            goto L_80187978;
    }
    // 0x8018787C: sh          $t2, 0x4($s0)
    MEM_H(0X4, ctx->r16) = ctx->r10;
    // 0x80187880: lbu         $t3, 0x1B($a2)
    ctx->r11 = MEM_BU(ctx->r6, 0X1B);
    // 0x80187884: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x80187888: mtc1        $t3, $f16
    ctx->f16.u32l = ctx->r11;
    // 0x8018788C: bgez        $t3, L_801878A0
    if (SIGNED(ctx->r11) >= 0) {
        // 0x80187890: cvt.s.w     $f0, $f16
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    ctx->f0.fl = CVT_S_W(ctx->f16.u32l);
            goto L_801878A0;
    }
    // 0x80187890: cvt.s.w     $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    ctx->f0.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80187894: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x80187898: nop

    // 0x8018789C: add.s       $f0, $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f18.fl;
L_801878A0:
    // 0x801878A0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801878A4: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801878A8: lwc1        $f4, 0x8($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X8);
    // 0x801878AC: lui         $at, 0x437F
    ctx->r1 = S32(0X437F << 16);
    // 0x801878B0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801878B4: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801878B8: div.s       $f2, $f10, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f8.fl);
    // 0x801878BC: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x801878C0: nop

    // 0x801878C4: bc1fl       L_801878EC
    if (!c1cs) {
        // 0x801878C8: sub.s       $f16, $f0, $f2
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
            goto L_801878EC;
    }
    goto skip_15;
    // 0x801878C8: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
    skip_15:
    // 0x801878CC: sb          $zero, 0x1B($a2)
    MEM_B(0X1B, ctx->r6) = 0;
    // 0x801878D0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x801878D4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801878D8: jalr        $t9
    // 0x801878DC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x801878DC: nop

    after_9:
    // 0x801878E0: b           L_80187C68
    // 0x801878E4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80187C68;
    // 0x801878E4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801878E8: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
L_801878EC:
    // 0x801878EC: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x801878F0: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x801878F4: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x801878F8: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x801878FC: nop

    // 0x80187900: cvt.w.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = CVT_W_S(ctx->f16.fl);
    // 0x80187904: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x80187908: nop

    // 0x8018790C: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x80187910: beql        $t5, $zero, L_80187960
    if (ctx->r13 == 0) {
        // 0x80187914: mfc1        $t5, $f18
        ctx->r13 = (int32_t)ctx->f18.u32l;
            goto L_80187960;
    }
    goto skip_16;
    // 0x80187914: mfc1        $t5, $f18
    ctx->r13 = (int32_t)ctx->f18.u32l;
    skip_16:
    // 0x80187918: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x8018791C: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80187920: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x80187924: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x80187928: nop

    // 0x8018792C: cvt.w.s     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.u32l = CVT_W_S(ctx->f18.fl);
    // 0x80187930: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x80187934: nop

    // 0x80187938: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x8018793C: bne         $t5, $zero, L_80187954
    if (ctx->r13 != 0) {
        // 0x80187940: nop
    
            goto L_80187954;
    }
    // 0x80187940: nop

    // 0x80187944: mfc1        $t5, $f18
    ctx->r13 = (int32_t)ctx->f18.u32l;
    // 0x80187948: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x8018794C: b           L_8018796C
    // 0x80187950: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
        goto L_8018796C;
    // 0x80187950: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
L_80187954:
    // 0x80187954: b           L_8018796C
    // 0x80187958: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
        goto L_8018796C;
    // 0x80187958: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
    // 0x8018795C: mfc1        $t5, $f18
    ctx->r13 = (int32_t)ctx->f18.u32l;
L_80187960:
    // 0x80187960: nop

    // 0x80187964: bltz        $t5, L_80187954
    if (SIGNED(ctx->r13) < 0) {
        // 0x80187968: nop
    
            goto L_80187954;
    }
    // 0x80187968: nop

L_8018796C:
    // 0x8018796C: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x80187970: sb          $t5, 0x1B($a2)
    MEM_B(0X1B, ctx->r6) = ctx->r13;
    // 0x80187974: nop

L_80187978:
    // 0x80187978: lhu         $t6, 0x50($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X50);
L_8018797C:
    // 0x8018797C: andi        $t7, $t6, 0x4000
    ctx->r15 = ctx->r14 & 0X4000;
    // 0x80187980: beql        $t7, $zero, L_80187A08
    if (ctx->r15 == 0) {
        // 0x80187984: lwc1        $f18, 0x64($s0)
        ctx->f18.u32l = MEM_W(ctx->r16, 0X64);
            goto L_80187A08;
    }
    goto skip_17;
    // 0x80187984: lwc1        $f18, 0x64($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X64);
    skip_17:
    // 0x80187988: lwc1        $f12, 0x68($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X68);
    // 0x8018798C: lwc1        $f0, 0x34($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X34);
    // 0x80187990: c.eq.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl == ctx->f0.fl;
    // 0x80187994: nop

    // 0x80187998: bc1tl       L_80187A08
    if (c1cs) {
        // 0x8018799C: lwc1        $f18, 0x64($s0)
        ctx->f18.u32l = MEM_W(ctx->r16, 0X64);
            goto L_80187A08;
    }
    goto skip_18;
    // 0x8018799C: lwc1        $f18, 0x64($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X64);
    skip_18:
    // 0x801879A0: lhu         $t8, 0x46($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X46);
    // 0x801879A4: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x801879A8: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x801879AC: andi        $t1, $t0, 0xFFFF
    ctx->r9 = ctx->r8 & 0XFFFF;
    // 0x801879B0: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x801879B4: sh          $t0, 0x46($s0)
    MEM_H(0X46, ctx->r16) = ctx->r8;
    // 0x801879B8: bgez        $t1, L_801879D0
    if (SIGNED(ctx->r9) >= 0) {
        // 0x801879BC: cvt.d.w     $f6, $f4
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
            goto L_801879D0;
    }
    // 0x801879BC: cvt.d.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
    // 0x801879C0: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x801879C4: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801879C8: nop

    // 0x801879CC: add.d       $f6, $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f10.d); 
    ctx->f6.d = ctx->f6.d + ctx->f10.d;
L_801879D0:
    // 0x801879D0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801879D4: ldc1        $f8, -0x3038($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3038);
    // 0x801879D8: mul.d       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f16.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x801879DC: cvt.s.d     $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f2.fl = CVT_S_D(ctx->f16.d);
    // 0x801879E0: sub.s       $f14, $f12, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = ctx->f12.fl - ctx->f2.fl;
    // 0x801879E4: c.lt.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl < ctx->f0.fl;
    // 0x801879E8: nop

    // 0x801879EC: bc1fl       L_80187A04
    if (!c1cs) {
        // 0x801879F0: swc1        $f14, 0x68($s0)
        MEM_W(0X68, ctx->r16) = ctx->f14.u32l;
            goto L_80187A04;
    }
    goto skip_19;
    // 0x801879F0: swc1        $f14, 0x68($s0)
    MEM_W(0X68, ctx->r16) = ctx->f14.u32l;
    skip_19:
    // 0x801879F4: swc1        $f0, 0x68($s0)
    MEM_W(0X68, ctx->r16) = ctx->f0.u32l;
    // 0x801879F8: b           L_80187A04
    // 0x801879FC: sh          $zero, 0x46($s0)
    MEM_H(0X46, ctx->r16) = 0;
        goto L_80187A04;
    // 0x801879FC: sh          $zero, 0x46($s0)
    MEM_H(0X46, ctx->r16) = 0;
    // 0x80187A00: swc1        $f14, 0x68($s0)
    MEM_W(0X68, ctx->r16) = ctx->f14.u32l;
L_80187A04:
    // 0x80187A04: lwc1        $f18, 0x64($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X64);
L_80187A08:
    // 0x80187A08: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80187A0C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80187A10: swc1        $f18, 0x50($a2)
    MEM_W(0X50, ctx->r6) = ctx->f18.u32l;
    // 0x80187A14: lwc1        $f4, 0x68($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X68);
    // 0x80187A18: swc1        $f4, 0x54($a2)
    MEM_W(0X54, ctx->r6) = ctx->f4.u32l;
    // 0x80187A1C: lwc1        $f10, 0x40($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X40);
    // 0x80187A20: swc1        $f10, 0x58($a2)
    MEM_W(0X58, ctx->r6) = ctx->f10.u32l;
    // 0x80187A24: lhu         $t2, 0x38($s0)
    ctx->r10 = MEM_HU(ctx->r16, 0X38);
    // 0x80187A28: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80187A2C: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x80187A30: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x80187A34: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80187A38: lhu         $t9, 0x677A($t9)
    ctx->r25 = MEM_HU(ctx->r25, 0X677A);
    // 0x80187A3C: bnel        $t9, $at, L_80187A68
    if (ctx->r25 != ctx->r1) {
        // 0x80187A40: lw          $t7, 0x3C($s0)
        ctx->r15 = MEM_W(ctx->r16, 0X3C);
            goto L_80187A68;
    }
    goto skip_20;
    // 0x80187A40: lw          $t7, 0x3C($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X3C);
    skip_20:
    // 0x80187A44: lh          $t4, 0x2B5C($a3)
    ctx->r12 = MEM_H(ctx->r7, 0X2B5C);
    // 0x80187A48: addiu       $t5, $zero, -0xC0
    ctx->r13 = ADD32(0, -0XC0);
    // 0x80187A4C: addiu       $t6, $zero, -0x1
    ctx->r14 = ADD32(0, -0X1);
    // 0x80187A50: bgezl       $t4, L_80187A64
    if (SIGNED(ctx->r12) >= 0) {
        // 0x80187A54: sw          $t6, 0x18($a2)
        MEM_W(0X18, ctx->r6) = ctx->r14;
            goto L_80187A64;
    }
    goto skip_21;
    // 0x80187A54: sw          $t6, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r14;
    skip_21:
    // 0x80187A58: b           L_80187A64
    // 0x80187A5C: sw          $t5, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r13;
        goto L_80187A64;
    // 0x80187A5C: sw          $t5, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r13;
    // 0x80187A60: sw          $t6, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r14;
L_80187A64:
    // 0x80187A64: lw          $t7, 0x3C($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X3C);
L_80187A68:
    // 0x80187A68: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x80187A6C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, main sees interaction flag ---
    if (lod_interact_trace_enabled() && ctx->r15 == ctx->r1) {
        lod_interact_trace_state(rdram, "main-flagged", ctx->r16);
    }
    // --- END PATCH ---
#endif
    // 0x80187A70: bne         $t7, $at, L_80187C64
    if (ctx->r15 != ctx->r1) {
        // 0x80187A74: addiu       $a3, $a3, -0x7D40
        ctx->r7 = ADD32(ctx->r7, -0X7D40);
            goto L_80187C64;
    }
    // 0x80187A74: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x80187A78: lhu         $v1, 0x38($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0X38);
    // 0x80187A7C: addiu       $v0, $v1, 0x1
    ctx->r2 = ADD32(ctx->r3, 0X1);
    // 0x80187A80: slti        $at, $v0, 0x31
    ctx->r1 = SIGNED(ctx->r2) < 0X31 ? 1 : 0;
    // 0x80187A84: beql        $at, $zero, L_80187AD0
    if (ctx->r1 == 0) {
        // 0x80187A88: slti        $at, $v0, 0x31
        ctx->r1 = SIGNED(ctx->r2) < 0X31 ? 1 : 0;
            goto L_80187AD0;
    }
    goto skip_22;
    // 0x80187A88: slti        $at, $v0, 0x31
    ctx->r1 = SIGNED(ctx->r2) < 0X31 ? 1 : 0;
    skip_22:
    // 0x80187A8C: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    // 0x80187A90: lui         $t8, 0x8000
    ctx->r24 = S32(0X8000 << 16);
    // 0x80187A94: addiu       $t8, $t8, 0x47C0
    ctx->r24 = ADD32(ctx->r24, 0X47C0);
    // 0x80187A98: beq         $a0, $zero, L_80187C4C
    if (ctx->r4 == 0) {
        // 0x80187A9C: lui         $t9, 0x8006
        ctx->r25 = S32(0X8006 << 16);
            goto L_80187C4C;
    }
    // 0x80187A9C: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x80187AA0: addiu       $t9, $t9, -0x2F10
    ctx->r25 = ADD32(ctx->r25, -0X2F10);
    // 0x80187AA4: jalr        $t9
    // 0x80187AA8: sw          $t8, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r24;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x80187AA8: sw          $t8, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r24;
    after_10:
    // 0x80187AAC: sh          $zero, 0x4A($s0)
    MEM_H(0X4A, ctx->r16) = 0;
    // 0x80187AB0: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
    // 0x80187AB4: lw          $t9, 0x24($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X24);
    // 0x80187AB8: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    // 0x80187ABC: jalr        $t9
    // 0x80187AC0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x80187AC0: nop

    after_11:
    // 0x80187AC4: b           L_80187C4C
    // 0x80187AC8: sh          $v0, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r2;
        goto L_80187C4C;
    // 0x80187AC8: sh          $v0, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r2;
    // 0x80187ACC: slti        $at, $v0, 0x31
    ctx->r1 = SIGNED(ctx->r2) < 0X31 ? 1 : 0;
L_80187AD0:
    // 0x80187AD0: bne         $at, $zero, L_80187C4C
    if (ctx->r1 != 0) {
        // 0x80187AD4: addiu       $t0, $v1, -0x30
        ctx->r8 = ADD32(ctx->r3, -0X30);
            goto L_80187C4C;
    }
    // 0x80187AD4: addiu       $t0, $v1, -0x30
    ctx->r8 = ADD32(ctx->r3, -0X30);
    // 0x80187AD8: slti        $at, $t0, 0x28
    ctx->r1 = SIGNED(ctx->r8) < 0X28 ? 1 : 0;
    // 0x80187ADC: bne         $at, $zero, L_80187BEC
    if (ctx->r1 != 0) {
        // 0x80187AE0: sll         $t1, $v1, 2
        ctx->r9 = S32(ctx->r3 << 2);
            goto L_80187BEC;
    }
    // 0x80187AE0: sll         $t1, $v1, 2
    ctx->r9 = S32(ctx->r3 << 2);
    // 0x80187AE4: addu        $t1, $t1, $v1
    ctx->r9 = ADD32(ctx->r9, ctx->r3);
    // 0x80187AE8: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x80187AEC: addiu       $t2, $t2, 0x6778
    ctx->r10 = ADD32(ctx->r10, 0X6778);
    // 0x80187AF0: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x80187AF4: addu        $v0, $t1, $t2
    ctx->r2 = ADD32(ctx->r9, ctx->r10);
    // 0x80187AF8: lhu         $t3, -0x308($v0)
    ctx->r11 = MEM_HU(ctx->r2, -0X308);
    // 0x80187AFC: andi        $t4, $t3, 0x4
    ctx->r12 = ctx->r11 & 0X4;
    // 0x80187B00: beql        $t4, $zero, L_80187B60
    if (ctx->r12 == 0) {
        // 0x80187B04: lhu         $t5, 0x38($s0)
        ctx->r13 = MEM_HU(ctx->r16, 0X38);
            goto L_80187B60;
    }
    goto skip_23;
    // 0x80187B04: lhu         $t5, 0x38($s0)
    ctx->r13 = MEM_HU(ctx->r16, 0X38);
    skip_23:
    // 0x80187B08: lw          $v1, -0x304($v0)
    ctx->r3 = MEM_W(ctx->r2, -0X304);
    // 0x80187B0C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80187B10: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80187B14: beq         $v1, $zero, L_80187B38
    if (ctx->r3 == 0) {
        // 0x80187B18: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80187B38;
    }
    // 0x80187B18: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187B1C: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80187B20: jalr        $t9
    // 0x80187B24: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x80187B24: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_12:
    // 0x80187B28: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x80187B2C: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x80187B30: b           L_80187B3C
    // 0x80187B34: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80187B3C;
    // 0x80187B34: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80187B38:
    // 0x80187B38: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80187B3C:
    // 0x80187B3C: beql        $v1, $zero, L_80187B60
    if (ctx->r3 == 0) {
        // 0x80187B40: lhu         $t5, 0x38($s0)
        ctx->r13 = MEM_HU(ctx->r16, 0X38);
            goto L_80187B60;
    }
    goto skip_24;
    // 0x80187B40: lhu         $t5, 0x38($s0)
    ctx->r13 = MEM_HU(ctx->r16, 0X38);
    skip_24:
    // 0x80187B44: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80187B48: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80187B4C: jalr        $t9
    // 0x80187B50: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_13;
    // 0x80187B50: nop

    after_13:
    // 0x80187B54: b           L_80187C68
    // 0x80187B58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80187C68;
    // 0x80187B58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80187B5C: lhu         $t5, 0x38($s0)
    ctx->r13 = MEM_HU(ctx->r16, 0X38);
L_80187B60:
    // 0x80187B60: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80187B64: addiu       $t7, $t7, 0x6778
    ctx->r15 = ADD32(ctx->r15, 0X6778);
    // 0x80187B68: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x80187B6C: addu        $t6, $t6, $t5
    ctx->r14 = ADD32(ctx->r14, ctx->r13);
    // 0x80187B70: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80187B74: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
    // 0x80187B78: lhu         $t8, -0x308($v0)
    ctx->r24 = MEM_HU(ctx->r2, -0X308);
    // 0x80187B7C: andi        $t0, $t8, 0x8
    ctx->r8 = ctx->r24 & 0X8;
    // 0x80187B80: beql        $t0, $zero, L_80187BF0
    if (ctx->r8 == 0) {
        // 0x80187B84: lhu         $t1, 0x38($s0)
        ctx->r9 = MEM_HU(ctx->r16, 0X38);
            goto L_80187BF0;
    }
    goto skip_25;
    // 0x80187B84: lhu         $t1, 0x38($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X38);
    skip_25:
    // 0x80187B88: lw          $v1, -0x304($v0)
    ctx->r3 = MEM_W(ctx->r2, -0X304);
    // 0x80187B8C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80187B90: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80187B94: beq         $v1, $zero, L_80187BB8
    if (ctx->r3 == 0) {
        // 0x80187B98: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80187BB8;
    }
    // 0x80187B98: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187B9C: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80187BA0: jalr        $t9
    // 0x80187BA4: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_14;
    // 0x80187BA4: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_14:
    // 0x80187BA8: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x80187BAC: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x80187BB0: b           L_80187BBC
    // 0x80187BB4: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80187BBC;
    // 0x80187BB4: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80187BB8:
    // 0x80187BB8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80187BBC:
    // 0x80187BBC: bnel        $v1, $zero, L_80187BF0
    if (ctx->r3 != 0) {
        // 0x80187BC0: lhu         $t1, 0x38($s0)
        ctx->r9 = MEM_HU(ctx->r16, 0X38);
            goto L_80187BF0;
    }
    goto skip_26;
    // 0x80187BC0: lhu         $t1, 0x38($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X38);
    skip_26:
    // 0x80187BC4: jal         0x801885A4
    // 0x80187BC8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Interactable_stopInteraction(rdram, ctx);
        goto after_15;
    // 0x80187BC8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_15:
    // 0x80187BCC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187BD0: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80187BD4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80187BD8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80187BDC: jalr        $t9
    // 0x80187BE0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_16;
    // 0x80187BE0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_16:
    // 0x80187BE4: b           L_80187C68
    // 0x80187BE8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80187C68;
    // 0x80187BE8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80187BEC:
    // 0x80187BEC: lhu         $t1, 0x38($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X38);
L_80187BF0:
    // 0x80187BF0: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x80187BF4: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80187BF8: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80187BFC: addu        $t2, $t2, $t1
    ctx->r10 = ADD32(ctx->r10, ctx->r9);
    // 0x80187C00: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80187C04: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x80187C08: lhu         $t3, 0x6470($t3)
    ctx->r11 = MEM_HU(ctx->r11, 0X6470);
    // 0x80187C0C: andi        $t4, $t3, 0x8000
    ctx->r12 = ctx->r11 & 0X8000;
    // 0x80187C10: beql        $t4, $zero, L_80187C2C
    if (ctx->r12 == 0) {
        // 0x80187C14: lw          $t7, 0x2908($a3)
        ctx->r15 = MEM_W(ctx->r7, 0X2908);
            goto L_80187C2C;
    }
    goto skip_27;
    // 0x80187C14: lw          $t7, 0x2908($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X2908);
    skip_27:
    // 0x80187C18: lw          $t5, 0x2908($a3)
    ctx->r13 = MEM_W(ctx->r7, 0X2908);
    // 0x80187C1C: ori         $t6, $t5, 0x40
    ctx->r14 = ctx->r13 | 0X40;
    // 0x80187C20: b           L_80187C44
    // 0x80187C24: sw          $t6, 0x2908($a3)
    MEM_W(0X2908, ctx->r7) = ctx->r14;
        goto L_80187C44;
    // 0x80187C24: sw          $t6, 0x2908($a3)
    MEM_W(0X2908, ctx->r7) = ctx->r14;
    // 0x80187C28: lw          $t7, 0x2908($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X2908);
L_80187C2C:
    // 0x80187C2C: ori         $t8, $t7, 0x40
    ctx->r24 = ctx->r15 | 0X40;
    // 0x80187C30: ori         $t9, $t8, 0x8
    ctx->r25 = ctx->r24 | 0X8;
    // 0x80187C34: sw          $t8, 0x2908($a3)
    MEM_W(0X2908, ctx->r7) = ctx->r24;
    // 0x80187C38: sw          $t9, 0x2908($a3)
    MEM_W(0X2908, ctx->r7) = ctx->r25;
    // 0x80187C3C: ori         $t2, $t9, 0x10
    ctx->r10 = ctx->r25 | 0X10;
    // 0x80187C40: sw          $t2, 0x2908($a3)
    MEM_W(0X2908, ctx->r7) = ctx->r10;
L_80187C44:
    // 0x80187C44: jal         0x80179750
    // 0x80187C48: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
    func_80179750(rdram, ctx);
        goto after_17;
    // 0x80187C48: lw          $a0, 0x2B6C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X2B6C);
    after_17:
L_80187C4C:
    // 0x80187C4C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187C50: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80187C54: sw          $zero, 0x60($s0)
    MEM_W(0X60, ctx->r16) = 0;
    // 0x80187C58: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80187C5C: jalr        $t9
    // 0x80187C60: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_18;
    // 0x80187C60: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_18:
L_80187C64:
    // 0x80187C64: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80187C68:
    // 0x80187C68: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80187C6C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80187C70: jr          $ra
    // 0x80187C74: nop

    return;
    // 0x80187C74: nop

;}
RECOMP_FUNC void Interactable_InitCheck(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, init-check-entry ---
    lod_interact_trace_state(rdram, "init-check-entry", ctx->r4);
    // --- END PATCH ---
#endif
    // 0x80187C78: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80187C7C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80187C80: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80187C84: lhu         $v1, 0x38($a0)
    ctx->r3 = MEM_HU(ctx->r4, 0X38);
    // 0x80187C88: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80187C8C: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80187C90: addiu       $t6, $v1, 0x1
    ctx->r14 = ADD32(ctx->r3, 0X1);
    // 0x80187C94: slti        $at, $t6, 0x31
    ctx->r1 = SIGNED(ctx->r14) < 0X31 ? 1 : 0;
    // 0x80187C98: beq         $at, $zero, L_80187D5C
    if (ctx->r1 == 0) {
        // 0x80187C9C: sll         $t7, $v1, 2
        ctx->r15 = S32(ctx->r3 << 2);
            goto L_80187D5C;
    }
    // 0x80187C9C: sll         $t7, $v1, 2
    ctx->r15 = S32(ctx->r3 << 2);
    // 0x80187CA0: addu        $t7, $t7, $v1
    ctx->r15 = ADD32(ctx->r15, ctx->r3);
    // 0x80187CA4: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80187CA8: addu        $a0, $a0, $t7
    ctx->r4 = ADD32(ctx->r4, ctx->r15);
    // 0x80187CAC: jal         0x8008F5E0
    // 0x80187CB0: lhu         $a0, 0x677A($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X677A);
    func_8008F5E0(rdram, ctx);
        goto after_0;
    // 0x80187CB0: lhu         $a0, 0x677A($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X677A);
    after_0:
    // 0x80187CB4: jal         0x80087070
    // 0x80187CB8: lbu         $a0, 0xC($v0)
    ctx->r4 = MEM_BU(ctx->r2, 0XC);
    func_80087070(rdram, ctx);
        goto after_1;
    // 0x80187CB8: lbu         $a0, 0xC($v0)
    ctx->r4 = MEM_BU(ctx->r2, 0XC);
    after_1:
    // 0x80187CBC: beq         $v0, $zero, L_80187DF0
    if (ctx->r2 == 0) {
        // 0x80187CC0: sw          $v0, 0x60($s0)
        MEM_W(0X60, ctx->r16) = ctx->r2;
            goto L_80187DF0;
    }
    // 0x80187CC0: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
    // 0x80187CC4: lhu         $v1, 0x38($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0X38);
    // 0x80187CC8: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80187CCC: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x80187CD0: sll         $t8, $v1, 2
    ctx->r24 = S32(ctx->r3 << 2);
    // 0x80187CD4: addu        $t8, $t8, $v1
    ctx->r24 = ADD32(ctx->r24, ctx->r3);
    // 0x80187CD8: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80187CDC: addu        $a0, $a0, $t8
    ctx->r4 = ADD32(ctx->r4, ctx->r24);
    // 0x80187CE0: lhu         $a0, 0x677A($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X677A);
    // 0x80187CE4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187CE8: addiu       $t9, $t9, 0x2410
    ctx->r25 = ADD32(ctx->r25, 0X2410);
    // 0x80187CEC: bne         $a0, $at, L_80187D1C
    if (ctx->r4 != ctx->r1) {
        // 0x80187CF0: addiu       $a1, $zero, 0x219D
        ctx->r5 = ADD32(0, 0X219D);
            goto L_80187D1C;
    }
    // 0x80187CF0: addiu       $a1, $zero, 0x219D
    ctx->r5 = ADD32(0, 0X219D);
    // 0x80187CF4: jalr        $t9
    // 0x80187CF8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80187CF8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x80187CFC: sw          $s0, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->r16;
    // 0x80187D00: lhu         $v1, 0x38($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0X38);
    // 0x80187D04: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80187D08: sll         $t0, $v1, 2
    ctx->r8 = S32(ctx->r3 << 2);
    // 0x80187D0C: addu        $t0, $t0, $v1
    ctx->r8 = ADD32(ctx->r8, ctx->r3);
    // 0x80187D10: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x80187D14: addu        $a0, $a0, $t0
    ctx->r4 = ADD32(ctx->r4, ctx->r8);
    // 0x80187D18: lhu         $a0, 0x677A($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X677A);
L_80187D1C:
    // 0x80187D1C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80187D20: bne         $a0, $at, L_80187D5C
    if (ctx->r4 != ctx->r1) {
        // 0x80187D24: lui         $v0, 0x801D
        ctx->r2 = S32(0X801D << 16);
            goto L_80187D5C;
    }
    // 0x80187D24: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80187D28: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80187D2C: lw          $t1, 0x2908($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X2908);
    // 0x80187D30: lw          $a0, 0x2B6C($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X2B6C);
    // 0x80187D34: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80187D38: ori         $t2, $t1, 0x40
    ctx->r10 = ctx->r9 | 0X40;
    // 0x80187D3C: ori         $t4, $t2, 0x8
    ctx->r12 = ctx->r10 | 0X8;
    // 0x80187D40: sw          $t2, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r10;
    // 0x80187D44: sw          $t4, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r12;
    // 0x80187D48: ori         $t6, $t4, 0x10
    ctx->r14 = ctx->r12 | 0X10;
    // 0x80187D4C: jal         0x80179750
    // 0x80187D50: sw          $t6, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r14;
    func_80179750(rdram, ctx);
        goto after_3;
    // 0x80187D50: sw          $t6, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r14;
    after_3:
    // 0x80187D54: sh          $zero, 0x4($s0)
    MEM_H(0X4, ctx->r16) = 0;
    // 0x80187D58: lhu         $v1, 0x38($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0X38);
L_80187D5C:
    // 0x80187D5C: addiu       $t7, $v1, 0x1
    ctx->r15 = ADD32(ctx->r3, 0X1);
    // 0x80187D60: slti        $at, $t7, 0x31
    ctx->r1 = SIGNED(ctx->r15) < 0X31 ? 1 : 0;
    // 0x80187D64: bne         $at, $zero, L_80187DDC
    if (ctx->r1 != 0) {
        // 0x80187D68: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80187DDC;
    }
    // 0x80187D68: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80187D6C: addiu       $v1, $v1, -0x30
    ctx->r3 = ADD32(ctx->r3, -0X30);
    // 0x80187D70: slti        $at, $v1, 0x28
    ctx->r1 = SIGNED(ctx->r3) < 0X28 ? 1 : 0;
    // 0x80187D74: bne         $at, $zero, L_80187DC0
    if (ctx->r1 != 0) {
        // 0x80187D78: sll         $t8, $v0, 2
        ctx->r24 = S32(ctx->r2 << 2);
            goto L_80187DC0;
    }
    // 0x80187D78: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x80187D7C: addu        $t8, $t8, $v0
    ctx->r24 = ADD32(ctx->r24, ctx->r2);
    // 0x80187D80: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80187D84: addiu       $t9, $t9, 0x6778
    ctx->r25 = ADD32(ctx->r25, 0X6778);
    // 0x80187D88: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80187D8C: addu        $v1, $t8, $t9
    ctx->r3 = ADD32(ctx->r24, ctx->r25);
    // 0x80187D90: lhu         $t0, -0x308($v1)
    ctx->r8 = MEM_HU(ctx->r3, -0X308);
    // 0x80187D94: andi        $t1, $t0, 0x8000
    ctx->r9 = ctx->r8 & 0X8000;
    // 0x80187D98: beq         $t1, $zero, L_80187DB0
    if (ctx->r9 == 0) {
        // 0x80187D9C: nop
    
            goto L_80187DB0;
    }
    // 0x80187D9C: nop

    // 0x80187DA0: jal         0x80087344
    // 0x80187DA4: lhu         $a0, 0x3A($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0X3A);
    func_80087344(rdram, ctx);
        goto after_4;
    // 0x80187DA4: lhu         $a0, 0x3A($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0X3A);
    after_4:
    // 0x80187DA8: b           L_80187DCC
    // 0x80187DAC: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
        goto L_80187DCC;
    // 0x80187DAC: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
L_80187DB0:
    // 0x80187DB0: jal         0x80087344
    // 0x80187DB4: lhu         $a0, -0x30A($v1)
    ctx->r4 = MEM_HU(ctx->r3, -0X30A);
    func_80087344(rdram, ctx);
        goto after_5;
    // 0x80187DB4: lhu         $a0, -0x30A($v1)
    ctx->r4 = MEM_HU(ctx->r3, -0X30A);
    after_5:
    // 0x80187DB8: b           L_80187DCC
    // 0x80187DBC: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
        goto L_80187DCC;
    // 0x80187DBC: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
L_80187DC0:
    // 0x80187DC0: jal         0x80087344
    // 0x80187DC4: andi        $a0, $v1, 0xFFFF
    ctx->r4 = ctx->r3 & 0XFFFF;
    func_80087344(rdram, ctx);
        goto after_6;
    // 0x80187DC4: andi        $a0, $v1, 0xFFFF
    ctx->r4 = ctx->r3 & 0XFFFF;
    after_6:
    // 0x80187DC8: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
L_80187DCC:
    // 0x80187DCC: lw          $t2, 0x60($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X60);
    // 0x80187DD0: beql        $t2, $zero, L_80187DF4
    if (ctx->r10 == 0) {
        // 0x80187DD4: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80187DF4;
    }
    goto skip_0;
    // 0x80187DD4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80187DD8: sh          $zero, 0x4($s0)
    MEM_H(0X4, ctx->r16) = 0;
L_80187DDC:
    // 0x80187DDC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187DE0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80187DE4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80187DE8: jalr        $t9
    // 0x80187DEC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80187DEC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_7:
L_80187DF0:
    // 0x80187DF0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80187DF4:
    // 0x80187DF4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80187DF8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80187DFC: jr          $ra
    // 0x80187E00: nop

    return;
    // 0x80187E00: nop

;}
RECOMP_FUNC void Interactable_SelectTextboxOption(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, select-option-entry ---
    lod_interact_trace_state(rdram, "select-option-entry", ctx->r4);
    // --- END PATCH ---
#endif
    // 0x80187E04: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80187E08: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80187E0C: lhu         $v1, 0x38($a0)
    ctx->r3 = MEM_HU(ctx->r4, 0X38);
    // 0x80187E10: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80187E14: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80187E18: addiu       $a1, $v1, 0x1
    ctx->r5 = ADD32(ctx->r3, 0X1);
    // 0x80187E1C: slti        $at, $a1, 0x31
    ctx->r1 = SIGNED(ctx->r5) < 0X31 ? 1 : 0;
    // 0x80187E20: beq         $at, $zero, L_80187FB4
    if (ctx->r1 == 0) {
        // 0x80187E24: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80187FB4;
    }
    // 0x80187E24: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80187E28: sll         $t6, $v1, 2
    ctx->r14 = S32(ctx->r3 << 2);
    // 0x80187E2C: addu        $t6, $t6, $v1
    ctx->r14 = ADD32(ctx->r14, ctx->r3);
    // 0x80187E30: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80187E34: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80187E38: lhu         $t7, 0x677A($t7)
    ctx->r15 = MEM_HU(ctx->r15, 0X677A);
    // 0x80187E3C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80187E40: bnel        $t7, $at, L_80187FB8
    if (ctx->r15 != ctx->r1) {
        // 0x80187E44: slti        $at, $a1, 0x31
        ctx->r1 = SIGNED(ctx->r5) < 0X31 ? 1 : 0;
            goto L_80187FB8;
    }
    goto skip_0;
    // 0x80187E44: slti        $at, $a1, 0x31
    ctx->r1 = SIGNED(ctx->r5) < 0X31 ? 1 : 0;
    skip_0:
    // 0x80187E48: jal         0x80087410
    // 0x80187E4C: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    func_80087410(rdram, ctx);
        goto after_0;
    // 0x80187E4C: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    after_0:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, white jewel choice result ---
    lod_interact_trace_choice(rdram, "white-jewel-choice", MEM_W(ctx->r29, 0x28), (uint32_t)ctx->r2);
    // --- END PATCH ---
#endif
    // 0x80187E50: beq         $v0, $zero, L_80187E6C
    if (ctx->r2 == 0) {
        // 0x80187E54: lw          $a3, 0x28($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X28);
            goto L_80187E6C;
    }
    // 0x80187E54: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80187E58: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80187E5C: beq         $v0, $at, L_80187F88
    if (ctx->r2 == ctx->r1) {
        // 0x80187E60: nop
    
            goto L_80187F88;
    }
    // 0x80187E60: nop

    // 0x80187E64: b           L_80188258
    // 0x80187E68: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80187E68: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187E6C:
    // 0x80187E6C: lui         $v1, 0x8008
    ctx->r3 = S32(0X8008 << 16);
    // 0x80187E70: addiu       $v1, $v1, 0x6EB0
    ctx->r3 = ADD32(ctx->r3, 0X6EB0);
    // 0x80187E74: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    // 0x80187E78: addiu       $a0, $zero, 0x400
    ctx->r4 = ADD32(0, 0X400);
    // 0x80187E7C: jalr        $v1
    // 0x80187E80: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_1;
    // 0x80187E80: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_1:
    // 0x80187E84: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80187E88: beq         $v0, $zero, L_80187EB0
    if (ctx->r2 == 0) {
        // 0x80187E8C: lw          $a3, 0x28($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X28);
            goto L_80187EB0;
    }
    // 0x80187E8C: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80187E90: lhu         $v0, 0x52($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X52);
    // 0x80187E94: bne         $v0, $zero, L_80188254
    if (ctx->r2 != 0) {
        // 0x80187E98: addiu       $t8, $v0, 0x1
        ctx->r24 = ADD32(ctx->r2, 0X1);
            goto L_80188254;
    }
    // 0x80187E98: addiu       $t8, $v0, 0x1
    ctx->r24 = ADD32(ctx->r2, 0X1);
    // 0x80187E9C: sh          $t8, 0x52($a3)
    MEM_H(0X52, ctx->r7) = ctx->r24;
    // 0x80187EA0: jal         0x800874B0
    // 0x80187EA4: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    func_800874B0(rdram, ctx);
        goto after_2;
    // 0x80187EA4: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    after_2:
    // 0x80187EA8: b           L_80188258
    // 0x80187EAC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80187EAC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187EB0:
    // 0x80187EB0: addiu       $a0, $zero, 0x800
    ctx->r4 = ADD32(0, 0X800);
    // 0x80187EB4: jalr        $v1
    // 0x80187EB8: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_3;
    // 0x80187EB8: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_3:
    // 0x80187EBC: beq         $v0, $zero, L_80187EE8
    if (ctx->r2 == 0) {
        // 0x80187EC0: lw          $a3, 0x28($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X28);
            goto L_80187EE8;
    }
    // 0x80187EC0: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80187EC4: lhu         $v0, 0x52($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X52);
    // 0x80187EC8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80187ECC: bne         $v0, $at, L_80188254
    if (ctx->r2 != ctx->r1) {
        // 0x80187ED0: addiu       $t9, $v0, -0x1
        ctx->r25 = ADD32(ctx->r2, -0X1);
            goto L_80188254;
    }
    // 0x80187ED0: addiu       $t9, $v0, -0x1
    ctx->r25 = ADD32(ctx->r2, -0X1);
    // 0x80187ED4: sh          $t9, 0x52($a3)
    MEM_H(0X52, ctx->r7) = ctx->r25;
    // 0x80187ED8: jal         0x800874B0
    // 0x80187EDC: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    func_800874B0(rdram, ctx);
        goto after_4;
    // 0x80187EDC: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    after_4:
    // 0x80187EE0: b           L_80188258
    // 0x80187EE4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80187EE4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187EE8:
    // 0x80187EE8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80187EEC: lhu         $v0, -0x7808($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X7808);
    // 0x80187EF0: andi        $t0, $v0, 0x8000
    ctx->r8 = ctx->r2 & 0X8000;
    // 0x80187EF4: bne         $t0, $zero, L_80187F04
    if (ctx->r8 != 0) {
        // 0x80187EF8: andi        $t1, $v0, 0x1
        ctx->r9 = ctx->r2 & 0X1;
            goto L_80187F04;
    }
    // 0x80187EF8: andi        $t1, $v0, 0x1
    ctx->r9 = ctx->r2 & 0X1;
    // 0x80187EFC: beql        $t1, $zero, L_80188258
    if (ctx->r9 == 0) {
        // 0x80187F00: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80188258;
    }
    goto skip_1;
    // 0x80187F00: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
L_80187F04:
    // 0x80187F04: lhu         $t2, 0x52($a3)
    ctx->r10 = MEM_HU(ctx->r7, 0X52);
    // 0x80187F08: bne         $t2, $zero, L_80187F4C
    if (ctx->r10 != 0) {
        // 0x80187F0C: nop
    
            goto L_80187F4C;
    }
    // 0x80187F0C: nop

    // 0x80187F10: jal         0x800873D8
    // 0x80187F14: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_800873D8(rdram, ctx);
        goto after_5;
    // 0x80187F14: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_5:
    // 0x80187F18: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x80187F1C: jal         0x80002410
    // 0x80187F20: addiu       $a1, $zero, 0x219B
    ctx->r5 = ADD32(0, 0X219B);
    object_createAndSetChild(rdram, ctx);
        goto after_6;
    // 0x80187F20: addiu       $a1, $zero, 0x219B
    ctx->r5 = ADD32(0, 0X219B);
    after_6:
    // 0x80187F24: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80187F28: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187F2C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80187F30: lw          $t3, 0x58($a3)
    ctx->r11 = MEM_W(ctx->r7, 0X58);
    // 0x80187F34: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80187F38: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x80187F3C: jalr        $t9
    // 0x80187F40: sh          $t3, 0x40($v0)
    MEM_H(0X40, ctx->r2) = ctx->r11;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80187F40: sh          $t3, 0x40($v0)
    MEM_H(0X40, ctx->r2) = ctx->r11;
    after_7:
    // 0x80187F44: b           L_80188258
    // 0x80187F48: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80187F48: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187F4C:
    // 0x80187F4C: jal         0x80087488
    // 0x80187F50: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_80087488(rdram, ctx);
        goto after_8;
    // 0x80187F50: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_8:
    // 0x80187F54: jal         0x800874DC
    // 0x80187F58: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_800874DC(rdram, ctx);
        goto after_9;
    // 0x80187F58: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_9:
    // 0x80187F5C: jal         0x801885A4
    // 0x80187F60: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    Interactable_stopInteraction(rdram, ctx);
        goto after_10;
    // 0x80187F60: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    after_10:
    // 0x80187F64: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80187F68: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80187F6C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80187F70: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x80187F74: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80187F78: jalr        $t9
    // 0x80187F7C: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x80187F7C: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_11:
    // 0x80187F80: b           L_80188258
    // 0x80187F84: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80187F84: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187F88:
    // 0x80187F88: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80187F8C: lhu         $v0, -0x7808($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X7808);
    // 0x80187F90: andi        $t4, $v0, 0x8000
    ctx->r12 = ctx->r2 & 0X8000;
    // 0x80187F94: bne         $t4, $zero, L_80187FA4
    if (ctx->r12 != 0) {
        // 0x80187F98: andi        $t5, $v0, 0x1
        ctx->r13 = ctx->r2 & 0X1;
            goto L_80187FA4;
    }
    // 0x80187F98: andi        $t5, $v0, 0x1
    ctx->r13 = ctx->r2 & 0X1;
    // 0x80187F9C: beql        $t5, $zero, L_80188258
    if (ctx->r13 == 0) {
        // 0x80187FA0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80188258;
    }
    goto skip_2;
    // 0x80187FA0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
L_80187FA4:
    // 0x80187FA4: jal         0x80087460
    // 0x80187FA8: nop

    func_80087460(rdram, ctx);
        goto after_12;
    // 0x80187FA8: nop

    after_12:
    // 0x80187FAC: b           L_80188258
    // 0x80187FB0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80187FB0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80187FB4:
    // 0x80187FB4: slti        $at, $a1, 0x31
    ctx->r1 = SIGNED(ctx->r5) < 0X31 ? 1 : 0;
L_80187FB8:
    // 0x80187FB8: bne         $at, $zero, L_80188240
    if (ctx->r1 != 0) {
        // 0x80187FBC: addiu       $t6, $v0, -0x30
        ctx->r14 = ADD32(ctx->r2, -0X30);
            goto L_80188240;
    }
    // 0x80187FBC: addiu       $t6, $v0, -0x30
    ctx->r14 = ADD32(ctx->r2, -0X30);
    // 0x80187FC0: slti        $at, $t6, 0x28
    ctx->r1 = SIGNED(ctx->r14) < 0X28 ? 1 : 0;
    // 0x80187FC4: bne         $at, $zero, L_80188240
    if (ctx->r1 != 0) {
        // 0x80187FC8: sll         $t7, $v0, 2
        ctx->r15 = S32(ctx->r2 << 2);
            goto L_80188240;
    }
    // 0x80187FC8: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x80187FCC: addu        $t7, $t7, $v0
    ctx->r15 = ADD32(ctx->r15, ctx->r2);
    // 0x80187FD0: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80187FD4: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80187FD8: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80187FDC: lhu         $t8, 0x6470($t8)
    ctx->r24 = MEM_HU(ctx->r24, 0X6470);
    // 0x80187FE0: andi        $t0, $t8, 0x10
    ctx->r8 = ctx->r24 & 0X10;
    // 0x80187FE4: beq         $t0, $zero, L_80188240
    if (ctx->r8 == 0) {
        // 0x80187FE8: nop
    
            goto L_80188240;
    }
    // 0x80187FE8: nop

    // 0x80187FEC: jal         0x80087410
    // 0x80187FF0: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_80087410(rdram, ctx);
        goto after_13;
    // 0x80187FF0: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_13:
    // 0x80187FF4: beq         $v0, $zero, L_80188010
    if (ctx->r2 == 0) {
        // 0x80187FF8: lw          $a3, 0x28($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X28);
            goto L_80188010;
    }
    // 0x80187FF8: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80187FFC: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80188000: beq         $v0, $at, L_80188214
    if (ctx->r2 == ctx->r1) {
        // 0x80188004: nop
    
            goto L_80188214;
    }
    // 0x80188004: nop

    // 0x80188008: b           L_80188258
    // 0x8018800C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x8018800C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80188010:
    // 0x80188010: lui         $v1, 0x8008
    ctx->r3 = S32(0X8008 << 16);
    // 0x80188014: addiu       $v1, $v1, 0x6EB0
    ctx->r3 = ADD32(ctx->r3, 0X6EB0);
    // 0x80188018: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    // 0x8018801C: addiu       $a0, $zero, 0x400
    ctx->r4 = ADD32(0, 0X400);
    // 0x80188020: jalr        $v1
    // 0x80188024: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_14;
    // 0x80188024: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_14:
    // 0x80188028: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x8018802C: beq         $v0, $zero, L_80188054
    if (ctx->r2 == 0) {
        // 0x80188030: lw          $a3, 0x28($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X28);
            goto L_80188054;
    }
    // 0x80188030: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80188034: lhu         $v0, 0x52($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X52);
    // 0x80188038: bne         $v0, $zero, L_80188254
    if (ctx->r2 != 0) {
        // 0x8018803C: addiu       $t1, $v0, 0x1
        ctx->r9 = ADD32(ctx->r2, 0X1);
            goto L_80188254;
    }
    // 0x8018803C: addiu       $t1, $v0, 0x1
    ctx->r9 = ADD32(ctx->r2, 0X1);
    // 0x80188040: sh          $t1, 0x52($a3)
    MEM_H(0X52, ctx->r7) = ctx->r9;
    // 0x80188044: jal         0x800874B0
    // 0x80188048: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    func_800874B0(rdram, ctx);
        goto after_15;
    // 0x80188048: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    after_15:
    // 0x8018804C: b           L_80188258
    // 0x80188050: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80188050: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80188054:
    // 0x80188054: addiu       $a0, $zero, 0x800
    ctx->r4 = ADD32(0, 0X800);
    // 0x80188058: jalr        $v1
    // 0x8018805C: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_16;
    // 0x8018805C: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_16:
    // 0x80188060: beq         $v0, $zero, L_8018808C
    if (ctx->r2 == 0) {
        // 0x80188064: lw          $a3, 0x28($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X28);
            goto L_8018808C;
    }
    // 0x80188064: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80188068: lhu         $v0, 0x52($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X52);
    // 0x8018806C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80188070: bne         $v0, $at, L_80188254
    if (ctx->r2 != ctx->r1) {
        // 0x80188074: addiu       $t2, $v0, -0x1
        ctx->r10 = ADD32(ctx->r2, -0X1);
            goto L_80188254;
    }
    // 0x80188074: addiu       $t2, $v0, -0x1
    ctx->r10 = ADD32(ctx->r2, -0X1);
    // 0x80188078: sh          $t2, 0x52($a3)
    MEM_H(0X52, ctx->r7) = ctx->r10;
    // 0x8018807C: jal         0x800874B0
    // 0x80188080: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    func_800874B0(rdram, ctx);
        goto after_17;
    // 0x80188080: lbu         $a0, 0x53($a3)
    ctx->r4 = MEM_BU(ctx->r7, 0X53);
    after_17:
    // 0x80188084: b           L_80188258
    // 0x80188088: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80188088: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8018808C:
    // 0x8018808C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80188090: lhu         $v0, -0x7808($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X7808);
    // 0x80188094: andi        $t3, $v0, 0x8000
    ctx->r11 = ctx->r2 & 0X8000;
    // 0x80188098: bne         $t3, $zero, L_801880A8
    if (ctx->r11 != 0) {
        // 0x8018809C: andi        $t9, $v0, 0x1
        ctx->r25 = ctx->r2 & 0X1;
            goto L_801880A8;
    }
    // 0x8018809C: andi        $t9, $v0, 0x1
    ctx->r25 = ctx->r2 & 0X1;
    // 0x801880A0: beq         $t9, $zero, L_801881F4
    if (ctx->r25 == 0) {
        // 0x801880A4: andi        $t7, $v0, 0x4000
        ctx->r15 = ctx->r2 & 0X4000;
            goto L_801881F4;
    }
    // 0x801880A4: andi        $t7, $v0, 0x4000
    ctx->r15 = ctx->r2 & 0X4000;
L_801880A8:
    // 0x801880A8: lhu         $t4, 0x52($a3)
    ctx->r12 = MEM_HU(ctx->r7, 0X52);
    // 0x801880AC: bne         $t4, $zero, L_801881D8
    if (ctx->r12 != 0) {
        // 0x801880B0: nop
    
            goto L_801881D8;
    }
    // 0x801880B0: nop

    // 0x801880B4: lhu         $t5, 0x38($a3)
    ctx->r13 = MEM_HU(ctx->r7, 0X38);
    // 0x801880B8: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x801880BC: addiu       $t7, $t7, 0x6778
    ctx->r15 = ADD32(ctx->r15, 0X6778);
    // 0x801880C0: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801880C4: addu        $t6, $t6, $t5
    ctx->r14 = ADD32(ctx->r14, ctx->r13);
    // 0x801880C8: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x801880CC: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
    // 0x801880D0: lhu         $v1, -0x308($v0)
    ctx->r3 = MEM_HU(ctx->r2, -0X308);
    // 0x801880D4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801880D8: andi        $t8, $v1, 0x20
    ctx->r24 = ctx->r3 & 0X20;
    // 0x801880DC: beql        $t8, $zero, L_80188134
    if (ctx->r24 == 0) {
        // 0x801880E0: andi        $t6, $v1, 0x80
        ctx->r14 = ctx->r3 & 0X80;
            goto L_80188134;
    }
    goto skip_3;
    // 0x801880E0: andi        $t6, $v1, 0x80
    ctx->r14 = ctx->r3 & 0X80;
    skip_3:
    // 0x801880E4: jal         0x800874DC
    // 0x801880E8: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_800874DC(rdram, ctx);
        goto after_18;
    // 0x801880E8: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_18:
    // 0x801880EC: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x801880F0: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x801880F4: addiu       $t2, $t2, 0x6778
    ctx->r10 = ADD32(ctx->r10, 0X6778);
    // 0x801880F8: lhu         $t0, 0x38($a3)
    ctx->r8 = MEM_HU(ctx->r7, 0X38);
    // 0x801880FC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80188100: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x80188104: addu        $t1, $t1, $t0
    ctx->r9 = ADD32(ctx->r9, ctx->r8);
    // 0x80188108: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x8018810C: addu        $t3, $t1, $t2
    ctx->r11 = ADD32(ctx->r9, ctx->r10);
    // 0x80188110: lhu         $t9, -0x300($t3)
    ctx->r25 = MEM_HU(ctx->r11, -0X300);
    // 0x80188114: sw          $t9, -0x5178($at)
    MEM_W(-0X5178, ctx->r1) = ctx->r25;
    // 0x80188118: lhu         $t4, 0x38($a3)
    ctx->r12 = MEM_HU(ctx->r7, 0X38);
    // 0x8018811C: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x80188120: addu        $t5, $t5, $t4
    ctx->r13 = ADD32(ctx->r13, ctx->r12);
    // 0x80188124: sll         $t5, $t5, 2
    ctx->r13 = S32(ctx->r13 << 2);
    // 0x80188128: addu        $v0, $t5, $t2
    ctx->r2 = ADD32(ctx->r13, ctx->r10);
    // 0x8018812C: lhu         $v1, -0x308($v0)
    ctx->r3 = MEM_HU(ctx->r2, -0X308);
    // 0x80188130: andi        $t6, $v1, 0x80
    ctx->r14 = ctx->r3 & 0X80;
L_80188134:
    // 0x80188134: beq         $t6, $zero, L_80188178
    if (ctx->r14 == 0) {
        // 0x80188138: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_80188178;
    }
    // 0x80188138: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8018813C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80188140: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x80188144: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80188148: lw          $a1, -0x304($v0)
    ctx->r5 = MEM_W(ctx->r2, -0X304);
    // 0x8018814C: jalr        $t9
    // 0x80188150: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_19;
    // 0x80188150: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_19:
    // 0x80188154: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80188158: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x8018815C: addiu       $t0, $t0, 0x6778
    ctx->r8 = ADD32(ctx->r8, 0X6778);
    // 0x80188160: lhu         $t7, 0x38($a3)
    ctx->r15 = MEM_HU(ctx->r7, 0X38);
    // 0x80188164: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80188168: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x8018816C: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80188170: addu        $v0, $t8, $t0
    ctx->r2 = ADD32(ctx->r24, ctx->r8);
    // 0x80188174: lhu         $v1, -0x308($v0)
    ctx->r3 = MEM_HU(ctx->r2, -0X308);
L_80188178:
    // 0x80188178: andi        $t1, $v1, 0x40
    ctx->r9 = ctx->r3 & 0X40;
    // 0x8018817C: beq         $t1, $zero, L_801881BC
    if (ctx->r9 == 0) {
        // 0x80188180: addiu       $a2, $zero, 0x6
        ctx->r6 = ADD32(0, 0X6);
            goto L_801881BC;
    }
    // 0x80188180: addiu       $a2, $zero, 0x6
    ctx->r6 = ADD32(0, 0X6);
    // 0x80188184: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80188188: addiu       $t9, $t9, -0x5C08
    ctx->r25 = ADD32(ctx->r25, -0X5C08);
    // 0x8018818C: lh          $a0, -0x300($v0)
    ctx->r4 = MEM_H(ctx->r2, -0X300);
    // 0x80188190: lhu         $a1, -0x2FE($v0)
    ctx->r5 = MEM_HU(ctx->r2, -0X2FE);
    // 0x80188194: jalr        $t9
    // 0x80188198: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_20;
    // 0x80188198: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_20:
    // 0x8018819C: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x801881A0: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x801881A4: lhu         $t3, 0x38($a3)
    ctx->r11 = MEM_HU(ctx->r7, 0X38);
    // 0x801881A8: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x801881AC: addu        $t4, $t4, $t3
    ctx->r12 = ADD32(ctx->r12, ctx->r11);
    // 0x801881B0: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x801881B4: addu        $v1, $v1, $t4
    ctx->r3 = ADD32(ctx->r3, ctx->r12);
    // 0x801881B8: lhu         $v1, 0x6470($v1)
    ctx->r3 = MEM_HU(ctx->r3, 0X6470);
L_801881BC:
    // 0x801881BC: andi        $t5, $v1, 0x100
    ctx->r13 = ctx->r3 & 0X100;
    // 0x801881C0: beq         $t5, $zero, L_801881D8
    if (ctx->r13 == 0) {
        // 0x801881C4: lui         $t2, 0x801D
        ctx->r10 = S32(0X801D << 16);
            goto L_801881D8;
    }
    // 0x801881C4: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801881C8: lw          $t2, -0x523C($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X523C);
    // 0x801881CC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801881D0: ori         $t6, $t2, 0x1
    ctx->r14 = ctx->r10 | 0X1;
    // 0x801881D4: sw          $t6, -0x523C($at)
    MEM_W(-0X523C, ctx->r1) = ctx->r14;
L_801881D8:
    // 0x801881D8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801881DC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801881E0: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x801881E4: jalr        $t9
    // 0x801881E8: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_21;
    // 0x801881E8: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_21:
    // 0x801881EC: b           L_80188258
    // 0x801881F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x801881F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801881F4:
    // 0x801881F4: beq         $t7, $zero, L_80188254
    if (ctx->r15 == 0) {
        // 0x801881F8: addiu       $a0, $a3, 0x8
        ctx->r4 = ADD32(ctx->r7, 0X8);
            goto L_80188254;
    }
    // 0x801881F8: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x801881FC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80188200: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80188204: jalr        $t9
    // 0x80188208: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_22;
    // 0x80188208: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_22:
    // 0x8018820C: b           L_80188258
    // 0x80188210: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x80188210: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80188214:
    // 0x80188214: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80188218: lhu         $v0, -0x7808($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X7808);
    // 0x8018821C: andi        $t8, $v0, 0x8000
    ctx->r24 = ctx->r2 & 0X8000;
    // 0x80188220: bne         $t8, $zero, L_80188230
    if (ctx->r24 != 0) {
        // 0x80188224: andi        $t0, $v0, 0x1
        ctx->r8 = ctx->r2 & 0X1;
            goto L_80188230;
    }
    // 0x80188224: andi        $t0, $v0, 0x1
    ctx->r8 = ctx->r2 & 0X1;
    // 0x80188228: beql        $t0, $zero, L_80188258
    if (ctx->r8 == 0) {
        // 0x8018822C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80188258;
    }
    goto skip_4;
    // 0x8018822C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_4:
L_80188230:
    // 0x80188230: jal         0x80087460
    // 0x80188234: nop

    func_80087460(rdram, ctx);
        goto after_23;
    // 0x80188234: nop

    after_23:
    // 0x80188238: b           L_80188258
    // 0x8018823C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80188258;
    // 0x8018823C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80188240:
    // 0x80188240: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80188244: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80188248: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x8018824C: jalr        $t9
    // 0x80188250: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_24;
    // 0x80188250: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_24:
L_80188254:
    // 0x80188254: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80188258:
    // 0x80188258: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8018825C: jr          $ra
    // 0x80188260: nop

    return;
    // 0x80188260: nop

;}
RECOMP_FUNC void Interactable_StopCheck(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, stop-check-entry ---
    lod_interact_trace_state(rdram, "stop-check-entry", ctx->r4);
    // --- END PATCH ---
#endif
    // 0x80188264: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80188268: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8018826C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80188270: lhu         $v1, 0x38($a0)
    ctx->r3 = MEM_HU(ctx->r4, 0X38);
    // 0x80188274: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80188278: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x8018827C: addiu       $v0, $v1, 0x1
    ctx->r2 = ADD32(ctx->r3, 0X1);
    // 0x80188280: slti        $at, $v0, 0x31
    ctx->r1 = SIGNED(ctx->r2) < 0X31 ? 1 : 0;
    // 0x80188284: beq         $at, $zero, L_8018833C
    if (ctx->r1 == 0) {
        // 0x80188288: sll         $t6, $v1, 2
        ctx->r14 = S32(ctx->r3 << 2);
            goto L_8018833C;
    }
    // 0x80188288: sll         $t6, $v1, 2
    ctx->r14 = S32(ctx->r3 << 2);
    // 0x8018828C: addu        $t6, $t6, $v1
    ctx->r14 = ADD32(ctx->r14, ctx->r3);
    // 0x80188290: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80188294: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80188298: lhu         $t7, 0x677A($t7)
    ctx->r15 = MEM_HU(ctx->r15, 0X677A);
    // 0x8018829C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801882A0: bnel        $t7, $at, L_801882F0
    if (ctx->r15 != ctx->r1) {
        // 0x801882A4: lw          $t8, 0x60($s0)
        ctx->r24 = MEM_W(ctx->r16, 0X60);
            goto L_801882F0;
    }
    goto skip_0;
    // 0x801882A4: lw          $t8, 0x60($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X60);
    skip_0:
    // 0x801882A8: jal         0x80002560
    // 0x801882AC: addiu       $a0, $zero, 0x219B
    ctx->r4 = ADD32(0, 0X219B);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x801882AC: addiu       $a0, $zero, 0x219B
    ctx->r4 = ADD32(0, 0X219B);
    after_0:
    // 0x801882B0: bnel        $v0, $zero, L_801882E4
    if (ctx->r2 != 0) {
        // 0x801882B4: lhu         $v0, 0x38($s0)
        ctx->r2 = MEM_HU(ctx->r16, 0X38);
            goto L_801882E4;
    }
    goto skip_1;
    // 0x801882B4: lhu         $v0, 0x38($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X38);
    skip_1:
    // 0x801882B8: jal         0x800874DC
    // 0x801882BC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_800874DC(rdram, ctx);
        goto after_1;
    // 0x801882BC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_1:
    // 0x801882C0: jal         0x801885A4
    // 0x801882C4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Interactable_stopInteraction(rdram, ctx);
        goto after_2;
    // 0x801882C4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x801882C8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801882CC: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801882D0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801882D4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801882D8: jalr        $t9
    // 0x801882DC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801882DC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_3:
    // 0x801882E0: lhu         $v0, 0x38($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X38);
L_801882E4:
    // 0x801882E4: b           L_8018833C
    // 0x801882E8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
        goto L_8018833C;
    // 0x801882E8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x801882EC: lw          $t8, 0x60($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X60);
L_801882F0:
    // 0x801882F0: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x801882F4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801882F8: bne         $t8, $at, L_80188328
    if (ctx->r24 != ctx->r1) {
        // 0x801882FC: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80188328;
    }
    // 0x801882FC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80188300: jal         0x801885A4
    // 0x80188304: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Interactable_stopInteraction(rdram, ctx);
        goto after_4;
    // 0x80188304: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_4:
    // 0x80188308: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018830C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80188310: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80188314: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80188318: jalr        $t9
    // 0x8018831C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x8018831C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_5:
    // 0x80188320: b           L_8018841C
    // 0x80188324: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8018841C;
    // 0x80188324: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80188328:
    // 0x80188328: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8018832C: jalr        $t9
    // 0x80188330: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x80188330: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_6:
    // 0x80188334: lhu         $v0, 0x38($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X38);
    // 0x80188338: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_8018833C:
    // 0x8018833C: slti        $at, $v0, 0x31
    ctx->r1 = SIGNED(ctx->r2) < 0X31 ? 1 : 0;
    // 0x80188340: bnel        $at, $zero, L_8018841C
    if (ctx->r1 != 0) {
        // 0x80188344: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8018841C;
    }
    goto skip_2;
    // 0x80188344: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x80188348: jal         0x80087410
    // 0x8018834C: nop

    func_80087410(rdram, ctx);
        goto after_7;
    // 0x8018834C: nop

    after_7:
    // 0x80188350: beq         $v0, $zero, L_8018836C
    if (ctx->r2 == 0) {
        // 0x80188354: addiu       $t0, $s0, 0x8
        ctx->r8 = ADD32(ctx->r16, 0X8);
            goto L_8018836C;
    }
    // 0x80188354: addiu       $t0, $s0, 0x8
    ctx->r8 = ADD32(ctx->r16, 0X8);
    // 0x80188358: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8018835C: beq         $v0, $at, L_801883F4
    if (ctx->r2 == ctx->r1) {
        // 0x80188360: nop
    
            goto L_801883F4;
    }
    // 0x80188360: nop

    // 0x80188364: b           L_8018841C
    // 0x80188368: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8018841C;
    // 0x80188368: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8018836C:
    // 0x8018836C: addiu       $t1, $s0, 0xE
    ctx->r9 = ADD32(ctx->r16, 0XE);
    // 0x80188370: sw          $t1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r9;
    // 0x80188374: jal         0x80087488
    // 0x80188378: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    func_80087488(rdram, ctx);
        goto after_8;
    // 0x80188378: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    after_8:
    // 0x8018837C: jal         0x800874DC
    // 0x80188380: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_800874DC(rdram, ctx);
        goto after_9;
    // 0x80188380: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_9:
    // 0x80188384: lhu         $t2, 0x38($s0)
    ctx->r10 = MEM_HU(ctx->r16, 0X38);
    // 0x80188388: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x8018838C: lui         $t6, 0x8000
    ctx->r14 = S32(0X8000 << 16);
    // 0x80188390: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80188394: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x80188398: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x8018839C: addu        $t4, $t4, $t3
    ctx->r12 = ADD32(ctx->r12, ctx->r11);
    // 0x801883A0: lhu         $t4, 0x6470($t4)
    ctx->r12 = MEM_HU(ctx->r12, 0X6470);
    // 0x801883A4: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x801883A8: addiu       $t6, $t6, 0x1E30
    ctx->r14 = ADD32(ctx->r14, 0X1E30);
    // 0x801883AC: andi        $t5, $t4, 0x2
    ctx->r13 = ctx->r12 & 0X2;
    // 0x801883B0: beq         $t5, $zero, L_801883D0
    if (ctx->r13 == 0) {
        // 0x801883B4: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_801883D0;
    }
    // 0x801883B4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801883B8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801883BC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801883C0: jalr        $t9
    // 0x801883C4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x801883C4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    after_10:
    // 0x801883C8: b           L_8018841C
    // 0x801883CC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8018841C;
    // 0x801883CC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801883D0:
    // 0x801883D0: jal         0x801885A4
    // 0x801883D4: sw          $t6, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r14;
    Interactable_stopInteraction(rdram, ctx);
        goto after_11;
    // 0x801883D4: sw          $t6, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r14;
    after_11:
    // 0x801883D8: lw          $t9, 0x20($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X20);
    // 0x801883DC: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x801883E0: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x801883E4: jalr        $t9
    // 0x801883E8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x801883E8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_12:
    // 0x801883EC: b           L_8018841C
    // 0x801883F0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8018841C;
    // 0x801883F0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801883F4:
    // 0x801883F4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801883F8: lhu         $v0, -0x7808($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X7808);
    // 0x801883FC: andi        $t7, $v0, 0x8000
    ctx->r15 = ctx->r2 & 0X8000;
    // 0x80188400: bne         $t7, $zero, L_80188410
    if (ctx->r15 != 0) {
        // 0x80188404: andi        $t8, $v0, 0x1
        ctx->r24 = ctx->r2 & 0X1;
            goto L_80188410;
    }
    // 0x80188404: andi        $t8, $v0, 0x1
    ctx->r24 = ctx->r2 & 0X1;
    // 0x80188408: beql        $t8, $zero, L_8018841C
    if (ctx->r24 == 0) {
        // 0x8018840C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8018841C;
    }
    goto skip_3;
    // 0x8018840C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_3:
L_80188410:
    // 0x80188410: jal         0x80087460
    // 0x80188414: nop

    func_80087460(rdram, ctx);
        goto after_13;
    // 0x80188414: nop

    after_13:
    // 0x80188418: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8018841C:
    // 0x8018841C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80188420: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80188424: jr          $ra
    // 0x80188428: nop

    return;
    // 0x80188428: nop

;}
RECOMP_FUNC void Interactable_Destroy(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, destroy-entry ---
    lod_interact_trace_state(rdram, "destroy-entry", ctx->r4);
    // --- END PATCH ---
#endif
    // 0x8018842C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80188430: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80188434: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80188438: lhu         $v0, 0x38($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X38);
    // 0x8018843C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80188440: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80188444: addiu       $t6, $v0, 0x1
    ctx->r14 = ADD32(ctx->r2, 0X1);
    // 0x80188448: slti        $at, $t6, 0x31
    ctx->r1 = SIGNED(ctx->r14) < 0X31 ? 1 : 0;
    // 0x8018844C: beq         $at, $zero, L_80188580
    if (ctx->r1 == 0) {
        // 0x80188450: sll         $t7, $v0, 2
        ctx->r15 = S32(ctx->r2 << 2);
            goto L_80188580;
    }
    // 0x80188450: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x80188454: addu        $t7, $t7, $v0
    ctx->r15 = ADD32(ctx->r15, ctx->r2);
    // 0x80188458: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x8018845C: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80188460: lhu         $t8, 0x677A($t8)
    ctx->r24 = MEM_HU(ctx->r24, 0X677A);
    // 0x80188464: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x80188468: addiu       $a0, $zero, 0x11
    ctx->r4 = ADD32(0, 0X11);
    // 0x8018846C: bne         $t8, $at, L_801884B8
    if (ctx->r24 != ctx->r1) {
        // 0x80188470: lui         $t9, 0x8009
        ctx->r25 = S32(0X8009 << 16);
            goto L_801884B8;
    }
    // 0x80188470: lui         $t9, 0x8009
    ctx->r25 = S32(0X8009 << 16);
    // 0x80188474: addiu       $t9, $t9, -0x35C
    ctx->r25 = ADD32(ctx->r25, -0X35C);
    // 0x80188478: jalr        $t9
    // 0x8018847C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8018847C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_0:
    // 0x80188480: jal         0x80002560
    // 0x80188484: addiu       $a0, $zero, 0x219D
    ctx->r4 = ADD32(0, 0X219D);
    func_80002560(rdram, ctx);
        goto after_1;
    // 0x80188484: addiu       $a0, $zero, 0x219D
    ctx->r4 = ADD32(0, 0X219D);
    after_1:
    // 0x80188488: bnel        $v0, $zero, L_80188594
    if (ctx->r2 != 0) {
        // 0x8018848C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80188594;
    }
    goto skip_0;
    // 0x8018848C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80188490: jal         0x801885A4
    // 0x80188494: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Interactable_stopInteraction(rdram, ctx);
        goto after_2;
    // 0x80188494: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x80188498: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018849C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801884A0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801884A4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801884A8: jalr        $t9
    // 0x801884AC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801884AC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_3:
    // 0x801884B0: b           L_80188594
    // 0x801884B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80188594;
    // 0x801884B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801884B8:
    // 0x801884B8: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x801884BC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801884C0: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x801884C4: beq         $v0, $zero, L_80188580
    if (ctx->r2 == 0) {
        // 0x801884C8: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80188580;
    }
    // 0x801884C8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801884CC: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x801884D0: jalr        $t9
    // 0x801884D4: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801884D4: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_4:
    // 0x801884D8: lhu         $t0, 0x38($s0)
    ctx->r8 = MEM_HU(ctx->r16, 0X38);
    // 0x801884DC: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x801884E0: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x801884E4: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x801884E8: addu        $t1, $t1, $t0
    ctx->r9 = ADD32(ctx->r9, ctx->r8);
    // 0x801884EC: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x801884F0: addu        $v0, $v0, $t1
    ctx->r2 = ADD32(ctx->r2, ctx->r9);
    // 0x801884F4: lhu         $v0, 0x677A($v0)
    ctx->r2 = MEM_HU(ctx->r2, 0X677A);
    // 0x801884F8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801884FC: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80188500: bne         $v0, $at, L_80188530
    if (ctx->r2 != ctx->r1) {
        // 0x80188504: nop
    
            goto L_80188530;
    }
    // 0x80188504: nop

    // 0x80188508: lw          $t2, 0x2858($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X2858);
    // 0x8018850C: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x80188510: ori         $t3, $t2, 0x100
    ctx->r11 = ctx->r10 | 0X100;
    // 0x80188514: sw          $t3, 0x2858($v1)
    MEM_W(0X2858, ctx->r3) = ctx->r11;
    // 0x80188518: lhu         $t4, 0x38($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0X38);
    // 0x8018851C: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x80188520: addu        $t5, $t5, $t4
    ctx->r13 = ADD32(ctx->r13, ctx->r12);
    // 0x80188524: sll         $t5, $t5, 2
    ctx->r13 = S32(ctx->r13 << 2);
    // 0x80188528: addu        $v0, $v0, $t5
    ctx->r2 = ADD32(ctx->r2, ctx->r13);
    // 0x8018852C: lhu         $v0, 0x677A($v0)
    ctx->r2 = MEM_HU(ctx->r2, 0X677A);
L_80188530:
    // 0x80188530: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80188534: addiu       $at, $zero, 0x5
    ctx->r1 = ADD32(0, 0X5);
    // 0x80188538: bne         $v0, $at, L_80188568
    if (ctx->r2 != ctx->r1) {
        // 0x8018853C: addiu       $v1, $v1, -0x7D40
        ctx->r3 = ADD32(ctx->r3, -0X7D40);
            goto L_80188568;
    }
    // 0x8018853C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80188540: lw          $t6, 0x2858($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2858);
    // 0x80188544: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x80188548: ori         $t7, $t6, 0x200
    ctx->r15 = ctx->r14 | 0X200;
    // 0x8018854C: sw          $t7, 0x2858($v1)
    MEM_W(0X2858, ctx->r3) = ctx->r15;
    // 0x80188550: lhu         $t8, 0x38($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X38);
    // 0x80188554: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80188558: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x8018855C: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x80188560: addu        $v0, $v0, $t9
    ctx->r2 = ADD32(ctx->r2, ctx->r25);
    // 0x80188564: lhu         $v0, 0x677A($v0)
    ctx->r2 = MEM_HU(ctx->r2, 0X677A);
L_80188568:
    // 0x80188568: addiu       $at, $zero, 0x6
    ctx->r1 = ADD32(0, 0X6);
    // 0x8018856C: bnel        $v0, $at, L_80188584
    if (ctx->r2 != ctx->r1) {
        // 0x80188570: lw          $t9, 0x10($s0)
        ctx->r25 = MEM_W(ctx->r16, 0X10);
            goto L_80188584;
    }
    goto skip_1;
    // 0x80188570: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    skip_1:
    // 0x80188574: lw          $t0, 0x2858($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X2858);
    // 0x80188578: ori         $t1, $t0, 0x400
    ctx->r9 = ctx->r8 | 0X400;
    // 0x8018857C: sw          $t1, 0x2858($v1)
    MEM_W(0X2858, ctx->r3) = ctx->r9;
L_80188580:
    // 0x80188580: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
L_80188584:
    // 0x80188584: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80188588: jalr        $t9
    // 0x8018858C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x8018858C: nop

    after_5:
    // 0x80188590: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80188594:
    // 0x80188594: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80188598: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8018859C: jr          $ra
    // 0x801885A0: nop

    return;
    // 0x801885A0: nop

;}
RECOMP_FUNC void Interactable_stopInteraction(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, stop-interaction ---
    lod_interact_trace_state(rdram, "stop-interaction", ctx->r4);
    // --- END PATCH ---
#endif
    // 0x801885A4: sw          $zero, 0x60($a0)
    MEM_W(0X60, ctx->r4) = 0;
    // 0x801885A8: sh          $zero, 0x4($a0)
    MEM_H(0X4, ctx->r4) = 0;
    // 0x801885AC: sh          $zero, 0x52($a0)
    MEM_H(0X52, ctx->r4) = 0;
    // 0x801885B0: jr          $ra
    // 0x801885B4: sw          $zero, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = 0;
    return;
    // 0x801885B4: sw          $zero, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = 0;
;}
RECOMP_FUNC void func_801885B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801885B8: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x801885BC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x801885C0: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x801885C4: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x801885C8: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801885CC: sw          $t6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r14;
    // 0x801885D0: lhu         $t7, 0x38($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X38);
    // 0x801885D4: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x801885D8: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x801885DC: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x801885E0: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x801885E4: addu        $a0, $a0, $t8
    ctx->r4 = ADD32(ctx->r4, ctx->r24);
    // 0x801885E8: jal         0x8008F5E0
    // 0x801885EC: lhu         $a0, 0x677A($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X677A);
    func_8008F5E0(rdram, ctx);
        goto after_0;
    // 0x801885EC: lhu         $a0, 0x677A($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X677A);
    after_0:
    // 0x801885F0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801885F4: sw          $v0, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r2;
    // 0x801885F8: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x801885FC: swc1        $f0, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->f0.u32l;
    // 0x80188600: lhu         $t9, 0x38($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X38);
    // 0x80188604: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    // 0x80188608: addiu       $a2, $zero, 0x15
    ctx->r6 = ADD32(0, 0X15);
    // 0x8018860C: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80188610: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80188614: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x80188618: addu        $t1, $t1, $t0
    ctx->r9 = ADD32(ctx->r9, ctx->r8);
    // 0x8018861C: lhu         $t1, 0x6784($t1)
    ctx->r9 = MEM_HU(ctx->r9, 0X6784);
    // 0x80188620: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x80188624: addiu       $a3, $sp, 0x70
    ctx->r7 = ADD32(ctx->r29, 0X70);
    // 0x80188628: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x8018862C: lui         $t2, 0x2000
    ctx->r10 = S32(0X2000 << 16);
    // 0x80188630: bgez        $t1, L_80188648
    if (SIGNED(ctx->r9) >= 0) {
        // 0x80188634: cvt.s.w     $f6, $f4
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
            goto L_80188648;
    }
    // 0x80188634: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80188638: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x8018863C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80188640: nop

    // 0x80188644: add.s       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f8.fl;
L_80188648:
    // 0x80188648: addiu       $t9, $t9, -0x6A94
    ctx->r25 = ADD32(ctx->r25, -0X6A94);
    // 0x8018864C: swc1        $f6, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->f6.u32l;
    // 0x80188650: swc1        $f0, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->f0.u32l;
    // 0x80188654: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x80188658: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x8018865C: jalr        $t9
    // 0x80188660: sw          $t2, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r10;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80188660: sw          $t2, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r10;
    after_1:
    // 0x80188664: beq         $v0, $zero, L_8018870C
    if (ctx->r2 == 0) {
        // 0x80188668: sw          $v0, 0x60($s0)
        MEM_W(0X60, ctx->r16) = ctx->r2;
            goto L_8018870C;
    }
    // 0x80188668: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
    // 0x8018866C: lw          $t3, 0x7C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X7C);
    // 0x80188670: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80188674: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x80188678: lwc1        $f10, 0x4($t3)
    ctx->f10.u32l = MEM_W(ctx->r11, 0X4);
    // 0x8018867C: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80188680: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x80188684: cvt.d.s     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f2.d = CVT_D_S(ctx->f10.fl);
    // 0x80188688: c.eq.d      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.d == ctx->f2.d;
    // 0x8018868C: nop

    // 0x80188690: bc1t        L_801886D0
    if (c1cs) {
        // 0x80188694: nop
    
            goto L_801886D0;
    }
    // 0x80188694: nop

    // 0x80188698: mtc1        $at, $f13
    ctx->f_odd[(13 - 1) * 2] = ctx->r1;
    // 0x8018869C: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x801886A0: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801886A4: addiu       $t9, $t9, -0x3768
    ctx->r25 = ADD32(ctx->r25, -0X3768);
    // 0x801886A8: mul.d       $f18, $f2, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f12.d); 
    ctx->f18.d = MUL_D(ctx->f2.d, ctx->f12.d);
    // 0x801886AC: addiu       $a1, $sp, 0x64
    ctx->r5 = ADD32(ctx->r29, 0X64);
    // 0x801886B0: addiu       $a2, $zero, 0x2000
    ctx->r6 = ADD32(0, 0X2000);
    // 0x801886B4: div.d       $f4, $f12, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = DIV_D(ctx->f12.d, ctx->f18.d);
    // 0x801886B8: cvt.s.d     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f0.fl = CVT_S_D(ctx->f4.d);
    // 0x801886BC: swc1        $f0, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->f0.u32l;
    // 0x801886C0: swc1        $f0, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f0.u32l;
    // 0x801886C4: swc1        $f0, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f0.u32l;
    // 0x801886C8: jalr        $t9
    // 0x801886CC: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801886CC: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    after_2:
L_801886D0:
    // 0x801886D0: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801886D4: addiu       $t9, $t9, -0x347C
    ctx->r25 = ADD32(ctx->r25, -0X347C);
    // 0x801886D8: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    // 0x801886DC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801886E0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801886E4: jalr        $t9
    // 0x801886E8: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801886E8: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_3:
    // 0x801886EC: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801886F0: addiu       $t9, $t9, -0x2FBC
    ctx->r25 = ADD32(ctx->r25, -0X2FBC);
    // 0x801886F4: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    // 0x801886F8: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801886FC: jalr        $t9
    // 0x80188700: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80188700: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_4:
    // 0x80188704: b           L_80188710
    // 0x80188708: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80188710;
    // 0x80188708: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8018870C:
    // 0x8018870C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80188710:
    // 0x80188710: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80188714: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80188718: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    // 0x8018871C: jr          $ra
    // 0x80188720: nop

    return;
    // 0x80188720: nop

;}
RECOMP_FUNC void Player_getActorCurrentlyInteractingWith(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, scanner locals ---
    const int lod_interact_trace_active = lod_interact_trace_enabled();
    const uint32_t lod_interact_scan_id = ++lod_interact_trace_scan_counter;
    int lod_interact_candidate_count = 0;
    int lod_interact_gate_hit_count = 0;
    // --- END PATCH ---
#endif
    // 0x80188730: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80188734: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80188738: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x8018873C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80188740: lw          $a0, 0x153C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X153C);
    // 0x80188744: lw          $s1, 0x1530($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X1530);
    // 0x80188748: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x8018874C: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80188750: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80188754: sltu        $at, $s1, $a0
    ctx->r1 = ctx->r17 < ctx->r4 ? 1 : 0;
    // 0x80188758: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8018875C: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80188760: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80188764: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80188768: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8018876C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80188770: or          $s5, $zero, $zero
    ctx->r21 = 0 | 0;
    // 0x80188774: sw          $zero, 0x44($sp)
    MEM_W(0X44, ctx->r29) = 0;
    // 0x80188778: or          $s4, $zero, $zero
    ctx->r20 = 0 | 0;
    // 0x8018877C: beq         $at, $zero, L_80188934
    if (ctx->r1 == 0) {
        // 0x80188780: or          $fp, $zero, $zero
        ctx->r30 = 0 | 0;
            goto L_80188934;
    }
    // 0x80188780: or          $fp, $zero, $zero
    ctx->r30 = 0 | 0;
    // 0x80188784: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x80188788: lui         $s3, 0x801D
    ctx->r19 = S32(0X801D << 16);
    // 0x8018878C: lui         $s2, 0x8019
    ctx->r18 = S32(0X8019 << 16);
    // 0x80188790: addiu       $s2, $s2, 0x6740
    ctx->r18 = ADD32(ctx->r18, 0X6740);
    // 0x80188794: addiu       $s3, $s3, -0x7D40
    ctx->r19 = ADD32(ctx->r19, -0X7D40);
    // 0x80188798: addiu       $s6, $s6, 0x6778
    ctx->r22 = ADD32(ctx->r22, 0X6778);
    // 0x8018879C: addiu       $s7, $zero, 0x14
    ctx->r23 = ADD32(0, 0X14);
    // 0x801887A0: lh          $v0, 0x0($s1)
    ctx->r2 = MEM_H(ctx->r17, 0X0);
L_801887A4:
    // 0x801887A4: addiu       $at, $zero, 0x27
    ctx->r1 = ADD32(0, 0X27);
    // 0x801887A8: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x801887AC: bne         $v0, $at, L_801888C0
    if (ctx->r2 != ctx->r1) {
        // 0x801887B0: andi        $v1, $v0, 0x7FF
        ctx->r3 = ctx->r2 & 0X7FF;
            goto L_801888C0;
    }
    // 0x801887B0: andi        $v1, $v0, 0x7FF
    ctx->r3 = ctx->r2 & 0X7FF;
    // 0x801887B4: lwc1        $f12, 0x64($s1)
    ctx->f12.u32l = MEM_W(ctx->r17, 0X64);
    // 0x801887B8: lwc1        $f14, 0x68($s1)
    ctx->f14.u32l = MEM_W(ctx->r17, 0X68);
    // 0x801887BC: jal         0x80188B84
    // 0x801887C0: lw          $a2, 0x40($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X40);
    playerCanInteractWithInteractuable(rdram, ctx);
        goto after_0;
    // 0x801887C0: lw          $a2, 0x40($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X40);
    after_0:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, per-candidate gate result ---
    if (lod_interact_trace_active) {
        lod_interact_candidate_count++;
        if (ctx->r2 != 0) {
            lod_interact_gate_hit_count++;
            lod_interact_trace_object(rdram, "candidate-hit", ctx->r17, (uint32_t)ctx->r2, lod_interact_scan_id);
        } else {
            lod_interact_trace_object(rdram, "candidate-miss", ctx->r17, 0, lod_interact_scan_id);
        }
    }
    // --- END PATCH ---
#endif
    // 0x801887C4: beq         $v0, $zero, L_801888B4
    if (ctx->r2 == 0) {
        // 0x801887C8: or          $fp, $v0, $zero
        ctx->r30 = ctx->r2 | 0;
            goto L_801888B4;
    }
    // 0x801887C8: or          $fp, $v0, $zero
    ctx->r30 = ctx->r2 | 0;
    // 0x801887CC: lhu         $v1, 0x38($s1)
    ctx->r3 = MEM_HU(ctx->r17, 0X38);
    // 0x801887D0: addiu       $t6, $v1, 0x1
    ctx->r14 = ADD32(ctx->r3, 0X1);
    // 0x801887D4: slti        $at, $t6, 0x31
    ctx->r1 = SIGNED(ctx->r14) < 0X31 ? 1 : 0;
    // 0x801887D8: bne         $at, $zero, L_80188860
    if (ctx->r1 != 0) {
        // 0x801887DC: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80188860;
    }
    // 0x801887DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x801887E0: bnel        $s5, $zero, L_801887F4
    if (ctx->r21 != 0) {
        // 0x801887E4: lhu         $v1, 0x38($s5)
        ctx->r3 = MEM_HU(ctx->r21, 0X38);
            goto L_801887F4;
    }
    goto skip_0;
    // 0x801887E4: lhu         $v1, 0x38($s5)
    ctx->r3 = MEM_HU(ctx->r21, 0X38);
    skip_0:
    // 0x801887E8: b           L_801888B4
    // 0x801887EC: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
        goto L_801888B4;
    // 0x801887EC: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
    // 0x801887F0: lhu         $v1, 0x38($s5)
    ctx->r3 = MEM_HU(ctx->r21, 0X38);
L_801887F4:
    // 0x801887F4: addiu       $t8, $v0, -0x30
    ctx->r24 = ADD32(ctx->r2, -0X30);
    // 0x801887F8: addiu       $t7, $v1, -0x30
    ctx->r15 = ADD32(ctx->r3, -0X30);
    // 0x801887FC: slti        $at, $t7, 0x28
    ctx->r1 = SIGNED(ctx->r15) < 0X28 ? 1 : 0;
    // 0x80188800: bne         $at, $zero, L_80188858
    if (ctx->r1 != 0) {
        // 0x80188804: slti        $at, $t8, 0x28
        ctx->r1 = SIGNED(ctx->r24) < 0X28 ? 1 : 0;
            goto L_80188858;
    }
    // 0x80188804: slti        $at, $t8, 0x28
    ctx->r1 = SIGNED(ctx->r24) < 0X28 ? 1 : 0;
    // 0x80188808: bne         $at, $zero, L_80188858
    if (ctx->r1 != 0) {
        // 0x8018880C: nop
    
            goto L_80188858;
    }
    // 0x8018880C: nop

    // 0x80188810: multu       $v0, $s7
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r23)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80188814: mflo        $t9
    ctx->r25 = lo;
    // 0x80188818: addu        $a0, $s6, $t9
    ctx->r4 = ADD32(ctx->r22, ctx->r25);
    // 0x8018881C: lw          $t0, -0x304($a0)
    ctx->r8 = MEM_W(ctx->r4, -0X304);
    // 0x80188820: multu       $v1, $s7
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r23)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80188824: mflo        $t1
    ctx->r9 = lo;
    // 0x80188828: addu        $t2, $s6, $t1
    ctx->r10 = ADD32(ctx->r22, ctx->r9);
    // 0x8018882C: lw          $t3, -0x304($t2)
    ctx->r11 = MEM_W(ctx->r10, -0X304);
    // 0x80188830: bne         $t0, $t3, L_80188850
    if (ctx->r8 != ctx->r11) {
        // 0x80188834: nop
    
            goto L_80188850;
    }
    // 0x80188834: nop

    // 0x80188838: lhu         $t4, -0x308($a0)
    ctx->r12 = MEM_HU(ctx->r4, -0X308);
    // 0x8018883C: andi        $t5, $t4, 0x4
    ctx->r13 = ctx->r12 & 0X4;
    // 0x80188840: beq         $t5, $zero, L_801888B4
    if (ctx->r13 == 0) {
        // 0x80188844: nop
    
            goto L_801888B4;
    }
    // 0x80188844: nop

    // 0x80188848: b           L_801888B4
    // 0x8018884C: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
        goto L_801888B4;
    // 0x8018884C: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
L_80188850:
    // 0x80188850: b           L_801888B4
    // 0x80188854: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
        goto L_801888B4;
    // 0x80188854: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
L_80188858:
    // 0x80188858: b           L_801888B4
    // 0x8018885C: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
        goto L_801888B4;
    // 0x8018885C: or          $s5, $s1, $zero
    ctx->r21 = ctx->r17 | 0;
L_80188860:
    // 0x80188860: sll         $t6, $v1, 2
    ctx->r14 = S32(ctx->r3 << 2);
    // 0x80188864: addu        $t6, $t6, $v1
    ctx->r14 = ADD32(ctx->r14, ctx->r3);
    // 0x80188868: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x8018886C: addu        $t7, $s6, $t6
    ctx->r15 = ADD32(ctx->r22, ctx->r14);
    // 0x80188870: lhu         $a0, 0x2($t7)
    ctx->r4 = MEM_HU(ctx->r15, 0X2);
    // 0x80188874: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80188878: lui         $t9, 0x8009
    ctx->r25 = S32(0X8009 << 16);
    // 0x8018887C: bne         $a0, $at, L_80188898
    if (ctx->r4 != ctx->r1) {
        // 0x80188880: addiu       $t9, $t9, -0x2C0
        ctx->r25 = ADD32(ctx->r25, -0X2C0);
            goto L_80188898;
    }
    // 0x80188880: addiu       $t9, $t9, -0x2C0
    ctx->r25 = ADD32(ctx->r25, -0X2C0);
    // 0x80188884: lh          $t8, 0x2B5C($s3)
    ctx->r24 = MEM_H(ctx->r19, 0X2B5C);
    // 0x80188888: bgez        $t8, L_80188898
    if (SIGNED(ctx->r24) >= 0) {
        // 0x8018888C: nop
    
            goto L_80188898;
    }
    // 0x8018888C: nop

    // 0x80188890: b           L_801888B4
    // 0x80188894: or          $fp, $zero, $zero
    ctx->r30 = 0 | 0;
        goto L_801888B4;
    // 0x80188894: or          $fp, $zero, $zero
    ctx->r30 = 0 | 0;
L_80188898:
    // 0x80188898: jalr        $t9
    // 0x8018889C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8018889C: nop

    after_1:
    // 0x801888A0: beql        $v0, $zero, L_801888B4
    if (ctx->r2 == 0) {
        // 0x801888A4: sw          $s1, 0x44($sp)
        MEM_W(0X44, ctx->r29) = ctx->r17;
            goto L_801888B4;
    }
    goto skip_1;
    // 0x801888A4: sw          $s1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r17;
    skip_1:
    // 0x801888A8: b           L_80188A50
    // 0x801888AC: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
        goto L_80188A50;
    // 0x801888AC: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x801888B0: sw          $s1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r17;
L_801888B4:
    // 0x801888B4: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x801888B8: b           L_80188924
    // 0x801888BC: lw          $a0, 0x153C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X153C);
        goto L_80188924;
    // 0x801888BC: lw          $a0, 0x153C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X153C);
L_801888C0:
    // 0x801888C0: slti        $at, $v1, 0x16
    ctx->r1 = SIGNED(ctx->r3) < 0X16 ? 1 : 0;
    // 0x801888C4: bne         $at, $zero, L_80188924
    if (ctx->r1 != 0) {
        // 0x801888C8: slti        $at, $v1, 0x88
        ctx->r1 = SIGNED(ctx->r3) < 0X88 ? 1 : 0;
            goto L_80188924;
    }
    // 0x801888C8: slti        $at, $v1, 0x88
    ctx->r1 = SIGNED(ctx->r3) < 0X88 ? 1 : 0;
    // 0x801888CC: beq         $at, $zero, L_80188924
    if (ctx->r1 == 0) {
        // 0x801888D0: or          $s0, $zero, $zero
        ctx->r16 = 0 | 0;
            goto L_80188924;
    }
    // 0x801888D0: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x801888D4: sll         $t2, $s0, 2
    ctx->r10 = S32(ctx->r16 << 2);
L_801888D8:
    // 0x801888D8: addu        $t0, $s2, $t2
    ctx->r8 = ADD32(ctx->r18, ctx->r10);
    // 0x801888DC: lh          $t3, 0x0($t0)
    ctx->r11 = MEM_H(ctx->r8, 0X0);
    // 0x801888E0: lh          $t1, 0x0($s1)
    ctx->r9 = MEM_H(ctx->r17, 0X0);
    // 0x801888E4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801888E8: bnel        $t1, $t3, L_80188908
    if (ctx->r9 != ctx->r11) {
        // 0x801888EC: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80188908;
    }
    goto skip_2;
    // 0x801888EC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_2:
    // 0x801888F0: jal         0x80188A80
    // 0x801888F4: lw          $a1, 0x2960($s3)
    ctx->r5 = MEM_W(ctx->r19, 0X2960);
    playerCanInteractWithSpecialTextbox(rdram, ctx);
        goto after_2;
    // 0x801888F4: lw          $a1, 0x2960($s3)
    ctx->r5 = MEM_W(ctx->r19, 0X2960);
    after_2:
    // 0x801888F8: beql        $v0, $zero, L_80188908
    if (ctx->r2 == 0) {
        // 0x801888FC: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80188908;
    }
    goto skip_3;
    // 0x801888FC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_3:
    // 0x80188900: or          $s4, $s1, $zero
    ctx->r20 = ctx->r17 | 0;
    // 0x80188904: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80188908:
    // 0x80188908: sll         $s0, $s0, 16
    ctx->r16 = S32(ctx->r16 << 16);
    // 0x8018890C: sra         $s0, $s0, 16
    ctx->r16 = S32(SIGNED(ctx->r16) >> 16);
    // 0x80188910: slti        $at, $s0, 0x6
    ctx->r1 = SIGNED(ctx->r16) < 0X6 ? 1 : 0;
    // 0x80188914: bnel        $at, $zero, L_801888D8
    if (ctx->r1 != 0) {
        // 0x80188918: sll         $t2, $s0, 2
        ctx->r10 = S32(ctx->r16 << 2);
            goto L_801888D8;
    }
    goto skip_4;
    // 0x80188918: sll         $t2, $s0, 2
    ctx->r10 = S32(ctx->r16 << 2);
    skip_4:
    // 0x8018891C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80188920: lw          $a0, 0x153C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X153C);
L_80188924:
    // 0x80188924: addiu       $s1, $s1, 0x74
    ctx->r17 = ADD32(ctx->r17, 0X74);
    // 0x80188928: sltu        $at, $s1, $a0
    ctx->r1 = ctx->r17 < ctx->r4 ? 1 : 0;
    // 0x8018892C: bnel        $at, $zero, L_801887A4
    if (ctx->r1 != 0) {
        // 0x80188930: lh          $v0, 0x0($s1)
        ctx->r2 = MEM_H(ctx->r17, 0X0);
            goto L_801887A4;
    }
    goto skip_5;
    // 0x80188930: lh          $v0, 0x0($s1)
    ctx->r2 = MEM_H(ctx->r17, 0X0);
    skip_5:
L_80188934:
    // 0x80188934: lw          $t4, 0x44($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X44);
    // 0x80188938: lui         $s3, 0x801D
    ctx->r19 = S32(0X801D << 16);
    // 0x8018893C: addiu       $s3, $s3, -0x7D40
    ctx->r19 = ADD32(ctx->r19, -0X7D40);
    // 0x80188940: beq         $t4, $zero, L_80188950
    if (ctx->r12 == 0) {
        // 0x80188944: nop
    
            goto L_80188950;
    }
    // 0x80188944: nop

    // 0x80188948: b           L_80188A50
    // 0x8018894C: or          $v0, $t4, $zero
    ctx->r2 = ctx->r12 | 0;
        goto L_80188A50;
    // 0x8018894C: or          $v0, $t4, $zero
    ctx->r2 = ctx->r12 | 0;
L_80188950:
    // 0x80188950: beq         $s5, $zero, L_80188960
    if (ctx->r21 == 0) {
        // 0x80188954: nop
    
            goto L_80188960;
    }
    // 0x80188954: nop

    // 0x80188958: b           L_80188A50
    // 0x8018895C: or          $v0, $s5, $zero
    ctx->r2 = ctx->r21 | 0;
        goto L_80188A50;
    // 0x8018895C: or          $v0, $s5, $zero
    ctx->r2 = ctx->r21 | 0;
L_80188960:
    // 0x80188960: beql        $s4, $zero, L_80188974
    if (ctx->r20 == 0) {
        // 0x80188964: lh          $t5, 0x28D0($s3)
        ctx->r13 = MEM_H(ctx->r19, 0X28D0);
            goto L_80188974;
    }
    goto skip_6;
    // 0x80188964: lh          $t5, 0x28D0($s3)
    ctx->r13 = MEM_H(ctx->r19, 0X28D0);
    skip_6:
    // 0x80188968: b           L_80188A50
    // 0x8018896C: or          $v0, $s4, $zero
    ctx->r2 = ctx->r20 | 0;
        goto L_80188A50;
    // 0x8018896C: or          $v0, $s4, $zero
    ctx->r2 = ctx->r20 | 0;
    // 0x80188970: lh          $t5, 0x28D0($s3)
    ctx->r13 = MEM_H(ctx->r19, 0X28D0);
L_80188974:
    // 0x80188974: addiu       $at, $zero, 0xD
    ctx->r1 = ADD32(0, 0XD);
    // 0x80188978: bne         $t5, $at, L_80188A44
    if (ctx->r13 != ctx->r1) {
        // 0x8018897C: nop
    
            goto L_80188A44;
    }
    // 0x8018897C: nop

    // 0x80188980: jal         0x80002560
    // 0x80188984: addiu       $a0, $zero, 0x2026
    ctx->r4 = ADD32(0, 0X2026);
    func_80002560(rdram, ctx);
        goto after_3;
    // 0x80188984: addiu       $a0, $zero, 0x2026
    ctx->r4 = ADD32(0, 0X2026);
    after_3:
    // 0x80188988: beq         $v0, $zero, L_80188A44
    if (ctx->r2 == 0) {
        // 0x8018898C: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_80188A44;
    }
    // 0x8018898C: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x80188990: lui         $at, 0x4170
    ctx->r1 = S32(0X4170 << 16);
    // 0x80188994: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80188998: lwc1        $f2, 0x64($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X64);
    // 0x8018899C: lw          $v1, 0x2960($s3)
    ctx->r3 = MEM_W(ctx->r19, 0X2960);
    // 0x801889A0: sub.s       $f4, $f2, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f2.fl - ctx->f12.fl;
    // 0x801889A4: lwc1        $f0, 0x50($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801889A8: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x801889AC: nop

    // 0x801889B0: bc1f        L_80188A44
    if (!c1cs) {
        // 0x801889B4: nop
    
            goto L_80188A44;
    }
    // 0x801889B4: nop

    // 0x801889B8: add.s       $f6, $f2, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = ctx->f2.fl + ctx->f12.fl;
    // 0x801889BC: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x801889C0: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x801889C4: nop

    // 0x801889C8: bc1f        L_80188A44
    if (!c1cs) {
        // 0x801889CC: nop
    
            goto L_80188A44;
    }
    // 0x801889CC: nop

    // 0x801889D0: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x801889D4: lwc1        $f2, 0x68($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X68);
    // 0x801889D8: lwc1        $f0, 0x54($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X54);
    // 0x801889DC: sub.s       $f8, $f2, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f2.fl - ctx->f12.fl;
    // 0x801889E0: c.le.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl <= ctx->f0.fl;
    // 0x801889E4: nop

    // 0x801889E8: bc1f        L_80188A44
    if (!c1cs) {
        // 0x801889EC: nop
    
            goto L_80188A44;
    }
    // 0x801889EC: nop

    // 0x801889F0: add.s       $f10, $f2, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = ctx->f2.fl + ctx->f12.fl;
    // 0x801889F4: lui         $at, 0x4180
    ctx->r1 = S32(0X4180 << 16);
    // 0x801889F8: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x801889FC: nop

    // 0x80188A00: bc1f        L_80188A44
    if (!c1cs) {
        // 0x80188A04: nop
    
            goto L_80188A44;
    }
    // 0x80188A04: nop

    // 0x80188A08: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80188A0C: lwc1        $f2, 0x40($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X40);
    // 0x80188A10: lwc1        $f0, 0x58($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80188A14: sub.s       $f16, $f2, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = ctx->f2.fl - ctx->f12.fl;
    // 0x80188A18: c.le.s      $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f16.fl <= ctx->f0.fl;
    // 0x80188A1C: nop

    // 0x80188A20: bc1f        L_80188A44
    if (!c1cs) {
        // 0x80188A24: nop
    
            goto L_80188A44;
    }
    // 0x80188A24: nop

    // 0x80188A28: add.s       $f18, $f2, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = ctx->f2.fl + ctx->f12.fl;
    // 0x80188A2C: c.le.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl <= ctx->f18.fl;
    // 0x80188A30: nop

    // 0x80188A34: bc1f        L_80188A44
    if (!c1cs) {
        // 0x80188A38: nop
    
            goto L_80188A44;
    }
    // 0x80188A38: nop

    // 0x80188A3C: b           L_80188A50
    // 0x80188A40: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
        goto L_80188A50;
    // 0x80188A40: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
L_80188A44:
    // 0x80188A44: bnel        $fp, $zero, L_80188A54
    if (ctx->r30 != 0) {
        // 0x80188A48: lw          $ra, 0x3C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X3C);
            goto L_80188A54;
    }
    goto skip_7;
    // 0x80188A48: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    skip_7:
    // 0x80188A4C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80188A50:
    // 0x80188A50: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_80188A54:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, scanner return ---
    if (lod_interact_trace_active && (ctx->r2 != 0 || lod_interact_candidate_count != 0 || ((lod_interact_scan_id & 0x1F) == 0))) {
        fprintf(stderr,
                "[INTERACT_TRACE] scan-return scan=%u map=0x%08X selected=0x%08X candidates=%d gate_hits=%d deferred=(text=0x%08X item=0x%08X special=0x%08X) fp=%u\n",
                lod_interact_scan_id,
                lod_current_map_overlay_rom(),
                (uint32_t)ctx->r2,
                lod_interact_candidate_count,
                lod_interact_gate_hit_count,
                (uint32_t)MEM_W(ctx->r29, 0x44),
                (uint32_t)ctx->r21,
                (uint32_t)ctx->r20,
                (uint32_t)ctx->r30);
        if (ctx->r2 != 0) {
            lod_interact_trace_object(rdram, "selected", ctx->r2, 1, lod_interact_scan_id);
        }
    }
    // --- END PATCH ---
#endif
    // 0x80188A54: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80188A58: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80188A5C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80188A60: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80188A64: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80188A68: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80188A6C: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80188A70: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80188A74: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80188A78: jr          $ra
    // 0x80188A7C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x80188A7C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void playerCanInteractWithSpecialTextbox(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80188A80: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80188A84: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80188A88: lwc1        $f0, 0x58($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X58);
    // 0x80188A8C: lwc1        $f16, 0x64($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X64);
    // 0x80188A90: lwc1        $f12, 0x50($a1)
    ctx->f12.u32l = MEM_W(ctx->r5, 0X50);
    // 0x80188A94: sub.s       $f4, $f16, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f16.fl - ctx->f0.fl;
    // 0x80188A98: c.le.s      $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f4.fl <= ctx->f12.fl;
    // 0x80188A9C: nop

    // 0x80188AA0: bc1fl       L_80188B74
    if (!c1cs) {
        // 0x80188AA4: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188B74;
    }
    goto skip_0;
    // 0x80188AA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80188AA8: add.s       $f6, $f0, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f0.fl + ctx->f16.fl;
    // 0x80188AAC: lui         $at, 0x40C0
    ctx->r1 = S32(0X40C0 << 16);
    // 0x80188AB0: c.le.s      $f12, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f12.fl <= ctx->f6.fl;
    // 0x80188AB4: nop

    // 0x80188AB8: bc1fl       L_80188B74
    if (!c1cs) {
        // 0x80188ABC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188B74;
    }
    goto skip_1;
    // 0x80188ABC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x80188AC0: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x80188AC4: lwc1        $f2, 0x68($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X68);
    // 0x80188AC8: lwc1        $f0, 0x54($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X54);
    // 0x80188ACC: sub.s       $f8, $f2, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = ctx->f2.fl - ctx->f14.fl;
    // 0x80188AD0: c.le.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl <= ctx->f0.fl;
    // 0x80188AD4: nop

    // 0x80188AD8: bc1fl       L_80188B74
    if (!c1cs) {
        // 0x80188ADC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188B74;
    }
    goto skip_2;
    // 0x80188ADC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_2:
    // 0x80188AE0: add.s       $f10, $f2, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f2.fl + ctx->f14.fl;
    // 0x80188AE4: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x80188AE8: nop

    // 0x80188AEC: bc1fl       L_80188B74
    if (!c1cs) {
        // 0x80188AF0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188B74;
    }
    goto skip_3;
    // 0x80188AF0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_3:
    // 0x80188AF4: lwc1        $f0, 0x40($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X40);
    // 0x80188AF8: lwc1        $f2, 0x5C($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X5C);
    // 0x80188AFC: lwc1        $f14, 0x58($a1)
    ctx->f14.u32l = MEM_W(ctx->r5, 0X58);
    // 0x80188B00: sub.s       $f18, $f0, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x80188B04: c.le.s      $f18, $f14
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f18.fl <= ctx->f14.fl;
    // 0x80188B08: nop

    // 0x80188B0C: bc1fl       L_80188B74
    if (!c1cs) {
        // 0x80188B10: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188B74;
    }
    goto skip_4;
    // 0x80188B10: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_4:
    // 0x80188B14: add.s       $f4, $f2, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f2.fl + ctx->f0.fl;
    // 0x80188B18: c.le.s      $f14, $f4
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f14.fl <= ctx->f4.fl;
    // 0x80188B1C: nop

    // 0x80188B20: bc1fl       L_80188B74
    if (!c1cs) {
        // 0x80188B24: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188B74;
    }
    goto skip_5;
    // 0x80188B24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_5:
    // 0x80188B28: lhu         $v1, 0x5E($a1)
    ctx->r3 = MEM_HU(ctx->r5, 0X5E);
    // 0x80188B2C: mfc1        $a2, $f16
    ctx->r6 = (int32_t)ctx->f16.u32l;
    // 0x80188B30: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x80188B34: addiu       $v1, $v1, 0x2000
    ctx->r3 = ADD32(ctx->r3, 0X2000);
    // 0x80188B38: sh          $v1, 0x1C($sp)
    MEM_H(0X1C, ctx->r29) = ctx->r3;
    // 0x80188B3C: jal         0x80051318
    // 0x80188B40: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    func_80051318(rdram, ctx);
        goto after_0;
    // 0x80188B40: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    after_0:
    // 0x80188B44: lhu         $v1, 0x1C($sp)
    ctx->r3 = MEM_HU(ctx->r29, 0X1C);
    // 0x80188B48: ori         $at, $zero, 0x9001
    ctx->r1 = 0 | 0X9001;
    // 0x80188B4C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80188B50: subu        $t7, $v0, $v1
    ctx->r15 = SUB32(ctx->r2, ctx->r3);
    // 0x80188B54: addiu       $t8, $t7, 0x4000
    ctx->r24 = ADD32(ctx->r15, 0X4000);
    // 0x80188B58: andi        $t9, $t8, 0xFFFF
    ctx->r25 = ctx->r24 & 0XFFFF;
    // 0x80188B5C: slt         $at, $t9, $at
    ctx->r1 = SIGNED(ctx->r25) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x80188B60: beql        $at, $zero, L_80188B74
    if (ctx->r1 == 0) {
        // 0x80188B64: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188B74;
    }
    goto skip_6;
    // 0x80188B64: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_6:
    // 0x80188B68: b           L_80188B74
    // 0x80188B6C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
        goto L_80188B74;
    // 0x80188B6C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80188B70: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80188B74:
    // 0x80188B74: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80188B78: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80188B7C: jr          $ra
    // 0x80188B80: nop

    return;
    // 0x80188B80: nop

;}
RECOMP_FUNC void playerCanInteractWithInteractuable(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, gate locals ---
    const uint64_t lod_interact_gate_actor = ctx->r7;
    const uint32_t lod_interact_gate_id = ++lod_interact_trace_gate_counter;
    const int lod_interact_gate_trace_active = lod_interact_trace_enabled();
    // --- END PATCH ---
#endif
    // 0x80188B84: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80188B88: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80188B8C: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x80188B90: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80188B94: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80188B98: lhu         $v0, 0x38($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X38);
    // 0x80188B9C: mtc1        $a2, $f22
    ctx->f22.u32l = ctx->r6;
    // 0x80188BA0: mov.s       $f20, $f12
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 12);
    ctx->f20.fl = ctx->f12.fl;
    // 0x80188BA4: addiu       $t6, $v0, 0x1
    ctx->r14 = ADD32(ctx->r2, 0X1);
    // 0x80188BA8: slti        $at, $t6, 0x31
    ctx->r1 = SIGNED(ctx->r14) < 0X31 ? 1 : 0;
    // 0x80188BAC: sll         $a0, $v0, 16
    ctx->r4 = S32(ctx->r2 << 16);
    // 0x80188BB0: bne         $at, $zero, L_80188CB0
    if (ctx->r1 != 0) {
        // 0x80188BB4: sra         $a0, $a0, 16
        ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
            goto L_80188CB0;
    }
    // 0x80188BB4: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80188BB8: lh          $t7, 0x5C($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X5C);
    // 0x80188BBC: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80188BC0: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x80188BC4: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x80188BC8: lwc1        $f12, 0x50($v1)
    ctx->f12.u32l = MEM_W(ctx->r3, 0X50);
    // 0x80188BCC: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80188BD0: sub.s       $f6, $f20, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f20.fl - ctx->f0.fl;
    // 0x80188BD4: c.le.s      $f6, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f6.fl <= ctx->f12.fl;
    // 0x80188BD8: nop

    // 0x80188BDC: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188BE0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_0;
    // 0x80188BE0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80188BE4: add.s       $f8, $f0, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = ctx->f0.fl + ctx->f20.fl;
    // 0x80188BE8: lui         $at, 0x40A0
    ctx->r1 = S32(0X40A0 << 16);
    // 0x80188BEC: c.le.s      $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f12.fl <= ctx->f8.fl;
    // 0x80188BF0: nop

    // 0x80188BF4: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188BF8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_1;
    // 0x80188BF8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x80188BFC: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80188C00: lwc1        $f0, 0x54($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X54);
    // 0x80188C04: lui         $at, 0x4100
    ctx->r1 = S32(0X4100 << 16);
    // 0x80188C08: sub.s       $f16, $f14, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f14.fl - ctx->f10.fl;
    // 0x80188C0C: c.le.s      $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f16.fl <= ctx->f0.fl;
    // 0x80188C10: nop

    // 0x80188C14: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188C18: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_2;
    // 0x80188C18: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_2:
    // 0x80188C1C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x80188C20: nop

    // 0x80188C24: add.s       $f4, $f14, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f14.fl + ctx->f18.fl;
    // 0x80188C28: c.le.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl <= ctx->f4.fl;
    // 0x80188C2C: nop

    // 0x80188C30: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188C34: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_3;
    // 0x80188C34: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_3:
    // 0x80188C38: lh          $t8, 0x5E($a3)
    ctx->r24 = MEM_H(ctx->r7, 0X5E);
    // 0x80188C3C: lwc1        $f14, 0x58($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80188C40: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x80188C44: nop

    // 0x80188C48: cvt.s.w     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    ctx->f0.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80188C4C: sub.s       $f8, $f22, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f22.fl - ctx->f0.fl;
    // 0x80188C50: c.le.s      $f8, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f8.fl <= ctx->f14.fl;
    // 0x80188C54: nop

    // 0x80188C58: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188C5C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_4;
    // 0x80188C5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_4:
    // 0x80188C60: add.s       $f10, $f0, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = ctx->f0.fl + ctx->f22.fl;
    // 0x80188C64: c.le.s      $f14, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f14.fl <= ctx->f10.fl;
    // 0x80188C68: nop

    // 0x80188C6C: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188C70: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_5;
    // 0x80188C70: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_5:
    // 0x80188C74: lhu         $s0, 0x5E($v1)
    ctx->r16 = MEM_HU(ctx->r3, 0X5E);
    // 0x80188C78: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x80188C7C: mfc1        $a3, $f22
    ctx->r7 = (int32_t)ctx->f22.u32l;
    // 0x80188C80: addiu       $s0, $s0, 0x2000
    ctx->r16 = ADD32(ctx->r16, 0X2000);
    // 0x80188C84: jal         0x80051318
    // 0x80188C88: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    func_80051318(rdram, ctx);
        goto after_0;
    // 0x80188C88: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    after_0:
    // 0x80188C8C: subu        $t0, $v0, $s0
    ctx->r8 = SUB32(ctx->r2, ctx->r16);
    // 0x80188C90: addiu       $t1, $t0, 0x4000
    ctx->r9 = ADD32(ctx->r8, 0X4000);
    // 0x80188C94: andi        $t2, $t1, 0xFFFF
    ctx->r10 = ctx->r9 & 0XFFFF;
    // 0x80188C98: ori         $at, $zero, 0x9001
    ctx->r1 = 0 | 0X9001;
    // 0x80188C9C: slt         $at, $t2, $at
    ctx->r1 = SIGNED(ctx->r10) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x80188CA0: beql        $at, $zero, L_80188E6C
    if (ctx->r1 == 0) {
        // 0x80188CA4: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_6;
    // 0x80188CA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_6:
    // 0x80188CA8: b           L_80188E6C
    // 0x80188CAC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80188E6C;
    // 0x80188CAC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80188CB0:
    // 0x80188CB0: sll         $t3, $a0, 2
    ctx->r11 = S32(ctx->r4 << 2);
    // 0x80188CB4: addu        $t3, $t3, $a0
    ctx->r11 = ADD32(ctx->r11, ctx->r4);
    // 0x80188CB8: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x80188CBC: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80188CC0: addu        $t4, $t4, $t3
    ctx->r12 = ADD32(ctx->r12, ctx->r11);
    // 0x80188CC4: lhu         $t4, 0x6788($t4)
    ctx->r12 = MEM_HU(ctx->r12, 0X6788);
    // 0x80188CC8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80188CCC: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x80188CD0: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x80188CD4: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x80188CD8: lwc1        $f12, 0x50($v1)
    ctx->f12.u32l = MEM_W(ctx->r3, 0X50);
    // 0x80188CDC: bgez        $t4, L_80188CF0
    if (SIGNED(ctx->r12) >= 0) {
        // 0x80188CE0: cvt.s.w     $f2, $f16
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    ctx->f2.fl = CVT_S_W(ctx->f16.u32l);
            goto L_80188CF0;
    }
    // 0x80188CE0: cvt.s.w     $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    ctx->f2.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80188CE4: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x80188CE8: nop

    // 0x80188CEC: add.s       $f2, $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f2.fl + ctx->f18.fl;
L_80188CF0:
    // 0x80188CF0: sub.s       $f4, $f20, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = ctx->f20.fl - ctx->f2.fl;
    // 0x80188CF4: c.le.s      $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f4.fl <= ctx->f12.fl;
    // 0x80188CF8: nop

    // 0x80188CFC: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188D00: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_7;
    // 0x80188D00: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_7:
    // 0x80188D04: add.s       $f6, $f2, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = ctx->f2.fl + ctx->f20.fl;
    // 0x80188D08: c.le.s      $f12, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f12.fl <= ctx->f6.fl;
    // 0x80188D0C: nop

    // 0x80188D10: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188D14: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_8;
    // 0x80188D14: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_8:
    // 0x80188D18: sub.s       $f8, $f14, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = ctx->f14.fl - ctx->f2.fl;
    // 0x80188D1C: lwc1        $f0, 0x54($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X54);
    // 0x80188D20: c.le.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl <= ctx->f0.fl;
    // 0x80188D24: nop

    // 0x80188D28: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188D2C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_9;
    // 0x80188D2C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_9:
    // 0x80188D30: add.s       $f10, $f2, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f2.fl + ctx->f14.fl;
    // 0x80188D34: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x80188D38: nop

    // 0x80188D3C: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188D40: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_10;
    // 0x80188D40: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_10:
    // 0x80188D44: sub.s       $f16, $f22, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f22.fl - ctx->f2.fl;
    // 0x80188D48: lwc1        $f14, 0x58($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80188D4C: c.le.s      $f16, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f16.fl <= ctx->f14.fl;
    // 0x80188D50: nop

    // 0x80188D54: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188D58: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_11;
    // 0x80188D58: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_11:
    // 0x80188D5C: add.s       $f18, $f2, $f22
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f18.fl = ctx->f2.fl + ctx->f22.fl;
    // 0x80188D60: c.le.s      $f14, $f18
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f14.fl <= ctx->f18.fl;
    // 0x80188D64: nop

    // 0x80188D68: bc1fl       L_80188E6C
    if (!c1cs) {
        // 0x80188D6C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_12;
    // 0x80188D6C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_12:
    // 0x80188D70: lhu         $s0, 0x5E($v1)
    ctx->r16 = MEM_HU(ctx->r3, 0X5E);
    // 0x80188D74: lui         $t9, 0x8009
    ctx->r25 = S32(0X8009 << 16);
    // 0x80188D78: addiu       $t9, $t9, 0x7330
    ctx->r25 = ADD32(ctx->r25, 0X7330);
    // 0x80188D7C: addiu       $s0, $s0, 0x2000
    ctx->r16 = ADD32(ctx->r16, 0X2000);
    // 0x80188D80: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80188D84: jalr        $t9
    // 0x80188D88: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80188D88: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    after_1:
    // 0x80188D8C: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80188D90: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x80188D94: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80188D98: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80188D9C: lui         $at, 0xC000
    ctx->r1 = S32(0XC000 << 16);
    // 0x80188DA0: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x80188DA4: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80188DA8: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80188DAC: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x80188DB0: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80188DB4: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80188DB8: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x80188DBC: addiu       $t9, $t9, 0x3A50
    ctx->r25 = ADD32(ctx->r25, 0X3A50);
    // 0x80188DC0: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80188DC4: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x80188DC8: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x80188DCC: nop

    // 0x80188DD0: mul.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = MUL_D(ctx->f4.d, ctx->f6.d);
    // 0x80188DD4: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x80188DD8: jalr        $t9
    // 0x80188DDC: swc1        $f10, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80188DDC: swc1        $f10, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f10.u32l;
    after_2:
    // 0x80188DE0: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80188DE4: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x80188DE8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80188DEC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80188DF0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80188DF4: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x80188DF8: lwc1        $f18, 0x30($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80188DFC: lui         $at, 0xC000
    ctx->r1 = S32(0XC000 << 16);
    // 0x80188E00: lwc1        $f16, 0x50($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X50);
    // 0x80188E04: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80188E08: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x80188E0C: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80188E10: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x80188E14: add.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x80188E18: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80188E1C: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80188E20: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x80188E24: mfc1        $a3, $f22
    ctx->r7 = (int32_t)ctx->f22.u32l;
    // 0x80188E28: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x80188E2C: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x80188E30: lwc1        $f16, 0x58($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80188E34: mul.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = MUL_D(ctx->f4.d, ctx->f6.d);
    // 0x80188E38: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x80188E3C: jal         0x80051318
    // 0x80188E40: add.s       $f14, $f16, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f16.fl + ctx->f10.fl;
    func_80051318(rdram, ctx);
        goto after_3;
    // 0x80188E40: add.s       $f14, $f16, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f16.fl + ctx->f10.fl;
    after_3:
    // 0x80188E44: subu        $t6, $v0, $s0
    ctx->r14 = SUB32(ctx->r2, ctx->r16);
    // 0x80188E48: addiu       $t7, $t6, 0x4000
    ctx->r15 = ADD32(ctx->r14, 0X4000);
    // 0x80188E4C: andi        $t8, $t7, 0xFFFF
    ctx->r24 = ctx->r15 & 0XFFFF;
    // 0x80188E50: ori         $at, $zero, 0x8001
    ctx->r1 = 0 | 0X8001;
    // 0x80188E54: slt         $at, $t8, $at
    ctx->r1 = SIGNED(ctx->r24) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x80188E58: beql        $at, $zero, L_80188E6C
    if (ctx->r1 == 0) {
        // 0x80188E5C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80188E6C;
    }
    goto skip_13;
    // 0x80188E5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_13:
    // 0x80188E60: b           L_80188E6C
    // 0x80188E64: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80188E6C;
    // 0x80188E64: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80188E68: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80188E6C:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, gate return ---
    if (lod_interact_gate_trace_active) {
        lod_interact_trace_object(rdram,
                                  ctx->r2 != 0 ? "gate-pass" : "gate-fail",
                                  lod_interact_gate_actor,
                                  (uint32_t)ctx->r2,
                                  lod_interact_gate_id);
    }
    // --- END PATCH ---
#endif
    // 0x80188E6C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80188E70: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80188E74: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x80188E78: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x80188E7C: jr          $ra
    // 0x80188E80: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x80188E80: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void interactables_getInteractingType(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80188E84: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x80188E88: addiu       $at, $zero, 0x27
    ctx->r1 = ADD32(0, 0X27);
    // 0x80188E8C: bnel        $v0, $at, L_80188EE8
    if (ctx->r2 != ctx->r1) {
        // 0x80188E90: addiu       $at, $zero, 0x2026
        ctx->r1 = ADD32(0, 0X2026);
            goto L_80188EE8;
    }
    goto skip_0;
    // 0x80188E90: addiu       $at, $zero, 0x2026
    ctx->r1 = ADD32(0, 0X2026);
    skip_0:
    // 0x80188E94: lhu         $v0, 0x38($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X38);
    // 0x80188E98: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80188E9C: addiu       $t6, $v0, 0x1
    ctx->r14 = ADD32(ctx->r2, 0X1);
    // 0x80188EA0: slti        $at, $t6, 0x31
    ctx->r1 = SIGNED(ctx->r14) < 0X31 ? 1 : 0;
    // 0x80188EA4: bne         $at, $zero, L_80188ED8
    if (ctx->r1 != 0) {
        // 0x80188EA8: sll         $t7, $v0, 2
        ctx->r15 = S32(ctx->r2 << 2);
            goto L_80188ED8;
    }
    // 0x80188EA8: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x80188EAC: addu        $t7, $t7, $v0
    ctx->r15 = ADD32(ctx->r15, ctx->r2);
    // 0x80188EB0: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80188EB4: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80188EB8: lhu         $t8, 0x6470($t8)
    ctx->r24 = MEM_HU(ctx->r24, 0X6470);
    // 0x80188EBC: andi        $t9, $t8, 0x100
    ctx->r25 = ctx->r24 & 0X100;
    // 0x80188EC0: beq         $t9, $zero, L_80188ED0
    if (ctx->r25 == 0) {
        // 0x80188EC4: nop
    
            goto L_80188ED0;
    }
    // 0x80188EC4: nop

    // 0x80188EC8: jr          $ra
    // 0x80188ECC: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    return;
    // 0x80188ECC: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
L_80188ED0:
    // 0x80188ED0: jr          $ra
    // 0x80188ED4: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    return;
    // 0x80188ED4: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_80188ED8:
    // 0x80188ED8: sh          $zero, 0x4($a0)
    MEM_H(0X4, ctx->r4) = 0;
    // 0x80188EDC: jr          $ra
    // 0x80188EE0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80188EE0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80188EE4: addiu       $at, $zero, 0x2026
    ctx->r1 = ADD32(0, 0X2026);
L_80188EE8:
    // 0x80188EE8: bne         $v0, $at, L_80188EF8
    if (ctx->r2 != ctx->r1) {
        // 0x80188EEC: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_80188EF8;
    }
    // 0x80188EEC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80188EF0: jr          $ra
    // 0x80188EF4: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    return;
    // 0x80188EF4: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_80188EF8:
    // 0x80188EF8: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80188EFC: addiu       $a1, $a1, 0x6740
    ctx->r5 = ADD32(ctx->r5, 0X6740);
    // 0x80188F00: sll         $t0, $v1, 2
    ctx->r8 = S32(ctx->r3 << 2);
L_80188F04:
    // 0x80188F04: addu        $t1, $a1, $t0
    ctx->r9 = ADD32(ctx->r5, ctx->r8);
    // 0x80188F08: lh          $t2, 0x0($t1)
    ctx->r10 = MEM_H(ctx->r9, 0X0);
    // 0x80188F0C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80188F10: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80188F14: bne         $v0, $t2, L_80188F60
    if (ctx->r2 != ctx->r10) {
        // 0x80188F18: sra         $v1, $v1, 16
        ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
            goto L_80188F60;
    }
    // 0x80188F18: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80188F1C: addiu       $at, $zero, 0x2018
    ctx->r1 = ADD32(0, 0X2018);
    // 0x80188F20: bne         $v0, $at, L_80188F58
    if (ctx->r2 != ctx->r1) {
        // 0x80188F24: nop
    
            goto L_80188F58;
    }
    // 0x80188F24: nop

    // 0x80188F28: lw          $t3, 0x60($a0)
    ctx->r11 = MEM_W(ctx->r4, 0X60);
    // 0x80188F2C: lui         $t5, 0x8019
    ctx->r13 = S32(0X8019 << 16);
    // 0x80188F30: addiu       $at, $zero, 0x2021
    ctx->r1 = ADD32(0, 0X2021);
    // 0x80188F34: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80188F38: addu        $t5, $t5, $t4
    ctx->r13 = ADD32(ctx->r13, ctx->r12);
    // 0x80188F3C: lh          $t5, 0x6758($t5)
    ctx->r13 = MEM_H(ctx->r13, 0X6758);
    // 0x80188F40: bne         $t5, $at, L_80188F50
    if (ctx->r13 != ctx->r1) {
        // 0x80188F44: nop
    
            goto L_80188F50;
    }
    // 0x80188F44: nop

    // 0x80188F48: jr          $ra
    // 0x80188F4C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    return;
    // 0x80188F4C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
L_80188F50:
    // 0x80188F50: jr          $ra
    // 0x80188F54: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    return;
    // 0x80188F54: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_80188F58:
    // 0x80188F58: jr          $ra
    // 0x80188F5C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    return;
    // 0x80188F5C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_80188F60:
    // 0x80188F60: slti        $at, $v1, 0x6
    ctx->r1 = SIGNED(ctx->r3) < 0X6 ? 1 : 0;
    // 0x80188F64: bnel        $at, $zero, L_80188F04
    if (ctx->r1 != 0) {
        // 0x80188F68: sll         $t0, $v1, 2
        ctx->r8 = S32(ctx->r3 << 2);
            goto L_80188F04;
    }
    goto skip_1;
    // 0x80188F68: sll         $t0, $v1, 2
    ctx->r8 = S32(ctx->r3 << 2);
    skip_1:
    // 0x80188F6C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80188F70: jr          $ra
    // 0x80188F74: nop

    return;
    // 0x80188F74: nop

;}
RECOMP_FUNC void interactables_setInteractingFlag(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80188F78: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x80188F7C: jr          $ra
    // 0x80188F80: sw          $t6, 0x3C($a0)
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, flag write ---
    if (lod_interact_trace_enabled()) {
        lod_interact_trace_object(rdram, "set-flag", ctx->r4, (uint32_t)ctx->r14, 0);
    }
    // --- END PATCH ---
#endif
    MEM_W(0X3C, ctx->r4) = ctx->r14;
    return;
    // 0x80188F80: sw          $t6, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r14;
;}
RECOMP_FUNC void interactable_createWithSettings(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80188F84: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80188F88: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80188F8C: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80188F90: jal         0x80002410
    // 0x80188F94: addiu       $a1, $zero, 0x27
    ctx->r5 = ADD32(0, 0X27);
    object_createAndSetChild(rdram, ctx);
        goto after_0;
    // 0x80188F94: addiu       $a1, $zero, 0x27
    ctx->r5 = ADD32(0, 0X27);
    after_0:
    // 0x80188F98: bne         $v0, $zero, L_80188FA8
    if (ctx->r2 != 0) {
        // 0x80188F9C: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80188FA8;
    }
    // 0x80188F9C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80188FA0: b           L_80188FB4
    // 0x80188FA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80188FB4;
    // 0x80188FA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80188FA8:
    // 0x80188FA8: lw          $t6, 0x1C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X1C);
    // 0x80188FAC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80188FB0: sw          $t6, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->r14;
L_80188FB4:
    // 0x80188FB4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80188FB8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80188FBC: jr          $ra
    // 0x80188FC0: nop

    return;
    // 0x80188FC0: nop

;}
RECOMP_FUNC void interactable_setPosition(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80188FC4: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x80188FC8: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x80188FCC: beq         $a0, $zero, L_80188FE4
    if (ctx->r4 == 0) {
        // 0x80188FD0: sw          $a3, 0xC($sp)
        MEM_W(0XC, ctx->r29) = ctx->r7;
            goto L_80188FE4;
    }
    // 0x80188FD0: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    // 0x80188FD4: swc1        $f12, 0x64($a0)
    MEM_W(0X64, ctx->r4) = ctx->f12.u32l;
    // 0x80188FD8: swc1        $f14, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f14.u32l;
    // 0x80188FDC: lwc1        $f4, 0xC($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XC);
    // 0x80188FE0: swc1        $f4, 0x40($a0)
    MEM_W(0X40, ctx->r4) = ctx->f4.u32l;
L_80188FE4:
    // 0x80188FE4: jr          $ra
    // 0x80188FE8: nop

    return;
    // 0x80188FE8: nop

;}
RECOMP_FUNC void func_80188FEC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80188FEC: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x80188FF0: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x80188FF4: beq         $a0, $zero, L_80189014
    if (ctx->r4 == 0) {
        // 0x80188FF8: nop
    
            goto L_80189014;
    }
    // 0x80188FF8: nop

    // 0x80188FFC: trunc.w.s   $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    ctx->f4.u32l = TRUNC_W_S(ctx->f12.fl);
    // 0x80189000: trunc.w.s   $f6, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    ctx->f6.u32l = TRUNC_W_S(ctx->f14.fl);
    // 0x80189004: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
    // 0x80189008: mfc1        $t9, $f6
    ctx->r25 = (int32_t)ctx->f6.u32l;
    // 0x8018900C: sh          $t7, 0x5C($a0)
    MEM_H(0X5C, ctx->r4) = ctx->r15;
    // 0x80189010: sh          $t9, 0x5E($a0)
    MEM_H(0X5E, ctx->r4) = ctx->r25;
L_80189014:
    // 0x80189014: jr          $ra
    // 0x80189018: nop

    return;
    // 0x80189018: nop

;}
RECOMP_FUNC void func_8018901C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018901C: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80189020: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80189024: beq         $a0, $zero, L_80189030
    if (ctx->r4 == 0) {
        // 0x80189028: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_80189030;
    }
    // 0x80189028: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x8018902C: sh          $a1, 0x3A($a0)
    MEM_H(0X3A, ctx->r4) = ctx->r5;
L_80189030:
    // 0x80189030: jr          $ra
    // 0x80189034: nop

    return;
    // 0x80189034: nop

;}
RECOMP_FUNC void func_80189038(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189038: beq         $a0, $zero, L_80189044
    if (ctx->r4 == 0) {
        // 0x8018903C: nop
    
            goto L_80189044;
    }
    // 0x8018903C: nop

    // 0x80189040: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
L_80189044:
    // 0x80189044: jr          $ra
    // 0x80189048: nop

    return;
    // 0x80189048: nop

;}
RECOMP_FUNC void func_80189050(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189050: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80189054: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80189058: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8018905C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80189060: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80189064: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80189068: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8018906C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80189070: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80189074: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80189078: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8018907C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80189080: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80189084: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80189088: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8018908C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80189090: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80189094: lw          $t9, 0x7130($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7130);
    // 0x80189098: jalr        $t9
    // 0x8018909C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8018909C: nop

    after_0:
    // 0x801890A0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801890A4: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801890A8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801890AC: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801890B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801890B4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801890B8: jr          $ra
    // 0x801890BC: nop

    return;
    // 0x801890BC: nop

;}
RECOMP_FUNC void func_801890C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801890C0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801890C4: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801890C8: lw          $v0, 0x2BC8($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X2BC8);
    // 0x801890CC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801890D0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801890D4: beq         $v0, $zero, L_80189154
    if (ctx->r2 == 0) {
        // 0x801890D8: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_80189154;
    }
    // 0x801890D8: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801890DC: sltiu       $at, $v0, 0x3
    ctx->r1 = ctx->r2 < 0X3 ? 1 : 0;
    // 0x801890E0: beq         $at, $zero, L_80189100
    if (ctx->r1 == 0) {
        // 0x801890E4: addiu       $a0, $a0, 0x8
        ctx->r4 = ADD32(ctx->r4, 0X8);
            goto L_80189100;
    }
    // 0x801890E4: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801890E8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801890EC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801890F0: jalr        $t9
    // 0x801890F4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801890F4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
    // 0x801890F8: b           L_80189158
    // 0x801890FC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80189158;
    // 0x801890FC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80189100:
    // 0x80189100: lw          $t6, 0x2960($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2960);
    // 0x80189104: beql        $t6, $zero, L_80189158
    if (ctx->r14 == 0) {
        // 0x80189108: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80189158;
    }
    goto skip_0;
    // 0x80189108: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x8018910C: lw          $v0, 0x2964($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X2964);
    // 0x80189110: lui         $a0, 0x8000
    ctx->r4 = S32(0X8000 << 16);
    // 0x80189114: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x80189118: sll         $t8, $t7, 0
    ctx->r24 = S32(ctx->r15 << 0);
    // 0x8018911C: bltzl       $t8, L_80189138
    if (SIGNED(ctx->r24) < 0) {
        // 0x80189120: sw          $zero, 0x2BCC($v1)
        MEM_W(0X2BCC, ctx->r3) = 0;
            goto L_80189138;
    }
    goto skip_1;
    // 0x80189120: sw          $zero, 0x2BCC($v1)
    MEM_W(0X2BCC, ctx->r3) = 0;
    skip_1:
    // 0x80189124: lw          $t0, 0x28C4($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X28C4);
    // 0x80189128: and         $t1, $t0, $a0
    ctx->r9 = ctx->r8 & ctx->r4;
    // 0x8018912C: beq         $t1, $zero, L_80189140
    if (ctx->r9 == 0) {
        // 0x80189130: nop
    
            goto L_80189140;
    }
    // 0x80189130: nop

    // 0x80189134: sw          $zero, 0x2BCC($v1)
    MEM_W(0X2BCC, ctx->r3) = 0;
L_80189138:
    // 0x80189138: b           L_80189154
    // 0x8018913C: sw          $zero, 0x2BC8($v1)
    MEM_W(0X2BC8, ctx->r3) = 0;
        goto L_80189154;
    // 0x8018913C: sw          $zero, 0x2BC8($v1)
    MEM_W(0X2BC8, ctx->r3) = 0;
L_80189140:
    // 0x80189140: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189144: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80189148: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x8018914C: jalr        $t9
    // 0x80189150: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80189150: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_80189154:
    // 0x80189154: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80189158:
    // 0x80189158: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8018915C: jr          $ra
    // 0x80189160: nop

    return;
    // 0x80189160: nop

;}
RECOMP_FUNC void func_80189164(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189164: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80189168: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8018916C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80189170: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80189174: sw          $zero, 0x44($a0)
    MEM_W(0X44, ctx->r4) = 0;
    // 0x80189178: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x8018917C: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80189180: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80189184: addiu       $a0, $a0, 0x7170
    ctx->r4 = ADD32(ctx->r4, 0X7170);
    // 0x80189188: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8018918C: addiu       $v1, $zero, 0xA
    ctx->r3 = ADD32(0, 0XA);
L_80189190:
    // 0x80189190: multu       $v0, $v1
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r3)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80189194: lw          $t6, 0x2BC8($t0)
    ctx->r14 = MEM_W(ctx->r8, 0X2BC8);
    // 0x80189198: addiu       $t1, $v0, 0x1
    ctx->r9 = ADD32(ctx->r2, 0X1);
    // 0x8018919C: mflo        $t7
    ctx->r15 = lo;
    // 0x801891A0: addu        $t8, $a0, $t7
    ctx->r24 = ADD32(ctx->r4, ctx->r15);
    // 0x801891A4: lhu         $t9, 0x2($t8)
    ctx->r25 = MEM_HU(ctx->r24, 0X2);
    // 0x801891A8: bnel        $t6, $t9, L_801891B8
    if (ctx->r14 != ctx->r25) {
        // 0x801891AC: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_801891B8;
    }
    goto skip_0;
    // 0x801891AC: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_0:
    // 0x801891B0: sw          $t1, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r9;
    // 0x801891B4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_801891B8:
    // 0x801891B8: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x801891BC: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x801891C0: slti        $at, $v0, 0x5D
    ctx->r1 = SIGNED(ctx->r2) < 0X5D ? 1 : 0;
    // 0x801891C4: bne         $at, $zero, L_80189190
    if (ctx->r1 != 0) {
        // 0x801891C8: nop
    
            goto L_80189190;
    }
    // 0x801891C8: nop

    // 0x801891CC: lw          $v1, 0x44($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X44);
    // 0x801891D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801891D4: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801891D8: bne         $v1, $zero, L_80189208
    if (ctx->r3 != 0) {
        // 0x801891DC: sll         $t3, $v1, 2
        ctx->r11 = S32(ctx->r3 << 2);
            goto L_80189208;
    }
    // 0x801891DC: sll         $t3, $v1, 2
    ctx->r11 = S32(ctx->r3 << 2);
    // 0x801891E0: lw          $t2, 0x2BC8($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X2BC8);
    // 0x801891E4: addiu       $at, $zero, 0x64
    ctx->r1 = ADD32(0, 0X64);
    // 0x801891E8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801891EC: beq         $t2, $at, L_801891F8
    if (ctx->r10 == ctx->r1) {
        // 0x801891F0: addiu       $t9, $t9, 0x1E30
        ctx->r25 = ADD32(ctx->r25, 0X1E30);
            goto L_801891F8;
    }
    // 0x801891F0: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801891F4: sw          $zero, 0x2BC8($t0)
    MEM_W(0X2BC8, ctx->r8) = 0;
L_801891F8:
    // 0x801891F8: jalr        $t9
    // 0x801891FC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801891FC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    after_0:
    // 0x80189200: b           L_80189334
    // 0x80189204: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_80189334;
    // 0x80189204: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_80189208:
    // 0x80189208: addu        $t3, $t3, $v1
    ctx->r11 = ADD32(ctx->r11, ctx->r3);
    // 0x8018920C: sll         $t3, $t3, 1
    ctx->r11 = S32(ctx->r11 << 1);
    // 0x80189210: addu        $v0, $a0, $t3
    ctx->r2 = ADD32(ctx->r4, ctx->r11);
    // 0x80189214: lhu         $t4, -0x6($v0)
    ctx->r12 = MEM_HU(ctx->r2, -0X6);
    // 0x80189218: addiu       $v0, $v0, -0xA
    ctx->r2 = ADD32(ctx->r2, -0XA);
    // 0x8018921C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80189220: andi        $t5, $t4, 0x8
    ctx->r13 = ctx->r12 & 0X8;
    // 0x80189224: beq         $t5, $zero, L_801892F8
    if (ctx->r13 == 0) {
        // 0x80189228: or          $a2, $zero, $zero
        ctx->r6 = 0 | 0;
            goto L_801892F8;
    }
    // 0x80189228: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8018922C: lwc1        $f4, 0x8($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0X8);
    // 0x80189230: lwc1        $f6, -0x3030($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3030);
    // 0x80189234: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80189238: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8018923C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80189240: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x80189244: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x80189248: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x8018924C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x80189250: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x80189254: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x80189258: nop

    // 0x8018925C: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x80189260: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x80189264: nop

    // 0x80189268: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x8018926C: beql        $a1, $zero, L_801892BC
    if (ctx->r5 == 0) {
        // 0x80189270: mfc1        $a1, $f10
        ctx->r5 = (int32_t)ctx->f10.u32l;
            goto L_801892BC;
    }
    goto skip_1;
    // 0x80189270: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
    skip_1:
    // 0x80189274: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80189278: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x8018927C: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80189280: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x80189284: nop

    // 0x80189288: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x8018928C: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x80189290: nop

    // 0x80189294: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x80189298: bne         $a1, $zero, L_801892B0
    if (ctx->r5 != 0) {
        // 0x8018929C: nop
    
            goto L_801892B0;
    }
    // 0x8018929C: nop

    // 0x801892A0: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
    // 0x801892A4: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x801892A8: b           L_801892C8
    // 0x801892AC: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
        goto L_801892C8;
    // 0x801892AC: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
L_801892B0:
    // 0x801892B0: b           L_801892C8
    // 0x801892B4: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
        goto L_801892C8;
    // 0x801892B4: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
    // 0x801892B8: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
L_801892BC:
    // 0x801892BC: nop

    // 0x801892C0: bltz        $a1, L_801892B0
    if (SIGNED(ctx->r5) < 0) {
        // 0x801892C4: nop
    
            goto L_801892B0;
    }
    // 0x801892C4: nop

L_801892C8:
    // 0x801892C8: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x801892CC: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x801892D0: jalr        $t9
    // 0x801892D4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801892D4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x801892D8: lw          $t8, 0x44($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X44);
    // 0x801892DC: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x801892E0: addiu       $t2, $t2, 0x7170
    ctx->r10 = ADD32(ctx->r10, 0X7170);
    // 0x801892E4: sll         $t6, $t8, 2
    ctx->r14 = S32(ctx->r24 << 2);
    // 0x801892E8: addu        $t6, $t6, $t8
    ctx->r14 = ADD32(ctx->r14, ctx->r24);
    // 0x801892EC: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x801892F0: addiu       $t1, $t6, -0xA
    ctx->r9 = ADD32(ctx->r14, -0XA);
    // 0x801892F4: addu        $v0, $t1, $t2
    ctx->r2 = ADD32(ctx->r9, ctx->r10);
L_801892F8:
    // 0x801892F8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801892FC: jal         0x80002410
    // 0x80189300: lhu         $a1, 0x0($v0)
    ctx->r5 = MEM_HU(ctx->r2, 0X0);
    object_createAndSetChild(rdram, ctx);
        goto after_2;
    // 0x80189300: lhu         $a1, 0x0($v0)
    ctx->r5 = MEM_HU(ctx->r2, 0X0);
    after_2:
    // 0x80189304: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80189308: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x8018930C: sw          $v0, 0x48($s0)
    MEM_W(0X48, ctx->r16) = ctx->r2;
    // 0x80189310: lw          $t3, 0x2BD0($t0)
    ctx->r11 = MEM_W(ctx->r8, 0X2BD0);
    // 0x80189314: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189318: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8018931C: ori         $t4, $t3, 0x1
    ctx->r12 = ctx->r11 | 0X1;
    // 0x80189320: sw          $t4, 0x2BD0($t0)
    MEM_W(0X2BD0, ctx->r8) = ctx->r12;
    // 0x80189324: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80189328: jalr        $t9
    // 0x8018932C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8018932C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
    // 0x80189330: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_80189334:
    // 0x80189334: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80189338: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8018933C: jr          $ra
    // 0x80189340: nop

    return;
    // 0x80189340: nop

;}
RECOMP_FUNC void func_80189344(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189344: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x80189348: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x8018934C: lw          $v0, 0x2968($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X2968);
    // 0x80189350: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80189354: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80189358: beq         $v0, $zero, L_80189410
    if (ctx->r2 == 0) {
        // 0x8018935C: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_80189410;
    }
    // 0x8018935C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80189360: lh          $t6, 0x2874($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X2874);
    // 0x80189364: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80189368: bnel        $t6, $at, L_8018939C
    if (ctx->r14 != ctx->r1) {
        // 0x8018936C: lw          $t0, 0x44($a2)
        ctx->r8 = MEM_W(ctx->r6, 0X44);
            goto L_8018939C;
    }
    goto skip_0;
    // 0x8018936C: lw          $t0, 0x44($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X44);
    skip_0:
    // 0x80189370: lh          $t7, 0x2876($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X2876);
    // 0x80189374: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x80189378: bnel        $t7, $at, L_8018939C
    if (ctx->r15 != ctx->r1) {
        // 0x8018937C: lw          $t0, 0x44($a2)
        ctx->r8 = MEM_W(ctx->r6, 0X44);
            goto L_8018939C;
    }
    goto skip_1;
    // 0x8018937C: lw          $t0, 0x44($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X44);
    skip_1:
    // 0x80189380: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x80189384: lui         $a0, 0x100
    ctx->r4 = S32(0X100 << 16);
    // 0x80189388: sll         $t8, $v1, 7
    ctx->r24 = S32(ctx->r3 << 7);
    // 0x8018938C: bltz        $t8, L_80189398
    if (SIGNED(ctx->r24) < 0) {
        // 0x80189390: or          $t9, $v1, $a0
        ctx->r25 = ctx->r3 | ctx->r4;
            goto L_80189398;
    }
    // 0x80189390: or          $t9, $v1, $a0
    ctx->r25 = ctx->r3 | ctx->r4;
    // 0x80189394: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
L_80189398:
    // 0x80189398: lw          $t0, 0x44($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X44);
L_8018939C:
    // 0x8018939C: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x801893A0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801893A4: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x801893A8: addu        $t1, $t1, $t0
    ctx->r9 = ADD32(ctx->r9, ctx->r8);
    // 0x801893AC: sll         $t1, $t1, 1
    ctx->r9 = S32(ctx->r9 << 1);
    // 0x801893B0: addu        $t2, $t2, $t1
    ctx->r10 = ADD32(ctx->r10, ctx->r9);
    // 0x801893B4: lhu         $t2, 0x716A($t2)
    ctx->r10 = MEM_HU(ctx->r10, 0X716A);
    // 0x801893B8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801893BC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801893C0: andi        $t3, $t2, 0x2
    ctx->r11 = ctx->r10 & 0X2;
    // 0x801893C4: beq         $t3, $zero, L_80189408
    if (ctx->r11 == 0) {
        // 0x801893C8: nop
    
            goto L_80189408;
    }
    // 0x801893C8: nop

    // 0x801893CC: lw          $t4, 0x2BD0($a1)
    ctx->r12 = MEM_W(ctx->r5, 0X2BD0);
    // 0x801893D0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801893D4: andi        $t5, $t4, 0x8
    ctx->r13 = ctx->r12 & 0X8;
    // 0x801893D8: beql        $t5, $zero, L_80189414
    if (ctx->r13 == 0) {
        // 0x801893DC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80189414;
    }
    goto skip_2;
    // 0x801893DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x801893E0: jal         0x8018AA70
    // 0x801893E4: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_8018AA70(rdram, ctx);
        goto after_0;
    // 0x801893E4: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x801893E8: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x801893EC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801893F0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801893F4: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801893F8: jalr        $t9
    // 0x801893FC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801893FC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
    // 0x80189400: b           L_80189414
    // 0x80189404: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80189414;
    // 0x80189404: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80189408:
    // 0x80189408: jalr        $t9
    // 0x8018940C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8018940C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_2:
L_80189410:
    // 0x80189410: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80189414:
    // 0x80189414: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80189418: jr          $ra
    // 0x8018941C: nop

    return;
    // 0x8018941C: nop

;}
RECOMP_FUNC void func_80189420(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189420: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x80189424: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x80189428: lh          $t6, 0x2874($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X2874);
    // 0x8018942C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80189430: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80189434: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80189438: bne         $t6, $at, L_8018946C
    if (ctx->r14 != ctx->r1) {
        // 0x8018943C: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_8018946C;
    }
    // 0x8018943C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80189440: lh          $t7, 0x2876($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X2876);
    // 0x80189444: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x80189448: bnel        $t7, $at, L_80189470
    if (ctx->r15 != ctx->r1) {
        // 0x8018944C: lw          $t0, 0x44($a2)
        ctx->r8 = MEM_W(ctx->r6, 0X44);
            goto L_80189470;
    }
    goto skip_0;
    // 0x8018944C: lw          $t0, 0x44($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X44);
    skip_0:
    // 0x80189450: lw          $v0, 0x2968($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X2968);
    // 0x80189454: lui         $a0, 0x100
    ctx->r4 = S32(0X100 << 16);
    // 0x80189458: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x8018945C: sll         $t8, $v1, 7
    ctx->r24 = S32(ctx->r3 << 7);
    // 0x80189460: bltz        $t8, L_8018946C
    if (SIGNED(ctx->r24) < 0) {
        // 0x80189464: or          $t9, $v1, $a0
        ctx->r25 = ctx->r3 | ctx->r4;
            goto L_8018946C;
    }
    // 0x80189464: or          $t9, $v1, $a0
    ctx->r25 = ctx->r3 | ctx->r4;
    // 0x80189468: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
L_8018946C:
    // 0x8018946C: lw          $t0, 0x44($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X44);
L_80189470:
    // 0x80189470: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80189474: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x80189478: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x8018947C: addu        $t1, $t1, $t0
    ctx->r9 = ADD32(ctx->r9, ctx->r8);
    // 0x80189480: sll         $t1, $t1, 1
    ctx->r9 = S32(ctx->r9 << 1);
    // 0x80189484: addu        $a0, $a0, $t1
    ctx->r4 = ADD32(ctx->r4, ctx->r9);
    // 0x80189488: jal         0x80002560
    // 0x8018948C: lhu         $a0, 0x7166($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X7166);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x8018948C: lhu         $a0, 0x7166($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X7166);
    after_0:
    // 0x80189490: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x80189494: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x80189498: bne         $v0, $zero, L_801894BC
    if (ctx->r2 != 0) {
        // 0x8018949C: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_801894BC;
    }
    // 0x8018949C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x801894A0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801894A4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801894A8: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801894AC: jalr        $t9
    // 0x801894B0: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801894B0: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
    // 0x801894B4: b           L_801894EC
    // 0x801894B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801894EC;
    // 0x801894B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801894BC:
    // 0x801894BC: lhu         $t2, 0x538($a1)
    ctx->r10 = MEM_HU(ctx->r5, 0X538);
    // 0x801894C0: andi        $t3, $t2, 0x1080
    ctx->r11 = ctx->r10 & 0X1080;
    // 0x801894C4: beql        $t3, $zero, L_801894EC
    if (ctx->r11 == 0) {
        // 0x801894C8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801894EC;
    }
    goto skip_1;
    // 0x801894C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x801894CC: lw          $t4, 0x2BC8($a1)
    ctx->r12 = MEM_W(ctx->r5, 0X2BC8);
    // 0x801894D0: sltiu       $at, $t4, 0x3
    ctx->r1 = ctx->r12 < 0X3 ? 1 : 0;
    // 0x801894D4: bnel        $at, $zero, L_801894EC
    if (ctx->r1 != 0) {
        // 0x801894D8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801894EC;
    }
    goto skip_2;
    // 0x801894D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x801894DC: lw          $v0, 0x48($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X48);
    // 0x801894E0: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x801894E4: sb          $t5, 0x70($v0)
    MEM_B(0X70, ctx->r2) = ctx->r13;
    // 0x801894E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801894EC:
    // 0x801894EC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801894F0: jr          $ra
    // 0x801894F4: nop

    return;
    // 0x801894F4: nop

;}
RECOMP_FUNC void func_801894F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801894F8: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801894FC: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80189500: lw          $v0, 0x2BD0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X2BD0);
    // 0x80189504: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80189508: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8018950C: andi        $t6, $v0, 0x10
    ctx->r14 = ctx->r2 & 0X10;
    // 0x80189510: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80189514: bne         $t6, $zero, L_8018955C
    if (ctx->r14 != 0) {
        // 0x80189518: sw          $ra, 0x24($sp)
        MEM_W(0X24, ctx->r29) = ctx->r31;
            goto L_8018955C;
    }
    // 0x80189518: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x8018951C: lw          $t7, 0x44($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X44);
    // 0x80189520: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80189524: addiu       $a1, $a1, 0x7170
    ctx->r5 = ADD32(ctx->r5, 0X7170);
    // 0x80189528: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8018952C: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80189530: sll         $t8, $t8, 1
    ctx->r24 = S32(ctx->r24 << 1);
    // 0x80189534: addu        $t9, $a1, $t8
    ctx->r25 = ADD32(ctx->r5, ctx->r24);
    // 0x80189538: lhu         $t1, -0x6($t9)
    ctx->r9 = MEM_HU(ctx->r25, -0X6);
    // 0x8018953C: andi        $t2, $t1, 0x2
    ctx->r10 = ctx->r9 & 0X2;
    // 0x80189540: beql        $t2, $zero, L_80189560
    if (ctx->r10 == 0) {
        // 0x80189544: lw          $v1, 0x2BC8($t0)
        ctx->r3 = MEM_W(ctx->r8, 0X2BC8);
            goto L_80189560;
    }
    goto skip_0;
    // 0x80189544: lw          $v1, 0x2BC8($t0)
    ctx->r3 = MEM_W(ctx->r8, 0X2BC8);
    skip_0:
    // 0x80189548: jal         0x8018AA70
    // 0x8018954C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8018AA70(rdram, ctx);
        goto after_0;
    // 0x8018954C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x80189550: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80189554: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80189558: lw          $v0, 0x2BD0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X2BD0);
L_8018955C:
    // 0x8018955C: lw          $v1, 0x2BC8($t0)
    ctx->r3 = MEM_W(ctx->r8, 0X2BC8);
L_80189560:
    // 0x80189560: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80189564: addiu       $a1, $a1, 0x7170
    ctx->r5 = ADD32(ctx->r5, 0X7170);
    // 0x80189568: beq         $v1, $zero, L_8018959C
    if (ctx->r3 == 0) {
        // 0x8018956C: andi        $t7, $v0, 0x20
        ctx->r15 = ctx->r2 & 0X20;
            goto L_8018959C;
    }
    // 0x8018956C: andi        $t7, $v0, 0x20
    ctx->r15 = ctx->r2 & 0X20;
    // 0x80189570: lw          $t3, 0x44($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X44);
    // 0x80189574: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80189578: addu        $t4, $t4, $t3
    ctx->r12 = ADD32(ctx->r12, ctx->r11);
    // 0x8018957C: sll         $t4, $t4, 1
    ctx->r12 = S32(ctx->r12 << 1);
    // 0x80189580: addu        $t5, $a1, $t4
    ctx->r13 = ADD32(ctx->r5, ctx->r12);
    // 0x80189584: lhu         $t6, -0x8($t5)
    ctx->r14 = MEM_HU(ctx->r13, -0X8);
    // 0x80189588: bne         $v1, $t6, L_8018959C
    if (ctx->r3 != ctx->r14) {
        // 0x8018958C: nop
    
            goto L_8018959C;
    }
    // 0x8018958C: nop

    // 0x80189590: beq         $v1, $zero, L_8018959C
    if (ctx->r3 == 0) {
        // 0x80189594: nop
    
            goto L_8018959C;
    }
    // 0x80189594: nop

    // 0x80189598: sw          $zero, 0x2BC8($t0)
    MEM_W(0X2BC8, ctx->r8) = 0;
L_8018959C:
    // 0x8018959C: bne         $t7, $zero, L_801895C0
    if (ctx->r15 != 0) {
        // 0x801895A0: addiu       $at, $zero, -0x2
        ctx->r1 = ADD32(0, -0X2);
            goto L_801895C0;
    }
    // 0x801895A0: addiu       $at, $zero, -0x2
    ctx->r1 = ADD32(0, -0X2);
    // 0x801895A4: and         $t8, $v0, $at
    ctx->r24 = ctx->r2 & ctx->r1;
    // 0x801895A8: andi        $t9, $t8, 0x10
    ctx->r25 = ctx->r24 & 0X10;
    // 0x801895AC: bne         $t9, $zero, L_801895C0
    if (ctx->r25 != 0) {
        // 0x801895B0: sw          $t8, 0x2BD0($t0)
        MEM_W(0X2BD0, ctx->r8) = ctx->r24;
            goto L_801895C0;
    }
    // 0x801895B0: sw          $t8, 0x2BD0($t0)
    MEM_W(0X2BD0, ctx->r8) = ctx->r24;
    // 0x801895B4: addiu       $at, $zero, -0x9
    ctx->r1 = ADD32(0, -0X9);
    // 0x801895B8: and         $t1, $t8, $at
    ctx->r9 = ctx->r24 & ctx->r1;
    // 0x801895BC: sw          $t1, 0x2BD0($t0)
    MEM_W(0X2BD0, ctx->r8) = ctx->r9;
L_801895C0:
    // 0x801895C0: lw          $t2, 0x44($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X44);
    // 0x801895C4: addiu       $a0, $zero, 0x4000
    ctx->r4 = ADD32(0, 0X4000);
    // 0x801895C8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801895CC: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801895D0: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x801895D4: sll         $t3, $t3, 1
    ctx->r11 = S32(ctx->r11 << 1);
    // 0x801895D8: addu        $v0, $a1, $t3
    ctx->r2 = ADD32(ctx->r5, ctx->r11);
    // 0x801895DC: lhu         $t4, -0x6($v0)
    ctx->r12 = MEM_HU(ctx->r2, -0X6);
    // 0x801895E0: addiu       $v0, $v0, -0xA
    ctx->r2 = ADD32(ctx->r2, -0XA);
    // 0x801895E4: addiu       $a1, $zero, 0x9
    ctx->r5 = ADD32(0, 0X9);
    // 0x801895E8: andi        $t5, $t4, 0x8
    ctx->r13 = ctx->r12 & 0X8;
    // 0x801895EC: beq         $t5, $zero, L_8018962C
    if (ctx->r13 == 0) {
        // 0x801895F0: or          $a3, $zero, $zero
        ctx->r7 = 0 | 0;
            goto L_8018962C;
    }
    // 0x801895F0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x801895F4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801895F8: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x801895FC: jalr        $t9
    // 0x80189600: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80189600: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x80189604: lw          $t6, 0x44($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X44);
    // 0x80189608: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x8018960C: addiu       $t1, $t1, 0x7170
    ctx->r9 = ADD32(ctx->r9, 0X7170);
    // 0x80189610: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80189614: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80189618: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x8018961C: addiu       $t8, $t7, -0xA
    ctx->r24 = ADD32(ctx->r15, -0XA);
    // 0x80189620: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80189624: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80189628: addu        $v0, $t8, $t1
    ctx->r2 = ADD32(ctx->r24, ctx->r9);
L_8018962C:
    // 0x8018962C: lbu         $t2, 0x7($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X7);
    // 0x80189630: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80189634: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80189638: beq         $t2, $zero, L_80189658
    if (ctx->r10 == 0) {
        // 0x8018963C: nop
    
            goto L_80189658;
    }
    // 0x8018963C: nop

    // 0x80189640: lw          $v0, 0x295C($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X295C);
    // 0x80189644: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189648: addiu       $t9, $t9, 0x1E64
    ctx->r25 = ADD32(ctx->r25, 0X1E64);
    // 0x8018964C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80189650: jalr        $t9
    // 0x80189654: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80189654: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_2:
L_80189658:
    // 0x80189658: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018965C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80189660: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80189664: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80189668: jalr        $t9
    // 0x8018966C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8018966C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_3:
    // 0x80189670: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80189674: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80189678: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8018967C: jr          $ra
    // 0x80189680: nop

    return;
    // 0x80189680: nop

;}
RECOMP_FUNC void func_80189690(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189690: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80189694: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80189698: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8018969C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801896A0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801896A4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801896A8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x801896AC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x801896B0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801896B4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801896B8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801896BC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801896C0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801896C4: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801896C8: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801896CC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801896D0: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801896D4: lw          $t9, 0x7150($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7150);
    // 0x801896D8: jalr        $t9
    // 0x801896DC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801896DC: nop

    after_0:
    // 0x801896E0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801896E4: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801896E8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801896EC: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801896F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801896F4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801896F8: jr          $ra
    // 0x801896FC: nop

    return;
    // 0x801896FC: nop

;}
RECOMP_FUNC void func_80189700(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189700: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80189704: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80189708: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x8018970C: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80189710: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80189714: lhu         $t6, 0x18($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X18);
    // 0x80189718: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8018971C: sw          $t6, 0x50($a0)
    MEM_W(0X50, ctx->r4) = ctx->r14;
    // 0x80189720: lw          $t7, 0x2968($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X2968);
    // 0x80189724: beql        $t7, $zero, L_80189874
    if (ctx->r15 == 0) {
        // 0x80189728: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80189874;
    }
    goto skip_0;
    // 0x80189728: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x8018972C: jal         0x80143234
    // 0x80189730: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_80143234(rdram, ctx);
        goto after_0;
    // 0x80189730: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x80189734: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80189738: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8018973C: lw          $t8, 0x2B10($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X2B10);
    // 0x80189740: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80189744: beql        $t8, $zero, L_80189874
    if (ctx->r24 == 0) {
        // 0x80189748: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80189874;
    }
    goto skip_1;
    // 0x80189748: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x8018974C: lw          $t9, 0x2BD0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X2BD0);
    // 0x80189750: sll         $t0, $t9, 0
    ctx->r8 = S32(ctx->r25 << 0);
    // 0x80189754: bgezl       $t0, L_80189780
    if (SIGNED(ctx->r8) >= 0) {
        // 0x80189758: lw          $t1, 0x2BD4($v1)
        ctx->r9 = MEM_W(ctx->r3, 0X2BD4);
            goto L_80189780;
    }
    goto skip_2;
    // 0x80189758: lw          $t1, 0x2BD4($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X2BD4);
    skip_2:
    // 0x8018975C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x80189760: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    // 0x80189764: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80189768: jalr        $t9
    // 0x8018976C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8018976C: nop

    after_1:
    // 0x80189770: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80189774: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80189778: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x8018977C: lw          $t1, 0x2BD4($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X2BD4);
L_80189780:
    // 0x80189780: lui         $t5, 0x8019
    ctx->r13 = S32(0X8019 << 16);
    // 0x80189784: andi        $t2, $t1, 0x1
    ctx->r10 = ctx->r9 & 0X1;
    // 0x80189788: beql        $t2, $zero, L_801897AC
    if (ctx->r10 == 0) {
        // 0x8018978C: lw          $t3, 0x50($a2)
        ctx->r11 = MEM_W(ctx->r6, 0X50);
            goto L_801897AC;
    }
    goto skip_3;
    // 0x8018978C: lw          $t3, 0x50($a2)
    ctx->r11 = MEM_W(ctx->r6, 0X50);
    skip_3:
    // 0x80189790: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    // 0x80189794: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80189798: jalr        $t9
    // 0x8018979C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8018979C: nop

    after_2:
    // 0x801897A0: b           L_80189874
    // 0x801897A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80189874;
    // 0x801897A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801897A8: lw          $t3, 0x50($a2)
    ctx->r11 = MEM_W(ctx->r6, 0X50);
L_801897AC:
    // 0x801897AC: addiu       $t5, $t5, 0x751C
    ctx->r13 = ADD32(ctx->r13, 0X751C);
    // 0x801897B0: lh          $t6, 0x28D0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X28D0);
    // 0x801897B4: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x801897B8: subu        $t4, $t4, $t3
    ctx->r12 = SUB32(ctx->r12, ctx->r11);
    // 0x801897BC: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x801897C0: subu        $t4, $t4, $t3
    ctx->r12 = SUB32(ctx->r12, ctx->r11);
    // 0x801897C4: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x801897C8: addu        $v0, $t4, $t5
    ctx->r2 = ADD32(ctx->r12, ctx->r13);
    // 0x801897CC: lbu         $t7, 0x0($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X0);
    // 0x801897D0: beql        $t6, $t7, L_801897F4
    if (ctx->r14 == ctx->r15) {
        // 0x801897D4: lhu         $v1, 0x10($v0)
        ctx->r3 = MEM_HU(ctx->r2, 0X10);
            goto L_801897F4;
    }
    goto skip_4;
    // 0x801897D4: lhu         $v1, 0x10($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X10);
    skip_4:
    // 0x801897D8: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    // 0x801897DC: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x801897E0: jalr        $t9
    // 0x801897E4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801897E4: nop

    after_3:
    // 0x801897E8: b           L_80189874
    // 0x801897EC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80189874;
    // 0x801897EC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801897F0: lhu         $v1, 0x10($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X10);
L_801897F4:
    // 0x801897F4: addiu       $at, $zero, 0x20
    ctx->r1 = ADD32(0, 0X20);
    // 0x801897F8: beq         $v1, $at, L_8018985C
    if (ctx->r3 == ctx->r1) {
        // 0x801897FC: addiu       $at, $zero, 0x40
        ctx->r1 = ADD32(0, 0X40);
            goto L_8018985C;
    }
    // 0x801897FC: addiu       $at, $zero, 0x40
    ctx->r1 = ADD32(0, 0X40);
    // 0x80189800: beq         $v1, $at, L_8018985C
    if (ctx->r3 == ctx->r1) {
        // 0x80189804: nop
    
            goto L_8018985C;
    }
    // 0x80189804: nop

    // 0x80189808: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    // 0x8018980C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189810: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189814: beq         $v1, $zero, L_80189838
    if (ctx->r3 == 0) {
        // 0x80189818: or          $a1, $v1, $zero
        ctx->r5 = ctx->r3 | 0;
            goto L_80189838;
    }
    // 0x80189818: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    // 0x8018981C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189820: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189824: jalr        $t9
    // 0x80189828: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80189828: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_4:
    // 0x8018982C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80189830: b           L_8018983C
    // 0x80189834: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_8018983C;
    // 0x80189834: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189838:
    // 0x80189838: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8018983C:
    // 0x8018983C: beq         $v1, $zero, L_8018985C
    if (ctx->r3 == 0) {
        // 0x80189840: nop
    
            goto L_8018985C;
    }
    // 0x80189840: nop

    // 0x80189844: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    // 0x80189848: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x8018984C: jalr        $t9
    // 0x80189850: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80189850: nop

    after_5:
    // 0x80189854: b           L_80189874
    // 0x80189858: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80189874;
    // 0x80189858: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8018985C:
    // 0x8018985C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189860: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80189864: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80189868: jalr        $t9
    // 0x8018986C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x8018986C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_6:
    // 0x80189870: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80189874:
    // 0x80189874: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80189878: jr          $ra
    // 0x8018987C: nop

    return;
    // 0x8018987C: nop

;}
RECOMP_FUNC void func_80189880(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189880: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80189884: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80189888: lw          $t6, 0x50($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X50);
    // 0x8018988C: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80189890: addiu       $t8, $t8, 0x751C
    ctx->r24 = ADD32(ctx->r24, 0X751C);
    // 0x80189894: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80189898: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x8018989C: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x801898A0: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x801898A4: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x801898A8: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x801898AC: lhu         $v1, 0xE($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0XE);
    // 0x801898B0: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801898B4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801898B8: beq         $v1, $zero, L_801898D0
    if (ctx->r3 == 0) {
        // 0x801898BC: addiu       $a0, $a2, 0x8
        ctx->r4 = ADD32(ctx->r6, 0X8);
            goto L_801898D0;
    }
    // 0x801898BC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801898C0: beql        $v1, $at, L_801899B0
    if (ctx->r3 == ctx->r1) {
        // 0x801898C4: lh          $t6, 0x16($v0)
        ctx->r14 = MEM_H(ctx->r2, 0X16);
            goto L_801899B0;
    }
    goto skip_0;
    // 0x801898C4: lh          $t6, 0x16($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X16);
    skip_0:
    // 0x801898C8: b           L_80189A44
    // 0x801898CC: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
        goto L_80189A44;
    // 0x801898CC: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
L_801898D0:
    // 0x801898D0: lh          $t9, 0x16($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X16);
    // 0x801898D4: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801898D8: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801898DC: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x801898E0: lwc1        $f0, 0x50($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801898E4: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801898E8: c.le.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl <= ctx->f0.fl;
    // 0x801898EC: nop

    // 0x801898F0: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x801898F4: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_1;
    // 0x801898F4: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_1:
    // 0x801898F8: lh          $t0, 0x1C($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X1C);
    // 0x801898FC: mtc1        $t0, $f8
    ctx->f8.u32l = ctx->r8;
    // 0x80189900: nop

    // 0x80189904: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80189908: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x8018990C: nop

    // 0x80189910: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x80189914: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_2;
    // 0x80189914: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_2:
    // 0x80189918: lh          $t1, 0x18($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X18);
    // 0x8018991C: lwc1        $f0, 0x54($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X54);
    // 0x80189920: mtc1        $t1, $f16
    ctx->f16.u32l = ctx->r9;
    // 0x80189924: nop

    // 0x80189928: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8018992C: c.le.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl <= ctx->f0.fl;
    // 0x80189930: nop

    // 0x80189934: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x80189938: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_3;
    // 0x80189938: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_3:
    // 0x8018993C: lh          $t2, 0x1E($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X1E);
    // 0x80189940: mtc1        $t2, $f4
    ctx->f4.u32l = ctx->r10;
    // 0x80189944: nop

    // 0x80189948: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8018994C: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x80189950: nop

    // 0x80189954: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x80189958: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_4;
    // 0x80189958: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_4:
    // 0x8018995C: lh          $t3, 0x1A($v0)
    ctx->r11 = MEM_H(ctx->r2, 0X1A);
    // 0x80189960: lwc1        $f0, 0x58($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80189964: mtc1        $t3, $f8
    ctx->f8.u32l = ctx->r11;
    // 0x80189968: nop

    // 0x8018996C: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80189970: c.le.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl <= ctx->f0.fl;
    // 0x80189974: nop

    // 0x80189978: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x8018997C: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_5;
    // 0x8018997C: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_5:
    // 0x80189980: lh          $t4, 0x20($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X20);
    // 0x80189984: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80189988: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x8018998C: nop

    // 0x80189990: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80189994: c.le.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl <= ctx->f18.fl;
    // 0x80189998: nop

    // 0x8018999C: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x801899A0: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_6;
    // 0x801899A0: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_6:
    // 0x801899A4: b           L_80189A40
    // 0x801899A8: sw          $t5, 0x44($a2)
    MEM_W(0X44, ctx->r6) = ctx->r13;
        goto L_80189A40;
    // 0x801899A8: sw          $t5, 0x44($a2)
    MEM_W(0X44, ctx->r6) = ctx->r13;
    // 0x801899AC: lh          $t6, 0x16($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X16);
L_801899B0:
    // 0x801899B0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801899B4: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801899B8: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x801899BC: lwc1        $f0, 0x50($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801899C0: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801899C4: c.le.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl <= ctx->f0.fl;
    // 0x801899C8: nop

    // 0x801899CC: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x801899D0: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_7;
    // 0x801899D0: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_7:
    // 0x801899D4: lh          $t7, 0x1C($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X1C);
    // 0x801899D8: mtc1        $t7, $f8
    ctx->f8.u32l = ctx->r15;
    // 0x801899DC: nop

    // 0x801899E0: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801899E4: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x801899E8: nop

    // 0x801899EC: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x801899F0: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_8;
    // 0x801899F0: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_8:
    // 0x801899F4: lh          $t8, 0x1A($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X1A);
    // 0x801899F8: lwc1        $f0, 0x58($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X58);
    // 0x801899FC: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x80189A00: nop

    // 0x80189A04: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80189A08: c.le.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl <= ctx->f0.fl;
    // 0x80189A0C: nop

    // 0x80189A10: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x80189A14: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_9;
    // 0x80189A14: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_9:
    // 0x80189A18: lh          $t9, 0x20($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X20);
    // 0x80189A1C: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x80189A20: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x80189A24: nop

    // 0x80189A28: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80189A2C: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x80189A30: nop

    // 0x80189A34: bc1fl       L_80189A44
    if (!c1cs) {
        // 0x80189A38: lw          $t1, 0x44($a2)
        ctx->r9 = MEM_W(ctx->r6, 0X44);
            goto L_80189A44;
    }
    goto skip_10;
    // 0x80189A38: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
    skip_10:
    // 0x80189A3C: sw          $t0, 0x44($a2)
    MEM_W(0X44, ctx->r6) = ctx->r8;
L_80189A40:
    // 0x80189A40: lw          $t1, 0x44($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X44);
L_80189A44:
    // 0x80189A44: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189A48: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80189A4C: beql        $t1, $zero, L_80189A60
    if (ctx->r9 == 0) {
        // 0x80189A50: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80189A60;
    }
    goto skip_11;
    // 0x80189A50: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_11:
    // 0x80189A54: jalr        $t9
    // 0x80189A58: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80189A58: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
    // 0x80189A5C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80189A60:
    // 0x80189A60: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80189A64: jr          $ra
    // 0x80189A68: nop

    return;
    // 0x80189A68: nop

;}
RECOMP_FUNC void func_80189A6C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80189A6C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80189A70: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80189A74: lw          $t6, 0x50($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X50);
    // 0x80189A78: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80189A7C: addiu       $t8, $t8, 0x751C
    ctx->r24 = ADD32(ctx->r24, 0X751C);
    // 0x80189A80: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80189A84: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80189A88: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80189A8C: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80189A90: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80189A94: addu        $a2, $t7, $t8
    ctx->r6 = ADD32(ctx->r15, ctx->r24);
    // 0x80189A98: lhu         $v1, 0x10($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0X10);
    // 0x80189A9C: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80189AA0: slti        $at, $v1, 0x41
    ctx->r1 = SIGNED(ctx->r3) < 0X41 ? 1 : 0;
    // 0x80189AA4: bne         $at, $zero, L_80189AC0
    if (ctx->r1 != 0) {
        // 0x80189AA8: sll         $t9, $v1, 2
        ctx->r25 = S32(ctx->r3 << 2);
            goto L_80189AC0;
    }
    // 0x80189AA8: sll         $t9, $v1, 2
    ctx->r25 = S32(ctx->r3 << 2);
    // 0x80189AAC: addiu       $at, $zero, 0x80
    ctx->r1 = ADD32(0, 0X80);
    // 0x80189AB0: beql        $v1, $at, L_80189AFC
    if (ctx->r3 == ctx->r1) {
        // 0x80189AB4: lw          $v0, 0x4($a2)
        ctx->r2 = MEM_W(ctx->r6, 0X4);
            goto L_80189AFC;
    }
    goto skip_0;
    // 0x80189AB4: lw          $v0, 0x4($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4);
    skip_0:
    // 0x80189AB8: b           L_80189F24
    // 0x80189ABC: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
        goto L_80189F24;
    // 0x80189ABC: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
L_80189AC0:
    // 0x80189AC0: slti        $at, $v1, 0x21
    ctx->r1 = SIGNED(ctx->r3) < 0X21 ? 1 : 0;
    // 0x80189AC4: bne         $at, $zero, L_80189ADC
    if (ctx->r1 != 0) {
        // 0x80189AC8: addiu       $at, $zero, 0x40
        ctx->r1 = ADD32(0, 0X40);
            goto L_80189ADC;
    }
    // 0x80189AC8: addiu       $at, $zero, 0x40
    ctx->r1 = ADD32(0, 0X40);
    // 0x80189ACC: beql        $v1, $at, L_80189EAC
    if (ctx->r3 == ctx->r1) {
        // 0x80189AD0: lw          $v0, 0x4($a2)
        ctx->r2 = MEM_W(ctx->r6, 0X4);
            goto L_80189EAC;
    }
    goto skip_1;
    // 0x80189AD0: lw          $v0, 0x4($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4);
    skip_1:
    // 0x80189AD4: b           L_80189F24
    // 0x80189AD8: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
        goto L_80189F24;
    // 0x80189AD8: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
L_80189ADC:
    // 0x80189ADC: sltiu       $at, $v1, 0x21
    ctx->r1 = ctx->r3 < 0X21 ? 1 : 0;
    // 0x80189AE0: beq         $at, $zero, L_80189F20
    if (ctx->r1 == 0) {
        // 0x80189AE4: lui         $at, 0x801A
        ctx->r1 = S32(0X801A << 16);
            goto L_80189F20;
    }
    // 0x80189AE4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80189AE8: addu        $at, $at, $t9
    gpr jr_addend_80189AF0 = ctx->r25;
    ctx->r1 = ADD32(ctx->r1, ctx->r25);
    // 0x80189AEC: lw          $t9, -0x3020($at)
    ctx->r25 = ADD32(ctx->r1, -0X3020);
    // 0x80189AF0: jr          $t9
    // 0x80189AF4: nop

    switch (jr_addend_80189AF0 >> 2) {
        case 0: goto L_80189AF8; break;
        case 1: goto L_80189AF8; break;
        case 2: goto L_80189C6C; break;
        case 3: goto L_80189F20; break;
        case 4: goto L_80189C6C; break;
        case 5: goto L_80189F20; break;
        case 6: goto L_80189F20; break;
        case 7: goto L_80189F20; break;
        case 8: goto L_80189F20; break;
        case 9: goto L_80189F20; break;
        case 10: goto L_80189F20; break;
        case 11: goto L_80189F20; break;
        case 12: goto L_80189F20; break;
        case 13: goto L_80189F20; break;
        case 14: goto L_80189F20; break;
        case 15: goto L_80189F20; break;
        case 16: goto L_80189D78; break;
        case 17: goto L_80189F20; break;
        case 18: goto L_80189F20; break;
        case 19: goto L_80189F20; break;
        case 20: goto L_80189F20; break;
        case 21: goto L_80189F20; break;
        case 22: goto L_80189F20; break;
        case 23: goto L_80189F20; break;
        case 24: goto L_80189F20; break;
        case 25: goto L_80189F20; break;
        case 26: goto L_80189F20; break;
        case 27: goto L_80189F20; break;
        case 28: goto L_80189F20; break;
        case 29: goto L_80189F20; break;
        case 30: goto L_80189F20; break;
        case 31: goto L_80189F20; break;
        case 32: goto L_80189E2C; break;
        default: switch_error(__func__, 0x80189AF0, 0x8019CFE0);
    }
    // 0x80189AF4: nop

L_80189AF8:
    // 0x80189AF8: lw          $v0, 0x4($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4);
L_80189AFC:
    // 0x80189AFC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80189B00: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189B04: beq         $v0, $zero, L_80189B2C
    if (ctx->r2 == 0) {
        // 0x80189B08: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80189B2C;
    }
    // 0x80189B08: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80189B0C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189B10: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189B14: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189B18: jalr        $t9
    // 0x80189B1C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80189B1C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_0:
    // 0x80189B20: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189B24: b           L_80189B2C
    // 0x80189B28: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80189B2C;
    // 0x80189B28: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189B2C:
    // 0x80189B2C: bne         $v1, $zero, L_80189C64
    if (ctx->r3 != 0) {
        // 0x80189B30: lui         $t3, 0x8019
        ctx->r11 = S32(0X8019 << 16);
            goto L_80189C64;
    }
    // 0x80189B30: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x80189B34: lw          $t1, 0x50($a3)
    ctx->r9 = MEM_W(ctx->r7, 0X50);
    // 0x80189B38: addiu       $t3, $t3, 0x751C
    ctx->r11 = ADD32(ctx->r11, 0X751C);
    // 0x80189B3C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    // 0x80189B40: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80189B44: subu        $t2, $t2, $t1
    ctx->r10 = SUB32(ctx->r10, ctx->r9);
    // 0x80189B48: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80189B4C: subu        $t2, $t2, $t1
    ctx->r10 = SUB32(ctx->r10, ctx->r9);
    // 0x80189B50: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80189B54: jal         0x8018A104
    // 0x80189B58: addu        $a0, $t2, $t3
    ctx->r4 = ADD32(ctx->r10, ctx->r11);
    func_8018A104(rdram, ctx);
        goto after_1;
    // 0x80189B58: addu        $a0, $t2, $t3
    ctx->r4 = ADD32(ctx->r10, ctx->r11);
    after_1:
    // 0x80189B5C: beq         $v0, $zero, L_80189C64
    if (ctx->r2 == 0) {
        // 0x80189B60: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_80189C64;
    }
    // 0x80189B60: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189B64: lw          $t4, 0x50($a3)
    ctx->r12 = MEM_W(ctx->r7, 0X50);
    // 0x80189B68: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x80189B6C: addiu       $t6, $t6, 0x751C
    ctx->r14 = ADD32(ctx->r14, 0X751C);
    // 0x80189B70: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x80189B74: subu        $t5, $t5, $t4
    ctx->r13 = SUB32(ctx->r13, ctx->r12);
    // 0x80189B78: sll         $t5, $t5, 2
    ctx->r13 = S32(ctx->r13 << 2);
    // 0x80189B7C: subu        $t5, $t5, $t4
    ctx->r13 = SUB32(ctx->r13, ctx->r12);
    // 0x80189B80: sll         $t5, $t5, 2
    ctx->r13 = S32(ctx->r13 << 2);
    // 0x80189B84: addu        $a2, $t5, $t6
    ctx->r6 = ADD32(ctx->r13, ctx->r14);
    // 0x80189B88: lw          $v0, 0x8($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X8);
    // 0x80189B8C: beql        $v0, $zero, L_80189BD8
    if (ctx->r2 == 0) {
        // 0x80189B90: lhu         $t8, 0x10($a2)
        ctx->r24 = MEM_HU(ctx->r6, 0X10);
            goto L_80189BD8;
    }
    goto skip_2;
    // 0x80189B90: lhu         $t8, 0x10($a2)
    ctx->r24 = MEM_HU(ctx->r6, 0X10);
    skip_2:
    // 0x80189B94: beq         $v0, $zero, L_80189BC4
    if (ctx->r2 == 0) {
        // 0x80189B98: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_80189BC4;
    }
    // 0x80189B98: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80189B9C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189BA0: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189BA4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189BA8: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189BAC: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80189BB0: jalr        $t9
    // 0x80189BB4: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80189BB4: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_2:
    // 0x80189BB8: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189BBC: b           L_80189BC4
    // 0x80189BC0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80189BC4;
    // 0x80189BC0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189BC4:
    // 0x80189BC4: beq         $v1, $zero, L_80189C64
    if (ctx->r3 == 0) {
        // 0x80189BC8: addiu       $t7, $zero, 0x2
        ctx->r15 = ADD32(0, 0X2);
            goto L_80189C64;
    }
    // 0x80189BC8: addiu       $t7, $zero, 0x2
    ctx->r15 = ADD32(0, 0X2);
    // 0x80189BCC: b           L_80189F20
    // 0x80189BD0: sw          $t7, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r15;
        goto L_80189F20;
    // 0x80189BD0: sw          $t7, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r15;
    // 0x80189BD4: lhu         $t8, 0x10($a2)
    ctx->r24 = MEM_HU(ctx->r6, 0X10);
L_80189BD8:
    // 0x80189BD8: addiu       $at, $zero, 0x80
    ctx->r1 = ADD32(0, 0X80);
    // 0x80189BDC: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80189BE0: bne         $t8, $at, L_80189C5C
    if (ctx->r24 != ctx->r1) {
        // 0x80189BE4: addiu       $t0, $t0, -0x7D40
        ctx->r8 = ADD32(ctx->r8, -0X7D40);
            goto L_80189C5C;
    }
    // 0x80189BE4: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80189BE8: lh          $t2, 0x285C($t0)
    ctx->r10 = MEM_H(ctx->r8, 0X285C);
    // 0x80189BEC: lh          $t1, 0x285E($t0)
    ctx->r9 = MEM_H(ctx->r8, 0X285E);
    // 0x80189BF0: addiu       $a1, $zero, 0x17D
    ctx->r5 = ADD32(0, 0X17D);
    // 0x80189BF4: sll         $t3, $t2, 3
    ctx->r11 = S32(ctx->r10 << 3);
    // 0x80189BF8: subu        $t3, $t3, $t2
    ctx->r11 = SUB32(ctx->r11, ctx->r10);
    // 0x80189BFC: addu        $v0, $t1, $t3
    ctx->r2 = ADD32(ctx->r9, ctx->r11);
    // 0x80189C00: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80189C04: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x80189C08: slti        $at, $v0, 0x10
    ctx->r1 = SIGNED(ctx->r2) < 0X10 ? 1 : 0;
    // 0x80189C0C: bne         $at, $zero, L_80189C64
    if (ctx->r1 != 0) {
        // 0x80189C10: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80189C64;
    }
    // 0x80189C10: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189C14: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189C18: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189C1C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189C20: jalr        $t9
    // 0x80189C24: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80189C24: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_3:
    // 0x80189C28: bne         $v0, $zero, L_80189C64
    if (ctx->r2 != 0) {
        // 0x80189C2C: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_80189C64;
    }
    // 0x80189C2C: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189C30: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189C34: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x80189C38: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189C3C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189C40: addiu       $a1, $zero, 0x1A8
    ctx->r5 = ADD32(0, 0X1A8);
    // 0x80189C44: jalr        $t9
    // 0x80189C48: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80189C48: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_4:
    // 0x80189C4C: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189C50: addiu       $t4, $zero, 0x2
    ctx->r12 = ADD32(0, 0X2);
    // 0x80189C54: b           L_80189F20
    // 0x80189C58: sw          $t4, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r12;
        goto L_80189F20;
    // 0x80189C58: sw          $t4, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r12;
L_80189C5C:
    // 0x80189C5C: addiu       $t5, $zero, 0x2
    ctx->r13 = ADD32(0, 0X2);
    // 0x80189C60: sw          $t5, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r13;
L_80189C64:
    // 0x80189C64: b           L_80189F24
    // 0x80189C68: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
        goto L_80189F24;
    // 0x80189C68: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
L_80189C6C:
    // 0x80189C6C: lw          $v0, 0x4($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4);
    // 0x80189C70: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80189C74: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189C78: beq         $v0, $zero, L_80189CA0
    if (ctx->r2 == 0) {
        // 0x80189C7C: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80189CA0;
    }
    // 0x80189C7C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80189C80: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189C84: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189C88: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189C8C: jalr        $t9
    // 0x80189C90: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80189C90: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_5:
    // 0x80189C94: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189C98: b           L_80189CA0
    // 0x80189C9C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80189CA0;
    // 0x80189C9C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189CA0:
    // 0x80189CA0: bne         $v1, $zero, L_80189D70
    if (ctx->r3 != 0) {
        // 0x80189CA4: lui         $t8, 0x8019
        ctx->r24 = S32(0X8019 << 16);
            goto L_80189D70;
    }
    // 0x80189CA4: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80189CA8: lw          $t6, 0x50($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X50);
    // 0x80189CAC: addiu       $t8, $t8, 0x751C
    ctx->r24 = ADD32(ctx->r24, 0X751C);
    // 0x80189CB0: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    // 0x80189CB4: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80189CB8: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80189CBC: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80189CC0: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80189CC4: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80189CC8: jal         0x8018A104
    // 0x80189CCC: addu        $a0, $t7, $t8
    ctx->r4 = ADD32(ctx->r15, ctx->r24);
    func_8018A104(rdram, ctx);
        goto after_6;
    // 0x80189CCC: addu        $a0, $t7, $t8
    ctx->r4 = ADD32(ctx->r15, ctx->r24);
    after_6:
    // 0x80189CD0: beq         $v0, $zero, L_80189D70
    if (ctx->r2 == 0) {
        // 0x80189CD4: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_80189D70;
    }
    // 0x80189CD4: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189CD8: lw          $t2, 0x50($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X50);
    // 0x80189CDC: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80189CE0: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80189CE4: sll         $t1, $t2, 2
    ctx->r9 = S32(ctx->r10 << 2);
    // 0x80189CE8: subu        $t1, $t1, $t2
    ctx->r9 = SUB32(ctx->r9, ctx->r10);
    // 0x80189CEC: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x80189CF0: lw          $t5, 0x2964($t0)
    ctx->r13 = MEM_W(ctx->r8, 0X2964);
    // 0x80189CF4: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x80189CF8: subu        $t1, $t1, $t2
    ctx->r9 = SUB32(ctx->r9, ctx->r10);
    // 0x80189CFC: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x80189D00: addiu       $t3, $t3, 0x751C
    ctx->r11 = ADD32(ctx->r11, 0X751C);
    // 0x80189D04: addu        $a2, $t1, $t3
    ctx->r6 = ADD32(ctx->r9, ctx->r11);
    // 0x80189D08: lw          $t4, 0x24($a2)
    ctx->r12 = MEM_W(ctx->r6, 0X24);
    // 0x80189D0C: lw          $t9, 0x0($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X0);
    // 0x80189D10: and         $t6, $t4, $t9
    ctx->r14 = ctx->r12 & ctx->r25;
    // 0x80189D14: beq         $t6, $zero, L_80189D70
    if (ctx->r14 == 0) {
        // 0x80189D18: nop
    
            goto L_80189D70;
    }
    // 0x80189D18: nop

    // 0x80189D1C: lw          $v0, 0x8($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X8);
    // 0x80189D20: addiu       $t8, $zero, 0x2
    ctx->r24 = ADD32(0, 0X2);
    // 0x80189D24: beql        $v0, $zero, L_80189D70
    if (ctx->r2 == 0) {
        // 0x80189D28: sw          $t8, 0x44($a3)
        MEM_W(0X44, ctx->r7) = ctx->r24;
            goto L_80189D70;
    }
    goto skip_3;
    // 0x80189D28: sw          $t8, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r24;
    skip_3:
    // 0x80189D2C: beq         $v0, $zero, L_80189D5C
    if (ctx->r2 == 0) {
        // 0x80189D30: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_80189D5C;
    }
    // 0x80189D30: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80189D34: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189D38: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189D3C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189D40: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189D44: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80189D48: jalr        $t9
    // 0x80189D4C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80189D4C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_7:
    // 0x80189D50: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189D54: b           L_80189D5C
    // 0x80189D58: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80189D5C;
    // 0x80189D58: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189D5C:
    // 0x80189D5C: beq         $v1, $zero, L_80189D70
    if (ctx->r3 == 0) {
        // 0x80189D60: addiu       $t7, $zero, 0x2
        ctx->r15 = ADD32(0, 0X2);
            goto L_80189D70;
    }
    // 0x80189D60: addiu       $t7, $zero, 0x2
    ctx->r15 = ADD32(0, 0X2);
    // 0x80189D64: b           L_80189F20
    // 0x80189D68: sw          $t7, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r15;
        goto L_80189F20;
    // 0x80189D68: sw          $t7, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r15;
    // 0x80189D6C: sw          $t8, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r24;
L_80189D70:
    // 0x80189D70: b           L_80189F24
    // 0x80189D74: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
        goto L_80189F24;
    // 0x80189D74: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
L_80189D78:
    // 0x80189D78: lw          $v0, 0x4($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4);
    // 0x80189D7C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80189D80: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189D84: beq         $v0, $zero, L_80189DAC
    if (ctx->r2 == 0) {
        // 0x80189D88: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80189DAC;
    }
    // 0x80189D88: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80189D8C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189D90: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189D94: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189D98: jalr        $t9
    // 0x80189D9C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x80189D9C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_8:
    // 0x80189DA0: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189DA4: b           L_80189DAC
    // 0x80189DA8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80189DAC;
    // 0x80189DA8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189DAC:
    // 0x80189DAC: bnel        $v1, $zero, L_80189F24
    if (ctx->r3 != 0) {
        // 0x80189DB0: lw          $t2, 0x44($a3)
        ctx->r10 = MEM_W(ctx->r7, 0X44);
            goto L_80189F24;
    }
    goto skip_4;
    // 0x80189DB0: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
    skip_4:
    // 0x80189DB4: lw          $t2, 0x50($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X50);
    // 0x80189DB8: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80189DBC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    // 0x80189DC0: sll         $t1, $t2, 2
    ctx->r9 = S32(ctx->r10 << 2);
    // 0x80189DC4: subu        $t1, $t1, $t2
    ctx->r9 = SUB32(ctx->r9, ctx->r10);
    // 0x80189DC8: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x80189DCC: subu        $t1, $t1, $t2
    ctx->r9 = SUB32(ctx->r9, ctx->r10);
    // 0x80189DD0: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x80189DD4: addu        $a0, $a0, $t1
    ctx->r4 = ADD32(ctx->r4, ctx->r9);
    // 0x80189DD8: jal         0x80002560
    // 0x80189DDC: lhu         $a0, 0x7528($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X7528);
    func_80002560(rdram, ctx);
        goto after_9;
    // 0x80189DDC: lhu         $a0, 0x7528($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X7528);
    after_9:
    // 0x80189DE0: bne         $v0, $zero, L_80189F20
    if (ctx->r2 != 0) {
        // 0x80189DE4: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_80189F20;
    }
    // 0x80189DE4: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189DE8: lw          $t3, 0x50($a3)
    ctx->r11 = MEM_W(ctx->r7, 0X50);
    // 0x80189DEC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80189DF0: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    // 0x80189DF4: sll         $t5, $t3, 2
    ctx->r13 = S32(ctx->r11 << 2);
    // 0x80189DF8: subu        $t5, $t5, $t3
    ctx->r13 = SUB32(ctx->r13, ctx->r11);
    // 0x80189DFC: sll         $t5, $t5, 2
    ctx->r13 = S32(ctx->r13 << 2);
    // 0x80189E00: subu        $t5, $t5, $t3
    ctx->r13 = SUB32(ctx->r13, ctx->r11);
    // 0x80189E04: sll         $t5, $t5, 2
    ctx->r13 = S32(ctx->r13 << 2);
    // 0x80189E08: addu        $a0, $a0, $t5
    ctx->r4 = ADD32(ctx->r4, ctx->r13);
    // 0x80189E0C: jal         0x8018A180
    // 0x80189E10: lhu         $a0, 0x7528($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X7528);
    func_8018A180(rdram, ctx);
        goto after_10;
    // 0x80189E10: lhu         $a0, 0x7528($a0)
    ctx->r4 = MEM_HU(ctx->r4, 0X7528);
    after_10:
    // 0x80189E14: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80189E18: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80189E1C: jalr        $t9
    // 0x80189E20: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x80189E20: nop

    after_11:
    // 0x80189E24: b           L_80189F20
    // 0x80189E28: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
        goto L_80189F20;
    // 0x80189E28: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
L_80189E2C:
    // 0x80189E2C: lw          $v0, 0x4($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4);
    // 0x80189E30: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80189E34: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189E38: beq         $v0, $zero, L_80189E60
    if (ctx->r2 == 0) {
        // 0x80189E3C: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80189E60;
    }
    // 0x80189E3C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80189E40: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189E44: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189E48: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189E4C: jalr        $t9
    // 0x80189E50: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x80189E50: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_12:
    // 0x80189E54: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189E58: b           L_80189E60
    // 0x80189E5C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80189E60;
    // 0x80189E5C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189E60:
    // 0x80189E60: bne         $v1, $zero, L_80189F20
    if (ctx->r3 != 0) {
        // 0x80189E64: lui         $a1, 0x8019
        ctx->r5 = S32(0X8019 << 16);
            goto L_80189F20;
    }
    // 0x80189E64: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80189E68: lw          $t4, 0x50($a3)
    ctx->r12 = MEM_W(ctx->r7, 0X50);
    // 0x80189E6C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189E70: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x80189E74: sll         $t6, $t4, 2
    ctx->r14 = S32(ctx->r12 << 2);
    // 0x80189E78: subu        $t6, $t6, $t4
    ctx->r14 = SUB32(ctx->r14, ctx->r12);
    // 0x80189E7C: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80189E80: subu        $t6, $t6, $t4
    ctx->r14 = SUB32(ctx->r14, ctx->r12);
    // 0x80189E84: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80189E88: addu        $a1, $a1, $t6
    ctx->r5 = ADD32(ctx->r5, ctx->r14);
    // 0x80189E8C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189E90: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189E94: lw          $a1, 0x7520($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X7520);
    // 0x80189E98: jalr        $t9
    // 0x80189E9C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_13;
    // 0x80189E9C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_13:
    // 0x80189EA0: b           L_80189F20
    // 0x80189EA4: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
        goto L_80189F20;
    // 0x80189EA4: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189EA8: lw          $v0, 0x4($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X4);
L_80189EAC:
    // 0x80189EAC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80189EB0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189EB4: beq         $v0, $zero, L_80189EDC
    if (ctx->r2 == 0) {
        // 0x80189EB8: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80189EDC;
    }
    // 0x80189EB8: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80189EBC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189EC0: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80189EC4: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189EC8: jalr        $t9
    // 0x80189ECC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_14;
    // 0x80189ECC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_14:
    // 0x80189ED0: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80189ED4: b           L_80189EDC
    // 0x80189ED8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80189EDC;
    // 0x80189ED8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80189EDC:
    // 0x80189EDC: beq         $v1, $zero, L_80189F20
    if (ctx->r3 == 0) {
        // 0x80189EE0: lui         $a1, 0x8019
        ctx->r5 = S32(0X8019 << 16);
            goto L_80189F20;
    }
    // 0x80189EE0: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80189EE4: lw          $t7, 0x50($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X50);
    // 0x80189EE8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80189EEC: addiu       $t9, $t9, 0x48EC
    ctx->r25 = ADD32(ctx->r25, 0X48EC);
    // 0x80189EF0: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80189EF4: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x80189EF8: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80189EFC: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x80189F00: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80189F04: addu        $a1, $a1, $t8
    ctx->r5 = ADD32(ctx->r5, ctx->r24);
    // 0x80189F08: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80189F0C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80189F10: lw          $a1, 0x7520($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X7520);
    // 0x80189F14: jalr        $t9
    // 0x80189F18: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_15;
    // 0x80189F18: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_15:
    // 0x80189F1C: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
L_80189F20:
    // 0x80189F20: lw          $t2, 0x44($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X44);
L_80189F24:
    // 0x80189F24: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80189F28: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80189F2C: bne         $t2, $at, L_80189FB8
    if (ctx->r10 != ctx->r1) {
        // 0x80189F30: addiu       $t0, $t0, -0x7D40
        ctx->r8 = ADD32(ctx->r8, -0X7D40);
            goto L_80189FB8;
    }
    // 0x80189F30: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80189F34: lw          $t1, 0x2964($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X2964);
    // 0x80189F38: lui         $v0, 0x8000
    ctx->r2 = S32(0X8000 << 16);
    // 0x80189F3C: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80189F40: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x80189F44: sll         $t5, $t3, 0
    ctx->r13 = S32(ctx->r11 << 0);
    // 0x80189F48: bltzl       $t5, L_80189F64
    if (SIGNED(ctx->r13) < 0) {
        // 0x80189F4C: sw          $v1, 0x44($a3)
        MEM_W(0X44, ctx->r7) = ctx->r3;
            goto L_80189F64;
    }
    goto skip_5;
    // 0x80189F4C: sw          $v1, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r3;
    skip_5:
    // 0x80189F50: lw          $t4, 0x28C4($t0)
    ctx->r12 = MEM_W(ctx->r8, 0X28C4);
    // 0x80189F54: and         $t6, $t4, $v0
    ctx->r14 = ctx->r12 & ctx->r2;
    // 0x80189F58: beql        $t6, $zero, L_80189F68
    if (ctx->r14 == 0) {
        // 0x80189F5C: lw          $t7, 0x50($a3)
        ctx->r15 = MEM_W(ctx->r7, 0X50);
            goto L_80189F68;
    }
    goto skip_6;
    // 0x80189F5C: lw          $t7, 0x50($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X50);
    skip_6:
    // 0x80189F60: sw          $v1, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r3;
L_80189F64:
    // 0x80189F64: lw          $t7, 0x50($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X50);
L_80189F68:
    // 0x80189F68: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x80189F6C: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80189F70: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80189F74: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x80189F78: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80189F7C: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x80189F80: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80189F84: addu        $v0, $v0, $t8
    ctx->r2 = ADD32(ctx->r2, ctx->r24);
    // 0x80189F88: lh          $v0, 0x7544($v0)
    ctx->r2 = MEM_H(ctx->r2, 0X7544);
    // 0x80189F8C: beql        $v0, $zero, L_80189FAC
    if (ctx->r2 == 0) {
        // 0x80189F90: lw          $t1, 0x2BC8($t0)
        ctx->r9 = MEM_W(ctx->r8, 0X2BC8);
            goto L_80189FAC;
    }
    goto skip_7;
    // 0x80189F90: lw          $t1, 0x2BC8($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X2BC8);
    skip_7:
    // 0x80189F94: lh          $t9, 0x2874($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X2874);
    // 0x80189F98: addiu       $t2, $t9, 0x1
    ctx->r10 = ADD32(ctx->r25, 0X1);
    // 0x80189F9C: beql        $t2, $v0, L_80189FAC
    if (ctx->r10 == ctx->r2) {
        // 0x80189FA0: lw          $t1, 0x2BC8($t0)
        ctx->r9 = MEM_W(ctx->r8, 0X2BC8);
            goto L_80189FAC;
    }
    goto skip_8;
    // 0x80189FA0: lw          $t1, 0x2BC8($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X2BC8);
    skip_8:
    // 0x80189FA4: sw          $v1, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r3;
    // 0x80189FA8: lw          $t1, 0x2BC8($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X2BC8);
L_80189FAC:
    // 0x80189FAC: beql        $t1, $zero, L_80189FBC
    if (ctx->r9 == 0) {
        // 0x80189FB0: lw          $t3, 0x44($a3)
        ctx->r11 = MEM_W(ctx->r7, 0X44);
            goto L_80189FBC;
    }
    goto skip_9;
    // 0x80189FB0: lw          $t3, 0x44($a3)
    ctx->r11 = MEM_W(ctx->r7, 0X44);
    skip_9:
    // 0x80189FB4: sw          $v1, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r3;
L_80189FB8:
    // 0x80189FB8: lw          $t3, 0x44($a3)
    ctx->r11 = MEM_W(ctx->r7, 0X44);
L_80189FBC:
    // 0x80189FBC: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80189FC0: bne         $t3, $at, L_8018A080
    if (ctx->r11 != ctx->r1) {
        // 0x80189FC4: nop
    
            goto L_8018A080;
    }
    // 0x80189FC4: nop

    // 0x80189FC8: lw          $t5, 0x2BD0($t0)
    ctx->r13 = MEM_W(ctx->r8, 0X2BD0);
    // 0x80189FCC: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80189FD0: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80189FD4: ori         $t4, $t5, 0x1
    ctx->r12 = ctx->r13 | 0X1;
    // 0x80189FD8: sw          $t4, 0x2BD0($t0)
    MEM_W(0X2BD0, ctx->r8) = ctx->r12;
    // 0x80189FDC: lw          $t6, 0x50($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X50);
    // 0x80189FE0: addiu       $t1, $t1, 0x751C
    ctx->r9 = ADD32(ctx->r9, 0X751C);
    // 0x80189FE4: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80189FE8: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80189FEC: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80189FF0: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80189FF4: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80189FF8: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80189FFC: lhu         $t8, 0x7528($t8)
    ctx->r24 = MEM_HU(ctx->r24, 0X7528);
    // 0x8018A000: sw          $t8, 0x2BC8($t0)
    MEM_W(0X2BC8, ctx->r8) = ctx->r24;
    // 0x8018A004: lw          $t9, 0x50($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X50);
    // 0x8018A008: sll         $t2, $t9, 2
    ctx->r10 = S32(ctx->r25 << 2);
    // 0x8018A00C: subu        $t2, $t2, $t9
    ctx->r10 = SUB32(ctx->r10, ctx->r25);
    // 0x8018A010: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x8018A014: subu        $t2, $t2, $t9
    ctx->r10 = SUB32(ctx->r10, ctx->r25);
    // 0x8018A018: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x8018A01C: addu        $a2, $t2, $t1
    ctx->r6 = ADD32(ctx->r10, ctx->r9);
    // 0x8018A020: lhu         $t3, 0x10($a2)
    ctx->r11 = MEM_HU(ctx->r6, 0X10);
    // 0x8018A024: sltiu       $at, $t3, 0x9
    ctx->r1 = ctx->r11 < 0X9 ? 1 : 0;
    // 0x8018A028: beq         $at, $zero, L_8018A064
    if (ctx->r1 == 0) {
        // 0x8018A02C: sll         $t3, $t3, 2
        ctx->r11 = S32(ctx->r11 << 2);
            goto L_8018A064;
    }
    // 0x8018A02C: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x8018A030: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018A034: addu        $at, $at, $t3
    gpr jr_addend_8018A03C = ctx->r11;
    ctx->r1 = ADD32(ctx->r1, ctx->r11);
    // 0x8018A038: lw          $t3, -0x2F9C($at)
    ctx->r11 = ADD32(ctx->r1, -0X2F9C);
    // 0x8018A03C: jr          $t3
    // 0x8018A040: nop

    switch (jr_addend_8018A03C >> 2) {
        case 0: goto L_8018A044; break;
        case 1: goto L_8018A064; break;
        case 2: goto L_8018A044; break;
        case 3: goto L_8018A064; break;
        case 4: goto L_8018A064; break;
        case 5: goto L_8018A064; break;
        case 6: goto L_8018A064; break;
        case 7: goto L_8018A064; break;
        case 8: goto L_8018A064; break;
        default: switch_error(__func__, 0x8018A03C, 0x8019D064);
    }
    // 0x8018A040: nop

L_8018A044:
    // 0x8018A044: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018A048: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x8018A04C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8018A050: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8018A054: lw          $a1, 0x4($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X4);
    // 0x8018A058: jalr        $t9
    // 0x8018A05C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_16;
    // 0x8018A05C: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_16:
    // 0x8018A060: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
L_8018A064:
    // 0x8018A064: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018A068: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8018A06C: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x8018A070: jalr        $t9
    // 0x8018A074: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_17;
    // 0x8018A074: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_17:
    // 0x8018A078: b           L_8018A0A0
    // 0x8018A07C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8018A0A0;
    // 0x8018A07C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8018A080:
    // 0x8018A080: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018A084: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x8018A088: sw          $zero, 0x44($a3)
    MEM_W(0X44, ctx->r7) = 0;
    // 0x8018A08C: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x8018A090: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x8018A094: jalr        $t9
    // 0x8018A098: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_18;
    // 0x8018A098: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_18:
    // 0x8018A09C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8018A0A0:
    // 0x8018A0A0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8018A0A4: jr          $ra
    // 0x8018A0A8: nop

    return;
    // 0x8018A0A8: nop

;}
RECOMP_FUNC void func_8018A0AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018A0AC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8018A0B0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8018A0B4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8018A0B8: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8018A0BC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8018A0C0: sw          $zero, 0x44($a0)
    MEM_W(0X44, ctx->r4) = 0;
    // 0x8018A0C4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x8018A0C8: jalr        $t9
    // 0x8018A0CC: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8018A0CC: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    after_0:
    // 0x8018A0D0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8018A0D4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8018A0D8: jr          $ra
    // 0x8018A0DC: nop

    return;
    // 0x8018A0DC: nop

;}
RECOMP_FUNC void func_8018A0E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018A0E0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8018A0E4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8018A0E8: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8018A0EC: jalr        $t9
    // 0x8018A0F0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8018A0F0: nop

    after_0:
    // 0x8018A0F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8018A0F8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8018A0FC: jr          $ra
    // 0x8018A100: nop

    return;
    // 0x8018A100: nop

;}
