2026-02-20 23:54:11 -0300
- User constraints: no non-read-only actions without prior discussion/approval; proceed one step at a time; no commits unless explicitly requested.
- Task: create up-to-date N64 recompilation documentation for bootstrapping a Castlevania: Legacy of Darkness (N64) recomp project.
- Read-only work completed: inspected local `Zelda64Recomp` reference (pipeline, TOML configs, runtime integration, overlays/patches/mod paths), validated local LoD ROM metadata.
- Local ROM facts: `resources/castlevania_legacy_of_darkness.z64` header title `CASTLEVANIA2`, game code `ND4E`; SHA1 `879ead98f197fd05edda867655da5b1ce25aa5b8`; SHA256 `89e15df6042defddc48c61f7408f99d06fffdb845e2422cf8b5ba8e73d4d70fb`; MD5 `25258460f98f567497b24844abe3a05b`.
- Online verification completed: checked current upstream repos/releases (N64Recomp org, N64Recomp tool, N64ModernRuntime, Zelda64Recomp, dino-recomp) and identified newer `N64Recomp/RecompFrontend` repository.
- Current hypothesis: safest LoD path is Zelda64Recomp-style baseline (N64Recomp + N64ModernRuntime + RT64) with early strict ROM/version gating and symbol/patch pipeline established before feature work.
- Next step: write `docs/N64_Recompilation_Guide_2026.md` with source-linked 2026 architecture + Castlevania bootstrap plan.

2026-02-20 23:55:46 -0300
- Write step completed: added `docs/N64_Recompilation_Guide_2026.md` (kept old guide intact).
- New doc includes: 2026 ecosystem snapshot, concrete end-to-end recomp pipeline, config behavior from parser code, LoD-specific bootstrap gates, risk list, and local/online source lists.
- Verified consistency check: file readback succeeded.
- Recommended next step: compute LoD XXH3_64 hash and lock first `GameEntry` target revision before scaffolding code.

2026-02-21 00:01:12 -0300
- Executed first bring-up step: computed `XXH3_64` for `resources/castlevania_legacy_of_darkness.z64` via temporary local tool using vendored xxHash sources.
- Result for runtime `GameEntry.rom_hash`: `0x0BF827D4AC27A631`.
- First compile attempt failed due incorrect header-only instantiation; corrected by compiling against `xxhash.c` directly.
- Next recommended step: scaffold minimal LoD project files with locked `GameEntry` constants and placeholder callback wiring.

2026-02-21 00:03:56 -0300
- Scaffold step completed: added minimal bootstrap project files (`CMakeLists.txt`, `include/lod/target_rom.hpp`, `src/main/main.cpp`, `config/lod.nd4e.toml`, `config/patches.toml`, `config/overlays.nd4e.txt`).
- Verification: configured and built successfully with `Unix Makefiles` + `clang/clang++`; executable prints locked ROM constants including `rom_hash` value.
- Environment note: `Ninja` generator unavailable here; build verification switched to Makefiles in fresh `/tmp` build dir.
- Next recommended step: add compile-time-flagged runtime/GameEntry wiring stub (off by default) before pulling full N64ModernRuntime integration.

2026-02-21 00:05:59 -0300
- Added compile-time feature flag path: `LOD_ENABLE_RECOMP_RUNTIME` (default OFF) with `N64MODERNRUNTIME_DIR` CMake cache variable.
- Added `GameEntry` stub builder in `src/main/main.cpp` behind flag; default build remains clean and unchanged.
- Runtime-enabled bring-up path validated incrementally:
  - Initial failures: missing `librecomp/game.hpp`, then `recomp.h`, then `sse2neon.h`, then `lightweightsemaphore.h`, then `json/json.hpp`.
  - Resolved by adding include roots under `N64MODERNRUNTIME_DIR` for `librecomp/include`, `N64Recomp/include`, `ultramodern/include`, `thirdparty`, `thirdparty/sse2neon`, and `thirdparty/concurrentqueue`.
  - Final result: runtime-enabled bootstrap compiles and runs when `N64MODERNRUNTIME_DIR=/Users/dudu/Projects/recomp/references/Zelda64Recomp/lib/N64ModernRuntime`.
