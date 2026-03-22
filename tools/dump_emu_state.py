#!/usr/bin/env python3
"""
Dump key memory regions from a Mupen64Plus savestate for comparison
with the recomp's RDRAM state.

Usage:
  1. Run: mupen64plus resources/castlevania_legacy_of_darkness.z64
  2. Play through save screen (press A/Start), Konami logo, KCEK logo
  3. When you reach the expansion pak screen, press F5 to save state
  4. Close the emulator
  5. Run: python3 tools/dump_emu_state.py <savestate_path>
     (savestates are usually in ~/.local/share/mupen64plus/save/)

The script extracts RDRAM and dumps:
  - Gamestate manager state (current gs, transition state)
  - overlay_system data section (function pointer table at 0x801D1880)
  - NI file_ptr_array (0x801C8830)
  - exec_flags and ni_sys_ptr
  - Object tree root state
"""

import sys
import struct
import gzip
import os

def find_rdram_in_savestate(data):
    """Mupen64Plus savestates are gzipped. RDRAM is at a known offset."""
    # Mupen64Plus savestate format:
    # The RDRAM (8MB) starts after headers. Let's search for it.
    # RDRAM is 0x800000 bytes (8MB). The savestate has it at a fixed offset.

    # Try to find RDRAM by looking for known N64 boot signatures
    # The game's entrypoint at 0x80000400 should have known code

    # Actually, mupen64plus savestates have a header then raw RDRAM at offset 0x275C
    # But this varies by version. Let's try common offsets.

    # For mupen64plus 2.5+, savestate format is:
    # [header][RDRAM 8MB][TLB][registers][...]

    # The header magic is "M64+" followed by version info
    if data[:4] == b'M64+':
        print(f"  Mupen64Plus savestate detected")
        # Version 1.0 header is 44 bytes, then RDRAM
        # But it varies. Let's search for a known pattern.

    # Search for the game's entrypoint code signature
    # At RDRAM offset 0x400 (VRAM 0x80000400), the entrypoint should be:
    # 3C0A8000 254A77E0 01400008 27BD3680
    target = struct.pack(">I", 0x3C0A8000)  # lui $t2, 0x8000

    for offset in range(0, min(len(data), 0x100000), 4):
        if data[offset:offset+4] == target:
            # Check next word too
            next_word = struct.unpack(">I", data[offset+4:offset+8])[0]
            if next_word == 0x254A77E0:  # addiu $t2, $t2, 0x77E0
                rdram_base = offset - 0x400  # RDRAM starts 0x400 bytes before entrypoint
                print(f"  Found entrypoint signature at offset 0x{offset:X}")
                print(f"  RDRAM base in savestate: 0x{rdram_base:X}")
                return rdram_base

    print("  WARNING: Could not find entrypoint signature. Trying offset 0x275C...")
    return 0x275C  # Common default


