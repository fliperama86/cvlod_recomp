import os
import re

def main():
    asm_dir = "asm"
    stubs_file = "symbols/stubs.txt"
    
    # Load functions to stub
    funcs_to_stub = set()
    if os.path.exists(stubs_file):
        with open(stubs_file, "r") as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith("#"):
                    funcs_to_stub.add(line)

    print(f"Loaded {len(funcs_to_stub)} functions to stub from {stubs_file}")

    total_patched_lines = 0
    patched_files = 0

    for root, dirs, files in os.walk(asm_dir):
        if "nonmatchings" in dirs:
            dirs.remove("nonmatchings")

        for filename in files:
            if filename.endswith(".s"):
                file_path = os.path.join(root, filename)
                
                with open(file_path, "r") as f:
                    lines = f.readlines()

                new_lines = []
                file_patched = False
                in_stubbed_func = False
                
                # Heuristics for relocation patching
                for line in lines:
                    stripped = line.strip()

                    # 1. Convert alabel func_8 to glabel for proper function export (do this first!)
                    if "alabel func_8" in line:
                        line = line.replace("alabel func_8", "glabel func_8")
                        file_patched = True

                    # 2. Handle Function Stubbing
                    # Check for function start
                    glabel_match = re.search(r"glabel\s+(\w+)", line)
                    if glabel_match:
                        func_name = glabel_match.group(1)
                        if func_name in funcs_to_stub:
                            in_stubbed_func = True
                            file_patched = True
                            new_lines.append(line)
                            new_lines.append("    jr $ra\n")
                            new_lines.append("    nop\n")
                            # print(f"  Stubbing {func_name} in {filename}")
                            continue

                    if in_stubbed_func:
                        if "endlabel" in line:
                            in_stubbed_func = False
                            new_lines.append(line)
                        else:
                            # Preserve labels to satisfy local jumps, but stub code
                            if stripped.endswith(":") or "glabel" in line or "dlabel" in line:
                                 new_lines.append(line)
                                 new_lines.append("    jr $ra\n")
                                 new_lines.append("    nop\n")
                        continue

                    # 3. Handle Relocation Truncation (Suspicious Branches)
                    # (alabel->glabel conversion was moved to step 1)
                    is_branch = False
                    if "bltz" in line or "bgez" in line or "blez" in line or "bgtz" in line or "beq" in line or "bne" in line or "bc1" in line:
                        is_branch = True
                    # Check for plain 'b' (unconditional branch)
                    if not is_branch and (" b " in line or "\tb" in line):
                         if re.search(r"\s+b\s+", line):
                             is_branch = True

                    if is_branch and "D_80" in line:
                        # Comment out and nop
                        new_lines.append(f"# PATCHED: {stripped}\n    nop\n")
                        total_patched_lines += 1
                        file_patched = True
                    else:
                        new_lines.append(line)

                if file_patched:
                    # Check if content actually changed to preserve mtime for incremental builds
                    if new_lines != lines:
                        with open(file_path, "w") as f:
                            f.writelines(new_lines)
                        patched_files += 1

    print(f"Total: Patched {total_patched_lines} relocation lines. Stubbed active functions in {patched_files} files.")

if __name__ == "__main__":
    main()