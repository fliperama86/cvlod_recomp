#!/usr/bin/env python3
"""
Auto-fix static function errors by adding them to recomp.toml as manual_funcs and stubs.
"""
import subprocess
import re
import sys

MAX_ITERATIONS = 50

def run_recomp():
    result = subprocess.run(
        ["lib/N64Recomp/build/N64Recomp", "recomp.toml"],
        capture_output=True, text=True
    )
    return result.returncode, result.stdout + result.stderr

def extract_static_funcs(output):
    """Extract full static_N_ADDR function names."""
    # Pattern: static_5_80059F30
    matches = re.findall(r"(static_\d+_8[0-9A-Fa-f]+)", output)
    return list(set(matches))  # Unique names

def read_toml():
    with open("recomp.toml", "r") as f:
        return f.read()

def get_existing_manual_funcs(content):
    """Get list of already defined manual func names."""
    matches = re.findall(r'name = "((?:func_|static_\d+_)[0-9A-Fa-f]+)"', content)
    return set(matches)

def get_existing_stubs(content):
    """Get list of already stubbed functions."""
    match = re.search(r'stubs = \[(.*?)\]', content, re.DOTALL)
    if match:
        return set(re.findall(r'"([^"]+)"', match.group(1)))
    return set()

def add_manual_func(content, static_name):
    """Add a manual_func entry before [output] section."""
    # Extract address from static_N_ADDR format
    addr = re.search(r'(8[0-9A-Fa-f]+)$', static_name).group(1).upper()
    # Use larger size to accommodate functions that branch far
    entry = f'''
[[input.manual_funcs]]
name = "{static_name}"
section = ".main"
vram = 0x{addr}
size = 0x400

'''
    # Insert before [output]
    return content.replace("\n[output]", entry + "[output]")

def add_stub(content, func_name):
    """Add function to stubs list."""
    # Find the stubs array and add to it
    match = re.search(r'(stubs = \[\n)', content)
    if match:
        return content.replace(match.group(1), f'{match.group(1)}    "{func_name}",\n')
    return content

def main():
    print("Auto-fixing static function errors...")

    added_count = 0

    for iteration in range(MAX_ITERATIONS):
        returncode, output = run_recomp()

        if returncode == 0:
            print(f"\nSuccess! N64Recomp completed after adding {added_count} static functions.")
            return 0

        # Check for "already exists" error - skip that function
        if "already exists" in output:
            match = re.search(r"Manual function ((?:func_|static_\d+_)[0-9A-Fa-f]+) already exists", output)
            if match:
                func_name = match.group(1)
                print(f"  Skipping {func_name} (already exists in ELF)")
                content = read_toml()
                # Remove the manual_func entry for this function
                content = re.sub(
                    rf'\[\[input\.manual_funcs\]\]\nname = "{re.escape(func_name)}".*?size = 0x[0-9A-Fa-f]+\n\n',
                    '', content, flags=re.DOTALL
                )
                with open("recomp.toml", "w") as f:
                    f.write(content)
                continue

        # Check for static function errors
        static_addrs = extract_static_funcs(output)

        if not static_addrs:
            # Check for other errors
            if "Error recompiling" in output:
                print(f"\nNon-static function error:")
                for line in output.split('\n'):
                    if "Error" in line:
                        print(f"  {line}")
            else:
                print(f"\nUnknown error. Output tail:")
                print(output[-500:])
            return 1

        content = read_toml()
        existing_funcs = get_existing_manual_funcs(content)
        existing_stubs = get_existing_stubs(content)

        added_this_round = 0
        for static_name in static_addrs:
            if static_name not in existing_funcs:
                content = add_manual_func(content, static_name)
                added_this_round += 1
                print(f"  [{iteration+1}] Added manual_func: {static_name}")

            if static_name not in existing_stubs:
                content = add_stub(content, static_name)

        if added_this_round == 0:
            print(f"\nNo new functions to add, but still failing. Check output:")
            print(output[-1000:])
            return 1

        with open("recomp.toml", "w") as f:
            f.write(content)

        added_count += added_this_round

    print(f"\nMax iterations ({MAX_ITERATIONS}) reached!")
    return 1

if __name__ == "__main__":
    sys.exit(main())
