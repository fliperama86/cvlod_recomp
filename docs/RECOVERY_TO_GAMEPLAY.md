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
- Last pushed baseline before this working checkpoint: `f5ebc66` (`Wire audio RSP diagnostics and list helpers`).
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
  - Caveat: long low-resolution idle in gameplay still crashes with the default 64KB post-RDRAM/KSEG0 guard: `Signal 10` at `0x380810002`, i.e. runtime RDRAM base plus `0x80810002`. A diagnostic 128KB guard fast-idle run survived 180s, suggesting an open-bus/guard-width issue, but the wider guard is not yet accepted as the default fix.
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
- The later `0x380810002` idle crash shows the default 64KB post-RDRAM guard is too narrow for all observed accesses. A diagnostic `LOD_POST_RDRAM_GUARD_SIZE=0x20000` fast-idle run survived 180s, which supports a larger open-bus mirror model, but do not blindly make it the default until the caller/path is better classified.
- Added default-off `LOD_ENABLE_KSEG0_FAULT_TRACE` to capture RDRAM-relative fault offsets plus lightweight context snapshots. The current low-perturbation trace wraps only `DMAMgr_updatePendingFileLoad` (`0x80011E48`); an earlier broader trace also wrapped `func_80043768`.
- A 700s automated boot-input + broader KSEG0-trace run reached the same low-res idle signature (`gs=3`, `exec=0x38000000`, `ni=0x803241C0`, width 320), passed the user's previous DL/VI crash window, and timed out cleanly beyond DL#21000/VI#40000 without a crash. This made the idle KSEG0 crash look nondeterministic or timing-sensitive.
- A later 700s automated boot-input + DMAMgr-only KSEG0-trace run reproduced the idle crash: `Signal 10` at `0x380810002`, with `Fault offset from rdram base: 0x80810002`, decoded as KSEG0 mirror `phys=0x810002` past 8MB RDRAM. The latest snapshot was from `DMAMgr_updatePendingFileLoad`, at `gs=3`, map ROM `0x007BA1D0`, `exec=0x38000000`, `ni=0x803241C0`, repeated `0x0F00C000` NI skip-copy maps, `a0=0x80323C50`, chunk pointer `0x8032EE58`, slot `15`, and a zero-looking entry. The local halfword helper was later confirmed to match generated `MEM_HU` for aligned fields; the remaining concern is that this snapshot may be the last DMAMgr entry before a downstream PI/DMA fault, not the exact faulting instruction.
- A follow-up 700s automated trace with added DMAMgr active/current DMA-field capture timed out cleanly past the previously reproduced crash window (`DL#21100` / `VI#41800`, `gs=3`, `exec=0x38000000`, `ni=0x803241C0`, repeated `0x0F00C000` NI skip-copy maps). The temporary scan also proved inactive `file_info[]` slots can contain garbage/code-like data, so the trace was tightened to report only active/current invalid RAM destinations rather than arbitrary stale slots.
- Added default-off `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT` for KSEG0/idle repros. It aggressively sends controller Start/A through skippable menu/cutscene sections, then releases input once it reaches `gs=3 exec=0x38000000 ni=0x803241C0`.
- Fast script validation reached the idle target at `DL#972`, released input, then reproduced `Signal 10` at `0x380810002` shortly after. The captured DMAMgr fields were valid (`cur_ram=0x803AC448`, `cur_size=0x2CE8`, `ram_ok=1`, `active_ram_ok=1`), so the immediate OOB access is probably not the active/current DMAMgr destination itself. Shift the next trace to downstream PI DMA or graphics/scheduler path evidence.
- Crash handler now prints a short native backtrace on POSIX platforms. Two fast backtrace-enabled attempts reached/released at the idle target and timed out cleanly, so a native stack for `0x380810002` is still pending.
- Added CMake cache variable `LOD_POST_RDRAM_GUARD_SIZE` (default `0x10000`) so the post-RDRAM guard width can be tested without code edits. With `LOD_POST_RDRAM_GUARD_SIZE=0x20000`, `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1`, and `LOD_ENABLE_KSEG0_FAULT_TRACE=1`, the fast route reached/released at the idle target and survived 180s with no crash (`/tmp/lod_fast_guard_0x20000.log`, `exit=124`).
- Extended default-off `LOD_ENABLE_KSEG0_FAULT_TRACE` to snapshot the last `DMA_ROMCopy` (`0x8001A42C`) ROM source, RAM destination, size, RA, decoded physical destination, and destination validity. A default-guard fast trace run built cleanly, reached/released at the idle target, timed out after 180s, and emitted no `romcopy-suspicious` destination logs (`/tmp/lod_fast_default_guard_romcopy_trace.log`, `exit=124`).
- CV64 cross-reference confirms the fault path is the engine `DMAMgr` / `DMAChunkMgr_t`; `0x8001A42C` is the CV64-equivalent `DMA_ROMCopy`, and `0x80011E48` is now named `DMAMgr_updatePendingFileLoad`.
- New manual high-res evidence means the old `0x3880006d0` no-match family is not fully eliminated. The observed paddr words (`27BCFFE8 AFBC0014 8480000C 24600000`) are absent from the ROM/resource set and differ by only a few bits from the common NI overlay prologue (`27BDFFE8 AFBF0014 8483000E 24630001`), which points more toward stale/corrupted TLB buffer contents than a legitimate missing overlay fingerprint.
- Enhanced the default no-match NI/TLB warning to include miss number, even/odd paddr, currently loaded pair/ROM/size for the mapped segment, both `0x0F`/`0x0E` loaded-pair slots, gamestate, exec flags, and NI-system pointer. This should make future high-res `0x3880006d0` crash logs actionable without enabling a noisy trace flag.