def dump_memory_regions(rdram, rdram_base, data):
    """Dump key memory regions from RDRAM."""

    def read_u32(vram):
        """Read big-endian uint32 from VRAM address."""
        offset = rdram_base + (vram - 0x80000000)
        if offset + 4 > len(data):
            return 0
        return struct.unpack(">I", data[offset:offset+4])[0]

    def dump_region(name, vram, size):
        print(f"\n=== {name} (VRAM 0x{vram:08X}, {size} bytes) ===")
        offset = rdram_base + (vram - 0x80000000)
        for i in range(0, min(size, 256), 16):
            words = []
            for j in range(0, 16, 4):
                if offset + i + j + 4 <= len(data):
                    w = struct.unpack(">I", data[offset+i+j:offset+i+j+4])[0]
                    words.append(f"{w:08X}")
                else:
                    words.append("????????")
            print(f"  {vram+i:08X}: {' '.join(words)}")

    # Current gamestate
    gsm_ptr = read_u32(0x800C1520)
    print(f"\n=== Gamestate Manager ===")
    print(f"  GSM pointer: 0x{gsm_ptr:08X}")
    if gsm_ptr != 0:
        gsm_phys = gsm_ptr & 0x1FFFFFFF
        cur_gs = struct.unpack(">i", data[rdram_base+gsm_phys+0x24:rdram_base+gsm_phys+0x28])[0]
        state_byte = data[rdram_base + gsm_phys + 0x09]
        print(f"  Current gamestate (obj+0x24): {cur_gs}")
        print(f"  State byte (obj+0x09): {state_byte}")
        dump_region("GSM object", gsm_ptr, 128)

    # exec_flags and ni_sys_ptr
    exec_flags = read_u32(0x801CABC8)
    ni_sys_ptr = read_u32(0x801CAC1C)
    print(f"\n=== System State ===")
    print(f"  exec_flags (sys+0x2908): 0x{exec_flags:08X}")
    print(f"  ni_sys_ptr (sys+0x295C): 0x{ni_sys_ptr:08X}")

    # Function pointer table at 0x801D1880 (overlay_system data)
    dump_region("overlay_system func ptr table", 0x801D1880, 64)

    # overlay_system data section start (TEXT+0x69E0 through TEXT+0x7AE0)
    dump_region("overlay_system text tail (jump tables)", 0x801D1880, 128)

    # overlay_system DATA section (at TEXT+0x7AE0 = 0x801CAEA0+0x7AE0 = 0x801D2980)
    dump_region("overlay_system DATA section", 0x801D2980, 128)

    # NI file_ptr_array (0x801C8830)
    print(f"\n=== NI file_ptr_array (0x801C8830) ===")
    non_zero = 0
    for i in range(472):
        val = read_u32(0x801C8830 + i * 4)
        if val != 0:
            non_zero += 1
            if non_zero <= 10:
                print(f"  [{i:3d}] = 0x{val:08X}")
    print(f"  Total non-zero entries: {non_zero}/472")

    # NI loaded bitmask (0x801C83EC)
    print(f"\n=== NI loaded bitmask (0x801C83EC) ===")
    for i in range(16):
        val = read_u32(0x801C83EC + i * 4)
        if val != 0:
            print(f"  word[{i:2d}] = 0x{val:08X} (files {i*32}-{i*32+31})")

    # TLB-mapped overlay regions
    dump_region("0x0F000000 segment (first 64 bytes)", 0x0F000000, 64)
    dump_region("0x0E000000 segment (first 64 bytes)", 0x0E000000, 64)

    # Object tree root
    dump_region("Object tree root (0x8031AC78)", 0x8031AC78, 128)

    # Gamestate table entries for gs=5, gs=6, gs=12
    for gs_id in [5, 6, 12]:
        entry_vram = 0x800AD020 + gs_id * 80
        dump_region(f"Gamestate {gs_id} table entry", entry_vram - 0x50, 80)


def main():
    if len(sys.argv) < 2:
        # Try to find savestates automatically
        save_dirs = [
            os.path.expanduser("~/.local/share/mupen64plus/save/"),
            os.path.expanduser("~/Library/Application Support/mupen64plus/save/"),
        ]
        print("Usage: python3 tools/dump_emu_state.py <savestate_path>")
        print("\nLooking for savestates...")
        for d in save_dirs:
            if os.path.isdir(d):
                files = [f for f in os.listdir(d) if 'castlevania' in f.lower() or f.endswith('.st0') or f.endswith('.savestate')]
                if files:
                    print(f"  Found in {d}: {files}")
        return

    path = sys.argv[1]
    print(f"Loading savestate: {path}")

    # Try gzip first (mupen64plus uses gzip)
    try:
        with gzip.open(path, 'rb') as f:
            data = f.read()
        print(f"  Decompressed: {len(data)} bytes")
    except:
        with open(path, 'rb') as f:
            data = f.read()
        print(f"  Raw: {len(data)} bytes")

    rdram_base = find_rdram_in_savestate(data)

    # Verify RDRAM is readable
    total_size = len(data) - rdram_base
    print(f"  Available RDRAM: {total_size} bytes ({total_size / 1024 / 1024:.1f} MB)")

    if total_size < 0x400000:
        print("ERROR: Not enough data for 4MB RDRAM. Wrong offset?")
        return

    dump_memory_regions(None, rdram_base, data)
    print("\nDone. Compare these values against the recomp's runtime state.")


if __name__ == "__main__":
    main()
