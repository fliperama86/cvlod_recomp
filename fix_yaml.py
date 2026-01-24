import re

def main():
    file_path = "castlevania2.yaml"
    with open(file_path, "r") as f:
        lines = f.readlines()

    new_lines = []
    in_main = False
    last_addr = -1
    
    # Regex to find segment start: - [0x12345678, ...
    seg_pattern = re.compile(r"(\s+-\s+\[)(0x[0-9A-Fa-f]+)(,.*)")

    for line in lines:
        if "- name: main" in line:
            in_main = True
        if "- name: common" in line:
            in_main = False
        
        if in_main:
            match = seg_pattern.match(line)
            if match:
                prefix = match.group(1)
                addr_str = match.group(2)
                suffix = match.group(3)
                
                addr = int(addr_str, 16)
                
                # Heuristic fixes
                # If we just started, initialized last_addr
                if last_addr == -1:
                    last_addr = addr
                else:
                    # Check for drop
                    if addr < last_addr:
                        # Attempt to fix
                        # Try adding 0x80000 (missing 8 in 4th nibble, 0x0008xxxx vs 0x0000xxxx)
                        # The corrupted addresses were like 0x000CAB0 (should be 0x0008CAB0)
                        # So adding 0x80000 fixes it.
                        # What about 0x000030 (should be 0x00090030)?
                        # Adding 0x90000 fixes it.
                        
                        # We can try to guess the high bits based on last_addr.
                        # last_addr = 0x8C970. curr = 0xCAB0.
                        # 0x80000 + 0xCAB0 = 0x8CAB0. > 0x8C970? Yes.
                        # last_addr = 0x8FD20. curr = 0x30.
                        # 0x90000 + 0x30 = 0x90030. > 0x8FD20. Yes.
                        
                        fixed_addr = addr
                        if fixed_addr < last_addr:
                            # Try adding 0x80000
                            if (fixed_addr + 0x80000) > last_addr:
                                fixed_addr += 0x80000
                            elif (fixed_addr + 0x90000) > last_addr:
                                fixed_addr += 0x90000
                            elif (fixed_addr + 0xA0000) > last_addr:
                                fixed_addr += 0xA0000
                        
                        # Verify it's still reasonable (not huge jump)
                        if fixed_addr > last_addr and (fixed_addr - last_addr < 0x100000):
                            # print(f"Fixed {hex(addr)} -> {hex(fixed_addr)}")
                            addr = fixed_addr
                            # Update line
                            line = f"{prefix}0x{addr:08X}{suffix}\n"
                        else:
                            pass # Can't fix or not a drop
                
                last_addr = addr
        
        new_lines.append(line)

    with open(file_path, "w") as f:
        f.writelines(new_lines)
    
    print("Fixed addresses in castlevania2.yaml")

if __name__ == "__main__":
    main()
