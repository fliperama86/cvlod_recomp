# CVLoD Recompiled

A project to statically recompile **Castlevania: Legacy of Darkness (N64)** into a native C executable using **N64Recomp**.

## 🚀 Status: Milestone 1 Achieved (Baseline Build)
We have successfully reached a **100% recompilation milestone**. All discovered functions in the game binary have been converted to native C code without requiring stubs for logic bypass.

*   **Total Functions Recompiled:** 16,646
*   **Active Logic Stubs:** 0
*   **Methodology:** Automated static analysis with dynamic lookup fallback.

## 🛠️ Key Findings & Architecture

### 1. Dynamic Lookup Fallback
The breakthrough for achieving 100% logic conversion was enabling the recompiler's undocumented fallback mode: `use_lookup_for_all_function_calls = true`. 
*   **Impact:** This allows `N64Recomp` to handle complex optimizations (like tail calls and cross-function jumps) by resolving targets at runtime rather than requiring a perfect static trace. This eliminated the need for ~3,800 manual stubs.

### 2. Intelligent Pre-Scanning
To handle the scale of 16,000+ functions, we developed an automated **Self-Healing Build System**.
*   **Symbol Discovery:** `scan_symbols.py` pre-emptively identifies missing cross-file labels (`.L...`), solving linker errors before they happen.
*   **Problematic Pattern Detection:** `scan_stubs.py` was evolved to identify "recompiler-breaking" patterns such as:
    *   Unbalanced stack frames (Shared Entry/Exit).
    *   Branch Likely instructions.
    *   Direct recursion and complex fall-throughs.

### 3. Streamlined Build Pipeline
The project uses `super_build.py` to orchestrate the entire process in a single pass:
1.  **Splat:** Splits the ROM into manageable assembly files.
2.  **Scan:** Auto-defines symbols and flags hardware-illegal instructions.
3.  **Patch:** Fixes relocation truncation errors and applies necessary assembly level patches.
4.  **Assemble & Link:** Generates a target ELF via parallel `mips-linux-gnu` toolchain.
5.  **Recompile:** Invokes `N64Recomp` to produce the final C project in `/recompiled`.

## 📂 Project Structure
*   `/asm`: Disassembled MIPS code from the original ROM.
*   `/recompiled`: The resulting native C code (ignored by git).
*   `recomp.toml`: Master configuration for the recompiler.
*   `symbol_addrs.txt`: Global symbol map for the entire binary.
*   `super_build.py`: The master build script.

## 🏁 Next Steps
With the logic successfully recompiled, the next phase is **Runtime Integration**:
1.  **Wrapper Implementation:** Developing the C wrapper to provide N64 hardware services (Input, VI, Audio, RCP) to the recompiled code.
2.  **Memory Mapping:** Mapping the recompiled binary's memory access to a modern system's heap.
3.  **Trace Debugging:** Utilizing the `trace_mode` flag to identify the boot sequence and resolve runtime initialization crashes.