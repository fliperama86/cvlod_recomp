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


def _repair_funcs_h_partial_declarations(content):
    """Complete/drop declarations that were cut mid-line before appending fixes."""
    lines = content.rstrip().split('\n')
    fixed_lines = []
    changed = False
    for line in lines:
        stripped = line.rstrip()
        if stripped and stripped.startswith('void ') and not stripped.endswith(';') and not stripped.endswith('{'):
            if '(' in stripped and 'recomp_context' not in stripped:
                # Truncated mid-declaration after '(' — complete it.
                repaired = re.sub(
                    r'\(uint8_t\*\s*rdram,\s*rec(?:om.*)?$',
                    '(uint8_t* rdram, recomp_context* ctx);',
                    line,
                )
                if repaired != line and repaired.rstrip().endswith(';'):
                    line = repaired
                    changed = True
                else:
                    # Unknown partial prototype; drop it so the complete declaration
                    # can be re-added from the function definitions below.
                    changed = True
                    continue
            elif '(' not in stripped:
                # Truncated before '(' (e.g. "void static_12") — remove line so it
                # can be re-added from the function definitions below.
                changed = True
                continue
        fixed_lines.append(line)

    return '\n'.join(fixed_lines) + '\n', changed


def fix_funcs_h():
    """Add missing forward declarations, repair partial declarations, and close extern C."""
    path = os.path.join(FUNC_DIR, "funcs.h")
    with open(path) as f:
        content = f.read()

    had_closing = "#ifdef __cplusplus\n}\n#endif" in content
    if had_closing:
        # Older fixer versions could append the closing guard while leaving a
        # partial prototype in the middle of the header.  Remove the trailer
        # temporarily, repair/complete declarations, then add one clean trailer.
        content = content.replace("\n#ifdef __cplusplus\n}\n#endif\n", "\n")

    content, repaired_partials = _repair_funcs_h_partial_declarations(content)

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
    if had_closing and not repaired_partials and not missing:
        return False  # already fixed

    if missing or repaired_partials or not had_closing:
        print("  Fixing funcs.h...")

    if missing:
        print(f"    Adding {len(missing)} missing forward declarations")
        content += "\n// Declarations truncated by N64Recomp overlay_system bug (auto-fixed)\n"
        for fn in missing:
            content += f"void {fn}(uint8_t* rdram, recomp_context* ctx);\n"

    content += "\n#ifdef __cplusplus\n}\n#endif\n"

    with open(path, 'w') as f:
        f.write(content)
    return True


def fix_recomp_overlays():
    """Fix truncated section_table and add overlay_sections_by_index."""
    path = os.path.join(FUNC_DIR, "recomp_overlays.inl")
    with open(path) as f:
        content = f.read()

    changed = False
    already_has_index = "overlay_sections_by_index" in content
    lines = content.rstrip().split('\n')

    # Older fixer versions could append the missing entries after a partial
    # section_table entry such as "    { .rom".  That leaves the table
    # syntactically invalid even though overlay_sections_by_index exists.
    repaired_lines = []
    in_section_table = False
    removed_partial = 0
    for line in lines:
        stripped = line.strip()
        if 'static SectionTableEntry section_table[]' in line:
            in_section_table = True
        elif in_section_table and stripped == '};':
            in_section_table = False

        if (in_section_table and stripped.startswith('{ .') and
                not stripped.endswith('},')):
            removed_partial += 1
            changed = True
            continue

        repaired_lines.append(line)

    if removed_partial:
        print(f"  Fixing recomp_overlays.inl...")
        print(f"    Removed {removed_partial} partial section_table entr{'y' if removed_partial == 1 else 'ies'}")

    lines = repaired_lines

    if already_has_index:
        if changed:
            with open(path, 'w') as f:
                f.write('\n'.join(lines) + '\n')
        return changed  # already fixed otherwise

    print("  Fixing recomp_overlays.inl...")

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


def fix_truncated_c_files():
    """Close any funcs_*.c file that was cut mid-function by N64Recomp."""
    fixed = False

    for fname in sorted(os.listdir(FUNC_DIR)):
        if not (fname.startswith("funcs_") and fname.endswith(".c")):
            continue

        path = os.path.join(FUNC_DIR, fname)
        with open(path) as f:
            content = f.read()

        lines = content.rstrip().split('\n')
        if lines and lines[-1].strip().endswith(';}'):
            continue  # already complete

        func_start = content.rfind("RECOMP_FUNC void ")
        if func_start == -1:
            continue

        func_body = content[func_start:]
        used_labels = set(re.findall(r'\bgoto\s+(L_[0-9A-Fa-f]+);', func_body))
        defined_labels = set(re.findall(r'^\s*(L_[0-9A-Fa-f]+):', func_body, re.MULTILINE))
        missing_labels = sorted(used_labels - defined_labels)

        print(f"  Fixing {fname} (truncated function)")
        if missing_labels:
            print(f"    Adding {len(missing_labels)} missing label(s): {', '.join(missing_labels)}")

        stub_lines = [
            "",
            "    // PATCH: rest of function truncated by N64Recomp overlay_system output bug",
        ]
        for label in missing_labels:
            stub_lines.append(f"{label}:")
        stub_lines.extend([
            "    return;",
            ";}",
        ])

        with open(path, 'a') as f:
            f.write("\n".join(stub_lines) + "\n")
        fixed = True

    return fixed


def fix_static_276_split():
    """Split a function body that N64Recomp emits without a RECOMP_FUNC header."""
    path = os.path.join(FUNC_DIR, "funcs_225.c")
    if not os.path.exists(path):
        return False

    with open(path) as f:
        content = f.read()

    if "RECOMP_FUNC void static_276_0E0037A0" in content:
        return False

    needle = (
        "    // 0x0E00379C: addiu       $sp, $sp, 0x78\n"
        "    ctx->r29 = ADD32(ctx->r29, 0X78);\n"
        "    // 0x0E0037A0: lw          $v0, 0x34($a0)\n"
    )
    if needle not in content:
        return False

    replacement = (
        "    // 0x0E00379C: addiu       $sp, $sp, 0x78\n"
        "    ctx->r29 = ADD32(ctx->r29, 0X78);\n"
        ";}\n"
        "RECOMP_FUNC void static_276_0E0037A0(uint8_t* rdram, recomp_context* ctx) {\n"
        "    uint64_t hi = 0, lo = 0, result = 0;\n"
        "    int c1cs = 0;\n"
        "    // 0x0E0037A0: lw          $v0, 0x34($a0)\n"
    )

    content = content.replace(needle, replacement, 1)
    with open(path, 'w') as f:
        f.write(content)

    print("  Fixed missing RECOMP_FUNC split for static_276_0E0037A0 in funcs_225.c")
    return True


def main():
    fixes = 0
    if fix_recomp_overlays():
        fixes += 1
    if fix_truncated_c_files():
        fixes += 1
    if fix_static_276_split():
        fixes += 1
    # Keep header repair last so it sees definitions created by the C-file
    # split/truncation repairs above.
    if fix_funcs_h():
        fixes += 1

    if fixes == 0:
        print("  No truncation fixes needed.")
    else:
        print(f"  Applied {fixes} truncation fix(es).")
    return 0


if __name__ == '__main__':
    sys.exit(main())
