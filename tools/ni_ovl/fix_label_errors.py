#!/usr/bin/env python3
"""
Find functions with undeclared label errors in RecompiledFuncs and add them to stubs.
Must be run AFTER N64Recomp + fix_n64recomp_truncation.py + cmake + make attempt.
"""

import subprocess, re, sys, os

os.chdir('/Users/dudu/Projects/recomp/lod')

MAX_ITERATIONS = 20
all_new_stubs = []

for iteration in range(MAX_ITERATIONS):
    # Run N64Recomp
    result = subprocess.run(
        ['lib/N64ModernRuntime/N64Recomp/build_tool/N64Recomp', 'recomp.toml'],
        capture_output=True, text=True
    )
    output = result.stdout + result.stderr
    if 'Function count:' not in output:
        print(f"N64Recomp failed:\n{output[-500:]}")
        sys.exit(1)

    # Fix truncation
    subprocess.run(['python3', 'tools/fix_n64recomp_truncation.py'], capture_output=True)

    # Reconfigure cmake
    subprocess.run(['cmake', '..'], capture_output=True, cwd='build')

    # Build
    result = subprocess.run(
        ['make', '-j8'],
        capture_output=True, text=True, cwd='build'
    )

    # Find undeclared label errors
    errors = [l for l in result.stderr.split('\n') if 'use of undeclared label' in l]

    if not errors:
        print(f"Build succeeded after {iteration} stub iterations!")
        break

    # Extract function names from errors
    new_stubs = set()
    for err_line in errors:
        # Extract file path and label
        m = re.match(r'(.+\.c):\d+:\d+: error: use of undeclared label \'(L_\w+)\'', err_line)
        if not m:
            continue
        filepath, label = m.group(1), m.group(2)

        # Find the function containing this label reference
        with open(filepath) as f:
            content = f.read()

        # Find the RECOMP_FUNC before the label usage
        func_name = None
        for fm in re.finditer(r'RECOMP_FUNC void (\w+)\(', content):
            if fm.start() < content.find(label, fm.start()):
                func_name = fm.group(1)
            else:
                break

        if func_name and func_name not in new_stubs:
            new_stubs.add(func_name)

    if not new_stubs:
        print(f"Could not extract function names from errors:")
        for e in errors[:5]:
            print(f"  {e}")
        sys.exit(1)

    # Add stubs to recomp.toml
    with open('recomp.toml') as f:
        toml = f.read()

    for stub in sorted(new_stubs):
        if f'"{stub}"' not in toml:
            print(f"  Stubbing: {stub}")
            all_new_stubs.append(stub)
            # Insert before the closing ] of stubs array
            toml = toml.replace(
                '    "static_64_0F009D98",\n]',
                f'    "{stub}",\n    "static_64_0F009D98",\n]'
            )

    with open('recomp.toml', 'w') as f:
        f.write(toml)

else:
    print(f"Still failing after {MAX_ITERATIONS} iterations")
    sys.exit(1)

if all_new_stubs:
    print(f"\nAdditional stubs added ({len(all_new_stubs)}):")
    for s in sorted(all_new_stubs):
        print(f'    "{s}",')
