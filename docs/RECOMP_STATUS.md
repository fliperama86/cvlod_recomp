# CVLoD Recompilation Project Status

## Project Goal
Recompile **Castlevania: Legacy of Darkness (N64)** into a native executable using **N64Recomp**.

## Current Phase: Automated Iterative Recompilation
We are currently bypassing low-level hardware implementation hurdles to reach a "Baseline Build" where the majority of the game logic (16,000+ functions) is successfully converted.

### 🛠️ Technical Achievements
1.  **Segment Reconstruction**: Fully mapped the `main` and `common` segments using detailed metadata.
2.  **Relocation Patching**: Implemented a heuristic patcher to fix "Relocation truncated to fit" errors.
3.  **Symbol Auto-Discovery**: Automated the definition of undefined symbols (`D_80xxxxxx`).
4.  **Hardware Instruction Stubbing**: Implemented a comprehensive scanner (`scan_stubs.py`) that pre-emptively identifies and stubs functions containing instructions unsupported by `N64Recomp` (e.g., `mfc0`, `cache`, `tlb`, `eret`).

### 🤖 Automation Pipeline: Self-Healing Build System 2.0
We have a robust, optimized pipeline that automatically detects errors, fixes them, and retries until success.

| Script | Purpose |
| :--- | :--- |
| `super_build.py` | **Master Orchestrator**. Runs the build loop: Splat -> Scan -> Patch -> Build -> Recompile. Detects silent failures and auto-retries. |
| `build.py` | **Parallel & Incremental Builder**. Uses multiprocessing to assemble thousands of `.s` files in parallel and skips files that haven't changed. |
| `scan_stubs.py` | **Pre-emptive Scanner**. Scans all assembly files for banned instructions (Cop0, Cache, TLB, etc.) and adds them to `stubs.txt` *before* the build fails. |
| `patch_asm.py` | **Smart Patcher**. Applies stubs from `stubs.txt` and fixes specific relocation errors. Only writes to files if changes are needed, preserving build cache. |
| `fix_symbols.py` | **Symbol Fixer**. Extracts undefined references from linker logs and adds them to `symbol_addrs.txt`. |

### 📊 Current Stats
*   **Total Functions**: ~16,668
*   **Active Stubs**: 20+ (Auto-identified via `scan_stubs.py`)
*   **Patched Relocations**: ~840 lines across 228 files
*   **Build Time**: Significantly reduced due to parallel assembly (utilizing all CPU cores) and incremental skipping of `splat` and `as` steps.
*   **Successful ELF Generation**: **YES** (Linker stage is stable)

### 🛑 Current Blockers
*   **Jump Table Analysis**: `N64Recomp` sometimes fails to size jump tables in complex functions. These are caught by the loop and auto-stubbed.
*   **Unsupported Instructions**: `N64Recomp` crashes on certain hardware instructions. `scan_stubs.py` now effectively neutralizes this by stubbing them in advance.

## Workflow
To run the full self-healing process:
```bash
python3 super_build.py
```
This command will:
1.  Run `splat` (if `asm/` is missing).
2.  Run `scan_stubs.py` to find and block problematic hardware code.
3.  Run `patch_asm.py` to apply stubs and fix relocations.
4.  Run `build.py` to assemble (parallel) and link the ELF.
5.  Run `N64Recomp`.
6.  If `N64Recomp` fails, it adds the failing function to `stubs.txt` and restarts from step 2 automatically.

## Next Steps
1.  **Achieve 100% Recompilation**: Let `super_build.py` run until `N64Recomp` exits with a clean success.
2.  **Verify C Output**: Inspect the `recompiled/` folder to ensure C code is generating correctly.
3.  **Refine Stubs**: Begin implementing the stubbed functions (hardware access, TLB handling) in C using `N64Recomp`'s runtime or custom hooks.