#!/usr/bin/env python3
"""Applies the vendored submodule patches in patches/ to the checked-out submodules.

Some fixes the Android port depends on live inside submodules we do not control:

  cvlod_recomp -> lib/rt64 (fliperama86/rt64) -> src/contrib/plume (renderbag/plume)

They cannot be committed here, and a PR to this repository cannot carry them, so they are
vendored as diffs and re-applied after a fresh `git submodule update --init --recursive`.

Without patches/plume.patch the Android build crashes on "Start game" on Adreno GPUs:
vkAllocateDescriptorSets returns VK_ERROR_OUT_OF_POOL_MEMORY because the descriptor pool is
sized to the requested variable count rather than the layout's declared maximum.

Idempotent: already-applied patches are detected and skipped, so it is safe to run on every
build. Exits non-zero only if a patch is neither applicable nor already applied, which means
the submodule moved and the patch needs regenerating.

Usage:
    python tools/apply_submodule_patches.py [--check]

    --check   report status without modifying anything (exit 1 if any patch is missing)
"""

import argparse
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent

# patch file -> submodule directory it applies to, relative to the repo root
PATCHES = [
    ("patches/rt64.patch", "lib/rt64"),
    ("patches/plume.patch", "lib/rt64/src/contrib/plume"),
    ("patches/n64modernruntime.patch", "lib/N64ModernRuntime"),
]


def git(target: Path, *args: str) -> subprocess.CompletedProcess:
    return subprocess.run(
        ["git", "-C", str(target), *args],
        capture_output=True,
        text=True,
    )


def applies_cleanly(target: Path, patch: Path) -> bool:
    return git(target, "apply", "--check", str(patch)).returncode == 0


def already_applied(target: Path, patch: Path) -> bool:
    # If the reverse patch applies, the forward patch is already in the tree.
    return git(target, "apply", "--reverse", "--check", str(patch)).returncode == 0


def process(patch_rel: str, submodule_rel: str, check_only: bool) -> bool:
    patch = REPO_ROOT / patch_rel
    target = REPO_ROOT / submodule_rel

    if not patch.is_file():
        print(f"  MISSING  {patch_rel} (patch file not found)")
        return False

    if not target.is_dir() or not (target / ".git").exists():
        print(f"  SKIP     {submodule_rel} not checked out; run "
              f"'git submodule update --init --recursive' first")
        return False

    if already_applied(target, patch):
        print(f"  OK       {submodule_rel} already patched")
        return True

    if not applies_cleanly(target, patch):
        print(f"  FAILED   {patch_rel} does not apply to {submodule_rel}")
        print(f"           The submodule probably moved. Regenerate with:")
        print(f"             git -C {submodule_rel} diff > {patch_rel}")
        return False

    if check_only:
        print(f"  NEEDED   {submodule_rel} is missing {patch_rel}")
        return False

    result = git(target, "apply", str(patch))
    if result.returncode != 0:
        print(f"  FAILED   applying {patch_rel}: {result.stderr.strip()}")
        return False

    print(f"  APPLIED  {patch_rel} -> {submodule_rel}")
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true",
                        help="report status without modifying anything")
    args = parser.parse_args()

    print("Vendored submodule patches:")
    ok = all(process(p, s, args.check) for p, s in PATCHES)

    if not ok:
        print("\nOne or more patches are not applied. The Android build will crash on "
              "'Start game' on Adreno GPUs without patches/plume.patch.")
        return 1

    print("All vendored submodule patches are in place.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
