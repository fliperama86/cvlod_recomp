#!/usr/bin/env python3
"""
Fix invalid identifiers in recompiled code.

Issues fixed:
1. .L* labels are invalid C identifiers - rename to L_*
2. main() function conflicts with C's main - rename to game_main
"""

import re
import sys
from pathlib import Path

def fix_file(file_path: Path) -> tuple[int, int]:
    """Fix a single file. Returns (dot_l_fixes, main_fixes)."""
    with open(file_path, 'r') as f:
        content = f.read()

    original = content

    # Replace .L with L_ for function names (e.g., .L8008A040 -> L_8008A040)
    # This handles declarations, definitions, and calls
    content = re.sub(r'\.L([0-9A-Fa-f]{8})', r'L_\1', content)

    # Rename main function to game_main to avoid conflict with C's main
    content = re.sub(r'\bmain\s*\(uint8_t\* rdram', r'game_main(uint8_t* rdram', content)

    dot_l_fixes = len(re.findall(r'L_[0-9A-Fa-f]{8}', content)) - len(re.findall(r'L_[0-9A-Fa-f]{8}', original))
    main_fixes = content.count('game_main') - original.count('game_main')

    if content != original:
        with open(file_path, 'w') as f:
            f.write(content)

    return (abs(dot_l_fixes) if dot_l_fixes else 0, main_fixes)

def main():
    project_root = Path(__file__).parent.parent
    funcs_dir = project_root / "recompiled" / "functions"

    if not funcs_dir.exists():
        print(f"Error: {funcs_dir} not found")
        return 1

    print("Fixing recompiled code...")

    total_dot_l = 0
    total_main = 0

    # Fix header
    header_path = funcs_dir / "funcs.h"
    if header_path.exists():
        dot_l, main_fix = fix_file(header_path)
        total_dot_l += dot_l
        total_main += main_fix
        print(f"  funcs.h: {dot_l} .L fixes, {main_fix} main fixes")

    # Fix C files
    for c_file in sorted(funcs_dir.glob("*.c")):
        dot_l, main_fix = fix_file(c_file)
        if dot_l or main_fix:
            total_dot_l += dot_l
            total_main += main_fix
            print(f"  {c_file.name}: {dot_l} .L fixes, {main_fix} main fixes")

    print(f"\nTotal: {total_dot_l} .L identifier fixes, {total_main} main renames")
    print("Done!")
    return 0

if __name__ == "__main__":
    sys.exit(main())
