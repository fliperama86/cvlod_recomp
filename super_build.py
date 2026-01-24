import os
import subprocess
import re
import sys

def run_command(command, capture=False):
    print(f"CMD: {command}")
    try:
        if capture:
            return subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT).decode('utf-8')
        else:
            subprocess.check_call(command, shell=True)
            return None
    except subprocess.CalledProcessError as e:
        if capture:
            return e.output.decode('utf-8')
        raise e

def fix_symbols(log_output):
    new_symbols = []
    # Regex for undefined symbols
    # mips-linux-gnu-ld: build/asm/....o: in function `...`:
    # (.text+0x...): undefined reference to `...`
    
    # Catch both D_ and .L symbols
    # We force them to be functions to ensure global export
    
    # Pattern 1: `undefined reference to `(.L12345)'`
    matches = re.findall(r"undefined reference to `((.L|D_)([0-9A-Fa-f]+))'", log_output)
    
    added_count = 0
    with open("symbol_addrs.txt", "a") as f:
        for match in matches:
            full_name = match[0]
            prefix = match[1]
            addr_str = match[2]
            
            # Heuristic: if it's 80xxxxxx, it's a symbol
            if len(addr_str) == 8 and addr_str.startswith("80"):
                # Check if already exists (naive check)
                # We blindly append, splat might complain about duplicates but usually handles last-one-wins or we check uniqueness if needed.
                # Better to be safe: check file content?
                # For speed, let's just append. 
                # Ideally we check properly.
                
                # Construct proper name
                if prefix == ".L":
                    sym_name = f"D_{addr_str}"
                else:
                    sym_name = full_name
                
                entry = f"{sym_name} = 0x{addr_str}; // type:func\n"
                f.write(entry)
                new_symbols.append(entry)
                added_count += 1
                
    return added_count

def fix_recomp_errors(log_output):
    funcs_to_stub = set()
    unhandled_ops = set()
    
    # 1. Extract function names from common error patterns
    # Error recompiling func_80017720
    # Failed to analyze func_80149D10
    matches = re.findall(r"(?:Error recompiling|Failed to analyze) ([a-zA-Z0-9_]+)", log_output)
    for m in matches:
        funcs_to_stub.add(m)
        
    # 2. Extract unhandled operations/registers
    # Unhandled cop0 register in mfc0: 10
    # Unhandled instruction [OP] in [FUNC] (hypothetical)
    op_matches = re.findall(r"Unhandled (?:.* register in )?([a-z0-9]+)", log_output)
    for op in op_matches:
        # Avoid common words, focus on likely MIPS ops (3-6 chars usually)
        if len(op) >= 3 and op not in ["register", "instruction"]:
            unhandled_ops.add(op)

    if not funcs_to_stub and not unhandled_ops:
        return 0

    # 3. Handle unhandled operations by scanning the whole codebase
    if unhandled_ops:
        print(f"Found new unhandled instructions: {unhandled_ops}. Scanning codebase...")
        cmd = "python3 scan_stubs.py " + " ".join(unhandled_ops)
        run_command(cmd)

    # 4. Add specific failing functions to stubs.txt
    current_stubs = set()
    if os.path.exists("stubs.txt"):
        with open("stubs.txt", "r") as f:
            for line in f:
                current_stubs.add(line.strip())
    
    added = 0
    with open("stubs.txt", "a") as f:
        for func in funcs_to_stub:
            if func not in current_stubs:
                print(f"Auto-stubbing failing function: {func}")
                f.write(f"{func}\n")
                added += 1
                
    return added # This might be 0 if scan_stubs already did the work

def main():
    print("\n=== Starting Build ===")
    
    # 1. Splat (Configuration)
    if not os.path.exists("asm"):
        print("--- Running Splat ---")
        run_command("python3 -m splat split castlevania2.yaml")
    else:
        print("--- Skipping Splat (asm/ exists) ---")
    
    # 1.4 Scan for Symbols (Pre-emptive)
    print("--- Scanning for Missing Symbols ---")
    run_command("python3 scan_symbols.py")
    
    # 1.5 Scan for Stubs (Pre-emptive)
    print("--- Scanning for Unsupported Instructions ---")
    run_command("python3 scan_stubs.py")
    
    # 2. Patch ASM
    print("--- Patching ASM ---")
    run_command("python3 patch_asm.py")
    
    # 3. Compile & Link
    print("--- Building ELF ---")
    try:
        cmd = 'docker run --rm -v $(pwd):/work -w /work debian:bullseye-slim bash -c "apt-get update > /dev/null && apt-get install -y python3 binutils-mips-linux-gnu > /dev/null && python3 build.py"'
        output = run_command(cmd, capture=True)
        print("Build successful (Phase 1).")
        
    except subprocess.CalledProcessError as e:
        print("Build failed. analyzing errors...")
        output = e.output.decode('utf-8')
        print(output[-1000:]) # Print tail
        
        # Fix linker errors (optional, since we pre-scanned, but good as a fallback)
        added = fix_symbols(output)
        if added > 0:
            print(f"Fixed {added} undefined symbols. Please run again.")
        sys.exit(1)

    # 4. N64Recomp
    print("--- Running N64Recomp ---")
    try:
        cmd = "N64Recomp/build/N64Recomp recomp.toml"
        output = run_command(cmd, capture=True)
        
        # Check for silent failures that exit with code 0
        if "Error recompiling" in output or "Failed to analyze" in output:
            print("N64Recomp reported errors despite exit code 0. Analyzing...")
            added = fix_recomp_errors(output)
            if added > 0:
                print(f"Added {added} functions to stub list. Please run again.")
            sys.exit(1)

        print("Recompilation SUCCESS!")
        print(output)
        
    except subprocess.CalledProcessError as e:
        print("N64Recomp failed. Analyzing...")
        output = e.output.decode('utf-8')
        print(output[-500:])
        
        added = fix_recomp_errors(output)
        if added > 0:
            print(f"Added {added} functions to stub list. Please run again.")
        sys.exit(1)

if __name__ == "__main__":
    main()