Next action:

1. Keep gameplay stability and audio as separate diagnostic tracks. For gameplay, continue to prioritize `gs=3` NI/TLB, high-res intro, and post-RDRAM/KSEG0 evidence; for audio, use only default-off trace builds from A1-001.
2. Use `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1` for KSEG0/idle repros; keep the full natural prologue route only for intro-stability checks.
3. Stop spending cycles on repeated KSEG0 guard A/B loops unless a new crash gives fresh evidence. Good telemetry is now in place: native backtrace, RDRAM-relative fault decoding, DMAMgr snapshot, and last-`DMA_ROMCopy` snapshot under `LOD_ENABLE_KSEG0_FAULT_TRACE`.
4. Keep `LOD_POST_RDRAM_GUARD_SIZE=0x20000` as diagnostic evidence for now. Leave the default at `0x10000`; only revisit guard width when a real crash provides native stack / last-DMA context.
5. Move next to the remaining gameplay/progression blockers, especially the high-res/base-NI no-match `0x3880006d0` family. Default no-match logs now include current loaded NI pair/ROM, paddr, gamestate, exec, and NI pointer; use new crash logs to classify whether pages are code-like/corrupted vs data-like before changing no-match behavior again.
6. Use the user's A/B result as the current repro split: low-res natural intro reaches gameplay; high-res reproduced `0x3880006d0`. Keep high-res as the primary repro mode until contradicted.
7. Repeat bounded scripted routes with CFB snapshots only when visual/scene-output evidence is needed; do not pay full intro cost for every KSEG0 iteration.
8. Use internal CFB snapshots for `gs=3`, `gs=6`, and `gs=8` to verify whether scene output is still valid immediately before any remaining crash.
9. Identify and name the `gs=3` scene/map transition functions encountered after character select, including the map overlays at `0x007A60F0`, `0x007AB090`, `0x007CDDE0`, `0x00835D10`, and the newly observed `0x007B1F60`/`0x007C89A0` path.

### A1-001 — Audio RSP Path Produces Audible Output

Objective:

- Keep audio investigation diagnostic-only and default-off.
- Avoid app-side gain or fake audio hacks; identify the real CPU/RSP handoff issue.
- Preserve the validated gameplay route while testing audio with bounded runs.

Current evidence:

