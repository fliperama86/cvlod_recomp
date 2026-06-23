#!/usr/bin/env python3
"""
apply_patches.py — Apply hand-written patches to regenerated RecompiledFuncs.

Patches are keyed by MIPS VRAM address (which never changes across regens),
not by function name or line number. This makes them resilient to renames.
"""

import re
import os
import sys

FUNC_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "RecompiledFuncs")
ENABLE_LEGACY_RECOMP_PATCHES = os.environ.get("LOD_ENABLE_LEGACY_RECOMP_PATCHES") == "1"

# =============================================================================
# Patch definitions — each patch targets a specific VRAM address or code pattern
# =============================================================================

PATCHES = []

def patch(description):
    """Decorator to register a legacy bring-up patch function."""
    def decorator(fn):
        if ENABLE_LEGACY_RECOMP_PATCHES:
            PATCHES.append((description, fn))
        return fn
    return decorator


def active_patch(description):
    """Decorator to register a current patch that should always be applied."""
    def decorator(fn):
        PATCHES.append((description, fn))
        return fn
    return decorator


def ensure_include(filepath, header):
    """Add #include if not already present."""
    with open(filepath) as f:
        content = f.read()
    if header in content:
        return content
    # Insert after the last #include line
    lines = content.split('\n')
    last_include = 0
    for i, line in enumerate(lines):
        if line.startswith('#include'):
            last_include = i
    lines.insert(last_include + 1, header)
    content = '\n'.join(lines)
    with open(filepath, 'w', newline='\n') as f:
        f.write(content)
    return content


def find_file_with_function(vram_hex):
    """Find which funcs_*.c file contains a RECOMP_FUNC at the given VRAM address."""
    pattern = f"// 0x{vram_hex}:"
    for fname in sorted(os.listdir(FUNC_DIR)):
        if fname.startswith("funcs_") and fname.endswith(".c"):
            path = os.path.join(FUNC_DIR, fname)
            with open(path) as f:
                content = f.read()
            if pattern in content:
                return path, content
    return None, None


# =============================================================================
# PATCH 1: Bootstrap overlay_system on first GSM dispatch
# Target: GameStateMgr_execute (VRAM 0x80000578)
# =============================================================================
@patch("Bootstrap overlay_system in GameStateMgr_execute (0x80000578)")
def patch_gsm_bootstrap():
    path, content = find_file_with_function("80000578")
    if not path:
        return False

    if "Bootstrap overlay_system" in content:
        return False  # already applied

    ensure_include(path, '#include <stdio.h>')

    # Re-read after include addition
    with open(path) as f:
        content = f.read()

    # Find the function by its first MIPS instruction comment
    marker = "    // 0x80000578: addiu       $sp, $sp, -0x28"
    if marker not in content:
        print(f"    WARNING: Cannot find insertion point for GSM bootstrap")
        return False

    patch_code = """    // --- PATCH: Bootstrap overlay_system on first GSM dispatch ---
    {
        uint32_t obj_phys = (uint32_t)ctx->r4 - 0x80000000;
        int32_t scene_arg = *(int32_t*)(rdram + obj_phys + 0x24);
        uint8_t state = rdram[(obj_phys + 0x09) ^ 3];
        fprintf(stderr, "[GSM] scene=%d state=%d\\n", scene_arg, state);
        static int bootstrap_done = 0;
        if (!bootstrap_done && state == 0) {
            bootstrap_done = 1;
            uint32_t ni_sys = *(uint32_t*)(rdram + (0x801CAC1C - 0x80000000));
            if (ni_sys == 0) {
                *(uint32_t*)(rdram + (0x801CADE4 - 0x80000000)) = 1;
                gpr saved_r4 = ctx->r4;
                gpr saved_r5 = ctx->r5;
                gpr saved_r31 = ctx->r31;
                overlay_system_create(rdram, ctx);
                ctx->r4 = saved_r4;
                ctx->r5 = saved_r5;
                ctx->r31 = saved_r31;
            }
        }
    }
    // --- END PATCH ---
"""

    # Insert before the first MIPS instruction
    content = content.replace(marker, patch_code + marker, 1)
    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


# =============================================================================
# PATCH 2: Bypass +0x4C early return in overlay state handler
# Target: map_ovl_00_func_802E406C (VRAM 0x802E406C)
# =============================================================================
@patch("Bypass +0x4C early return in map_ovl_00_func_802E406C")
def patch_ni_4c_bypass():
    # Must search for the specific function name, not VRAM (shared across overlays)
    path = None
    content = None
    for fname in sorted(os.listdir(FUNC_DIR)):
        if fname.startswith("funcs_") and fname.endswith(".c"):
            fpath = os.path.join(FUNC_DIR, fname)
            with open(fpath) as f:
                c = f.read()
            if "map_ovl_00_func_802E406C" in c:
                path, content = fpath, c
                break

    if not path:
        return False

    if "PATCH: When +0x4C is 0" in content:
        return False  # already applied

    # Find the beql pattern at 0x802E4084
    old = """    // 0x802E4084: beql        $t6, $zero, L_802E40CC
    if (ctx->r14 == 0) {
        // 0x802E4088: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_802E40CC;
    }
    goto skip_0;"""

    new = """    // 0x802E4084: beql        $t6, $zero, L_802E40CC
    // PATCH: When +0x4C is 0, bypass early return and fall through to timer path.
    if (ctx->r14 == 0) {
        ctx->r15 = MEM_H(ctx->r2, 0X8);
        goto skip_0;
    }
    goto skip_0;"""

    if old not in content:
        print(f"    WARNING: Cannot find 0x802E4084 pattern for +0x4C bypass")
        return False

    content = content.replace(old, new, 1)
    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


# =============================================================================
# PATCH 3: Hook poolObject_tick in NI system dispatcher
# Target: ni_system_dispatcher (VRAM 0x800113EC), after inner loop at 0x80011554
# =============================================================================
@patch("Hook poolObject_tick in ni_system_dispatcher (after 0x80011550)")
def patch_pool_tick_hook():
    path, content = find_file_with_function("800113EC")
    if not path:
        return False

    if "PATCH: Hook poolObject_tick" in content:
        return False  # already applied

    ensure_include(path, '#include <stdio.h>')

    # Re-read
    with open(path) as f:
        content = f.read()

    # Find insertion point: after the NI entry loop exits, before Phase 3
    marker = "    // 0x80011554: lui         $s1, 0x801D"
    if marker not in content:
        print(f"    WARNING: Cannot find 0x80011554 insertion point for poolObject_tick hook")
        return False

    patch_code = """    // --- PATCH: Hook poolObject_tick (NI per-frame update) ---
    {
        static int ni_update_started = 0;
        uint32_t ni_type = MEM_W(ctx->r4, 0X0);
        if (!ni_update_started && ni_type != 0)
            ni_update_started = 1;
        if (ni_update_started) {
            gpr saved_r4 = ctx->r4;
            gpr saved_r31 = ctx->r31;
            poolObject_tick(rdram, ctx);
            ctx->r4 = saved_r4;
            ctx->r31 = saved_r31;
        }
    }
    // --- END PATCH ---
"""

    content = content.replace(marker, patch_code + marker, 1)
    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True



