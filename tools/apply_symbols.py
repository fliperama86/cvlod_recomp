#!/usr/bin/env python3
"""
apply_symbols.py — Apply named symbol replacements to regenerated RecompiledFuncs.

Runs AFTER apply_patches.py in the regen pipeline. Replaces hex literals with
named constants from include/lod_symbols.h, scoped to specific functions
(identified by VRAM address) to avoid ambiguity.

Design:
  - Each replacement is keyed by VRAM address (rename-safe, like patches)
  - Replacements are string substitutions within a specific function's body
  - The lod_symbols.h include is added to every funcs_*.c file
  - Idempotent: safe to re-run (checks for existing include/replacements)
"""

import os
import re
import sys

FUNC_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "RecompiledFuncs")
SYMBOLS_INCLUDE = '#include "lod_symbols.h"'

# =============================================================================
# Symbol replacement definitions
# Each entry: (vram_hex, [(old_literal, new_name), ...])
# Replacements are applied in order within the function body.
# =============================================================================

FUNCTION_SYMBOLS = []

def symbols(vram_hex, description):
    """Decorator to register symbol replacements for a function."""
    def decorator(fn):
        FUNCTION_SYMBOLS.append((vram_hex, description, fn))
        return fn
    return decorator


# --- Global replacements (applied to ALL funcs files) ---
# These are unambiguous constants that mean the same thing everywhere.
GLOBAL_REPLACEMENTS = [
    # Template IDs — only appear as immediate args to object_createAndSetChild
    ("ADD32(0, 0X1AB)", "ADD32(0, TMPL_OVERLAY_SYSTEM)"),
    ("ADD32(0, 0X1AB);", "ADD32(0, TMPL_OVERLAY_SYSTEM);"),
]


# --- Per-function replacements ---

@symbols("80000460", "gamestate_create: scene ID arg and slot copy")
def sym_gamestate_create():
    return [
        # The scene descriptor table access: s2-0x48 = init func
        ("MEM_W(ctx->r18, -0X48)", "MEM_W(ctx->r18, GSS_CFG_INIT_FUNC)"),
        # s2-0x40 = slot template start (64 bytes = 16 slots)
        ("ADD32(ctx->r18, -0X40)", "ADD32(ctx->r18, GSS_CFG_SLOTS)"),
        # Copy 0x40 bytes = 16 GSS slots
        ("ADD32(0, 0X40);", "ADD32(0, 0X40); /* NUM_GSS_SLOTS * 4 */"),
        # obj+0x10 = handler function
        ("MEM_W(0X10, ctx->r13) = ctx->r12", "MEM_W(OBJ_DESTROY, ctx->r13) = ctx->r12"),
        # obj+0x24 = scene_arg
        ("MEM_W(0X24, ctx->r15) = ctx->r14", "MEM_W(OBJ_FIGURES_0, ctx->r15) = ctx->r14 /* scene_arg */"),
    ]

@symbols("80000578", "GameStateMgr_execute: scene dispatch")
def sym_gsm_execute():
    return [
        # Read scene_arg from obj+0x24
        ("MEM_W(ctx->r4, 0X24)", "MEM_W(ctx->r4, OBJ_FIGURES_0) /* scene_arg */"),
        # Read state from obj+0x09
        ("MEM_BU(ctx->r16, 0X9)", "MEM_BU(ctx->r16, OBJ_STATE)"),
    ]

@symbols("8000067C", "scene4_init: heaps, objects, display")
def sym_scene4_init():
    return [
        # heap_init calls: a0=heap_id
        ("ADD32(0, 0X12C)", "ADD32(0, 0X12C) /* 300 objects */"),
    ]

@symbols("8001BA78", "overlay_system_create: creates NI overlay system object")
def sym_overlay_system_create():
    return [
        # Guard check: sys+0x2B24
        ("MEM_W(ctx->r14, -0X521C)", "MEM_W(ctx->r14, SYS_IMM_NI_SYSTEM_PTR)"),
        # object_createAndSetChild(0, TMPL_OVERLAY_SYSTEM)
        # (handled by global replacement for 0x1AB)
        # Store result at sys+0x2924
        ("MEM_W(0X2924, ctx->r3)", "MEM_W(0X2924, ctx->r3) /* sys.overlay_system_ptr */"),
    ]

@symbols("800113EC", "ni_system_dispatcher: NI file entry loop + phase 3 clear")
def sym_ni_system_dispatcher():
    return [
        # Loop bound: 0x2B0 entries
        ("SIGNED(ctx->r23) < 0X2B0", "SIGNED(ctx->r23) < 0X2B0 /* NI_MAX_ENTRIES */"),
    ]

