#!/usr/bin/env bash
#
# regen_recomp.sh — Re-run N64Recomp and preserve hand-written patches
#
# Usage:
#   ./tools/regen_recomp.sh              # full cycle: save patches, regen, fix, reapply
#   ./tools/regen_recomp.sh --save-only  # just save current patches
#   ./tools/regen_recomp.sh --dry-run    # show what would happen
#
set -euo pipefail
cd "$(git -C "$(dirname "$0")/.." rev-parse --show-toplevel)"

N64RECOMP="${N64RECOMP:-../lod_recomp_failed/tools/N64Recomp/build/N64Recomp}"
RECOMP_TOML="recomp.toml"
PATCH_DIR="patches"
PATCH_FILE="$PATCH_DIR/recompiled_funcs.patch"
FUNC_DIR="RecompiledFuncs"

# --- Parse args ---
DRY_RUN=0
SAVE_ONLY=0
for arg in "$@"; do
    case "$arg" in
        --dry-run)   DRY_RUN=1 ;;
        --save-only) SAVE_ONLY=1 ;;
        -h|--help)
            echo "Usage: $0 [--save-only] [--dry-run]"
            echo "  --save-only  Only save current patches, don't regenerate"
            echo "  --dry-run    Show what would happen without doing it"
            exit 0 ;;
        *) echo "Unknown arg: $arg"; exit 1 ;;
    esac
done

# =============================================================================
# Fix N64Recomp truncation bugs — delegated to Python for reliability
# =============================================================================
fix_truncation_bugs() {
    python3 "$(dirname "$0")/fix_n64recomp_truncation.py"
}

# =============================================================================
# Main flow
# =============================================================================

# --- Step 1: Save current patches (git-tracked files only) ---
echo "==> Checking for patches in $FUNC_DIR/..."
if git diff --quiet "$FUNC_DIR/"; then
    echo "    No local modifications in tracked $FUNC_DIR/ files."
    HAS_PATCHES=0
else
    mkdir -p "$PATCH_DIR"
    MODIFIED=$(git diff --name-only "$FUNC_DIR/" | wc -l | tr -d ' ')
    echo "    Found modifications in $MODIFIED file(s):"
    git diff --name-only "$FUNC_DIR/" | sed 's/^/      /'

    if [ $DRY_RUN -eq 1 ]; then
        echo "    [dry-run] Would save patch to $PATCH_FILE"
    else
        git diff "$FUNC_DIR/" > "$PATCH_FILE"
        echo "    Saved patch to $PATCH_FILE ($(wc -l < "$PATCH_FILE" | tr -d ' ') lines)"
    fi
    HAS_PATCHES=1
fi

[ $SAVE_ONLY -eq 1 ] && { echo "==> Done (save-only)."; exit 0; }

# --- Step 2: Verify N64Recomp exists ---
if [ ! -x "$N64RECOMP" ]; then
    echo "ERROR: N64Recomp not found at $N64RECOMP"
    echo "Set N64RECOMP env var to the correct path."
    exit 1
fi

# --- Step 3: Reset tracked RecompiledFuncs to clean state ---
echo "==> Resetting tracked $FUNC_DIR/ files to git HEAD..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: git checkout $FUNC_DIR/"
else
    git checkout "$FUNC_DIR/"
fi

# --- Step 4: Run N64Recomp ---
echo "==> Running N64Recomp..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: $N64RECOMP $RECOMP_TOML"
else
    "$N64RECOMP" "$RECOMP_TOML" || true  # N64Recomp returns non-zero on overlay_system info msgs
    echo "    Done. Generated files:"
    ls "$FUNC_DIR/"*.c | wc -l | xargs -I{} echo "      {} .c files"
fi

# --- Step 5: Fix N64Recomp truncation bugs ---
echo "==> Fixing N64Recomp truncation bugs..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would fix funcs.h, recomp_overlays.inl, funcs_102.c"
else
    fix_truncation_bugs
fi

# --- Step 6: Re-apply hand-written patches (address-keyed, rename-safe) ---
echo "==> Re-applying hand-written patches..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: python3 tools/apply_patches.py"
else
    python3 "$(dirname "$0")/apply_patches.py"
fi

# --- Step 7: Apply named symbol replacements ---
echo "==> Applying symbol replacements..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: python3 tools/apply_symbols.py"
else
    python3 "$(dirname "$0")/apply_symbols.py"
fi

echo "==> Done. Review changes with: git diff $FUNC_DIR/"
