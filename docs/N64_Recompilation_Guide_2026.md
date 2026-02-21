# N64 Recompilation Projects in 2026 and a Castlevania: Legacy of Darkness Foundation

Last updated: 2026-02-21.

## Scope

This document is a current-state technical guide for Nintendo 64 static recompilation projects, with a concrete bootstrap plan for **Castlevania: Legacy of Darkness** (N64).  
It is based on:

- current upstream project sources (N64Recomp ecosystem repos/releases),
- local reference implementation details from `Zelda64Recomp`,
- local ROM metadata from `resources/castlevania_legacy_of_darkness.z64`.

---

## 1) 2026 Ecosystem Snapshot (Verified)

As of 2026-02-21:

- **Core static recompiler**: `N64Recomp/N64Recomp`.
- **Core runtime layer**: `N64Recomp/N64ModernRuntime` (`ultramodern` + `librecomp`).
- **Newer shared frontend direction**: `N64Recomp/RecompFrontend` (active in 2026).
- **Mature project reference**: `Zelda64Recomp/Zelda64Recomp` (latest release `v1.2.2`, 2025-09-01).
- **Second active full game reference**: `DinosaurPlanetRecomp/dino-recomp` (latest release `v0.0.201`, 2025-12-06).
- **Castlevania reverse-engineering reference**: `k64ret/cv64` (Castlevania 64 decompilation work; useful for symbol/header strategy even though LoD is a different target build).

Important: the N64 recomp ecosystem is actively moving. The older “single monolithic project” view is now incomplete; shared components and tooling are still evolving.

---

## 2) How N64 Recomp Projects Actually Work

N64 recomp projects are not emulators. They are compiled native binaries built from translated MIPS code plus a runtime that replaces N64 OS/hardware services.

### 2.1 Core execution model

1. Identify one exact ROM variant and lock it via hash + internal name checks.
2. Provide code metadata (symbols and section data) to `N64Recomp`.
3. Generate recompiled C/C++ output for game code.
4. Recompile RSP microcode separately (`RSPRecomp`) when needed.
5. Compile/link generated output into a native executable with:
- runtime shims (`N64ModernRuntime`),
- renderer backend (commonly RT64),
- platform callbacks (window, audio, input, threading, errors).
6. Optionally compile patches in a fast iteration path using single-file output mode.
7. Register overlays, exports, events, and mod hooks at startup.

### 2.2 Concrete pipeline from Zelda64Recomp (local reference)

From local reference files:

- Base recompile config: `us.rev1.toml`.
- Patch recompile config: `patches.toml`.
- Runtime integration and callbacks: `src/main/main.cpp`.
- Overlay registration: `src/main/register_overlays.cpp`.
- Patch/export/event registration: `src/main/register_patches.cpp`.
- Build orchestration: `CMakeLists.txt`.

The practical split is:

1. **Base game recompile output** into `RecompiledFuncs`.
2. **Patch ELF build** (`patches/patches.elf`), then `N64Recomp patches.toml` into `RecompiledPatches/patches.c` and `patches.bin`.
3. **Link order** keeps patch functions overriding base output where symbols match.

---

## 3) Current Config Reality (from N64Recomp parser/behavior)

The most useful details are in `N64Recomp/src/config.cpp` and `config.h` (not just README).

### 3.1 High-value `toml` fields

Key `[input]` fields:

- `entrypoint` (optional if inferable in chosen mode).
- `rom_file_path`
- `output_func_path` (required)
- `symbols_file_path` or `elf_path` (mutually exclusive)
- `relocatable_sections_path`
- `single_file_output`
- `functions_per_output_file` (default `50`)
- `recomp_include` (default `#include "recomp.h"`)
- `uses_mips3_float_mode`
- `use_absolute_symbols` (primarily for ELF/reference symbol scenarios)
- `func_reference_syms_file`
- `data_reference_syms_files`
- `allow_exports`
- `strict_patch_mode`

`[patches]` supports:

- `stubs`
- `ignored`
- `renamed`
- `[[patches.instruction]]`
- `[[patches.hook]]` parsing exists; practical use still project-dependent.

### 3.2 Behavioral rules worth designing around

- `symbols_file_path` and `elf_path` cannot be used together.
- `data_reference_syms_files` requires `func_reference_syms_file`.
- `strict_patch_mode` defaults to enabled when function reference symbols are provided.
- Runtime `rom_hash` validation in `librecomp` uses **XXH3_64**, not SHA-1.

---

## 4) Runtime Architecture You Need to Recreate

For a new game project, the minimum required runtime integration pattern is:

1. Define one or more `GameEntry` records.
2. Register them before startup.
3. Register callbacks for:
- RSP ucode dispatch,
- renderer creation,
- audio queue/frame count,
- input polling/state/rumble,
- graphics/window/event pumping,
- error handling,
- optional thread naming helpers.
4. Register overlays and patch exports/events/manual symbols.
5. Call `recomp::start(...)`.

The local Zelda reference is a valid template for all of the above.

---

## 5) Castlevania: Legacy of Darkness Foundation (ND4E)

Local ROM observations from `resources/castlevania_legacy_of_darkness.z64`:

