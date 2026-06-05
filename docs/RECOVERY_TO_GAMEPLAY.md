# Recovery to Gameplay Tracker

Live tracker for getting Castlevania: Legacy of Darkness recompilation from boot/save/intro screens into actual controllable gameplay.

`SCRATCHBOOK.md` is retired. Keep durable recovery state here: baseline, validated experiments, hypotheses, dependencies, and the next concrete action.

## Maintenance Rules

- Update this file whenever recovery work changes the baseline, a hypothesis, or the next action.
- Record every meaningful experiment with build flags, runtime bound, result, and conclusion.
- Keep risky experiments behind compile-time flags; default-off unless the fix is a validated compatibility shim.
- Do not use forced gamestate skips as permanent fixes.
- When a function is identified, update `FUNCTION_MAP.md`, `castlevania2.syms.toml`, and `symbol_addrs.txt` as appropriate.

## Current Baseline (2026-06-04)

- Branch: `main`.
- Last pushed baseline before this working checkpoint: `13c0a12` (`Trace NI pair 120 save reset caller`).
- Build status: clean default-off diagnostic configuration builds successfully on macOS.
- Launch discipline:
  - codesign before every macOS launch;
  - bounded `timeout` runs only;
  - verify no stale `LodRecomp` process after each run.
- Default runtime flags:
  - no boot gamestate skip;
  - no auto-input;
  - all tracing/snapshot flags default off;
  - pair-120 internal-label dispatch fix is default-on;
  - stale direct `func_8009F400`/`osPfsFindFile` shim is default-off.
- Overlay-loading baseline:
  - map overlays no longer hard-code generated recomp overlay IDs; they register by ROM range via `load_overlays(rom, vram, size)`;
  - NI overlays no longer hard-code generated recomp overlay IDs; they register by ROM range from `ni_ovl_data`;
  - the four `overlay_system` variants are modeled as separate sections (`0x745230`, `0x74CD10`, `0x75A570`, `0x7639A0`) and dynamically registered when the game DMAs them to `0x801CAEA0`;
  - the old concatenated `overlay_system` copy/restore hack is removed. A compatibility export `lod_restore_overlay_system_data` remains as a no-op only because the current runtime references it.
- Validated behavior:
  - Clean default-off smoke (`/tmp/lod_clean_default_smoke.log`, 12s) builds, launches, follows save/Pak reads, reaches `gs=12`, times out cleanly, and leaves no stale process.
  - Diagnostic scripted-input run (`/tmp/lod_ni_range_fix_trace_run.log`, 20s) progresses `gs=4 → -1 → 1 → -12 → 12 → -5 → 5 → -6 → 6`, registers `overlay_system` variants and map overlays, and leaves no stale process.
  - User visual confirmation: the game reached the start/title screen after the NI range-loader fix.
  - User manual new-game attempt reached character select, then transitioned `gs=8 → -3 → 3` after the book-close sequence and stayed visually black while still rendering/polling.
  - Local debug-input reproduction now reaches the same route automatically. With NI loading keyed to the actual TLB-mapped vaddr, the pair-233 black-screen loop advances through two `gs=3` scene loads and produces non-black CFB snapshots.
  - User visual confirmation: actual gameplay is reachable by skipping/advancing the intro sequence after character selection; screenshot shows Reinhardt in a playable scene with HUD/status/gold/item UI and action-view text.
  - User visual confirmation: in low-resolution mode, the prologue/intro sequence can play through fully and enter actual gameplay without skipping.
  - Caveat: long low-resolution idle in gameplay still crashes in the post-RDRAM/KSEG0 guard family: `Signal 10` at `0x380810002`, i.e. runtime RDRAM base plus `0x80810002`. This is two bytes past the current 64KB guard, so the earlier small guard did not fully model the game's out-of-RDRAM reads.
  - Caveat: high-resolution mode still appears to expose a no-match NI/TLB crash during the intro/book animation path. Treat gameplay stability and resolution-dependent intro stability as active blockers, not basic gameplay reachability.
  - Earlier user idle test in actual gameplay also hit the same late-crash family seen locally: `Signal 10` at `0x380800002`, after many `gs=3` frames and repeated last lookups through `func_80011E48`.
  - User high-resolution manual run hit a different-looking instance of the older no-match NI TLB crash family: `Signal 10` at `0x3880006d0` after a base `0x0F000000` map from paddr `0x0039B000` whose first words were a close but non-matching/corrupted NI prologue. Treat display-resolution choice as a repro variable until low-res/high-res A/B confirms otherwise.
  - Previous `gs=5` crash after NI pair `104` was caused by stale generated overlay IDs after adding new sections, not by pair-104 game logic itself.

## Goal

Reach actual controllable gameplay through the game's natural state flow, not by permanently skipping screens or forcing gamestates.

## Current Active Work Item

### G6-001 — Gameplay Reached; Stabilize Intro/Cutscene Route

Objective:

- Preserve the now-working route from title/start/new game into actual controllable gameplay.
- Stabilize long gameplay idle and the high-resolution intro/cutscene path.
- Exercise controller input through the game's natural menu flow; do not permanently force gamestates.
- Keep overlay registration range-based and avoid generated overlay-ID assumptions.

Current evidence:

