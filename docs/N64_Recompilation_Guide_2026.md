# N64 Recompilation Projects in 2026 and a Castlevania: Legacy of Darkness Foundation

Last updated: 2026-02-21 (rev 3).

## Scope

This document is a current-state technical guide for Nintendo 64 static recompilation projects, with a concrete bootstrap plan for **Castlevania: Legacy of Darkness** (N64).  
It is based on:

- current upstream project sources (N64Recomp ecosystem repos/releases),
- local reference implementation details from `Zelda64Recomp`,
- local ROM metadata from `resources/castlevania_legacy_of_darkness.z64`,
- local `cv64` decompilation reference (same Konami engine as LoD),
- investigation of the `splat` binary-splitting tool and its role in the symbol pipeline.

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
- XXH3_64 (runtime `GameEntry.rom_hash`): `0x0BF827D4AC27A631`

The scaffold wires this value in `include/lod/target_rom.hpp`.

### 5.1 Recommended bootstrap order

1. **ROM lock gate**
- Define one exact LoD target variant first (region/revision).
- Compute/store XXH3_64 for runtime `rom_hash`.
- Keep SHA-1/SHA-256 as human-facing validation in docs/scripts.

2. **Minimal compile/run skeleton**
- Wire `N64ModernRuntime`, `N64Recomp` output, and a minimal startup path.
- Stub non-critical callbacks only where safe.

3. **Symbol pipeline (critical path — see section 8)**
- N64Recomp cannot auto-detect function boundaries. It requires a `.syms.toml` with explicit `{ name, vram, size }` per function — size=0 functions are silently skipped and produce empty output.
- Every successful recomp project generates symbols from a decompilation/disassembly project built with `splat` (see section 8).
- For LoD: bootstrap a `splat`-based disassembly using the cv64 decomp as template (see section 9), produce an ELF, then convert to `.syms.toml` via `N64Recomp --dump-context`.
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
- `has_compressed_code`: **confirmed `true`** — Nisitenma-Ichigo compressed overlays contain code loaded at runtime (see section 5.3).
- `decompression_routine`: required — must implement or port Nisitenma-Ichigo (LZK64) decompression.

### 5.3 Verified ROM map (from splat + hex analysis)

Total ROM size: 16 MB (`0x1000000`). Real data ends at `0xEEE924`.

```
Offset Range          Size      Type              VRAM Base     Details
──────────────────────────────────────────────────────────────────────────
0x000000 - 0x000040     64 B    N64 header        —             Standard ROM header
0x000040 - 0x001000     ~4 KB   IPL3 bootcode     —             CIC boot
0x001000 - 0x001060     96 B    Entry point       0x80000400    Boot → main
0x001060 - 0x0AB9F0   ~698 KB   Main .text        0x80000460    2,298 functions
0x0AB9F0 - 0x0C2120    ~90 KB   Main .data/.ro    (contiguous)  Pointer tables, floats, strings
0x0C2120 - 0x10D000   ~300 KB   Common .text      0x800C10C0    1,183 functions
0x10D000 - 0x140000   ~208 KB   Common .data      (contiguous)  Asset tables, animation data
0x140000 - 0xEEE930  ~13.7 MB   Compressed        —             Nisitenma-Ichigo assets + overlays
0xEEE930 - 0x1000000  ~1.1 MB   Zero padding      —             Pad to 16 MB
```

**Total uncompressed code: ~998 KB, 4,637 functions** (main + common segments).

Key findings:

- **Entry point** `0x80000400` and `main` function at `0x800177E0` confirmed by splat auto-detection.
- **Main and common segments** have contiguous VRAM (`0x80000460` → `0x800C10C0`+). The common segment contains game logic (effects, enemies, player, stage systems) — similar to cv64's "common" segment pattern.
- **BSS** starts around VRAM `0x800C3680` (stack top inferred from entry code relocations). Data references up to `0x8018xxxx` are BSS or runtime-loaded.
- **Overlay code** (functions at `0x8014xxxx`–`0x8018xxxx` called from main) is **inside compressed Nisitenma-Ichigo assets**, not at fixed ROM offsets. These are decompressed to RAM at runtime.
- **Nisitenma-Ichigo file table** at ROM `0x0B1B88`: 300 entries covering compressed assets from `0x85C590` to `0xE168BC`. Same format as cv64 (LZK64 compression, 16-byte "Nisitenma-Ichigo" magic string header).
- **No uncompressed overlays** found in ROM (unlike cv64 which has both uncompressed and compressed overlays). All LoD overlays appear to be compressed.

### 5.4 Splat configuration