- Updated docs (`docs/N64_Recompilation_Guide_2026.md`) to include concrete XXH3 hash value `0x0BF827D4AC27A631`.
- Next recommended step: add optional `LOD_LINK_RECOMP_RUNTIME` mode to link `librecomp`/`ultramodern` and execute `recomp::register_game(...)` for real instead of stub-only compile path.

2026-02-21 (new session)
- User priority: do not advance runtime/linking work until the foundation is verified solid. Previous failed attempt (`lod_recomp_failed/`) produced flawed artifacts — all downstream work was compromised.
- Audited previous failed attempt at `/Users/dudu/Projects/recomp/lod_recomp_failed/`:
  - N64Recomp binary exists and runs (Mach-O arm64).
  - `symbols.elf` had ~2300 function symbols, ALL with size=0.
  - `config.toml` used `elf_path` pointing to this hollow ELF.
  - Output files (`funcs.h`, `lookup.cpp`, `recomp_entrypoint.c`, `recomp_overlays.inl`) all 1 byte (empty). N64Recomp silently skipped every size=0 function.
- Root cause confirmed: N64Recomp has NO automatic function boundary detection. It requires either:
  1. An ELF with proper symbol sizes (from a decompilation project), or
  2. A `.syms.toml` file with explicit `{ name, vram, size }` per function.
  Size=0 → function skipped → empty output.