# =============================================================================
# Current patch: translate intro asset 0x6C model display-list returns
# Target: ni_ovl_003_func_0F000070 (VRAM 0x0F000070)
# =============================================================================
@active_patch("Translate intro asset 0x6C model DL returns through active TLB")
def patch_intro_6c_model38_tlb():
    path, content = find_file_with_function("0F000070")
    if not path:
        return False

    if "PATCH: translate intro asset 0x6C model DL return" in content:
        return False

    ensure_include(path, '#include "lod_symbols.h"')
    with open(path) as f:
        content = f.read()

    patch_code = """#ifdef LOD_FIX_INTRO_6C_MODEL38_TLB
    // --- PATCH: translate intro asset 0x6C model DL return through active TLB ---
    {
        uint32_t translated_model_dl = lod_tlb_translate_virtual_to_physical_or_zero((uint32_t)ctx->r4, 1);
        if (translated_model_dl != 0) {
            ctx->r2 = translated_model_dl;
        }
    }
    // --- END PATCH ---
#endif
"""

    patched = 0
    for addr in [
        "0F000178", "0F000198", "0F0001BC", "0F0001F8", "0F000238",
        "0F000248", "0F000260", "0F000274", "0F000290", "0F0002A0",
        "0F0002B8", "0F0002CC", "0F0002FC",
    ]:
        marker = f"    // 0x{addr}: sw          $v0, 0x38("
        idx = content.find(marker)
        if idx < 0:
            print(f"    WARNING: Cannot find intro 0x6C store at 0x{addr}")
            continue
        content = content[:idx] + patch_code + content[idx:]
        patched += 1

    if patched == 0:
        return False

    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


# =============================================================================
# Current patch: skip invalid heap/data object callback targets
# Target: func_80058B84 (VRAM 0x80058B84), before object +0x44 dispatch
# =============================================================================
@active_patch("Skip invalid object callback")
def patch_invalid_object_callback():
    path, content = find_file_with_function("80058B84")
    if not path:
        return False

    if "PATCH: skip invalid object callback" in content:
        return False

    ensure_include(path, '#include <stdio.h>')
    with open(path) as f:
        content = f.read()

    support_code = """
#ifndef LOD_FIX_BAD_OBJECT_CALLBACK
#define LOD_FIX_BAD_OBJECT_CALLBACK 0
#endif

#if LOD_FIX_BAD_OBJECT_CALLBACK
extern int recomp_is_function_loaded(int32_t addr);
#endif

#if LOD_FIX_BAD_OBJECT_CALLBACK || LOD_ENABLE_BAD_OBJECT_CALLBACK_TRACE
extern uint32_t lod_current_map_overlay_rom(void);
#endif
"""
    if "#ifndef LOD_FIX_BAD_OBJECT_CALLBACK" not in content:
        lines = content.split('\n')
        last_include = 0
        for i, line in enumerate(lines):
            if line.startswith('#include'):
                last_include = i
        lines.insert(last_include + 1, support_code.strip('\n'))
        content = '\n'.join(lines)

    marker = """    // 0x80058C70: jalr        $v0
    // 0x80058C74: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
"""
    if marker not in content:
        print("    WARNING: Cannot find 0x80058C70 insertion point for object callback guard")
        return False

    patch_code = """#if LOD_FIX_BAD_OBJECT_CALLBACK
    // --- PATCH: skip invalid object callback ---
    {
        uint32_t cb = (uint32_t)ctx->r2;
        if (!recomp_is_function_loaded((int32_t)cb)) {
            MEM_W(ctx->r16, 0x04) = MEM_W(ctx->r16, 0x04) | 0x4;
            static int bad_object_callback_log_count = 0;
            if (bad_object_callback_log_count++ < 16) {
                fprintf(stderr,
                        "[OBJECT_CB_GUARD] disabled invalid object callback cb=0x%08X obj=0x%08X map_rom=0x%08X w00=0x%08X w04=0x%08X w44=0x%08X\\n",
                        cb,
                        (uint32_t)ctx->r16,
                        lod_current_map_overlay_rom(),
                        MEM_W(ctx->r16, 0x00),
                        MEM_W(ctx->r16, 0x04),
                        MEM_W(ctx->r16, 0x44));
            }
            goto L_80058C78;
        }
    }
    // --- END PATCH ---
#endif
"""

    content = content.replace(marker, marker + patch_code, 1)
    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


# =============================================================================
# Current patch: expand scene graph pool minimums
# Target: scene4_init / scene_init calls to sceneGraphPools_init (VRAM 0x80004CB0)
# =============================================================================
@active_patch("Expand scene graph pool minimums")
def patch_expand_scene_node_pool_minimum():
    path, content = find_file_with_function("800007B0")
    if not path:
        return False

    if "PATCH: expand scene-pool minimums" in content or "PATCH: expand scene-node pool minimum" in content:
        return False

    support_code = """
#ifndef LOD_FIX_EXPAND_SCENE_NODE_POOL
#define LOD_FIX_EXPAND_SCENE_NODE_POOL 0
#endif

#ifndef LOD_SCENE_NODE_POOL_MIN
#define LOD_SCENE_NODE_POOL_MIN 0x2A0
#endif

#ifndef LOD_SCENE_DATA_POOL_MIN
#define LOD_SCENE_DATA_POOL_MIN 0x480
#endif

#if LOD_FIX_EXPAND_SCENE_NODE_POOL
static void lod_scene_pools_apply_minimum(recomp_context* ctx) {
    if ((uint32_t)ctx->r4 < (uint32_t)LOD_SCENE_NODE_POOL_MIN) {
        ctx->r4 = ADD32(0, LOD_SCENE_NODE_POOL_MIN);
    }
    if ((uint32_t)ctx->r5 < (uint32_t)LOD_SCENE_DATA_POOL_MIN) {
        ctx->r5 = ADD32(0, LOD_SCENE_DATA_POOL_MIN);
    }
}
#endif
"""
    if "#ifndef LOD_FIX_EXPAND_SCENE_NODE_POOL" not in content:
        lines = content.split('\n')
        last_include = 0
        for i, line in enumerate(lines):
            if line.startswith('#include'):
                last_include = i
        lines.insert(last_include + 1, support_code.strip('\n'))
        content = '\n'.join(lines)

    call_markers = [
        (
            """    // 0x8000077C: jal         0x80004CB0
    // 0x80000780: nop

    sceneGraphPools_init(rdram, ctx);""",
            """    // 0x8000077C: jal         0x80004CB0
    // 0x80000780: nop

#if LOD_FIX_EXPAND_SCENE_NODE_POOL
    // --- PATCH: expand scene-pool minimums, scene4_init ---
    lod_scene_pools_apply_minimum(ctx);
    // --- END PATCH ---
#endif
    sceneGraphPools_init(rdram, ctx);""",
            "scene4_init scene pool call",
        ),
        (
            """    // 0x80000B98: jal         0x80004CB0
    // 0x80000B9C: nop

    sceneGraphPools_init(rdram, ctx);""",
            """    // 0x80000B98: jal         0x80004CB0
    // 0x80000B9C: nop

#if LOD_FIX_EXPAND_SCENE_NODE_POOL
    // --- PATCH: expand scene-pool minimums, scene_init ---
    lod_scene_pools_apply_minimum(ctx);
    // --- END PATCH ---
#endif
    sceneGraphPools_init(rdram, ctx);""",
            "scene_init scene pool call",
        ),
    ]

    for old, new, label in call_markers:
        if old not in content:
            print(f"    WARNING: Cannot find {label} for scene-pool expansion")
            return False
        content = content.replace(old, new, 1)

    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