- ROM header title: `CASTLEVANIA2`
- game code: `ND4E`
- byte order appears already correct for `.z64` (`80 37 12 40` at start)
- SHA-1: `879ead98f197fd05edda867655da5b1ce25aa5b8`
- SHA-256: `89e15df6042defddc48c61f7408f99d06fffdb845e2422cf8b5ba8e73d4d70fb`
- MD5: `25258460f98f567497b24844abe3a05b`

Important: runtime validation still needs the corresponding **XXH3_64** hash for `GameEntry.rom_hash`.

### 5.1 Recommended bootstrap order

1. **ROM lock gate**
- Define one exact LoD target variant first (region/revision).
- Compute/store XXH3_64 for runtime `rom_hash`.
- Keep SHA-1/SHA-256 as human-facing validation in docs/scripts.

2. **Minimal compile/run skeleton**
- Wire `N64ModernRuntime`, `N64Recomp` output, and a minimal startup path.
- Stub non-critical callbacks only where safe.

3. **Symbol pipeline**
- If no full LoD decomp symbols are available, start with minimal/manual symbol coverage and grow iteratively.
- Reuse structure from Zelda symbol repos (`syms`, `datasyms`, `datasyms_static`) once LoD equivalents are generated.

4. **RSP microcode strategy**
- Identify game-used microcodes early.
- Recompile known microcodes first (audio first is usually fastest path to “playable”).

5. **Patch iteration loop**
- Set up `patches.elf` + `single_file_output` pipeline from day one for fast functional fixes.
- Keep `strict_patch_mode = true` as default once symbol references are reliable.

6. **Overlay registration and mod/export plan**
- Implement overlay registration tables as soon as base execution enters overlay-heavy game paths.
- Keep exports/events explicit and versioned to avoid breaking downstream mods.

### 5.2 Safe initial defaults for LoD (until proven otherwise)

- `save_type`: start with `AllowAll` during bring-up, then lock to true hardware save type after verification.
- `has_compressed_code`: default `false` until a verified code decompression requirement is found.
- `decompression_routine`: keep `nullptr` unless code-compressed pipeline is confirmed.

---

## 6) Practical Milestones (Methodical Bring-Up)

Use strict go/no-go gates:

1. **Gate A: ROM acceptance**
- Correct ROM recognized by internal name + hash.
- Invalid variants rejected with explicit error category.

2. **Gate B: Entrypoint execution**
- Reach recompiled entrypoint without fatal CPU state errors.

3. **Gate C: Video/audio heartbeat**
- VI timing stable, first frame path alive, audio task callback path active.

4. **Gate D: Input/save loop**
- Controller input works end-to-end.
- Save I/O path functional and deterministic.

5. **Gate E: Overlay-heavy gameplay**
- Dynamic overlays load/relocate/dispatch correctly.

6. **Gate F: Patch and mod baseline**
- Patches override base functions reliably.
- Base exports/events available for mods.

Only move to polish (fps unlocks, widescreen fixes, advanced rendering) after Gate D/E stability.

---

## 7) Known High-Risk Areas

- Overlay relocation correctness and section mapping.
- RSP microcode coverage mismatches.
- Timing behavior differences (thread scheduling, VI cadence, buffered audio).
- Floating-point mode/state edge cases.
- Missing or low-quality symbols slowing patching/debug velocity.

---

## 8) Legal/Distribution Baseline

- Do not distribute copyrighted ROM/assets.
- Ship executable/runtime/patch infrastructure only.
- Require user-provided ROM and validate exact supported revision.

---

## 9) Source Links

### Upstream (online)

- N64Recomp: https://github.com/N64Recomp/N64Recomp
- N64Recomp releases: https://github.com/N64Recomp/N64Recomp/releases
- N64ModernRuntime: https://github.com/N64Recomp/N64ModernRuntime
- RecompFrontend: https://github.com/N64Recomp/RecompFrontend
- Zelda64Recomp: https://github.com/Zelda64Recomp/Zelda64Recomp
- Zelda64Recomp releases: https://github.com/Zelda64Recomp/Zelda64Recomp/releases
- Dinosaur Planet Recompiled: https://github.com/DinosaurPlanetRecomp/dino-recomp
- Dinosaur Planet Recompiled releases: https://github.com/DinosaurPlanetRecomp/dino-recomp/releases
- Castlevania 64 decompilation reference: https://github.com/k64ret/cv64

### Local reference paths used

- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/README.md`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/BUILDING.md`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/CMakeLists.txt`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/us.rev1.toml`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/patches.toml`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/src/main/main.cpp`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/src/main/register_overlays.cpp`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/src/main/register_patches.cpp`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/src/game/rom_decompression.cpp`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/lib/N64ModernRuntime/README.md`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/lib/N64ModernRuntime/N64Recomp/src/config.h`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/lib/N64ModernRuntime/N64Recomp/src/config.cpp`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/lib/N64ModernRuntime/librecomp/include/librecomp/game.hpp`
- `/Users/dudu/Projects/recomp/references/Zelda64Recomp/lib/N64ModernRuntime/librecomp/src/recomp.cpp`
- `/Users/dudu/Projects/recomp/lod/resources/castlevania_legacy_of_darkness.z64`

