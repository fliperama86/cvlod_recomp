#!/usr/bin/env python3
"""
Fix undeclared label references in recompiled code.

When N64Recomp splits functions, it sometimes leaves goto statements
pointing to labels in other functions. This script finds and fixes these.
"""

import re
import sys
from pathlib import Path
from collections import defaultdict

def find_labels(content: str) -> set[str]:
    """Find all declared labels in a file."""
    # Match labels like L_8014831C: or skip_0:
    return set(re.findall(r'^([a-zA-Z_][a-zA-Z0-9_]*):', content, re.MULTILINE))

def find_gotos(content: str) -> set[str]:
    """Find all goto targets in a file."""
    return set(re.findall(r'goto\s+([a-zA-Z_][a-zA-Z0-9_]*);', content))

def fix_file(file_path: Path) -> int:
    """Fix undeclared labels in a file by commenting out invalid gotos."""
    with open(file_path, 'r') as f:
        content = f.read()

    # Find all functions and their labels
    # Split by RECOMP_FUNC
    functions = re.split(r'(RECOMP_FUNC void \w+\([^)]+\) \{)', content)

    fixed_content = []
    total_fixes = 0

    i = 0
    while i < len(functions):
        part = functions[i]

        if part.startswith('RECOMP_FUNC void'):
            # This is a function header
            func_header = part
            func_body = functions[i + 1] if i + 1 < len(functions) else ""

            # Find the end of this function (next RECOMP_FUNC or end of file)
            # Function body ends at the matching }
            # We need to find where this function ends
            brace_count = 1
            func_end = 0
            for j, c in enumerate(func_body):
                if c == '{':
                    brace_count += 1
                elif c == '}':
                    brace_count -= 1
                    if brace_count == 0:
                        func_end = j + 1
                        break

            actual_body = func_body[:func_end]
            rest = func_body[func_end:]

            # Find labels and gotos in this function
            labels = find_labels(actual_body)
            gotos = find_gotos(actual_body)

            # Find undeclared labels
            undeclared = gotos - labels

            if undeclared:
                # Comment out gotos to undeclared labels
                for label in undeclared:
                    pattern = rf'(\s*)(goto\s+{re.escape(label)};)'
                    replacement = r'\1/* FIXME: undeclared label */ // \2'
                    actual_body, count = re.subn(pattern, replacement, actual_body)
                    total_fixes += count

            fixed_content.append(func_header)
            fixed_content.append(actual_body)
            fixed_content.append(rest)
            i += 2
        else:
            fixed_content.append(part)
            i += 1

    new_content = ''.join(fixed_content)

    if new_content != content:
        with open(file_path, 'w') as f:
            f.write(new_content)

    return total_fixes

def main():
    project_root = Path(__file__).parent.parent
    funcs_dir = project_root / "recompiled" / "functions"

    if not funcs_dir.exists():
        print(f"Error: {funcs_dir} not found")
        return 1

    print("Fixing undeclared label references...")

    total_fixes = 0

    for c_file in sorted(funcs_dir.glob("*.c")):
        fixes = fix_file(c_file)
        if fixes:
            print(f"  {c_file.name}: {fixes} fixes")
            total_fixes += fixes

    print(f"\nTotal: {total_fixes} undeclared label gotos fixed")
    return 0

if __name__ == "__main__":
    sys.exit(main())