- `gs=4` save/Controller Pak flow is no longer the blocker.
- `gs=12` Expansion Pak screen is no longer the blocker.
- `gs=5`/NI pair-104 crash is no longer the blocker after range-based NI overlay registration.
- A diagnostic scripted-input run reached `gs=6`; the user visually confirmed seeing the start/title screen.
- Manual input can progress through new-game/character-select into the post-book-close transition.
- Debug-only input can now reproduce the route locally: `gs=6 → -8 → 8 → -3 → 3`.
- The old pair-233 `gs=3` black-screen loop was caused by NI overlays being registered at a static guessed segment. NI loading now uses the actual TLB-mapped vaddr (`0x0E`/`0x0F`) as the source of truth.
- User confirmed actual gameplay is reachable by skipping/advancing the intro sequence after character select.
- User confirmed actual gameplay is also reachable through the natural intro/prologue path in low-resolution mode, with no skip.
- Remaining active risk: high-resolution mode appears to expose a no-match NI/TLB crash during the same intro/book animation route. Earlier natural-intro crashes were reported as `Signal 15`-style, and later logs clarified actionable faults as `Signal 10` at `0x380800002` and `0x3880006d0`. Low-res natural flow is now validated; high-res/no-match classification remains open.
- Long low-res gameplay idle is not yet stable. A later idle crash hit `Signal 10` at `0x380810002`, with last lookups again in the graphics/DMAMgr path (`0x80043768`, `createGraphicTasks_overlay`, `ni_system_dispatcher`, `func_80011D10`, `DMAMgr_updatePendingFileLoad`). The address decodes as runtime RDRAM base `0x300000000` plus `0x80810002`, which is two bytes beyond the current `rdram+0x80800000` 64KB guard.
- The user idle-crash log showed repeated NI/TLB maps where `0x0F00A000` was skipped as NI overlay copy, but `0x0F012000` was not. Extending the skip window to the actual loaded NI overlay size fixed that symptom in local automation: `0x0F012000` now logs `(NI skip copy)`.
- New `gs=3` stability progress: local automation gets past the previous `0x8F00AB8C` missing-function abort and the later `0x3880006d0` crash when base NI TLB maps with no fingerprint match are handled by preserving the registered overlay functions while allowing the normal TLB copy for that base page.
- A 190s scripted `gs=3` route reached later map overlay `ROM 0x00835D10`, cycled multiple NI pairs (`3`, `20`, `28`, `57`, `67`, `74`, `95`, `130`, `131`), produced non-black CFB snapshots, and ended only by timeout.
- The repeated `0x380800002` crash decoded to the runtime RDRAM base `0x300000000` plus `0x80800002`: a KSEG0 access two bytes past the 8MB Expansion Pak mirror. A small zero-filled guard at `rdram+0x80800000` lets this behave as a bounded open/guard read instead of host SIGBUS. With the guard, the same scripted route reached `map_ovl_09` (`ROM 0x00835D10`) and ended only by timeout (`exit=124`) after 180s.
- The later `0x380810002` idle crash shows the current 64KB post-RDRAM guard is too narrow for all observed accesses; do not blindly grow it without deciding whether this should become a larger open-bus mirror model or whether the caller is using a bad pointer/resource.
- Added default-off `LOD_ENABLE_KSEG0_FAULT_TRACE` to capture RDRAM-relative fault offsets plus lightweight context snapshots. The current low-perturbation trace wraps only `DMAMgr_updatePendingFileLoad` (`0x80011E48`); an earlier broader trace also wrapped `func_80043768`.
- A 700s automated boot-input + broader KSEG0-trace run reached the same low-res idle signature (`gs=3`, `exec=0x38000000`, `ni=0x803241C0`, width 320), passed the user's previous DL/VI crash window, and timed out cleanly beyond DL#21000/VI#40000 without a crash. This made the idle KSEG0 crash look nondeterministic or timing-sensitive.
- A later 700s automated boot-input + DMAMgr-only KSEG0-trace run reproduced the idle crash: `Signal 10` at `0x380810002`, with `Fault offset from rdram base: 0x80810002`, decoded as KSEG0 mirror `phys=0x810002` past 8MB RDRAM. The latest snapshot was from `DMAMgr_updatePendingFileLoad`, at `gs=3`, map ROM `0x007BA1D0`, `exec=0x38000000`, `ni=0x803241C0`, repeated `0x0F00C000` NI skip-copy maps, `a0=0x80323C50`, chunk pointer `0x8032EE58`, slot `15`, and a zero-looking entry. The local halfword helper was later confirmed to match generated `MEM_HU` for aligned fields; the remaining concern is that this snapshot may be the last DMAMgr entry before a downstream PI/DMA fault, not the exact faulting instruction.
- A follow-up 700s automated trace with added DMAMgr active/current DMA-field capture timed out cleanly past the previously reproduced crash window (`DL#21100` / `VI#41800`, `gs=3`, `exec=0x38000000`, `ni=0x803241C0`, repeated `0x0F00C000` NI skip-copy maps). The temporary scan also proved inactive `file_info[]` slots can contain garbage/code-like data, so the trace was tightened to report only active/current invalid RAM destinations rather than arbitrary stale slots.
- Added default-off `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT` for KSEG0/idle repros. It aggressively sends controller Start/A through skippable menu/cutscene sections, then releases input once it reaches `gs=3 exec=0x38000000 ni=0x803241C0`.
- Fast script validation reached the idle target at `DL#972`, released input, then reproduced `Signal 10` at `0x380810002` shortly after. The captured DMAMgr fields were valid (`cur_ram=0x803AC448`, `cur_size=0x2CE8`, `ram_ok=1`, `active_ram_ok=1`), so the immediate OOB access is probably not the active/current DMAMgr destination itself. Shift the next trace to downstream PI DMA or graphics/scheduler path evidence.
- Crash handler now prints a short native backtrace on POSIX platforms. Two fast backtrace-enabled attempts reached/released at the idle target and timed out cleanly, so a native stack for `0x380810002` is still pending.
- CV64 cross-reference confirms the fault path is the engine `DMAMgr` / `DMAChunkMgr_t`; `0x8001A42C` is the CV64-equivalent `DMA_ROMCopy`, and `0x80011E48` is now named `DMAMgr_updatePendingFileLoad`.
- New manual high-res evidence means the old `0x3880006d0` no-match family is not fully eliminated. The observed paddr words (`27BCFFE8 AFBC0014 8480000C 24600000`) are absent from the ROM/resource set and differ by only a few bits from the common NI overlay prologue (`27BDFFE8 AFBF0014 8483000E 24630001`), which points more toward stale/corrupted TLB buffer contents than a legitimate missing overlay fingerprint.