- The SDL/AI handoff is alive: the app queues buffers and SDL drains them.
- The old default `alEnvmixerPull` no-op suppresses synthesis; audio tests must build with `LOD_OVERRIDE_ALENVMIXERPULL=0`.
- The list-helper fix for `0x80096EC0` / `0x80096EF0` is a real compatibility fix: these routines are libaudio doubly-linked-list helpers in this ROM, despite their old misleading OS-thread names.
- CV64 fingerprint matches identified and renamed LoD libaudio functions, including `alSynNew`, `alAudioFrame`, `alEnvmixerNew`, `alEnvmixerPull`, `alEnvmixerParam`, and the `alSyn*Voice*` helpers.
- The `osPiStartDma` rename/fix changed the evidence materially: RSP sample `LOADBUFF` now reads nonzero sample/codebook data instead of zero source buffers.
- The LoD-local `lod_osAiSetNextBuffer_recomp` wrapper normalizes/validates AI buffer pointers before queueing and avoids dirtying `lib/N64ModernRuntime`; this fixed the `rdram+0x80810000` crash seen when LoD passed a low/physical AI pointer.
- Latest 20s trace build (`/tmp/lod_audio_ai_normalized_trace_20260605.log`) had no crash/stale process and produced sustained nonzero queued audio: 449 of 1,043 traced buffers were nonzero, with 657,559 nonzero sample words and peak absolute sample value `19225`.
- Representative late queue buffers are fully populated (`queue_samples#1098 min=-5592 max=4443 nonzero=1440`), so the old "near-silent/sparse output" classification is superseded for this build.
- User manual confirmation: the current audio build produces audible game audio, including music.
- Manual audio run stability caveat: `build-audio-manual/LodRecomp` ran for about 340s, reached/held `gs=3` gameplay, then crashed with `Signal 11` after DL#10300/VI#20200. Last lookups interleaved libaudio callbacks (`0x8009092C`, `alEnvmixerPull`, `0x8009F1EC`, `0x8009FC7C`) with the familiar gameplay/DMAMgr tail (`func_80011D10`, `DMAMgr_updatePendingFileLoad`), so audio is audible but not yet long-run-stability proven.
- CPU pull-chain callbacks are active at high volume (`envmixerPull#140000`, `adpcmPull#42500`, etc. in the latest log), so the chain is executing continuously.
- New CPU voice wrappers installed successfully, but no `AUDIO_VOICE` calls occurred after wrapper installation during the latest 25s run; the active per-frame evidence is the envmixer event stream, not fresh voice-start APIs.
- `alEnvmixerParam id=3` queues or replaces envelope event pointers at envmixer state `+0x3C/+0x40`; event types observed in the latest run are mostly `0xB`, with `0xC`, `0xD`, `0xE`, `0xF`, and `0x10` also present.
- Earlier RSP ENVMIXER collapse examples showed nonzero decoded input entering ENVMIXER, but main/aux output collapsing when the saved/current envelope state was tiny. Keep this evidence for quality/regression analysis, but do not treat early collapse logs as proof of silence now that late queue buffers are strong and nonzero.
- Targeted CPU envmixer-state trace maps RSP ENVMIXER `w1` physical state addresses back to CPU nodes. Example: collapse at `w1=0x00106180` maps to node `0x80105260`; earlier CPU snapshots for that node were already zero/tiny (`st20` all zero, `lr=1/1`, `next=0/0`), but the current throttle did not capture that node immediately before the late collapse.

Next audio step:

