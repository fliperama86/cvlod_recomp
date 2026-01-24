#!/usr/bin/env python3
"""
Comprehensive script to fix all static function errors in N64Recomp.
This script iteratively:
1. Runs N64Recomp
2. Extracts static function errors
3. Adds them to symbol_addrs.txt
4. Rebuilds the ELF
5. Updates recomp.toml with manual_funcs and stubs
6. Repeats until N64Recomp succeeds
"""
import subprocess
import re
import sys
import os

MAX_OUTER_ITERATIONS = 100
MAX_INNER_ITERATIONS = 50

def run_cmd(cmd, timeout=300):
    """Run a command and return (returncode, output)."""
    result = subprocess.run(
        cmd, shell=True, capture_output=True, text=True, timeout=timeout
    )
    return result.returncode, result.stdout + result.stderr

def run_recomp():
    result = subprocess.run(
        ["lib/N64Recomp/build/N64Recomp", "recomp.toml"],
        capture_output=True, text=True
    )
    return result.returncode, result.stdout + result.stderr

def extract_static_funcs(output):
    """Extract full static_N_ADDR function names from errors."""
    # Match both error patterns
    matches = re.findall(r"(static_\d+_8[0-9A-Fa-f]+)", output)
    return list(set(matches))

def extract_static_addrs(output):
    """Extract addresses from static function names."""
    matches = re.findall(r"static_\d+_(8[0-9A-Fa-f]+)", output)
    return list(set(matches))

def read_file(path):
    with open(path, "r") as f:
        return f.read()

def write_file(path, content):
    with open(path, "w") as f:
        f.write(content)

def get_existing_symbol_names():
    """Get set of symbol NAMES already in symbol_addrs.txt."""
    content = read_file("symbols/symbol_addrs.txt")
    matches = re.findall(r'^(\S+)\s*=', content, re.MULTILINE)
    return set(matches)

def add_symbols(static_funcs):
    """Add static functions to symbol_addrs.txt and undefined_funcs_auto.txt."""
    existing_names = get_existing_symbol_names()
    undef_content = read_file("symbols/undefined_funcs_auto.txt")

    added = []
    for func in static_funcs:
        addr = re.search(r'(8[0-9A-Fa-f]+)$', func).group(1).upper()
        symbol_line = f"{func} = 0x{addr};\n"
        actually_added = False

        # Add to symbol_addrs.txt if symbol name not exists
        if func not in existing_names:
            with open("symbols/symbol_addrs.txt", "a") as f:
                f.write(symbol_line)
            existing_names.add(func)
            actually_added = True

        # Add to undefined_funcs_auto.txt if symbol name not exists
        if func not in undef_content:
            with open("symbols/undefined_funcs_auto.txt", "a") as f:
                f.write(symbol_line)
            undef_content += symbol_line
            actually_added = True

        if actually_added:
            added.append(func)

    return added

def rebuild_elf():
    """Rebuild the ELF file."""
    print("  Rebuilding ELF...")
    returncode, output = run_cmd("./docker_build.sh", timeout=300)
    if returncode != 0:
        print(f"  ERROR: ELF build failed: {output[-500:]}")
        return False
    return True

def get_existing_manual_funcs(content):
    """Get set of function names already in manual_funcs."""
    matches = re.findall(r'name = "((?:func_|static_\d+_)[0-9A-Fa-f]+)"', content)
    return set(matches)

def get_existing_stubs(content):
    """Get set of function names already in stubs."""
    match = re.search(r'stubs = \[(.*?)\]', content, re.DOTALL)
    if match:
        return set(re.findall(r'"([^"]+)"', match.group(1)))
    return set()

def add_manual_func(content, func_name):
    """Add a manual_func entry before [output] section."""
    addr = re.search(r'(8[0-9A-Fa-f]+)$', func_name).group(1).upper()
    addr_int = int(addr, 16)

    # Determine section based on address
    # .main: 0x80000460 to 0x800B20D0
    # .common: 0x80128900 onwards
    if addr_int >= 0x80128900:
        section = ".common"
    else:
        section = ".main"

    entry = f'''
[[input.manual_funcs]]
name = "{func_name}"
section = "{section}"
vram = 0x{addr}
size = 0x400

'''
    return content.replace("\n[output]", entry + "[output]")

def add_stub(content, func_name):
    """Add function to stubs list."""
    match = re.search(r'(stubs = \[\n)', content)
    if match:
        return content.replace(match.group(1), f'{match.group(1)}    "{func_name}",\n')
    return content

