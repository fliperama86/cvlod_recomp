#!/usr/bin/env python3
"""
match_cv64.py — Match CV64 named functions to LoD ROM by instruction fingerprinting.

For each named function in CV64's symbol_addrs.txt:
  1. Read the raw MIPS instructions from the CV64 ROM
  2. Build a fingerprint: opcodes + register operands, with address-dependent
     fields (lui/addiu immediates, jal targets) masked out
  3. Search all LoD functions (from castlevania2.syms.toml) for matching fingerprints
  4. Report matches with confidence level

Usage:
    python3 tools/match_cv64.py [--min-size 8] [--max-results 5]
"""

import struct
import sys
import re
import os

# Paths
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(SCRIPT_DIR)
CV64_ROM = os.path.join(PROJECT_DIR, "resources", "castlevania.z64")
LOD_ROM = os.path.join(PROJECT_DIR, "resources", "castlevania_legacy_of_darkness.z64")
CV64_SYMS = os.path.join(PROJECT_DIR, "..", "references", "cv64_decomp", "linker", "symbol_addrs.txt")
LOD_SYMS = os.path.join(PROJECT_DIR, "castlevania2.syms.toml")

# MIPS instruction field extraction
def get_opcode(instr):
    return (instr >> 26) & 0x3F

def get_rs(instr):
    return (instr >> 21) & 0x1F

def get_rt(instr):
    return (instr >> 16) & 0x1F

def get_rd(instr):
    return (instr >> 11) & 0x1F

def get_funct(instr):
    return instr & 0x3F

def get_imm(instr):
    return instr & 0xFFFF

def get_target(instr):
    return instr & 0x3FFFFFF

def fingerprint_instruction(instr):
    """Create a normalized fingerprint for one MIPS instruction.
    Masks out address-dependent fields while preserving opcode structure."""
    op = get_opcode(instr)

    if op == 0:  # R-type (add, sub, sll, etc.)
        # Keep opcode + rs + rt + rd + shamt + funct
        return instr  # fully deterministic

    elif op == 2 or op == 3:  # j / jal
        # Mask target address — different ROMs have different addresses
        return op << 26  # keep only opcode

    elif op == 0x0F:  # lui
        # lui rt, imm — mask immediate (address-dependent)
        return (op << 26) | (get_rt(instr) << 16)

    elif op in (0x09, 0x0D, 0x0C, 0x0E, 0x0A, 0x0B):
        # addiu, ori, andi, xori, slti, sltiu
        # These often carry address lo16 parts — mask immediate
        # BUT: small constants (loop counters, struct offsets) are NOT address-dependent
        imm = get_imm(instr)
        rs = get_rs(instr)
        rt = get_rt(instr)

        # Heuristic: if rs == $at or rs == $gp, or previous was lui, likely address
        # Simple approach: mask if |imm| > 0x1000 (large constants are likely addresses)
        # Keep small immediates as they're structural (struct offsets, loop bounds)
        if op == 0x09:  # addiu
            simm = imm if imm < 0x8000 else imm - 0x10000
            if abs(simm) > 0x2000:
                return (op << 26) | (rs << 21) | (rt << 16)  # mask imm
        elif op in (0x0D, 0x0C, 0x0E):  # ori, andi, xori
            if imm > 0x2000:
                return (op << 26) | (rs << 21) | (rt << 16)  # mask imm

        return instr  # keep full instruction for small constants

    elif op in (0x04, 0x05, 0x06, 0x07, 0x01, 0x14, 0x15, 0x16, 0x17):
        # branch instructions — mask branch offset (address-dependent)
        return (op << 26) | (get_rs(instr) << 21) | (get_rt(instr) << 16)

    elif op >= 0x20 and op <= 0x2E:
        # load/store — keep opcode + rs + rt + offset
        # Offset is usually a struct field access (deterministic) or address lo16
        imm = get_imm(instr)
        if imm > 0x2000 and imm < 0xE000:  # likely address lo16
            return (op << 26) | (get_rs(instr) << 21) | (get_rt(instr) << 16)
        return instr  # keep struct offsets

    elif op == 0x11:  # COP1 (floating point)
        return instr  # FP instructions are fully deterministic

    elif op == 0x10:  # COP0
        return instr

    else:
        return instr


def fingerprint_function(rom_data, rom_offset, size):
    """Build fingerprint for a function: list of normalized instructions."""
    n_instrs = size // 4
    fp = []
    for i in range(n_instrs):
        off = rom_offset + i * 4
        if off + 4 > len(rom_data):
            break
        instr = struct.unpack('>I', rom_data[off:off+4])[0]
        fp.append(fingerprint_instruction(instr))
    return tuple(fp)