# =============================================================================
# Current patch: Tower interactable/save/drop diagnostics
# Target: common interactable helpers around Player_getActorCurrentlyInteractingWith
# =============================================================================
@active_patch("Trace Tower interactable selection")
def patch_tower_interact_trace():
    path, content = find_file_with_function("80188730")
    if not path:
        return False

    if "PATCH: Tower interactable trace" in content:
        return False

    ensure_include(path, '#include <stdio.h>')
    with open(path) as f:
        content = f.read()

    support_code = """
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
        fprintf(stderr, "[INTERACT_TRACE] %s scan=%u obj=NULL result=%u map=0x%08X\\n",
                tag, scan_id, result, lod_current_map_overlay_rom());
        return;
    }
    if (!lod_interact_trace_can_read(obj, 0, 2)) {
        fprintf(stderr, "[INTERACT_TRACE] %s scan=%u obj=0x%08X unreadable result=%u map=0x%08X\\n",
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
            "[INTERACT_TRACE] %s scan=%u result=%u map=0x%08X obj=0x%08X id=0x%04X idx=%u flag3C=%u settings=0x%08X cfg=(%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X) actor_xyz=(%.2f,%.2f,%.2f) player=0x%08X player_xyz=(%.2f,%.2f,%.2f) delta=(%.2f,%.2f,%.2f) trigger=%u trigXZ=(%d,%d) yaw=0x%04X contpak=%d\\n",
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
        fprintf(stderr, "[INTERACT_STATE] %s map=0x%08X obj=0x%08X unreadable\\n",
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
            "[INTERACT_STATE] %s map=0x%08X obj=0x%08X id=0x%04X idx=%u kind=%s type=%u item=0x%04X flag3C=%u func0E=%d textbox60=0x%08X field04=%d field52=%d event50=0x%04X model24=0x%08X tb=(flags=0x%08X prev=%u opt=%u display=%u window=0x%08X lens=0x%08X) input=(held=0x%04X pressed=0x%04X joy=%d,%d)\\n",
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
        fprintf(stderr, "[INTERACT_CHOICE] %s map=0x%08X obj=0x%08X choice=%u unreadable\\n",
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
            "[INTERACT_CHOICE] %s map=0x%08X obj=0x%08X idx=%u kind=%s type=%u item=0x%04X choice=%u field52=%d textbox60=0x%08X tb=(flags=0x%08X prev=%u opt=%u display=%u window=0x%08X lens=0x%08X) input=(held=0x%04X pressed=0x%04X joy=%d,%d)\\n",
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
"""
    if "#ifndef LOD_ENABLE_INTERACT_TRACE" not in content:
        lines = content.split('\n')
        last_include = 0
        for i, line in enumerate(lines):
            if line.startswith('#include'):
                last_include = i
        lines.insert(last_include + 1, support_code.strip('\n'))
        content = '\n'.join(lines)

    scanner_entry = "    // 0x80188730: addiu       $sp, $sp, -0x50"
    scanner_entry_patch = """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, scanner locals ---
    const int lod_interact_trace_active = lod_interact_trace_enabled();
    const uint32_t lod_interact_scan_id = ++lod_interact_trace_scan_counter;
    int lod_interact_candidate_count = 0;
    int lod_interact_gate_hit_count = 0;
    // --- END PATCH ---
#endif
"""
    if scanner_entry not in content:
        print("    WARNING: Cannot find 0x80188730 scanner entry for interact trace")
        return False
    content = content.replace(scanner_entry, scanner_entry_patch + scanner_entry, 1)

    scanner_candidate = """    // 0x801887C0: lw          $a2, 0x40($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X40);
    after_0:
"""
    scanner_candidate_patch = """#if LOD_ENABLE_INTERACT_TRACE
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
"""
    if scanner_candidate not in content:
        print("    WARNING: Cannot find 0x801887C0 candidate point for interact trace")
        return False
    content = content.replace(scanner_candidate, scanner_candidate + scanner_candidate_patch, 1)

    scanner_return = """L_80188A54:
    // 0x80188A54: lw          $s0, 0x18($sp)
"""
    scanner_return_patch = """L_80188A54:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, scanner return ---
    if (lod_interact_trace_active && (ctx->r2 != 0 || lod_interact_candidate_count != 0 || ((lod_interact_scan_id & 0x1F) == 0))) {
        fprintf(stderr,
                "[INTERACT_TRACE] scan-return scan=%u map=0x%08X selected=0x%08X candidates=%d gate_hits=%d deferred=(text=0x%08X item=0x%08X special=0x%08X) fp=%u\\n",
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
"""
    if scanner_return not in content:
        print("    WARNING: Cannot find 0x80188A54 scanner return for interact trace")
        return False
    content = content.replace(scanner_return, scanner_return_patch, 1)

    gate_entry = "    // 0x80188B84: addiu       $sp, $sp, -0x38"
    gate_entry_patch = """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, gate locals ---
    const uint64_t lod_interact_gate_actor = ctx->r7;
    const uint32_t lod_interact_gate_id = ++lod_interact_trace_gate_counter;
    const int lod_interact_gate_trace_active = lod_interact_trace_enabled();
    // --- END PATCH ---
#endif
"""
    if gate_entry not in content:
        print("    WARNING: Cannot find 0x80188B84 gate entry for interact trace")
        return False
    content = content.replace(gate_entry, gate_entry_patch + gate_entry, 1)

    gate_return = """L_80188E6C:
    // 0x80188E6C: lw          $ra, 0x2C($sp)
"""
    gate_return_patch = """L_80188E6C:
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
"""
    if gate_return not in content:
        print("    WARNING: Cannot find 0x80188E6C gate return for interact trace")
        return False
    content = content.replace(gate_return, gate_return_patch, 1)

    flag_store = """    // 0x80188F7C: jr          $ra
    // 0x80188F80: sw          $t6, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r14;
"""
    flag_store_patch = """    // 0x80188F7C: jr          $ra
    // 0x80188F80: sw          $t6, 0x3C($a0)
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, flag write ---
    if (lod_interact_trace_enabled()) {
        lod_interact_trace_object(rdram, "set-flag", ctx->r4, (uint32_t)ctx->r14, 0);
    }
    // --- END PATCH ---
#endif
    MEM_W(0X3C, ctx->r4) = ctx->r14;
"""
    if flag_store not in content:
        print("    WARNING: Cannot find 0x80188F80 flag store for interact trace")
        return False
    content = content.replace(flag_store, flag_store_patch, 1)

    main_flag_check = """    // 0x80187A6C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80187A70: bne         $t7, $at, L_80187C64
"""
    main_flag_check_patch = """    // 0x80187A6C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, main sees interaction flag ---
    if (lod_interact_trace_enabled() && ctx->r15 == ctx->r1) {
        lod_interact_trace_state(rdram, "main-flagged", ctx->r16);
    }
    // --- END PATCH ---
#endif
    // 0x80187A70: bne         $t7, $at, L_80187C64
"""
    if main_flag_check not in content:
        print("    WARNING: Cannot find Interactable_Main flag check for interact trace")
        return False
    content = content.replace(main_flag_check, main_flag_check_patch, 1)

    state_entry_patches = [
        ("    // 0x80187C78: addiu       $sp, $sp, -0x20",
         "init-check-entry"),
        ("    // 0x80187E04: addiu       $sp, $sp, -0x28",
         "select-option-entry"),
        ("    // 0x80188264: addiu       $sp, $sp, -0x38",
         "stop-check-entry"),
        ("    // 0x8018842C: addiu       $sp, $sp, -0x20",
         "destroy-entry"),
        ("    // 0x801885A4: sw          $zero, 0x60($a0)",
         "stop-interaction"),
    ]
    for marker, tag in state_entry_patches:
        state_patch = f"""#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, {tag} ---
    lod_interact_trace_state(rdram, "{tag}", ctx->r4);
    // --- END PATCH ---
#endif
"""
        if marker not in content:
            print(f"    WARNING: Cannot find {tag} marker for interact trace")
            return False
        content = content.replace(marker, state_patch + marker, 1)

    white_jewel_choice_marker = """    after_0:
    // 0x80187E50: beq         $v0, $zero, L_80187E6C
"""
    white_jewel_choice_patch = """    after_0:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Tower interactable trace, white jewel choice result ---
    lod_interact_trace_choice(rdram, "white-jewel-choice", MEM_W(ctx->r29, 0x28), (uint32_t)ctx->r2);
    // --- END PATCH ---
#endif
    // 0x80187E50: beq         $v0, $zero, L_80187E6C
"""
    if white_jewel_choice_marker not in content:
        print("    WARNING: Cannot find white jewel choice marker for interact trace")
        return False
    content = content.replace(white_jewel_choice_marker, white_jewel_choice_patch, 1)

    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


