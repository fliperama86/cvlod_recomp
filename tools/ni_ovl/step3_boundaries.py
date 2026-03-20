#!/usr/bin/env python3
"""
Step 3: Find .text/.data boundaries in decompressed NI overlays.
Scans for the last `jr $ra` (0x03E00008) + delay slot, aligns to 16 bytes.

Usage: python3 tools/ni_ovl/step3_boundaries.py
"""

import sys, json, struct
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
JR_RA = 0x03E00008

def find_text_data_boundary(data):
    """Find the boundary between .text and .data sections.

    Strategy: scan backwards from end to find the last jr $ra + delay slot.
    The .text section ends after the delay slot, aligned to 16 bytes.
    """
    if len(data) < 8:
        return len(data)

    # Find the last jr $ra in the data
    last_jr_ra = -1
    for i in range(len(data) - 8, -1, -4):
        word = struct.unpack_from('>I', data, i)[0]
        if word == JR_RA:
            last_jr_ra = i
            break

    if last_jr_ra < 0:
        # No jr $ra found - entire blob might be data-only or a single leaf function
        # Check if it looks like MIPS code throughout
        return len(data)

    # .text ends after the delay slot (jr $ra + 4 bytes delay slot)
    text_end = last_jr_ra + 8  # jr_ra (4) + delay slot (4)

    # Align to 16 bytes
    text_end_aligned = (text_end + 15) & ~15

    # Clamp to data size
    if text_end_aligned > len(data):
        text_end_aligned = len(data)

    return text_end_aligned

def validate_text_region(data, text_size):
    """Check that the .text region contains valid MIPS code."""
    if text_size == 0:
        return False, "empty .text"

    # Check first word is valid MIPS
    first_word = struct.unpack_from('>I', data, 0)[0]
    opcode = (first_word >> 26) & 0x3F

    # Count jr $ra instructions in .text region
    jr_count = 0
    for i in range(0, text_size - 3, 4):
        word = struct.unpack_from('>I', data, i)[0]
        if word == JR_RA:
            jr_count += 1

    if jr_count == 0:
        return False, "no jr $ra in .text"

    return True, f"{jr_count} functions"

def validate_data_region(data, text_size):
    """Check that .data region doesn't contain function prologues."""
    data_region = data[text_size:]
    if len(data_region) == 0:
        return True, "no .data"

    # Count prologues in data region (should be 0 or very few)
    prologue_count = 0
    for i in range(0, len(data_region) - 3, 4):
        word = struct.unpack_from('>I', data_region, i)[0]
        if (word >> 16) == 0x27BD:
            prologue_count += 1

    if prologue_count > 2:
        return False, f"{prologue_count} prologues in .data"

    return True, f"{len(data_region)} bytes"

def main():
    info_path = SCRIPT_DIR / 'out' / 'decomp_info.json'
    with open(info_path) as f:
        decomp_info = json.load(f)

    raw_dir = SCRIPT_DIR / 'out' / 'raw'
    boundaries = []
    issues = []

    for d in decomp_info:
        pair_idx = d['pair_index']
        bin_path = raw_dir / f'ni_ovl_{pair_idx:03d}.bin'
        with open(bin_path, 'rb') as f:
            data = f.read()

        text_size = find_text_data_boundary(data)
        data_size = len(data) - text_size

        text_ok, text_msg = validate_text_region(data, text_size)
        data_ok, data_msg = validate_data_region(data, text_size)

        entry = {
            'pair_index': pair_idx,
            'text_ni_index': d['text_ni_index'],
            'data_ni_index': d['data_ni_index'],
            'total_size': len(data),
            'text_size': text_size,
            'data_size': data_size,
            'text_valid': text_ok,
            'data_valid': data_ok,
        }
        boundaries.append(entry)

        if not text_ok or not data_ok:
            issues.append(f"pair {pair_idx}: text={text_msg}, data={data_msg}")

    # Stats
    total_text = sum(b['text_size'] for b in boundaries)
    total_data = sum(b['data_size'] for b in boundaries)
    text_only = sum(1 for b in boundaries if b['data_size'] == 0)

    print(f"Boundaries found: {len(boundaries)}")
    print(f"  Text-only (no .data): {text_only}")
    print(f"  Total .text: {total_text} bytes ({total_text/1024/1024:.1f} MB)")
    print(f"  Total .data: {total_data} bytes ({total_data/1024:.0f} KB)")

    if issues:
        print(f"\nIssues ({len(issues)}):")
        for iss in issues:
            print(f"  {iss}")
    else:
        print("\nAll boundaries validated OK")

    # Show some examples
    print(f"\nFirst 5 overlays:")
    for b in boundaries[:5]:
        print(f"  pair {b['pair_index']:3d}: total={b['total_size']:6d}  "
              f".text={b['text_size']:6d}  .data={b['data_size']:6d}")

    out_path = SCRIPT_DIR / 'out' / 'boundaries.json'
    with open(out_path, 'w') as f:
        json.dump(boundaries, f, indent=2)

    print(f"\nSaved to {out_path}")

if __name__ == '__main__':
    main()
