import os
import re
import sys

def main():
    asm_dir = "asm"
    stubs_file = "symbols/stubs.txt"
    
    # Only keep the absolute "hard" blockers
    banned_patterns = [
        r"\bmfc0\b", r"\bmtc0\b", 
        r"\bdmfc0\b", r"\bdmtc0\b",
        r"\bcfc0\b", r"\bctc0\b",
        r"\bcache\b",
        r"\btlbp\b", r"\btlbr\b", r"\btlbwi\b", r"\btlbwr\b",
        r"\beret\b",
        r"\bsync\b",
        r"\bsyscall\b",
        r"\bbreak\b",
    ]
    
    compiled_patterns = [re.compile(p) for p in banned_patterns]
    
    # Advanced Control Flow hard blockers
    jump_table_regex = re.compile(r"^\s*jr\s+\$(?!ra)\w+", re.IGNORECASE)
    jalr_regex = re.compile(r"^\s*jalr\s+\$(?!(t9|ra))\w+", re.IGNORECASE)

    current_stubs = set()
    if os.path.exists(stubs_file):
        with open(stubs_file, "r") as f:
            for line in f:
                if line.strip():
                    current_stubs.add(line.strip())

    found_funcs = set()
    
    print("Scanning asm/ for Hard Blockers (Moderate Scanning v9)...")
    
    for root, dirs, files in os.walk(asm_dir):
        if "nonmatchings" in dirs:
            dirs.remove("nonmatchings")

        for filename in files:
            if filename.endswith(".s"):
                file_path = os.path.join(root, filename)
                
                with open(file_path, "r") as f:
                    try:
                        lines = f.readlines()
                    except UnicodeDecodeError:
                        continue 

                    current_func = None
                    for line in lines:
                        glabel_match = re.search(r"glabel\s+(\w+)", line)
                        if glabel_match:
                            current_func = glabel_match.group(1)
                            continue
                        
                        if not current_func or current_func in current_stubs:
                            continue
                            
                        code_part = line.split("#")[0]
                        
                        # 1. Banned Instructions
                        is_problematic = False
                        for pat in compiled_patterns:
                            if pat.search(code_part):
                                is_problematic = True
                                break
                        
                        # 2. Jump Tables / Indirect Calls
                        if not is_problematic:
                            if jump_table_regex.search(code_part) or jalr_regex.search(code_part):
                                is_problematic = True
                        
                        if is_problematic:
                            found_funcs.add(current_func)
                                
    if found_funcs:
        print(f"Adding {len(found_funcs)} hard-blocked functions to {stubs_file}")
        with open(stubs_file, "a") as f:
            for func in sorted(found_funcs):
                if func not in current_stubs:
                    f.write(f"{func}\n")
    else:
        print("No new problematic functions found.")

if __name__ == "__main__":
    main()
