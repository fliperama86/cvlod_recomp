#!/usr/bin/env python3
"""Copy the UI assets required by release packages."""

from __future__ import annotations

import argparse
import shutil
from pathlib import Path


TOP_LEVEL_PATTERNS = (
    "*.rml",
    "*.rcss",
    "*.ttf",
    "*.otf",
    "*.png",
    "*.svg",
)

DIRECTORIES = (
    "icons",
    "promptfont",
    "config_menu",
)


def copy_file(src: Path, dst: Path) -> None:
    dst.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst)


def copy_tree(src: Path, dst: Path) -> None:
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(src, dst)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("destination", type=Path)
    parser.add_argument(
        "--source",
        type=Path,
        default=Path(__file__).resolve().parent.parent / "assets",
        help="asset source directory, defaults to repo assets/",
    )
    args = parser.parse_args()

    source = args.source.resolve()
    destination = args.destination.resolve()
    if not source.is_dir():
        raise SystemExit(f"asset source does not exist: {source}")

    destination.mkdir(parents=True, exist_ok=True)

    copied = 0
    for pattern in TOP_LEVEL_PATTERNS:
        for src in sorted(source.glob(pattern)):
            if src.is_file():
                copy_file(src, destination / src.name)
                copied += 1

    for directory in DIRECTORIES:
        src_dir = source / directory
        if src_dir.is_dir():
            copy_tree(src_dir, destination / directory)
            copied += 1

    if copied == 0:
        raise SystemExit(f"no assets copied from {source}")

    print(f"Copied {copied} release asset entries to {destination}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
