#!/usr/bin/env python3
"""
Decompress Nisitenma-Ichigo compressed files from LoD ROM.
Fault-tolerant: skips files that fail to decompress.
Dumps individual decompressed files for analysis.

Based on cv64_decomp/tools/decompress.py by @Fluvian / @LiquidCat64.
"""

import os
import sys
import struct

from liblzkn64 import (
    MAX_LZKN64_FILE_SIZE,
    NISITENMA_ICHIGO_HEADER,
    decompress_buffer,
    find_nisitenma_ichigo_offset,
)


def parse_table(rom: bytes, table_addr: int):
    """Parse the file table, yielding (index, flag, rom_addr, compressed_size) tuples."""
    pos = 0
    idx = 0
    while True:
        entry = rom[table_addr + pos : table_addr + pos + 4]
        if len(entry) < 4:
            break
        flag = entry[0]
        addr = int.from_bytes(entry[1:4], "big")
        if addr == 0:
            break

        # For compressed files (flag & 0x80), size comes from the file header
        if flag & 0x80:
            comp_size = int.from_bytes(rom[addr + 1 : addr + 4], "big")
        else:
            # Uncompressed — size is distance to next entry's address
            next_entry = rom[table_addr + pos + 4 : table_addr + pos + 8]
            if len(next_entry) >= 4:
                next_addr = int.from_bytes(next_entry[1:4], "big")
                comp_size = next_addr - addr if next_addr > addr else 0
            else:
                comp_size = 0

        yield idx, flag, addr, comp_size
        idx += 1
        pos += 4


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <rom_file> <output_dir>", file=sys.stderr)
        sys.exit(1)

    rom_path = sys.argv[1]
    out_dir = sys.argv[2]
    os.makedirs(out_dir, exist_ok=True)

    with open(rom_path, "rb") as f:
        rom = f.read()

    table_addr = find_nisitenma_ichigo_offset(rom)
    if table_addr is None:
        print("No Nisitenma-Ichigo table found!", file=sys.stderr)
        sys.exit(1)
    print(f"Table at ROM offset 0x{table_addr:08X}")

    stats = {"total": 0, "compressed": 0, "decompressed_ok": 0, "failed": 0, "skipped": 0}
    code_files = []

    for idx, flag, addr, size in parse_table(rom, table_addr):
        stats["total"] += 1

        if size == 0:
            stats["skipped"] += 1
            continue

        is_compressed = bool(flag & 0x80)

        if not is_compressed:
            # Uncompressed file — just extract raw bytes
            raw = rom[addr : addr + size]
            out_path = os.path.join(out_dir, f"file_{idx:04d}_0x{addr:08X}_raw.bin")
            with open(out_path, "wb") as f:
                f.write(raw)
            stats["skipped"] += 1
            continue

        stats["compressed"] += 1

        # Attempt decompression
        file_buf = bytearray(rom[addr : addr + size])
        # Pad to MAX_LZKN64_FILE_SIZE to avoid index errors in decompressor
        if len(file_buf) < MAX_LZKN64_FILE_SIZE:
            file_buf.extend(b"\x00" * (MAX_LZKN64_FILE_SIZE - len(file_buf)))

        try:
            raw = bytes(decompress_buffer(file_buf))
        except Exception as e:
            print(f"  [{idx:4d}] FAIL  0x{addr:08X} size=0x{size:X}: {e}")
            stats["failed"] += 1
            continue

        stats["decompressed_ok"] += 1
        dec_size = len(raw)

        # Check if decompressed data looks like MIPS code (starts with common prologue patterns)
        has_code = False
        if dec_size >= 8:
            # Check for addiu sp, sp, -N (0x27BDxxxx) or lui (0x3Cxxxxxx)
            first_word = struct.unpack(">I", raw[0:4])[0]
            if (first_word >> 16) == 0x27BD or (first_word >> 26) == 0x0F:
                has_code = True

        tag = "CODE" if has_code else "data"
        out_path = os.path.join(out_dir, f"file_{idx:04d}_0x{addr:08X}_{tag}.bin")
        with open(out_path, "wb") as f:
            f.write(raw)

        if has_code:
            code_files.append((idx, addr, dec_size))

        print(f"  [{idx:4d}] OK    0x{addr:08X} -> {dec_size:8d} bytes  [{tag}]")

    print(f"\nStats: {stats}")
    if code_files:
        print(f"\nFiles containing code ({len(code_files)}):")
        for idx, addr, size in code_files:
            print(f"  file_{idx:04d} @ ROM 0x{addr:08X}, decompressed {size} bytes")


if __name__ == "__main__":
    main()