1. Classify audible quality issues separately: volume, crackle, missing instruments, channel/order, and timing.
2. Keep the audio fix path clean: no app-side gain/fake audio; investigate only specific audible defects.
3. Consider promoting the minimal required pieces only after stability/quality is acceptable: `alEnvmixerPull` no-op disabled for audio builds, generated audio RSP enabled, list-helper compatibility fix, `osPiStartDma` naming/callsite fix, and LoD-local AI pointer wrapper.
4. Keep audio RSP, pull traces, and raw dumps default-off.

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
| `LOD_OVERRIDE_ALENVMIXERPULL` | on | `alEnvmixerPull` (`func_8009E480`) libultra envmixer hard no-op; suppresses audio command generation | Build/run with `=0` once audio traces are needed; old `LOD_OVERRIDE_FUNC_8009E480=0` alias is still accepted |
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
| `LOD_ENABLE_AUDIO_TRACE` | off | Debug-only app-layer audio telemetry: SDL device open, frequency changes, queued sample min/max/nonzero counts, SDL queued byte count, and AI pointer validation | Use with `LOD_OVERRIDE_ALENVMIXERPULL=0`; latest evidence shows sustained nonzero buffers after the `osPiStartDma` rename and AI pointer wrapper, and the user confirmed audible audio |
| `LOD_ENABLE_AUDIO_RAW_DUMP` | off | Debug-only app-layer raw `s16le` AI queue dump to `/tmp/lod_audio_queue_s16le.raw` | Use only for short local diagnostics, then disable again; convert to WAV for amplitude/RMS checks |
| `LOD_ENABLE_AUDIO_PULL_TRACE` | off | Debug-only CPU libaudio pull-chain wrappers for source/envmixer/resample/ADPCM functions and current CPU envmixer event evidence | Use for targeted `alEnvmixerPull`/command-emission correlation; broad voice wrappers are installed but may not fire after init in short runs |
| `LOD_ENABLE_RSP_AUDIO_TRACE` | off | Debug-only RSP audio task telemetry, alist dumps, and ENVMIXER collapse summaries | Use to correlate RSP `cmd10/saved20/saved40` against CPU envmixer fields; keep default-off |
| `LOD_USE_RECOMPILED_AUDIO_RSP` | off | Selects experimental generated LoD audio RSP microcode instead of the timing-only stub | Required for audible audio in current testing; keep off in baseline until quality/stability and required companion fixes are accepted |
| `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT` | off | Debug-only aggressive controller/PIF input to skip quickly to the gameplay idle signature; releases input at `gs=3 exec=0x38000000 ni=0x803241C0` | Use for KSEG0 idle repro loops instead of paying full prologue cost |
| `LOD_POST_RDRAM_GUARD_SIZE` | `0x10000` | CMake cache variable for the zero-filled post-RDRAM guard at `rdram+0x80800000`; `0x20000` survived one 180s fast-idle diagnostic run | Keep default at `0x10000`; A/B test `0x20000` when validating open-bus/guard-width hypothesis |
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
| G6-001R | `LOD_POST_RDRAM_GUARD_SIZE=0x20000`, `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1`, `LOD_ENABLE_KSEG0_FAULT_TRACE=1`, no CFB/GS3 anim trace | 180s | Fast script reached `gs=3 exec=0x38000000 ni=0x803241C0` at log line 472 and released input. The run then stayed in the same gameplay idle signature through `DL#5500` / `VI#10600` and ended only by bounded timeout (`exit=124` / Signal 15); no `[CRASH]`. | Widening the post-RDRAM guard from 64KB to 128KB covers the observed `0x80810002` fault window in this diagnostic run. This supports an open-bus/guard-width hypothesis, but because the crash is timing-sensitive, keep the wider guard as an A/B test rather than silently changing the baseline. |
| G6-001S | `LOD_POST_RDRAM_GUARD_SIZE=0x10000`, `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1`, `LOD_ENABLE_KSEG0_FAULT_TRACE=1` extended with last-`DMA_ROMCopy` snapshot | 180s | Fast script reached `gs=3 exec=0x38000000 ni=0x803241C0` and released input at log line 472. The run stayed in gameplay idle through `DL#5500` / `VI#10600` and ended only by bounded timeout (`exit=124` / Signal 15). No `[CRASH]` and no `romcopy-suspicious` lines were emitted. | The extra PI/DMA destination trace is low-noise and build-valid, but this pass did not reproduce the default-guard `0x80810002` crash. Keep using the fast harness; if a crash recurs, compare the crash handler's last-`DMA_ROMCopy` fields against DMAMgr active/current fields before deciding on a wider baseline guard. |
| G6-001T | default/off build with enhanced NI no-match warning context | compile-only | Added low-noise context to the always-on first-10 NI no-match warnings: even/odd paddr, loaded pair/ROM/size, both segment loaded-pair slots, gamestate, exec flags, and NI-system pointer. Default/off `LodRecomp` build succeeded. | Future high-res `0x3880006d0` logs should be actionable without repeated guard loops or broad tracing. Use the next real high-res/no-match crash to classify stale/corrupt page behavior. |
| AUDIO-001 | `LOD_OVERRIDE_ALENVMIXERPULL=0` (via old alias `LOD_OVERRIDE_FUNC_8009E480=0`), no auto-input/traces | user manual | User reported no audible audio. | Disabling the audio-chain no-op is not sufficient; add app-layer audio telemetry to determine whether AI buffers are queued, whether samples are nonzero, and whether SDL receives data. |
| AUDIO-002 | `LOD_OVERRIDE_ALENVMIXERPULL=0`, `LOD_ENABLE_AUDIO_TRACE=1`, no auto-input | compile-only/manual-ready | Added default-off audio trace flag and built `build-audio-trace/LodRecomp` for manual testing. | Next manual result should include `[AUDIO]` lines to classify the remaining audio blocker. |
| AUDIO-003 | CV64 fingerprint of `func_8009E480` | source hygiene | LoD `0x8009E480` matched CV64 `alEnvmixerPull` at 320/322 normalized instructions (99.4%, same 0x508 size). | Rename symbols/stub to `alEnvmixerPull`; this explains why the default override mutes audio synthesis rather than only affecting PFS. |
| AUDIO-004 | local audio trace, `LOD_OVERRIDE_ALENVMIXERPULL=0`, `LOD_ENABLE_AUDIO_TRACE=1` | 45s bounded local run | SDL opened and drained queued buffers, but every AI buffer had `min=0 max=0 nonzero=0` from `queue_samples#1` onward. `rsp/aspMain.cpp` is still an audio RSP stub that returns without processing the alist. | Root audio blocker is missing audio RSP microcode execution, not SDL output. Next: dump first audio OSTask ucode/alist and generate/wire an `RSPRecomp` implementation. |
| AUDIO-005 | `LOD_USE_RECOMPILED_AUDIO_RSP=ON`, `LOD_OVERRIDE_ALENVMIXERPULL=0`, `LOD_ENABLE_AUDIO_TRACE=1`, `LOD_ENABLE_RSP_AUDIO_TRACE=1`, `LOD_ENABLE_AUDIO_PULL_TRACE=1` | bounded local runs, 2026-06-05 | Recompiled audio RSP runs and processes real alists, but audio remained effectively silent until the CPU-side voice allocator was fixed. Root cause: `__osDequeueThread_recomp` / `__osEnqueueThread_recomp` had been empty "OS thread" stubs, but LoD's `0x80096EC0` / `0x80096EF0` are libaudio ALLink/PVoice doubly-linked-list helpers. Empty stubs left `alSynNew` with no usable PVoice free list, so `alSynAllocVoice` failed and `alSynStartVoiceParams` returned before dispatching envmixer events. | Keep the list-helper fix in tracked code; this is not an audio hack. These symbols are named like OS thread helpers but must mutate RDRAM list links exactly like the ROM functions. |
| AUDIO-006 | same flags after list-helper fix | `/tmp/lod_audio_list_fix_trace.log`, bounded local run | PVoice allocation and start events now work: `alSynAllocVoice` succeeds, `alSynStartVoiceParams` dispatches, `alEnvmixerParam` receives id `9` start/id `4` stop/id `5` source events, `_pullSubFrame` calls `alResamplePull`/`alAdpcmPull`, and audio alists contain ADPCM/resample/envmixer/mixer/save commands. RSP reads nonzero source/codebook data and writes nonzero ADPCM state. | CPU scheduling is no longer the primary blocker. Remaining blocker moved downstream: final AI/output buffers are still zero or near-silent. |
| AUDIO-007 | CPU volume/event telemetry plus generated RSP trace | `/tmp/lod_audio_vol_trace.log`, `/tmp/lod_audio_sisd_trace.log`, bounded local runs | `alSynSetVol` sends many nonzero volume events after the initial zero-volume starts, and decoded alists contain plausible nonzero `SETVOL`/`MIXER` commands. The app queue eventually sees only near-silent samples (`min=-1 max=0`, ~12–22 nonzero samples in 720-frame buffers). Forcing RSP VU scalar/SISD instead of SIMD/sse2neon produced the same near-silent output. | Near-silence is not explained by SDL queueing, endian/channel swap, CPU volume, or SIMD-vs-SISD. The likely blocker is generated audio-RSP command execution/scale in the envmixer/mixer path, or an RSPRecomp/codegen semantic issue specific to this microcode. |
| AUDIO-008 | alist dumps from generated RSP build | `/tmp/lod_aspMain_alist_task0001.bin`, `task0600`, `task0840`, `task0960`, `task1080` | Representative alists are dumped in N64 byte order. Later tasks show nonzero sound-heavy command streams: ADPCM/resample/envmixer chains feed mixer/interleave/save paths, with `SETVOL` values like `0x09060BD5`, `0x09041034`, `0x09020BD5`, `0x09001034`, and full-scale mixer commands like `0x0C007FFF`. | Next audio step: instrument generated RSP command execution around `SETVOL`, `ENVMIXER`, `MIXER`, `INTERLEAVE`, and final `SAVEBUFF` to identify where normal decoded sample amplitudes collapse to `0`/`±1`; do not paper over this with app-side gain. |
| AUDIO-009 | `LOD_USE_RECOMPILED_AUDIO_RSP=ON`, `LOD_OVERRIDE_ALENVMIXERPULL=0`, audio/RSP/pull traces, temporary AI-buffer comparison, `LOD_ENABLE_AUDIO_RAW_DUMP=1` | bounded local runs, `/tmp/lod_audio_ai_loud.log`, `/tmp/lod_audio_queue.wav` | The RSP/AI/SDL handoff is not totally silent. `osAiSetNextBuffer` and `queue_samples` both see a few high buffers (`queue_samples#870`, `#994`, `#1008`, `#1013`, `#1022`) with peaks up to `-2094..3422`. Whole-run raw capture: 1,642,912 samples (~18.63s at 44.1kHz stereo), only 9,690 nonzero samples (~0.59%), global RMS ~17, top 0.1s RMS ~146. Direct pointer and `MEM_H` views matched, so byte order/channel handoff is not the cause. | Reclassify blocker as sparse/missing synthesized audio, not dead SDL/AI queueing. Do not use app-side gain. Next audio investigation should compare generated RSP command amplitudes against expected ABI behavior and/or CV64/known ABI audio ucode, and check why normal music/ambience voices produce mostly zero output. |
| AUDIO-010 | `LOD_USE_RECOMPILED_AUDIO_RSP=ON`, `LOD_OVERRIDE_ALENVMIXERPULL=0`, audio/RSP/pull traces, fast gameplay input, clean regenerated `RecompiledFuncs/` | bounded local self-test, `/tmp/lod_audio_selftest_clean_20260605.log` | 25s run timed out cleanly with no `[CRASH]`. Audio remained sparse: 1,089 queue trace lines, 13 nonzero queue logs, 11 high-amplitude queue logs; first burst `queue_samples#870 min=-246 max=2190 nonzero=232`, later peak `#994 min=-2094 max=3422 nonzero=128`, final queue line still zero. A useful amplitude-collapse example: `resample.out#11460` / `envmixer.in#11460` had `-588..2313` over 47/160 nonzero samples, but `envmixer.main#11460` collapsed to `-2..8`; downstream `mixer.dst#25920` / `interleave.out#4320` stayed around `-2..1`. | Current hypothesis tightened: the generated RSP is executing and sometimes produces real bursts, but normal voice paths are heavily attenuated/collapsed inside or immediately after ENVMIXER/MIXER. Next: compare ENVMIXER fixed-point volume/ramp math against known ABI/CV64 behavior before touching SDL/app gain. |
| AUDIO-011 | CV64 fingerprint pass for LoD libaudio functions, then `./tools/regen_recomp.sh` | compile/regen | Renamed `alSynSetVol`, `alSynAllocVoice`, `alSynSetFXParam`, `alSynStartVoice`, `alSynSetPitch`, `alSynSetPan`, `alSynNew`, `alAudioFrame`, `alEnvmixerNew`, `alEnvmixerParam`, `alSynSetFXMix`, `alSynGetFXRef`, `alSynAddPlayer`, `alSynStartVoiceParams`, `alSynAllocFx`, and `alSynStopVoice` in `castlevania2.syms.toml`, `symbol_addrs.txt`, and `FUNCTION_MAP.md`. Regen completed through the safe script; tracked generated whitespace noise was reverted. | Symbol hygiene for the audio path is now much better; future traces should use libaudio names instead of `func_8009....` aliases. |
| AUDIO-012 | `LOD_USE_RECOMPILED_AUDIO_RSP=ON`, `LOD_ENABLE_AUDIO_TRACE=ON`, `LOD_ENABLE_RSP_AUDIO_TRACE=ON`, `LOD_ENABLE_AUDIO_PULL_TRACE=ON`, `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=ON`, `LOD_OVERRIDE_ALENVMIXERPULL=0` | 25s bounded local run, `/tmp/lod_audio_voice_state_trace_20260605.log` | Built/codesigned and timed out cleanly (`rc=124`), no stale process. 1,071 queued buffers, but only 11 had nonzero samples: 2,099 nonzero sample words out of 1,574,240 (`~0.133%`), peak abs `3422`. CPU pull-chain callbacks are active; no post-install `AUDIO_VOICE` calls occurred in this run. `envmixerParam` produced 6,723 lines, mostly `id=3` event-pointer updates. RSP ENVMIXER collapse cases correlate with tiny saved/current envelope state (`saved20` as low as `0x0008`/`0x0000`) despite nonzero decoded input. | Next targeted trace should instrument CPU `alEnvmixerPull` / `func_8009EA68` command emission fields, then correlate them with RSP `cmd10/saved20/saved40` to decide whether the collapse is CPU envelope-state scheduling/packing or generated RSP ENVMIXER math. |
| AUDIO-013 | targeted CPU envmixer-state trace added to `LOD_ENABLE_AUDIO_PULL_TRACE`; same audio/RSP build flags as AUDIO-012 | 25s bounded local run, `/tmp/lod_audio_envmixer_cpu_trace_20260605.log` | Built/codesigned and timed out cleanly (`rc=124`), no stale process. Smaller log (~45k lines). One queue buffer had nonzero output (`queue_samples#1348`, peak `2190`). One RSP ENVMIXER collapse occurred: `w1=0x00106180`, input abs `2313`, output abs `8/8`. CPU trace maps `0x00106180` to node `0x80105260`; earlier snapshots for that node were already zero/tiny, but the global throttle did not capture the late pre-collapse state. | The trace now has the right CPU↔RSP mapping. Next refinement is per-state throttling for collapse-prone state buffers, then decide whether the root is CPU envelope scheduling/sequence silence or RSP ENVMIXER math. |
| AUDIO-014 | `osPiStartDma` symbol rename, LoD-local `lod_osAiSetNextBuffer_recomp` wrapper, `LOD_USE_RECOMPILED_AUDIO_RSP=ON`, `LOD_ENABLE_AUDIO_TRACE=ON`, `LOD_ENABLE_RSP_AUDIO_TRACE=ON`, `LOD_ENABLE_AUDIO_PULL_TRACE=ON`, `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=ON`, `LOD_OVERRIDE_ALENVMIXERPULL=0` | 20s bounded local run, `/tmp/lod_audio_ai_normalized_trace_20260605.log` | Built/codesigned and timed out cleanly (`rc=124`), no stale process and no `[CRASH]`. `osPiStartDma` rename fixed sample DMA evidence: first audio DMA/RSP loads now contain nonzero source data. The AI wrapper prevents the low/physical AI-buffer `rdram+0x80810000` crash and leaves submodules clean. Queue telemetry improved from sparse to sustained output: 449/1,043 traced queue buffers were nonzero, 657,559 nonzero sample words total, peak abs `19225`, with late full buffers like `queue_samples#1098 min=-5592 max=4443 nonzero=1440`. Two invalid `osAiSetNextBuffer` calls (`raw=0x80136D40 bytes=0xFFFFED00`) were skipped instead of crashing. | Audio data path is now alive enough for audible/manual validation. If sound is heard, classify quality/timing issues next; if still silent despite nonzero queue buffers, investigate SDL/host output rather than returning first to ENVMIXER math. |
| AUDIO-015 | same current audio build as AUDIO-014 | user manual test | User confirmed audible game audio. | Audio is no longer blocked at SDL/AI/RSP data-path level. Next work is quality/stability triage and deciding which minimal audio pieces can graduate from experimental/default-off to accepted baseline. |
| AUDIO-016 | `build-audio-manual/LodRecomp`; `LOD_USE_RECOMPILED_AUDIO_RSP=ON`, `LOD_OVERRIDE_ALENVMIXERPULL=0`, no audio traces, no fast input | user manual, ~340s | User confirmed audible game audio including music, then the run crashed with `exit=139` / `Signal 11` around `gs=3`, `DL#10300`, `VI#20200`. Native backtrace was an ObjC autorelease-pool/thread-exit path and printed twice; last recomp lookups repeatedly included `0x8009092C`, `alEnvmixerPull`, `0x8009F1EC`, `0x8009FC7C`, then `0x80000578`, `0x8001BB3C`, `func_80011D10`, `DMAMgr_updatePendingFileLoad`. | Audio output is real, but the manual audio build still needs long-run crash classification. Next useful run should preserve the manual route and add targeted low-noise crash context rather than broad audio traces. |

