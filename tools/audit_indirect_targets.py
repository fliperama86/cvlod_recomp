#!/usr/bin/env python3
"""
Audit NI overlay raw blobs for likely indirect function targets that are not
declared as function starts in castlevania2.syms.toml.

Why this exists:
  N64Recomp only emits callable functions for known function boundaries. Small
  state-table/callback targets can be missed when they are referenced only by
  data tables. One concrete bug was NI44 target 0x0F004030, a 0x14-byte helper
  used by the Map76 werewolf boss state machine.

The audit is intentionally conservative:
  * It scans each raw NI overlay blob independently.
  * By default, it treats 4-byte aligned words inside that overlay's executable
    VRAM range as candidate indirect targets.
  * With --scan-full-raw, it scans the whole raw blob for data/rodata pointer
    tables, while still only accepting targets into executable text.
  * It reports candidates that are not declared function starts in the same
    syms.toml section.

Usage:
  python3 tools/audit_indirect_targets.py
  python3 tools/audit_indirect_targets.py --fail-on-missing
  python3 tools/audit_indirect_targets.py --section 44
  python3 tools/audit_indirect_targets.py --scan-full-raw --section 7 --target 0x0F010218
"""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import re
import struct
import sys
from typing import Any

try:
    import tomllib
except ModuleNotFoundError:  # pragma: no cover - Python < 3.11 fallback
    tomllib = None  # type: ignore[assignment]


PROJECT_DIR = Path(__file__).resolve().parents[1]
DEFAULT_SYMS = PROJECT_DIR / "castlevania2.syms.toml"
DEFAULT_RAW_DIR = PROJECT_DIR / "tools" / "ni_ovl" / "out" / "raw"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Audit NI overlay raw blobs for missing indirect function targets.",
    )
    parser.add_argument("--syms", type=Path, default=DEFAULT_SYMS,
                        help="Path to castlevania2.syms.toml")
    parser.add_argument("--raw-dir", type=Path, default=DEFAULT_RAW_DIR,
                        help="Directory containing ni_ovl_XXX.bin raw blobs")
    parser.add_argument("--section", type=int, action="append",
                        help="Only audit a specific NI overlay pair/index; may be repeated")
    parser.add_argument("--scan-full-raw", action="store_true",
                        help=("Scan the full raw blob for table references. Targets are still "
                              "restricted to the executable text range."))
    parser.add_argument("--target", type=lambda s: int(s, 0), action="append",
                        help="Only report a specific target VRAM address; may be repeated")
    parser.add_argument("--limit", type=int, default=200,
                        help="Maximum missing target rows to print")
    parser.add_argument("--fail-on-missing", action="store_true",
                        help="Exit non-zero if any missing target is found")
    return parser.parse_args()


def load_toml(path: Path) -> dict[str, Any]:
    if tomllib is None:
        raise RuntimeError("Python 3.11+ is required for tomllib TOML parsing")
    with path.open("rb") as f:
        return tomllib.load(f)


def iter_ni_sections(syms: dict[str, Any], raw_dir: Path, only: set[int] | None):
    for section in syms.get("section", []):
        name = section.get("name", "")
        match = re.fullmatch(r"\.\.ni_ovl_(\d+)", name)
        if not match:
            continue

        pair = int(match.group(1))
        if only is not None and pair not in only:
            continue

        raw_path = raw_dir / f"ni_ovl_{pair:03d}.bin"
        if not raw_path.exists():
            yield {
                "pair": pair,
                "section": section,
                "raw_path": raw_path,
                "missing_raw": True,
            }
            continue

        yield {
            "pair": pair,
            "section": section,
            "raw_path": raw_path,
            "missing_raw": False,
        }


def words_at(data: bytes, offset: int, count: int = 4) -> list[int]:
    words: list[int] = []
    for i in range(count):
        word_off = offset + i * 4
        if 0 <= word_off <= len(data) - 4:
            words.append(struct.unpack_from(">I", data, word_off)[0])
    return words


