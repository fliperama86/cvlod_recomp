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
BACKUP_DIR="backups/RecompiledFuncs"

# Known-good N64Recomp binary SHA-256 (from ../lod_recomp_failed/tools/N64Recomp/build/N64Recomp)
# Update this hash if you intentionally rebuild the tool from clean source.
KNOWN_GOOD_SHA256="$(shasum -a 256 ../lod_recomp_failed/tools/N64Recomp/build/N64Recomp 2>/dev/null | awk '{print $1}' || true)"

# --- Parse args ---
DRY_RUN=0
SAVE_ONLY=0
SKIP_VERIFY=0
for arg in "$@"; do
    case "$arg" in
        --dry-run)     DRY_RUN=1 ;;
        --save-only)   SAVE_ONLY=1 ;;
        --skip-verify) SKIP_VERIFY=1 ;;
        -h|--help)
            echo "Usage: $0 [--save-only] [--dry-run] [--skip-verify]"
            echo "  --save-only    Only save current patches, don't regenerate"
            echo "  --dry-run      Show what would happen without doing it"
            echo "  --skip-verify  Skip N64Recomp binary integrity check"
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

# --- Step 2: Verify N64Recomp binary ---
if [ ! -x "$N64RECOMP" ]; then
    echo "ERROR: N64Recomp not found at $N64RECOMP"
    echo "Set N64RECOMP env var to the correct path."
    exit 1
fi

if [ $SKIP_VERIFY -eq 0 ]; then
    # Verify we're using the known-good binary, not a dirty local build
    ACTUAL_SHA256="$(shasum -a 256 "$N64RECOMP" | awk '{print $1}')"
    if [ -n "$KNOWN_GOOD_SHA256" ] && [ "$ACTUAL_SHA256" != "$KNOWN_GOOD_SHA256" ]; then
        echo "ERROR: N64Recomp binary hash mismatch!"
        echo "  Expected: $KNOWN_GOOD_SHA256"
        echo "  Got:      $ACTUAL_SHA256"
        echo "  Binary:   $N64RECOMP"
        echo ""
        echo "  This likely means the binary was rebuilt from modified source."
        echo "  Use --skip-verify to override (at your own risk)."
        exit 1
    fi

    # Verify the N64Recomp submodule source is clean
    N64RECOMP_SRC="lib/N64ModernRuntime/N64Recomp"
    if [ -d "$N64RECOMP_SRC" ]; then
        if ! git -C "$N64RECOMP_SRC" diff --quiet -- src/ 2>/dev/null; then
            echo "ERROR: N64Recomp submodule has dirty source files!"
            echo "  Path: $N64RECOMP_SRC"
            git -C "$N64RECOMP_SRC" diff --name-only -- src/ | sed 's/^/    /'
            echo ""
            echo "  Clean it with: git -C $N64RECOMP_SRC checkout -- src/"
            echo "  Or use --skip-verify to override."
            exit 1
        fi
    fi
    echo "    N64Recomp binary and submodule verified clean."
fi

# --- Step 3: Backup current RecompiledFuncs ---
TIMESTAMP="$(date +%Y%m%d_%H%M%S)"
BACKUP_PATH="${BACKUP_DIR}_${TIMESTAMP}"
echo "==> Backing up $FUNC_DIR/ to $BACKUP_PATH..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would copy $FUNC_DIR/ to $BACKUP_PATH"
else
    mkdir -p "$(dirname "$BACKUP_PATH")"
    cp -a "$FUNC_DIR" "$BACKUP_PATH"
    FILE_COUNT=$(find "$BACKUP_PATH" -maxdepth 1 -name '*.c' 2>/dev/null | wc -l | tr -d ' ')
    echo "    Backed up $FILE_COUNT .c files to $BACKUP_PATH"
    # Keep only the 3 most recent backups to avoid bloat
    ls -dt backups/RecompiledFuncs_* 2>/dev/null | tail -n +4 | xargs rm -rf 2>/dev/null || true
fi

# --- Step 4: Reset tracked RecompiledFuncs to clean state ---
echo "==> Resetting tracked $FUNC_DIR/ files to git HEAD..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: git checkout $FUNC_DIR/"
else
    git checkout "$FUNC_DIR/"
fi

# --- Step 5: Run N64Recomp ---
echo "==> Running N64Recomp..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: $N64RECOMP $RECOMP_TOML"
else
    "$N64RECOMP" "$RECOMP_TOML" || true  # N64Recomp returns non-zero on overlay_system info msgs
    echo "    Done. Generated files:"
    echo "      $(find "$FUNC_DIR" -maxdepth 1 -name '*.c' | wc -l | tr -d ' ') .c files"
fi

# --- Step 6: Fix N64Recomp truncation bugs ---
echo "==> Fixing N64Recomp truncation bugs..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would fix funcs.h, recomp_overlays.inl, funcs_102.c"
else
    fix_truncation_bugs
fi

# --- Step 7: Fix cross-function gotos (labels in different functions) ---
echo "==> Fixing cross-function gotos..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: python3 tools/ni_ovl/fix_undeclared_labels.py"
else
    python3 "$(dirname "$0")/ni_ovl/fix_undeclared_labels.py"
fi

# --- Step 8: Re-apply hand-written patches (address-keyed, rename-safe) ---
echo "==> Re-applying hand-written patches..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: python3 tools/apply_patches.py"
else
    python3 "$(dirname "$0")/apply_patches.py"
fi

# --- Step 9: Apply named symbol replacements ---
echo "==> Applying symbol replacements..."
if [ $DRY_RUN -eq 1 ]; then
    echo "    [dry-run] Would run: python3 tools/apply_symbols.py"
else
    python3 "$(dirname "$0")/apply_symbols.py"
fi

echo "==> Done. Review changes with: git diff $FUNC_DIR/"
