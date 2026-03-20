#!/usr/bin/env python3
"""
Step 2: Decompress code overlays from ROM.
Uses table.json to find code pairs, decompresses each unique overlay.
All LoD NI entries use zlib with a 4-byte header (byte 0 = flag, bytes 1-3 = compressed size).

Usage: python3 tools/ni_ovl/step2_decompress.py resources/castlevania_legacy_of_darkness.z64
"""

import sys, json, struct, zlib
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent

def decompress_ni(rom, offset):
    """Decompress NI entry at offset. Format: 4-byte header + zlib stream."""
    if offset + 4 >= len(rom):
        return None
    try:
        # Use streaming decompressor to handle trailing data
        d = zlib.decompressobj()
        result = d.decompress(rom[offset + 4:offset + 4 + 512 * 1024])
        result += d.flush()
        if len(result) > 0:
            return result
    except Exception:
        pass
    return None

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <rom.z64>", file=sys.stderr)
        sys.exit(1)

    rom_path = sys.argv[1]
    with open(rom_path, 'rb') as f:
        rom = f.read()

    table_path = SCRIPT_DIR / 'out' / 'table.json'
    with open(table_path) as f:
        table = json.load(f)

    pairs = table['pairs']
    out_dir = SCRIPT_DIR / 'out' / 'raw'
    out_dir.mkdir(parents=True, exist_ok=True)

    stats = {'failed': 0}
    decomp_info = []

    for i, pair in enumerate(pairs):
        rom_offset = pair['rom_offset']
        text_idx = pair['text_index']

        data = decompress_ni(rom, rom_offset)
        if data is None:
            print(f"  FAIL: pair {i} (text=0x{text_idx:X}) at ROM 0x{rom_offset:06X}")
            stats['failed'] += 1
            continue

        # Check if first bytes look like MIPS prologue (addiu sp, sp, -N = 0x27BDxxxx)
        first_word = struct.unpack_from('>I', data, 0)[0] if len(data) >= 4 else 0
        is_mips_prologue = (first_word >> 16) == 0x27BD

        out_path = out_dir / f'ni_ovl_{i:03d}.bin'
        with open(out_path, 'wb') as f:
            f.write(data)

        decomp_info.append({
            'pair_index': i,
            'text_ni_index': text_idx,
            'data_ni_index': pair['data_index'],
            'rom_offset': rom_offset,
            'decomp_size': len(data),
            'method': 'zlib',
            'first_word': f'0x{first_word:08X}',
            'is_mips_prologue': is_mips_prologue,
        })

        if i < 3 or not is_mips_prologue:
            print(f"  pair {i}: text=0x{text_idx:X} rom=0x{rom_offset:06X} "
                  f"size={len(data)} first=0x{first_word:08X} {'MIPS' if is_mips_prologue else 'NOT-MIPS'}")

    print(f"\nDecompressed: {len(decomp_info)} overlays")
    print(f"  Failed: {stats['failed']}")

    mips_count = sum(1 for d in decomp_info if d['is_mips_prologue'])
    print(f"  MIPS prologues: {mips_count}/{len(decomp_info)}")

    sizes = [d['decomp_size'] for d in decomp_info]
    if sizes:
        print(f"  Size range: {min(sizes)} - {max(sizes)} bytes")
        print(f"  Total: {sum(sizes)} bytes ({sum(sizes)/1024/1024:.1f} MB)")

    info_path = SCRIPT_DIR / 'out' / 'decomp_info.json'
    with open(info_path, 'w') as f:
        json.dump(decomp_info, f, indent=2)

    print(f"\nSaved to {out_dir}/ and {info_path}")

if __name__ == '__main__':
    main()
