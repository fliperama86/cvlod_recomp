import os
import re

def main():
    asm_dir = "asm"
    symbol_file = "symbols/symbol_addrs.txt"
    
    defined_labels = set()
    used_labels = set()

    # Pattern for label definitions: .L80xxxxxx:
    def_pattern = re.compile(r"^\s*(\.L[0-9A-Fa-f]{8}):", re.MULTILINE)
    # Pattern for label usage: la $t0, .L80xxxxxx or j .L80xxxxxx
    use_pattern = re.compile(r"\b(\.L[0-9A-Fa-f]{8})\b")

    if not os.path.exists(asm_dir):
        print(f"Directory {asm_dir} not found.")
        return

    print("Scanning for cross-file .L symbols...")
    
    for root, _, files in os.walk(asm_dir):
        for filename in files:
            if filename.endswith(".s"):
                with open(os.path.join(root, filename), "r") as f:
                    try:
                        content = f.read()
                        # Find definitions in THIS file
                        for match in def_pattern.finditer(content):
                            defined_labels.add(match.group(1))
                        # Find usages in THIS file
                        for match in use_pattern.finditer(content):
                            used_labels.add(match.group(1))
                    except UnicodeDecodeError:
                        continue

    # Identify labels used but not defined in any assembly file
    missing_symbols = used_labels - defined_labels
    
    if not missing_symbols:
        print("No missing .L symbols found.")
        return

    print(f"Found {len(missing_symbols)} missing .L symbols. Checking against {symbol_file}...")

    # Load existing to avoid duplicates
    existing = set()
    if os.path.exists(symbol_file):
        with open(symbol_file, "r") as f:
            for line in f:
                if "=" in line and "0x" in line:
                    try:
                        addr_part = line.split("0x")[1].split(";")[0].strip().upper()
                        # Ensure we only catch 8-digit hex
                        if len(addr_part) >= 8:
                            existing.add(addr_part[:8])
                    except IndexError:
                        pass

    new_entries = []
    for sym in sorted(missing_symbols):
        addr = sym[2:].upper() # Strip .L
        if addr not in existing:
            new_entries.append(f"D_{addr} = 0x{addr}; // type:func")

    if new_entries:
        with open(symbol_file, "a") as f:
            f.write("\n// Pre-emptively discovered cross-file labels\n")
            for entry in new_entries:
                f.write(entry + "\n")
        print(f"Added {len(new_entries)} symbols.")
    else:
        print("All missing symbols already exist in symbol_addrs.txt.")

if __name__ == "__main__":
    main()
