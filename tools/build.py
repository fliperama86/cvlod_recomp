#!/usr/bin/env python3
import os
import subprocess
import sys
import multiprocessing

def run_command(command):
    try:
        subprocess.check_call(command, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running command: {command}")
        sys.exit(1)

def assemble_file(args):
    input_path, output_path = args
    
    # Incremental build check
    if os.path.exists(output_path) and os.path.getmtime(output_path) > os.path.getmtime(input_path):
        return # Skip if up to date

    # Ensure output directory exists
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    # Assemble
    cmd = f"mips-linux-gnu-as -march=vr4300 -mabi=32 -I include {input_path} -o {output_path}"
    # print(f"Assembling {input_path}...") # Too noisy for parallel
    try:
        subprocess.check_call(cmd, shell=True)
    except subprocess.CalledProcessError:
        print(f"Failed to assemble {input_path}")
        sys.exit(1)

def main():
    # Create build directories
    os.makedirs("build/asm", exist_ok=True)
    os.makedirs("build/assets", exist_ok=True)

    # Patch ASM files (Hack to fix relocation truncation)
    print("Patching ASM...")
    run_command("python3 tools/patch_asm.py")

    # Collect tasks
    asm_dir = "asm"
    tasks = []
    
    print("Scanning for asm files...")
    for root, dirs, files in os.walk(asm_dir):
        if "nonmatchings" in dirs:
            dirs.remove("nonmatchings") # Don't traverse into nonmatchings
        if "nonmatchings" in root:
            continue

        for filename in files:
            if filename.endswith(".s"):
                input_path = os.path.join(root, filename)
                rel_dir = os.path.relpath(root, asm_dir)
                output_path = os.path.join("build/asm", rel_dir, filename.replace(".s", ".o"))
                tasks.append((input_path, output_path))

    # Run in parallel
    print(f"Assembling {len(tasks)} files using {multiprocessing.cpu_count()} threads...")
    with multiprocessing.Pool() as pool:
        pool.map(assemble_file, tasks)

    # Convert binary assets (ipl3)
    if os.path.exists("assets/bin/ipl3.bin"):
        print("Converting assets/bin/ipl3.bin...")
        run_command("mips-linux-gnu-objcopy -I binary -O elf32-tradbigmips -B mips assets/bin/ipl3.bin build/assets/ipl3.o --rename-section .data=.data")

    # Link
    print("Linking...")
    run_command("mips-linux-gnu-ld -m elf32btsmip -T config/castlevania2.ld -T symbols/undefined_syms_auto.txt -T symbols/undefined_funcs_auto.txt -Map build/castlevania2.map -o build/castlevania2.elf")

    print("Done! ELF created at build/castlevania2.elf")

if __name__ == "__main__":
    main()
