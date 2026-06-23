#!/usr/bin/env python3
"""
Audit map overlay text sections for hidden function boundaries.

N64Recomp only emits callable functions for boundaries listed in
castlevania2.syms.toml. Some map overlays contain callback/state-table
functions that start immediately after a previous function's `jr $ra` epilogue
but were missed by the current symbol list. If an indirect table jumps to one
of those starts, the runtime range fallback calls the containing function
instead, producing wrong behavior without necessarily crashing.

This checker is intentionally conservative. It only reports starts that:
  * are inside a declared map overlay function range,
  * are not already declared as a function start,
  * begin with `addiu sp, sp, -N`, and
  * are immediately preceded by `jr $ra` plus its delay slot.

Usage:
  python3 tools/audit_map_hidden_boundaries.py
  python3 tools/audit_map_hidden_boundaries.py --fail-on-missing
  python3 tools/audit_map_hidden_boundaries.py --section 29
  python3 tools/audit_map_hidden_boundaries.py --section 29 --fix-syms
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
import re
import struct
import sys
from typing import Any, Iterable

try:
    import tomllib
except ModuleNotFoundError:  # pragma: no cover - Python < 3.11 fallback
    tomllib = None  # type: ignore[assignment]


PROJECT_DIR = Path(__file__).resolve().parents[1]
DEFAULT_SYMS = PROJECT_DIR / "castlevania2.syms.toml"
DEFAULT_MAP_OVERLAYS = PROJECT_DIR / "assets" / "map_overlays.bin"
JR_RA = 0x03E00008


@dataclass(frozen=True)
class MissingBoundary:
    map_index: int
    section_name: str
    target: int
    containing_name: str
    containing_start: int
    containing_end: int
    direct_refs: tuple[int, ...]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Audit map overlays for hidden function-boundary starts.",
    )
    parser.add_argument("--syms", type=Path, default=DEFAULT_SYMS,
                        help="Path to castlevania2.syms.toml")
    parser.add_argument("--map-overlays", type=Path, default=DEFAULT_MAP_OVERLAYS,
                        help="Path to assets/map_overlays.bin")
    parser.add_argument("--section", type=int, action="append",
                        help="Only audit a specific map overlay index; may be repeated")
    parser.add_argument("--target", type=lambda s: int(s, 0), action="append",
                        help="Only report/fix a specific target VRAM address; may be repeated")
    parser.add_argument("--fix-syms", action="store_true",
                        help="Update castlevania2.syms.toml by splitting missing boundaries")
    parser.add_argument("--fail-on-missing", action="store_true",
                        help="Exit non-zero if any missing boundary is found")
    return parser.parse_args()


def load_toml(path: Path) -> dict[str, Any]:
    if tomllib is None:
        raise RuntimeError("Python 3.11+ is required for tomllib TOML parsing")
    with path.open("rb") as f:
        return tomllib.load(f)


def read_u32_be(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def is_stack_prologue(word: int) -> bool:
    # addiu sp, sp, imm16 with a negative stack adjustment.
    return (word & 0xFFFF0000) == 0x27BD0000 and bool(word & 0x8000)


def iter_map_sections(syms: dict[str, Any], only: set[int] | None):
    for section in syms.get("section", []):
        name = section.get("name", "")
        match = re.fullmatch(r"\.\.map_ovl_(\d+)", name)
        if not match:
            continue
        map_index = int(match.group(1))
        if only is not None and map_index not in only:
            continue
        yield map_index, section


def known_function_data(section: dict[str, Any]):
    functions = section.get("functions", [])
    known_starts = {int(func["vram"]) for func in functions}
    known_ranges = [
        (int(func["vram"]), int(func["vram"]) + int(func["size"]), str(func["name"]))
        for func in functions
    ]
    return functions, known_starts, known_ranges


def containing_function(target: int, known_ranges: Iterable[tuple[int, int, str]]) -> tuple[int, int, str] | None:
    for start, end, name in known_ranges:
        if start < target < end:
            return start, end, name
    return None


def find_direct_refs(data: bytes, vram: int, target: int) -> tuple[int, ...]:
    refs: list[int] = []
    for offset in range(0, max(0, len(data) - 3), 4):
        if read_u32_be(data, offset) == target:
            refs.append(vram + offset)
    return tuple(refs)


def scan_section(map_index: int, section: dict[str, Any], blob: bytes,
                 rom_base: int, next_rom: int | None,
                 only_targets: set[int] | None) -> list[MissingBoundary]:
    section_name = str(section["name"])
    rom = int(section["rom"])
    vram = int(section["vram"])
    text_size = int(section["size"])
    offset = rom - rom_base
    if offset < 0 or offset + text_size > len(blob):
        raise RuntimeError(
            f"{section_name}: ROM range 0x{rom:08X}+0x{text_size:X} "
            f"is outside {DEFAULT_MAP_OVERLAYS}"
        )

    text = blob[offset:offset + text_size]
    # Keep direct-ref context broad enough to include the map overlay's rodata,
    # but stop at the next map overlay because every map shares the same VRAM.
    full_size = (next_rom - rom) if next_rom is not None else (len(blob) - offset)
    full_size = max(text_size, min(full_size, len(blob) - offset))
    full_data = blob[offset:offset + full_size]

    _functions, known_starts, known_ranges = known_function_data(section)
    missing: list[MissingBoundary] = []
    for local in range(8, max(8, text_size - 3), 4):
        target = vram + local
        if only_targets is not None and target not in only_targets:
            continue
        if target in known_starts:
            continue

        word = read_u32_be(text, local)
        if not is_stack_prologue(word):
            continue
        if read_u32_be(text, local - 8) != JR_RA:
            continue

        containing = containing_function(target, known_ranges)
        if containing is None:
            continue

        start, end, containing_name = containing
        refs = find_direct_refs(full_data, vram, target)
        missing.append(MissingBoundary(
            map_index=map_index,
            section_name=section_name,
            target=target,
            containing_name=containing_name,
            containing_start=start,
            containing_end=end,
            direct_refs=refs,
        ))

    return missing


def generated_map_func_name(map_index: int, vram: int) -> str:
    return f"map_ovl_{map_index:02d}_func_{vram:08X}"


def split_functions_for_map(map_index: int, section: dict[str, Any],
                            targets: set[int]) -> tuple[list[dict[str, Any]], int]:
    functions = sorted(section.get("functions", []), key=lambda f: int(f["vram"]))
    existing_starts = {int(func["vram"]) for func in functions}
    updated: list[dict[str, Any]] = []
    added = 0

    for func in functions:
        start = int(func["vram"])
        end = start + int(func["size"])
        internal = sorted(
            target for target in targets
            if start < target < end and target not in existing_starts
        )
        if not internal:
            updated.append(dict(func))
            continue

        boundaries = [start, *internal, end]
        first = dict(func)
        first["size"] = boundaries[1] - boundaries[0]
        updated.append(first)

        for index, target in enumerate(internal, start=1):
            next_boundary = boundaries[index + 1]
            updated.append({
                "name": generated_map_func_name(map_index, target),
                "vram": target,
                "size": next_boundary - target,
            })
            added += 1

    return updated, added


def replace_section_functions_block(text: str, section_name: str,
                                    functions: list[dict[str, Any]]) -> str:
    section_pattern = re.compile(
        rf"(?ms)^\[\[section\]\]\n(?:(?!^\[\[section\]\]).)*?"
        rf"^name = \"{re.escape(section_name)}\"(?:(?!^\[\[section\]\]).)*"
    )
    match = section_pattern.search(text)
    if not match:
        raise RuntimeError(f"Could not find text block for {section_name}")

    section_text = match.group(0)
    funcs_match = re.search(r"(?ms)^functions = \[\n.*?^\]", section_text)
    if not funcs_match:
        raise RuntimeError(f"Could not find functions array for {section_name}")

    lines = ["functions = ["]
    for func in functions:
        lines.append(
            f"    {{ name = \"{func['name']}\", "
            f"vram = 0x{int(func['vram']):08X}, "
            f"size = 0x{int(func['size']):X} }},"
        )
    lines.append("]")
    new_section_text = (
        section_text[:funcs_match.start()] +
        "\n".join(lines) +
        section_text[funcs_match.end():]
    )
    return text[:match.start()] + new_section_text + text[match.end():]


def fix_syms(syms_path: Path, syms: dict[str, Any],
             missing: list[MissingBoundary]) -> int:
    targets_by_map: dict[int, set[int]] = {}
    for row in missing:
        targets_by_map.setdefault(row.map_index, set()).add(row.target)

    if not targets_by_map:
        print("[audit_map_hidden_boundaries] No map hidden boundaries need syms splits.")
        return 0

    text = syms_path.read_text()
    sections_by_map = {
        map_index: section
        for map_index, section in iter_map_sections(syms, None)
    }
    total_added = 0
    for map_index in sorted(targets_by_map):
        section = sections_by_map[map_index]
        updated_functions, added = split_functions_for_map(
            map_index, section, targets_by_map[map_index]
        )
        if added == 0:
            continue
        text = replace_section_functions_block(text, str(section["name"]), updated_functions)
        section["functions"] = updated_functions
        total_added += added
        print(f"[audit_map_hidden_boundaries] map_ovl_{map_index:02d}: split {added} hidden boundary target(s)")

    syms_path.write_text(text)
    print(f"[audit_map_hidden_boundaries] Updated {syms_path}: added {total_added} function boundary split(s).")
    return total_added


def print_missing(missing: list[MissingBoundary]) -> None:
    print(f"[audit_map_hidden_boundaries] Found {len(missing)} missing map hidden boundary target(s):")
    for row in missing:
        refs = ", ".join(f"0x{ref:08X}" for ref in row.direct_refs[:8])
        if len(row.direct_refs) > 8:
            refs += ", ..."
        refs_text = refs if refs else "(none)"
        print(
            f"  map_ovl_{row.map_index:02d} target=0x{row.target:08X} "
            f"containing={row.containing_name} "
            f"range=0x{row.containing_start:08X}-0x{row.containing_end:08X} "
            f"direct_refs=[{refs_text}]"
        )


def main() -> int:
    args = parse_args()
    only = set(args.section) if args.section else None
    only_targets = set(args.target) if args.target else None

    syms = load_toml(args.syms)
    all_sections = sorted(iter_map_sections(syms, None),
                          key=lambda entry: int(entry[1]["rom"]))
    sections = [entry for entry in all_sections
                if only is None or entry[0] in only]
    if not sections:
        print("[audit_map_hidden_boundaries] No map overlay sections matched.")
        return 1 if args.fail_on_missing else 0

    blob = args.map_overlays.read_bytes()
    rom_base = min(int(section["rom"]) for _map_index, section in all_sections)
    next_rom_by_map: dict[int, int | None] = {}
    for index, (map_index, section) in enumerate(all_sections):
        if index + 1 < len(all_sections):
            next_rom_by_map[map_index] = int(all_sections[index + 1][1]["rom"])
        else:
            next_rom_by_map[map_index] = None

    missing: list[MissingBoundary] = []
    for map_index, section in sections:
        missing.extend(scan_section(
            map_index, section, blob, rom_base,
            next_rom_by_map[map_index], only_targets))
    missing.sort(key=lambda row: (row.map_index, row.target))

    if args.fix_syms:
        fix_syms(args.syms, syms, missing)
        return 0

    if not missing:
        target_note = ""
        if only_targets:
            target_note = " for target(s) " + ", ".join(
                f"0x{target:08X}" for target in sorted(only_targets)
            )
        print(f"[audit_map_hidden_boundaries] OK: scanned {len(sections)} map overlay section(s){target_note}; no hidden boundaries found.")
        return 0

    print_missing(missing)
    return 1 if args.fail_on_missing else 0


if __name__ == "__main__":
    sys.exit(main())