@active_patch("Trace gameplay common textbox busy state")
def patch_common_textbox_busy_trace():
    path, content = find_file_with_function("80087070")
    if not path:
        return False

    if "PATCH: Gameplay common textbox busy trace" in content:
        return False

    ensure_include(path, '#include <stdio.h>')
    with open(path) as f:
        content = f.read()

    support_code = """
#ifndef LOD_ENABLE_INTERACT_TRACE
#define LOD_ENABLE_INTERACT_TRACE 0
#endif

#if LOD_ENABLE_INTERACT_TRACE
extern uint32_t lod_current_map_overlay_rom(void);

static uint64_t lod_common_textbox_guest_addr(uint32_t addr) {
    return (uint64_t)(int64_t)(int32_t)addr;
}

static int lod_common_textbox_rdram_offset(uint64_t guest, int32_t offset, uint32_t size, uint32_t xor_mask, uint64_t* out_offset) {
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

static uint32_t lod_common_textbox_read_w(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint32_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_common_textbox_rdram_offset(lod_common_textbox_guest_addr(guest_addr), offset, 4, 0, &rdram_offset)) {
        return default_value;
    }
    return (uint32_t)(*(int32_t*)(rdram + rdram_offset));
}

static uint16_t lod_common_textbox_read_hu(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint16_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_common_textbox_rdram_offset(lod_common_textbox_guest_addr(guest_addr), offset, 2, 2, &rdram_offset)) {
        return default_value;
    }
    return *(uint16_t*)(rdram + rdram_offset);
}

static uint8_t lod_common_textbox_read_bu(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint8_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_common_textbox_rdram_offset(lod_common_textbox_guest_addr(guest_addr), offset, 1, 3, &rdram_offset)) {
        return default_value;
    }
    return *(uint8_t*)(rdram + rdram_offset);
}

static void lod_common_textbox_trace(uint8_t* rdram, const char* tag, uint32_t item, uint32_t textbox, uint32_t busy_result) {
    const uint32_t flags = lod_common_textbox_read_w(rdram, textbox, 0x00, 0xFFFFFFFFu);
    const uint32_t ptr38 = lod_common_textbox_read_w(rdram, textbox, 0x38, 0);
    const uint32_t ptr38_flags20 = lod_common_textbox_read_w(rdram, ptr38, 0x20, 0xFFFFFFFFu);
    const uint32_t ptr38_word24 = lod_common_textbox_read_w(rdram, ptr38, 0x24, 0xFFFFFFFFu);
    const uint32_t ptr38_busy26 = lod_common_textbox_read_bu(rdram, ptr38, 0x26, 0xFFu);
    const uint32_t ptr38_byte27 = lod_common_textbox_read_bu(rdram, ptr38, 0x27, 0xFFu);
    const uint32_t mgr = lod_common_textbox_read_w(rdram, 0x801CAC0Cu, 0, 0);
    const uint32_t mgr_func0e = lod_common_textbox_read_hu(rdram, mgr, 0x0E, 0xFFFFu);
    const uint32_t mgr_state = lod_common_textbox_read_w(rdram, mgr, 0x48, 0xFFFFFFFFu);
    const uint32_t mgr_menu = lod_common_textbox_read_w(rdram, mgr, 0x50, 0xFFFFFFFFu);
    fprintf(stderr,
            "[COMMON_TEXTBOX] %s map=0x%08X item=0x%04X ret_busy=%u mgr=0x%08X mgr_func0e=%u mgr_state=0x%08X mgr_menu=0x%08X textbox=0x%08X flags=0x%08X bits(open=%u close=%u open_lens=%u close_lens=%u parsed=%u hide=%u) ptr38=0x%08X ptr38_flags20=0x%08X ptr38_word24=0x%08X ptr38_busy26=%u ptr38_byte27=%u display=%u option=%u prev=%u window=0x%08X lens=0x%08X\\n",
            tag,
            lod_current_map_overlay_rom(),
            item,
            busy_result,
            mgr,
            mgr_func0e,
            mgr_state,
            mgr_menu,
            textbox,
            flags,
            (flags & 0x08000000u) != 0,
            (flags & 0x04000000u) != 0,
            (flags & 0x20000000u) != 0,
            (flags & 0x10000000u) != 0,
            (flags & 0x40000000u) != 0,
            (flags & 0x80000000u) != 0,
            ptr38,
            ptr38_flags20,
            ptr38_word24,
            ptr38_busy26,
            ptr38_byte27,
            lod_common_textbox_read_bu(rdram, textbox, 0x3A, 0xFFu),
            lod_common_textbox_read_bu(rdram, textbox, 0x2E, 0xFFu),
            lod_common_textbox_read_bu(rdram, textbox, 0x2D, 0xFFu),
            lod_common_textbox_read_w(rdram, textbox, 0x54, 0xFFFFFFFFu),
            lod_common_textbox_read_w(rdram, textbox, 0x5C, 0));
}
#endif
"""
    if "#ifndef LOD_ENABLE_INTERACT_TRACE" not in content:
        lines = content.split('\n')
        last_include = 0
        for i, line in enumerate(lines):
            if line.startswith('#include'):
                last_include = i
        lines.insert(last_include + 1, support_code.strip('\n'))
        content = '\n'.join(lines)

    busy_check_marker = """    after_1:
    // 0x80087090: beq         $v0, $zero, L_800870A8
"""
    busy_check_patch = """    after_1:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Gameplay common textbox busy trace, item busy check ---
    lod_common_textbox_trace(rdram, "addItem-after-busycheck", (uint32_t)MEM_W(0X30, ctx->r29), (uint32_t)ctx->r16, (uint32_t)ctx->r2);
    // --- END PATCH ---
#endif
    // 0x80087090: beq         $v0, $zero, L_800870A8
"""
    if busy_check_marker not in content:
        print("    WARNING: Cannot find common textbox addItem busy check")
        return False
    content = content.replace(busy_check_marker, busy_check_patch, 1)

    busy_return_marker = """    // 0x80087098: jal         0x80084004
    // 0x8008709C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80084004(rdram, ctx);
"""
    busy_return_patch = """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Gameplay common textbox busy trace, item returns null ---
    lod_common_textbox_trace(rdram, "addItem-busy-return-null", (uint32_t)MEM_W(0X30, ctx->r29), (uint32_t)ctx->r16, 1);
    // --- END PATCH ---
#endif
    // 0x80087098: jal         0x80084004
    // 0x8008709C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80084004(rdram, ctx);
"""
    if busy_return_marker not in content:
        print("    WARNING: Cannot find common textbox addItem busy return")
        return False
    content = content.replace(busy_return_marker, busy_return_patch, 1)

    success_marker = """L_8008732C:
    // 0x8008732C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80087330:
"""
    success_patch = """L_8008732C:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: Gameplay common textbox busy trace, item success ---
    lod_common_textbox_trace(rdram, "addItem-success", (uint32_t)MEM_W(0X30, ctx->r29), (uint32_t)ctx->r16, 0);
    // --- END PATCH ---
#endif
    // 0x8008732C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80087330:
"""
    if success_marker not in content:
        print("    WARNING: Cannot find common textbox addItem success return")
        return False
    content = content.replace(success_marker, success_patch, 1)

    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


