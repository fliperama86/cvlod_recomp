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
  - Previous `gs=5` crash after NI pair `104` was caused by stale generated overlay IDs after adding new sections, not by pair-104 game logic itself.

## Goal

Reach actual controllable gameplay through the game's natural state flow, not by permanently skipping screens or forcing gamestates.

## Current Active Work Item

### G6-001 — Drive Title/Start Screen to Controllable Gameplay

Objective:

- Continue from the now-reached title/start screen into actual controllable gameplay.
- Exercise controller input through the game's natural menu flow; do not permanently force gamestates.
- Keep overlay registration range-based and avoid generated overlay-ID assumptions.

Current evidence:

- `gs=4` save/Controller Pak flow is no longer the blocker.
- `gs=12` Expansion Pak screen is no longer the blocker.
- `gs=5`/NI pair-104 crash is no longer the blocker after range-based NI overlay registration.
- A diagnostic scripted-input run reached `gs=6`; the user visually confirmed seeing the start/title screen.
- Manual input can progress through new-game/character-select into the post-book-close transition.
- The current blocker is `gs=3` after `gs=8 → -3 → 3`: map overlay ROM `0x007A60F0` (`map_ovl_09`) is loaded, `overlay_system_2` ROM `0x0075A570` is registered, and NI pair `233` loops while the visible picture is black.

Next action:

1. Add default-off probes for the `gs=3` post-character/book-close path, focused on NI pair `233`, `map_ovl_09` (`ROM 0x007A60F0`), and the `0x8032A490` NI/system state.
2. Extend internal CFB snapshots to capture `gs=3`, `gs=6`, and `gs=8` so the black-screen state can be distinguished from a rendered-black transition.
3. Extend or replace the debug-only input script to reproduce the manual new-game route without forcing gamestates.
4. Identify and name any title/menu/post-selection functions/gamestates encountered on the path to gameplay.
5. When a playable scene is reached, run bounded stability checks: controller input, renderer output, RSS, no stale process.
6. After gameplay is reachable, start burning down old default-on PFS/audio overrides one at a time.

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

## Open Questions

- What does NI pair `233` do in the post-character/book-close `gs=3` path, and why does it loop?
- Is the `gs=3` black screen a legitimate loading/fade wait, a missing render/CFB issue, or a scheduler/object state stall?
- What scene/map does `map_ovl_09` (`ROM 0x007A60F0`) correspond to, and should it lead directly into gameplay?
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

## Current Next Step

Work item `G6-001`: reproduce the manual new-game route with debug-only input, then instrument the `gs=3` post-character/book-close black-screen loop around NI pair `233`, `map_ovl_09`, and internal CFB snapshots. Keep the route bounded and no-skip so the next blocker remains real.
