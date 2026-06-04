# CVLoD Recompiled

A project to statically recompile **Castlevania: Legacy of Darkness (N64)** into a native executable using **N64Recomp**.

## Status

| Phase | Status |
|-------|--------|
| ROM Disassembly | ✅ Complete |
| Static Recompilation | ✅ 3,437 functions converted to C |
| Runtime Skeleton | ✅ Builds and runs |
| Infrastructure Tests | ✅ All passing |
| ROM Loading | ✅ 16MB loaded, format detected |
| Boot Sequence | ✅ BSS clear, stack setup |
| Entry Point | ✅ game_main() executes and returns |
| Thread System | ✅ Integrated with pthreads |
| Graphics (RT64) | ⏳ Hooked via native syscall overrides |
| Audio | ⏳ Pending |
| Input | ⏳ Pending |
| **Playable** | ❌ Not yet |

## Quick Start

### Build Runtime

```bash
# Clone with submodules
git clone --recursive https://github.com/user/cvlod_recomp.git
cd cvlod_recomp

# Build (WITH graphics enabled)
mkdir cmake-build && cd cmake-build
cmake -DBUILD_WITH_RT64=ON ..
cmake --build . --parallel 8

# Run with ROM
./cvlod_recomp ../path/to/game.z64

# Test infrastructure
./cvlod_test
```

### Regenerate from ROM

```bash
# Full pipeline (requires ROM and Docker)
python3 tools/super_build.py

# Fix generated code
python3 tools/fix_recompiled.py
python3 tools/fix_undeclared_labels.py
python3 tools/gen_func_table.py
python3 tools/gen_stubs.py

# Rebuild runtime
cd cmake-build && cmake .. && cmake --build . --parallel 8
```

## Architecture

```
ROM → splat → MIPS ASM → ELF → N64Recomp → C Code → CMake → Executable
                                              ↓
                                    N64ModernRuntime
                                    (ultramodern + RT64)
```

### Key Stats

- **Functions recompiled:** ~3,437
- **Generated C code:** 18MB (72 files)
- **Function lookup table:** 3,437 entries
- **Code fixes applied:** 9,724 (gotos), 65 (.L identifiers)

## Project Structure

```
cvlod_recomp/
├── lib/
│   ├── N64ModernRuntime/   # Runtime library (ultramodern + librecomp)
│   ├── N64Recomp/          # Static recompiler
│   └── rt64/               # Graphics backend
├── src/                    # Runtime source
│   ├── main.cpp            # Entry point
│   ├── libultra_wrappers.cpp # Zelda-style syscall overrides
│   ├── rt64_context.cpp    # RT64 integration
│   └── funcs.cpp           # Function lookup table
├── recompiled/             # Generated C code (gitignored)
├── tools/                  # Build scripts
├── CMakeLists.txt          # Runtime build
└── recomp.toml             # N64Recomp config
```

## What's Working

- ✅ All game functions converted to native C
- ✅ Memory access simulation (RDRAM)
- ✅ Register context (32 GPR, 32 FPR)
- ✅ 64-bit arithmetic (DMULT, DDIV)
- ✅ Function lookup for indirect calls (incl. labels)
- ✅ ROM loading (z64/v64/n64 formats)
- ✅ Boot sequence (BSS clear, stack setup)
- ✅ game_main() executes and returns normally
- ✅ Native syscall overrides (Zelda-style hijacking)
- ✅ Graphics hooked (osViSwapBuffer, osViSetEvent captured)

## What's Next

1. **Thread System** - Implement osCreateThread/osStartThread stubs
2. **Hardware Emulation** - RSP/RDP/VI/AI/PI register stubs
3. **RT64 Graphics** - Enable `-DBUILD_WITH_RT64=ON`
4. **Audio System** - Implement RSP callbacks
5. **Input Handling** - Controller support via SDL2

## Documentation

- [CLAUDE.md](CLAUDE.md) - Development guide
- [docs/RUNTIME_INTEGRATION.md](docs/RUNTIME_INTEGRATION.md) - Runtime integration details

## Credits

- [N64Recomp](https://github.com/N64Recomp/N64Recomp) - Static recompilation tool
- [N64ModernRuntime](https://github.com/N64Recomp/N64ModernRuntime) - Runtime library
- [RT64](https://github.com/rt64/rt64) - N64 graphics renderer
- [Zelda64Recomp](https://github.com/Zelda64Recomp/Zelda64Recomp) - Reference implementation

## License

This project is for educational and preservation purposes. The original game is property of Konami.