Next action:

1. Do not chase audio yet. Stabilize the `gs=3` NI/TLB and post-RDRAM/KSEG0 paths first.
2. Use `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1` for KSEG0/idle repros; keep the full natural prologue route only for intro-stability checks.
3. Improve KSEG0/PI evidence before changing the guard model: capture a native backtrace on the next fast-script crash and/or add targeted PI DMA destination logging to prove whether any path is writing/reading `0x80810000`/past-RDRAM addresses after DMAMgr itself reports valid active/current RAM.
4. Avoid blindly widening the post-RDRAM guard. Only test a larger/open-bus mirror behind a temporary flag after the DMAMgr/PI DMA pointer/resource state is understood.
5. Add/enable only targeted no-match classification evidence for base `0x0F000000` maps: current loaded NI pair/ROM, paddr, first words, active resolution mode if available, and whether the page is code-like/corrupted vs data-like.
6. Use the user's A/B result as the current repro split: low-res natural intro reaches gameplay; high-res reproduced `0x3880006d0`. Keep high-res as the primary repro mode until contradicted.
7. Repeat bounded scripted routes with CFB snapshots only when visual/scene-output evidence is needed; do not pay full intro cost for every KSEG0 iteration.
8. Use internal CFB snapshots for `gs=3`, `gs=6`, and `gs=8` to verify whether scene output is still valid immediately before any remaining crash.
9. Identify and name the `gs=3` scene/map transition functions encountered after character select, including the map overlays at `0x007A60F0`, `0x007AB090`, `0x007CDDE0`, `0x00835D10`, and the newly observed `0x007B1F60`/`0x007C89A0` path.

## Resolved Work Item

### G5-001 — Overlay-System and NI Overlay Registration

Status: resolved for the current baseline; keep regression tests.

What changed:

- Added the missing `overlay_system_1`, `overlay_system_2`, and `overlay_system_3` sections to `castlevania2.syms.toml` and regenerated through `./tools/regen_recomp.sh`.
- Changed map overlay registration from hard-coded generated overlay IDs to ROM-range based `load_overlays`.
- Changed NI overlay registration from hard-coded `NI_OVERLAY_ID_START + pair` to ROM-range based `load_overlays`.
- Kept RDRAM byte copying separate from recomp function registration.
- Implemented `__ll_rshift_recomp` as a real signed 64-bit right-shift helper instead of an empty OS-style stub.
- Removed the old/disproven concatenated `overlay_system` copy experiment.

Key facts:

- The game has four mutually exclusive `overlay_system` variants loaded to the same VRAM base `0x801CAEA0`. Treating later variants as data hidden behind variant 0 caused invalid dispatch pointers such as `0xA20B0074`.
- Adding the missing `overlay_system` sections shifted generated section/overlay IDs. Any code using manually derived IDs became stale immediately.
- The `gs=4` regression after adding the new sections was caused by the NI loader registering the wrong section while copying pair-105 data. Runtime symptom: repeated `get_function(0x00000000)` and no PAK path.
- Range-based NI registration fixed that mismatch and restored the PAK path, then allowed progress into title/start.
- A/B test of the old concatenated init copy did not restore behavior and caused `Signal 10`; it is not a safe baseline path.

Regression checks to keep:

- Clean default-off existing-save smoke should reach `gs=12` without `get_function(0x00000000)` before the PAK path.
- Scripted-input diagnostic run should reach `gs=6` / title-start screen without generated overlay-ID hard-coding.
- `RecompiledFuncs/recomp_overlays.inl` may shift section indices after regen; map/NI loaders must remain range-based.

### G4-001 — Save/Controller Pak Screen Exit

Status: functionally resolved for the current baseline; keep regression tests.

What changed:

- Retired stale direct `func_8009F400` (`osPfsFindFile`) shim by making `LOD_OVERRIDE_FUNC_8009F400=0` the default.
- Added a scoped compatibility fix for NI overlay pair 120's result dispatcher (`LOD_FIX_NI_PAIR120_RESULT_LABELS=1` default).
- Kept both changes reversible with compile-time flags for A/B testing.

Key facts:

- The previous `gs=4` loop was not solved by a gamestate skip.
- The direct `func_8009F400` shim could return stale NOT FOUND after the game created a Controller Pak note. Letting the recompiled/native PFS path run lets the note be found/used.
- `ni_ovl_120_result_schedule_dispatch` indexes callback table `0x0F0016BC` by `obj+0x48`, stores the callback result to `obj+0x38`, resets current schedule to `0` on nonzero, and advances on zero.
- The pair-120 table intentionally mixes function starts and internal labels:
  - `[0]=0x0F00086C`, `[1]=0x0F000884`, `[2]=0x0F0008D4`, `[3]=0x0F0008E4`, `[4]=0x0F0008F4`, `[5]=0x0F00091C`, `[6]=0x0F000958`, `[7]=0x0F000964`, `[8]=0x0F000974`, `[9]=0x0F000984`, `[10]=0x0F000070`, `[11]=0x0F0000BC`.
