# CVLoD Recomp - Runtime Integration Tracker

## Overview

This document tracks the progress of properly integrating N64ModernRuntime and fixing our recompilation approach to match the Zelda64Recomp methodology.

**Problem:** We stubbed 3,864 functions to achieve "100% recompilation", but this resulted in empty game logic. The correct approach is to reimplement libultra via N64ModernRuntime and only stub 2-3 hardware-specific functions.

---

## Phase 1: Setup N64ModernRuntime

| Task | Status | Notes |
|------|--------|-------|
| Add N64ModernRuntime as submodule | DONE | `lib/N64ModernRuntime` |
| Add RT64 as submodule | DONE | `lib/rt64` |
| Study N64ModernRuntime structure | TODO | Understand ultramodern, audio systems |
| Identify build requirements | TODO | CMake, dependencies, platform support |

---

## Phase 2: Identify libultra Functions

| Task | Status | Notes |
|------|--------|-------|
| List libultra functions in CVLoD | TODO | Check `config/castlevania2.yaml` segments |
| Cross-reference with N64ModernRuntime | TODO | Which functions are already reimplemented? |
| Document missing implementations | TODO | Functions we may need to add |

### Known libultra segments (from castlevania2.yaml):
- `libultra/os/*` - OS functions (threads, messages, interrupts)
- `libultra/io/*` - I/O functions (PI, SI, VI, AI, controller)
- `libultra/gu/*` - Graphics utilities (matrix, perspective)
- `libultra/al/*` - Audio library
- `libultra/libc/*` - C library functions

---

## Phase 3: Fix Recompilation Config

| Task | Status | Notes |
|------|--------|-------|
| Clear stubs.txt | DONE | Removed all 3,864 entries |
| Update recomp.toml | TODO | Add proper stub/ignore sections |
| Identify true hardware stubs | TODO | Only RCP manipulation, kseg1 access |
| Run N64Recomp without mass stubs | TODO | Collect actual errors |

### Expected minimal stubs (based on Zelda64Recomp):
```toml
[patches]
stubs = [
    # Functions that directly manipulate RCP status registers
    # Functions that access kseg1 addresses
    # TBD after analysis
]

ignored = [
    # Data symbols incorrectly marked as functions
    # TBD after analysis
]
```

---

## Phase 4: Handle Recompilation Errors

| Error Type | Count | Status | Solution |
|------------|-------|--------|----------|
| Indirect jumps (`jr $reg`) | TBD | TODO | Use `manual_funcs` or jump tables |
| COP0 instructions | TBD | TODO | Reimplement in runtime |
| Unhandled instructions | TBD | TODO | Check N64Recomp issues/PRs |

---

## Phase 5: Build Runtime Integration

| Task | Status | Notes |
|------|--------|-------|
| Create CVLoD-specific runtime wrapper | TODO | Based on Zelda64Recomp structure |
| Implement game-specific hooks | TODO | Save system,ings, etc. |
| Set up CMake build | TODO | Integrate recompiled code with runtime |

---

## Phase 6: Testing

| Milestone | Status | Notes |
|-----------|--------|-------|
| Compiles without errors | TODO | |
| Links successfully | TODO | |
| Boots toings | TODO | |
| Title screen works | TODO | |
|Ings transition works | TODO | |
| Gameplay functional | TODO | |
| Audio working | TODO | |
| Save/load working | TODO | |

---

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                     CVLoD Recomp                        │
│              (Game-specific wrapper)                    │
├─────────────────────────────────────────────────────────┤
│                  N64ModernRuntime                       │
│    ┌─────────────┬─────────────┬─────────────────┐     │
│    │ ultramodern │    RT64     │     Audio       │     │
│    │  (libultra) │ (graphics)  │   (HLE audio)   │     │
│    └─────────────┴─────────────┴─────────────────┘     │
├─────────────────────────────────────────────────────────┤
│                 N64Recomp Output                        │
│            (Recompiled game C code)                     │
└─────────────────────────────────────────────────────────┘
```

### Component Responsibilities

| Component | Purpose |
|-----------|---------|
| **N64Recomp** | Converts MIPS → C code |
| **ultramodern** | Reimplements libultra (threads, messages, timers) |
| **RT64** | Modern GPU renderer (D3D12/Vulkan/Metal) |
| **Audio** | HLE audio processing |
| **Game wrapper** | CVLoD-specific: saves, settings, enhancements |

---

## Reference Links

- [N64Recomp](https://github.com/N64Recomp/N64Recomp) - Static recompilation tool
- [Zelda64Recomp](https://github.com/Zelda64Recomp/Zelda64Recomp) - Reference implementation
- [N64ModernRuntime](https://github.com/N64Recomp/N64ModernRuntime) - Runtime library (includes ultramodern)
- [RT64](https://github.com/rt64/rt64) - N64 graphics renderer (D3D12/Vulkan/Metal)
- [Zelda64RecompSyms](https://github.com/Zelda64Recomp/Zelda64RecompSyms) - Symbol format reference

---

## Progress Log

### 2026-01-24
- Identified that our approach (3,864 stubs) was incorrect
- Researched Zelda64Recomp methodology (only 3 stubs)
- Discovered N64ModernRuntime provides libultra reimplementation
- Created this tracking document
- Added N64ModernRuntime submodule (`lib/N64ModernRuntime`)
- Added RT64 submodule (`lib/rt64`)
- Cleared stubs.txt (was 3,864 entries, now empty)

---

## Questions to Resolve

1. Does N64ModernRuntime support all libultra functions used by CVLoD?
2. What Konami-specific SDK functions does CVLoD use (if any)?
3. Are there CVLoD-specific hardware interactions beyond standard N64?
4. What's the best approach for the audio system (CVLoD uses custom audio code)?
