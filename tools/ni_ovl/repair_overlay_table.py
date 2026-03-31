#!/usr/bin/env python3
"""
Repair and verify NI overlay section metadata in RecompiledFuncs/recomp_overlays.inl.

Why this exists:
- fix_n64recomp_truncation.py can append placeholder section entries when
  recomp_overlays.inl is truncated.
- Those placeholders use rom=0, ram=0x802E3B70, size=4, which is invalid for NI
  overlays and breaks runtime lookup/loading behavior.

This script validates every NI section table entry against generated NI metadata
and can rewrite bad entries deterministically.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR = SCRIPT_DIR.parent.parent

INL_PATH = ROOT_DIR / "RecompiledFuncs" / "recomp_overlays.inl"
NI_DATA_PATH = ROOT_DIR / "src" / "main" / "ni_ovl_data.h"
FUNCTIONS_PATH = ROOT_DIR / "tools" / "ni_ovl" / "out" / "functions.json"
CLASSIFICATION_PATH = ROOT_DIR / "tools" / "ni_ovl" / "out" / "vram_classification.json"

EXPECTED_PAIR_COUNT = 245

ENTRY_RE = re.compile(
    r"^(?P<indent>\s*)\{ \.rom_addr = 0x(?P<rom>[0-9A-Fa-f]+), "
    r"\.ram_addr = 0x(?P<ram>[0-9A-Fa-f]+), "
    r"\.size = 0x(?P<size>[0-9A-Fa-f]+), "
    r"\.funcs = section_(?P<section>\d+)_ni_ovl_(?P<pair>\d+)_funcs, "
    r"\.num_funcs = ARRLEN\(section_(?P<section2>\d+)_ni_ovl_(?P<pair2>\d+)_funcs\), "
    r"\.relocs = nullptr, \.num_relocs = 0, \.index = (?P<index>\d+) \},?$"
)


def load_expected_rom_offsets() -> dict[int, int]:
    text = NI_DATA_PATH.read_text()
    expected: dict[int, int] = {}
    # Example:
    # { 0x0143DF10, 0x01320 }, // ni_ovl_231 vram=0x0F000000
    for m in re.finditer(
        r"\{\s*0x([0-9A-Fa-f]+),\s*0x([0-9A-Fa-f]+)\s*\},\s*//\s*ni_ovl_(\d+)\s+vram=0x([0-9A-Fa-f]+)",
        text,
    ):
        rom_offset = int(m.group(1), 16)
        pair = int(m.group(3))
        expected[pair] = rom_offset
    return expected


def load_expected_text_sizes() -> dict[int, int]:
    with FUNCTIONS_PATH.open() as f:
        data = json.load(f)
    return {int(row["pair_index"]): int(row["text_size"]) for row in data}


def load_0e_pairs() -> set[int]:
    with CLASSIFICATION_PATH.open() as f:
        data = json.load(f)
    return {int(x) for x in data["0x0E_pairs"]}


def expected_ram_addr(pair: int, pairs_0e: set[int]) -> int:
    return 0x0E000000 if pair in pairs_0e else 0x0F000000


def build_entry_line(indent: str, section: int, pair: int, index: int, rom: int, ram: int, size: int) -> str:
    return (
        f"{indent}{{ .rom_addr = 0x{rom:08X}, .ram_addr = 0x{ram:08X}, .size = 0x{size:08X}, "
        f".funcs = section_{section}_ni_ovl_{pair:03d}_funcs, .num_funcs = ARRLEN(section_{section}_ni_ovl_{pair:03d}_funcs), "
        f".relocs = nullptr, .num_relocs = 0, .index = {index} }},"
    )


def validate_and_optionally_fix(do_fix: bool) -> int:
    rom_offsets = load_expected_rom_offsets()
    text_sizes = load_expected_text_sizes()
    pairs_0e = load_0e_pairs()

    missing_meta = []
    for pair in range(EXPECTED_PAIR_COUNT):
        if pair not in rom_offsets:
            missing_meta.append(f"ni_ovl_data missing pair {pair}")
        if pair not in text_sizes:
            missing_meta.append(f"functions.json missing pair {pair}")
    if missing_meta:
        for msg in missing_meta:
            print(f"ERROR: {msg}")
        return 1

    lines = INL_PATH.read_text().splitlines()

    pair_to_lines: dict[int, list[int]] = {}
    mismatches: list[str] = []
    parse_errors: list[str] = []
    changes = 0

    for i, line in enumerate(lines):
        if "_ni_ovl_" not in line or ".rom_addr" not in line:
            continue

        m = ENTRY_RE.match(line)
        if not m:
            parse_errors.append(f"line {i + 1}: cannot parse NI entry")
            continue

        section = int(m.group("section"))
        pair = int(m.group("pair"))
        section2 = int(m.group("section2"))
        pair2 = int(m.group("pair2"))
        index = int(m.group("index"))
        rom = int(m.group("rom"), 16)
        ram = int(m.group("ram"), 16)
        size = int(m.group("size"), 16)
        indent = m.group("indent")

        if section != section2 or pair != pair2:
            parse_errors.append(
                f"line {i + 1}: funcs/num_funcs mismatch (section {section}/{section2}, pair {pair}/{pair2})"
            )
            continue

        pair_to_lines.setdefault(pair, []).append(i)

        expected_rom = rom_offsets[pair]
        expected_ram = expected_ram_addr(pair, pairs_0e)
        expected_size = text_sizes[pair]

        if (rom, ram, size) != (expected_rom, expected_ram, expected_size):
            mismatches.append(
                f"pair {pair:03d} line {i + 1}: "
                f"have rom=0x{rom:08X} ram=0x{ram:08X} size=0x{size:08X}, "
                f"expected rom=0x{expected_rom:08X} ram=0x{expected_ram:08X} size=0x{expected_size:08X}"
            )
            if do_fix:
                lines[i] = build_entry_line(indent, section, pair, index, expected_rom, expected_ram, expected_size)
                changes += 1

    duplicates = {pair: locs for pair, locs in pair_to_lines.items() if len(locs) > 1}
    missing_pairs = [pair for pair in range(EXPECTED_PAIR_COUNT) if pair not in pair_to_lines]
    extra_pairs = sorted(pair for pair in pair_to_lines if pair < 0 or pair >= EXPECTED_PAIR_COUNT)

    if parse_errors:
        for msg in parse_errors:
            print(f"ERROR: {msg}")
        return 1

    if duplicates:
        for pair, locs in sorted(duplicates.items()):
            human = ", ".join(str(x + 1) for x in locs)
            print(f"ERROR: duplicate NI pair {pair:03d} in section_table (lines {human})")
        return 1

    if missing_pairs:
        print(f"ERROR: missing NI pairs in section_table: {missing_pairs[:20]}" + (" ..." if len(missing_pairs) > 20 else ""))
        return 1

    if extra_pairs:
        print(f"ERROR: unexpected NI pair ids in section_table: {extra_pairs}")
        return 1

    if do_fix and changes > 0:
        INL_PATH.write_text("\n".join(lines) + "\n")
        print(f"Fixed {changes} NI section_table entries in {INL_PATH}")

    if mismatches:
        if do_fix:
            # Re-run validation in check mode to assert clean output.
            return validate_and_optionally_fix(False)
        for msg in mismatches:
            print(f"ERROR: {msg}")
        print(f"Found {len(mismatches)} NI metadata mismatches.")
        return 1

    print("NI overlay section metadata verification passed.")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Repair/verify NI overlay section metadata.")
    parser.add_argument("--fix", action="store_true", help="Rewrite invalid NI entries in recomp_overlays.inl.")
    args = parser.parse_args()

    return validate_and_optionally_fix(do_fix=args.fix)


if __name__ == "__main__":
    sys.exit(main())