## Open Questions

- What exact signal/address/function context occurs when the intro sequence is allowed to play naturally?
- Does the reported natural intro `Signal 15` match the repeated late `gs=3` `Signal 10`/`0x380800002` crash, or are they separate blockers?
- Does high-resolution mode uniquely reproduce the `0x3880006d0` no-match crash, or can long low-resolution sessions still hit it later?
- Is the NI TLB skip range supposed to cover the full 1MB reusable `0x0E`/`0x0F` segment rather than only the currently loaded overlay span?
- Are `0x8E...`/`0x8F...` dispatch targets always safe to canonicalize/register as aliases for `0x0E...`/`0x0F...` NI overlays?
- For no-match base NI TLB maps, how should code-like but non-fingerprinted/corrupted pages be handled? Normal-copying while preserving old registrations helped one scripted route, but the manual high-res crash shows stale registered functions plus mismatched bytes can still be unsafe.
- What exact caller/source produces the `0x80800002` and `0x80810002` KSEG0 accesses? Are these legitimate open-bus/end-of-RDRAM probes or upstream object/resource pointer bugs? The captured DMAMgr-only trace points near `DMAMgr_updatePendingFileLoad`, active/current DMA RAM was valid, a 128KB guard survived one fast-idle run, and the first default-guard `DMA_ROMCopy` trace pass produced no suspicious destinations, but native-stack evidence is still needed before making the wider guard the default.
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

