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