@symbols("802E406C", "map_ovl_00: NI descriptor +0x4C check")
def sym_ovl00_ni_check():
    return [
        # ni_desc+0x4C = sub-figure bitmask
        ("MEM_W(ctx->r2, 0X4C)", "MEM_W(ctx->r2, NI_DESC_SUBFIG_BITMASK)"),
        # ni_desc+0x8 = frame counter
        ("MEM_H(ctx->r2, 0X8)", "MEM_H(ctx->r2, NI_DESC_FRAME_CTR)"),
    ]


# =============================================================================
# Engine
# =============================================================================

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


def find_function_body(content, vram_hex):
    """Find the start and end of the function containing the given VRAM address."""
    marker = f"// 0x{vram_hex}:"
    pos = content.find(marker)
    if pos == -1:
        return None, None, None

    # Find RECOMP_FUNC before this position
    func_start = content.rfind("RECOMP_FUNC ", 0, pos)
    if func_start == -1:
        return None, None, None

    # Find the next RECOMP_FUNC after
    func_end = content.find("\nRECOMP_FUNC ", pos)
    if func_end == -1:
        func_end = len(content)

    return func_start, func_end, content[func_start:func_end]


def ensure_symbols_include(filepath):
    """Add #include "lod_symbols.h" if not already present."""
    with open(filepath) as f:
        content = f.read()
    if SYMBOLS_INCLUDE in content:
        return content, False

    # Insert after #include "funcs.h" (or after last #include)
    lines = content.split('\n')
    insert_after = 0
    for i, line in enumerate(lines):
        if line.startswith('#include'):
            insert_after = i
    lines.insert(insert_after + 1, SYMBOLS_INCLUDE)
    content = '\n'.join(lines)
    with open(filepath, 'w') as f:
        f.write(content)
    return content, True


def apply_global_replacements(filepath):
    """Apply unambiguous global replacements to a file."""
    with open(filepath) as f:
        content = f.read()
    changed = False
    for old, new in GLOBAL_REPLACEMENTS:
        if old in content and new not in content:
            content = content.replace(old, new)
            changed = True
    if changed:
        with open(filepath, 'w') as f:
            f.write(content)
    return changed


def apply_function_symbols(vram_hex, description, get_replacements):
    """Apply symbol replacements within a specific function."""
    path, content = find_file_with_function(vram_hex)
    if not path:
        return False

    func_start, func_end, func_body = find_function_body(content, vram_hex)
    if func_body is None:
        return False

    new_body = func_body
    applied = 0
    for old, new in get_replacements():
        if old in new_body and new not in new_body:
            new_body = new_body.replace(old, new, 1)  # Only first occurrence in function
            applied += 1

    if applied > 0:
        content = content[:func_start] + new_body + content[func_end:]
        with open(path, 'w') as f:
            f.write(content)
        return True
    return False


def main():
    # Step 1: Add #include "lod_symbols.h" to all funcs_*.c files
    include_count = 0
    for fname in sorted(os.listdir(FUNC_DIR)):
        if fname.startswith("funcs_") and fname.endswith(".c"):
            path = os.path.join(FUNC_DIR, fname)
            _, added = ensure_symbols_include(path)
            if added:
                include_count += 1

    if include_count > 0:
        print(f"  Added {SYMBOLS_INCLUDE} to {include_count} file(s).")
    else:
        print(f"  {SYMBOLS_INCLUDE} already present in all files.")

    # Step 2: Apply global replacements
    global_count = 0
    for fname in sorted(os.listdir(FUNC_DIR)):
        if fname.startswith("funcs_") and fname.endswith(".c"):
            path = os.path.join(FUNC_DIR, fname)
            if apply_global_replacements(path):
                global_count += 1
    if global_count > 0:
        print(f"  Applied global symbol replacements in {global_count} file(s).")

    # Step 3: Apply per-function replacements
    func_count = 0
    for vram_hex, description, get_replacements in FUNCTION_SYMBOLS:
        if apply_function_symbols(vram_hex, description, get_replacements):
            print(f"  Symbols: {description}")
            func_count += 1

    if func_count == 0 and global_count == 0:
        print("  All symbol replacements already applied.")
    else:
        print(f"  Applied {func_count} function-scoped replacement(s).")

    return 0


if __name__ == '__main__':
    sys.exit(main())
