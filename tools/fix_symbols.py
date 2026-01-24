import re
import os

def main():
    log_file = "build_errors.log"
    symbol_file = "symbols/symbol_addrs.txt"
    
    if not os.path.exists(log_file):
        print(f"Log file {log_file} not found.")
        return

    undefined_symbols = set()
    
    # Regex to capture .Lxxxxxxxx symbols
    # Pattern: undefined reference to `.L801403E8'
    # Note: The error message format might vary slightly but `undefined reference to ` is standard.
    # The symbol might be quoted like `.L...` or just .L...
    
    pattern = re.compile(r"undefined reference to `(\.L([0-9A-Fa-f]+))'")
    
    with open(log_file, "r") as f:
        for line in f:
            match = pattern.search(line)
            if match:
                full_sym = match.group(1) # .L801403E8
                addr_str = match.group(2) # 801403E8
                
                # Check if it looks like a valid address (8 characters, starts with 80)
                if len(addr_str) == 8 and addr_str.startswith("8"):
                    undefined_symbols.add(addr_str)

    if not undefined_symbols:
        print("No .L undefined symbols found in log.")
        return

    print(f"Found {len(undefined_symbols)} undefined .L symbols.")
    
    # Read existing symbols to avoid duplicates
    existing_addrs = set()
    if os.path.exists(symbol_file):
        with open(symbol_file, "r") as f:
            for line in f:
                # Naive parse: look for = 0x...
                if "=" in line and ";" in line:
                    parts = line.split("=")
                    if len(parts) > 1:
                        addr_part = parts[1].split(";")[0].strip()
                        try:
                            # Handle 0x prefix
                            val = int(addr_part, 16)
                            existing_addrs.add(f"{val:08X}")
                        except ValueError:
                            pass

    new_entries = []
    for addr in sorted(undefined_symbols):
        if addr in existing_addrs:
            continue
            
        # Guess name: D_801403E8
        name = f"D_{addr}"
        # We assume it's data for now to be safe, splat handles it.
        # Format: name = 0xADDR; // type:data
        # Use type:func to ensure glabel/export
        new_entries.append(f"{name} = 0x{addr}; // type:func") 
        # Using type:label or type:data is safer than func if we don't know.
        # But splat might not export 'label' types to C headers/obj?
        # Actually for linking, we just need the symbol to exist.
        
    if new_entries:
        print(f"Adding {len(new_entries)} new symbols to {symbol_file}...")
        with open(symbol_file, "a") as f:
            f.write("\n// Auto-generated from link errors\n")
            for entry in new_entries:
                f.write(entry + "\n")
    else:
        print("No new symbols to add.")

if __name__ == "__main__":
    main()