Work item `G6-001`: preserve the now-validated low-resolution natural route into gameplay, then stabilize long-idle gameplay and high-resolution intro/gameplay `gs=3`. The NI TLB skip-window bug and `0x8E`/`0x8F` mirror dispatch miss have concrete local fixes. The post-RDRAM guard fixed the first `rdram+0x80800002` SIGBUS but the default 64KB guard did not cover the later `rdram+0x80810002` idle crash. `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT` now reaches the idle target around `DL#972`; active/current DMAMgr RAM was valid in the reproduced crash snapshot. Telemetry is now good enough to move on: native backtrace, RDRAM-relative fault decoding, DMAMgr snapshot, last-`DMA_ROMCopy` snapshot, and enhanced NI no-match context are available for future crashes. Do not keep looping on the guard unless a new crash provides fresh evidence.

Parallel audio work is now a real milestone: generated audio RSP is wired, the libaudio PVoice free-list bug was traced to empty `__osDequeueThread`/`__osEnqueueThread` stubs that must behave as ALLink list helpers, `osPiStartDma` naming fixed sample DMA evidence, and the LoD-local AI pointer wrapper prevents the physical-buffer queue crash. The user confirmed audible game audio, including music, from `build-audio-manual/LodRecomp`. Audio is no longer blocked at SDL/AI/RSP data-path level; remaining audio work is quality/stability triage and deciding what minimal pieces can graduate from experimental flags. The latest manual audio run still crashed after about 340s in `gs=3` with `Signal 11`, so next focus is real progression: high-res/base-NI no-match `0x3880006d0` classification, longer gameplay/manual crash capture, and targeted low-noise crash context for the manual audio build. Use fast-skip for KSEG0 idle loops; reserve full natural prologue runs for intro-stability checks. Keep debug-only input/CFB/audio snapshots as test harnesses and avoid gamestate skips or app-side audio gain as permanent fixes.
