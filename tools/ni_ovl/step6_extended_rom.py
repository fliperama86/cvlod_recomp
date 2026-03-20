#!/usr/bin/env python3
"""
Step 6: Build extended ROM.
Appends decompressed .text sections to the ROM at offset 0x1000000 (16MB).
N64Recomp reads raw bytes from rom_file_path at section.rom_addr + func.offset.

Usage: python3 tools/ni_ovl/step6_extended_rom.py resources/castlevania_legacy_of_darkness.z64
"""

import sys, json, struct
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
EXTENDED_OFFSET = 0x1000000  # 16 MB mark

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <rom.z64>", file=sys.stderr)
        sys.exit(1)

    rom_path = sys.argv[1]
    with open(rom_path, 'rb') as f:
        rom = bytearray(f.read())

    functions_path = SCRIPT_DIR / 'out' / 'functions.json'
    with open(functions_path) as f:
        all_functions = json.load(f)

    raw_dir = SCRIPT_DIR / 'out' / 'raw'

    # Pad ROM to 16 MB if needed
    if len(rom) < EXTENDED_OFFSET:
        rom.extend(b'\x00' * (EXTENDED_OFFSET - len(rom)))
    elif len(rom) > EXTENDED_OFFSET:
        print(f"WARNING: ROM is already {len(rom)} bytes (> 16 MB), truncating extension area")
        rom = rom[:EXTENDED_OFFSET]

    rom_offsets = []
    cursor = EXTENDED_OFFSET

    for ovl in all_functions:
        pair_idx = ovl['pair_index']
        text_size = ovl['text_size']

        bin_path = raw_dir / f'ni_ovl_{pair_idx:03d}.bin'
        with open(bin_path, 'rb') as f:
            data = f.read()

        # Extract .text section only
        text_data = data[:text_size]

        rom_offset = cursor

        # Append to ROM
        rom.extend(text_data)

        # Pad to 16-byte alignment
        pad = (16 - (len(rom) % 16)) % 16
        rom.extend(b'\x00' * pad)

        rom_offsets.append({
            'pair_index': pair_idx,
            'text_ni_index': ovl['text_ni_index'],
            'rom_offset': rom_offset,
            'text_size': text_size,
        })

        cursor = len(rom)

    # Verify first 16MB is unchanged
    with open(rom_path, 'rb') as f:
        original_rom = f.read()
    orig_size = min(len(original_rom), EXTENDED_OFFSET)
    assert rom[:orig_size] == bytearray(original_rom[:orig_size]), "First 16MB modified!"

    # Write extended ROM
    out_rom_path = Path('resources/castlevania2_ni_extended.z64')
    with open(out_rom_path, 'wb') as f:
        f.write(rom)

    # Save ROM offsets
    offsets_path = SCRIPT_DIR / 'out' / 'rom_offsets.json'
    with open(offsets_path, 'w') as f:
        json.dump(rom_offsets, f, indent=2)

    print(f"Extended ROM: {out_rom_path} ({len(rom)} bytes, {len(rom)/1024/1024:.1f} MB)")
    print(f"  Original: {orig_size} bytes")
    print(f"  Appended: {len(rom) - EXTENDED_OFFSET} bytes ({len(rom_offsets)} .text sections)")
    print(f"  Offset range: 0x{EXTENDED_OFFSET:X} - 0x{len(rom):X}")
    print(f"\nSaved offsets to {offsets_path}")

if __name__ == '__main__':
    main()
