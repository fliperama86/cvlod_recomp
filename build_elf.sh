#!/bin/bash
set -e

# Install tools
apt-get update > /dev/null
apt-get install -y binutils-mips-linux-gnu > /dev/null

# Create directories
mkdir -p build/asm build/assets

# Assemble
echo "Assembling..."
mips-linux-gnu-as -march=vr4300 -mabi=32 -I include asm/header.s -o build/asm/header.o
mips-linux-gnu-as -march=vr4300 -mabi=32 -I include asm/1000.s -o build/asm/1000.o
mips-linux-gnu-as -march=vr4300 -mabi=32 -I include asm/1060.s -o build/asm/1060.o

# Convert binaries
echo "Converting binaries..."
# Use correct target for N64 (Big Endian MIPS)
mips-linux-gnu-objcopy -I binary -O elf32-tradbigmips -B mips assets/ipl3.bin build/assets/ipl3.o --rename-section .data=.data
mips-linux-gnu-objcopy -I binary -O elf32-tradbigmips -B mips assets/AB9F0.bin build/assets/AB9F0.o --rename-section .data=.data

# Link
echo "Linking..."
mips-linux-gnu-ld -m elf32btsmip -T castlevania2.ld -T undefined_syms_auto.txt -T undefined_funcs_auto.txt -Map build/castlevania2.map -o build/castlevania2.elf

echo "Done! ELF created at build/castlevania2.elf"