- Entries `0`, `2`, `3`, `6`, `7`, and `8` are internal labels, not function starts. N64Recomp range fallback resolves them to the containing function start, which changes callback return values and can perturb the save/Pak schedule.
- The fix handles only the pair-120 dispatcher at `0x0F000484`; it does **not** globally register internal-label addresses in the reusable `0x0F...` overlay range, avoiding cross-overlay contamination when pair `101`, `105`, etc. reuse the same VRAM.

Regression checks to keep:

- Existing-save default 20s run should reach `gs=12` without missing functions or crashes.
- True-empty-Pak default no-input run should format the Pak and stay in `gs=4` without crash.
- True-empty-Pak auto-input run should reach `gs=12` without using the direct `func_8009F400` shim.
- Compile-only check for `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS=0` with `LOD_OVERRIDE_FUNC_8001D398=1` must remain buildable.

## Milestones

### Milestone 1 — Explain and Exit `gs=4` Naturally

Status: achieved for the current baseline.

Success evidence:

- Existing-save path reaches `gs=12` with default flags.
- Fresh-Pak + scripted create path reaches `gs=12` with default PFS semantics plus debug input only.
- No permanent forced gamestate skip is involved.

### Milestone 2 — Identify and Drive `gs=12`

Status: achieved.

Success evidence:

- `gs=12` is confirmed as the Expansion Pak screen.
- Default existing-save run advances through it naturally to `gs=5`.
- No forced `gs=12` skip is needed in the baseline.

### Milestone 3 — Reach Title/Intro Flow

Status: achieved for the current baseline.

Success evidence:

- Diagnostic scripted-input run survived `gs=5`, advanced to `gs=6`, and loaded the title/start map overlay.
- User visually confirmed seeing the game start/title screen.

### Milestone 4 — Reach Controllable Gameplay

Status: active.

Success condition:

- A playable scene is reached.
- Controller input works.
- Renderer remains stable.
- RSS remains bounded.
- No stale process remains after bounded run.

### Milestone 5 — Burn Down Remaining Overrides

Status: ongoing.

Each override should be removed or documented as a required compatibility shim.

## Override / Probe Matrix

| Flag | Default | Current disposition | Next test |
|------|---------|---------------------|-----------|
| `LOD_OVERRIDE_FUNC_8009E480` | on | Unknown; audio-chain no-op may mask a real issue | Build/run with `=0` after `gs=12` input path is understood |
| `LOD_OVERRIDE_FUNC_8001D398` | on | Runtime necessity unknown; build-gate combo with inserted-status disabled is fixed | Build/run with `=0` |
| `LOD_OVERRIDE_FUNC_8001C93C` | on | Unknown; Pak validation gate | Build/run with `=0` |
| `LOD_OVERRIDE_FUNC_8009F400` | **off** | Retired stale direct PFS find shim; keep only for A/B regression checks | Confirm no future path depends on it; eventually remove |
| `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS` | on | Compile combo passed with this disabled while `FUNC_8001D398` stayed enabled | Build/run with `=0` |
| `LOD_FIX_NI_PAIR120_RESULT_LABELS` | **on** | Validated scoped compatibility fix for pair-120 internal-label callback table | Keep on; use `=0` only for A/B regression |
| `LOD_ENABLE_SAVE_PAIR120_TRACE` | off | Debug-only trace for pair-120 dispatcher/table callback details | Enable only when investigating pair-120 regressions |
| `LOD_ENABLE_SAVE_AUTO_INPUT` | off | Debug-only scripted input for `gs=4` save/Pak path | Replace or generalize for `gs=12` scripted input tests |
| `LOD_ENABLE_SAVE_HANDLER_TRACE` | off | Debug-only function-map wrapper probe | Use only for targeted handler-entry confirmation |
| `LOD_ENABLE_SAVE_EXIT_TRACE` | off | Debug-only wrapper probe for main-code save/PFS exit candidates and schedule advances | Use only if `gs=4` regresses |
| `LOD_ENABLE_SAVE_STATE45_TRACE` | off | Debug-only state-4/5 schedule-helper trace | Use only if `gs=4` regresses |
| `LOD_ENABLE_KSEG0_FAULT_TRACE` | off | Debug-only lightweight crash-context snapshot for post-RDRAM/KSEG0 faults; current low-perturbation form wraps only `DMAMgr_updatePendingFileLoad` and records active/current DMA RAM validity | Use with faster skip-to-gameplay harness; add PI DMA destination evidence if active/current fields stay valid before crash |
| `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT` | off | Debug-only aggressive controller/PIF input to skip quickly to the gameplay idle signature; releases input at `gs=3 exec=0x38000000 ni=0x803241C0` | Use for KSEG0 idle repro loops instead of paying full prologue cost |
| `LOD_ENABLE_BOOT_GS_SKIP` | off | Debug-only; never a permanent fix | Use only for downstream comparison, never as baseline |

## Experiment Log

Record durable experiments here. Keep entries concise and technical.

