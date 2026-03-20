#!/usr/bin/env python3
"""
Add NI overlay section entries to recomp_overlays.inl.
N64Recomp generates the C functions but only outputs section_table entries
for the original (non-NI) sections. This script adds the missing NI entries.

Must be run AFTER N64Recomp and fix_n64recomp_truncation.py.
"""

import json, re, os

BASE_DIR = '/Users/dudu/Projects/recomp/lod'
INL_PATH = os.path.join(BASE_DIR, 'RecompiledFuncs/recomp_overlays.inl')

def main():
    with open(os.path.join(BASE_DIR, 'tools/ni_ovl/out/functions.json')) as f:
        all_functions = json.load(f)

    with open(os.path.join(BASE_DIR, 'tools/ni_ovl/out/rom_offsets.json')) as f:
        rom_offsets = json.load(f)
    offset_map = {o['pair_index']: o for o in rom_offsets}

    with open(os.path.join(BASE_DIR, 'tools/ni_ovl/out/vram_classification.json')) as f:
        classification = json.load(f)
    pairs_0e = set(classification['0x0E_pairs'])

    with open(INL_PATH) as f:
        content = f.read()

    # Find current max section index in section_table
    max_idx = max(int(m.group(1)) for m in re.finditer(r'\.index = (\d+)', content))
    print(f"Current max section index: {max_idx}")

    # Generate FuncEntry arrays for NI overlays
    func_arrays = []
    table_entries = []

    for i, ovl in enumerate(all_functions):
        pair_idx = ovl['pair_index']
        rom_info = offset_map[pair_idx]
        rom_offset = rom_info['rom_offset']
        text_size = ovl['text_size']
        section_idx = max_idx + 1 + i
        vram_base = 0x0E000000 if pair_idx in pairs_0e else 0x0F000000

        arr_name = f"section_{section_idx}_ni_ovl_{pair_idx:03d}_funcs"

        entries = []
        for func in ovl['functions']:
            offset = func['offset']
            vram = vram_base + offset
            size = func['size']
            fname = f"ni_ovl_{pair_idx:03d}_func_{vram:08X}"
            entries.append(f"    {{ .func = {fname}, .offset = 0x{offset:08X}, .rom_size = 0x{size:08X} }}")

        func_arrays.append(
            f"static FuncEntry {arr_name}[] = {{\n"
            + ",\n".join(entries)
            + f"\n}};"
        )

        table_entries.append(
            f"    {{ .rom_addr = 0x{rom_offset:08X}, .ram_addr = 0x{vram_base:08X}, "
            f".size = 0x{text_size:08X}, .funcs = {arr_name}, "
            f".num_funcs = ARRLEN({arr_name}), .relocs = nullptr, "
            f".num_relocs = 0, .index = {section_idx} }}"
        )

    total_sections = max_idx + 1 + len(all_functions)

    # Strategy: find the three key structures and rebuild them
    # 1. Insert func arrays BEFORE section_table
    # 2. Append NI entries to section_table
    # 3. Replace num_sections and overlay_sections_by_index

    # Find section_table start
    st_match = re.search(r'static SectionTableEntry section_table\[\]', content)
    if not st_match:
        print("ERROR: Cannot find section_table")
        return

    # Insert func arrays before section_table
    insert_pos = st_match.start()
    func_arrays_text = "\n// NI overlay function arrays (auto-generated)\n\n"
    func_arrays_text += "\n\n".join(func_arrays)
    func_arrays_text += "\n\n"

    content = content[:insert_pos] + func_arrays_text + content[insert_pos:]

    # Find section_table closing }; (it's followed by num_sections or overlay_sections)
    # Find the FIRST }; after section_table that's followed by static
    st_start = content.find('static SectionTableEntry section_table[]')
    # Find the closing }; of section_table
    brace_depth = 0
    st_end = None
    in_table = False
    for i in range(st_start, len(content)):
        if content[i] == '{':
            brace_depth += 1
            in_table = True
        elif content[i] == '}':
            brace_depth -= 1
            if in_table and brace_depth == 0:
                st_end = i + 1  # past the }
                # Skip the ;
                if st_end < len(content) and content[st_end] == ';':
                    st_end += 1
                break

    if st_end is None:
        print("ERROR: Cannot find end of section_table")
        return

    # Remove everything from section_table end to end of file
    # (num_sections, overlay_sections_by_index, possibly duplicates)
    after_table = content[st_end:]

    # Insert NI entries before the closing of section_table
    # Find the last entry line before };
    closing_brace = content.rfind('};', st_start, st_end)
    last_entry_end = content.rfind(',', st_start, closing_brace)

    new_content = content[:last_entry_end + 1]
    new_content += "\n    // NI overlay sections (auto-generated)\n"
    new_content += ",\n".join(table_entries)
    new_content += "\n};\n"

    # Add num_sections
    new_content += f"\nstatic constexpr size_t num_sections = {total_sections};\n\n"

    # Generate overlay_sections_by_index
    idx_entries = []
    for i in range(total_sections):
        if i < 3:
            idx_entries.append("-1")
        else:
            idx_entries.append(str(i - 3))

    idx_lines = []
    for i in range(0, len(idx_entries), 10):
        chunk = idx_entries[i:i+10]
        idx_lines.append("    " + ", ".join(f"{x:>4}" for x in chunk) + ",")

    new_content += "static int overlay_sections_by_index[] = {\n"
    new_content += "\n".join(idx_lines)
    new_content += "\n};\n"

    with open(INL_PATH, 'w') as f:
        f.write(new_content)

    print(f"Added {len(all_functions)} NI overlay sections")
    print(f"Total sections: {total_sections}")
    print(f"Section index range for NI: {max_idx+1} - {max_idx+len(all_functions)}")

if __name__ == '__main__':
    main()
