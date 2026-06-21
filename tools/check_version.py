#!/usr/bin/env python3
from __future__ import annotations

import os
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
VERSION_RE = re.compile(r"^[0-9]+\.[0-9]+\.[0-9]+$")
HARDCODED_VERSION_RE = re.compile(r"\b[0-9]+\.[0-9]+\.[0-9]+\b")


def fail(message: str) -> int:
    print(f"version check failed: {message}", file=sys.stderr)
    return 1


def main() -> int:
    version_path = ROOT / "VERSION"
    try:
        version = version_path.read_text(encoding="utf-8").strip()
    except FileNotFoundError:
        return fail("VERSION file is missing")

    if not VERSION_RE.fullmatch(version):
        return fail(f"VERSION must contain MAJOR.MINOR.PATCH, got {version!r}")

    ref_type = os.environ.get("GITHUB_REF_TYPE", "")
    ref_name = os.environ.get("GITHUB_REF_NAME", "")
    if ref_type == "tag":
        tag_version = ref_name[1:] if ref_name.startswith("v") else ref_name
        if tag_version != version:
            return fail(f"tag {ref_name!r} does not match VERSION {version!r}")

    # These files must get the version from generated configuration, not literals.
    checked_files = [
        ROOT / "src/main/main.cpp",
        ROOT / "src/main/lod_recomp.rc.in",
        ROOT / "src/ui/lod_ui_overlay.cpp",
    ]
    for path in checked_files:
        text = path.read_text(encoding="utf-8")
        matches = sorted(set(HARDCODED_VERSION_RE.findall(text)))
        if matches:
            rel = path.relative_to(ROOT)
            return fail(f"{rel} contains hard-coded version literal(s): {', '.join(matches)}")

    print(f"VERSION ok: {version}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