@active_patch("Trace MFDS common textbox state machine")
def patch_mfds_common_textbox_state_trace():
    path, content = find_file_with_function("8007E510")
    if not path:
        return False

    if "PATCH: MFDS common textbox state trace" in content:
        return False

    ensure_include(path, '#include <stdio.h>')
    with open(path) as f:
        content = f.read()

    support_code = """
#ifndef LOD_ENABLE_INTERACT_TRACE
#define LOD_ENABLE_INTERACT_TRACE 0
#endif

#if LOD_ENABLE_INTERACT_TRACE
extern uint32_t lod_current_map_overlay_rom(void);

static uint64_t lod_mfds_trace_guest_addr(uint32_t addr) {
    return (uint64_t)(int64_t)(int32_t)addr;
}

static int lod_mfds_trace_rdram_offset(uint64_t guest, int32_t offset, uint32_t size, uint32_t xor_mask, uint64_t* out_offset) {
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

static uint32_t lod_mfds_trace_read_w(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint32_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_mfds_trace_rdram_offset(lod_mfds_trace_guest_addr(guest_addr), offset, 4, 0, &rdram_offset)) {
        return default_value;
    }
    return (uint32_t)(*(int32_t*)(rdram + rdram_offset));
}

static uint16_t lod_mfds_trace_read_hu(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint16_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_mfds_trace_rdram_offset(lod_mfds_trace_guest_addr(guest_addr), offset, 2, 2, &rdram_offset)) {
        return default_value;
    }
    return *(uint16_t*)(rdram + rdram_offset);
}

static uint8_t lod_mfds_trace_read_bu(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint8_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_mfds_trace_rdram_offset(lod_mfds_trace_guest_addr(guest_addr), offset, 1, 3, &rdram_offset)) {
        return default_value;
    }
    return *(uint8_t*)(rdram + rdram_offset);
}

static uint32_t lod_mfds_trace_common_state(uint8_t* rdram) {
    const uint32_t mgr = lod_mfds_trace_read_w(rdram, 0x801CAC0Cu, 0, 0);
    return lod_mfds_trace_read_w(rdram, mgr, 0x5C, 0);
}

static int lod_mfds_trace_should_log(uint8_t* rdram, uint32_t obj) {
    if (lod_current_map_overlay_rom() != 0x007D9790 || obj == 0) {
        return 0;
    }
    const uint32_t work = lod_mfds_trace_read_w(rdram, obj, 0x38, 0);
    const uint32_t common = lod_mfds_trace_common_state(rdram);
    const uint32_t common_work = lod_mfds_trace_read_w(rdram, common, 0x38, 0);
    return work != 0 && common_work != 0 && work == common_work;
}

static void lod_mfds_trace_state(uint8_t* rdram, const char* tag, uint32_t obj) {
    if (!lod_mfds_trace_should_log(rdram, obj)) {
        return;
    }
    const uint32_t common = lod_mfds_trace_common_state(rdram);
    const uint32_t work = lod_mfds_trace_read_w(rdram, obj, 0x38, 0);
    const uint32_t common_work = lod_mfds_trace_read_w(rdram, common, 0x38, 0);
    const uint32_t child70 = lod_mfds_trace_read_w(rdram, work, 0x70, 0);
    const uint32_t child70_obj24 = lod_mfds_trace_read_w(rdram, child70, 0x24, 0xFFFFFFFFu);
    const uint32_t child70_state40 = lod_mfds_trace_read_w(rdram, child70, 0x40, 0xFFFFFFFFu);
    fprintf(stderr,
            "[MFDS_TRACE] %s map=0x%08X obj=0x%08X obj_func=0x%08X obj_func0E=%u common=0x%08X common_flags=0x%08X work=0x%08X common_work=0x%08X flags20=0x%08X word24=0x%08X busy26=%u byte27=%u child70=0x%08X child70_obj24=0x%08X child70_state40=0x%08X state0=%u state2=%u state4=%u\\n",
            tag,
            lod_current_map_overlay_rom(),
            obj,
            lod_mfds_trace_read_w(rdram, obj, 0x10, 0),
            lod_mfds_trace_read_hu(rdram, obj, 0x0E, 0xFFFFu),
            common,
            lod_mfds_trace_read_w(rdram, common, 0x00, 0xFFFFFFFFu),
            work,
            common_work,
            lod_mfds_trace_read_w(rdram, work, 0x20, 0xFFFFFFFFu),
            lod_mfds_trace_read_w(rdram, work, 0x24, 0xFFFFFFFFu),
            lod_mfds_trace_read_bu(rdram, work, 0x26, 0xFFu),
            lod_mfds_trace_read_bu(rdram, work, 0x27, 0xFFu),
            child70,
            child70_obj24,
            child70_state40,
            lod_mfds_trace_read_hu(rdram, work, 0x00, 0xFFFFu),
            lod_mfds_trace_read_hu(rdram, work, 0x02, 0xFFFFu),
            lod_mfds_trace_read_hu(rdram, work, 0x04, 0xFFFFu));
}
#endif
"""
    if "lod_mfds_trace_state" not in content:
        lines = content.split('\n')
        last_include = 0
        for i, line in enumerate(lines):
            if line.startswith('#include'):
                last_include = i
        lines.insert(last_include + 1, support_code.strip('\n'))
        content = '\n'.join(lines)

    replacements = [
        (
            "    // 0x8007E510: addiu       $sp, $sp, -0x20",
            """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E510 entry ---
    lod_mfds_trace_state(rdram, "E510-entry", (uint32_t)ctx->r4);
    // --- END PATCH ---
#endif
    // 0x8007E510: addiu       $sp, $sp, -0x20""",
            1,
            "E510 entry",
        ),
        (
            """    // 0x8007E644: sb          $t6, 0x26($v1)
    MEM_B(0X26, ctx->r3) = ctx->r14;""",
            """    // 0x8007E644: sb          $t6, 0x26($v1)
    MEM_B(0X26, ctx->r3) = ctx->r14;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E510 set busy 2 ---
    lod_mfds_trace_state(rdram, "E510-set2", (uint32_t)ctx->r7);
    // --- END PATCH ---
#endif""",
            0,
            "E510 set2",
        ),
        (
            "    // 0x8007E680: addiu       $sp, $sp, -0x38",
            """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E680 entry ---
    lod_mfds_trace_state(rdram, "E680-entry", (uint32_t)ctx->r4);
    // --- END PATCH ---
#endif
    // 0x8007E680: addiu       $sp, $sp, -0x38""",
            1,
            "E680 entry",
        ),
        (
            """    // 0x8007E74C: sw          $zero, 0x20($a3)
    MEM_W(0X20, ctx->r7) = 0;
    // 0x8007E750: sb          $zero, 0x26($a3)
    MEM_B(0X26, ctx->r7) = 0;""",
            """    // 0x8007E74C: sw          $zero, 0x20($a3)
    MEM_W(0X20, ctx->r7) = 0;
    // 0x8007E750: sb          $zero, 0x26($a3)
    MEM_B(0X26, ctx->r7) = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E680 cleared ---
    lod_mfds_trace_state(rdram, "E680-clear0", (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "E680 clear0",
        ),
        (
            """    // 0x8007E8B0: sb          $t5, 0x26($a3)
    MEM_B(0X26, ctx->r7) = ctx->r13;""",
            """    // 0x8007E8B0: sb          $t5, 0x26($a3)
    MEM_B(0X26, ctx->r7) = ctx->r13;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E680 set busy 3 ---
    lod_mfds_trace_state(rdram, "E680-set3", (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "E680 set3",
        ),
        (
            """L_8007E8E4:
    // 0x8007E8E4: lw          $s0, 0x18($sp)""",
            """L_8007E8E4:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E680 exit ---
    lod_mfds_trace_state(rdram, "E680-exit", (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif
    // 0x8007E8E4: lw          $s0, 0x18($sp)""",
            1,
            "E680 exit",
        ),
        (
            "    // 0x8007E8F4: addiu       $sp, $sp, -0x38",
            """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E8F4 entry ---
    lod_mfds_trace_state(rdram, "E8F4-entry", (uint32_t)ctx->r4);
    // --- END PATCH ---
#endif
    // 0x8007E8F4: addiu       $sp, $sp, -0x38""",
            1,
            "E8F4 entry",
        ),
        (
            """    // 0x8007E998: sb          $t1, 0x26($s0)
    MEM_B(0X26, ctx->r16) = ctx->r9;""",
            """    // 0x8007E998: sb          $t1, 0x26($s0)
    MEM_B(0X26, ctx->r16) = ctx->r9;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E8F4 set busy 3 ---
    lod_mfds_trace_state(rdram, "E8F4-set3", (uint32_t)ctx->r17);
    // --- END PATCH ---
#endif""",
            1,
            "E8F4 set3",
        ),
        (
            """    // 0x8007E9CC: sw          $zero, 0x20($s0)
    MEM_W(0X20, ctx->r16) = 0;
    // 0x8007E9D0: sb          $zero, 0x26($s0)
    MEM_B(0X26, ctx->r16) = 0;""",
            """    // 0x8007E9CC: sw          $zero, 0x20($s0)
    MEM_W(0X20, ctx->r16) = 0;
    // 0x8007E9D0: sb          $zero, 0x26($s0)
    MEM_B(0X26, ctx->r16) = 0;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E8F4 cleared ---
    lod_mfds_trace_state(rdram, "E8F4-clear0", (uint32_t)ctx->r17);
    // --- END PATCH ---
#endif""",
            1,
            "E8F4 clear0",
        ),
        (
            """L_8007ECE0:
    // 0x8007ECE0: lw          $s0, 0x18($sp)""",
            """L_8007ECE0:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS common textbox state trace, E8F4 exit ---
    lod_mfds_trace_state(rdram, "E8F4-exit", (uint32_t)ctx->r17);
    // --- END PATCH ---
#endif
    // 0x8007ECE0: lw          $s0, 0x18($sp)""",
            1,
            "E8F4 exit",
        ),
    ]

    for old, new, count, label in replacements:
        if old not in content:
            print(f"    WARNING: Cannot find {label} marker for MFDS trace")
            return False
        content = content.replace(old, new, count if count != 0 else content.count(old))

    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True


@active_patch("Trace MFDS child window object 0x186")
def patch_mfds_child_window_trace():
    path, content = find_file_with_function("800875E0")
    if not path:
        return False

    if "PATCH: MFDS child window object trace" in content:
        return False

    ensure_include(path, '#include <stdio.h>')
    with open(path) as f:
        content = f.read()

    support_code = """
#ifndef LOD_ENABLE_INTERACT_TRACE
#define LOD_ENABLE_INTERACT_TRACE 0
#endif

#if LOD_ENABLE_INTERACT_TRACE
extern uint32_t lod_current_map_overlay_rom(void);

static uint64_t lod_mfds_child_trace_guest_addr(uint32_t addr) {
    return (uint64_t)(int64_t)(int32_t)addr;
}

static int lod_mfds_child_trace_rdram_offset(uint64_t guest, int32_t offset, uint32_t size, uint32_t xor_mask, uint64_t* out_offset) {
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

static uint32_t lod_mfds_child_trace_read_w(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint32_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_mfds_child_trace_rdram_offset(lod_mfds_child_trace_guest_addr(guest_addr), offset, 4, 0, &rdram_offset)) {
        return default_value;
    }
    return (uint32_t)(*(int32_t*)(rdram + rdram_offset));
}

static uint16_t lod_mfds_child_trace_read_hu(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint16_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_mfds_child_trace_rdram_offset(lod_mfds_child_trace_guest_addr(guest_addr), offset, 2, 2, &rdram_offset)) {
        return default_value;
    }
    return *(uint16_t*)(rdram + rdram_offset);
}

static int16_t lod_mfds_child_trace_read_h(uint8_t* rdram, uint32_t guest_addr, int32_t offset, int16_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_mfds_child_trace_rdram_offset(lod_mfds_child_trace_guest_addr(guest_addr), offset, 2, 2, &rdram_offset)) {
        return default_value;
    }
    return *(int16_t*)(rdram + rdram_offset);
}

static uint8_t lod_mfds_child_trace_read_bu(uint8_t* rdram, uint32_t guest_addr, int32_t offset, uint8_t default_value) {
    uint64_t rdram_offset = 0;
    if (guest_addr == 0 || !lod_mfds_child_trace_rdram_offset(lod_mfds_child_trace_guest_addr(guest_addr), offset, 1, 3, &rdram_offset)) {
        return default_value;
    }
    return *(uint8_t*)(rdram + rdram_offset);
}

static uint32_t lod_mfds_child_trace_common_state(uint8_t* rdram) {
    const uint32_t mgr = lod_mfds_child_trace_read_w(rdram, 0x801CAC0Cu, 0, 0);
    return lod_mfds_child_trace_read_w(rdram, mgr, 0x5C, 0);
}

static uint32_t lod_mfds_child_trace_common_window(uint8_t* rdram) {
    const uint32_t common = lod_mfds_child_trace_common_state(rdram);
    const uint32_t work = lod_mfds_child_trace_read_w(rdram, common, 0x38, 0);
    return lod_mfds_child_trace_read_w(rdram, work, 0x70, 0);
}

static int lod_mfds_child_trace_should_log(uint8_t* rdram, uint32_t obj, const char* tag) {
    static uint32_t last_obj = 0;
    static uint32_t log_count = 0;
    (void)tag;
    if (lod_current_map_overlay_rom() != 0x007D9790 || obj == 0) {
        return 0;
    }
    if (lod_mfds_child_trace_read_hu(rdram, obj, 0x00, 0xFFFFu) != 0x0186u) {
        return 0;
    }
    if (obj != last_obj) {
        last_obj = obj;
        log_count = 0;
    }
    if (log_count < 160 || ((log_count & 0x3Fu) == 0)) {
        log_count++;
        return 1;
    }
    log_count++;
    return 0;
}

static void lod_mfds_child_trace_state(uint8_t* rdram, const char* tag, uint32_t obj, uint32_t extra) {
    if (!lod_mfds_child_trace_should_log(rdram, obj, tag)) {
        return;
    }
    const uint32_t common_window = lod_mfds_child_trace_common_window(rdram);
    const uint32_t child60 = lod_mfds_child_trace_read_w(rdram, obj, 0x60, 0);
    const uint32_t child60_data34 = lod_mfds_child_trace_read_w(rdram, child60, 0x34, 0xFFFFFFFFu);
    fprintf(stderr,
            "[MFDS_CHILD] %s map=0x%08X obj=0x%08X extra=0x%08X common_window=0x%08X id=0x%04X flags02=0x%04X timer04=%d f06=%d cf=(%u,%u,%u,%u,%u,%u) func_info=%d destroy=0x%08X parent=0x%08X next=0x%08X child=0x%08X figs=(0x%08X,0x%08X,0x%08X,0x%08X) data=(0x%08X,0x%08X,%u,%u,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X) child60=0x%08X child60_34=0x%08X\\n",
            tag,
            lod_current_map_overlay_rom(),
            obj,
            extra,
            common_window,
            lod_mfds_child_trace_read_hu(rdram, obj, 0x00, 0xFFFFu),
            lod_mfds_child_trace_read_hu(rdram, obj, 0x02, 0xFFFFu),
            lod_mfds_child_trace_read_h(rdram, obj, 0x04, -32768),
            lod_mfds_child_trace_read_h(rdram, obj, 0x06, -32768),
            lod_mfds_child_trace_read_bu(rdram, obj, 0x08, 0xFFu),
            lod_mfds_child_trace_read_bu(rdram, obj, 0x09, 0xFFu),
            lod_mfds_child_trace_read_bu(rdram, obj, 0x0A, 0xFFu),
            lod_mfds_child_trace_read_bu(rdram, obj, 0x0B, 0xFFu),
            lod_mfds_child_trace_read_bu(rdram, obj, 0x0C, 0xFFu),
            lod_mfds_child_trace_read_bu(rdram, obj, 0x0D, 0xFFu),
            lod_mfds_child_trace_read_h(rdram, obj, 0x0E, -32768),
            lod_mfds_child_trace_read_w(rdram, obj, 0x10, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x14, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x18, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x1C, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x24, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x28, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x2C, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x30, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x34, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x38, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x3C, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x40, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x44, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x48, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x4C, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x50, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x54, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x58, 0),
            lod_mfds_child_trace_read_w(rdram, obj, 0x5C, 0),
            child60,
            child60_data34);
}

static void lod_mfds_child_trace_scene_pool(uint8_t* rdram, uint32_t* total, uint32_t* used, uint32_t* free_count, uint32_t* first_free) {
    const uint32_t base = lod_mfds_child_trace_read_w(rdram, 0x800C1550u, 0, 0);
    const uint32_t end = lod_mfds_child_trace_read_w(rdram, 0x800C1554u, 0, 0);
    *total = 0;
    *used = 0;
    *free_count = 0;
    *first_free = 0;
    if (base == 0 || end == 0 || end <= base) {
        return;
    }
    for (uint32_t entry = base; entry + 0xB8u <= end && *total < 4096u; entry += 0xB8u) {
        (*total)++;
        if (lod_mfds_child_trace_read_hu(rdram, entry, 0, 0xFFFFu) == 0) {
            if (*first_free == 0) {
                *first_free = entry;
            }
            (*free_count)++;
        } else {
            (*used)++;
        }
    }
}

static uint32_t lod_mfds_child_trace_popcount32(uint32_t value) {
    uint32_t count = 0;
    while (value != 0) {
        count += value & 1u;
        value >>= 1;
    }
    return count;
}

static void lod_mfds_child_trace_data_pool(uint8_t* rdram, uint32_t* total, uint32_t* used, uint32_t* free_count, uint32_t* first_free) {
    const uint32_t base = lod_mfds_child_trace_read_w(rdram, 0x800C1568u, 0, 0);
    const uint32_t end = lod_mfds_child_trace_read_w(rdram, 0x800C156Cu, 0, 0);
    const uint32_t configured_count = lod_mfds_child_trace_read_w(rdram, 0x800C1570u, 0, 0);
    const uint32_t bitmap = lod_mfds_child_trace_read_w(rdram, 0x800C1564u, 0, 0);
    uint32_t count = configured_count;
    *total = 0;
    *used = 0;
    *free_count = 0;
    *first_free = 0;
    if (base == 0 || end == 0 || end <= base || bitmap == 0) {
        return;
    }
    if (count == 0 || base + count * 0x40u > end) {
        count = (end - base) / 0x40u;
    }
    if (count > 8192u) {
        count = 8192u;
    }
    *total = count;
    for (uint32_t word_index = 0; word_index < (count + 31u) / 32u; word_index++) {
        uint32_t word = lod_mfds_child_trace_read_w(rdram, bitmap + word_index * 4u, 0, 0);
        if ((word_index + 1u) * 32u > count) {
            const uint32_t valid_bits = count - word_index * 32u;
            if (valid_bits < 32u) {
                word &= (1u << valid_bits) - 1u;
            }
        }
        *used += lod_mfds_child_trace_popcount32(word);
        if (*first_free == 0 && word != 0xFFFFFFFFu) {
            for (uint32_t bit = 0; bit < 32u; bit++) {
                const uint32_t index = word_index * 32u + bit;
                if (index >= count) {
                    break;
                }
                if ((word & (1u << bit)) == 0) {
                    *first_free = base + index * 0x40u;
                    break;
                }
            }
        }
    }
    if (*used <= count) {
        *free_count = count - *used;
    }
}

static void lod_mfds_child_trace_layout_node(uint8_t* rdram, const char* tag, uint32_t obj, uint32_t node, uint32_t data) {
    if (!lod_mfds_child_trace_should_log(rdram, obj, tag)) {
        return;
    }
    uint32_t pool_total = 0;
    uint32_t pool_used = 0;
    uint32_t pool_free = 0;
    uint32_t pool_first_free = 0;
    uint32_t data_total = 0;
    uint32_t data_used = 0;
    uint32_t data_free = 0;
    uint32_t data_first_free = 0;
    lod_mfds_child_trace_scene_pool(rdram, &pool_total, &pool_used, &pool_free, &pool_first_free);
    lod_mfds_child_trace_data_pool(rdram, &data_total, &data_used, &data_free, &data_first_free);
    fprintf(stderr,
            "[MFDS_CHILD_NODE] %s map=0x%08X obj=0x%08X node=0x%08X data=0x%08X obj24=0x%08X data_nodes=(0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X) node_fields=(h0=0x%04X h2=0x%04X w8=0x%08X w10=0x%08X w14=0x%08X w3C=0x%08X w40=0x%08X w44=0x%08X w48=0x%08X w4C=0x%08X) scene_pool=(base=0x%08X end=0x%08X cursor=0x%08X total=%u used=%u free=%u first_free=0x%08X) data_pool=(base=0x%08X end=0x%08X cursor=0x%08X bitmap=0x%08X total=%u used=%u free=%u first_free=0x%08X)\\n",
            tag,
            lod_current_map_overlay_rom(),
            obj,
            node,
            data,
            lod_mfds_child_trace_read_w(rdram, obj, 0x24, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x00, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x04, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x08, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x0C, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x10, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x14, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x18, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x1C, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x20, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x24, 0),
            lod_mfds_child_trace_read_w(rdram, data, 0x28, 0),
            lod_mfds_child_trace_read_hu(rdram, node, 0x00, 0xFFFFu),
            lod_mfds_child_trace_read_hu(rdram, node, 0x02, 0xFFFFu),
            lod_mfds_child_trace_read_w(rdram, node, 0x08, 0),
            lod_mfds_child_trace_read_w(rdram, node, 0x10, 0),
            lod_mfds_child_trace_read_w(rdram, node, 0x14, 0),
            lod_mfds_child_trace_read_w(rdram, node, 0x3C, 0),
            lod_mfds_child_trace_read_w(rdram, node, 0x40, 0),
            lod_mfds_child_trace_read_w(rdram, node, 0x44, 0),
            lod_mfds_child_trace_read_w(rdram, node, 0x48, 0),
            lod_mfds_child_trace_read_w(rdram, node, 0x4C, 0),
            lod_mfds_child_trace_read_w(rdram, 0x800C1550u, 0, 0),
            lod_mfds_child_trace_read_w(rdram, 0x800C1554u, 0, 0),
            lod_mfds_child_trace_read_w(rdram, 0x800C155Cu, 0, 0),
            pool_total,
            pool_used,
            pool_free,
            pool_first_free,
            lod_mfds_child_trace_read_w(rdram, 0x800C1568u, 0, 0),
            lod_mfds_child_trace_read_w(rdram, 0x800C156Cu, 0, 0),
            lod_mfds_child_trace_read_w(rdram, 0x800C1574u, 0, 0),
            lod_mfds_child_trace_read_w(rdram, 0x800C1564u, 0, 0),
            data_total,
            data_used,
            data_free,
            data_first_free);
}
#endif
"""
    if "lod_mfds_child_trace_state" not in content:
        lines = content.split('\n')
        last_include = 0
        for i, line in enumerate(lines):
            if line.startswith('#include'):
                last_include = i
        lines.insert(last_include + 1, support_code.strip('\n'))
        content = '\n'.join(lines)

    replacements = [
        (
            "    // 0x800875E0: addiu       $sp, $sp, -0x18",
            """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, dispatcher entry ---
    lod_mfds_child_trace_state(rdram, "875E0-entry", (uint32_t)ctx->r4, 0);
    // --- END PATCH ---
#endif
    // 0x800875E0: addiu       $sp, $sp, -0x18""",
            1,
            "875E0 entry",
        ),
        (
            """    // 0x80087628: jalr        $t9
    // 0x8008762C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);""",
            """    // 0x80087628: jalr        $t9
    // 0x8008762C: nop

#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, dispatcher call ---
    lod_mfds_child_trace_state(rdram, "875E0-dispatch", (uint32_t)ctx->r4, (uint32_t)ctx->r25);
    // --- END PATCH ---
#endif
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);""",
            1,
            "875E0 dispatch",
        ),
        (
            """    after_0:
    // 0x80087630: lw          $a0, 0x18($sp)""",
            """    after_0:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, dispatcher after callee ---
    lod_mfds_child_trace_state(rdram, "875E0-after", (uint32_t)MEM_W(ctx->r29, 0X18), 0);
    // --- END PATCH ---
#endif
    // 0x80087630: lw          $a0, 0x18($sp)""",
            1,
            "875E0 after",
        ),
        (
            "    // 0x80087650: addiu       $sp, $sp, -0x28",
            """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, init entry ---
    lod_mfds_child_trace_state(rdram, "87650-entry", (uint32_t)ctx->r4, 0);
    // --- END PATCH ---
#endif
    // 0x80087650: addiu       $sp, $sp, -0x28""",
            1,
            "87650 entry",
        ),
        (
            """    after_0:
    // 0x80087678: lw          $t6, 0x3C($s0)""",
            """    after_0:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, after object data alloc ---
    lod_mfds_child_trace_state(rdram, "87650-after-02808", (uint32_t)ctx->r16, (uint32_t)ctx->r2);
    // --- END PATCH ---
#endif
    // 0x80087678: lw          $t6, 0x3C($s0)""",
            1,
            "87650 after 02808",
        ),
        (
            """    // 0x800876D4: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;""",
            """    // 0x800876D4: sw          $v0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, child 0x188 created ---
    lod_mfds_child_trace_state(rdram, "87650-after-create188", (uint32_t)ctx->r16, (uint32_t)ctx->r2);
    // --- END PATCH ---
#endif""",
            1,
            "87650 after create 188",
        ),
        (
            """    after_2:
    // 0x80087700: lw          $ra, 0x1C($sp)""",
            """    after_2:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, init exit ---
    lod_mfds_child_trace_state(rdram, "87650-exit", (uint32_t)ctx->r16, 0);
    // --- END PATCH ---
#endif
    // 0x80087700: lw          $ra, 0x1C($sp)""",
            1,
            "87650 exit",
        ),
        (
            "    // 0x80087714: addiu       $sp, $sp, -0x38",
            """#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, layout entry ---
    lod_mfds_child_trace_state(rdram, "87714-entry", (uint32_t)ctx->r4, 0);
    // --- END PATCH ---
#endif
    // 0x80087714: addiu       $sp, $sp, -0x38""",
            1,
            "87714 entry",
        ),
        (
            """    // 0x80087A38: sw          $v0, 0x24($s2)
    MEM_W(0X24, ctx->r18) = ctx->r2;""",
            """    // 0x80087A38: sw          $v0, 0x24($s2)
    MEM_W(0X24, ctx->r18) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, main layout node ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-main", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node main",
        ),
        (
            """    // 0x80087AA8: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;""",
            """    // 0x80087AA8: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +04 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data04", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data04",
        ),
        (
            """    // 0x80087B0C: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;""",
            """    // 0x80087B0C: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +08 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data08", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data08",
        ),
        (
            """    // 0x80087B70: sw          $v0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r2;""",
            """    // 0x80087B70: sw          $v0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +0C ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data0C", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data0C",
        ),
        (
            """    // 0x80087BDC: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;""",
            """    // 0x80087BDC: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +10 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data10", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data10",
        ),
        (
            """    // 0x80087CB8: sw          $v0, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->r2;""",
            """    // 0x80087CB8: sw          $v0, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +14 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data14", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data14",
        ),
        (
            """    // 0x80087D0C: sw          $v0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r2;""",
            """    // 0x80087D0C: sw          $v0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +18 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data18", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data18",
        ),
        (
            """    // 0x80087D60: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;""",
            """    // 0x80087D60: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +1C ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data1C", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data1C",
        ),
        (
            """    // 0x80087DB4: sw          $v0, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->r2;""",
            """    // 0x80087DB4: sw          $v0, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +20 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data20", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data20",
        ),
        (
            """    // 0x80087E08: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;""",
            """    // 0x80087E08: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +24 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data24", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data24",
        ),
        (
            """    // 0x80087EAC: sw          $v0, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->r2;""",
            """    // 0x80087EAC: sw          $v0, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->r2;
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, data node +28 ---
    lod_mfds_child_trace_layout_node(rdram, "87714-node-data28", (uint32_t)ctx->r18, (uint32_t)ctx->r2, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif""",
            1,
            "87714 node data28",
        ),
        (
            """    // 0x80087F74: jalr        $t9
    // 0x80087F78: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);""",
            """    // 0x80087F74: jalr        $t9
    // 0x80087F78: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, before next-function advance ---
    lod_mfds_child_trace_state(rdram, "87714-before-nextfunc", (uint32_t)ctx->r18, (uint32_t)ctx->r16);
    // --- END PATCH ---
#endif
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);""",
            1,
            "87714 before nextfunc",
        ),
        (
            """    after_15:
    // 0x80087F7C: lw          $ra, 0x34($sp)""",
            """    after_15:
#if LOD_ENABLE_INTERACT_TRACE
    // --- PATCH: MFDS child window object trace, layout exit ---
    lod_mfds_child_trace_state(rdram, "87714-exit", (uint32_t)ctx->r18, 0);
    // --- END PATCH ---
#endif
    // 0x80087F7C: lw          $ra, 0x34($sp)""",
            1,
            "87714 exit",
        ),
    ]

    for old, new, count, label in replacements:
        if old not in content:
            print(f"    WARNING: Cannot find {label} marker for MFDS child trace")
            return False
        content = content.replace(old, new, count)

    with open(path, 'w', newline='\n') as f:
        f.write(content)
    return True



# =============================================================================
# Main
# =============================================================================
def main():
    if not ENABLE_LEGACY_RECOMP_PATCHES:
        print("  Legacy hand-written RecompiledFuncs patches disabled.")
        print("  Set LOD_ENABLE_LEGACY_RECOMP_PATCHES=1 to reapply old bring-up patches.")

    applied = 0
    for desc, fn in PATCHES:
        if fn():
            print(f"  Applied: {desc}")
            applied += 1

    if applied == 0:
        print("  All patches already applied.")
    else:
        print(f"  Applied {applied} patch(es).")
    return 0


if __name__ == '__main__':
    sys.exit(main())
