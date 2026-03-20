#!/usr/bin/env python3
"""
Add NI overlay section entries to recomp_overlays.inl.
N64Recomp generates the C functions but truncates the overlay table.
"""

import json, re, os

BASE_DIR = '/Users/dudu/Projects/recomp/lod'
INL_PATH = os.path.join(BASE_DIR, 'RecompiledFuncs/recomp_overlays.inl')
FUNC_DIR = os.path.join(BASE_DIR, 'RecompiledFuncs')

def main():
    with open(os.path.join(BASE_DIR, 'tools/ni_ovl/out/functions.json')) as f:
        all_functions = json.load(f)

    with open(os.path.join(BASE_DIR, 'tools/ni_ovl/out/rom_offsets.json')) as f:
        rom_offsets = json.load(f)
    offset_map = {o['pair_index']: o for o in rom_offsets}

    with open(INL_PATH) as f:
        content = f.read()

    # Find current max section index
    max_idx = max(int(m.group(1)) for m in re.finditer(r'\.index = (\d+)', content))
    print(f"Current max section index: {max_idx}")

    # Generate FuncEntry arrays and section_table entries for NI overlays
    func_arrays = []
    table_entries = []

    for i, ovl in enumerate(all_functions):
        pair_idx = ovl['pair_index']
        rom_info = offset_map[pair_idx]
        rom_offset = rom_info['rom_offset']
        text_size = ovl['text_size']
        section_idx = max_idx + 1 + i
        arr_name = f"section_{section_idx}_ni_ovl_{pair_idx:03d}_funcs"

        # Generate func array
        entries = []
        for func in ovl['functions']:
            vram = func['vram']
            offset = func['offset']
            size = func['size']
            fname = f"ni_ovl_{pair_idx:03d}_func_{vram:08X}"
            entries.append(f"    {{ .func = {fname}, .offset = 0x{offset:08X}, .rom_size = 0x{size:08X} }}")

        func_arrays.append(
            f"static FuncEntry {arr_name}[] = {{\n"
            + ",\n".join(entries)
            + f"\n}};"
        )

        table_entries.append(
            f"    {{ .rom_addr = 0x{rom_offset:08X}, .ram_addr = 0x0F000000, "
            f".size = 0x{text_size:08X}, .funcs = {arr_name}, "
            f".num_funcs = ARRLEN({arr_name}), .relocs = nullptr, "
            f".num_relocs = 0, .index = {section_idx} }}"
        )

    total_sections = max_idx + 1 + len(all_functions)

    # Remove the old section_table closing, num_sections, and overlay_sections_by_index
    # Find where section_table is closed
    content = re.sub(r'\};\s*\nstatic constexpr size_t num_sections.*', '', content, flags=re.DOTALL)

    # Re-remove trailing }; if still present from section_table
    # Find last }; before end of content
    content = content.rstrip()
    if content.endswith('};'):
        content = content[:-2]
    elif content.endswith('},'):
        pass  # good, we'll append more entries

    # Add func arrays before section_table
    # Actually, we need to insert func arrays BEFORE the section_table
    # Find where section_table starts
    table_start = content.find('static SectionTableEntry section_table[]')
    if table_start < 0:
        print("ERROR: Cannot find section_table in .inl")
        return

    # Insert func arrays before section_table
    insert_text = "\n\n// NI overlay function arrays (auto-generated)\n"
    insert_text += "\n\n".join(func_arrays)
    insert_text += "\n\n"

    content = content[:table_start] + insert_text + content[table_start:]

    # Now append NI entries to section_table
    content = content.rstrip()
    if not content.endswith(','):
        content += ','
    content += "\n    // NI overlay sections (auto-generated)\n"
    content += ",\n".join(table_entries)
    content += "\n};\n"

    # Add num_sections
    content += f"\nstatic constexpr size_t num_sections = {total_sections};\n\n"

    # Generate overlay_sections_by_index
    # First 3 sections (entry, main, common) are non-relocatable
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

    content += "static int overlay_sections_by_index[] = {\n"
    content += "\n".join(idx_lines)
    content += "\n};\n"

    with open(INL_PATH, 'w') as f:
        f.write(content)

    print(f"Added {len(all_functions)} NI overlay sections")
    print(f"Total sections: {total_sections}")
    print(f"Section index range for NI: {max_idx+1} - {max_idx+len(all_functions)}")

if __name__ == '__main__':
    main()