def scan_section(pair: int, section: dict[str, Any], raw_path: Path,
                 scan_full_raw: bool = False,
                 only_targets: set[int] | None = None) -> list[dict[str, Any]]:
    vram = int(section["vram"])
    size = int(section["size"])
    functions = section.get("functions", [])
    known_starts = {int(func["vram"]) for func in functions}
    known_ranges = [
        (int(func["vram"]), int(func["vram"]) + int(func["size"]), func["name"])
        for func in functions
    ]

    data = raw_path.read_bytes()
    text_size = min(size, len(data))
    text_end = vram + text_size
    scan_size = len(data) if scan_full_raw else text_size

    refs: dict[int, list[int]] = {}
    for raw_off in range(0, max(0, scan_size - 3), 4):
        value = struct.unpack_from(">I", data, raw_off)[0]
        if value % 4 != 0:
            continue
        if vram <= value < text_end:
            if only_targets is not None and value not in only_targets:
                continue
            refs.setdefault(value, []).append(vram + raw_off)

    missing: list[dict[str, Any]] = []
    for target, sites in sorted(refs.items()):
        if target in known_starts:
            continue

        target_off = target - vram
        target_words = words_at(data, target_off)
        if not target_words or target_words[0] in (0x00000000, 0xFFFFFFFF):
            continue

        containing = None
        for start, end, name in known_ranges:
            if start <= target < end:
                containing = name
                break

        missing.append({
            "pair": pair,
            "target": target,
            "refs": len(sites),
            "sites": sites[:8],
            "site_regions": [
                "text" if vram <= site < vram + text_size else "data"
                for site in sites[:8]
            ],
            "words": target_words,
            "containing": containing,
        })

    return missing


def main() -> int:
    args = parse_args()
    only = set(args.section) if args.section else None
    only_targets = set(args.target) if args.target else None

    syms = load_toml(args.syms)
    sections = list(iter_ni_sections(syms, args.raw_dir, only))

    if not sections:
        print("[audit_indirect_targets] No NI sections matched.")
        return 1 if args.fail_on_missing else 0

    missing_raw = [entry for entry in sections if entry["missing_raw"]]
    scanned = 0
    missing: list[dict[str, Any]] = []

    for entry in sections:
        if entry["missing_raw"]:
            continue
        scanned += 1
        missing.extend(scan_section(entry["pair"], entry["section"], entry["raw_path"],
                                    scan_full_raw=args.scan_full_raw,
                                    only_targets=only_targets))

    if missing_raw:
        print(f"[audit_indirect_targets] Warning: {len(missing_raw)} raw blob(s) missing; skipped.")
        for entry in missing_raw[:10]:
            print(f"  ni_ovl_{entry['pair']:03d}: {entry['raw_path']}")

    if not missing:
        scope = "full raw blobs" if args.scan_full_raw else "executable ranges"
        target_note = ""
        if only_targets:
            target_note = " for target(s) " + ", ".join(f"0x{target:08X}" for target in sorted(only_targets))
        print(f"[audit_indirect_targets] OK: scanned {scanned} NI overlay {scope}{target_note}; no missing indirect targets found.")
        return 0

    scope = "full-raw" if args.scan_full_raw else "text-range"
    print(f"[audit_indirect_targets] Found {len(missing)} missing {scope} candidate indirect target(s):")
    for row in missing[:args.limit]:
        sites = ", ".join(
            f"0x{site:08X}:{region}"
            for site, region in zip(row["sites"], row["site_regions"])
        )
        words = " ".join(f"{word:08X}" for word in row["words"])
        containing = row["containing"] or "(no containing declared function)"
        print(
            f"  ni_ovl_{row['pair']:03d} target=0x{row['target']:08X} "
            f"refs={row['refs']} sites=[{sites}] containing={containing} words=[{words}]"
        )

    if len(missing) > args.limit:
        print(f"  ... {len(missing) - args.limit} more omitted by --limit={args.limit}")

    return 1 if args.fail_on_missing else 0


if __name__ == "__main__":
    sys.exit(main())
