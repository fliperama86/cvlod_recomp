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

2026-02-21 (continued — GFX pipeline investigation)

### Diagnostic Round 1: RSP task tracing
- Added fprintf diagnostics in 3 locations:
  1. `osSpTaskStartGo_recomp` (sp.cpp): logs every RSP task with type and data_ptr
  2. `submit_rsp_task` (events.cpp): logs GFX vs non-GFX routing
  3. `osSendMesg`/`osJamMesg`/`osRecvMesg` (mesgqueue.cpp): traces messages targeting GRAPH queue `0x800C5EBC`

### Key Finding: No GFX tasks are ever submitted
- Over 10 seconds of runtime, ONLY `type=2` (`M_AUDTASK`) tasks reach `osSpTaskStartGo`. Zero `type=1` (`M_GFXTASK`).
- GRAPH thread (17) blocks on `osRecvMesg(0x800C5EBC, blocking=yes)` with `validCount=0` and never wakes.
- No message is ever sent to `0x800C5EBC` (GRAPH queue) — neither via `osSendMesg` nor `osJamMesg`.
- Audio path works correctly: AUDIO thread (18) → scheduler → AUDIO thread → osSpTaskStartGo(type=2) cycles normally.

### Architecture Understanding: Scheduler task flow
- Scheduler struct at `D_800C5E80`, initialized by `func_800184E0`:
  - `sched[0] = 1` (halfword) — VI retrace task type code (→ GFX)
  - `sched[2] = 3` (halfword) — PRE-NMI/audio task type code
  - Queue at `sched+0xAC`: receives VI retrace (0x29A) and PRE-NMI (0x29D) events
  - Queue at `sched+0x3C` (`0x800C5EBC`): GRAPH thread input queue
  - Queue at `sched+0x4` (`0x800C5E84`): AUDIO thread input queue
- SCHED thread (`func_80018774`, thread 19):
  - Receives VI retrace → calls `func_80018968(sched, sched)` → dispatches tasks with type `1`
  - Receives PRE-NMI → calls `func_80018968(sched, sched+2)` → dispatches with type `3`
  - `func_80018968` walks linked list at `sched+0x888`, jams messages to task reply queues
  - **SCHED thread IS running and receiving VI retraces** (confirmed in logs)
- MAIN thread (`func_800905D8`, thread 3):
  - Submits ONE initial task to scheduler via `func_80018890` at startup
  - Waits on `D_800F9B60` for completion messages
  - Message type `1` → calls `func_80090744` (frame builder) → builds display list → submits GFX task
  - Message type `3` → skip (audio completion)
  - Message type `0x20` → exit
- GFX tasks bypass `get_rsp_microcode` entirely — they go directly to the runtime's graphics thread via `action_queue` in `submit_rsp_task`

### Diagnostic Round 2: Tracing frame building and task flow
- Added fprintf in recompiled `func_80090744` (funcs_37.c) to trace `func_8009CA90` return value (display list builder).
  - Required `#include <stdio.h>` (not `<cstdio>` — recompiled C files are compiled as C, not C++).
- **Result: `func_80090744` IS being called repeatedly.** `func_8009CA90` returns non-zero values (e.g., `dl_ptr=0x0000009B`, `0x000000BA`, `0x000000D9`). Display lists ARE being built.
- But the task `func_80090744` submits via `osJamMesg(sched+0x4, ...)` is always `type=2` (AUDIO task), not GFX.
- **`func_80090744` is an AUDIO task builder**, not a GFX task builder.

### Confirmed Architecture: Separate GFX and AUDIO submission paths
- **AUDIO path** (working): Main thread (`func_80090744`) → `osJamMesg(sched+0x4)` → AUDIO thread → `osSpTaskStartGo(type=2)` ✓
- **GFX path** (broken): `func_80019034` → `func_80018E30(sched, graph_queue, task)` → `osJamMesg(sched+0x3C)` → GRAPH thread → `osSpTaskStartGo(type=1)` ✗ Never happens
- `func_80019034` builds the GFX OSTask (type=1), writes RDP end commands (0xE9000000, 0xDF000000), and sends to GRAPH thread via `func_80018E30`.

### GFX task submission callers (who invokes `func_80019034`)
1. `func_80017EC0` (in funcs_11.c) — called from thread 5 entry function `func_800178D0` (render/drawing thread, priority 0xA).
2. `func_80000A30` (in funcs_0.c) — called indirectly via function pointer (overlay/recomp_overlays.inl entry).

### `osSpTaskYielded` fix applied (necessary but not sufficient)
- Changed `osSpTaskYielded_recomp` in `lib/N64ModernRuntime/librecomp/src/sp.cpp` from returning 0 to returning 1.
- Rationale: AUDIO thread checks `osSpTaskYielded` after running audio tasks. If non-zero, it runs the pending GFX task stored at `sched->0x88C`. Previous `return 0` prevented this.
- **Not sufficient alone**: `sched->0x88C` is only set by the GRAPH thread when it receives a GFX task. Since no GFX tasks reach the GRAPH thread, `sched->0x88C` stays 0 regardless.

### GRAPH thread / `sched->0x88C` mechanism
- `sched->0x888`: linked list head for pending tasks (used by scheduler dispatch)
- `sched->0x88C`: current GFX task pointer — set ONLY by GRAPH thread (`func_80018B50`) when it receives a GFX task on queue `sched+0x3C`
- `sched->0x898`: frame submission counter (incremented by `func_80018E30`, capped at 3)
- `sched->0x89C`: another counter (checked by `func_80000A30`, if >= 2 skips GFX submission)
- GRAPH thread flow: receive on `sched+0x3C` → store at `sched->0x88C` → `osSpTaskLoad` + `osSpTaskStartGo` → clear `sched->0x88C = 0`
- AUDIO thread checks `sched->0x88C` after audio: if non-zero, participates in GFX/audio interleaving via yield mechanism

### Diagnostic Round 3: Is `func_80019034` ever called?
- Added fprintf at entry of recompiled `func_80019034` (funcs_12.c). Added `#include <stdio.h>` to funcs_12.c.
- **Result: `func_80019034` is NEVER called.** Zero invocations in 15 seconds of runtime.
- This confirms the problem is upstream — thread 5 (`func_800178D0`) either never runs, never reaches `func_80017EC0`, or `func_80017EC0` never reaches the `func_80019034` call. Similarly `func_80000A30` is never invoked.

### Diagnostic Round 4: Thread 5 crash & lldb investigation
- Initial fprintf diagnostic in `func_800178D0` (funcs_11.c) caused a SIGSEGV.
- Used lldb to catch crash: `EXC_BAD_ACCESS (code=1, address=0x4000b008a)` at `func_800178D0 + 2844`.
- **Root cause of crash: bad diagnostic code.** `MEM_HU(0X800B0088, 0)` in fprintf used a 32-bit positive literal `0x800B0088` which doesn't get sign-extended to 64-bit, causing the address subtraction `- 0xFFFFFFFF80000000` in the macro to produce an out-of-bounds address (`rdram + 0x1000B008A`). Lesson: NEVER use raw N64 addresses in MEM_* macros in diagnostic code — they need proper sign extension or use register values as the recompiled code does.
- Fixed by removing the `MEM_HU` call from the diagnostic fprintf.

### Diagnostic Round 4 (continued): Thread 5 runs but only gets one message
- After crash fix, added checkpoints throughout `func_800178D0` init sequence.
- **Thread 5 runs fully**: enters → `func_8001A350` → `osUnmapTLBAll` → bzero → struct init → main loop. All checkpoints reached.
- Thread 5 main loop at `L_80017C78`: `osRecvMesg(0x800C5D38, blocking)` → checks msg type → dispatches.
- **Receives exactly ONE message: type=1** (VI retrace completion from scheduler dispatch).
- On type=1, checks `D_800B0088` (halfword at `0x800B0088`):
  - If `D_800B0088 == 0` → calls `func_80017DB8` (init/calibration) → loops back
  - If `D_800B0088 != 0` → calls `osViSetYScale`, `osViSetXScale`, `osViBlack(1)`, then `func_80017EC0` (GFX submit!) → loops back
- After the first message, thread 5 blocks forever on `osRecvMesg` — no more messages arrive.

### ROOT CAUSE FOUND: `OS_EVENT_PRENMI` not supported by runtime

- `D_800B0088` is set to 1 ONLY by the SCHED thread (`func_80018774`) when it receives message `0x29D`.
- Message `0x29D` is registered via `osSetEventMesg(OS_EVENT_PRENMI=14, sched_queue, 0x29D)` during scheduler init (`func_800184E0`).
- **`osSetEventMesg` in `ultramodern/src/events.cpp` (line 143-163) does NOT handle event 14 (`OS_EVENT_PRENMI`).** The switch only covers `OS_EVENT_SP`, `OS_EVENT_DP`, `OS_EVENT_AI`, `OS_EVENT_SI`. Event 14 falls through silently.
- Therefore `0x29D` is never sent to the scheduler queue, `D_800B0088` stays 0, and thread 5 never transitions from init to the GFX rendering loop.
- This game uses `OS_EVENT_PRENMI` as a "video system ready" initialization signal — unusual (normally PRE-NMI means reset button pressed), but confirmed by code analysis:
  - The `0x29D` handler in SCHED calls `func_80091108` (video init), `osViSetYScale(1.0)`, `osViSetXScale(1.0)`, `osViBlack(1)` (unblank screen), stores `1` to `D_800B0088`, then dispatches tasks.
  - This is the ONE-TIME initialization that unlocks the GFX rendering loop.

### Scheduler event registration (from `func_800184E0`)
- `osViSetEvent(sched+0xAC_queue, 0x29A, retrace_count)` — VI retrace → `0x29A`
- `osSetEventMesg(OS_EVENT_SP=4, sched+0xAC_queue, 0x29B)` — SP done → `0x29B`
- `osSetEventMesg(OS_EVENT_DP=9, sched+0xAC_queue, 0x29C)` — DP done → `0x29C`
- `osSetEventMesg(OS_EVENT_PRENMI=14, sched+0xAC_queue, 0x29D)` — PRE-NMI → `0x29D` ← **SILENTLY IGNORED**

### Current state of diagnostics in codebase
- `sp.cpp`: fprintf in `osSpTaskStartGo_recomp` (task type/data_ptr)
- `sp.cpp`: `osSpTaskYielded_recomp` returns 1 (was 0)
- `events.cpp`: fprintf in `submit_rsp_task` (GFX vs non-GFX routing)
- `mesgqueue.cpp`: fprintf in osSendMesg/osJamMesg/osRecvMesg for GRAPH queue `0x800C5EBC`
- `funcs_37.c`: fprintf in `func_80090744` tracing dl_ptr (+ `#include <stdio.h>`)
- `funcs_12.c`: fprintf at entry of `func_80019034` (+ `#include <stdio.h>`)
- `funcs_11.c`: fprintf at entry/checkpoints of `func_800178D0` (+ `#include <stdio.h>`)

### FIX APPLIED: OS_EVENT_PRENMI support (Option 2)
- Added `prenmi` struct (mq, msg, fired flag) to `events_context` in `events.cpp`.
- Added `case OS_EVENT_PRENMI:` to `osSetEventMesg` switch — registers the queue/msg pair.
- Fire the event ONCE on the first VI retrace after game start (in the VI thread loop, after VI/AI messages).
- Uses `enqueue_external_message` (not `_src`) with `requeue_if_blocked=false` — one-shot, no requeue.
- **RESULT: GFX pipeline unlocked!**
  - Thread 5 receives `type=3` (PRE-NMI message `0x29D`) → sets `D_800B0088 = 1`
  - Subsequent `type=1` messages now take the GFX path → `func_80017EC0` → `func_80019034`
  - `func_80019034` builds GFX OSTask (type=1) and sends to GRAPH thread via `func_80018E30`
  - GRAPH thread receives, calls `osSpTaskStartGo(type=1)` → `submit_rsp_task` → RT64 `action_queue`
  - **First GFX frame submitted: `data_ptr=0x801B42C0`**
  - ~5 GFX tasks submitted in 10 seconds (throttled by RT64 display list processing)
- Files changed: `lib/N64ModernRuntime/ultramodern/src/events.cpp`

### Milestone: GFX tasks flowing to RT64 renderer
- Both AUDIO (type=2) and GFX (type=1) RSP tasks now reach the runtime.
- GFX tasks route to RT64's `action_queue` for display list interpretation.
- Frame rate is low (~0.5 fps) — likely blocked in RT64 display list processing or waiting for completion signals.

### Diagnostic Round 5: Low frame rate investigation (2026-02-21)

**Verified facts (via lldb SIGSTOP-attach, no debugger interference):**
- RT64 Workload thread is ALIVE and healthy (no crash). Previous EXC_BREAKPOINT crash was debugger-induced.
- Only 3 `send_dl` calls in 15 seconds — RT64 processes them instantly, not the bottleneck.
- All game threads blocked on `osRecvMesg` → `semaphore_wait_trap`.
- Game thread `func_800178D0` (SCHED/thread 5) received 800+ msg type=1 (VI retrace) but only 3 msg type=2 (dp_complete) before they stop arriving.
- `func_80019034` (GFX submit) called 5 times total, then stops — because it waits for msg type=2 after each submit.
- 5 GFX tasks submitted but only 3 `send_dl` — 2 GFX tasks were dropped/lost.

