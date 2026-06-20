#!/usr/bin/env python3
"""
Audit NI overlay raw blobs for likely indirect function targets that are not
declared as function starts in castlevania2.syms.toml.

Why this exists:
  N64Recomp only emits callable functions for known function boundaries. Small
  state-table/callback targets can be missed when they are referenced only by
  data tables. Concrete examples include:
    * NI44 target 0x0F004030, a 0x14-byte helper used by the Map76 werewolf
      boss state machine.
    * NI15 target 0x0F002A4C, an Art Tower knight state-table target that lived
      in raw data past the declared executable range. Without a function split,
      range fallback called the containing handler at 0x0F002830 instead.

The audit has two complementary scans:
  * The legacy word scan treats aligned words inside the overlay executable
    range as candidate indirect targets.
  * --scan-call-tables follows local MIPS patterns where a table word is loaded
    and immediately consumed by jalr. This catches callback/state tables in raw
    data without treating every pointer-looking rodata word as a function.

Use --fix-syms with --scan-call-tables to split proven call-table targets that
land inside an existing declared function range. The splitter only changes
castlevania2.syms.toml; always regenerate with ./tools/regen_recomp.sh after it.

Usage:
  python3 tools/audit_indirect_targets.py
  python3 tools/audit_indirect_targets.py --fail-on-missing
  python3 tools/audit_indirect_targets.py --scan-call-tables --fail-on-missing
  python3 tools/audit_indirect_targets.py --scan-call-tables --fix-syms
  python3 tools/audit_indirect_targets.py --section 44
  python3 tools/audit_indirect_targets.py --scan-full-raw --section 7 --target 0x0F010218
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
DEFAULT_RAW_DIR = PROJECT_DIR / "tools" / "ni_ovl" / "out" / "raw"

# Targets that look like jalr call-table entries but are unsafe to auto-split
# with the current N64Recomp. Keep this list tiny and documented: every entry is
# still visible in raw data, but adding a boundary there currently prevents the
# generator from reaching the normal truncation/repair point.
CALL_TABLE_TARGET_EXCLUDES = {
    # ni118 has tiny branch-label targets inside ni_ovl_118_func_0F0013A4.
    # Splitting either one causes N64Recomp to abort early while sizing the
    # local jump table at 0x0F001A70 for instruction 0x0F0014D0, leaving later
    # overlay functions undeclared. Leave these to the existing range fallback
    # unless a targeted runtime bug proves they need a hand-written shim.
    (118, 0x0F0014D8),
    (118, 0x0F00150C),

    # ni120's result scheduler is already handled by the scoped
    # LOD_FIX_NI_PAIR120_INTERNAL_LABELS runtime shim. Auto-splitting these
    # tiny result-table labels makes N64Recomp abort early while sizing the
    # jump table at 0x0F001700 for instruction 0x0F000828.
    (120, 0x0F000830),
    (120, 0x0F000864),
    (120, 0x0F00086C),
    (120, 0x0F0008D4),
    (120, 0x0F0008E4),
    (120, 0x0F000958),
    (120, 0x0F000964),
    (120, 0x0F000974),
}


@dataclass(frozen=True)
class RegValue:
    kind: str
    value: int | None = None


UNKNOWN = RegValue("unknown")


def const(value: int) -> RegValue:
    return RegValue("const", value & 0xFFFFFFFF)


def dynbase(value: int) -> RegValue:
    """A dynamic expression with a known constant base: value + unknown."""
    return RegValue("dynbase", value & 0xFFFFFFFF)


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
                              "restricted to the executable text range. This is intentionally "
                              "broad/noisy; prefer --scan-call-tables for callback tables."))
    parser.add_argument("--scan-call-tables", action="store_true",
                        help=("Also scan local MIPS jalr call-table patterns and audit the "
                              "raw-data function pointers they consume."))
    parser.add_argument("--fix-syms", action="store_true",
                        help=("Update castlevania2.syms.toml by splitting missing call-table "
                              "targets that land inside existing functions. Implies "
                              "--scan-call-tables and ignores legacy word-scan candidates."))
    parser.add_argument("--target", type=lambda s: int(s, 0), action="append",
                        help="Only report/fix a specific target VRAM address; may be repeated")
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
    for index, section in enumerate(syms.get("section", [])):
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
                "section_index": index,
                "section": section,
                "raw_path": raw_path,
                "missing_raw": True,
            }
            continue

        yield {
            "pair": pair,
            "section_index": index,
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


def sign_extend_16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def add_values(lhs: RegValue, rhs: RegValue) -> RegValue:
    if lhs.kind == "const" and rhs.kind == "const":
        assert lhs.value is not None and rhs.value is not None
        return const(lhs.value + rhs.value)
    if lhs.kind == "dynbase" and rhs.kind == "const":
        assert lhs.value is not None and rhs.value is not None
        return dynbase(lhs.value + rhs.value)
    if rhs.kind == "dynbase" and lhs.kind == "const":
        assert lhs.value is not None and rhs.value is not None
        return dynbase(rhs.value + lhs.value)
    if lhs.kind == "const" and rhs.kind == "unknown":
        assert lhs.value is not None
        return dynbase(lhs.value)
    if rhs.kind == "const" and lhs.kind == "unknown":
        assert rhs.value is not None
        return dynbase(rhs.value)
    return UNKNOWN


def decode(ins: int) -> dict[str, int]:
    return {
        "op": (ins >> 26) & 0x3F,
        "rs": (ins >> 21) & 0x1F,
        "rt": (ins >> 16) & 0x1F,
        "rd": (ins >> 11) & 0x1F,
        "sh": (ins >> 6) & 0x1F,
        "fn": ins & 0x3F,
        "imm": ins & 0xFFFF,
    }


def written_gpr(d: dict[str, int]) -> int | None:
    op = d["op"]
    fn = d["fn"]
    if op == 0:
        # Common SPECIAL integer destinations. jr does not write; jalr writes rd
        # (or ra when rd is omitted, though emitted encodings normally include rd).
        if fn in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2A, 0x2B,
                  0x00, 0x02, 0x03):
            return d["rd"] or None
        if fn == 0x09:  # jalr
            return d["rd"] or 31
        return None

    # I-type ALU and load destinations.
    if op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
              0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
              0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37):
        return d["rt"] or None

    return None


def resolve_reg_value(instrs: list[int], reg: int, before_index: int,
                      start_index: int, depth: int = 0) -> RegValue:
    """Best-effort local backward slice for a GPR before instrs[before_index]."""
    if reg == 0:
        return const(0)
    if depth > 10:
        return UNKNOWN

    for index in range(before_index - 1, start_index - 1, -1):
        d = decode(instrs[index])
        if written_gpr(d) != reg:
            continue

        op = d["op"]
        fn = d["fn"]
        imm = d["imm"]

        if op == 0x0F:  # lui
            return const(imm << 16)

        if op in (0x08, 0x09):  # addi/addiu
            return add_values(resolve_reg_value(instrs, d["rs"], index, start_index, depth + 1),
                              const(sign_extend_16(imm)))

        if op == 0x0D:  # ori
            lhs = resolve_reg_value(instrs, d["rs"], index, start_index, depth + 1)
            if lhs.kind == "const":
                assert lhs.value is not None
                return const(lhs.value | imm)
            return UNKNOWN

        if op == 0 and fn in (0x20, 0x21):  # add/addu
            return add_values(resolve_reg_value(instrs, d["rs"], index, start_index, depth + 1),
                              resolve_reg_value(instrs, d["rt"], index, start_index, depth + 1))

        if op == 0 and fn == 0x25:  # or
            lhs = resolve_reg_value(instrs, d["rs"], index, start_index, depth + 1)
            rhs = resolve_reg_value(instrs, d["rt"], index, start_index, depth + 1)
            if lhs.kind == "const" and rhs.kind == "const":
                assert lhs.value is not None and rhs.value is not None
                return const(lhs.value | rhs.value)
            if d["rs"] == 0:
                return rhs
            if d["rt"] == 0:
                return lhs
            return UNKNOWN

        if op == 0 and fn == 0x00:  # sll
            rhs = resolve_reg_value(instrs, d["rt"], index, start_index, depth + 1)
            if rhs.kind == "const":
                assert rhs.value is not None
                return const(rhs.value << d["sh"])
            return UNKNOWN

        # A load or unmodeled ALU write makes the value unknown.
        return UNKNOWN

    return UNKNOWN


def function_start_indices(vram: int, instruction_count: int,
                           functions: list[dict[str, Any]]) -> list[int]:
    starts = sorted(int(func["vram"]) for func in functions)
    if not starts:
        return [0] * instruction_count

    out: list[int] = []
    current = 0
    for index in range(instruction_count):
        addr = vram + index * 4
        while current + 1 < len(starts) and starts[current + 1] <= addr:
            current += 1
        out.append(max(0, (starts[current] - vram) // 4))
    return out


def find_jalr_table_base(instrs: list[int], jalr_index: int, function_start_index: int) -> int | None:
    """Return a local raw table base for a jalr target loaded from base+index."""
    jalr = decode(instrs[jalr_index])
    target_reg = jalr["rs"]

    # The callback-table pattern is normally lw target, imm(base) immediately
    # before jalr, sometimes with a delay-slot setup between. Keep the window
    # small so stale register values across branches are not interpreted as
    # fresh table loads.
    for index in range(jalr_index - 1, max(function_start_index, jalr_index - 16) - 1, -1):
        d = decode(instrs[index])
        if written_gpr(d) != target_reg:
            continue

        if d["op"] != 0x23:  # lw target, imm(base)
            return None

        base = resolve_reg_value(instrs, d["rs"], index, function_start_index)
        if base.kind not in ("const", "dynbase") or base.value is None:
            return None
        return (base.value + sign_extend_16(d["imm"])) & 0xFFFFFFFF

    return None


def read_call_table_entries(data: bytes, vram: int, text_size: int, table_base: int,
                            max_entries: int = 256) -> list[tuple[int, int]]:
    """Read contiguous executable VRAM pointers from a local raw table base."""
    offset = table_base - vram
    entries: list[tuple[int, int]] = []
    if offset < 0 or offset > len(data) - 4:
        return entries

    text_end = vram + text_size
    for index in range(max_entries):
        word_off = offset + index * 4
        if word_off > len(data) - 4:
            break
        value = struct.unpack_from(">I", data, word_off)[0]
        if value % 4 != 0 or not (vram <= value < text_end):
            break
        entries.append((table_base + index * 4, value))

    return entries


def known_function_data(section: dict[str, Any]):
    functions = section.get("functions", [])
    known_starts = {int(func["vram"]) for func in functions}
    known_ranges = [
        (int(func["vram"]), int(func["vram"]) + int(func["size"]), func["name"])
        for func in functions
    ]
    return functions, known_starts, known_ranges


def containing_function(target: int, known_ranges: Iterable[tuple[int, int, str]]) -> tuple[int, int, str] | None:
    for start, end, name in known_ranges:
        if start <= target < end:
            return start, end, name
    return None


def candidate_is_code(data: bytes, vram: int, target: int) -> bool:
    target_off = target - vram
    target_words = words_at(data, target_off)
    return bool(target_words and target_words[0] not in (0x00000000, 0xFFFFFFFF))


def scan_word_refs(pair: int, section: dict[str, Any], raw_path: Path,
                   scan_full_raw: bool = False,
                   only_targets: set[int] | None = None) -> list[dict[str, Any]]:
    vram = int(section["vram"])
    size = int(section["size"])
    _functions, known_starts, known_ranges = known_function_data(section)

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
        if not candidate_is_code(data, vram, target):
            continue

        containing = containing_function(target, known_ranges)
        missing.append({
            "source": "word-ref-full-raw" if scan_full_raw else "word-ref-text",
            "pair": pair,
            "target": target,
            "refs": len(sites),
            "sites": sites[:8],
            "site_regions": [
                "text" if vram <= site < vram + text_size else "data"
                for site in sites[:8]
            ],
            "calls": [],
            "table_bases": [],
            "words": words_at(data, target - vram),
            "containing": containing[2] if containing else None,
            "containing_start": containing[0] if containing else None,
            "containing_end": containing[1] if containing else None,
        })

    return missing


def scan_call_table_refs(pair: int, section: dict[str, Any], raw_path: Path,
                         only_targets: set[int] | None = None) -> list[dict[str, Any]]:
    vram = int(section["vram"])
    size = int(section["size"])
    functions, known_starts, known_ranges = known_function_data(section)

    data = raw_path.read_bytes()
    text_size = min(size, len(data))
    instrs = [struct.unpack_from(">I", data, raw_off)[0]
              for raw_off in range(0, max(0, text_size - 3), 4)]
    start_indices = function_start_indices(vram, len(instrs), functions)

    refs: dict[int, dict[str, Any]] = {}
    for index, ins in enumerate(instrs):
        d = decode(ins)
        if d["op"] != 0 or d["fn"] != 0x09:  # jalr
            continue

        call_site = vram + index * 4
        table_base = find_jalr_table_base(instrs, index, start_indices[index])
        if table_base is None:
            continue

        entries = read_call_table_entries(data, vram, text_size, table_base)
        if not entries:
            continue

        for table_site, target in entries:
            if (pair, target) in CALL_TABLE_TARGET_EXCLUDES:
                continue
            if only_targets is not None and target not in only_targets:
                continue
            if target in known_starts:
                continue
            if not candidate_is_code(data, vram, target):
                continue

            row = refs.setdefault(target, {
                "source": "call-table",
                "pair": pair,
                "target": target,
                "refs": 0,
                "sites": [],
                "site_regions": [],
                "calls": [],
                "table_bases": [],
                "words": words_at(data, target - vram),
                "containing": None,
                "containing_start": None,
                "containing_end": None,
            })
            row["refs"] += 1
            if table_site not in row["sites"] and len(row["sites"]) < 8:
                row["sites"].append(table_site)
                row["site_regions"].append("call-table")
            if call_site not in row["calls"]:
                row["calls"].append(call_site)
            if table_base not in row["table_bases"]:
                row["table_bases"].append(table_base)

    missing: list[dict[str, Any]] = []
    for target, row in sorted(refs.items()):
        containing = containing_function(target, known_ranges)
        row["containing"] = containing[2] if containing else None
        row["containing_start"] = containing[0] if containing else None
        row["containing_end"] = containing[1] if containing else None
        row["calls"] = sorted(row["calls"])[:8]
        row["table_bases"] = sorted(row["table_bases"])[:8]
        missing.append(row)

    return missing


def merge_missing(rows: Iterable[dict[str, Any]]) -> list[dict[str, Any]]:
    merged: dict[tuple[int, int, str], dict[str, Any]] = {}
    for row in rows:
        key = (row["pair"], row["target"], row["source"])
        if key not in merged:
            merged[key] = row
            continue
        existing = merged[key]
        existing["refs"] += row.get("refs", 0)
        for field in ("sites", "site_regions", "calls", "table_bases"):
            for value in row.get(field, []):
                if value not in existing[field] and len(existing[field]) < 8:
                    existing[field].append(value)
    return sorted(merged.values(), key=lambda r: (r["pair"], r["target"], r["source"]))


def generated_func_name(pair: int, vram: int) -> str:
    return f"ni_ovl_{pair:03d}_func_{vram:08X}"


def split_functions_for_pair(pair: int, section: dict[str, Any], targets: set[int]) -> tuple[list[dict[str, Any]], int]:
    functions = sorted(section.get("functions", []), key=lambda f: int(f["vram"]))
    existing_starts = {int(func["vram"]) for func in functions}
    updated: list[dict[str, Any]] = []
    added = 0

    for func in functions:
        start = int(func["vram"])
        end = start + int(func["size"])
        internal = sorted(target for target in targets if start < target < end and target not in existing_starts)
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
                "name": generated_func_name(pair, target),
                "vram": target,
                "size": next_boundary - target,
            })
            added += 1

    return updated, added


def replace_section_functions_block(text: str, pair: int, functions: list[dict[str, Any]]) -> str:
    section_pattern = re.compile(rf"(?ms)^\[\[section\]\]\n(?:(?!^\[\[section\]\]).)*?^name = \"\.\.ni_ovl_{pair:03d}\"(?:(?!^\[\[section\]\]).)*")
    match = section_pattern.search(text)
    if not match:
        raise RuntimeError(f"Could not find text block for ni_ovl_{pair:03d}")

    section_text = match.group(0)
    funcs_match = re.search(r"(?ms)^functions = \[\n.*?^\]", section_text)
    if not funcs_match:
        raise RuntimeError(f"Could not find functions array for ni_ovl_{pair:03d}")

    lines = ["functions = ["]
    for func in functions:
        lines.append(
            f"    {{ name = \"{func['name']}\", vram = 0x{int(func['vram']):08X}, size = 0x{int(func['size']):X} }},"
        )
    lines.append("]")
    new_section_text = section_text[:funcs_match.start()] + "\n".join(lines) + section_text[funcs_match.end():]
    return text[:match.start()] + new_section_text + text[match.end():]


def fix_syms(syms_path: Path, syms: dict[str, Any], sections: list[dict[str, Any]],
             missing: list[dict[str, Any]]) -> int:
    targets_by_pair: dict[int, set[int]] = {}
    skipped: list[dict[str, Any]] = []
    for row in missing:
        if row["source"] != "call-table":
            continue
        if row["containing_start"] is None:
            skipped.append(row)
            continue
        targets_by_pair.setdefault(row["pair"], set()).add(row["target"])

    if not targets_by_pair:
        print("[audit_indirect_targets] No call-table internal targets need syms splits.")
        return 0

    text = syms_path.read_text()
    total_added = 0
    sections_by_pair = {entry["pair"]: entry["section"] for entry in sections if not entry["missing_raw"]}

    for pair in sorted(targets_by_pair):
        section = sections_by_pair[pair]
        updated_functions, added = split_functions_for_pair(pair, section, targets_by_pair[pair])
        if added == 0:
            continue
        text = replace_section_functions_block(text, pair, updated_functions)
        total_added += added
        section["functions"] = updated_functions
        print(f"[audit_indirect_targets] ni_ovl_{pair:03d}: split {added} call-table target(s)")

    syms_path.write_text(text)
    if skipped:
        print(f"[audit_indirect_targets] Warning: skipped {len(skipped)} target(s) with no containing function")
    print(f"[audit_indirect_targets] Updated {syms_path}: added {total_added} function boundary split(s).")
    return total_added


def print_missing(missing: list[dict[str, Any]], limit: int, scan_full_raw: bool,
                  scan_call_tables: bool) -> None:
    scopes = []
    scopes.append("full-raw" if scan_full_raw else "text-range")
    if scan_call_tables:
        scopes.append("call-table")
    scope = "+".join(scopes)
    print(f"[audit_indirect_targets] Found {len(missing)} missing {scope} candidate indirect target(s):")
    for row in missing[:limit]:
        sites = ", ".join(
            f"0x{site:08X}:{region}"
            for site, region in zip(row["sites"], row["site_regions"])
        )
        words = " ".join(f"{word:08X}" for word in row["words"])
        containing = row["containing"] or "(no containing declared function)"
        extras = ""
        if row["source"] == "call-table":
            calls = ", ".join(f"0x{call:08X}" for call in row["calls"])
            bases = ", ".join(f"0x{base:08X}" for base in row["table_bases"])
            extras = f" calls=[{calls}] tables=[{bases}]"
        print(
            f"  ni_ovl_{row['pair']:03d} source={row['source']} target=0x{row['target']:08X} "
            f"refs={row['refs']} sites=[{sites}] containing={containing}{extras} words=[{words}]"
        )

    if len(missing) > limit:
        print(f"  ... {len(missing) - limit} more omitted by --limit={limit}")


def main() -> int:
    args = parse_args()
    if args.fix_syms:
        args.scan_call_tables = True

    only = set(args.section) if args.section else None
    only_targets = set(args.target) if args.target else None

    syms = load_toml(args.syms)
    sections = list(iter_ni_sections(syms, args.raw_dir, only))

    if not sections:
        print("[audit_indirect_targets] No NI sections matched.")
        return 1 if args.fail_on_missing else 0

    missing_raw = [entry for entry in sections if entry["missing_raw"]]
    scanned = 0
    missing_rows: list[dict[str, Any]] = []

    for entry in sections:
        if entry["missing_raw"]:
            continue
        scanned += 1
        if args.fix_syms:
            missing_rows.extend(scan_call_table_refs(entry["pair"], entry["section"], entry["raw_path"],
                                                     only_targets=only_targets))
        else:
            missing_rows.extend(scan_word_refs(entry["pair"], entry["section"], entry["raw_path"],
                                               scan_full_raw=args.scan_full_raw,
                                               only_targets=only_targets))
            if args.scan_call_tables:
                missing_rows.extend(scan_call_table_refs(entry["pair"], entry["section"], entry["raw_path"],
                                                         only_targets=only_targets))

    missing = merge_missing(missing_rows)

    if args.fix_syms:
        changed = fix_syms(args.syms, syms, sections, missing)
        return 0 if changed >= 0 else 1

    if missing_raw:
        print(f"[audit_indirect_targets] Warning: {len(missing_raw)} raw blob(s) missing; skipped.")
        for entry in missing_raw[:10]:
            print(f"  ni_ovl_{entry['pair']:03d}: {entry['raw_path']}")

    if not missing:
        scope = "full raw blobs" if args.scan_full_raw else "executable ranges"
        if args.scan_call_tables:
            scope += " + call tables"
        target_note = ""
        if only_targets:
            target_note = " for target(s) " + ", ".join(f"0x{target:08X}" for target in sorted(only_targets))
        print(f"[audit_indirect_targets] OK: scanned {scanned} NI overlay {scope}{target_note}; no missing indirect targets found.")
        return 0

    print_missing(missing, args.limit, args.scan_full_raw, args.scan_call_tables)
    return 1 if args.fail_on_missing else 0


if __name__ == "__main__":
    sys.exit(main())