The project's splat config is `castlevania2.yaml` (auto-generated, then refined):

- Generated via `python3 -m splat create_config` on the LoD ROM.
- Main and common code segments split into ~236 individual asm files using NOP-padding heuristics.
- Options: `libultra_symbols: True`, `gfx_ucode: f3dex2`, `compiler: IDO`.
- Symbol inputs: `symbol_addrs.txt`, `reloc_addrs.txt`.
- Generated outputs (not tracked in git): `asm/`, `assets/`, linker scripts, auto symbol files.

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

## 8) Symbol Pipeline: From ROM to Recompilable Output

N64Recomp has **no automatic function boundary detection**. It operates in two mutually exclusive modes, both requiring pre-existing symbol data:

### 8.1 Why symbols are the critical dependency

N64Recomp requires either:
- An **ELF file** (`elf_path`) with a symbol table containing function entries with non-zero sizes, or
- A **TOML symbol file** (`symbols_file_path`) with explicit section/function definitions.

If function sizes are zero, N64Recomp silently skips them and produces empty output files. This was the root cause of the previous failed LoD attempt (see 8.5).

### 8.2 How every successful project solves this

All known working recomp projects follow the same chain:

```
1. Decompilation/disassembly project (using splat)
   → splits ROM into segments, produces asm files + linker script
2. Compile with N64 toolchain (IDO or GCC cross-compiler)
   → produces ELF with real symbol table (names + sizes)
3. N64Recomp --dump-context <config.toml>
   → converts ELF symbol table into syms.toml + datasyms.toml
4. N64Recomp <recomp_config.toml> (with symbols_file_path pointing to syms.toml)
   → reads syms.toml + ROM → generates valid recompiled C code
```

