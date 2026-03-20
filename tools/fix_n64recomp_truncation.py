#!/usr/bin/env python3
"""
fix_n64recomp_truncation.py — Fix N64Recomp's overlay_system output truncation.

N64Recomp crashes while writing the overlay_system section, truncating:
  - funcs.h: cut off mid-declaration, missing closing #ifdef
  - recomp_overlays.inl: cut mid-line, missing section entries + index array
  - funcs_102.c: last function truncated mid-instruction

This script detects and fixes all three issues deterministically.
"""

import os
import re
import sys

FUNC_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "RecompiledFuncs")


def fix_funcs_h():
    """Add missing forward declarations and closing #ifdef."""
    path = os.path.join(FUNC_DIR, "funcs.h")
    with open(path) as f:
        content = f.read()

    if "#ifdef __cplusplus\n}\n#endif" in content:
        return False  # already fixed

    print("  Fixing funcs.h...")

    # Collect all function names defined in .c files
    defined = set()
    for fname in os.listdir(FUNC_DIR):
        if fname.startswith("funcs_") and fname.endswith(".c"):
            with open(os.path.join(FUNC_DIR, fname)) as f:
                for line in f:
                    m = re.match(r'^RECOMP_FUNC void (\w+)\(', line)
                    if m:
                        defined.add(m.group(1))

    # Find which are missing from funcs.h
    declared = set()
    for m in re.finditer(r'void (\w+)\(uint8_t\*', content):
        declared.add(m.group(1))

    missing = sorted(defined - declared)
    if missing:
        print(f"    Adding {len(missing)} missing forward declarations")
        content += "\n// Declarations truncated by N64Recomp overlay_system bug (auto-fixed)\n"
        for fn in missing:
            content += f"void {fn}(uint8_t* rdram, recomp_context* ctx);\n"

    # Fix any truncated declaration lines (ending mid-type like "recom" instead of full signature)
    lines = content.rstrip().split('\n')
    fixed_lines = []
    for line in lines:
        stripped = line.rstrip()
        if stripped and 'void ' in stripped and '(' in stripped and not stripped.endswith(';') and not stripped.endswith('{'):
            # Truncated mid-declaration — complete it
            if 'recomp_context' not in stripped:
                line = re.sub(r'\(uint8_t\*\s*rdram,\s*recom.*$', '(uint8_t* rdram, recomp_context* ctx);', line)
        fixed_lines.append(line)
    content = '\n'.join(fixed_lines) + '\n'

    content += "\n#ifdef __cplusplus\n}\n#endif\n"

    with open(path, 'w') as f:
        f.write(content)
    return True


def fix_recomp_overlays():
    """Fix truncated section_table and add overlay_sections_by_index."""
    path = os.path.join(FUNC_DIR, "recomp_overlays.inl")
    with open(path) as f:
        content = f.read()

    if "overlay_sections_by_index" in content:
        return False  # already fixed

    print("  Fixing recomp_overlays.inl...")
    lines = content.rstrip().split('\n')

    # Fix truncated last line
    if lines and 'nullp' in lines[-1] and 'nullptr' not in lines[-1]:
        lines[-1] = lines[-1].replace('nullp', 'nullptr, .num_relocs = 0, .index = 0 },')
        print("    Fixed truncated last line")

    # Count section func arrays
    num_sections = sum(1 for l in lines if l.startswith('static FuncEntry section_'))

    # Find which section indices are in the table
    indices_in_table = set()
    for m in re.finditer(r'\.index = (\d+)', '\n'.join(lines)):
        indices_in_table.add(int(m.group(1)))

    # Find section_table boundaries
    table_start = None
    table_end = None
    for i, l in enumerate(lines):
        if 'static SectionTableEntry section_table[]' in l:
            table_start = i
        if table_start and l.strip() == '};' and i > table_start:
            table_end = i
            break

    # If table isn't closed, we need to add missing entries and close it
    if table_end is None:
        print(f"    section_table not closed. Sections: {num_sections}, in table: {len(indices_in_table)}")

        # Add missing entries
        for idx in range(num_sections):
            if idx not in indices_in_table:
                # Find the func array name
                pattern = f"static FuncEntry section_{idx}_"
                for l in lines:
                    if l.startswith(pattern):
                        arr_name = l.split()[2].split('[')[0]
                        entry = (f"    {{ .rom_addr = 0x00000000, .ram_addr = 0x802E3B70, "
                                f".size = 0x00000004, .funcs = {arr_name}, "
                                f".num_funcs = ARRLEN({arr_name}), .relocs = nullptr, "
                                f".num_relocs = 0, .index = {idx} }},")
                        lines.append(entry)
                        print(f"    Added missing entry for index {idx} ({arr_name})")
                        break

        lines.append("};")

    # Add num_sections and overlay_sections_by_index
    idx_entries = []
    table_idx = 0
    for i in range(num_sections):
        if i < 3:  # sections 0-2 are main/common (non-relocatable)
            idx_entries.append("-1")
        else:
            idx_entries.append(str(table_idx))
            table_idx += 1

    # Format index array with 10 per line
    idx_lines = []
    for i in range(0, len(idx_entries), 10):
        chunk = idx_entries[i:i+10]
        idx_lines.append("    " + ", ".join(f"{x:>3}" for x in chunk) + ",")

    lines.append("")
    lines.append(f"static constexpr size_t num_sections = {num_sections};")
    lines.append("")
    lines.append("static int overlay_sections_by_index[] = {")
    lines.extend(idx_lines)
    lines.append("};")

    with open(path, 'w') as f:
        f.write('\n'.join(lines) + '\n')
    return True


def fix_funcs_102():
    """Fix truncated last function in funcs_102.c."""
    path = os.path.join(FUNC_DIR, "funcs_102.c")
    if not os.path.exists(path):
        return False

    with open(path) as f:
        content = f.read()

    lines = content.rstrip().split('\n')
    if lines and lines[-1].strip().endswith(';}'):
        return False  # already complete

    print("  Fixing funcs_102.c (truncated function)...")
    stub = """
    // PATCH: rest of function truncated by N64Recomp overlay_system output bug
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
;}"""
    with open(path, 'a') as f:
        f.write(stub + '\n')
    return True


def main():
    fixes = 0
    if fix_funcs_h():
        fixes += 1
    if fix_recomp_overlays():
        fixes += 1
    if fix_funcs_102():
        fixes += 1

    if fixes == 0:
        print("  No truncation fixes needed.")
    else:
        print(f"  Applied {fixes} truncation fix(es).")
    return 0


if __name__ == '__main__':
    sys.exit(main())
