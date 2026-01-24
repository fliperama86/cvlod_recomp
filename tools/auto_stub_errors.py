#!/usr/bin/env python3
"""
Auto-stub functions that N64Recomp can't handle.
Iteratively runs N64Recomp and adds failing functions to stubs.
"""
import subprocess
import re
import sys

MAX_ITERATIONS = 500

def run_recomp():
    """Run N64Recomp and capture output."""
    result = subprocess.run(
        ["lib/N64Recomp/build/N64Recomp", "recomp.toml"],
        capture_output=True,
        text=True
    )
    return result.returncode, result.stdout + result.stderr

def extract_failing_func(output):
    """Extract the name of the failing function from N64Recomp output."""
    # Pattern: "Error recompiling FUNCNAME"
    match = re.search(r"Error recompiling ([A-Za-z0-9_\.]+)", output)
    if match:
        func_name = match.group(1)
        # Handle static functions: static_N_ADDR -> func_ADDR
        if func_name.startswith("static_"):
            parts = func_name.split("_")
            if len(parts) >= 3:
                addr = parts[-1]  # Get the address part
                return f"func_{addr}"
        return func_name
    return None

def read_current_stubs():
    """Read current stubs from recomp.toml."""
    stubs = []
    try:
        with open("recomp.toml", "r") as f:
            content = f.read()
            # Extract stubs array content
            match = re.search(r'stubs\s*=\s*\[(.*?)\]', content, re.DOTALL)
            if match:
                stubs_str = match.group(1)
                # Parse individual stub names
                stubs = re.findall(r'"([^"]+)"', stubs_str)
    except FileNotFoundError:
        pass
    return stubs

def write_stubs(stubs):
    """Update recomp.toml with new stubs list."""
    with open("recomp.toml", "r") as f:
        content = f.read()

    # Build new stubs array
    stubs_formatted = ',\n    '.join(f'"{s}"' for s in stubs)
    new_stubs_block = f'stubs = [\n    {stubs_formatted}\n]'

    # Replace existing stubs block
    content = re.sub(r'stubs\s*=\s*\[.*?\]', new_stubs_block, content, flags=re.DOTALL)

    with open("recomp.toml", "w") as f:
        f.write(content)

def main():
    print("Auto-stubbing N64Recomp errors...")

    stubs = read_current_stubs()
    print(f"Starting with {len(stubs)} stubs")

    for iteration in range(MAX_ITERATIONS):
        returncode, output = run_recomp()

        if returncode == 0:
            print(f"\nSuccess! N64Recomp completed after {iteration} new stubs.")
            print(f"Total stubs in recomp.toml: {len(stubs)}")
            return 0

        failing_func = extract_failing_func(output)
        if not failing_func:
            print(f"\nFailed to extract failing function from output:")
            print(output[-500:])  # Print last 500 chars
            return 1

        if failing_func in stubs:
            print(f"\nFunction {failing_func} already stubbed but still failing!")
            print(output[-500:])
            return 1

        print(f"  [{iteration+1}] Stubbing: {failing_func}")
        stubs.append(failing_func)
        write_stubs(stubs)

    print(f"\nMax iterations ({MAX_ITERATIONS}) reached!")
    return 1

if __name__ == "__main__":
    sys.exit(main())