Concrete examples:
- **Zelda64Recomp** depends on `zeldaret/mm` (full Majora's Mask decomp) → `Zelda64RecompSyms/mm.us.rev1.syms.toml`.
- **dino-recomp** depends on `zestydevy/dinosaur-planet` (parallel decomp) → same pipeline.

### 8.3 The `splat` tool

`splat` (https://github.com/ethteck/splat) is the standard N64 binary-splitting tool used by every major N64 decomp project.

**What it does:**
- Takes a `.z64` ROM + YAML config defining segment layout.
- Auto-detects function split points via NOP-padding / 16-byte alignment heuristics.
- Outputs: individual assembly files per function, C source stubs, auto-generated linker scripts, and symbol address files.
- Accepts `symbol_addrs.txt` files with known symbol names/addresses to improve output quality.

**What it does NOT do:**
- Does not produce an ELF directly — you must compile the output with an N64 toolchain.
- Does not perfectly detect all function boundaries — iterative refinement is expected.

**Setup:**
```bash
pip install splat64[mips]                       # Install (requires Python 3.9+)
python3 -m splat create_config baserom.z64      # Auto-generate initial YAML config
python3 -m splat split config.yaml              # Split ROM into segments
```

`create_config` produces a starter YAML with auto-detected segment boundaries and the correct SHA-1.

### 8.4 The `.syms.toml` format (from N64Recomp parser)

N64Recomp uses three types of symbol TOML files:

**Function symbols (`*.syms.toml`):**
```toml
[[section]]
name = "..boot"
rom = 0x00001060        # ROM offset (required for executable sections)
vram = 0x80080060       # Virtual RAM address (required)
size = 0x194A0          # Section size in bytes (required)

functions = [
    { name = "some_func", vram = 0x800888A8, size = 0x3C },
    { name = "another_func", vram = 0x8008BD24, size = 0x140 },
]
```

**Data symbols (`*.datasyms.toml`):**
```toml
[[section]]
name = "..dmadata"
rom = 0x0001A500
vram = 0x8009F8B0
size = 0x6200

symbols = [
    { name = "dmadata", vram = 0x8009F8B0 },
]
```

**Validation rules enforced by the parser:**
- Function `vram` and `size` must be word-aligned (divisible by 4).
- Section `rom` must be word-aligned.
- `functions` entries require `name`, `vram`, and `size` (all mandatory).
- `symbols` entries (data) require `name` and `vram` (no size field).
- Relocation entries (optional per section) require `vram`, `target_vram`, and `type` (`R_MIPS_HI16`, `R_MIPS_LO16`, `R_MIPS_26`, `R_MIPS_32`).

### 8.5 Previous failed LoD attempt: post-mortem

The previous attempt at `/Users/dudu/Projects/recomp/lod_recomp_failed/` failed because:

1. `symbols.s` was auto-generated with ~2300 function addresses, all with **size=0**.
2. This was assembled into `symbols.elf` — a valid ELF structurally, but with no usable size information.
3. `config.toml` used `elf_path` pointing to this hollow ELF.
4. N64Recomp ran without errors but skipped every function (size=0 → 0 instructions → skip).
5. All output files (`funcs.h`, `lookup.cpp`, `recomp_entrypoint.c`, `recomp_overlays.inl`) were 1 byte (empty).
6. Subsequent work (runtime wiring, linking) was built on this empty foundation.

**Lesson:** always verify N64Recomp output is non-empty and contains actual recompiled function bodies before proceeding to runtime integration.

---

## 9) CV64 Decompilation as LoD Template

The Castlevania 64 decompilation (`k64ret/cv64`, local at `references/cv64_decomp/`) uses the same Konami engine as LoD and provides a directly adaptable template.

### 9.1 Engine overlap with LoD

Both CV64 and LoD share:
- **Nisitenma-Ichigo** asset compression/loading system.
- **Object/actor system** (0x74-byte headers, function arrays, parent-child hierarchies).
- **system_work global state** structure (confirmed by LoD debug string references in CV64 source).
- **F3DEX2 GBI** graphics pipeline (display lists).
- **libultra N64 OS interface** (Vi, audio, DMA, threading) — 100% reusable.
- **Common Konami library** code (MNSG, etc.).

The CV64 decomp even uses leftover LoD debug strings to name CV64 symbols (e.g., `disp_fade_cnt`, `disp_fade_time`), confirming shared codebase ancestry.

**Not overlapping:** game-specific content (maps, enemies, characters, cutscene IDs), some menu systems, and overlay-specific addresses.

Estimated engine-level code overlap: ~60-70%.

### 9.2 CV64 splat setup (reference for LoD)

The CV64 decomp has a complete `splat` configuration:

- **Config:** `castlevania.yaml` (3006 lines) — full segment map with main code, 28+ overlays, compressed assets, libultra.
- **Symbols:** 4 files totaling 3,780 entries (1,942 in main `symbol_addrs.txt`, 547 overlay symbols, 77 asset symbols).
- **Build system:** CMake with splat integration — ROM → decompress → splat split → compile → ELF → verify.
- **Segment types:** header, boot, entrypoint, main code, common code, audio assets, soundfonts, sequences, player/NPC animations, character overlays (with `exclusive_ram_id` for shared VRAM), map overlays, Nisitenma-Ichigo compressed assets.

**CV64 symbol_addrs.txt format:**
```
gamestate_create = 0x80000460; // type:func
memory_clear = 0x80000770; // type:func
gGameState = 0x80340000; // type:data size:0x800
```

### 9.3 Adapting for LoD

To bootstrap an LoD disassembly from the CV64 template:

1. **Generate initial config:** `python3 -m splat create_config castlevania_legacy_of_darkness.z64` — produces starter YAML with auto-detected segments.
2. **Refine segment layout:** use CV64's `castlevania.yaml` as structural reference (similar engine → similar segment organization), but all ROM offsets and VRAM addresses will differ.
3. **Seed symbol_addrs.txt:** port CV64's libultra and engine-level symbols by byte-pattern matching known functions into the LoD ROM. Game-specific symbols will need fresh identification.
4. **Iterate:** run `splat split`, review output, refine boundaries, add symbols, repeat.
5. **Compile to ELF:** once segment layout stabilizes, set up N64 cross-compilation to produce an ELF with real symbol sizes.
6. **Generate syms.toml:** run `N64Recomp --dump-context` against the ELF to produce the `.syms.toml` that the recomp pipeline needs.

---

## 10) Legal/Distribution Baseline

- Do not distribute copyrighted ROM/assets.
- Ship executable/runtime/patch infrastructure only.
- Require user-provided ROM and validate exact supported revision.

---

## 11) Source Links

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
- splat (N64 binary splitter): https://github.com/ethteck/splat
- splat wiki/quickstart: https://github.com/ethteck/splat/wiki/Quickstart
- Dinosaur Planet decompilation: https://github.com/zestydevy/dinosaur-planet
- Zelda64RecompSyms: https://github.com/Zelda64Recomp/Zelda64RecompSyms

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
- `/Users/dudu/Projects/recomp/references/cv64_decomp/castlevania.yaml`
- `/Users/dudu/Projects/recomp/references/cv64_decomp/linker/symbol_addrs.txt`
- `/Users/dudu/Projects/recomp/references/cv64_decomp/CMakeLists.txt`
- `/Users/dudu/Projects/recomp/lod_recomp_failed/` (previous failed attempt, kept for reference)