| ID | Flags / build | Runtime bound | Result | Conclusion |
|----|---------------|---------------|--------|------------|
| BASE-001 | default baseline at `18a56c5` | 20s | Built, launched, reached `gs=4`; save object `255 → 1 → 2 → 3`; NI loop `105 / 120 / 101`; no internal crash | Clean baseline was viable; primary blocker was natural transition out of `gs=4` |
| BUILD-001 | `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS=0`, `LOD_OVERRIDE_FUNC_8001D398=1` compile-only check | n/a | `ignored_func_stubs.cpp` compiled successfully | Review-reported dependent override-gate compile failure is fixed |
| G4-001A..L | historical traces through map_ovl_34 dispatch, object schedule helpers, and NI pair 120 | 20-25s | Identified parent reset caller as `ni_ovl_120_result_schedule_dispatch`; decoded table at `0x0F0016BC`; proved tested loop was not the main-code `0x8001AF54` path | Active blocker narrowed to PFS find semantics + pair-120 internal-label callbacks |
| G4-001M | `LOD_ENABLE_SAVE_AUTO_INPUT=1`, `LOD_ENABLE_SAVE_STATE45_TRACE=1`, `LOD_ENABLE_SAVE_PAIR120_TRACE=1`, `LOD_FIX_NI_PAIR120_RESULT_LABELS=1`, `LOD_OVERRIDE_FUNC_8009F400=0` | 35s | Native/recompiled PFS path found/used the note after writes; pair-120 label fix allowed advance; reached `gs=12`; no missing function/crash | Direct `func_8009F400` shim was stale/harmful; scoped pair-120 fix is the right compatibility layer |
| G4-001N | default flags, existing virtual Pak save | 20s | Reached `gs=12`; no auto-input; no `Failed to find function`; no `[CRASH]`; no stale process | Default existing-save baseline now leaves `gs=4` naturally |
| G4-001O | default flags, truly empty virtual Pak, no input | 25s | Formatted empty Controller Pak; stayed in `gs=4`; no missing function/crash; no stale process | Empty-Pak default waits for menu input, as expected |
| G4-001P | `LOD_ENABLE_SAVE_AUTO_INPUT=1`, truly empty virtual Pak | 35s | Formatted Pak, injected `Down`, `A`, `A`, transitioned `gs=4 → -1 → 1 → -12 → 12`; no missing function/crash | Fresh-save create/use path reaches `gs=12` with native PFS and debug input only |
| BUILD-002 | syntax-only: `LOD_ENABLE_SAVE_PAIR120_TRACE=1` with fix on and with fix off | n/a | `ni_overlay_loader.cpp` compiled in both combinations | Trace/fix gates remain buildable |
| BUILD-003 | syntax-only: `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS=0`, `LOD_OVERRIDE_FUNC_8001D398=1`, `LOD_OVERRIDE_FUNC_8009F400=1` | n/a | `ignored_func_stubs.cpp` compiled with warnings only from third-party headers | Dependent override gates remain buildable |
| G12-001A | default flags, existing virtual Pak save | 45s | User visually confirmed `gs=12` as Expansion Pak screen; run advanced `gs=12 → -5 → 5`, loaded NI pair `104`, then crashed with `Signal 11` at `0x4001cad9c`; no missing-function lookup; no stale process | `gs=12` is not the blocker; the next blocker is `gs=5` / NI pair-104 transition |
| G12-001B | default flags after docs/symbol updates | 12s | Built, codesigned, reached `gs=12`, timed out cleanly before the known `gs=5` crash window; no missing-function lookup/crash/stale process | Short smoke confirms the committed baseline reaches Expansion Pak screen cleanly |

