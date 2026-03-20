#!/usr/bin/env python3
"""
Step 1: Dump NI table from ROM.
Parse the Nisitenma-Ichigo table, record every entry with index, ROM offset,
flag bit, and identify code pairs.

Usage: python3 tools/ni_ovl/step1_dump_table.py resources/castlevania_legacy_of_darkness.z64
"""

import sys, struct, json
from pathlib import Path

NI_TABLE_OFFSET = 0xB1B98  # After 16-byte "Nisitenma-Ichigo" magic

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <rom.z64>", file=sys.stderr)
        sys.exit(1)

    rom_path = sys.argv[1]
    with open(rom_path, 'rb') as f:
        rom = f.read()

    entries = []
    pos = NI_TABLE_OFFSET
    idx = 0

    while pos + 3 < len(rom):
        raw = struct.unpack_from('>I', rom, pos)[0]
        if raw == 0:
            break

        flag_byte = (raw >> 24) & 0xFF
        rom_offset_24 = raw & 0x00FFFFFF
        high_bit = (raw >> 31) & 1

        entries.append({
            'index': idx,
            'raw': raw,
            'flag_byte': flag_byte,
            'high_bit': high_bit,
            'rom_offset_24': rom_offset_24,
            'rom_offset_full': raw,  # full 32-bit as used by extract_ni.py/lod_init.cpp
        })

        idx += 1
        pos += 4

    print(f"Total NI entries: {len(entries)}")

    # Identify code overlay range (indices 0x1C5-0x3AE per plan)
    # But let's also discover this empirically by looking at flag patterns
    code_range_start = 0x1C5
    code_range_end = 0x3AE

    # Analyze flag byte distribution
    flag_counts = {}
    for e in entries:
        fb = e['flag_byte']
        flag_counts[fb] = flag_counts.get(fb, 0) + 1

    print(f"\nFlag byte distribution:")
    for fb in sorted(flag_counts.keys()):
        print(f"  0x{fb:02X}: {flag_counts[fb]} entries")

    # Identify code pairs in the code overlay range
    # Pairs: consecutive entries where one has high_bit=0, next has high_bit=1,
    # and they share the same lower 24-bit offset
    code_entries = []
    pairs = []
    i = code_range_start
    while i < min(code_range_end + 1, len(entries)):
        e = entries[i]
        # Check if this forms a pair with the next entry
        if i + 1 <= code_range_end and i + 1 < len(entries):
            e_next = entries[i + 1]
            if e['rom_offset_24'] == e_next['rom_offset_24'] and e['high_bit'] != e_next['high_bit']:
                text_entry = e if e['high_bit'] == 0 else e_next
                data_entry = e_next if e['high_bit'] == 0 else e
                pairs.append({
                    'text_index': text_entry['index'],
                    'data_index': data_entry['index'],
                    'rom_offset': text_entry['rom_offset_24'],
                    'flag_text': text_entry['flag_byte'],
                    'flag_data': data_entry['flag_byte'],
                })
                code_entries.append(text_entry['index'])
                code_entries.append(data_entry['index'])
                i += 2
                continue
        code_entries.append(e['index'])
        i += 1

    print(f"\nCode overlay range: 0x{code_range_start:X} - 0x{code_range_end:X}")
    print(f"Code overlay pairs found: {len(pairs)}")
    if pairs:
        print(f"  First pair: text=0x{pairs[0]['text_index']:X} data=0x{pairs[0]['data_index']:X} rom=0x{pairs[0]['rom_offset']:06X}")
        print(f"  Last pair:  text=0x{pairs[-1]['text_index']:X} data=0x{pairs[-1]['data_index']:X} rom=0x{pairs[-1]['rom_offset']:06X}")

    # Also check: are there unpaired entries in the code range?
    paired_indices = set()
    for p in pairs:
        paired_indices.add(p['text_index'])
        paired_indices.add(p['data_index'])

    unpaired = []
    for i in range(code_range_start, min(code_range_end + 1, len(entries))):
        if i not in paired_indices:
            unpaired.append(entries[i])
    if unpaired:
        print(f"\nUnpaired entries in code range: {len(unpaired)}")
        for u in unpaired[:5]:
            print(f"  idx=0x{u['index']:X} raw=0x{u['raw']:08X} flag=0x{u['flag_byte']:02X}")

    # Save results
    out_dir = Path(__file__).parent / 'out'
    out_dir.mkdir(parents=True, exist_ok=True)

    result = {
        'total_entries': len(entries),
        'entries': entries,
        'code_range': {'start': code_range_start, 'end': code_range_end},
        'pairs': pairs,
        'unpaired_in_code_range': [u['index'] for u in unpaired],
    }

    out_path = out_dir / 'table.json'
    with open(out_path, 'w') as f:
        json.dump(result, f, indent=2)

    print(f"\nSaved to {out_path}")

if __name__ == '__main__':
    main()