**Root cause hypothesis: message queue overflow dropping dp_complete/sp_complete signals**
- `dp_complete()` and `sp_complete()` call `enqueue_external_message_src()` which calls `do_send(block=false)`.
- If the target queue is full, `do_send` returns false.
- `requeue_dp` and `requeue_sp` both default to false in `message_queue_control`.
- Therefore if the scheduler's message queue is full of VI retrace messages when dp_complete arrives, the dp_complete message is SILENTLY DROPPED.
- The game then waits forever for the "frame done" signal → deadlock.
- VI thread fires at 60Hz, flooding the scheduler queue. If the queue depth is small (e.g., 8 slots), it fills up almost immediately.

**Fix options:**
1. Set `requeue_sp = true` and `requeue_dp = true` in message_queue_control config — messages will be re-enqueued until the queue has space.
2. Increase the game's scheduler message queue depth (harder — would need to patch recompiled code).
3. Both.

### Fix attempt: requeue SP/DP (2026-02-21)
- Applied `.requeue_sp = true, .requeue_dp = true` in message_queue_control.
- Result: msg type=2 now delivered (msgs #4,#5,#7) — better than before (was 0), but still stalls after 3 frames.
- `func_80019034` called 5 times, only 3 GFX tasks reach `osSpTaskStartGo` — game's internal scheduler drops 2.

### Diagnostic: update_screen and osViSwapBuffer tracing
- `update_screen`: 600+ calls in 20s — RT64 asked to present every VI retrace. Working.
- `osViSwapBuffer`: **only 1 call** (fb=0x80200000) — game sets framebuffer once after send_dl #1, then never again.
- Timeline: update_screen x3 → send_dl #1 → osViSwapBuffer #1 → update_screen #4 → send_dl #2 → send_dl #3 → then only update_screen forever.
- Game's double-buffer logic stalls: it rendered into both buffers but never gets "buffer freed" signal.

### Current hypothesis: scheduler internal GFX slot deadlock
- Game scheduler has a limited number of GFX task "slots" (likely 2, matching double-buffered framebuffers).
- After submitting GFX tasks, it waits for DP_COMPLETE to free the slot before allowing the next submit.
- With requeue enabled, DP_COMPLETE messages DO arrive (3 of them), but the game submitted 5 tasks total.
- 2 tasks never reached osSpTaskStartGo → scheduler internally rejected them (slot occupied).
- After all slots fill up, the game spins waiting for a free slot that will never come.

### Scheduler architecture deep dive (2026-02-21)
- Scheduler struct has multiple queues: `sched+0xAC` (events), `sched+0xE4` (SP task done), `sched+0x11C`, etc.
- `osSetEventMesg(OS_EVENT_SP)` → `sched+0xAC` (main event queue, receives 0x29B)
- Audio/task thread (`func_800189B8`) waits on `sched+0xE4` for task completion — NOT sched+0xAC.
- `func_80018968` dispatches from a linked list at `sched->0x888` — sends messages to registered client queues.
- `sched->0x88C`: currently running GFX task ptr (0 = none). When non-zero, scheduler yields it before starting new task.
- `sched->0x890`: current task being run
- `sched->0x894`: DP complete notification flag
- `sched->0x888`: linked list of event notification clients

### Key observation: osViBlack(1) called every frame
- msg type 1 (VI retrace) handler calls `osViBlack(1)` → blanks screen
- msg type 2 (dp_complete) handler calls `osViBlack(0)` → unblanks screen
- Since type 2 stops arriving after 3 frames, screen stays permanently black

### Recompiled code quality
- Mechanically appears correct — direct MIPS-to-C translation with proper delay slot handling
- Cannot fully verify without original disassembly comparison
- The `bnel` (branch-not-equal-likely) pattern looks correctly handled with goto/skip

### CV64 decomp cross-reference (2026-02-21)
Source: `/Users/dudu/Projects/recomp/references/cv64_decomp` (k64ret/cv64)
- `include/ultra64/PR/sched.h` — N64 SDK scheduler structs (OSSched, OSScTask, OSScClient, OSScMsg)
- `linker/symbol_addrs.txt` — named functions for CV64

**Function name mapping (LoD addr → CV64 name → CV64 addr):**
| LoD Address | Name | CV64 Address | Description |
|---|---|---|---|
| 0x800184E0 | scheduler_create | 0x80015aa0 | Init scheduler struct, create queues, register events |
| 0x8001876C | scheduler_getAudioCmdQ | 0x80015d2c | Returns audio request message queue |
| 0x80018774 | scheduler_eventHandler | 0x80015d44 | Scheduler thread: receives VI/SP/DP events, dispatches |
| 0x80018968 | scheduler_eventBroadcast | 0x80015ed8 | Iterates client list, sends msg to each client's queue |
| 0x800189B8 | scheduler_executeAudio | 0x80015f28 | Audio thread: runs audio+GFX tasks on RSP |
| 0x80018B50 | scheduler_executeGraphics | 0x800160b0 | Graphics thread (thread 17) |
| 0x80018E30 | scheduler_taskDispatch | — | Sends task to scheduler via osJamMesg, checks OS_SC_SWAPBUFFER |
| 0x80019034 | createGraphicTasks | 0x80016440 | Builds GFX task struct, submits to scheduler |
| 0x800178D0 | graphThread_entrypoint | — | Thread 5: main render loop, receives retrace/done msgs |
| 0x800905D8 | mainThread_entrypoint | — | Main game logic thread |

**OSSched struct field mapping (from sched.h + recompiled code analysis):**
| Offset | Type | Name | Description |
|---|---|---|---|
| 0x000 | short | retrace_count | |
| 0x002 | short | numFields | |
| 0x004 | OSMesgQueue | cmdQ | Task command queue |
| 0x03C | OSMesgQueue | queue2 | |
| 0x074 | OSMesgQueue | queue3 | |
| 0x0AC | OSMesgQueue | interruptQ | Main event queue (VI, SP, DP, PRE-NMI events) |
| 0x0E4 | OSMesgQueue | spDoneQ | SP task completion queue |
| 0x11C | OSMesgQueue | queue5 | |
| 0x154 | OSMesgQueue | dpDoneQ | DP completion notification queue |
| 0x18C | OSMesgQueue | queue7 | |
| 0x888 | OSScClient* | clientList | Linked list of event notification clients |
| 0x88C | OSScTask* | curRSPTask | Currently running GFX task on RSP |
| 0x890 | OSScTask* | curTask | Current task being run |
| 0x894 | u32 | dpNotify | DP completion notification pending |
| 0x898 | u32 | swapCount | Swapbuffer tracking counter |
| 0x89C | u32 | swapCount2 | Swapbuffer tracking counter 2 |
| 0x8A0 | u32 | field_8A0 | Init to 1 |
| 0x8A4 | u32 | field_8A4 | Init to 1 |

**OSScTask struct (from sched.h):**
| Offset | Type | Name | Description |
|---|---|---|---|
| 0x00 | OSScTask* | next | Linked list pointer |
| 0x04 | u32 | state | |
| 0x08 | u32 | flags | OS_SC_SWAPBUFFER=0x40 |
| 0x0C | void* | framebuffer | Used by graphics tasks |
| 0x10 | OSTask | list | The actual RSP task struct (0x40 bytes) |
| 0x50 | OSMesgQueue* | msgQ | Where to send completion msg |
| 0x54 | OSMesg | msg | Completion message value |

**Key insight: OS_SC_SWAPBUFFER (0x40) flag**
- `scheduler_taskDispatch` checks `task->flags & 0x40`
- When set, scheduler tracks swapbuffer state in sched+0x898/0x89C
- The scheduler only calls osViSwapBuffer when the flagged task completes
- This is why only 1 osViSwapBuffer call happens — the flag/completion tracking stalls

### Updated symbol_addrs.txt
- Added all scheduler and game function names to `symbol_addrs.txt`

### Next step
- With named functions, can now investigate the specific stall point more methodically
- Key question: does `scheduler_executeAudio` receive the SP completion message on `sched+0xE4`?
- The SP event goes to `sched+0xAC` (interruptQ), handled by `scheduler_eventHandler`, which should forward to `scheduler_executeAudio`'s completion queue
- Need to trace this forwarding path to find where the message gets lost

### 2026-02-21 — Process management lesson (post-reboot)
- **Problem**: launching LodRecomp with `&` (shell background) and killing with `kill -9` caused UNE (uninterruptible sleep) zombie processes. Accumulated 5+ unkillable processes that required a reboot.
- **Root cause**: the app opens a macOS Metal/AppKit window (WindowServer connection). When killed in background without proper GUI teardown, the WindowServer connection gets stuck → UNE state.
- **`sample` tool limitation**: couldn't see real threads (only showed 160K footprint, stuck in dyld) — even for the "alive" process.
- **lldb attach also failed**: `lldb -b -o "process attach -p $PID"` → "Aborted" error. SIP/entitlements prevent external attach to JIT-entitled processes.
- **Safe approach going forward**:
  1. NEVER background the app with `&` — use `lldb -b -o "run"` which manages the full process lifecycle
  2. ALWAYS wrap with `timeout` to ensure termination: `timeout 20 lldb -b -o "run" ...`
  3. Use fprintf diagnostics instead of external debugger attachment for thread inspection
  4. After each test, verify no stale processes: `pgrep -la LodRecomp`
- **Next diagnostic**: add fprintf tracing in scheduler forwarding path (scheduler_eventHandler → spDoneQ) to trace where SP/DP completion messages get lost

### 2026-02-21 — Critical fix: osViGetNextFramebuffer misidentification

#### Root cause
- `func_80099970` at vram 0x80099970 was misidentified as `osGetThreadId` in `castlevania2.syms.toml`.
- The function loads `D_800BB914 + 0x4` — actually `__osViNext->buffer` (next framebuffer pointer), NOT `__osRunningThread->id`.
- Both `osViGetCurrentFramebuffer` (D_800BB910+0x4) and this function (D_800BB914+0x4) have identical instruction patterns but operate on different globals.
- The misidentification caused `osGetThreadId_recomp` to be called (returns small integer thread ID) instead of `osViGetNextFramebuffer_recomp` (returns framebuffer pointer).
- In `scheduler_executeGraphics` (func_80018B50), the double-buffer wait logic compares `osViGetNextFramebuffer() == task->framebuffer` to decide if the framebuffer is still pending display. With the wrong function, the comparison was always `threadId != framebufferPtr`, breaking the swap wait logic.
- Result: after 3 frames, `curFB == taskFB` triggered the swap wait loop, which called `func_80018890` + `osRecvMesg(sched+0x154)` in a tight loop because `osViGetNextFramebuffer()` never returned the expected value.

#### Fix applied
1. Renamed `osGetThreadId` → `osViGetNextFramebuffer` in `castlevania2.syms.toml` (vram 0x80099970).
2. Search-and-replaced `osGetThreadId_recomp` → `osViGetNextFramebuffer_recomp` in `RecompiledFuncs/funcs_12.c`, `funcs.h`, `recomp_overlays.inl`.
3. Fixed N64Recomp truncation artifacts in `funcs.h`, `funcs_102.c`, and `recomp_overlays.inl` (overlay_system section causes tool crash at output phase — manually completed the truncated entries).
4. Made SDL audio device failure non-fatal (CoreAudio error -66681 intermittent on macOS after repeated test sessions).

#### Other changes (reverted to Zelda-aligned approach)
- `sp_complete()` / `dp_complete()` in `events.cpp`: reverted back to `enqueue_external_message_src` with `requeue_sp=true, requeue_dp=true` (not `osSendMesg` — runtime's `osSendMesg` routes non-game-thread calls to `enqueue_external_message` with `requeue=false`, silently dropping messages).
- `osSpTaskYielded_recomp` in `sp.cpp`: returns 0 (task yield requests ignored, matches Zelda).
- `message_queue_control` in `main.cpp`: `requeue_sp=true, requeue_dp=true` re-enabled.

#### Result
- GFX pipeline now renders continuously: 17 frames in 25s (~0.7 fps).
- Framebuffers cycle correctly: 0x80200000, 0x80225800, 0x8024B000.
- `nextFB!=taskFB` check consistently passes — no more swap wait deadlock.
- Frame rate is still low; likely bottlenecked by audio scheduling overhead and other factors.

#### Next steps
- Investigate low frame rate (0.7 fps target ~20+ fps).
- Check if audio stub completion rate floods SP done queue, starving GFX tasks.
- Consider profiling the game thread scheduling to identify bottleneck.

2026-03-18 18:00:52 -0300
- User confirmed new direction: isolate work in a clean worktree/branch from `main`; prioritize game logic and state progression correctness, with graphics secondary for now.
- Read-only verification: superproject `main` still reports dirty submodule state in `lib/N64ModernRuntime`; no extra worktree existed after aborted create attempt.
- Safe pattern for this session: diagnose from a clean superproject branch/worktree so submodule-local edits do not contaminate logic-state debugging.
- Next recommended step: create sibling worktree `../lod-state-diag` on branch `diag/state-main`, then inspect runtime loop/state progression paths before any build/run changes.

2026-03-18 18:52:39 -0300
- Current logic-first blocker: controller input is still not reaching parsed pad state (`0x800EFBA0`) or game input state (`0x801C87F4`) even though rendering works and SI DMA logs show injected buttons.
- Read-only trace narrowed this to the PIF/SI path: game-side parser `func_80098BD4` reads error bits from byte 2 of each 8-byte slot and button/stick data from bytes 4-7, while formatter `func_80098C60` builds the command slots before WRITE DMA.
- Working hypothesis: `__osSiRawStartDma_recomp` is bypassing the expected WRITE->PIF RAM->READ round-trip and/or populating the READ buffer with the wrong logical slot layout, so the game keeps seeing zeroed parsed input.
- Next recommended step: make one minimal, isolated change in the diagnostics worktree to emulate a persistent 64-byte PIF RAM shadow across WRITE/READ and only fill bytes 4-7 of each controller slot on READ; then rebuild and retest state progression.

2026-03-18 18:55:12 -0300
- Applied one isolated SI/PIF-path change in the diagnostics worktree: preserve a 64-byte PIF RAM shadow across WRITE/READ and inject controller response bytes into slot bytes 4-7 (matching `func_80098BD4` expectations).
- Rebuild succeeded and runtime test confirmed the injected logical PIF bytes now look correct (`FF 01 04 01 ...`) while the intro still renders and high-level state remains stuck (`scene_arg=0x00000004`, `guard=1`, `exec_flags` eventually forced to `0x380080C8`).
- New diagnosis: the current `[PAD]` logger is itself invalid because it reads 16-bit N64 fields with raw host pointers, but the recomp runtime stores halfwords at `addr ^ 2`; the zero-valued parsed/game pad logs are therefore not trustworthy yet.
- Next recommended step: fix the diagnostics logger to use correct N64 halfword reads, rerun once, and only then decide whether input is truly broken downstream of SI/PIF or whether the blocker is elsewhere in the forced main-loop/state hacks.

2026-03-18 18:59:18 -0300
- Fixed the `[PAD]` diagnostics to read N64 halfwords correctly; rerun confirmed the zero pad state is real, not a logging artifact. Parsed pad data at `0x800EFBA0` and game input state at `0x801C87F4` remain zero even when SI logs show injected `0x9000` button data.
- Strong current hypothesis: the shared controller-count byte at `0x8013EDB1` never reaches/stays at `1` on the live boot path. Evidence: the first game-side PIF WRITE buffer arrived empty enough that the fallback path had to synthesize a controller slot, which is consistent with `func_80098C60` seeing controller count `<= 0`; `func_80098BD4` uses the same byte and would then skip parsing entirely.
- Supporting observation: no `[CONT] __osContGetInitData...` log ever appeared during runtime, despite `game_main` calling `osContInit_recomp` in the recompiled code. This suggests controller init is not completing normally, or the forced boot/state patches are bypassing part of the expected init path.
- Safe next recommended step: add one narrow diagnostic/guard in the diagnostics worktree to log and, if necessary, reassert `0x8013EDB1 = 1` immediately before the game's controller formatter/parser path runs; only after confirming that should broader rollback work begin on the forced main-loop hacks.

2026-03-18 19:27:27 -0300
- User explicitly approved continuing with file modifications in the diagnostics worktree.
- No stale `LodRecomp` process was left running after the previous interrupted turn.
- Next narrow step: patch the worktree-only diagnostics so SI DMA logs the live controller-count byte at `0x8013EDB1` and reasserts it to `1` if it has dropped to `0`; also make controller-init entry logging unmistakable.

2026-03-18 19:29:59 -0300
- New confirmed fact from live diagnostics: `0x8013EDB1` really does begin at `0` on the active runtime path. Reasserting it to `1` at SI DMA time keeps it at `1`, but parsed pad state still remains zero.
- `__osContGetInitData_recomp` still never logs during runtime, so normal controller init is still not visibly completing on this boot path.
- Stronger proof added in `func_800196DC`: the game-side controller-read function itself logs `parsed=0x0000 sx=0 sy=0` every frame after `func_80098BD4`, even when the SI stub has just injected logical PIF bytes representing `0x9000`. The break is therefore before or inside `func_80098BD4`'s interpretation of the emulated PIF buffer.
- Next high-value test: a tightly scoped fallback in the diagnostics worktree that copies button/stick data directly from the already-injected PIF buffer into the parsed pad struct when the parser leaves it zero; this will show whether the save screen/state machine responds once the game actually sees non-zero pad bits.

2026-03-18 19:35:40 -0300
- User noted the first interactive screen is the save/create-save-or-continue-without prompt; this is likely the next blocker after raw input reaches the game-state machine.
- Applied a diagnostics-only parsed-pad fallback in the worktree: after `func_80098BD4`, `func_800196DC` now decodes the injected PIF read slot directly into the parsed pad struct if the parser still leaves it zero. This is intended only to prove whether the save screen and core state progression respond once button bits are visible to game logic.

2026-03-18 19:47:18 -0300
- Live rerun proved the parsed-pad fallback works: `PAD_FB` fires, `func_800196DC` now sees `parsed=0x9000`, and the game's current/new pad state reflects the injected buttons. Input is therefore no longer the immediate blocker.
- Despite live button state, the high-level runtime still remains stuck at `scene_arg=0x00000004`, `guard=1`, and forced `exec_flags=0x380080C8`. This strengthens the hypothesis that the initial save/Controller Pak flow, not raw input delivery, is the next gating issue.
- New safe next step in the worktree: replace the empty Controller Pak helper stubs with defined success returns and light logging (`__osPfsCheckRamArea`, `__osPackRequestData`, and low-level PI raw I/O), because recompiled save/Pak code branches directly on these return values and an empty stub leaves the flow undefined.

2026-03-18 19:58:31 -0300
- Defined-success Pak stubs and logs did not change runtime behavior, and no `PAK` logs appeared during repeated live runs. The active boot path is therefore not even reaching normal save/Controller Pak init.
- Stronger root cause now: the forced `msg_type==1` bring-up patch in `funcs_11.c` is likely trapping the runtime in synthetic init behavior. It forces flags/overlay/NI state for rendering, but it does not naturally transition into the normal frame-state path that would lead to the save screen flow.
- Next focused experiment in the diagnostics worktree: after the forced bring-up steps run, explicitly promote execution from the synthetic init loop into the normal frame-state branch (`L_80017CFC`) and observe whether save/Pak helpers and state progression begin.

2026-03-18 20:07:42 -0300
- The `funcs_11.c` promotion experiment did fire (`[BOOT_DIAG] promoting synthetic init loop into frame-state path after 4 retraces`), so we proved the forced init loop can be escaped.
- After that promotion, detailed DL/state logs stopped entirely and the runtime only kept repeating a frozen VI origin (`game_origin=0x24B280`, displayed/VI origin fixed at `0x24B000/0x24B280`). No `PAK` logs appeared even after extended runtime.
- Interpretation: the forced bring-up patch is indeed the structural blocker, but a naive branch jump into `L_80017CFC` is not sufficient. The runtime is missing additional scheduler/event/state setup that the original path would have established before the normal frame-state machine can run.
- Recommended next step: instrument the message/event source feeding `func_80017C78` and compare the original `msg_type==2/3` side effects to the synthetic patch, then replace the current jump hack with a faithful reconstruction of the missing scheduler/event transition rather than forcing branch flow.

2026-03-18 20:15:31 -0300
- The original `msg_type==3` handler in `funcs_11.c` was confirmed to do more than VI/scheduler setup: it also writes `1` to `0x800B0088` before returning to the loop. The synthetic bring-up patch had intentionally avoided that write.
- The temporary branch-promotion experiment is now disabled again (`LOD_DIAG_PROMOTE_INIT_MSG_TO_FRAME_SM=0`) because it proved branch-forcing alone is not sufficient and only led to a frozen VI origin.
- Next diagnostics are now loop-level message traces in `funcs_11.c` so we can see the actual `msg_type` sequence and key state fields (`0x800B0088`, `s0+0x20`, `s0+0x2A`, `s0+0x34`, exec flags, overlay selector) on the live path.

2026-03-18 20:21:34 -0300
- Loop tracing showed the runtime already receives a stable `msg_type` pattern of `1,2,1,2,...`; `msg_type==3` never appears on the live path.
- The loop-local guard at `0x800B0088` stayed `0` throughout those traces even while rendering continued, which explains why the synthetic path never naturally handed off to the original guarded branch behavior.
- Next minimal experiment applied in the diagnostics worktree: keep the existing synthetic bring-up, but now explicitly reproduce the original `msg_type==3` side effect by writing `1` to `0x800B0088` once the synthetic init threshold is reached, while leaving message tracing enabled.

2026-03-18 20:27:12 -0300
- Setting the guard synthetically did change control flow: loop traces switched to `guard=1`, and the runtime stopped camping in the init-only branch. However, `exec_flags` dropped back to `0`, display lists collapsed to a single fill-only framebuffer path, and no save/Pak activity appeared.
- Extended runtime after the guard handoff showed only a frozen repeating VI origin; there was no delayed recovery.
- Next minimal experiment in the diagnostics worktree: preserve the successful guard handoff, but reassert the previously required execution-flag bundle (`0x380080C8`) after `guard=1` so the game-state handler gate does not immediately disappear during the transition.

2026-03-18 20:09:12 -0300
- Read-only trace refinement: `s0+0x34` (`0x801C82F4`) is not the primary logic gate. It only gates the VI scale path inside `msg_type==2`; the more important missing transition remains entry into the guarded frame-state machine.
- Scheduler/event tracing showed VI `0x29A` is forwarded by the scheduler event loop, but the main queue at `0x800C5D38` receives simplified `1/2` message values from downstream producers and still never sees `3`.
- New diagnostics change applied in the worktree only: added capped `[MQ_MAIN]` logging in `lib/N64ModernRuntime/ultramodern/src/mesgqueue.cpp` for enqueues/receives targeting `0x800C5D38` so we can identify which sender produces `1`, which produces `2`, and whether `3` is ever generated at all.

2026-03-18 20:09:12 -0300
- `[MQ_MAIN]` tracing showed the main loop receives message pointers, not raw literals: `0x800C5E80 -> type 1`, `0x800C5D70/72/74 -> type 2`. No `0x800C5E82 -> type 3` was ever queued during normal boot.
- `func_800184E0` initializes scheduler base `0x800C5E80` with halfwords `{1, 3}` at offsets `+0` and `+2`. The scheduler event loop forwards `s1+2` only from the OS event `14` branch, and the runtime defines `OS_EVENT_PRENMI = 14`.
- This means the existing synthetic `msg3` handoff in `funcs_11.c` is emulating a PRENMI path at boot. That matches the observed regression after forcing `guard=1`: object-dispatch logic stalls and rendering collapses into a repetitive fill-only path.
- Next experiment applied in the worktree only: disabled the synthetic `msg3`/`guard=1` handoff and the dependent post-guard exec-flag reassert so we can test whether the real gameplay path is the normal `guard==0 -> func_80017DB8` loop.

2026-03-18 20:09:12 -0300
- Rollback test result: with the synthetic handoff disabled, the main loop stays on `guard==0` and receives a stable alternating `type=1 / type=2` stream from `0x800C5E80` and `0x800C5D70/72/74`. No `type=3` appears, which is now expected because it is PRENMI-only.
- This avoids the previous `guard=1` regression completely: no exec-flag reassertion, no fill-only collapse, and no frozen repeating VI origin caused by the fake pre-NMI path.
- State still does not advance: `scene_arg` remains `0x4`, `exec_flags` remain `0`, `ni_sys` remains `0`, save/Pak helpers still do not appear, and the display lists repeat the same intro-state pattern while input is present.
- Recommended next step: instrument the normal `guard==0` object/game-state path, especially root object state and the game-state manager (`0x80000578`, `0x80002E3C`, `0x801C82E8`, root obj state/children), because the scheduler diagnosis is now good enough and the remaining failure is logic progression within the ordinary frame loop.

2026-03-18 20:40:00 -0300
- Added diagnostics-only tracing in worktree: root object handler (func_80000578) now logs root state, scene_arg, child slots, and DMAMgr pointer; game-state manager (func_80002E3C) now logs game_state, root work list head, first node/task, and DMAMgr pointer. No behavioral change intended.
- Purpose of this step: confirm whether the normal guard==0 loop is stuck because the root object never leaves state 0, because child creation/dispatch is malformed, or because the game-state manager never receives live work.

2026-03-18 20:43:00 -0300
- Root diagnostics changed the diagnosis: root object handler is alive and advances state 0 -> 1 -> 2, and DMAMgr registers at 0x800C1600 by the second type-1 tick. The ordinary boot loop is not stuck at the root-state byte.
- More important new symptom: game-state manager trace repeatedly showed an empty work list (`root_list=0`) even after root state reached 2. That points to missing task/list population or the wrong object being fed into the manager, not a missing scheduler handoff.
- Short run crashed with SIGSEGV after the new manager logger chased pointers too aggressively. This was a diagnostics bug in the logger, not evidence of a new game-side regression; next step is to make that trace address-safe before running again.

2026-03-18 20:46:00 -0300
- Hardened the new game-state manager diagnostics against invalid pointer chasing: it now only dereferences list pointers that resolve into the 8 MB RDRAM mirror. Added object address/state to the trace so we can identify which object owns func_80002E3C without crashing the run.

2026-03-18 20:53:00 -0300
- Added diagnostics-only tracing to func_80002D5C (game-state manager queue helper). It now logs enqueue attempts, command-node allocation results, and final manager `obj+0x34` after linking. Goal: distinguish between a dead producer path and a live producer path whose queue node allocation/linking fails.

2026-03-18 20:56:00 -0300
- Queue-helper tracing found the first concrete active-path failure. A producer does call func_80002D5C for manager object 0x8031B1E8, but the command-node allocator helper func_80001FD0 returns `0x00000001` instead of an RDRAM pointer. That leaves the manager queue invalid and matches the immediate crash on first real enqueue attempt.
- New primary hypothesis: the command/list allocator subsystem around 0x800C1530/1534/153C is misinitialized or one of its helpers returns a boolean/sentinel where callers expect a node pointer.

2026-03-18 21:00:00 -0300
- Corrected the new queue-helper interpretation: func_80001FD0 is only an in-pool classifier, not the node allocator. Added the next diagnostic checkpoint after func_80001968 so the run will show whether queue-node allocation succeeds before linking into manager `obj+0x34`.

2026-03-18 21:03:00 -0300
- Queue-helper tracing narrowed the logic failure further: the first live enqueue reaches func_80002D5C, but func_80001968 returns 0, so the command-node pool cannot supply an entry. The failure is now in queue/pool initialization or exhaustion, upstream of the game-state manager itself.

2026-03-18 21:08:00 -0300
- Added queue-pool diagnostics in funcs_0.c: func_80001940 now logs when the static command-node table is cleared, and func_80001968 now logs the first few slot payloads when allocation fails. Goal: distinguish “clear never ran” from “pool got polluted later.”

2026-03-18 21:12:00 -0300
- Added raw queue-slot payload logging at the GSM enqueue failure point so the next run can distinguish a saturated node table from allocator logic skipping free entries.

2026-03-18 21:16:00 -0300
- Corrected the GSM queue checkpoint: the previous `entry=0` read was taken before the delay-slot store of `v0` to `sp+0x18`, so it was not trustworthy. The next run now logs `ctx->r2` directly after func_80001968 returns.

2026-03-18 21:20:00 -0300
- LLDB showed the first fatal fault is in native runtime code: `vi_thread_func` in `lib/N64ModernRuntime/ultramodern/src/events.cpp`, dereferencing a bad `ViState*` during VI message delivery. That means the apparent game-state enqueue stall is entangled with a native VI-state initialization/swap bug, not just recompiled game logic.

2026-03-18 21:24:00 -0300
- Applied a diagnostics-only native runtime hardening patch in `lib/N64ModernRuntime/ultramodern/src/events.cpp`: initialize both `ViState` slots before launching the VI thread, clamp the active VI-state index to 0/1, and fall back to the dummy VI mode/framebuffer if the next state is missing mode data. Goal: keep the VI thread alive long enough to observe whether the first real game-state enqueue completes.

2026-03-18 21:27:00 -0300
- Fixed the VI-hardening build issue by replacing the bad forward declaration of `dummy_mode` with a `get_dummy_mode()` helper. No behavior change beyond the intended diagnostics/runtime hardening.

2026-03-18 21:31:00 -0300
- LLDB after the VI hardening patch showed the actual crashing thread/frame: `[Game] SCHED` inside `func_80002D5C`, not the VI thread. The bad address `0x40031B21C` is a flagged pointer, so the current root cause is in the game-state queue helper dereferencing a payload/list entry without masking off flag bits.

2026-03-18 21:35:00 -0300
- Diagnosed the latest SIGSEGV as a diagnostics-only bug: the `[GSM_Q] exit` logger in func_80002D5C dereferenced `owner+0x34` without stripping flag bits. Hardened that log to only read the field when the owner resolves into the 8 MB RDRAM mirror.

2026-03-18 21:49:00 -0300
- Rebuilt and reran the diagnostics worktree after hardening the `[GSM]` logger in func_80002E3C. The previous crash is gone.
- New strongest finding: the game-state manager queue is not dead. The first enqueue succeeds (`cmd_list_after=0x801B1D70`), later frames show `cmd_list=0x801B1D70` with `first_task` transitioning from `0x00000002` to `0x00000001`, and then the queue returns to `0x00000000` while `game_state` remains `0`.
- Root progression still reaches state `2`, input remains live (`parsed/game_cur=0x9000`), and the main loop keeps alternating msg types `1/2` with no crash. Current hypothesis: the queued transition is being consumed, but it either resolves to a no-op or the follow-up transition side never updates the global game state/save-flow state.

2026-03-18 22:02:00 -0300
- Added consume-side GSM diagnostics and reran. The first manager node is not empty: when it expires it executes four child commands: `0xC031B4A0`, `0xA0303940`, `0xA0303980`, `0xC031B42C`. These are cleanup/release-style entries (bit `0x40000000` or `0x20000000` set), after which the manager queue returns to `0` and `game_state` still stays `0`.
- Read back through root state handler `func_80000578`. It still contains major bring-up hacks: manual NI init, manual child creation sequencing, forced child-dispatch mask `0xFFFFFFFF`, and per-frame clearing of bit 30 on all root child slots. New primary hypothesis: these root-sequencing patches are causing the boot/save-flow state graph to collapse into a cleanup-only path instead of scheduling the real follow-up transition.

2026-03-18 22:11:00 -0300
- Ran an A/B check with only the root forced-dispatch override disabled (`LOD_DIAG_FORCE_ROOT_CHILD_DISPATCH=0`). This did not unblock progression: the same GSM cleanup packet still appears and `game_state` still remains `0`.
- The A/B did expose a sharper invariant: without the forced bit-clearing hack, root slot3 stays `0x4731B42C` in state 2 instead of becoming `0x0731B42C`. So bit 30 on that child really is not being cleared by the normal path. New next-step hypothesis: the actual blocker is the child-activation/flag-clearing path around root state 1/2 (`func_80010C80` / `func_80010DD4`), not the GSM queue itself.

2026-03-18 22:18:00 -0300
- Added targeted root diagnostics for slot3 and reran. This produced the clearest blocker so far: after root state 1 hands off to state 2, slot3 points at `0x8031B42C` but that object remains completely uninitialized for the whole run (`state=0`, `func+0x10=0x00000000`).
- This means the logic failure is earlier than the GSM cleanup packet. The child object that should drive the next save-flow transition is dead on arrival, so later GSM work can only clean up rather than advance state.
- Read `func_80010B84`: it is the child-creation path that calls `func_80002410` to instantiate slots whose type table entry is zero. Next recommended step is to instrument `func_80002410` / the relevant slot-spec path for the `0x8031B42C` child and determine why initialization returns a blank object instead of a live handler.

2026-03-18 21:16 -0300
- Read the root-slot lifecycle more carefully and corrected the earlier diagnosis: slot3 values like `0x4731B42C` are tagged slot entries, not raw object pointers. `func_80010DD4` decodes them with `entry & 0x803FFFFF`, which yields the real child object `0x8031B42C`.
- New working hypothesis: the child is probably not blank after construction; instead, the deferred bit (`0x40000000`) on the positive slot entry may still be preventing `func_80010DD4` from dispatching it, depending on the live caller mask. Added a narrow next-step diagnostic in the worktree to log the decoded slot3 entry and its dispatch gate conditions.

2026-03-18 21:16 -0300
- Rebuilt and reran with the corrected slot decode. This invalidated the earlier “blank child” diagnosis: slot3’s tagged entry `0x4731B42C` decodes to `0x8031B42C`, and that child does progress live on the current path. Over the first few frames its local state moves `0 -> 1 -> 2`, its handler changes from the generic `0x80002CE0` wrapper to overlay handler `0x802E4610`, and it creates child `0x8031B4A0` (spec `0x380`) before the GSM cleanup packet is enqueued.
- The narrow `func_80010DD4` trace did not fire for `0x8031B42C`, so this child is likely advancing via a different path than the dispatch gate I targeted. New strongest hypothesis: the real failure is inside the live overlay handler path rooted at `map_ovl_00_func_802E4610`, which still culminates in the cleanup-only GSM packet instead of a state-advancing transition.

2026-03-18 21:16 -0300
- Added a focused overlay trace in `funcs_69.c` and reran. This showed that `0x802E4610` is not dispatching a deeper overlay-specific substate on the failing path: its `ctx34+0x18` handler for `0x8031B42C` is just the generic `0x80002CE0` child traversal.
- On the first live `0x802E4610` call, that generic traversal sees the manually created render child `0x8031B4A0` (spec `0x380`) and immediately produces the GSM cleanup packet (`C031B4A0`, `A0303940`, `A0303980`, `C031B42C`). This makes the existing patch comment in `map_ovl_00_func_802E3F08` the strongest current root cause: the project is still faking a missing async NI/DMAMgr completion by creating the render child, but that shortcut is not reproducing the real state transition the game needs.

2026-03-18 21:16 -0300
- Ran an A/B with the manual `0x380` render-child creation in `map_ovl_00_func_802E3F08` flag-gated off. This did change behavior, but not enough: the GSM cleanup packet still occurs and state progression still fails.
- The cleanup packet shrinks from four commands to three when the manual render child is disabled: `A0303940`, `A0303980`, `C031B42C` remain, while `C031B4A0` disappears. So the forced render child is contributory noise, not the primary blocker. New strongest next step is to trace the live `0x8031B42C` path itself to find why it enqueues the remaining cleanup-only sequence instead of a state-advancing transition.

2026-03-18 21:36 -0300
- Used `lldb` on the diagnostics worktree and broke on `func_80002D5C` to capture the exact producers of the cleanup packet. All three commands are queued by the same live owner object `0x8031B1E8`: `0x80303940` and `0x80303980` are enqueued on the stack `map_ovl_00_func_802E4610 -> func_80002CE0 -> func_80002B10 -> func_8000516C -> func_80002D5C`, while `0x8031B42C` is enqueued on the slightly shorter sibling path `map_ovl_00_func_802E4610 -> func_80002CE0 -> func_80002B10 -> func_80002D5C`.
- Read-only source follow-up showed the earlier `0x802ECB78` clue was not a function pointer but a `jr` landing pad inside overlay 34. The producer object `0x8031B1E8` is executing `map_ovl_34_func_802ECA84`, which calls `map_ovl_34_func_802F014C` and then writes a short command table through `func_80001E30`. New strongest hypothesis: the failure is inside overlay-34 state selection, where `map_ovl_34_func_802F014C` is returning a value that causes `map_ovl_34_func_802ECA84` to emit only the cleanup-table sequence.
- Added a new diagnostics-only, compile-time-gated trace in `RecompiledFuncs/funcs_92.c` (`LOD_DIAG_OVL34_FLOW_TRACE`) to log `map_ovl_34_func_802ECA84` call/emit state: object pointer, `ctx34->48`, the two global gating values, `map_ovl_34_func_802F014C` return code, and the short table emitted at `obj+8`. Next safe step: rebuild and rerun once, then correlate the new `OVL34_CA84` logs with the known cleanup packet.

2026-03-18 21:46 -0300
- Follow-up diagnostics invalidated the overlay-34 hypothesis. The new `OVL34_CA84` trace never fired in the failing path, while `strings` confirmed the trace strings were present in the rebuilt executable. The failing cleanup packet still reproduces exactly, so the authoritative data remains the `lldb` stacks and direct memory reads.
- A fresh `lldb` session on `map_ovl_00_func_802E4610` showed that the live save-flow child `0x8031B42C` reaches overlay-00 handler `0x802E4610` with `obj+0x24 = 0x802ECB78`, `obj+0x34 = 0x803244C8`, and `obj+0x38/+0x3C/+0x44 = 0`. So the queued `0x80303940` / `0x80303980` values are not stored inline in `0x8031B42C` at handler entry.
- Breaking directly in `func_8000516C` resolved the indirection: that helper is called with object argument `0x802ECB78`, not `0x8031B42C`. Reading live memory at `0x802ECB78` showed the queue-source fields directly: `+0x44 = 0x80303940`, `+0x48 = 0x80303980`, `+0x4C = 0x80303940`, with small control fields at `+0x50/+0x54/+0x58`. This is the strongest concrete diagnosis so far: the failing path is intentionally driving a static cleanup/config descriptor rooted at `0x802ECB78`, and `0x8031B42C` currently points at that descriptor via `obj+0x24`.
- New strongest next step: trace who writes `0x802ECB78` into `0x8031B42C+0x24` and what condition should swap that pointer to the “real save/menu progression” descriptor instead. A write watchpoint on `0x8031B42C+0x24` or a constructor/init trace for spec `0x0004` is now the highest-signal path.

2026-03-18 22:04 -0300
- Read-only source tracing in `RecompiledFuncs/funcs_69.c` found the most likely writer path for the live child descriptor. In the block around `0x802E3D20..0x802E3DC8`, overlay-00 allocates a companion object (`s4 = v0`), stores that pointer into another object’s `+0x24`, seeds `s4->+0x3C` / `+0x40`, writes `s4->+0x24 = sys+0x7C`, and then installs handler `0x802E4610` on the companion/next object. This matches the observed shape `0x8031B42C -> 0x802ECB78`.
- Upstream, `sys+0x7C` is not hardcoded in overlay 00. In `RecompiledFuncs/funcs_47.c`, `func_80149DE4` writes `sys+0x2B0C` and `sys+0x7C` from a selected 7-word record. The caller block around `0x80149474..0x801494E8` either copies a default record directly or first calls `func_80149780` using globals at `0x8019D6A0/9C/98/94` (`8019-2960/-2964/-2968/-296C`) before passing the resulting record into `func_80149DE4`.
- Current strongest hypothesis: the project is still landing on the default/cleanup selector record that propagates to `sys+0x7C`, which overlay 00 later installs into the live save/menu child. The next highest-signal read-only step is to identify which caller path feeds `func_80149DE4` on this boot and whether the relevant `0x8019xxxx` selector globals are remaining in their default state when the first save-screen object is built.

2026-03-18 22:35 -0300
- Continuing with a narrow `lldb` trace on `func_801492B0` before any code change. Goal: dump selector globals, the chosen 7-word record, and the path into `func_80149DE4` to confirm why `sys+0x7C` resolves to the cleanup descriptor on the first save-screen flow.

2026-03-18 23:02 -0300
- Fresh `lldb` tracing invalidated the earlier “`0x8031B42C+0x24 = 0x802ECB78`” model. Live host-side RDRAM dump of `0x8031B42C` shows `+0x10 = 0x802E4610`, `+0x14 = 0x8031AC78`, `+0x34 = 0x803244C8`, and `+0x24 = 0` at the traced stop; source at `RecompiledFuncs/funcs_69.c:3915-3965` confirms `map_ovl_00_func_802E4610` reads `obj+0x34`, then dispatches via that context object’s `+0x18`.
- The live `OVL00_4610` trace on the failing path is now: `obj=0x8031B42C ctx34=0x803244C8 subobj=0x800F42E8 ctx18=0x80002CE0`, with queued fields coming from `obj+0x34/+0x38/+0x3C/+0x44`, not `obj+0x24`. New safest hypothesis: the cleanup-only flow is hanging off the `ctx34/subobj` chain and generic traversal (`0x80002CE0`), so the next highest-signal step is to inspect the live `0x800F42E8` branch and/or break `func_8000516C` on the actual translated `$a0` object rather than a host register guess.

2026-03-18 23:10 -0300
- Corrected the above with a more precise `lldb` pass. In translated functions, original MIPS `$a0` is stored at `ctx+0x20`; at the live failing `func_8000516C` stop, `ctx+0x20 = 0x802ECB78` and the stack is `map_ovl_00_func_802E4610 -> func_80002CE0 -> func_80002B10 -> func_8000516C`.
- Source at `RecompiledFuncs/funcs_1.c:2365-2440` shows `func_80002B10` iterates the original object’s slots at `+0x24/+0x28/+0x2C/+0x30` and calls `func_8000516C` on each nonzero slot. Live host-side memory at the same failing stop confirms the save object contents: `0x8031B42C+0x24 = 0x802ECB78`, `+0x28/+0x2C/+0x30 = 0`, and `+0x34 = 0x803244C8`.
- Live memory of `0x802ECB78` still shows the cleanup descriptor payload at `+0x44/+0x48/+0x4C = 0x80303940/0x80303980/0x80303940`. So the core blocker is now exact: the save-screen object `0x8031B42C` is being seeded with the cleanup descriptor directly in slot 0 (`+0x24`), and generic traversal is executing that slot as designed. The next highest-signal step is to catch or source-trace the writer that seeds `0x8031B42C+0x24` to `0x802ECB78`.

2026-03-18 23:18 -0300
- Static source tracing identified the concrete writer path in overlay 00: `RecompiledFuncs/funcs_69.c:2617-2628` (`map_ovl_00_func_802E3F08`) loads the global at `0x801C833C` (`0x801D0000 - 0x7CC4`) and stores it into the live object slot `0x24($v1)`. This explains the failing save object exactly: it is copying a preselected global descriptor into slot 0, not deriving the cleanup descriptor locally.
- Follow-up tracing in `RecompiledFuncs/funcs_47.c` confirmed the expected selector writers to `0x801C833C` exist at `80149C60`, `80149DAC`, and `80149EAC` (`func_80149BC4`, `func_80149D30`, `func_80149DE4` family), but fresh `lldb` runs with breakpoints on `func_80149D30` and `func_80149DE4` never hit on the observed failing boot path.
- Current strongest hypothesis: the selector-update path that should refresh `0x801C833C` is not running at all under the current forced bring-up, so overlay 00 keeps copying the stale/default cleanup descriptor (`0x802ECB78`) into the first save-screen object. The next safest step is to prove the initial/default value of `0x801C833C` from a controlled early stop or add a narrow, flag-gated diagnostic log around the `map_ovl_00_func_802E3F08` copy and the selector-writer family.
2026-03-18 22:31:28 -0300 - Read-only runtime + lldb correction: save object `0x8031B42C` reaches `map_ovl_00_func_802E4610` with `+0x1C=0`, `+0x24=0x802ECB78`, `+0x28/+0x2C/+0x30=0`, `+0x34=0x803244C8`. So the cleanup path is coming from slot 0, not a child list. Backtrace at the failing stop: `map_ovl_00_func_802E4610 <- map_ovl_00_func_802E3F08 <- map_ovl_00_func_802E3B70 <- func_80002950 <- func_800029EC <- func_80010DD4 ...`. This weakens the earlier “copy site is bypassed” hypothesis; next safe step is to inspect the live selector/global value feeding `+0x24` at `map_ovl_00_func_802E3F08`.
2026-03-18 22:33:40 -0300 - Narrow overlay-00 slot tracing result: the write that seeds `0x8031B42C+0x24` with the failing value is `funcs_69.c` site `0x802E3D38` (`[SLOT24_WRITE] site=802E3D38 dst=0x8031B42C value=0x802ECB78`). No later overlay-00 write changed that slot before `map_ovl_00_func_802E4610`. Updated diagnosis: the save object is being initialized with the cleanup/overlay-34 entry in slot 0 at construction time, not corrupted later. Next safe step: inspect `map_ovl_00_func_802E3D38` and its inputs to learn why save-flow construction chooses `0x802ECB78` instead of the expected save/menu progression entry.
2026-03-18 22:33:40 -0300 - One layer deeper: `func_801431CC` in `funcs_44.c` does not compute a new entry; it reads `a0->0x70`, copies transform-ish fields from that object into `a1`, and returns with `$v0` still equal to `a0->0x70`. Therefore `map_ovl_00_func_802E3D0C` seeds `save_obj+0x24` from the source object's preexisting `+0x70` field. New root-cause target: identify which source object is at `sp+0xA8` / `a0` in `map_ovl_00_func_802E3D0C`, and why its `+0x70` is already `0x802ECB78`.

### 2026-03-19 — Upstream N64ModernRuntime migration + save object tracing (main branch)

#### N64ModernRuntime updated to upstream (Jan 2026, commit 0bb76b0)
- 50+ upstream commits gained: action_queue scheduler, external message requeue (#125), controller detection (#134), display list event extensions (#128)
- Game-specific init moved to `lod_init.cpp` via `GameEntry::on_init_callback` pattern (NI decompression, KSEK0/1 mprotect, overlay loading, ROM mapping)
- Section 2 overlay linear≠effective address fixed (0x800C1520 → 0x80141870): upstream `load_overlays` computes linearly from ROM offset; LoD's section 2 has a relocation gap. Fix: `unload_overlays` at wrong address, then `load_overlays` at correct effective address.
- NI file format: original ROM uses 32-bit BE offsets + 4-byte header + zlib. NOT flag+24bit as previously assumed.
- Committed as 0f113b7.

#### Rendering pipeline confirmed working
- DL test harness: 10/10 synthetic tests pass
- Blue fill visible on screen (user confirmed)
- 507 faces from NI file[97] rendered (3D room geometry)
- Vertex positions collapse to screen center (camera at Z=-1000, 0.1x MV scale) — intentional for save screen background
- Exported geometry as OBJ (1519 verts, 169 faces) — user identified as indoor room/building, NOT the save screen

#### OS_EVENT_PRENMI investigation
- Added PRENMI support to events.cpp (struct + osSetEventMesg case 14 + fire once on first VI retrace)
- With PRENMI: game enters GFX mode (func_80017EC0 path), fbPairs=1, faceIdx=0 (fill-only DLs)
- Without PRENMI: game stays in init mode (func_80017DB8 path), fbPairs=6, faceIdx=507 (3D room geometry)
- **Critical finding**: init mode dispatches objects; GFX mode does NOT. The game needs init mode for the object system.
- PRENMI currently DISABLED — init mode needed for save object creation.
- TODO: find proper interleaving of init mode (object dispatch) + GFX mode (DL building)

#### Save object tracing (continuing diag branch work)
- Instrumented `map_ovl_00_func_802E3C4C` (funcs_69.c:2257), `func_801431CC` (funcs_44.c:4300), `map_ovl_00_func_802E3BE0` (state 0), `map_ovl_00_func_802E3B70` (entry), `func_80010B84` (child creation), `func_80000578` (root object manager)

**Object creation flow confirmed:**
1. `func_80000578` (root manager) dispatches every frame with scene_arg=4
2. `func_80010B84` creates child object template=0x90F6 at 0x8031B42C
3. Section 4 (map_ovl_00) entry `func_802E3B70` dispatches via state table at 0x802EB958
4. State 0 (`func_802E3BE0`): calls `func_80002808(obj+8, 0, 0x1C, 0)` → allocates 0x80324960 → success
5. State 1 (`func_802E3C4C`): reads `save_obj+0x70` via `func_801431CC` → gets NULL
6. SLOT24 trace: `dst=0x8031B42C val=0x802ED9D8 target_func=0x801431CC`
7. func_801431CC: `source=0x8031B42C +0x70=0x00000000` → returns NULL

**Root cause identified: save_obj+0x70 is NULL**
- `+0x70` is GSS_SLOT[15] in the object struct (offset 0x34 + 15*4 = 0x70)
- Root object (0x8031AC78) has slots 0-10 populated, slots 11-15 = 0x00000000
- Scene 4 descriptor only has 11 entries → slot[15] is never set
- The save object COPIES from root's slot area → its +0x70 = root's slot[15] = NULL
- Diag branch saw +0x70=0x802ECB78 (different forced state), main branch sees +0x70=0x00000000

**Difference from diag branch:**
- Diag branch had `0x802ECB78` in `+0x70` — this came from a different code path where the global at `0x801C833C` was copied into the slot. The selector-update functions (`func_80149BC4` family) were supposed to set `0x801C833C` but never ran.
- Main branch has `0x00000000` — the slot was never written at all because scene 4 only has 11 GSS entries.
- Both branches have the same root cause: the save object's state function reads `+0x70` expecting a scene state pointer, but the field is never properly initialized by the game's scene system.

**Next step:** Determine what the real N64 game puts at `save_obj+0x70`. Either:
1. The scene descriptor for the save screen should have 16 slots (not just 11)
2. `+0x70` is set by a different mechanism than GSS_SLOT copying
3. The save object reads `+0x70` from a different base (not self, but another object)
4. The selector-update path (`func_80149BC4` family) should run before state 1 reads `+0x70`

### CORRECTION: State machine DOES advance (byte-swap macros are correct)
- MEM_H/MEM_B macros have XOR byte-swap (^ 2 for halfword, ^ 3 for byte) — CORRECT
- State dispatch trace: state_index goes 0→1→2 across 3 calls, then object stops being dispatched
- Call #1: state 0 (allocate memory → success)
- Call #2: state 1 (reads `+0x70=NULL`, stores NULL into `+0x24`)
- Call #3: state 2 (`func_802E3F08`) — tries to use `+0x24` (which is NULL-derived) → fails/destroys object
- After call #3: object no longer dispatched (destroyed or deactivated)
- The `+0x70 = NULL` IS the root cause — state 1 propagates the NULL into `+0x24`, state 2 fails

### Next: Why is save_obj+0x70 NULL?
- `+0x70` is NOT in the GSS_SLOT area (which is +0x34 to +0x73)
- Object allocator zeroes entire 0x74-byte struct → `+0x70` starts at 0
- No code path writes to save_obj+0x70 between creation and state 1
- On the real N64, `+0x70` might be populated by the scene init or object template system
- Need to trace what the emulator has at this offset during the same game state

### CV64 decomp cross-reference: Object struct layout
From `/Users/dudu/Projects/recomp/references/cv64_decomp/include/game/object.h`:
```c
typedef struct Object {
    ObjectHeader header;          // 0x00-0x1F (32 bytes)
    u16 alloc_data_entries;       // 0x20 (bitmask of allocated entries)
    u16 graphic_container_entries;// 0x22
    Figure* figures[4];           // 0x24-0x33 — figures[0] is what SLOT24 writes
    void* alloc_data[16];        // 0x34-0x73 — generic data pointers
} Object; // Size = 0x74
```
- `+0x24` = `figures[0]` — display model/mesh pointer (written by SLOT24 trace)
- `+0x70` = `alloc_data[15]` — generic pointer, set via `object_allocEntryInList(obj, heap, size, 15)`
- State 0 allocates `alloc_data[0]` (+0x34) via `func_80002808` (= `object_allocEntryInList`)
- `alloc_data[15]` (+0x70) is NEVER allocated for the save object
- `func_801431CC` reads `alloc_data[15]` as if it's a valid pointer → gets NULL → state machine fails
- Need to find: who/what should populate `alloc_data[15]` before state 1 runs

### MAJOR FINDING: Scene descriptor ROM vs RDRAM mismatch
Scene 4 ROM data (at ROM 0xADD1C):
```
slot[0]: 0x90000864 (template 100)
slot[1]: 0x10000002 (template 2)
slot[2]: 0x0700200C (template 12) ← the "save" entry
slot[3]: 0x1000008A (template 138)
slot[4]: 0x10000088 (template 136)
slot[5-9]: various templates
slot[15]: 0x00000001 (template 1) ← NOT empty!
```
Runtime RDRAM (from ROOT_OBJ dump):
```
slot[0]: 0x9000C87C ← DIFFERENT from ROM (0x90000864)!
slot[1]: 0x90000864 ← SHIFTED by one position!
slot[2]: 0x1031B1E8 ← object ptr (already created)
slot[3]: 0x0731B42C ← save object (template 0x90F6 — WRONG, ROM has 0x0700200C)
slot[15]: 0x00000000 ← NULL (ROM has 0x00000001)
```

The scene descriptor slots are MISALIGNED — everything is shifted by 1 position and has wrong values. The root cause is either:
1. The scene descriptor table read is off-by-one or reads from wrong offset
2. The func_800007B0 (scene init) writes the entries incorrectly
3. The ROM data at runtime (RDRAM copy) is byte-swapped differently than expected

### CORRECTION: Scene table in RDRAM is CORRECT
- Dumped scene table at 0x800AD11C via MEM_W-equivalent native reads
- RDRAM matches ROM exactly: word[0]=0x800007B0, word[4]=0x0700200C, etc.
- `do_rom_read` uses `MEM_B` (with XOR `^3` swap) — byte-swap IS correct
- The mismatch is between the scene DESCRIPTOR and the root OBJECT SLOTS
- Root object slot[0]=0x9000C87C = scene desc word[1] (scene_func)
- This means func_800007B0 (scene init) TRANSFORMS the descriptor before populating slots
- The transformation includes the scene_func as slot[0], shifting everything
- This IS by design — the scene init function processes the descriptor

### Save object creation confirmed correct
- Scene descriptor word[4] = 0x0700200C (template 12, flags 0x07)
- Slot initially populated as 0x4700200C (bit 30 set by first pass)
- First pass (object_executeChildObject): skips template 12 because overlay table[12] != 0 (pre-loaded)
- Second pass (func_80010C80): detects bit 30, calls object_create with slot=0x0000200C, tmpl=12
- Object created: 0x8031B42C with template 12 ← CORRECT
- State machine dispatches correctly through states 0→1→2
- State 1 reads alloc_data[15] (+0x70) = NULL → propagates NULL → state 2 fails
- The template is RIGHT. The alloc_data[15] issue remains the blocker.
- alloc_data[15] must be populated by inter-object communication or scene system before state 1

### ROOT CAUSE FOUND: 0x90XXXXXX scene descriptor entries not relocated
Scene descriptor slots with `0x90XXXXXX` values (KSEK1 ROM pointers) are:
- slot[0] = 0x9000C87C → template 124 (after masking)
- slot[1] = 0x90000864 → template 100
- slot[6] = 0x9000C914 → template 276
- slot[7] = 0x900065DC → template 1500
- slot[8] = 0x90000A30 → template 560

These ROM pointers are NOT relocated to RAM addresses. On the real N64:
1. The game reads scene descriptor from ROM via DMA
2. `0x90XXXXXX` entries = pointers to sub-descriptors/overlay data in ROM
3. The scene loading system DMA's sub-data from ROM, resolves to RAM objects
4. Objects get proper dispatch functions from their overlay code

In our recomp:
1. Scene descriptor loaded correctly (ROM→RDRAM DMA with byte-swap ✓)
2. `0x90XXXXXX` entries stay as ROM addresses in the object slots
3. `object_executeChildObject` masks them: `0x9000C87C & 0x3FFFFFFF = 0x1000C87C`
4. `0x1000C87C` looks like a "pre-existing object" (flag 0x10) → SKIPPED
5. These slots never create actual objects → scene system is broken
6. Only templates with small IDs (2,3,4) and the one overlay (12) get created
7. The "scene function" (0x9000C87C) that should set up alloc_data[15] NEVER RUNS

**Fix needed:** Implement `0x90XXXXXX` → RAM address relocation during scene loading:
- `0x9000YYYY` → ROM offset `0xYYYY` → DMA to RAM → `0x8000XXXX`
- OR: translate in `object_executeChildObject`: if slot has `0x90` prefix,
  resolve to ROM DMA source, load overlay data, create proper object

This is a core engine feature missing from the recomp — the scene descriptor
ROM pointer resolution system. Without it, 5 of 11 scene slots are dead.

### Current status
- Scene table in RDRAM: ✓ correct
- DMA byte-swap: ✓ correct (MEM_B with XOR 3)
- MEM_H/MEM_B macros: ✓ correct (have XOR byte-swap)
- State machine dispatch: ✓ advances through states 0→1→2
- Root object slot population: done by func_800007B0, values differ from raw descriptor (by design)
- Save object creation: uses template from processed slot value
- alloc_data[15] (+0x70): NULL — never allocated for this object type
- State 1 reads +0x70 = NULL → propagates NULL → state 2 fails → object destroyed
- Next: trace func_800007B0 to understand how it transforms descriptor → slots,
  and whether the template ID for the save object is correct

This explains everything:
- Save object gets template 0x90F6 (slot[3] runtime) instead of 0x00C (slot[2] ROM)
- The wrong template creates a different object type whose state machine fails
- alloc_data[15] is never populated because the wrong object type doesn't allocate it

Key addresses for investigation:
| Address | Description |
|---------|-------------|
| 0x8031AC78 | Root object (scene_arg=4) |
| 0x8031B42C | Save object (template 0x90F6) |
| 0x802E3B70 | map_ovl_00 entry (state dispatch) |
| 0x802EB958 | State dispatch table (in overlay data) |
| 0x80324960 | Allocated memory from state 0 |
| 0x802ED9D8 | Pre-call value stored in save_obj+0x24 |
| 0x801C833C | Global selector for save descriptor (diag branch finding) |
| 0x800AEA8C | Object type lookup table |
2026-03-19 09:52:33 -0300 - Added slot70 traces in worktree and ran GUI capture with a dedicated /tmp trace file.
2026-03-19 09:52:33 -0300 - Result: at `802E3D20.pre`, save object `0x8031B42C` has `slot24=0` and `slot70=0`; no slot70 write to that object fired before the bad save-flow path.
2026-03-19 09:52:33 -0300 - Corrected prior diagnosis: `0x802E3D38` is a MIPS delay-slot store, so `save_obj+0x24` is seeded from `func_80005A30(0x201, 0x8019E5F0)` return, not from `func_801431CC(a0->0x70)`.
2026-03-19 09:52:33 -0300 - Current best hypothesis: the wrong static descriptor/context is being selected upstream by `func_80005A30` / `func_80005574` for the save object; `slot70` is not the root cause on this path.
2026-03-19 10:35:38 -0300 - Compared current diagnostics branch against uncommitted `main` changes from another agent.
2026-03-19 10:35:38 -0300 - Useful carry-over: `main` also instrumented the save constructor in `funcs_69.c`, but it was still tracing the old `slot70` / `func_801431CC` theory.
2026-03-19 10:35:38 -0300 - Stronger current read: `main` changes do not contradict the corrected model; the useful clue is that overlay 00 reaches the fallback constructor path, not the normal save-progression path.
2026-03-19 10:35:38 -0300 - New concrete root-cause target: bit 2 in the bitset rooted at `0x801CAA60` is clear, so `map_ovl_00_func_802E3C4C` takes fallback `0x201 -> func_80005A30` and seeds `save_obj+0x24` from the wrong descriptor/context.
2026-03-19 10:48:00 -0300 - Added narrow bit-2 tracing in the diagnostics worktree around `func_800048A0`, `func_800048C4`, and `func_800048EC`, then rebuilt and ran a fresh GUI capture.
2026-03-19 10:48:00 -0300 - Result from `/tmp/lod-bit2-diag.log`: exactly one event fired on the broken boot path, `site=800048A0 root=0x801CAA60 bit=2 before=0 after=0`; no matching `800048C4` set or `800048EC` clear occurred.
2026-03-19 10:48:00 -0300 - Current strongest diagnosis: the producer that should set bit 2 in `0x801CAA60` never runs on this boot path, so overlay 00 always enters the fallback save constructor.
2026-03-19 10:48:00 -0300 - Recommended next step: trace the caller that performs the lone `func_800048A0(0x801CAA60, 2)` read and walk one level earlier to identify the missing producer path, rather than tracing downstream save-object cleanup again.
2026-03-19 11:16:14 -0300 - Read-only follow-up in `funcs_69.c`: overlay-00 local set/clear sites for `0x801CAA60` bit 2 are downstream of save-object creation, but `map_ovl_00_func_802E4238` is a concrete local producer candidate that would set bit 2 at `0x802E42E8` when its computed value goes negative.
2026-03-19 11:16:14 -0300 - Short `lldb` breakpoint run on `map_ovl_00_func_802E4238` did not hit during a fresh reproduction window, while the stuck save-screen path still reproduced.
2026-03-19 11:16:14 -0300 - Current strongest diagnosis: the state path that should reach `map_ovl_00_func_802E4238` is missing or bypassed before overlay 00 constructs the save object, so bit 2 never becomes set and the fallback `0x201 -> func_80005A30` constructor remains deterministic.
2026-03-19 11:16:14 -0300 - Safe next step: trace who should dispatch `map_ovl_00_func_802E4238` (object state table / handler chain), rather than adding more downstream save-object diagnostics.
2026-03-19 17:05 - Diagnostics worktree `lod-state-diag`: removed forced-ready return in `func_8014314C`; save object now stays in overlay-00 state 2 and nested subdispatch runs repeatedly instead of handing off immediately to `0x802E4610`.
2026-03-19 17:05 - Current live blocker: save nested substate 0 (`0x802E406C`) repeats forever because wrapped object `0x800F42E8` has `+0x4C == 0`; timer is already at 3000, so progression is gated by missing nonzero ready/completion state, not time.
2026-03-19 17:05 - Next step: run one fresh capture with expanded `SAVE_SUB0` fields (`fn10/fn3c/p40/p6c/p70`) to identify whether `0x800F42E8` is a wrapper and which producer path is supposed to populate `+0x4C`.
2026-03-19 17:42 - Fresh capture confirms `0x800F42E8` is a deliberate pooled object from `func_80040524`, not random corruption; `SAVE_SUB0` still shows `+0x4C == 0` forever while overlay-00 state 2 loops.
2026-03-19 17:42 - Constructor path: `map_ovl_00_func_802E3C4C` allocates pooled container `s5` via `func_80040524`, attaches children via `func_80040628` and `func_80040844`, and later save state 2 reads `save_obj->34->4 == s5`.
2026-03-19 17:42 - Strongest hypothesis: container completion path in `func_80041C54` never runs for the live save container, or runs with an empty child/list state so it never writes container `+0x4C/+0x50`. Next step: narrow trace for `SAVE_SUB0` container list fields and `func_80041C54` entry on `0x800F42E8`.
2026-03-19 18:08 - Container trace result: `save_ctx->4 = 0x800F42E8`, `container->58 = 0x802ECB78`, `container->60 = 0`, `container->4C = 0`; forced `func_800409FC` tick in save substate 0 produced no state change and no `func_80041C54` hit for the save container.
2026-03-19 18:08 - Revised conclusion: the save path is still taking the earlier fallback constructor seeded from cleanup descriptor `0x802ECB78`; later pooled-object updates are downstream symptoms, not the root cause. Next experiment should target the earlier bit-2 gate in `0x801CAA60` to avoid the fallback path entirely.
2026-03-19 12:13:23 -0300 - Diagnostics worktree: disabled the late `LOD_DIAG_FORCE_POOL_TICK` experiment and enabled a narrower `LOD_DIAG_FORCE_SAVE_BIT2` gate in `map_ovl_00_func_802E3C4C`, forcing bit 2 in `0x801CAA60` immediately before the save constructor precheck.
2026-03-19 12:13:23 -0300 - Purpose of this run: isolate whether the broken save flow is caused by the missing earlier bit-2 producer, without stacking a second downstream workaround on top of it.
2026-03-19 12:13:23 -0300 - Run result: no behavioral change. The constructor still logged `BIT2 before=0 after=0`, still wrote `save_obj+0x24 = 0x802ECB78`, and save substate 0 stayed pinned with `container->58 = 0x802ECB78` and `container->4C = 0`.
2026-03-19 12:13:23 -0300 - Follow-up root cause on the experiment itself: `MEM_W` expects full KSEG-style addresses (`rdram + (reg - 0xFFFFFFFF80000000)`), but the direct force masked `0x801CAA60` down to `0x001CAA60`, so that specific write was invalid. The earlier bit-2 hypothesis remains viable; next fix should use the real address form or call `func_800048C4` directly.
2026-03-19 12:13:23 -0300 - Corrected the bit-2 force to use the real KSEG address form (`MEM_W(0, ctx->r4)`), rebuilt, and reran the same bounded capture.
2026-03-19 12:13:23 -0300 - High-signal result: `map_ovl_00_func_802E3C4C` now sees bit 2 set (`BIT2 before=0x20000000 after=0x20000000`), the bad `save_obj+0x24 = 0x802ECB78` write disappeared, and the old state-2/substate-0 stall no longer appears.
2026-03-19 12:13:23 -0300 - New blocker with bit 2 forced: the save object remains in overlay-00 state 1, queues a single GSM command `0xC031B42C`, that command is consumed, and then the object stays in state 1 instead of advancing. Next step should be read-only tracing of the state-1 path after GSM consumption.
2026-03-19 12:21:54 -0300 - Read-only follow-up: `func_801476FC(1)` in state 1 only ORs bit 1 into global request word `0x8019D19C`; it does not advance state by itself. The nominal branch still looks like a request/consumer handshake, not a self-contained transition.
2026-03-19 12:21:54 -0300 - Added narrow diagnostics in `funcs_46.c` around `func_801476FC`, `func_80147718`, and `func_80147728` for `a0 == 1` to determine whether the state-1 request bit is ever tested or cleared on the live boot path.
2026-03-19 12:21:54 -0300 - Fresh run result with those traces: the save state-1 path sets request bit 1 exactly once (`REQ1_SET before=0x00000000 after=0x00000001`) and then no `REQ1_TEST` or `REQ1_CLEAR` for bit 1 occurs anywhere on the live boot path.
2026-03-19 12:21:54 -0300 - Stronger current diagnosis: with the earlier bit-2 gate corrected, the remaining blocker is a missing consumer for request bit 1 at `0x8019D19C`. The nominal branch is waiting on an external request-processing path that never runs in the current runtime.
2026-03-19 12:21:54 -0300 - New experiment in diagnostics worktree: with `LOD_DIAG_FORCE_SAVE_BIT2` still enabled, disabled the state-1 nominal-branch self-dispatch (`obj+0x10`) in `map_ovl_00_func_802E3C4C` behind `LOD_DIAG_SKIP_STATE1_SELF_DISPATCH`.
2026-03-19 12:21:54 -0300 - Purpose of this run: the current self-dispatch only queues `0xC031B42C` (self-release). This may be short-circuiting the external request-bit consumer path we actually need, so the next check is whether leaving only `REQ1_SET` active allows another system to consume it.
[2026-03-19 16:25:00 -03] Read-only pass: overlay 07 contains the request-bit consumer path. Current hypothesis is that request bit 1 stalls because the relevant overlay-07 object is never created or never reaches its consumer state machine. Next step: lldb break on overlay-07 constructor/consumer entry points before any new code edits.
[2026-03-19 16:38:00 -03] Edited diagnostics worktree `funcs_11.c`: enabled a narrow synthetic msg3 handoff experiment that now only reproduces the original msg_type==3 branch (`func_80091108(10)`, VI scale/black, guard=1). Removed the older forced exec-flags / overlay / NI side effects from that block.
[2026-03-19 16:42:00 -03] Run result for narrow synthetic msg3 handoff: it flips the loop guard to 1, but the runtime still stalls. Logs show no request-bit consumer or overlay-07 activity, `exec_flags` remain 0, root state stays 0, and the message loop degrades into repeated type=1 events once `s0+20` reaches 19. Next step: inspect the specific execution-flag gate instead of restoring the earlier full mask.
[2026-03-19 16:50:00 -03] User asked to continue investigation after checking main. Current focus is read-only: trace the execution-flag gate at sys+0x2908 (`0x801CABC8`) and `func_80177860` before any further edits/builds.
[2026-03-19 12:47:15 -03] Read-only tracing in lod-state-diag: identified funcs_45.c:func_80146240 as the concrete producer of sys+0x2908 |= 0x10000000 and sys+0x2B10 = 1; lldb symbol breakpoint resolved and was never hit during the stuck boot sample. Guard still flips to 1 via the narrow synthetic msg3 handoff, but exec_flags remain 0x00000000 and the runtime never enters the active state path. 0x8019D180 (+0x18) is known to be rebuilt on other paths after the block reset in funcs_45.c, so the missing handoff is now narrowed to the post-reset activation flow that should eventually reach func_80146240.
[2026-03-19 12:47:45 -03] Added a flag-gated one-shot activation shim in lod-state-diag/RecompiledFuncs/funcs_11.c (LOD_DIAG_ACTIVATE_BG_STATE_ONCE) to emulate only the concrete missing func_80146240 side effects after the synthetic msg3 handoff: sys+0x2908 |= 0x10000000 and sys+0x2B10 = 1, with a single BOOT_DIAG log. Next step: rebuild and short-run to see whether background state flow advances or fails differently.
[2026-03-19 12:48:20 -03] Activation experiment result in lod-state-diag: the one-shot shim fired and changed sys flags from 0x00000000 to 0x10000000 with sys+0x2B10=1, but state flow still stalled. Root stayed at scene_arg=4 / root_state=0 and the message loop still collapsed into repeated retrace traffic. Conclusion: the missing active-state bit was necessary enough to test but is not sufficient by itself; there is still an upstream or parallel handoff missing after guard.
[2026-03-19 12:56:40 -03] User reported a prior 100GB RAM runaway. Short bounded run-only memory watch in lod-state-diag (20s, killed on timeout, 4GB RSS safety threshold) did not reproduce it: LodRecomp rose from ~706MB RSS to ~729MB RSS while VSZ stayed ~439.9GB and CPU ~103-108%. No live LodRecomp process remained afterward.
[2026-03-19 12:56:40 -03] Run log during the memory watch still shows the same stalled boot/runtime pattern: repeated MQ send-jam/recv on 0x800C5D38/0x800C5E80, guard=1, s0+20=19, flags=0x10000000, overlay=1, and fixed VI origin values. Memory is currently bounded, but logic/state flow is still stuck in the same loop.
[2026-03-19 12:58:10 -03] User asked to keep watching for RAM runaway. Ongoing workflow note: future LodRecomp runs in this session should be bounded and memory-watched, with early kill if RSS starts climbing abnormally. Treat high memory growth as a standing regression signal during all further diagnostics.
[2026-03-19 13:02:25 -03] Added a narrow flag-gated trace in lod-state-diag/RecompiledFuncs/funcs_60.c for func_80177860 to log the first few hits, the active/inactive split, and whether the active path routes to func_80177948 or func_80177B4C.
[2026-03-19 13:02:25 -03] First run with that trace crashed immediately due to a diagnostics bug: `MEM_W` arguments were reversed inside the new trace, causing a host SIGSEGV before the traced function could be reached. Fixed the argument order in the worktree and rebuilt.
[2026-03-19 13:02:25 -03] After fixing the trace bug, bounded reruns still exit early around root state 2 with `[CRASH] Signal 11 at address 0x4001cd6d8 (code=2)` before any `BG77860` log appears. RSS stayed bounded (~681MB at 1s), so this is a new crash regression to root-cause before resuming the background-state handoff investigation.
[2026-03-19 13:18:10 -03] Reverted the funcs_60 trace entirely after confirming it was perturbing code layout/timing. Rebuild + bounded rerun returned to the prior stable stall: no early crash, RSS stayed ~701MB→712MB over 12s, and the runtime again collapsed into the same repeated `MQ_MAIN` retrace loop with guard=1 and flags=0x10000000.
[2026-03-19 13:18:10 -03] Sampled the stable stalled process. Main UI thread was idle/sleeping as expected; the hot game thread was `mainThread_entrypoint` (func_800905D8) mostly blocked on `osRecvMesg`, with occasional frame/audio builder calls. This confirms the runtime is alive but parked in its message-driven loop rather than progressing game state.
[2026-03-19 13:18:10 -03] Read full `func_80146240`: earlier one-shot activation shim was incomplete. The real function also requires `0x8019D194` and `0x8019D198` to be nonzero and then calls helper `func_80001CE8(obj+8, obj+E)` after setting sys+0x2908 bit 28 and sys+0x2B10=1.
[2026-03-19 13:18:10 -03] Added narrow gate logging to the existing stable activation shim in funcs_11.c. Bounded run result: at activation time both prerequisites were dead: `gate14=0x00000000` and `gate18=0x00000000`. This means calling `func_80146240` directly would still be wrong; the producer path for the 0x8019D180 control block is missing upstream.
[2026-03-19 13:18:10 -03] Identified a concrete `gate14` producer in funcs_47.c (`func_8014B370`): it increments `0x8019D194` and calls the same `func_80001CE8` helper on its object. Added a first-few-hits trace there and reran bounded sample; no `GATE14_PROD` logs fired at all. Stronger diagnosis: the overlay/state path that should enter `func_8014B370` never runs on the current live boot, which is why the activation control block stays zeroed and the later background-state handoff never becomes valid.
[2026-03-19 13:27:50 -03] Read the surrounding overlay-47 cluster: `func_8014B300` is a local dispatcher that selects substates from a table at 0x8019E600, and `func_8014B370` is one of those entries. Added a narrow generic object-exec trace in funcs_1.c that only logs when the dispatched function pointer is in the overlay-47 `0x8014B300..0x8014C000` range.
[2026-03-19 13:27:50 -03] Bounded rerun result with the generic overlay-47 exec trace: no `OVL47_EXEC` logs fired at all, while activation still happened with `gate14=0` and `gate18=0`. Stronger current diagnosis: the live stalled boot path never dispatches any handler in the overlay-47 producer family, so the missing gate14/gate18 activation prerequisites are upstream of the entire overlay-47 object/state machine, not just one missing substate inside it.
[2026-03-19 13:37:40 -03] Big-picture model for the user: current blocker no longer looks like “many missing syscalls.” Controller/SI path did need fixes, but the main stall is higher-level. The runtime reaches a partially alive state (input works, renderer runs, threads/messages stay alive), but an upstream game-side object/state family (overlay-47 producer path) never dispatches. Because of that, the shared activation control block at 0x8019D180 never gets seeded (`+0x14/+0x18` stay zero), the real activation function `func_80146240` never becomes valid, and the background game-state handler never fully enters its intended flow. Expected fix shape: restore the missing original handoff/dispatch path, likely in recompiled game-state/boot logic, with only minimal runtime fixes if a specific host behavior is still preventing that path.

[2026-03-19 ~17:00 -03] Fresh approach on main branch: removed ALL 6 forced patches from prior debugging (funcs_0, funcs_1, funcs_44, funcs_60, funcs_62, funcs_69). Let the game run with clean recompiled code. Key findings:

### Patch removal results
- Memory leak gone: RSS stable at ~1.4MB (was 100GB with forced patches)
- DMA manager completes file loads correctly (PI DMA verified, 64-bit result check passes)
- Overlay state machine alive: dispatches state 0 sub-states 0→1→2 correctly
- `func_8014314C` no longer force-returns 1 (was killing the overlay object immediately)

### BSS clearing discovery (root cause of file_ptr_array=0)
- Our `lod_init.cpp` populates `file_ptr_array` at 0x801C8830 with pre-decompressed NI addresses
- Game's bootstrap code zeros BSS (gap between section 1 end 0x800AADF0 and section 2 start 0x80141870, plus gap up to section 3)
- `file_ptr_array` is in that BSS region → gets wiped after our init
- Fix: `lod_repopulate_file_ptr_array()` helper in lod_init.cpp, called at END of `func_800113EC` (template 3 / NI system dispatcher) every frame. Must be at END because Phase 3 of that function intentionally clears file_ptr_array entries not in the current bitmask.

### ni_desc+0x4C is a sub-figure bitmask, NOT a pointer
- The NI descriptor at 0x800F42E8 has +0x4C which is checked by the overlay state handler
- `func_800448FC` uses it as a bitmask: each bit = one active sub-figure, iterates linked descriptors at +0x64 via +0x34 chain, right-shifts bitmask each iteration
- On real N64, this bitmask is populated by the NI file parsing pipeline after decompression
- Currently forced to 0x1 as experiment — overlay advances to timer handler (0x802E40D8)
- Timer handler stuck at 30: parent handler resets `alloc0+0xC` to 30 every frame because `ni_desc+0x8` (value 3000) never decrements
- `ni_desc+0x8` should be decremented by the figure processing code (`func_800448FC` chain) as animation frames progress, but with bitmask=1 it may not process correctly

### Current state
- Overlay dispatch running: state=0, handler=0x802E3F08 (repeating), nested handler=0x802E40D8
- exec_flags still 0x00000000
- Next: understand what correct ni_desc+0x4C bitmask value should be, and what makes ni_desc+0x8 decrement

### 2026-03-19 14:58 - process / workaround framing
- User asked for the general process, not only this repo's specific bug.
- General N64 recomp bring-up pattern: first establish whether failure class is runtime contract, boot/scheduler/event ordering, asset/init state, input/save I/O, or game-side state-machine/object dispatch.
- Normal long-lived workaround categories in other recomp projects: runtime shims, targeted recompiled-function patches, boot/init ordering fixes, asset/state repopulation after BSS clears, and small fidelity restores for platform services.
- Current repo still looks primarily like a missing original game-side state/dispatch handoff, not a broad "many syscalls missing" problem.

### 2026-03-20 — Boot sequence clarification (user-confirmed)
- LoD boots directly into the **save game screen** (scene 4) as its FIRST scene. This differs from CV64 (which starts with Konami logo).
- Full boot order on real N64:
  1. Save game screen (create save / continue without saving)
  2. Konami logo, then KCEK logo
  3. Expansion Pak screen (high/low resolution selection — requires input)
  4. Intro movie (real-time rendered cutscene)
  5. Title screen (game start, options)
- Scene 4 being the initial scene is **correct and natural** — the game is NOT being forced into scene 4.
- The `lod_init.cpp` comment "since we boot directly into scene 4" was accurate; it's the game's intended first scene.

### 2026-03-20 — Symbol system + function renames
- Created `include/lod_symbols.h`: named constants from CV64 decomp (Object struct, system_work, OSSched, gamestate IDs, template IDs, NI system).
- Created `tools/apply_symbols.py`: post-regen script that replaces hex literals with named constants in function-scoped context. Wired into `regen_recomp.sh` as step 7.
- Renamed 9 functions in `castlevania2.syms.toml` and `symbol_addrs.txt`:
  - `func_80000460` → `gamestate_create` (CV64 exact match)
  - `func_8000053C` → `gamestate_change` (CV64 equivalent)
  - `func_8000067C` → `scene4_init` (scene 4's init function from descriptor)
  - `func_8001783C` → `idleThread_entrypoint` (creates PI mgr + graph thread)
  - `func_80018890` → `scheduler_addClient` (registers scheduler event client)
  - `func_800404D0` → `atari_work_table_init` (CV64 match: pool table zeroing)
  - `func_80040948` → `atari_work_entry_tick` (per-entry pool tick)
  - `func_80040A6C` → `atari_work_post_update` (post-tick cleanup)
  - `func_8001BA78` → `overlay_system_create` (creates TMPL 0x1AB)
  - `func_8001B9A0` → `ni_system_handler` (NI system object handler)
- Full regen + build + run verified: same behavior as before.
- Fixed N64Recomp truncation: funcs.h line 5179 and recomp_overlays.inl line 5353 had mid-line truncation not caught by fix script.

### 2026-03-20 — Patch audit: all 3 patches mask the same root cause
- **Patch 1** (GSM bootstrap): creates overlay_system prematurely — on cold boot, `sys+0x2B24` is 0 and overlay_system should NOT exist yet. It's created by `ni_system_handler` after NI files load.
- **Patch 2** (+0x4C bypass): skips NI pipeline completion gate. `ni_desc+0x4C` is set by DMA→decompress→figure creation chain.
- **Patch 3** (poolObject_tick hook): calls pool tick from wrong context. Normally called via object dispatch system.
- **Root cause**: NI file loading pipeline isn't completing. DMAMgr (template 3/5) needs to be properly dispatched.
- Next step: trace exactly where the NI/DMA pipeline stalls.

### 2026-03-20 — NI pipeline tracing: DMA works, decompression path runs
- `ni_system_dispatcher` called every frame ✓ (object at 0x8031AEBC)
- DMAMgr (object at 0x8031AF30) called every frame, state 0→1 ✓
- Phase 2 iterates 687 entries: 382 non-zero file_info, 305 zero
- Only 1 file requested: **file 0x14D** (333) with entry `0x4000014D` (bit30=requested, bit31=not loaded)
- DMA DOES complete: pending goes 1→0, cur_idx advances 0→1
- DMA result: `v1=0x8038DE10` (negative = **compressed data**) → takes compressed path
- `object_curLevel_goToFunc(obj+8, obj+E, 4)` correctly sets state from 1 to 4
- State 4 handler (`0x800129F8`) runs inline (ENTER pattern dispatches it immediately within same frame)
- After full frame: state is back to 1 — state 4 handler likely resets to 1 after processing
- **KEY**: the decompression IS happening within one frame. The question is whether the decompressed result is stored to `file_ptr_array` and whether bit 31 gets set on the NI entry.
- Next: trace state 4/5/6/7 handlers and the completion callback that should set bit 31.

### 2026-03-20 — ROOT CAUSE FOUND: TLB-mapped NI overlay code at 0x0F000000
- DMA + decompression WORKS: file 0x14D decompressed to 0x8038F000, stored in file_ptr_array ✓
- State 6 (completion) NEVER fires because the game crashes before reaching it
- **Crash**: `osMapTLB(idx=0, vaddr=0x0F000000, even=0x0038F000, odd=0x00390000)` maps the decompressed NI data as executable TLB region, then tries `LOOKUP_FUNC(0x0F000000)` → "Failed to find function" → abort
- The decompressed NI file 0x14D contains **overlay CODE** that the game executes via TLB mapping
- On real N64: TLB maps 0x0F000000 → physical 0x0038F000, CPU runs code from there
- In recomp: `osMapTLB` doesn't register functions, `LOOKUP_FUNC` has no entry → crash
- **Fix needed**: register recompiled functions for the NI overlay at the TLB-mapped address (0x0F000000+), similar to how map overlays at 0x802E3B70 are loaded via `load_overlays`
- This explains why ALL three patches were needed: the crash happens silently (caught by error handler), preventing state 6 from completing, leaving bit 31 unset, so the NI pipeline never finishes

### 2026-03-20 — NI overlays: the missing recompilation layer
- 464 NI compressed files contain MIPS executable code (NI files 0x1C5–0x3AE, ~232 unique overlays in pairs)
- These are ALL game object overlays: enemies, effects, menus, save screen, cutscenes, stage objects
- They load at TLB-mapped VRAM `0x0F000000` (shared/exclusive like map overlays at `0x802E3B70`)
- 131 templates dispatch to `0x0F000000` entry, 100+ more dispatch to specific offsets within
- **None of this code is recompiled.** It only exists as zlib-compressed MIPS in the NI file table.
- Full overlay architecture:
  1. Main code (0x80000460) — ✅ recompiled
  2. Common code (0x80141870) — ✅ recompiled
  3. Overlay system (0x801CAEA0) — ✅ recompiled
  4. Map overlays ×47 (0x802E3B70) — ✅ recompiled
  5. **NI overlays ~232 (0x0F000000) — ❌ NOT recompiled**
- Next steps: extract NI overlay code → splat/disassemble → feed to N64Recomp → register at 0x0F000000

### 2026-03-20 — MAJOR CORRECTION: two object tables, NI pipeline IS running
- There are TWO object tables, not one:
  - `0x800AD640` = **dispatch table** (per-frame handler). Templates 0-10 are POPULATED here. `ni_system_dispatcher` (tmpl 3) and DMAMgr (tmpl 4) ARE dispatched every frame.
  - `0x800AEA8C` = **file info table** (NI file lookup / creation guard). Templates 0-10 are NULL here. This is NOT the dispatch table.
- `object_dispatch1` uses `0x800AD640` for dispatch. `object_createAndSetChild` uses `0x800AEA8C` for NI file info.
- **Templates 3 and 4 ARE alive and dispatched every frame.** Previous analysis was WRONG about them being dead.
- The real stall is within the NI pipeline itself, not object dispatch. Need to trace what `ni_system_dispatcher` and DMAMgr actually do on each frame and why the pipeline doesn't complete.

### 2026-03-20 — NI overlay extraction, recompilation, and runtime loading (DONE)

Implemented full 9-step plan to extract, recompile, and load all NI overlays.

**Extraction pipeline** (`tools/ni_ovl/step1-7`):
- NI table: 944 entries, 245 code pairs. Flag byte 0x00=text, 0x80=data. All use **zlib** (not LZKN64 — `78 DA` magic at byte 4 of every entry, streaming `decompressobj()` needed for trailing data).
- Decompressed 245 overlays: 4.3 MB total. 238/245 start with MIPS `addiu sp` prologues, 7 are leaf functions (`jr $ra` or `lui`).
- .text/.data boundaries: 3.0 MB code, 1.4 MB data. Found via last `jr $ra` + delay slot + 16-byte alignment.
- 6,261 functions identified. Zero validation issues (full .text coverage, 4-byte aligned).
- Extended ROM: `resources/castlevania2_ni_extended.z64` (19 MB). Original 16 MB + .text sections appended at 0x1000000.
- Generated 245 `[[section]]` entries (vram=0x0F000000, `..ni_ovl_NNN` prefix).

**N64Recomp integration**:
- 11,508 total functions (5,247 original + 6,261 NI).
- 235 NI functions stubbed (jump table analysis failures) — iteratively discovered via `find_stubs.sh`.
- Post-recomp fixes required:
  - `funcs.h`: truncated by N64Recomp. Missing header guard (`#ifndef`/`extern "C"`), missing 50 OS function declarations (`osJamMesg_recomp` etc.).
  - `recomp_overlays.inl`: N64Recomp only output 51 section table entries (original sections). 245 NI sections missing. Fixed by `fix_overlay_table.py` — generates `FuncEntry` arrays + section_table entries.
  - Cross-function `goto` labels: N64Recomp emits `goto L_0FXXXXXX` where target is in a different function (function boundary detection imperfect). Fixed by `fix_undeclared_labels.py` — replaces with `return;`.
  - `funcs_229.c` truncated mid-instruction. Manually completed.
  - 212 `static_NNN_0FXXXXXX` symbols: N64Recomp generates references to jump table helper functions but doesn't output their definitions when parent is stubbed. Generated `ni_ovl_static_stubs.c` with empty stubs.
  - `register_overlays.cpp` needed `#include "funcs.h"` for OS function declarations used in `.inl`.

**Runtime loader** (`src/main/ni_overlay_loader.cpp`):
- Hooks `osMapTLB` in `tlb_segment.cpp` when `vaddr == 0x0F000000`.
- Matches `even_paddr` to NI file index via `ni_decompressed_addrs[]` table (from `lod_init.cpp`).
- Converts NI text index → pair index → overlay ID. Calls `load_overlay_by_id()` / `unload_overlay_by_id()` (mutually exclusive at 0x0F).

**Build**: compiles and links. Committed `8e7cdac`, pushed to main.

**Not yet tested at runtime.** Expected issues:
- ~~Stubbed functions (235) may be game-critical~~ — FIXED: down to 1 stub (see below).
- Relocations: NI code references main-code via `lui`/`addiu` (0x801DXXXX). May need R_MIPS reloc entries if N64Recomp doesn't handle automatically.
- Physical address matching in loader may need tuning depending on how DMA pipeline delivers data.

### 2026-03-20 — Jump table root cause: .data in ROM + 0x0E/0x0F VRAM classification

**Problem**: 235 NI functions had "Failed to determine size of jump table" errors, requiring stubs.

**Root cause 1: Missing .data in extended ROM.** Jump tables (switch/case dispatch) live in the `.data` section. N64Recomp reads jump table entries from `rom_file_path` at `section.rom_addr + offset`. We only put `.text` in the extended ROM — N64Recomp read garbage from adjacent overlays at jump table offsets.

MIPS jump table pattern:
```
sll   $25, $25, 2          # index * 4
lui   $1, 0x0F01           # high half of table addr
addu  $1, $1, $25          # base + idx*4
lw    $25, -5648($1)       # load from 0x0F00E9F0 = .data section
jr    $25                  # jump to table entry
```

**Fix**: Include full `.text + .data` in extended ROM. Section `size` in syms.toml stays as `.text` only (N64Recomp won't try to disassemble .data as code), but the ROM data behind it includes .data so jump table reads work.

**Root cause 2: 79 overlays compiled for `vram = 0x0E000000`, not `0x0F000000`.** The LoD engine (same as CV64) has TWO TLB-mapped overlay regions:
- `0x0F000000` = `nisitenma_ichigo_overlays` — game object code (enemies, UI)
- `0x0E000000` = `nisitenma_ichigo_cutscenes` — cutscene/textbox code

(Confirmed via [CV64 decomp](https://github.com/k64ret/cv64) `castlevania.yaml`)

79 NI overlays are compiled for `0x0E`. Their internal `jal` calls use absolute addresses baked at compile time — `jal 0x0E00XXXX` for self-calls, which we misinterpreted as cross-segment references.

**Discovery method**: For each overlay, count internal `jal` targets in `0x0E` vs `0x0F`. If self-calls go to `0x0E`, the overlay was compiled for `0x0E`. Clean separation: 0 mixed overlays.

**Result**: 1 NI stub remains (`ni_ovl_242_func_0F000358` — genuine cross-segment jump table in 0x0F code reading from 0x0E data). Down from 235.

**Runtime loader** updated: tracks 0x0E and 0x0F independently, loads/unloads the correct section when osMapTLB maps either vaddr.

### 2026-03-20 — Runtime test: NI overlay loading WORKS

**First runtime test** — NI overlay successfully loads at 0x0F000000, passing the original crash.

**Problem**: paddr matching failed. The game decompresses NI files to its own heap in low RDRAM (e.g., `paddr=0x00325000`), NOT our pre-decompressed region at `rdram+0x14000000`. The `ni_decompressed_addrs[]` table stores addresses like `0x94XXXXXX` — completely different.

**Fix**: Content fingerprinting. Read first 128 bytes of decompressed data at `paddr` and match against a precomputed table (`ni_ovl_fingerprints.h`). 128 bytes gives 245/245 unique signatures. Also fixed byte order: RDRAM stores data in original BE format, no swap needed.

**Result**: `[ni_ovl] #1 pair 105 → 0x0F000000 (overlay_id=153)` — overlay loaded, game proceeds past the `LOOKUP_FUNC(0x0F000000)` crash.

**New crash**: `Failed to find function at 0x00000000` — null dispatch pointer, unrelated to NI overlays. Some object's function pointer is NULL. Next investigation target.

### Current state (end of 2026-03-20)
- 245 NI overlays: 166 at 0x0F (game objects), 79 at 0x0E (cutscenes)
- 6,261 functions recompiled, 1 stub (cross-segment jump table)
- Runtime: fingerprint-based overlay identification working
- Blocker: null function pointer crash after first overlay loads