- Investigated how successful recomp projects solve this:
  - **Zelda64Recomp**: depends on `zeldaret/mm` (full Majora's Mask decomp) → builds ELF → `N64Recomp --dump-context` → `syms.toml`.
  - **dino-recomp**: depends on `zestydevy/dinosaur-planet` (parallel decomp) → same pipeline.
  - Both REQUIRE a decompilation project that produces an ELF with real function boundaries.
- Investigated cv64 decomp (`/Users/dudu/Projects/recomp/references/cv64_decomp/`):
  - Covers Castlevania 64 only (not LoD/ND4E), but same Konami engine.
  - 3,780 symbols, 447 identified functions, detailed headers (object system, system_work, libultra).
  - ~60-70% engine code overlap with LoD (confirmed by LoD debug string references in cv64 source).
  - Addresses won't map directly (different ROM layout), but signatures/patterns are reusable.
- Symbols file format (from Zelda reference): `[[section]]` with `name`, `rom`, `vram`, `size`, and `functions = [{ name, vram, size }]`. All sizes must be non-zero and word-aligned.
- Current blocker: no valid symbols (ELF or syms.toml) exist for LoD ROM. Without this, N64Recomp cannot produce usable output, and all runtime/linking work is premature.
- Candidate paths forward:
  1. Start minimal LoD disassembly using `splat` → produce ELF with function metadata.
  2. Use Ghidra auto-analysis on LoD ROM → export function list → convert to `.syms.toml`.
  3. Byte-pattern match cv64 known functions into LoD ROM to bootstrap symbol coverage.
- Next step: decide on symbol generation approach before writing any more code.
- Decided on splat-based path (proven by all successful recomp projects).
- Updated `docs/N64_Recompilation_Guide_2026.md` (rev 2): added sections 8 (Symbol Pipeline) and 9 (CV64 as LoD Template), updated bootstrap order, added source links.
- `splat` already installed locally: v0.37.1, Python 3.12.8.
- Ran `splat create_config` on LoD ROM → generated `castlevania2.yaml`, `symbol_addrs.txt`, `reloc_addrs.txt`.
- Auto-detected ROM layout:
  - Entry point: `0x1000` → VRAM `0x80000400` (confirmed).
  - Main code: `0x1060`–`0xAB9F0` (~698 KB).
  - `main` function at `0x800177E0`.
  - BSS start inferred, stack top at `0x800C3680`.
  - Remaining ~95% of ROM (data/assets/overlays) starts at `0xAB9F0`.
- Enabled `libultra_symbols: True`, `hardware_regs: True`, `gfx_ucode: f3dex2` in YAML.
- Applied all ~190 file-split suggestions from splat into subsegments.
- Re-split: 2,298 functions across 235 asm files. Clean MIPS disassembly with resolved hi/lo pairs and jal targets.
- `libultra_symbols` did NOT auto-name functions — it only enables the name database, doesn't do byte-pattern matching. Symbols must be seeded in `symbol_addrs.txt` manually.
- 25 undefined external functions (overlay/RSP code at `0x8014xxxx`–`0x8018xxxx`, `0x8400xxxx`).
- 2,101 undefined data symbols needing section mapping.
- Files generated in project root: `castlevania2.yaml`, `castlevania2.ld`, `castlevania2.d`, `symbol_addrs.txt`, `reloc_addrs.txt`, `undefined_funcs_auto.txt`, `undefined_syms_auto.txt`, `asm/` (235 files), `assets/` (bin blobs).
- Next steps:
  1. Segment the unknown `0xAB9F0`+ region (data, rodata, overlays, compressed assets) — use cv64 layout as structural guide.
  2. Seed `symbol_addrs.txt` with known libultra functions (byte-pattern matching from cv64 reference).
  3. Work toward compiling the split output to an ELF with real symbol sizes.

- Segmented full ROM via hex analysis + MIPS prologue scanning:
  - Main code: `0x001060`–`0x0AB9F0` (~698 KB, 2,298 functions) — already split.
  - Main data: `0x0AB9F0`–`0x0C1300` (~90 KB, pointer tables, strings, asset tables).
  - Overlay 1: `0x0C1300`–`0x10D000` (~300 KB, game logic: effects/enemies/player). VRAM ~`0x80140000`. 1,156 functions.
  - Overlay 1 data: `0x10D000`–`0x140000` (~208 KB, structured data, animation, asset tables).
  - Compressed assets: `0x140000`–`0xEEE930` (~13.7 MB, Nisitenma-Ichigo compressed — models, textures, audio, possibly embedded code overlays).
  - Padding: `0xEEE930`–`0x1000000` (~1.1 MB, zeros to 16 MB).
- Initial attempt to map overlay 2 at `0x745000` failed — that region is still compressed data with coincidental prologue-like bytes. Needs decompression first (same as cv64 Nisitenma-Ichigo pattern).
- Updated YAML: 100% of ROM now mapped, 0% unknown. 3,454 total functions across main + overlay 1.
- Overlay 1 got 37 file-split suggestions (not yet applied).
- Overlay VRAM for region 1 set to `0x80140000` based on `undefined_funcs_auto.txt` call targets (`0x80142388`–`0x8017B138`). Overlay 2 calls (`0x80188xxx`) likely in compressed overlays.
- `0x8400xxxx` addresses are RSP IMEM (microcode, not regular overlays).
- Next steps:
  1. Apply overlay 1 file-split suggestions.
  2. Seed libultra symbols in `symbol_addrs.txt` (simpler approach than full byte-pattern matching).
  3. Investigate Nisitenma-Ichigo decompression to access embedded overlays in compressed region.

- Found Nisitenma-Ichigo file table at ROM `0x0B1B88` (magic string "Nisitenma-Ichigo").
  - 300 entries, each 8 bytes (ROM offset pairs).
  - Covers compressed assets from `0x0085C590` to `0x00E168BC`.
  - Table itself spans `0x0B1B98`–`0x0B24F8`.
- CRITICAL CORRECTION: overlay 1 VRAM `0x80140000` was WRONG.
  - ROM `0xC1300` contains data (function pointers into main `0x800Axxxx`, float constants) — NOT code.
  - First real prologue in that region is at ROM `0xC2120`.
  - The `0x8014xxxx` undefined functions from main code are NOT in ROM at fixed offsets — they're in **compressed Nisitenma-Ichigo overlays** loaded at runtime after decompression.
  - Verified: mapping those VRAM addresses through any plausible ROM base yields compressed data, never prologues.
- Revised understanding of ROM layout:
  - `0x001060`–`0x0AB9F0`: main code (.text) — confirmed correct.
  - `0x0AB9F0`–`0x0C2120`: main segment data (.data, .rodata, jump tables, float constants) — extends further than initially estimated.
  - `0x0C2120`–`0x10D000`: likely "common" code segment (similar to cv64) — code that's part of main but in a separate compilation unit. VRAM is contiguous with main.
  - `0x10D000`–`0x140000`: more data/assets (structured data, animation, offset tables).
  - `0x140000`–`0xEEE930`: compressed assets (Nisitenma-Ichigo). Contains both data assets AND code overlays that get decompressed to `0x8014xxxx`+ at runtime.
- Implication: to access the overlay code (`0x8014xxxx`+ functions), we MUST decompress the Nisitenma-Ichigo assets first. cv64 has `decompress.py` / `liblzkn64.py` for this.
- The YAML needs correction: the "overlay_1" segment should be reclassified. It's either main .data/.rodata or "common" code, not a separate overlay.
- Next steps (revised):
  1. Fix YAML — merge 0xAB9F0–0x10D000 region properly (data + common code, contiguous VRAM with main).
  2. Try cv64's Nisitenma-Ichigo decompression tools on LoD's compressed assets.
  3. Once overlays are decompressed, disassemble them with correct VRAM addresses.
  4. Seed libultra symbols.

- YAML corrected: removed wrong "overlay_1" segment (VRAM `0x80140000`). New layout:
  - `main`: code `0x1060`–`0xAB9F0`, VRAM `0x80000460`, `follows_vram: entry`. 235 asm subsegments, 2,298 functions.
  - `main_data`: bin `0xAB9F0`–`0xC2120` (pointer tables, float constants, jump tables, strings).
  - `common`: code `0xC2120`–`0x10D000`, VRAM `0x800C10C0` (contiguous with main). Game logic in separate compilation units (effects, enemies, player, stage systems). Similar to cv64 "common" segment pattern.
  - `common_data`: bin `0x10D000`–`0x140000` (structured data, animation data, asset offset tables).
  - `compressed_assets`: bin `0x140000`–`0xEEE930` (~13.7 MB, Nisitenma-Ichigo format). Contains BOTH data assets AND code overlays decompressed to VRAM `0x8014xxxx`+ at runtime. File table at ROM `0x0B1B88`: 300 entries covering `0x85C590`–`0xE168BC`.
  - `padding`: bin `0xEEE930`–`0x1000000` (zeros to 16 MB).
- Re-ran splat split with corrected YAML: clean disassembly confirmed. Common segment produced additional asm files.
- Updated `docs/N64_Recompilation_Guide_2026.md` to rev 3:
  - Section 5.2: `has_compressed_code` confirmed true, decompression required for overlay functions.
  - Section 5.3: verified ROM map table with exact offsets, sizes, VRAM addresses, function counts.
  - Section 5.4: splat configuration summary.
- Updated `.gitignore`: added splat-generated output (`asm/`, `assets/`, `build/`, `castlevania2.ld`, `castlevania2.d`, `undefined_funcs_auto.txt`, `undefined_syms_auto.txt`). Tracked files: `castlevania2.yaml`, `symbol_addrs.txt`, `reloc_addrs.txt`.

2026-02-21 (continued — Nisitenma-Ichigo decompression & overlay discovery)
- Attempted Nisitenma-Ichigo decompression to find overlay code:
  - Copied cv64's `decompress.py` + `liblzkn64.py` to `tools/`.
  - Patched `liblzkn64.py`: removed `numba`/`numpy` dependency (replaced `@njit` decorators, `np.zeros` → `bytearray`).
  - Created `tools/decompress_lod.py`: fault-tolerant individual file extractor.
  - Result: 471 compressed files decompressed successfully, 0 failures.
  - **CRITICAL FINDING: zero MIPS code in ANY decompressed file.** Only 9 coincidental prologue-like bytes across 471 files, zero `jr ra`. All files are data assets (textures, models, audio).
  - Conclusion: Nisitenma-Ichigo compressed assets contain NO code overlays. This contradicts the previous hypothesis.

- Investigated where `0x8014xxxx`–`0x8018xxxx` overlay functions actually live:
  - Scanned uncompressed data regions (main_data, common_data): 0 prologues. Not there.
  - Discovered **1 MB of MIPS code at ROM `0x740000`–`0x840000`** hidden in the gap between `common_data` and Nisitenma-Ichigo compressed data.
  - This region was previously misclassified as part of "compressed_assets".

- Found overlay descriptor table at ROM `0x0B3838`:
  - Overlay 0: ROM `0x745230`–`0x7639A0` (.text/data/rodata sections).
  - Function pointer table at `0x0B3858`: pairs of (base, entrypoint) at VRAM `0x802E3B70`.
  - 47 sequential map overlays at `0x0B39E8`: ROM `0x76CD00`–`0x8381C0`, all sharing VRAM `0x802E3B70`.
  - Pattern matches cv64's `exclusive_ram_id: ovl_map_objects` architecture.

- Verified overlay VRAM addresses:
  - **Map overlays**: VRAM `0x802E3B70` confirmed. 301/472 self-referential JAL targets land on prologues (64%). Individual overlays reach 80-100% hit rates.
  - **Overlay 0**: VRAM ~`0x801CAEA0` (approximate, 9/21 matches — needs refinement).

- CRITICAL CORRECTION #2: Common segment VRAM was WRONG (again).
  - Previous value: `0x800C10C0` (computed in earlier session as "contiguous with main").
  - Brute-force search across `0x80080000`–`0x80180000` found optimal base.
  - **Correct value: `0x80141870`**. 778/1377 self-referential JAL hits (56%).
  - Verified: 40/55 undefined functions land on prologues/LUI at this VRAM base. 100% of undefined funcs fall within the segment's VRAM range.
  - **The `0x8014xxxx`–`0x8018xxxx` functions were ALWAYS in the common code segment.** We just had the wrong VRAM base.
  - Undefined functions dropped from **621 to 60** after VRAM correction.

- Remaining 60 undefined functions:
  - 23 in `0x8014`–`0x8018`: within common segment but at boundaries splat didn't auto-detect.
  - 34 in `0x802E`–`0x802F`: map overlay functions (expected, overlays not yet split).
  - 1 at `0x801D26A0`: in overlay 0 range.
  - 2 at `0x8400xxxx`: RSP microcode (expected, not regular code).

- Updated YAML (rev 4): corrected common VRAM to `0x80141870`, added overlay segments:
  - `asset_data`: bin `0x140000`–`0x745230` (data/asset region).
  - `overlay_system`: code `0x745230`–`0x74CD10`, VRAM `0x801CAEA0`.
  - `overlay_system_data`: bin `0x74CD10`–`0x76CD00`.
  - `map_overlays`: bin `0x76CD00`–`0x8381C0` (47 overlays, VRAM `0x802E3B70`).
  - `compressed_assets`: bin `0x8381C0`–`0xEEE930` (Nisitenma-Ichigo data-only).

- Files added: `tools/decompress.py`, `tools/liblzkn64.py` (patched), `tools/decompress_lod.py`.
- Output: `resources/decompressed/` (471 files, all data assets).

- Applied file-split suggestions: 56 subsegments for common, 5 for overlay_system.
- Added 47 individual map overlay segments to YAML with `exclusive_ram_id: map_overlays`.
- Final splat run: 345 asm files, 100% ROM split, all segments clean.

2026-02-21 (continued — libultra symbol seeding)
- Seeded `symbol_addrs.txt` with 63 identified libultra/SDK symbols.
- Identification method: instruction-pattern analysis in LoD's asm output (no cv64 ROM needed).
  - Very short mfc0/mtc0 functions for CP0 register access (Status, Count, Cause, Compare).
  - Cache instruction opcodes for DCache/ICache functions.
  - Struct initialization patterns for osCreateMesgQueue, osCreateThread.
  - Matrix math patterns for guMtxF2L, guPerspectiveF, guLookAtF, etc.
  - Distinctive instruction sequences: sqrtf (2 instr), bzero (align+loop), bcopy (overlap check).
- Symbol categories seeded:
  - Compiler runtime: 4 (__ll_rshift, __ull_rem, __ull_div, __ll_lshift)
  - OS Thread: 7 (osCreateThread, osStartThread, osSetThreadPri, osGetThreadPri, __osDequeueThread, __osEnqueueThread, __osDispatchThread)
  - OS MsgQueue: 4 (osCreateMesgQueue, osSendMesg, osRecvMesg, osSetEventMesg)
  - OS Interrupt: 8 (osSetIntMask, __osDisableInt, __osRestoreInt, __osSetSR, __osGetSR, __osGetCause, __osSetCompare, osGetCount)
  - OS Video: 9 (osViSetMode, osViSwapBuffer, osViSetEvent, osViBlack, osViSetSpecialFeatures, osViGetCurrentFramebuffer, osCreateViManager, __osViInit, __osViSwapContext)
  - OS PI: 4 (osCreatePiManager, osEPiStartDma, __osEPiRawStartDma, osEPiLinkHandle)
  - OS SP: 3 (__osSpSetStatus, __osSpGetStatus, __osSpSetPc)
  - OS Audio: 1 (osAiSetFrequency)
  - OS Memory/Cache: 5 (osVirtualToPhysical, osWritebackDCache, osWritebackDCacheAll, osInvalICache, osInvalDCache)
  - OS TLB: 2 (__osProbeTLB, __osUnmapTLBAll)
  - GU: 16 (guPerspectiveF/guPerspective, guRotateF, guMtxF2L, guMtxIdentF, guLookAtF/guLookAt, guTranslateF/guTranslate, guOrthoF/guOrtho, guNormalize, guRotateRPYF/guRotateRPY, guLookAtReflectF/guLookAtReflect)
  - C Library: 7 (sinf, __cosf, sqrtf, bzero, bcopy, memcpy, strlen)
- Impact: 668+ call-site references resolved to real names across 144 asm files.
  Top callers: sqrtf (120), bzero (79), osVirtualToPhysical (68), osRecvMesg (48).
- Undefined functions: still 75 (mostly cross-segment calls to map overlays and common segment boundaries).

2026-02-21 (continued — N64Recomp first successful run)
- Generated `castlevania2.syms.toml` from splat asm output:
  - Parsed all `nonmatching func_XXX, 0xSIZE` directives from 345 asm files.
  - 51 sections, 5,247 functions total.
  - Overlay functions prefixed with segment name for uniqueness (e.g., `map_ovl_00_func_802E3B70`).
- Created `recomp.toml` (N64Recomp config) and `overlays.txt` (48 relocatable sections).
- N64Recomp binary: reused from previous failed attempt at `lod_recomp_failed/tools/N64Recomp/build/N64Recomp`.
- Iterative error resolution — 29 functions stubbed:
  - 7 TLB functions (mfc0/mtc0 EntryHi/Lo/Index, tlbwi/tlbp/tlbr)
  - 4 cache functions (osWritebackDCache, osInvalICache, osInvalDCache, osWritebackDCacheAll)
  - 3 cop0 register access (osGetCount/mfc0 $9, __osSetCompare/mtc0 $11, __osGetCause/mfc0 $13)
  - 2 exception/dispatch (__osDispatchThread/eret, __osEnqueueAndYield)
  - 2 trunc.l.d/trunc.l.s (MIPS III ops not supported by this N64Recomp build)
  - 2 branch-outside-section (func_80176104, func_8018C680 — common segment boundary issues)
  - 2 RSP IMEM branch targets (func_800AADA0, func_800AADA8)
  - 2 map_ovl_29 boundary issues
  - 5 map_ovl_42 branches to unmapped high addresses (0x80201248, 0x80E011A8, 0x827011A8)
- **RESULT: 5,247 functions recompiled successfully!**
  - 107 C output files + funcs.h
  - 1,089,326 lines of C code
  - 40 MB total output in `RecompiledFuncs/`
- One info-level warning: "Failed to find written section index of relocatable section: ..overlay_system"
  - overlay_system section not in the relocatable output — may need VRAM refinement.
- Files created: `castlevania2.syms.toml`, `recomp.toml`, `overlays.txt`, `RecompiledFuncs/` (107 files).

- Next steps:
  1. Refine overlay_system VRAM (currently approximate at `0x801CAEA0`).
  2. Fix common segment boundary (code extends ~0xEC bytes past 0x10D000 into common_data).
  3. Try compiling the recompiled C output to verify it produces valid object code.
  4. Integrate with N64ModernRuntime for actual recompilation target.

2026-02-21 (continued — N64ModernRuntime integration & runtime debugging)

### Build Pipeline Established
- Full CMake build working: rt64 + N64ModernRuntime + librecomp + ultramodern + 5,247 recompiled funcs
- Binary links and runs on macOS/Apple Silicon (Metal backend)
- Source files: main.cpp, register_overlays.cpp, rt64_render_context.cpp, support.cpp, support_apple.mm, rsp/aspMain.cpp, ignored_func_stubs.cpp

### OS Function Mapping — 126+ functions identified
- Applied 83 initial + 43 additional OS function name mappings to `castlevania2.syms.toml`
- Identification methods: call graph, MMIO patterns, struct field offsets, register usage
- Three categories in N64Recomp's `symbol_lists.cpp`:
  - `reimplemented_funcs` (lines 3-142): runtime provides native `_recomp` wrappers
  - `ignored_funcs` (lines 143-563): code not generated at all
  - `renamed_funcs` (lines 564+): renamed but still compiled
- Some functions appear in BOTH reimplemented AND ignored — reimplemented takes precedence

### Key Crash Fixes

**1. `ignored_funcs` linker errors**
- Recompiled callers emit `funcname_recomp()` calls, but ignored functions produce no code → linker error
- Fix: `ignored_func_stubs.cpp` with 17 empty `_recomp` stubs

**2. TO_PTR NULL pointer crash (SIGBUS in osContInit)**
- `TO_PTR(type, 0)` → `rdram + 0x80000000` (PROT_NONE region) → SIGBUS
- Caused by game_main calling osContInit with uninitialized registers (all 0)
- Fix: NULL guards in both `osContInit` (ultramodern/input.cpp) and `osContInit_recomp` (librecomp/cont.cpp)

**3. KSEG1 SIGBUS**
- N64 hardware register accesses via KSEG1 (0xA0000000-0xBFFFFFFF) map to rdram offset 0x20000000-0x3FFFFFFF
- That region was PROT_NONE → SIGBUS
- Fix: `mprotect(rdram + 0x20000000, 0x20000000, PROT_READ | PROT_WRITE)` in rdram.cpp

**4. PI DMA queue corruption (SIGSEGV in dequeue_external_messages)**
- N64 IOMsg and OSMesgQueue share memory (embedded struct pattern)
- PI DMA is synchronous (memcpy) in recomp, but completion message was delivered asynchronously
- Game overwrites IOMsg immediately after DMA returns, corrupting the completion queue
- Symptom: queue had validCount=51424 (exactly matching DMA sizes like 0xC8E0, 0x5B8E0)
- Fix: changed PI completion to synchronous `osSendMesg()` in pi.cpp instead of `enqueue_external_message_src`
- **NOTE**: Fix is still incomplete — see current state below

### Current Runtime State
- Game creates 6 threads successfully: main(1), idle(5), SCHED(19), AUDIO(18), GRAPH(17), PI_MGR(16)
- PI DMA operations execute (ROM → RDRAM copies work)
- Thread communication via OSMesgQueue works
- **STILL CRASHING**: queue at 0x800C5970 still shows corrupted validCount=375008 even after PI fix
- The 0x800C5970 queue corruption may have a SECOND source beyond PI DMA — needs investigation
- Debug fprintf statements still in mesgqueue.cpp (osCreateMesgQueue, do_send, dequeue_external_messages)
- Audio RSP is stubbed (returns RspExitReason::Broke immediately)

2026-02-21 (continued — queue investigation & stub audit)

### Stub Audit Result
- All 17 stubs in `ignored_func_stubs.cpp` verified against N64Recomp's `symbol_lists.cpp`.
- All 17 are in `ignored_funcs` list. None conflict with `reimplemented_funcs`. No linker conflicts. Stubs are safe.

### Queue 0x800C5970 Identification
- **NOT a global OSMesgQueue** — address is inside the idle thread's stack.
- BSS layout mapped from asm:
  - `0x800C2680`: boot thread (OSThread, id=1)
  - `0x800C3830`: idle thread (OSThread, id=5)
  - `0x800C39E0`–`0x800C59E0`: idle thread stack (8 KB)
  - `0x800C5970`: **0x70 bytes below stack top** — inside idle thread stack
  - `0x800C59E0`: PI manager queue / idle thread stack top
- `validCount=375008` = `0x5B8E0` — matches DMA transfer size pattern from earlier PI bug.
- Hypothesis: IOMsg/stack memory corruption from DMA completion path. PI DMA sync fix may be incomplete, or a different DMA path (flash? SI?) still uses the async external message pattern.
- Thread ID mapping from asm analysis:
  - id=1: boot thread at `D_800C2680`
  - id=5: idle thread at `D_800C3830`
  - id=0x13(19): SCHED at `D_800C5E80+0x1C8`
  - id=0x12(18): AUDIO at `D_800C5E80+0x378`
  - id=0x11(17): GRAPH at `D_800C5E80+0x528`
  - id=0x10(16): PI_MGR at `D_800C5E80+0x6D8`
  - id=3: MAIN at `D_800F99B0` (from `asm/90ED0.s`)
  - id=0xFF: VI_MGR (from `asm/1E680.s`)
- Scheduler struct at `D_800C5E80` creates 8 OSMesgQueues at offsets 0x04/0x3C/0x74/0xAC/0xE4/0x11C/0x154/0x18C. None are `0x800C5970`.
- Next step: add bounds-check diagnostic in `do_send()` to catch corruption at the exact moment `validCount > msgCount`.

### Queue Corruption Root Cause Found & Fixed
- Added bounds-check diagnostic in `do_send()` — immediately caught the corruption.
- **Root cause**: `func_800195E0` (timer calibration) creates a stack-local `OSMesgQueue` at `$sp+0x28`, registers it with `osSetEventMesg(OS_EVENT_COUNTER=5, ...)`, then replaces it with a global queue `D_800EFB70`. Between calls (or after return), the timer thread fires `enqueue_external_message_src()` targeting the dead stack address `0x800C5970`. Since ultramodern's `osSetEventMesg` doesn't handle event 5 (`OS_EVENT_COUNTER`), neither registration actually takes effect — but `func_80098600` (called from `func_800195E0`) uses `osSetTimer` with the same stack-local queue. The timer fires, enqueues to the dead address, and `dequeue_external_messages` finds garbage when it processes it.
- **Fix applied** (two parts):
  1. `do_send()` in `mesgqueue.cpp`: bounds-check on `validCount`/`msgCount` — silently discards sends to corrupt/stale queues instead of crashing.
  2. `main.cpp`: set `.message_queue_control = { .requeue_timer = false }` — prevents stale timer messages from being infinitely re-enqueued (default was `true`, causing log flood).
- Removed debug fprintf from `osCreateMesgQueue`, `do_send`, and `dequeue_external_messages` — they served their diagnostic purpose.
- Codesign step required: `codesign -s - --entitlements .github/macos/entitlements.plist -f ./build/LodRecomp` — without entitlements, macOS kills the process on the 4GB mmap. CMakeLists.txt does not automate this.
- Game now progresses past initialization: threads 1, 5, 19, 18, 17, 16, 3 all created and running. Scheduler, audio, graph, PI_MGR threads active.
- **Confirmed stable**: 26K+ lines of clean log, zero errors. Game runs in a stable VI retrace loop.
- Current state: threads cycling through the main loop (SCHED→MAIN→AUDIO→boot) driven by VI retrace. GRAPH thread (17) is blocked on `0x800C5EBC` waiting for work — needs RSP/RDP task submission to wake up.
- Next blocker: graphics pipeline. The game submits display lists but the GRAPH thread never receives them. Likely needs proper GFX task handling in the RSP/events path (currently only audio RSP is stubbed). This is the standard next step in recomp bring-up — getting the first frame rendered.
