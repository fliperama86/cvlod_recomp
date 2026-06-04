This file provides guidance to AI agents when working with code in this repository.

## Project Overview

This is a **static binary recompilation project** for Castlevania: Legacy of Darkness (N64). The goal is to convert the original N64 MIPS binary into native C code using N64Recomp, enabling the game to run on modern systems.

**Current Status:** Runtime skeleton complete - recompiled code compiles and links with N64ModernRuntime.

| Milestone            | Status                           |
| -------------------- | -------------------------------- |
| ROM Disassembly      | ✅ Complete                      |
| Static Recompilation | ✅ 3,437 functions (18MB C code) |
| Runtime Skeleton     | ✅ Builds and runs               |
| Infrastructure Tests | ✅ All passing                   |
| Graphics (RT64)      | ⏳ Hooked, awaiting frames       |
| Audio                | ⏳ Not integrated                |
| Input                | ⏳ Not integrated                |
| Playable             | ❌ Not yet                       |

## Build Commands

### Recompilation Pipeline (generates C code from ROM)

```bash
# Full self-healing build pipeline
python3 tools/super_build.py

# Individual steps:
python3 -m splat split config/castlevania2.yaml   # Disassemble ROM
python3 tools/scan_symbols.py                      # Find missing symbols
python3 tools/scan_stubs.py                        # Find unsupported instructions
python3 tools/patch_asm.py                         # Apply patches
python3 tools/build.py                             # Assemble and link ELF

# Run N64Recomp
lib/N64Recomp/build/N64Recomp recomp.toml

# Fix generated code for compilation
python3 tools/fix_recompiled.py          # Fix .L identifiers and main conflict
python3 tools/fix_undeclared_labels.py   # Fix cross-function goto targets
python3 tools/gen_func_table.py          # Generate function lookup table (supports L_ prefix)
python3 tools/gen_stubs.py               # Generate stub implementations
```

### Runtime Build (compiles native executable)

```bash
# Configure and build (WITH RT64 graphics)
mkdir -p cmake-build && cd cmake-build
cmake -DBUILD_WITH_RT64=ON ..
cmake --build . --parallel 8

# Run the executable
./cvlod_recomp ../resources/castlevania_legacy_of_darkness.z64

# Run tests
./cvlod_test
```

### Full Rebuild from Scratch

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
```

## Architecture

### build/castlevania2.elf symbol table
The recompiled code relies on names from the ELF. System functions (libultra) are often named `func_XXXXXXXX` or `.LXXXXXXXX`.

### Zelda-Style Hijacking
To override recompiled functions with native C++ logic (e.g., graphics tasks), we use **Linker Interposition**:
1. Add the function name (e.g., `func_8008A690`) to the `ignored` list in `recomp.toml`.
2. Define a "strong" version of the function in `src/libultra_wrappers.cpp` using `extern "C"`.
3. This native version will intercept all direct and indirect calls from the game code.

### Runtime Architecture

```
┌─────────────────────────────────────────────────────────┐
│                   cvlod_recomp executable               │
├─────────────────────────────────────────────────────────┤
│  src/main.cpp      - Entry point, RDRAM, SDL events     │
│  src/libultra_wrappers.cpp - Native syscall hooks       │
│  src/funcs.cpp     - Function lookup table (3,437 fn)   │
├─────────────────────────────────────────────────────────┤
│  libcvlod_recompiled.a - Recompiled game code           │
├─────────────────────────────────────────────────────────┤
│                   N64ModernRuntime                      │
│  ├── librecomp     - Glue layer                         │
│  ├── ultramodern   - libultra reimplementation          │
│  └── RT64          - Graphics backend                   │
└─────────────────────────────────────────────────────────┘
```

### Key Configuration Files

- **recomp.toml** - N64Recomp config; `use_lookup_for_all_function_calls = true` enables dynamic resolution. Critical functions are placed in `ignored` to allow native overrides.
- **CMakeLists.txt** - CMake build for runtime executable
- **config/castlevania2.yaml** - Splat config for ROM disassembly
- **symbols/stubs.txt** - Functions to stub at assembly level

### Directory Structure

```
cvlod_recomp/
├── asm/                    # Disassembled MIPS (generated)
├── build/                  # ELF and object files (generated)
├── cmake-build/            # CMake build output (generated)
├── config/                 # Splat and linker configs
├── docs/                   # Documentation
├── lib/                    # Submodules
│   ├── N64ModernRuntime/   # Runtime library
│   ├── N64Recomp/          # Static recompiler
│   └── rt64/               # Graphics backend
├── recompiled/             # Generated C code (gitignored)
│   └── functions/          # 72 C files, funcs.h header
├── src/                    # Runtime source code
│   ├── main.cpp            # Entry point
│   ├── libultra_wrappers.cpp # Zelda-style hijacking
│   ├── rt64_context.cpp    # RT64/SDL glue
│   ├── funcs.cpp           # Function lookup table
│   └── test_runtime.cpp    # Infrastructure tests
├── symbols/                # Symbol definitions
├── tools/                  # Build and fix scripts
├── CMakeLists.txt          # Runtime build config
└── recomp.toml             # N64Recomp config
```

### Code Fixes Applied

The recompiled code requires these fixes before it can compile:

| Issue                     | Script                     | Count |
| ------------------------- | -------------------------- | ----- |
| `.L*` invalid identifiers | `fix_recompiled.py`        | 65    |
| `main()` name conflict    | `fix_recompiled.py`        | 2     |
| Cross-function gotos      | `fix_undeclared_labels.py` | 9,724 |
| Missing stub definitions  | `gen_stubs.py`             | 0     |
| `.L` function lookups     | `gen_func_table.py`        | 1     |

### Memory Model

N64 addresses must be sign-extended for the runtime:

- N64 address: `0x80001000`
- Runtime address: `0xFFFFFFFF80001000` (sign-extended)
- RDRAM offset: `address - 0xFFFFFFFF80000000`

## Next Steps

1. **Verify VI Interrupts** - Ensure `osViSetEvent` is successfully signaling the scheduler.
2. **Handle Graphics Tasks** - Correctly identify microcode in `osSpTaskLoad/StartGo`.
3. **Audio/Input** - Implement similar hijacking for audio and input functions.
4. **Playable** - Get the first frame rendered!