def update_toml(static_funcs):
    """Update recomp.toml with new manual_funcs and stubs."""
    content = read_file("recomp.toml")
    existing_funcs = get_existing_manual_funcs(content)
    existing_stubs = get_existing_stubs(content)

    added = []
    for func in static_funcs:
        if func not in existing_funcs:
            content = add_manual_func(content, func)
            added.append(func)

        if func not in existing_stubs:
            content = add_stub(content, func)

    write_file("recomp.toml", content)
    return added

def main():
    print("=" * 60)
    print("Comprehensive Static Function Fixer")
    print("=" * 60)

    total_symbols_added = 0
    total_funcs_added = 0

    for outer in range(MAX_OUTER_ITERATIONS):
        print(f"\n--- Outer iteration {outer + 1} ---")

        # Inner loop: fix recomp.toml until no more static func errors
        for inner in range(MAX_INNER_ITERATIONS):
            returncode, output = run_recomp()

            if returncode == 0:
                print(f"\n{'=' * 60}")
                print(f"SUCCESS! N64Recomp completed.")
                print(f"Total symbols added to symbol_addrs.txt: {total_symbols_added}")
                print(f"Total functions added to recomp.toml: {total_funcs_added}")
                print(f"{'=' * 60}")
                return 0

            # Check for "already exists" error
            if "already exists" in output:
                match = re.search(r"Manual function ((?:func_|static_\d+_)[0-9A-Fa-f]+) already exists", output)
                if match:
                    func_name = match.group(1)
                    print(f"  Removing duplicate manual_func: {func_name}")
                    content = read_file("recomp.toml")
                    content = re.sub(
                        rf'\[\[input\.manual_funcs\]\]\nname = "{re.escape(func_name)}".*?size = 0x[0-9A-Fa-f]+\n\n',
                        '', content, flags=re.DOTALL
                    )
                    write_file("recomp.toml", content)
                    continue

            # Extract static function errors
            static_funcs = extract_static_funcs(output)

            if not static_funcs:
                # Check for other errors
                if "Error recompiling" in output:
                    print(f"\nNon-static function error:")
                    for line in output.split('\n'):
                        if "Error" in line:
                            print(f"  {line}")
                    # Try to find the problematic function
                    match = re.search(r"Error recompiling (\S+)", output)
                    if match:
                        func = match.group(1)
                        # Check if it's a function we haven't seen
                        if func.startswith("static_"):
                            print(f"  -> Adding {func} to stubs")
                            content = read_file("recomp.toml")
                            if func not in get_existing_stubs(content):
                                content = add_stub(content, func)
                                write_file("recomp.toml", content)
                            continue
                    break
                else:
                    print(f"\nUnknown error. Output tail:")
                    print(output[-500:])
                    break

            # Update recomp.toml
            added = update_toml(static_funcs)
            if added:
                print(f"  [Outer {outer+1}, Inner {inner+1}] Added to recomp.toml: {', '.join(added)}")
                total_funcs_added += len(added)
            else:
                # No new functions but still failing - need to add symbols and rebuild
                print(f"  No new functions to add to recomp.toml, need to add symbols and rebuild")
                break

        # After inner loop, check if we need to add symbols and rebuild
        returncode, output = run_recomp()
        if returncode == 0:
            print(f"\n{'=' * 60}")
            print(f"SUCCESS! N64Recomp completed.")
            print(f"Total symbols added to symbol_addrs.txt: {total_symbols_added}")
            print(f"Total functions added to recomp.toml: {total_funcs_added}")
            print(f"{'=' * 60}")
            return 0

        # Extract all static functions from errors and add to symbol_addrs.txt
        static_funcs = extract_static_funcs(output)
        if static_funcs:
            added = add_symbols(static_funcs)
            if added:
                print(f"  Added to symbol_addrs.txt: {', '.join(added)}")
                total_symbols_added += len(added)

                # Rebuild ELF
                if not rebuild_elf():
                    print("ELF rebuild failed, exiting")
                    return 1
            else:
                print("  No new symbols to add, but still failing")
                # Print the error
                for line in output.split('\n'):
                    if "Error" in line or "Unhandled" in line:
                        print(f"  {line}")
                return 1
        else:
            print("  No static functions found in errors")
            for line in output.split('\n'):
                if "Error" in line:
                    print(f"  {line}")
            return 1

    print(f"\nMax iterations ({MAX_OUTER_ITERATIONS}) reached!")
    return 1

if __name__ == "__main__":
    sys.exit(main())