def vram_to_rom_cv64(vram):
    """Convert CV64 VRAM address to ROM offset.
    CV64 entry point: ROM 0x1000, VRAM 0x80000400."""
    return (vram - 0x80000400) + 0x1000


def vram_to_rom_lod(vram):
    """Convert LoD VRAM address to ROM offset.
    LoD entry point: ROM 0x1060, VRAM 0x80000460.
    But main code starts at ROM 0x1060, VRAM 0x80000460."""
    if vram >= 0x80141870:  # common/section 2
        return (vram - 0x80141870) + 0xC2120
    else:  # main section
        return (vram - 0x80000460) + 0x1060


def parse_cv64_symbols(path):
    """Parse CV64 symbol_addrs.txt → list of (name, vram)."""
    syms = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('//'):
                continue
            m = re.match(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;.*type:func', line)
            if m:
                name = m.group(1)
                vram = int(m.group(2), 16)
                syms.append((name, vram))
    return syms


def parse_lod_symbols(path):
    """Parse castlevania2.syms.toml → list of (name, vram, size)."""
    syms = []
    with open(path) as f:
        for line in f:
            m = re.search(r'name\s*=\s*"([^"]+)".*vram\s*=\s*0x([0-9A-Fa-f]+).*size\s*=\s*0x([0-9A-Fa-f]+)', line)
            if m:
                name = m.group(1)
                vram = int(m.group(2), 16)
                size = int(m.group(3), 16)
                syms.append((name, vram, size))
    return syms


def compute_cv64_sizes(syms):
    """Compute function sizes from sorted symbol list (size = next_addr - this_addr)."""
    sorted_syms = sorted(syms, key=lambda s: s[1])
    sized = []
    for i, (name, vram) in enumerate(sorted_syms):
        if i + 1 < len(sorted_syms):
            size = sorted_syms[i+1][1] - vram
        else:
            size = 0x100  # default for last function
        if size > 0 and size < 0x10000:  # sanity check
            sized.append((name, vram, size))
    return sized


def is_engine_func(name):
    """Filter to likely shared engine functions (not game-specific enemies/objects)."""
    skip_prefixes = (
        'os', '__os', 'gu', 'al', '__ll', '__ull', '__d_', '__f_',
        'sinf', 'sqrtf', 'strlen', 'memcpy', 'bcopy', 'bzero',
        'ldiv', 'lldiv', 'strchr', 'coss', 'viMgrMain',
        '_', 'commonMoon', 'corrupted', 'obj0', 'obj80',
        'Player_', 'player', 'spawnCastle', 'removeKey',
        'Interactable', 'interactable', 'HUDParams',
        'EnemyList_print', 'EnemyList_isAny',
        'GameplayMenu', 'processMeter', 'moveSelection',
        'check_inserted', 'init_lpfilter',
        'findFirst', 'findNext', 'getMapEvent',
    )
    for p in skip_prefixes:
        if name.startswith(p):
            return False
    if name.startswith('func_'):
        return False
    return True


def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--min-size', type=int, default=16,
                       help='Minimum function size in bytes to match')
    parser.add_argument('--min-match', type=float, default=0.85,
                       help='Minimum fingerprint match ratio (0-1)')
    parser.add_argument('--show-existing', action='store_true',
                       help='Show matches even if LoD already has a name')
    args = parser.parse_args()

    print(f"Loading ROMs...")
    with open(CV64_ROM, 'rb') as f:
        cv64_data = f.read()
    with open(LOD_ROM, 'rb') as f:
        lod_data = f.read()

    print(f"  CV64: {len(cv64_data)} bytes")
    print(f"  LoD:  {len(lod_data)} bytes")

    cv64_syms = parse_cv64_symbols(CV64_SYMS)
    lod_syms = parse_lod_symbols(LOD_SYMS)
    print(f"  CV64 symbols: {len(cv64_syms)}")
    print(f"  LoD symbols:  {len(lod_syms)}")

    # Compute CV64 function sizes
    cv64_sized = compute_cv64_sizes(cv64_syms)

    # Filter to engine functions
    cv64_engine = [(n, v, s) for n, v, s in cv64_sized
                   if is_engine_func(n) and s >= args.min_size]
    print(f"  CV64 engine functions to match: {len(cv64_engine)}")

    # Build LoD fingerprint index: size → [(name, vram, fingerprint)]
    print(f"\nBuilding LoD fingerprint index...")
    lod_by_size = {}
    for name, vram, size in lod_syms:
        if size < args.min_size:
            continue
        rom_off = vram_to_rom_lod(vram)
        if rom_off < 0 or rom_off + size > len(lod_data):
            continue
        fp = fingerprint_function(lod_data, rom_off, size)
        if size not in lod_by_size:
            lod_by_size[size] = []
        lod_by_size[size].append((name, vram, fp))

    # Also index nearby sizes (±8 bytes) for fuzzy size matching
    # Functions can differ slightly in size due to alignment padding

    # Match CV64 functions against LoD
    print(f"\nMatching...")
    matches = []
    no_match = []

    for cv_name, cv_vram, cv_size in cv64_engine:
        cv_rom = vram_to_rom_cv64(cv_vram)
        if cv_rom < 0 or cv_rom + cv_size > len(cv64_data):
            continue

        cv_fp = fingerprint_function(cv64_data, cv_rom, cv_size)
        if not cv_fp:
            continue

        best_match = None
        best_ratio = 0

        # Search LoD functions with similar size (±20%)
        size_lo = int(cv_size * 0.8)
        size_hi = int(cv_size * 1.2)

        candidates = []
        for sz in range(size_lo, size_hi + 4, 4):
            if sz in lod_by_size:
                candidates.extend(lod_by_size[sz])

        for lod_name, lod_vram, lod_fp in candidates:
            # Compare fingerprints
            min_len = min(len(cv_fp), len(lod_fp))
            if min_len == 0:
                continue

            matching = sum(1 for a, b in zip(cv_fp[:min_len], lod_fp[:min_len]) if a == b)
            ratio = matching / max(len(cv_fp), len(lod_fp))

            if ratio > best_ratio:
                best_ratio = ratio
                best_match = (lod_name, lod_vram, ratio)

        if best_match and best_ratio >= args.min_match:
            lod_name, lod_vram, ratio = best_match
            already_named = not lod_name.startswith('func_') and not lod_name.startswith('map_ovl_')
            if already_named and not args.show_existing:
                continue
            confidence = "HIGH" if ratio >= 0.95 else "MEDIUM" if ratio >= 0.90 else "LOW"
            matches.append((cv_name, cv_vram, cv_size, lod_name, lod_vram, ratio, confidence, already_named))
        else:
            no_match.append((cv_name, cv_vram, cv_size, best_ratio))

    # Print results
    print(f"\n{'='*90}")
    print(f" MATCHES ({len(matches)} found)")
    print(f"{'='*90}")

    new_matches = [m for m in matches if not m[7]]
    existing_matches = [m for m in matches if m[7]]

    if new_matches:
        print(f"\n--- NEW names to apply ({len(new_matches)}) ---")
        print(f"{'CV64 Name':<45} {'LoD Current':<25} {'LoD VRAM':<12} {'Match':>5} {'Conf'}")
        print(f"{'-'*45} {'-'*25} {'-'*12} {'-'*5} {'-'*6}")
        for cv_name, cv_vram, cv_size, lod_name, lod_vram, ratio, conf, _ in sorted(new_matches, key=lambda x: -x[5]):
            print(f"{cv_name:<45} {lod_name:<25} 0x{lod_vram:08X}  {ratio:4.0%}  {conf}")

    if existing_matches:
        print(f"\n--- Already named (verification, {len(existing_matches)}) ---")
        print(f"{'CV64 Name':<45} {'LoD Current':<25} {'LoD VRAM':<12} {'Match':>5}")
        print(f"{'-'*45} {'-'*25} {'-'*12} {'-'*5}")
        for cv_name, cv_vram, cv_size, lod_name, lod_vram, ratio, conf, _ in sorted(existing_matches, key=lambda x: -x[5]):
            same = "OK" if cv_name == lod_name else "DIFF"
            print(f"{cv_name:<45} {lod_name:<25} 0x{lod_vram:08X}  {ratio:4.0%}  {same}")

    if no_match:
        print(f"\n--- No match ({len(no_match)}) ---")
        for cv_name, cv_vram, cv_size, best in sorted(no_match, key=lambda x: -x[3])[:20]:
            print(f"  {cv_name:<45} (best {best:4.0%}, size={cv_size})")

    # Output syms.toml rename commands
    if new_matches:
        print(f"\n{'='*90}")
        print(f" SUGGESTED sed commands for castlevania2.syms.toml:")
        print(f"{'='*90}")
        for cv_name, cv_vram, cv_size, lod_name, lod_vram, ratio, conf, _ in sorted(new_matches, key=lambda x: -x[5]):
            if conf in ("HIGH", "MEDIUM"):
                print(f'sed -i \'\' \'s/name = "{lod_name}"/name = "{cv_name}"/\' castlevania2.syms.toml')


if __name__ == '__main__':
    main()
