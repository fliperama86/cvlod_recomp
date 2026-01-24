#!/usr/bin/env python3
"""
Auto-fix missing symbols for N64Recomp.
Iteratively runs N64Recomp, adds missing function symbols, and rebuilds.
"""
import subprocess
import re
import sys

MAX_ITERATIONS = 100

def run_recomp():
    """Run N64Recomp and capture output."""
    result = subprocess.run(
        ["lib/N64Recomp/build/N64Recomp", "recomp.toml"],
        capture_output=True,
        text=True
    )
    return result.returncode, result.stdout + result.stderr

def extract_missing_func(output):
    """Extract missing function address from N64Recomp output."""
    # Pattern: "No function found for jal target: 0xADDRESS"
    match = re.search(r"No function found for jal target: (0x[0-9A-Fa-f]+)", output)
    if match:
        return match.group(1)
    return None

def extract_failing_func(output):
    """Extract the name of the failing function from N64Recomp output."""
    match = re.search(r"Error recompiling ([A-Za-z0-9_\.]+)", output)
    if match:
        func_name = match.group(1)
        # Handle static functions: static_N_ADDR -> func_ADDR
        if func_name.startswith("static_"):
            parts = func_name.split("_")
            if len(parts) >= 3:
                addr = parts[-1]
                return f"func_{addr}"
        return func_name
    return None

def add_symbol(addr):
    """Add a function symbol to symbol_addrs.txt."""
    addr_hex = addr.upper().replace("0X", "")
    func_name = f"func_{addr_hex}"
    line = f"{func_name} = 0x{addr_hex}; // type:func\n"

    with open("symbols/symbol_addrs.txt", "a") as f:
        f.write(line)

    return func_name

def read_current_stubs():
    """Read current stubs from recomp.toml."""
    stubs = []
    try:
        with open("recomp.toml", "r") as f:
            content = f.read()
            match = re.search(r'stubs\s*=\s*\[(.*?)\]', content, re.DOTALL)
            if match:
                stubs_str = match.group(1)
                stubs = re.findall(r'"([^"]+)"', stubs_str)
    except FileNotFoundError:
        pass
    return stubs

def write_stubs(stubs):
    """Update recomp.toml with new stubs list."""
    with open("recomp.toml", "r") as f:
        content = f.read()

    stubs_formatted = ',\n    '.join(f'"{s}"' for s in stubs)
    new_stubs_block = f'stubs = [\n    {stubs_formatted}\n]'
    content = re.sub(r'stubs\s*=\s*\[.*?\]', new_stubs_block, content, flags=re.DOTALL)

    with open("recomp.toml", "w") as f:
        f.write(content)

def rebuild():
    """Rebuild assembly and ELF."""
    print("  Regenerating assembly...")
    subprocess.run(["python3", "-m", "splat", "split", "castlevania2.yaml"],
                   capture_output=True)
    print("  Rebuilding ELF...")
    subprocess.run(["./docker_build.sh"], capture_output=True)

def main():
    print("Auto-fixing N64Recomp symbol issues...")

    stubs = read_current_stubs()
    added_symbols = []

    for iteration in range(MAX_ITERATIONS):
        returncode, output = run_recomp()

        if returncode == 0:
            print(f"\nSuccess! N64Recomp completed.")
            print(f"Added {len(added_symbols)} symbols: {added_symbols}")
            print(f"Total stubs: {len(stubs)}")
            return 0

        # Check for missing function
        missing_addr = extract_missing_func(output)
        if missing_addr:
            func_name = add_symbol(missing_addr)
            print(f"  [{iteration+1}] Added symbol: {func_name}")
            added_symbols.append(func_name)
            rebuild()
            continue

        # Check for failing function (needs stub)
        failing_func = extract_failing_func(output)
        if failing_func:
            if failing_func in stubs:
                print(f"\nFunction {failing_func} already stubbed but still failing!")
                print(output[-1000:])
                return 1
            print(f"  [{iteration+1}] Stubbing: {failing_func}")
            stubs.append(failing_func)
            write_stubs(stubs)
            continue

        print(f"\nCouldn't extract error from output:")
        print(output[-1000:])
        return 1

    print(f"\nMax iterations ({MAX_ITERATIONS}) reached!")
    return 1

if __name__ == "__main__":
    sys.exit(main())
