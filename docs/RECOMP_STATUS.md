# CVLoD Recompilation Project Status

## Project Goal
Recompile **Castlevania: Legacy of Darkness (N64)** into a native executable using **N64Recomp**.

## Current Phase: Runtime Integration

### Status Summary (January 2026)
| Component | Status |
| :--- | :--- |
| ROM Disassembly | Complete |
| Static Recompilation | Complete (3,437 functions) |
| Runtime Skeleton | Complete |
| RT64 Graphics Init | **Working** |
| Game Thread System | **Working** |
| VI Thread | **Working** (60Hz update_screen calls) |
| Graphics Rendering | Pending (no display lists yet) |
| Audio | Not integrated |
| Input | Not integrated |

### Recent Breakthrough
RT64 graphics backend now initializes successfully on macOS with Metal:
- SDL window creation
- RT64::Application setup
- VI thread running at ~60Hz
- Game threads (idle + main) created and started correctly

### Technical Achievements
1. **Segment Reconstruction**: Fully mapped the `main` and `common` segments using detailed metadata.
2. **Relocation Patching**: Implemented a heuristic patcher to fix "Relocation truncated to fit" errors.
3. **Symbol Auto-Discovery**: Automated the definition of undefined symbols (`D_80xxxxxx`).
4. **Hardware Instruction Stubbing**: Pre-emptive scanner identifies and stubs functions with unsupported instructions.
5. **Zelda-Style Hijacking**: Direct symbol overrides for `libultra` functions via linker interposition.
6. **RT64 Integration**: Full RT64 renderer context with Metal backend on macOS.
7. **Thread System**: ultramodern's pthread-based N64 thread emulation working correctly.
8. **macOS Metal Deadlock Fix**: Solved RT64 setup deadlock by pumping SDL events during initialization.

### Current Stats
* **Total Functions**: ~16,668 (ELF), ~3,437 (Recompiled)
* **Active Stubs**: 280+ (Listed in `recomp.toml`)
* **Patched Relocations**: ~840 lines across 228 files
* **Build Time**: significantly reduced via parallelization
* **Successful ELF Generation**: YES
* **RT64 Graphics Init**: YES
* **Game Threads Running**: YES

### Current Blockers
* **ELF Function Boundaries**: `func_800178D0` (main thread entry) has incorrect boundaries in the ELF, preventing real game initialization from running. The function ends prematurely and continues in `D_80017994`.
* **No Game Initialization**: Because func_800178D0 is stubbed, no VI mode is set, no framebuffers are created, and no display lists are submitted.
* **Window Close Crash**: Cleanup on window close causes `libc++abi: terminating` (low priority, runtime is stable during operation).

## Automation Pipeline

| Script | Purpose |
| :--- | :--- |
| `super_build.py` | Master Orchestrator - runs full build loop |
| `build.py` | Parallel & Incremental Builder |
| `scan_stubs.py` | Pre-emptive scanner for banned instructions |
| `patch_asm.py` | Smart patcher for stubs and relocations |
| `fix_symbols.py` | Extracts undefined references |
| `gen_func_table.py` | Creates function dispatch table |
| `fix_recompiled.py` | Fixes .L identifiers and main() conflict |
| `fix_undeclared_labels.py` | Fixes cross-function goto targets |
| `gen_stubs.py` | Generates stub implementations |

## Build Commands

### Full Rebuild
```bash
# 1. Recompile from ROM
python3 tools/super_build.py

# 2. Fix generated code
python3 tools/fix_recompiled.py
python3 tools/fix_undeclared_labels.py
python3 tools/gen_func_table.py
python3 tools/gen_stubs.py

# 3. Build runtime
rm -rf cmake-build && mkdir cmake-build && cd cmake-build
cmake -DBUILD_WITH_RT64=ON .. && cmake --build . --parallel 8

# 4. Run
./cvlod_recomp ../resources/castlevania_legacy_of_darkness.z64
```

## Next Steps

### Priority 1: Fix ELF Function Boundaries
The main game thread entry function (`func_800178D0` at 0x80018770) is incomplete in the ELF. We need to:
1. Investigate why splat creates incorrect function boundaries
2. Manually merge `func_800178D0` with its continuation at `D_80017994`
3. Or implement proper initialization in our native stub

### Priority 2: Get First Frame Rendered
Once game initialization runs:
1. osViSetMode will be called to set video mode
2. osViSetEvent will register VI interrupt handler
3. Display lists will be submitted via osSpTaskStartGo
4. RT64 will process them and render to screen

### Priority 3: Audio & Input
Implement remaining syscall bridges:
- `osAi*` for audio
- `osCont*` for controller input
- `osPi*` for peripheral interface
