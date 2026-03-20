#!/usr/bin/env python3
"""
Step 4: Identify function boundaries within .text regions.
Finds function starts (prologue: addiu sp, sp, -N) and ends (jr $ra + delay slot).
Handles leaf functions and NOP padding.

Usage: python3 tools/ni_ovl/step4_functions.py
"""

import sys, json, struct
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
JR_RA = 0x03E00008
NOP = 0x00000000
VRAM_BASE = 0x0F000000

def find_functions(data, text_size):
    """Find all function boundaries in the .text region."""
    functions = []
    i = 0

    while i < text_size:
        word = struct.unpack_from('>I', data, i)[0]

        # Skip NOP padding between functions
        if word == NOP:
            i += 4
            continue

        # This is the start of a function
        func_start = i

        # Scan forward to find the end: jr $ra + delay slot
        # Some functions may have multiple jr $ra (early returns), so we need
        # to find the one that's actually the end. Strategy: find jr $ra and
        # check if after the delay slot we hit a new function prologue or NOP padding.
        func_end = None
        j = i
        while j < text_size - 4:
            w = struct.unpack_from('>I', data, j)[0]
            if w == JR_RA:
                # Found jr $ra at j, delay slot at j+4
                candidate_end = j + 8  # past delay slot
                # Check what comes next
                if candidate_end >= text_size:
                    func_end = candidate_end
                    break
                next_word = struct.unpack_from('>I', data, candidate_end)[0]
                # Next is NOP padding, new function prologue, or another valid start
                if (next_word == NOP or
                    (next_word >> 16) == 0x27BD or  # addiu sp, sp, -N
                    next_word == JR_RA or            # leaf function: jr $ra
                    (next_word >> 26) in (0x0F, 0x3C, 0x24)):  # lui, addiu, common starts
                    func_end = candidate_end
                    break
            j += 4

        if func_end is None:
            # Couldn't find clean end - take everything to text_size
            func_end = text_size

        func_size = func_end - func_start
        if func_size < 4:
            i += 4
            continue

        vram = VRAM_BASE + func_start
        functions.append({
            'offset': func_start,
            'size': func_size,
            'vram': vram,
        })
        i = func_end

    return functions

def validate_functions(data, text_size, functions):
    """Verify functions tile the .text region (no gaps except NOP padding)."""
    covered = [False] * text_size
    for f in functions:
        for j in range(f['offset'], f['offset'] + f['size']):
            if j < text_size:
                covered[j] = True

    # Check uncovered bytes - should only be NOP padding
    uncovered = []
    for i in range(0, text_size, 4):
        if not covered[i] and i + 3 < text_size:
            word = struct.unpack_from('>I', data, i)[0]
            if word != NOP:
                uncovered.append((i, word))

    return uncovered

def main():
    boundaries_path = SCRIPT_DIR / 'out' / 'boundaries.json'
    with open(boundaries_path) as f:
        boundaries = json.load(f)

    raw_dir = SCRIPT_DIR / 'out' / 'raw'
    all_functions = []
    total_funcs = 0
    total_issues = 0

    for b in boundaries:
        pair_idx = b['pair_index']
        text_size = b['text_size']
        bin_path = raw_dir / f'ni_ovl_{pair_idx:03d}.bin'
        with open(bin_path, 'rb') as f:
            data = f.read()

        functions = find_functions(data, text_size)
        total_funcs += len(functions)

        uncovered = validate_functions(data, text_size, functions)
        if uncovered:
            total_issues += len(uncovered)
            if len(uncovered) <= 3:
                for off, word in uncovered:
                    print(f"  pair {pair_idx}: uncovered non-NOP at 0x{off:X} = 0x{word:08X}")

        # Check alignment
        for f in functions:
            if f['offset'] % 4 != 0:
                print(f"  pair {pair_idx}: misaligned function at 0x{f['offset']:X}")
                total_issues += 1

        all_functions.append({
            'pair_index': pair_idx,
            'text_ni_index': b['text_ni_index'],
            'data_ni_index': b['data_ni_index'],
            'text_size': text_size,
            'data_size': b['data_size'],
            'total_size': b['total_size'],
            'num_functions': len(functions),
            'functions': functions,
        })

    print(f"Function analysis complete:")
    print(f"  Overlays: {len(all_functions)}")
    print(f"  Total functions: {total_funcs}")
    print(f"  Validation issues: {total_issues}")

    func_counts = [a['num_functions'] for a in all_functions]
    print(f"  Functions per overlay: min={min(func_counts)}, max={max(func_counts)}, "
          f"avg={sum(func_counts)/len(func_counts):.1f}")

    out_path = SCRIPT_DIR / 'out' / 'functions.json'
    with open(out_path, 'w') as f:
        json.dump(all_functions, f, indent=2)

    print(f"\nSaved to {out_path}")

if __name__ == '__main__':
    main()