| OVLSYS-001 | Added `overlay_system_1/2/3`, range-registered overlay-system DMA, no concat copy | 20s | Build succeeded, but NI loader still hard-coded stale generated IDs; run stuck in `gs=4` with repeated `get_function(0x00000000)` after pair 105 | Missing overlay-system variants were necessary but exposed the next generated-ID bug |
| OVLSYS-002 | `LOD_ENABLE_OVLSYS_CONCAT_INIT_COPY=1` A/B diagnostic | 20s | Did not restore PAK path and later hit `Signal 10` | Old concatenated overlay-system copy is unsafe and was removed |
| NI-001 | Range-based NI overlay registration, diagnostic save/title traces and scripted input | 20s | Restored PAK path; progressed `gs=4 → -1 → 1 → -12 → 12 → -5 → 5 → -6 → 6`; user saw start/title screen | Hard-coded NI overlay IDs were the `gs=4` regression and `gs=5` crash cause after section additions |
| CLEAN-004 | all debug/input/trace flags off | 12s | Built, codesigned, reached `gs=12`, timed out cleanly; no stale process | Clean baseline is buildable and reaches Expansion Pak screen without diagnostics |
| G6-001A | manual input from title/start into new game | user run | Character select worked; after book-close transition the state changed `gs=8 → -3 → 3`; `osViBlack active=0`, VI framebuffers continued rotating at 640px, map overlay `ROM 0x007A60F0` loaded, `overlay_system_2` (`ROM 0x0075A570`) registered, and NI pair `233` looped while the picture was black | Start/new-game flow is functional up to post-selection; next blocker is a live `gs=3` black-screen loop, not the old save/title blockers |
| G6-001B | `LOD_ENABLE_BOOT_INPUT_SCRIPT=1`, `LOD_ENABLE_CFB_SNAPSHOT=1`, NI loader uses actual TLB `mapped_vaddr` | 85s | Reproduced `gs=6 → -8 → 8 → -3 → 3` locally; pair-233/0x0E dispatch no longer aborts; `gs=3` advances through `map_ovl_09` then `map_ovl_10`; CFB snapshots become non-black; later crashes with `Signal 10` at `0x380800002` | Static 0E/0F NI classification was wrong; mapped-vaddr registration is a real fix. New blocker is a later intro/cutscene stability crash in `gs=3` |
| G6-001C | manual input, skip/advance intro after character select | user run | User reached actual gameplay; screenshot shows Reinhardt in a playable scene with HUD/status/gold/item UI and action-view text | Basic gameplay is reachable. Do not mark recovery complete until natural intro playout crash is characterized and a bounded gameplay stability pass is done |
| G6-001D | manual skip-to-gameplay, then idle | user run | After several minutes in `gs=3`, crashed with `Signal 10` at `0x380800002`. Last lookups were mostly `0x80043768`, then `createGraphicTasks_overlay`/`ni_system_dispatcher`/`func_80011D10`/`func_80011E48`. Log alternated NI skip-copy maps at `0x0F00A000` with normal-copy maps at `0x0F012000`. | Gameplay route is real but not yet stable. The `0x0F012000` normal-copy path is a concrete suspect because it lies beyond the current 64KB NI skip range while larger NI overlays can exceed that size. |
| G6-001E | NI TLB skip window extended to actual loaded overlay size; `LOD_ENABLE_BOOT_INPUT_SCRIPT=1`, `LOD_ENABLE_CFB_SNAPSHOT=1` | 140s | Default-off smoke still reached `gs=12`. Scripted route reached `gs=3`; `0x0F012000` now logs `ni_span=0x14000 (NI skip copy)` instead of normal-copy; previous `0x380800002` crash was not reproduced. Run later failed at `get_function(0x8F00AB8C)`, with loaded section `ram=0x0F000000 size=0xDD30`. | Skip-window fix is progress and exposed the next bug: NI dispatch can use the `0x8F...` CPU/MEM_W mirror of a loaded `0x0F...` function address. |
| G6-001F | NI CPU mirror aliases registered for `0x8F`/`0x8E`; `LOD_ENABLE_BOOT_INPUT_SCRIPT=1`, `LOD_ENABLE_CFB_SNAPSHOT=1` | 170s | Route survived past the previous `get_function(0x8F00AB8C)` abort. It advanced through additional `gs=3` transitions and map overlay `ROM 0x007CDDE0`, then crashed with `Signal 10` at `0x3880006d0`. Last lookups included `0x80011060`, `0x0F0004E0`, `0x0F000554`, and final `0x0F000858`. Just before the crash, `ni_overlay_on_tlb_map` warned that paddr `0x003A9000` was zero-filled and paddr `0x0038E000` did not match any known NI fingerprint. | Mirror aliasing is progress. Next blocker is no-match TLB map handling or missing NI fingerprint/variant, not the old `0x8F00AB8C` lookup failure. |
| G6-001G | Base NI no-match maps preserve registered functions but fall through to normal TLB copy; `LOD_ENABLE_BOOT_INPUT_SCRIPT=1`, `LOD_ENABLE_CFB_SNAPSHOT=1` | 190s | Default-off smoke reached `gs=12`. Scripted route reached and stayed in `gs=3` through the previous no-match map area, loaded later map overlay `ROM 0x00835D10`, cycled NI pairs including `3`, `20`, `28`, `57`, `67`, `74`, `95`, `130`, and `131`, produced non-black CFB snapshots, and exited only due to the bounded `timeout` (`exit=124`). No `[CRASH]`, missing-function abort, or stale process. | Correct no-match behavior appears to be “normal-copy the base TLB page, but keep current NI function registrations unless a new fingerprinted overlay is loaded.” This removes the known automated `gs=3` crash signatures. |
| G6-001H | `LOD_ENABLE_GS3_ANIM_TRACE=1`, CV64 DMAMgr cross-reference, then KSEG0 post-8MB guard | 180s | Widened DMAMgr trace showed repeated `DMAMgr_updatePendingFileLoad` calls on `DMAChunkMgr_t` with `pending=0` before the old `Signal 10` at `0x380800002`. The fault address decoded as `rdram=0x300000000` + `0x80800002`, i.e. a KSEG0 access two bytes beyond the 8MB RDRAM mirror. Adding a zero-filled 64KB guard at `rdram+0x80800000` let the same route pass the old crash, advance through map overlays `0x007B1F60`, `0x007C89A0`, `0x007CDDE0`, and `0x00835D10`, produce non-black CFB snapshots, and exit only due to timeout. | The old `0x380800002` fault is not a missing function; it is an out-of-range KSEG0/open-bus-style read. Keep the guard small and documented while tracing the exact caller. |
| G6-001I | all debug/input/trace flags off, post-RDRAM guard present | 20s | Clean/default build reached `gs=12`; no `[CRASH]` or missing-function abort. The bounded `timeout` delivered SIGTERM (`Signal 15`). A concurrent/manual `LodRecomp` process was mistakenly treated as stale and killed; do not treat that as app behavior. | Default baseline remains buildable with the guard. Longer no-heavy-trace scripted route still needs confirmation before asking for manual natural-intro retest. Never kill a visible `LodRecomp` unless it is proven to be the process just launched by the test command. |
| G6-001J | manual high-resolution route after post-RDRAM guard | user run | During the intro/book animation path, crashed with `Signal 10` at `0x3880006d0`. Just before the crash, `ni_overlay_on_tlb_map` warned for base `0x0F000000` paddr `0x0039B000` with no matching overlay. Words were `27BCFFE8 AFBC0014 8480000C 24600000`; expected common NI prologue was `27BDFFE8 AFBF0014 8483000E 24630001`. Last lookups ended at `0x0F000858`. Offline search did not find the observed word sequence in LoD/CV64 ROM resources. | High-res may alter timing/memory pressure enough to surface the no-match path. This looks more like stale/corrupted TLB buffer contents than a missing ROM fingerprint. The `0x3880006d0` family remains active and needs no-match classification before further gameplay-stability claims. |
| G6-001K | manual low-resolution natural intro route after post-RDRAM guard | user run | User let the prologue/intro play through fully with no skip and reached actual gameplay. | Natural boot/title/new-game/intro-to-gameplay flow is now validated in low-res mode. The remaining intro blocker appears resolution-dependent or timing-dependent, with high-res as the best current repro for the no-match crash. |
| G6-001L | manual low-resolution natural intro route, then idle in actual gameplay | user run | After a while standing still in `gs=3`, crashed with `Signal 10` at `0x380810002`. Last logs showed repeated `0x0F000000` TLB maps from paddr `0x00393000` with `ni_span=0x14000 (NI skip copy)`, VI width 320, and final lookups through `0x80043768`, `createGraphicTasks_overlay`, `ni_system_dispatcher`, `func_80011D10`, and `DMAMgr_updatePendingFileLoad`. | Low-res natural intro route works, but long gameplay idle is not stable. The fault is `rdram=0x300000000 + 0x80810002`, two bytes past the 64KB post-RDRAM guard added for `0x80800002`; this is the same KSEG0/open-bus family, not a new missing-function failure. |
| G6-001M | `LOD_ENABLE_KSEG0_FAULT_TRACE=1`, `LOD_ENABLE_BOOT_INPUT_SCRIPT=1`, no CFB/DMAMgr heavy trace | 700s | Added default-off lightweight KSEG0 fault snapshots, confirmed wrappers install, then ran automated route through `gs=12 → 5 → 6 → 8 → 3`. It reached the same low-res idle signature as the user's crash (`gs=3`, `exec=0x38000000`, `ni=0x803241C0`, width 320), passed DL#17000/VI#34200, continued beyond DL#21000/VI#40000, and exited only due to bounded timeout (`exit=124`). Build was reset to default/off afterward. | Trace support is validated, but this run did not reproduce the idle crash. Treat the `0x80810002` fault as nondeterministic/timing-sensitive until captured with `LOD_ENABLE_KSEG0_FAULT_TRACE=1`; do not blindly widen the post-RDRAM guard yet. |
| G6-001N | `LOD_ENABLE_KSEG0_FAULT_TRACE=1` reduced to DMAMgr-only, `LOD_ENABLE_BOOT_INPUT_SCRIPT=1`, no CFB/GS3 anim trace | 700s | Reproduced `Signal 10` at `0x380810002` (`exit=138`) near DL#16200/VI#32400. Crash handler decoded fault offset `0x80810002` as KSEG0 mirror `phys=0x810002` past 8MB. Snapshot: `func=0x80011E48`, `gs=3`, map ROM `0x007BA1D0`, `exec=0x38000000`, `ni=0x803241C0`, repeated `0x0F00C000` NI skip-copy maps, `a0=0x80323C50`, chunk `0x8032EE58`, slot `15`, zero-looking entry. Build was reset to default/off afterward. | Reduced trace confirms the low-res idle crash is temporally near the DMAMgr/update path, not only the graphics helper. The aligned halfword helper matches `MEM_HU`; next evidence should prove whether a downstream PI DMA writes to a past-RDRAM destination before deciding whether to grow the post-RDRAM guard. |
| G6-001O | `LOD_ENABLE_KSEG0_FAULT_TRACE=1` with active/current DMAMgr DMA-field capture, `LOD_ENABLE_BOOT_INPUT_SCRIPT=1`, no CFB/GS3 anim trace | 700s | Reached the same idle signature and passed the prior reproduced crash window (`DL#21100`, `VI#41800`, `gs=3`, `exec=0x38000000`, `ni=0x803241C0`, repeated `0x0F00C000` NI skip-copy maps). No `[CRASH]`; bounded run ended by `Signal 15` timeout. A temporary all-slot scan showed inactive `file_info[]` slots contain garbage/code-like stale data, so the trace was tightened to active/current invalid RAM destinations only. Build was reset to default/off afterward. | Crash remains nondeterministic/timing-sensitive. Arbitrary `file_info[]` garbage is not evidence unless the slot is active/pending; next iterations should skip the prologue and, if the crash recurs with valid DMAMgr fields, instrument PI DMA destination just before `do_rom_read`. |
| G6-001P | `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1`, `LOD_ENABLE_KSEG0_FAULT_TRACE=1`, no CFB/GS3 anim trace | 300s | Fast script reached `gs=3 exec=0x38000000 ni=0x803241C0` at `DL#972` and released input. It then reproduced `Signal 10` at `0x380810002` (`exit=138`) soon after. Crash snapshot: map ROM `0x007BA1D0`, chunk `0x8032EE58`, `slot=15`, `pending=0`, `cur={rom=0x00BD66C0 ram=0x803AC448 size=0x2CE8 file=0x84}`, `ram_ok=1`, `active_ram_ok=1`; last lookups again ended in `func_80011D10` / `DMAMgr_updatePendingFileLoad`. Build was reset to default/off afterward. | Fast harness is validated and makes this crash cheap to repro. Since active/current DMAMgr RAM is valid at crash, the next instrument should be lower/downstream (PI DMA destination or graphics/scheduler path), not arbitrary DMAMgr file-info slot scanning or blind guard widening. |
| G6-001Q | `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1`, `LOD_ENABLE_KSEG0_FAULT_TRACE=1`, crash native backtrace enabled | 2 × 180s | Both attempts reached `gs=3 exec=0x38000000 ni=0x803241C0` and released input at the same fast-script checkpoint. Neither reproduced the crash; both ended by bounded timeout (`exit=124`). Build was reset to default/off afterward. | Native backtrace support is ready but not yet captured for the `0x380810002` fault. Failure remains timing-sensitive; use fast harness for further repros and consider PI/DMA destination logging if backtrace capture remains elusive. |

