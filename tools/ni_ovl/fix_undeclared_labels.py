#!/usr/bin/env python3
"""
Fix undeclared label errors in RecompiledFuncs.
Labels in C are function-scoped, so a goto to a label in another function is invalid.
Replaces such gotos with 'return;'.
"""

import os, re

FUNC_DIR = '/Users/dudu/Projects/recomp/lod/RecompiledFuncs'

def fix_file(filepath):
    with open(filepath) as f:
        lines = f.readlines()

    # Parse functions and their labels/gotos
    functions = []  # list of (start_line, end_line, name, defined_labels, goto_lines)
    func_start = None
    func_name = None
    brace_depth = 0

    for i, line in enumerate(lines):
        m = re.match(r'RECOMP_FUNC void (\w+)\(', line)
        if m:
            func_start = i
            func_name = m.group(1)
            brace_depth = 0

        if func_start is not None:
            brace_depth += line.count('{') - line.count('}')
            if brace_depth <= 0 and '{' in ''.join(lines[func_start:i+1]):
                # End of function
                functions.append((func_start, i, func_name))
                func_start = None

    # For each function, find defined labels and goto targets
    fixes = 0
    for start, end, name in functions:
        func_text = ''.join(lines[start:end+1])
        defined = set(re.findall(r'\b(L_[0-9A-Fa-f]+):', func_text))

        for j in range(start, end + 1):
            m = re.search(r'goto (L_[0-9A-Fa-f]+);', lines[j])
            if m and m.group(1) not in defined:
                label = m.group(1)
                lines[j] = re.sub(
                    rf'goto {re.escape(label)};',
                    f'return; // goto {label} (label in different function)',
                    lines[j]
                )
                fixes += 1

    if fixes > 0:
        with open(filepath, 'w') as f:
            f.writelines(lines)

    return fixes

total = 0
for fname in sorted(os.listdir(FUNC_DIR)):
    if not fname.endswith('.c'):
        continue
    filepath = os.path.join(FUNC_DIR, fname)
    fixes = fix_file(filepath)
    if fixes > 0:
        print(f"  {fname}: {fixes} fix(es)")
        total += fixes

print(f"\nFixed {total} cross-function goto(s)")