## Open Questions

- What exact signal/address/function context occurs when the intro sequence is allowed to play naturally?
- Does the reported natural intro `Signal 15` match the repeated late `gs=3` `Signal 10`/`0x380800002` crash, or are they separate blockers?
- Does high-resolution mode uniquely reproduce the `0x3880006d0` no-match crash, or can long low-resolution sessions still hit it later?
- Is the NI TLB skip range supposed to cover the full 1MB reusable `0x0E`/`0x0F` segment rather than only the currently loaded overlay span?
- Are `0x8E...`/`0x8F...` dispatch targets always safe to canonicalize/register as aliases for `0x0E...`/`0x0F...` NI overlays?
- For no-match base NI TLB maps, how should code-like but non-fingerprinted/corrupted pages be handled? Normal-copying while preserving old registrations helped one scripted route, but the manual high-res crash shows stale registered functions plus mismatched bytes can still be unsafe.
- What exact caller/source produces the `0x80800002` and `0x80810002` KSEG0 accesses? Are these legitimate open-bus/end-of-RDRAM probes or upstream object/resource pointer bugs? The captured DMAMgr-only trace points near `DMAMgr_updatePendingFileLoad`, but downstream PI DMA destination evidence is still needed.
- What scene/maps do `map_ovl_09` (`ROM 0x007A60F0`) and `map_ovl_10` (`ROM 0x007AB090`) correspond to, and should either lead directly into gameplay?
- Are remaining black-looking moments in `gs=3` legitimate fades while the non-black CFB snapshots show scene progress?
- Are remaining default-on PFS/Pak/audio overrides still required once gameplay is reachable?
- Is the NI pair `53`/`104` alternation in `gs=5` expected intro/title behavior or a sign of a loop that should be broken by normal input?

## Dependency / CV64 Posture

Do not wholesale update dependencies while the boot-to-gameplay path is still being characterized. Prefer small, justified cherry-picks only if directly relevant to:

- Controller Pak/PIF behavior;
- overlay loading/function lookup;
- scheduler/message queue correctness;
- renderer crash/leak fixes encountered on the active path.

Checked 2026-06-04 with `git fetch --all --prune --tags`:

| Repo | Local state | Upstream delta | Notes |
|------|-------------|----------------|-------|
| `lib/N64ModernRuntime` | detached `366e149` | origin/main has 4 commits not in local; local is 9 commits off origin/main history | Potentially relevant changes include argument stack handling and lookup API changes; do not update blindly |
| `lib/rt64` | detached `20d75f5` | origin/main has 8 commits not in local; local is 6 commits off origin/main history | Mostly renderer/library updates; consider only if renderer blocks gameplay path |
| `lib/RmlUi` | detached `7a06f27d` | origin/master much newer | UI library update not relevant to current boot blocker |
| `lib/lunasvg` | detached `4166d0c` | origin/master much newer | SVG library update not relevant to current boot blocker |
| `../references/cv64_decomp` | `main` at `5307217`, matches origin/main | up to date | Recent upstream includes matched `honmaru_1f.c`, `pause_menu.c`, `honmaru_5f.c`, and common/gameplay textbox refactors; useful for naming and later gameplay/menu comparison |

## Known Safe Patterns

- Prefer compile-time feature flags for experiments.
- Default risky experiments to off.
- Keep each runtime run bounded.
- Always codesign before macOS launch:

```sh
codesign -s - --entitlements .github/macos/entitlements.plist -f build/LodRecomp
```

- Never launch in the background with `&`.
- After each run:

```sh
pgrep -la LodRecomp
```

## Known Unsafe Patterns

- Forced gamestate skip as a permanent fix.
- Auto-input as a permanent fix.
- Registering internal-label functions globally in reusable overlay VRAM ranges.
- Reintroducing dirty submodule-only runtime changes.
- Hard-coding generated recomp overlay IDs for map, NI, or shared-VRAM overlays. Use ROM-range registration instead.
- Reintroducing concatenated `overlay_system` copies/restores as a baseline fix.
- Running N64Recomp directly instead of `./tools/regen_recomp.sh`.
- Long unbounded runs during leak-prone experiments.
- Killing `LodRecomp` just because `pgrep` finds it. A user/manual run may be active; only terminate a process that is proven to belong to the just-launched bounded test, or when explicitly asked.

## Current Next Step

Work item `G6-001`: preserve the now-validated low-resolution natural route into gameplay, then stabilize long-idle gameplay and high-resolution intro/gameplay `gs=3`. The NI TLB skip-window bug and `0x8E`/`0x8F` mirror dispatch miss have concrete local fixes. The post-RDRAM guard fixed the first `rdram+0x80800002` SIGBUS but not the later `rdram+0x80810002` idle crash. `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT` now reaches the idle target around `DL#972` and reproduced the crash quickly; active/current DMAMgr RAM was valid in that crash snapshot. Native crash backtrace support is now installed but still needs a reproducing run. Next iteration should capture a native stack or add downstream PI DMA destination / graphics-scheduler evidence. Do not blindly widen the guard. The manual high-res `0x3880006d0` crash also shows base NI no-match handling is not fully classified. Keep debug-only input/CFB snapshots as test harnesses, avoid gamestate skips as permanent fixes, and keep audio disabled until the CPU/TLB/DMAMgr route is stable.
