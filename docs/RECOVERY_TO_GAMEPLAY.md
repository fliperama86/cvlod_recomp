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
  - RT64 graphics safety guards for invalid/unknown mid-display-list ucode loads and stale NI `G_DL` targets are default-on;
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
  - User visual confirmation: the later boss/animated-sequence route that previously produced black screen with audio now renders after the RT64 NI display-list safety baseline. The accepted fix is general: stale `0x8E`/`0x8F` NI display-list targets are checked against the currently loaded overlay span before RT64 dereferences or parses them.
  - Caveat: long low-resolution idle in gameplay still crashes with the default 64KB post-RDRAM/KSEG0 guard: `Signal 10` at `0x380810002`, i.e. runtime RDRAM base plus `0x80810002`. A diagnostic 128KB guard fast-idle run survived 180s, suggesting an open-bus/guard-width issue, but the wider guard is not yet accepted as the default fix.
  - Caveat: native Expansion Pak high-resolution mode is intentionally skipped in default builds (`LOD_SKIP_EXPANSION_PAK_SCREEN=ON`, `LOD_ENABLE_NATIVE_HIGH_RES=OFF`). Reporting 4MB alone did not hide the selector, and soft controller auto-advance still followed an unstable path. The default baseline now hard-skips only `gs=12` by requesting the natural post-selector transition (`gs=-5`, which creates `gs=5`), not directly to title. High-res previously exposed both presentation issues and a no-match NI/TLB crash during the intro/book animation path; keep it as an explicit investigation path rather than normal tester baseline.
  - Earlier user idle test in actual gameplay also hit the same late-crash family seen locally: `Signal 10` at `0x380800002`, after many `gs=3` frames and repeated last lookups through `func_80011E48`.
  - User high-resolution manual run hit a different-looking instance of the older no-match NI TLB crash family: `Signal 10` at `0x3880006d0` after a base `0x0F000000` map from paddr `0x0039B000` whose first words were a close but non-matching/corrupted NI prologue. Treat display-resolution choice as a repro variable until low-res/high-res A/B confirms otherwise.
  - Previous `gs=5` crash after NI pair `104` was caused by stale generated overlay IDs after adding new sections, not by pair-104 game logic itself.

## Goal

Reach actual controllable gameplay through the game's natural state flow, not by permanently skipping screens or forcing gamestates.

## Current Active Work Item

### G6-001 — Gameplay Reached; Stabilize Intro/Cutscene Route

Objective:

- Preserve the now-working route from title/start/new game into actual controllable gameplay.
- Stabilize long gameplay idle; native high-resolution remains disabled by default until its presentation and high-res-only crash evidence are understood.
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
- Latest graphics recovery: the boss/animated-sequence black-screen route reached RT64 HLE with stale NI display-list targets and repeated unknown mid-DL `LOAD_UCODE` misses. The promoted baseline keeps RT64 from parsing those stale targets by (a) ending invalid `LOAD_UCODE` commands that lack `RDPHALF_1`, (b) preserving the active GBI on unknown mid-DL ucode loads, (c) validating `G_DL` targets against the active GBI opcode map, and (d) validating `0x8E`/`0x8F` NI targets against the currently loaded overlay span before dereference. This is not sequence-specific.

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
10. Treat the RT64 NI display-list black-screen/crash class as resolved for the current route. If it regresses, re-enable `LOD_ENABLE_RENDER_ADDR_TRACE`/`LOD_ENABLE_GBI_MISS_TRACE` and check for stale NI target skips or new unknown `LOAD_UCODE` signatures before adding narrower hacks.

### A1-001 — Audio RSP Path Produces Audible Output

Objective:

- Keep debug audio traces diagnostic-only and default-off.
- Avoid app-side gain or fake audio hacks; preserve the real CPU/RSP handoff.
- Keep the audible audio configuration in the normal baseline while continuing bounded stability/quality tests.

Current evidence:

- The SDL/AI handoff is alive: the app queues buffers and SDL drains them.
- The previous default `alEnvmixerPull` no-op suppressed synthesis; the baseline now builds with `LOD_OVERRIDE_ALENVMIXERPULL=OFF` so the real pull path is active.
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
- Windows issue #7 crackle evidence: reporter v0.2.5 log used SDL `directsound` with a 44.1 kHz/8-channel default device while LoD requested 48 kHz/F32/stereo; the Windows baseline now forces SDL `wasapi` before audio init. SUPERNOVA smoke build/run logged `SDL audio driver: wasapi`, 48 kHz stereo default/output, and no stale process; still needs reporter confirmation that crackle is fixed.

Next audio step:

1. Classify audible quality issues separately: volume, crackle, missing instruments, channel/order, and timing.
2. Keep the audio fix path clean: no app-side gain/fake audio; investigate only specific audible defects.
3. Keep the promoted audio baseline minimal: generated audio RSP enabled, `alEnvmixerPull` no-op disabled, list-helper compatibility fix, `osPiStartDma` naming/callsite fix, and the LoD-local AI pointer wrapper.
4. Keep pull traces, RSP traces, and raw dumps default-off.

## Visual Effects / Small Texture Fix (2026-06-06)

Status: validated by user manual test; promoted to default-on compatibility shim.

What changed:

- Added `LOD_FIX_SEGMENT_86_BASES` for RT64, default ON.
- When RT64 receives a display-list segment base in the invalid `0x86xxxxxx` range, normalize it to `0x80xxxxxx` before storing it in the RSP segment table.
- Removed the one-off render/asset-bank trace-only edits from the committed baseline; the accepted fix is the small segment-base normalization only.

Evidence:

- Missing visuals affected small HUD/effect/item assets: clock/item HUD elements, red jewels/save crystals/pickups, prologue fire/Dracula-style effects, and similar small textured elements, while most world geometry/HUD rendered correctly.
- Trace build showed bad texture/TLUT loads came from segments `5` and `9`, not segment `6`: e.g. `segaddr=0x05000000` with base `0x861A0B20`, and `segaddr=0x09000400` resolving to `0x861A0F20`.
- RT64 then masked those to empty `rdram+0x061A....`; no PI DMA populated that region.
- With the normalization experiment enabled, the same loads became `0x801A.... -> rdram+0x001A....` and contained nonzero texture/TLUT data. The log showed `0` remaining `0x061A....` loads and no crash.
- User confirmed visually that the missing elements rendered after the fix.

Regression checks:

- Start gameplay and verify HUD item/clock elements plus small world pickups/save crystals render.
- Verify logs do not show repeated zero texture loads from `0x061A....` when render tracing is enabled.
- Keep the fix scoped to segment-table bases only; do not globally rewrite arbitrary `0x86....` texture addresses unless future evidence requires it.

## Pause Item Menu Rendering Fix (2026-06-06)

Status: locally validated with automated controller input and built-in CFB snapshots; ready for user manual verification.

What changed:

- Added `LOD_FIX_NI_PRESERVE_SAME_PAIR_DATA`, default ON, so repeated TLB maps for the already-loaded NI pair do not re-copy pristine overlay bytes over mutable segment data.
- Added `LOD_FIX_NI_PAIR99_ITEM_LIST_PERSIST`, default ON, to preserve pair 99's pause-item list table at `0x0F004EA0` while the menu alternates between pair 99 and pair 101.
- The fix is scoped to the item-list scratch/table range (`0x4EA0..0x4F6B`) and only saves data that looks like a live item list.

Evidence:

- Save slot 1 contains at least one menu item (`item_id=7`, amount `1`), but before the fix the item-list table was built as `7:1` and then overwritten back to zero during pair99/pair101 NI swaps.
- With the fix, `LOD_ENABLE_PAUSE_ITEM_TRACE=ON` showed the list remain `7:1` through the same swap window and row text creation used item text index `6` (`Roast chicken`) instead of falling back to zero/blank data.
- A trace-off validation run with `LOD_ENABLE_BOOT_INPUT_SCRIPT=ON`, `LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT=ON`, and `LOD_ENABLE_CFB_SNAPSHOT=ON` exited cleanly by timeout, produced no crash/RSP failure, and `/tmp/lod_cfb_gs3_us2487_dl1439_cfb225800.png` showed `Roast chicken x01` rendered in the item list.

Regression checks:

- Use the automated menu script on save slot 1 and verify the item list is not blank in the CFB snapshot around `gs=3`, `dl≈1439`.
- Manually verify pause → Item after loading a save with at least one item.
- Keep broader pause/menu traces default-off; the compatibility shims should work without `LOD_ENABLE_PAUSE_ITEM_TRACE`.

## Controls Config + Map76 Boss Trace/Fix (2026-06-06)

Status: build-validated locally; awaiting user gameplay validation.

What changed:

- Added persistent `controls.json` next to `graphics.json`.
- The generated default matches the current requested modern gamepad layout: A jump, X attack 1, Y attack 2, B interact/collect, R1 throw item, L3 lock-on, L2 Z, R2 L, inverted right-stick camera/D-pad.
- Control remaps are startup-loaded; edit `controls.json` while the game is closed, then relaunch.
- Added narrow traces for suspected missing enemy/boss evidence: `[NI99_MAP76]` and `[B2_ASSET]` around map overlay ROM `0x0076CD00` and suspected asset/file path (`file=0xB2`, ROM `0x00C3FE4E`, RAM `0x802A3B70`, size `0xF3B2`). These replaced the failed LLDB conditional-breakpoint approach, which was too intrusive and caused game/audio stalls.
- After user/open-emulator save-state comparison, the missing werewolf was classified as a missing NI44 indirect function boundary rather than a missing B2 asset DMA.

Evidence:

- User reported a missing boss/enemy around a log window containing `KSEG0-DMAMGR suspicious` entries followed by `MAP_OVL rom=0x76CD00`.
- Local `lod.log` showed the same transition followed by many repeated `ni_ovl_099` loads (`rom=0x01335560 data=0x4FB0`).
- LLDB caught the suspected B2 load path once (`DMA_ROMCopy -> DMAMgr_updatePendingFileLoad -> func_80011D10 -> object_execute -> GameStateMgr_execute`), but LLDB condition evaluation on `DMA_ROMCopy` was slow enough to stop the game and overload CoreAudio. The first source-level B2 run showed the B2 file queued and current before `map76-loaded`, but no ROMCopy wrapper hit; this confirmed that the final copy must be traced at the PI layer.
- The DMAMgr suspicious line alone may be stale/current-slot garbage because the active entry becomes valid on the following call; the repeated NI99/map76 pattern and B2 asset load are more actionable.
- Short smoke launch created `~/Library/Application Support/LodRecomp/controls.json`, loaded it successfully, opened the DualSense controller, initialized the hidden UI hook, and reached ROM startup with no crash/RSP failure.

Next checks:

- Reproduce the missing enemy/boss with the new build and collect:
  `grep -E '\[B2_ASSET\]|\[NI99_MAP76\]|\[KSEG0-DMAMGR\]|\[MAP_OVL\]|\[ni_ovl\]|\[CRASH\]' lod.log`
- If `[B2_ASSET] pi_dma` appears, compare `before_hash`/`after_hash`, target `dram`, and whether `[B2_ASSET] map76-loaded` immediately follows. A valid post-copy hash but missing actor points downstream at object/model registration; a missing/invalid PI copy points at DMAMgr file-table, ROM source handling, or asset destination handling.
- If `[NI99_MAP76]` repeats while the actor is missing, compare `prev0f`, paddr, gamestate, exec flags, and NI-system pointer to determine whether pair 99 is thrashing, being replaced between frames, or just serving a legitimate object overlay.
- The trace is now default-off. Re-enable only if Map76 boss/entity loading regresses.

Root cause and fix:

- Symptom: the Fog Lake / Map76 werewolf boss appeared briefly, then vanished while boss music/life-bar state could remain inconsistent.
- Root cause: NI overlay pair 44 dispatch tables referenced `0x0F004030`, but `castlevania2.syms.toml` did not declare that address as a function boundary. N64Recomp kept those bytes as unreachable/tail code inside `ni_ovl_044_func_0F003FCC`, so the target was not callable via generated overlay lookup.
- Confirmed missing-helper behavior: `*(s16*)(obj->data[0] + 0x132) |= 1`, i.e. `data0+0xDC+0x56` bit 0, required by the boss state-machine handoff.
- Fixed by splitting `ni_ovl_044_func_0F003FCC` from size `0x78` to `0x64` and adding `ni_ovl_044_set_data0_sub56_bit0` at `0x0F004030`, size `0x14`, in both `castlevania2.syms.toml` and `symbol_addrs.txt`.
- Regenerated with `./tools/regen_recomp.sh`; verified `RecompiledFuncs/funcs_166.c`, `funcs.h`, and `recomp_overlays.inl` now emit/register the target. The temporary runtime shim was removed.
- Robustness: added `tools/audit_indirect_targets.py` to scan NI overlay raw blobs for in-section pointer-table targets that are not declared function starts. The default low-noise executable-range audit now runs at the end of `./tools/regen_recomp.sh`; use `--scan-full-raw --section N --target 0x...` to triage data/rodata pointer-table targets like the pair-7 internal-label crashes. Current default result: 245 NI raw blobs scanned, no missing indirect targets.

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
| `LOD_OVERRIDE_ALENVMIXERPULL` | **off** | `alEnvmixerPull` (`func_8009E480`) no-op override is retained only for A/B diagnostics; enabling it suppresses audio command generation and mutes synthesis | Keep off for normal/audio builds; old `LOD_OVERRIDE_FUNC_8009E480=0` source-level alias remains accepted, but the CMake option is preferred |
| `LOD_OVERRIDE_FUNC_8001D398` | on | Runtime necessity unknown; build-gate combo with inserted-status disabled is fixed | Build/run with `=0` |
| `LOD_OVERRIDE_FUNC_8001C93C` | on | Unknown; Pak validation gate | Build/run with `=0` |
| `LOD_OVERRIDE_FUNC_8009F400` | **off** | Retired stale direct PFS find shim; keep only for A/B regression checks | Confirm no future path depends on it; eventually remove |
| `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS` | on | Compile combo passed with this disabled while `FUNC_8001D398` stayed enabled | Build/run with `=0` |
| `LOD_FIX_NI_PAIR120_RESULT_LABELS` | **on** | Validated scoped compatibility fix for pair-120 internal-label callback table | Keep on; use `=0` only for A/B regression |
| `LOD_FIX_NI_PRESERVE_SAME_PAIR_DATA` | **on** | Repeated TLB maps for an already-resident NI pair must not re-copy pristine ROM bytes over mutable segment data | Keep on; use `=0` only to reproduce disappearing pause-item list state |
| `LOD_FIX_NI_PAIR99_ITEM_LIST_PERSIST` | **on** | Scoped preservation for pair 99 pause-item list table while pair99/pair101 alternate during item-menu rendering | Keep on; use `=0` only for item-menu A/B regression |
| `LOD_FIX_INVALID_GBI_LOAD_UCODE` / `LOD_FIX_INVALID_GBI_LOAD_UCODE_END_DL` | **on** | RT64 safety baseline: invalid mid-display-list ucode loads without `RDPHALF_1` are treated as malformed display-list data and end the current list instead of clobbering HLE state | Keep on; use `=0` only to reproduce old black-screen/crash paths |
| `LOD_FIX_PRESERVE_GBI_ON_LOAD_MISS` | **on** | RT64 safety baseline: unknown mid-display-list ucode loads preserve the active GBI rather than making the HLE interpreter null | Keep on; investigate any new unknown ucode hashes before expanding the GBI database |
| `LOD_FIX_RUN_DL_USE_GBI_MAP` | **on** | RT64 safety baseline: `G_DL` targets are validated against the active GBI opcode map instead of a broad numeric opcode range that accepts garbage/MIPS data | Keep on; use `=0` only for targeted A/B regression |
| `LOD_FIX_RUN_DL_NI_BOUNDS` | **on** | RT64 safety baseline: `0x8E`/`0x8F` NI display-list targets must fall inside the currently loaded NI overlay span before RT64 dereferences them | Keep on; `LOD_ENABLE_RENDER_ADDR_TRACE=1` enables throttled skip logs |
| `LOD_ENABLE_GBI_MISS_TRACE` / `LOD_ENABLE_RUN_DL_SUSPICIOUS_TRACE` | off | Debug-only RT64 HLE diagnostics for unknown ucode hashes, preserve events, and suspicious NI `G_DL` parent dumps | Enable only for fresh black-screen/HLE regressions; keep off for normal gameplay |
| `LOD_FIX_NI_SEG6_CPU_ALIAS` | off | Rejected A/B experiment that mirrors active NI segment data into CPU `0x06xxxxxx`; did not solve the current RT64 black-screen path | Keep off unless a future CPU-side segment-6 pointer crash proves this exact alias is needed |
| `LOD_FIX_NI_TLB_SKIP_ACTUAL_SPAN` | off | A/B experiment to skip normal NI TLB copies only across the actual overlay span instead of preserving the legacy 64KB minimum; current accepted baseline keeps the legacy minimum | Keep off; use only to reproduce/diagnose stale bytes beyond small NI overlays |
| `LOD_ENABLE_TLB_SEG6_TRACE` / `LOD_ENABLE_FIGURE_SEG_TRACE` | off | Debug-only traces for segment-6/figure pointer investigations from the RT64 black-screen work | Enable only for targeted segmented-model pointer regressions |
| `LOD_ENABLE_SAVE_PAIR120_TRACE` | off | Debug-only trace for pair-120 dispatcher/table callback details | Enable only when investigating pair-120 regressions |
| `LOD_ENABLE_SAVE_AUTO_INPUT` | off | Debug-only scripted input for `gs=4` save/Pak path | Replace or generalize for `gs=12` scripted input tests |
| `LOD_ENABLE_SAVE_HANDLER_TRACE` | off | Debug-only function-map wrapper probe | Use only for targeted handler-entry confirmation |
| `LOD_ENABLE_SAVE_EXIT_TRACE` | off | Debug-only wrapper probe for main-code save/PFS exit candidates and schedule advances | Use only if `gs=4` regresses |
| `LOD_ENABLE_SAVE_STATE45_TRACE` | off | Debug-only state-4/5 schedule-helper trace | Use only if `gs=4` regresses |
| `LOD_ENABLE_KSEG0_FAULT_TRACE` | off | Debug-only lightweight crash-context snapshot for post-RDRAM/KSEG0 faults; current low-perturbation form wraps only `DMAMgr_updatePendingFileLoad` and records active/current DMA RAM validity | Use with faster skip-to-gameplay harness; add PI DMA destination evidence if active/current fields stay valid before crash |
| `LOD_ENABLE_B2_ASSET_TRACE` | off | Historical diagnostic trace for the Map76 missing boss investigation; root cause was missing NI44 indirect target symbol `0x0F004030`, not a missing B2 asset DMA | Enable only if Map76 boss/entity loading regresses |
| `LOD_ENABLE_AUDIO_TRACE` | off | Debug-only app-layer audio telemetry: SDL device open, frequency changes, queued sample min/max/nonzero counts, SDL queued byte count, and AI pointer validation | Use for targeted quality/stability diagnostics; latest evidence shows sustained nonzero buffers after the `osPiStartDma` rename and AI pointer wrapper, and the user confirmed audible audio |
| `LOD_ENABLE_AUDIO_RAW_DUMP` | off | Debug-only app-layer raw `s16le` AI queue dump to `/tmp/lod_audio_queue_s16le.raw` | Use only for short local diagnostics, then disable again; convert to WAV for amplitude/RMS checks |
| `LOD_ENABLE_AUDIO_PULL_TRACE` | off | Debug-only CPU libaudio pull-chain wrappers for source/envmixer/resample/ADPCM functions and current CPU envmixer event evidence | Use for targeted `alEnvmixerPull`/command-emission correlation; broad voice wrappers are installed but may not fire after init in short runs |
| `LOD_ENABLE_RSP_AUDIO_TRACE` | off | Debug-only RSP audio task telemetry, alist dumps, and ENVMIXER collapse summaries | Use to correlate RSP `cmd10/saved20/saved40` against CPU envmixer fields; keep default-off |
| `LOD_USE_RECOMPILED_AUDIO_RSP` | **on** | Selects generated LoD audio RSP microcode instead of the timing-only stub | Required for audible audio in current testing; turn off only for A/B diagnostics where silence is expected |
| `LOD_ENABLE_ENEMY_TRACE` | off | Debug-only enemy-list/damage-state trace for unkillable or weird enemy behavior; generated hooks log helper calls around the list at `0x800F9120` | Enable only for targeted manual routes such as the current unkillable knight repro or Stage 6 Art Tower knights |
| `LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT` | off | Debug-only aggressive controller/PIF input to skip quickly to the gameplay idle signature; releases input at `gs=3 exec=0x38000000 ni=0x803241C0` | Use for KSEG0 idle repro loops instead of paying full prologue cost |
| `LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT` | off | Debug-only controller/PIF input to load a save, open pause, and select Item for menu-rendering repros | Use with CFB snapshots for pause/item menu regressions |
| `LOD_ENABLE_PAUSE_ITEM_TRACE` | off | Debug-only pause/item NI overlay and text-object tracing | Enable only when item-menu state or text creation regresses |
| `LOD_ENABLE_CFB_SNAPSHOT` | off | Debug-only internal VI color framebuffer PPM snapshots | Use for visual regressions where OS screenshots are unreliable |
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
| ITEMMENU-001 | `LOD_ENABLE_PAUSE_ITEM_TRACE=ON`, boot/menu scripts, CFB snapshots, save slot 1 | 150s | Pair99 item list built as `7:1`, survived pair99/pair101 swaps after preservation fix, and row text creation used `Roast chicken`; no crash | Disappearing item rows were caused by mutable pair99 segment data being overwritten by NI overlay re-copy/swap behavior |
| ITEMMENU-002 | trace off, boot/menu scripts, CFB snapshots, save slot 1 | 150s | Clean `lod.log` aside from automation markers; no crash/RSP failure; CFB snapshot showed `Roast chicken x01` in the pause Item list | Fix is not dependent on pause-item trace wrappers |
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
| G6-001U | static investigation of `get_function(0x4E6F206D)` after pair-23 route | regen/build + 20s smoke | Crash target `0x4E6F206D` is not random corruption: in NI pair 23, table `0x0F006FC8` has two valid entries (`0x0F004744`, internal label `0x0F004A24`) and index `2` lands on adjacent `"No memory..."` string data. Added a scoped `ni_ovl_023_bike_skeleton_spawn_dispatch` compatibility shim that handles the internal label directly and skips invalid state targets instead of calling string data. `./tools/regen_recomp.sh` succeeded, default macOS build succeeded, and a bounded 20s launch exited only via timeout/Signal 15 with no stale process. | Same pattern as pair 120: internal-label dispatch must be fixed inside the owning dispatcher, not by globally registering labels in reused NI VRAM. Original route should still be manually rechecked to confirm the pair-23 crash no longer reproduces. |
| G6-001V | pair-23 shim canonical GPR/callee-save repair | user manual route | After G6-001U, the original `0x4E6F206D` lookup was gone but manual route produced a new SIGSEGV immediately after `0x0F00469C`; native/source mapping put it at pair-23 root update `0x0F000F9C` returning from table `0x0F006F80`. The fault address indicated a zero-extended KSEG0 object pointer (`0x80xxxxxx`) had been returned in a callee-saved register. Added reusable NI helpers to canonicalize 32-bit MIPS GPR/pointer values and preserve callee-saved registers around the handwritten pair-23 shim. User repeated the specific area/enemy route and reported no crash. | General rule: handwritten/native NI shims must preserve MIPS ABI callee-saved regs and return sign-extended `gpr` pointers. Future internal-label fixes should use the same helper pattern; broader generalization can move the helper to a shared header if non-NI shims hit the same class. |
| G6-001W | RT64 NI display-list safety baseline promoted | manual boss/animated-sequence route | The route that previously crashed or played audio over a black screen reached the sequence successfully after enabling the RT64 safety set: invalid `LOAD_UCODE` end-DL, preserve active GBI on unknown mid-DL ucode loads, active-GBI opcode-map `G_DL` validation, and NI `0x8E`/`0x8F` target span validation. Logs showed stale NI targets such as `0x0F00E7D0`, `0x0F00E7F0`, `0x0F013C88`, and `0x0F013D08` being skipped because the current overlay span did not contain them. | Promote the safety set to default-on. The fix is general for stale NI display-list target parsing, not specific to one sequence. Keep the skip logs behind diagnostics so normal runs do not spam. |
| G6-001X | pair-7 internal-label function-boundary splits | regen + build + user manual route | User's latest save route first crashed by recursively re-entering `ni_ovl_007_func_0F001444` when table `0x0F01A0F8` targeted internal label `0x0F00162C`; splitting that boundary moved the crash to the same pattern in `ni_ovl_007_func_0F010084` via table `0x0F01A5C0` target `0x0F010218`. Added both symbol splits, regenerated with `./tools/regen_recomp.sh`, repaired a truncation-fixer edge case for prototypes missing only `;`, rebuilt `LodRecomp` successfully, and the user confirmed the route worked. | Specific pair-7 crash is validated fixed. The broader pattern is raw NI pointer tables targeting labels inside generated function ranges; before pushing, generalize the indirect-target audit and/or `get_function` range-fallback policy so exact in-range dispatch targets are caught before runtime instead of silently calling the containing function. |
| G6-001Y | generalized NI indirect-target audit workflow | tool validation + regen dry-run | Extended `tools/audit_indirect_targets.py` with `--scan-full-raw` for data/rodata pointer tables, `--target` filtering for crash-address triage, and site-region labels. Added the default low-noise `--fail-on-missing` audit to `./tools/regen_recomp.sh`. Validation: default audit passed across 245 NI executable ranges; full-raw focused audit for fixed pair-7 targets `0x0F00162C` and `0x0F010218` passed; pair-7 full-raw mode now reports 12 remaining data-table internal-label candidates for future triage instead of requiring ad hoc scripts. | This is the safe generalization before push: runtime behavior is unchanged, broad internal-label splitting is avoided, and future regen now fails on low-noise missing targets while full-raw mode gives a repeatable workflow for the noisier internal-label table class. |
| INPUT-001 | default input path plus `build-audio-manual` rebuild | compile-only/manual feedback | Added SDL GameController support without removing keyboard input: first detected Xbox/DualSense/DualShock-style controller maps left stick to N64 analog, D-pad to D-pad, Start to Start, shoulders to L/R, triggers to Z, and right stick to C-buttons. After user feedback that movement worked but attack/jump did not, face buttons were aliased as A/Cross or Y/Triangle → N64 A/jump and B/Circle or X/Square → N64 B/attack. User trace then proved raw PIF A/B (`0x8000`/`0x4000`) reach gameplay and work in menus, so mapping is no longer the primary suspect. | Investigate gameplay control-update/remap path. Corrected `LOD_ENABLE_INPUT_TRACE` to sample `sys.controllers[0]` at `0x801C87F4` plus `playerControllerData` at `0x800F35F0`, so the next manual run can show whether A/B survive into player action state. |
| INPUT-002 | `LOD_ENABLE_INPUT_TRACE=1` manual controller trace | user manual/compile-ready | User trace showed A/B raw PIF buttons reach `playerControllerData` (`held`/`pressed` at `0x800F35F0`) during gameplay, so SDL mapping and `updatePlayerControls` are not the blocker. Added debug-only action-candidate wrappers for indirect callbacks `0x8002D6E8`, `0x80031470`, and `0x800332C4`, which read `playerControllerData + 0x8` and branch on A. | Next manual run should paste `[INPUT_ACTION]` lines while pressing A/B. If no wrappers fire, identify the active player callback table; if they fire but no state changes, inspect the logged object/actor gates. |
| INPUT-003 | controller-init symbol repair plus `build-audio-manual` rebuild | compile-only/manual-ready | Renamed `0x80098600` from `osContStartReadData` to `osContInit` and `0x800994C0` from `osContInit` to `osInitialize`, then regenerated via `./tools/regen_recomp.sh`. Verified generated call sites: `game_main` now calls `osInitialize_recomp`, while PADMGR `contpak_init` now calls `osContInit_recomp`. Also report controller 0 as a `ControllerPak` in runtime device info so native `osContInit` sets the pak/status bit consistently with the virtual pak path. Rebuilt/codesigned `build-audio-manual/LodRecomp`. | Manual test should confirm `sys@1C87F4.conn` and `player@0F35F0.conn` are nonzero during gameplay. If jump/attack still fail with `conn=1` and A/B held, trace pair 24 action function `0x0F009338`; if `conn` remains zero, instrument `contpak_init`/`func_8001EFA0`. |
| ENEMY-001 | `LOD_ENABLE_ENEMY_TRACE=ON`, generated enemy-list helper hooks | compile-only/manual-ready, `build-enemy-trace/LodRecomp` | Added default-off tracing for core enemy-list helpers around `0x80045668..0x8004594C`. Logs include gamestate, map overlay ROM/load count, controller attack buttons, list entry index/flags/aux words, and actor state before/after helper calls. | Manual repro should attack the current unkillable knight target and/or Stage 6 Art Tower knights, then inspect `[ENEMY_TRACE]`/`[ENEMY_ACTOR_TRACE]` around the attacks to see whether damage/dying flags or actor functions advance. |
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
| AUDIO-017 | default baseline cleanup: `LOD_USE_RECOMPILED_AUDIO_RSP=ON`, `LOD_OVERRIDE_ALENVMIXERPULL=OFF`, no audio traces | local build/user manual confirmation | Added a real CMake option for `LOD_OVERRIDE_ALENVMIXERPULL`, removed the need for manual `CMAKE_CXX_FLAGS`, restored generated audio RSP as the default path, and documented the audio baseline in README/recovery notes. User confirmed the preceding equivalent build restored audible audio. | Audio should remain on in normal tester builds; future audio regressions should first verify these two flags before adding traces. |

## Rendering / RT64 Glitch Investigation

| ID | Area | Validation | Findings | Next Action |
|----|------|------------|----------|-------------|
| RENDER-001 | intro/prologue/merchant geometry explosions, black models, widescreen edge popping | default build and diagnostic A/B build compiled on `diag/render-glitch-root-causes`; user manual all-toggle and no-forceBranch tests | Added default-off RT64 diagnostics and toggles for three renderer-address hypotheses: direct `0x86xxxxxx` display-list operands bypassing segment-6 lookup, RDP/RSP physical-data stripping of LoD NI `0x8E`/`0x8F` mirrors to empty `0x0E`/`0x0F`, and global `f3dex.forceBranch` selecting alternate BranchZ/W display-list paths. In the all-toggle diagnostic build, geometry explosions and black/missing models appeared unchanged, while widescreen edge popping seemed reduced. The no-forceBranch-only build kept popping minimal/good-enough, but skirt/lantern/black-model issues remained. This weakens the address-mapping hypotheses for the model corruption and makes `LOD_DISABLE_RT64_FORCE_BRANCH` the active widescreen-popping lead. | Treat widescreen popping as secondary unless it regresses. For remaining geometry/black-model glitches, focus on RT64 F3DEX vertex/matrix/lighting/combiner handling and unsupported display-list command paths rather than raw segmented address mapping. |
| RENDER-002 | targeted geometry/display-list diagnostics for skirt/lantern/black-model glitches | default build and `build-render-geomtrace` compiled on `diag/render-glitch-root-causes`; corrected manual trace captured suspicious vertex sources | Added `LOD_ENABLE_RENDER_GEOM_TRACE`, default OFF. When enabled it emits throttled RT64 traces for suspicious/overlay-sourced sub-display-list calls, vertex loads, matrix loads, black/overlay light loads, and BranchZ/W decisions. The trace build also uses `LOD_DISABLE_RT64_FORCE_BRANCH=ON` so manual tests can keep the better widescreen behavior while capturing model-glitch evidence. Corrected manual trace showed no invalid sub-DLs and no matrix/light/branch signal; the strong signal was repeated suspicious standard vertex loads from `0x003784E8..0x00378738` after `MAP_OVL #2`, with raw coordinates like `-32768`/`21812`. Refined the trace to suppress duplicate valid overlay DL spam and add `[RT64-GEOM][VTXCMD]`/`[VTXRAW]` caller, decoder, raw command, alternate vertex-decode, and raw vertex dumps. | Manual repro should relaunch `build-render-geomtrace/LodRecomp`, route through the bad cutscenes, then inspect `[RT64-GEOM][VTXCMD]` and `[RT64-GEOM][VTXRAW]` first. Use decoder/caller/raw-command context to decide whether the model bug is wrong microcode/GBI vertex decode, wrong asset source, or nonstandard/skinned vertex data being treated as plain `Vtx`. |
| RENDER-003 | vertex-tail validation for intro forest geometry trace | `build-render-geomtrace/LodRecomp`, local bounded 22s run, `/tmp/lod_render_geomtrace_22s.log` | Added flag-gated extreme-index dumps and triangle-use logging for suspicious local vertices. The repeated `F3DEX2` vertex commands after `MAP_OVL #2` still overread into display-list/data-looking tails (for example source `0x00378738` has bad extrema at `+11`, `+17`, `+23`, `+25`), but the run produced `TRI_SUSPICIOUS=0`, plus no matrix or light anomaly logs. This means the strongest raw-vertex-tail signal is probably not itself the visible skirt explosion in the captured window, at least with the current `abs(coord)>20000` triangle threshold. | Next diagnostic should lower/target the triangle outlier threshold (for example `>4096` or referenced tail slots) and/or add a visual tint mode for candidate draw clusters so manual testing can identify the skirt/lantern/merchant geometry directly. |
| RENDER-004 | candidate-cluster triangle/tint diagnostic | `build-render-geomtrace/LodRecomp`, `build-render-tint/LodRecomp`, local bounded 22s runs, `/tmp/lod_render_geomtrace_candidate_22s.log`, `/tmp/lod_render_tint_22s.log` | Lowering the triangle threshold to `4096` was initially too broad and only found a legitimate flat terrain/grid cluster at `0x00385000` (`geom=0x00A10405`, all `y=0`). Refined tracing now suppresses that case and marks triangles that reference slots from suspicious vertex loads. Result: 84 candidate triangles, all `level=CANDIDATE`, all `cluster=0x00378000`, `geom=0x00A30405`, no `SUSPICIOUS`/extreme referenced triangles, and no matrix/light anomalies. The candidate geometry only uses source indices `0..9`; the bad tail entries remain unreferenced. Added default-off `LOD_ENABLE_RENDER_CLUSTER_TINT` to visibly tint the `0x00378000` candidate cluster for manual identification. | Ask tester whether the tinted/oddly-lit candidate cluster corresponds to the skirt/dress. If yes, next focus is transform/matrix/skin/display-list mode for this small valid-vertex cluster, not vertex-tail overread. If no, add a broader tint selector for the next candidate cluster seen in the bad cutscene. |
| RENDER-005 | confirmed early forest/skirt geometry root | `build-render-tint/LodRecomp`, local bounded 20s runs, `/tmp/lod_render_earlyroot_tint_20s_v3.log`, `/tmp/lod_render_earlyroot_filter_20s.log` | User-provided scene log showed the relevant early forest window occurs before `MAP_OVL #2`, not in the later candidate cluster. Added flag-gated RT64 DL-root context tracking for `0x0F002230`/`0x0F0022A8`, including branch-style `G_DL`/tail-call roots. The corrected trace produced 512 `EARLY_ROOT` triangles under root `0x0F0022A8/0x8F0022A8`, all from dynamic vertex cluster `0x0037A000` with source range `0x0037AB30..0x0037AF20`, `geom=0x00A30405`, and xforms `42/43/44/72`. No VTX/MTX/LIGHT loads occurred inside the root; vertices are preloaded/generated before the root draws triangles. Weak tinting did not show because texture/combiner state dominated, but changing the default-off diagnostic marker to drop these early-root triangles visibly removed/altered the skirt/underclothes geometry, confirming this is the target. This supersedes the earlier `0x00378000`/tail-overread lead for the user's skirt scene. | Next inspect the dynamic vertex generation/skinning or transform selection feeding `0x0037AB30..0x0037AF20`, especially why some frames produce stretched skirt triangles with large deltas under xforms `42/43/44/72`. Keep the drop-filter diagnostic flag-gated and off in default builds. |
| RENDER-006 | NI overlay mutable-data persistence fixes skirt deformation | `build-skirt-compute/LodRecomp`, local bounded 20s runs: `/tmp/lod_render_skirt_compute_detail_20s.log`, `/tmp/lod_render_skirt_anchor_defer2_20s.log`, `/tmp/lod_render_ni_persist_skirt_20s.log`; user visual validation | Confirmed CPU-side dynamic vertex generation, not RT64 drawing, drives the skirt cluster. `model_compute_vertex_delta` feeds `model_apply_scaled_vertex_delta_cmds`/`model_apply_vertex_delta_cmd`; the skirt cmdlist `0x0F002190` ping-pongs `0x0037AB30..0x0037AF20`. Skip/defer anchor tests reduced individual huge deltas but broke animation or reintroduced the bad frame, so anchor suppression was rejected. Root cause is broader: pair 53's writable NI overlay static data at `0x0F0023BC` (previous position used by `ni_ovl_053_func_0F00087C`) was reset to pristine ROM whenever pair 104 swapped into the shared 0x8F mirror. Generic per-pair NI segment save/restore (`LOD_FIX_NI_PERSIST_OVERLAY_DATA`, default ON) preserves mutable overlay data across pair swaps; logs show later `s0` deltas shrink from roughly `(1202,18,-300)` to frame-local values such as `(0.047,0,4.29)`, and user confirmed the skirt visual is fixed. | Keep the generic NI persistence fix. Temporary generated diagnostics were removed via the regen pipeline before final validation; use this same class of bug as a candidate explanation for other model/animation glitches involving rapidly swapped NI overlays. |

## Gameplay / Enemy Damage Investigation

| ID | Area | Validation | Findings | Next Action |
|----|------|------------|----------|-------------|
| ENEMY-001 | Current knight target unkillable / no attack behavior | `build-enemy-trace/LodRecomp` with `LOD_ENABLE_ENEMY_TRACE=ON`; manual attack traces in `~/Desktop/enemy-damage-pair*.log` | Collision reaches the core Atari damage-pair resolver: player-weapon/knight pairs hit `0x80042338`, target Atari base gets `damage_received` (`h4A`, e.g. `1300`) and attacker flags (`w4C=1`), but normal enemy-list state only toggles contact/grab flags. The default-off `LOD_FIX_ENEMY_ATARI_DAMAGE_TEST` experiment proves the raw Atari health field can be decremented at the damage write site (`0x80042404`), but user validation showed the enemy still does not enter normal damaged/death/attack behavior. | Treat the health subtraction as a diagnostic only. Next trace is actor/AI-state focused (`[ENEMY_ACTOR_TRACE]`) to identify why current-function/state transitions are not consuming damage or starting attacks. |
| ENEMY-002 | Stage 6 Art Tower knights (`actor_id=0x20DD`, `ni_ovl_015`) AI/damage/death transition | `build-enemy-trace/LodRecomp`, local/manual route, `~/Desktop/enemy-actor-state.log`, `~/Desktop/knight-split-test.log`, `~/Desktop/knight-split-clean.log`; user visual validation | Root AI/animation bug was a missing internal function boundary: table targets `0x0F008944[8]` and `0x0F008964[0]` jump to `0x0F002A4C`, but generated code previously folded that target into `0x0F002830`, so indirect dispatch range-fallback ran the wrong handler and the knights looped instead of walking/chasing/swinging. Splitting `ni_ovl_015_knight_clear_motion_substate` at `0x0F002A4C` and regenerating made the knights animate correctly. Clean split-only validation with damage/lethal/engage experiments off was reported perfect by the user and had no illegal/SIG/crash/assert signals; the earlier falling-floor death was caused by the temporary lethal-gate diagnostic, not the split. | Keep the `0x0F002A4C` split and the generalized call-table audit from ENEMY-003. |
| ENEMY-003 | Generalized NI call-table internal-target splitting | `tools/audit_indirect_targets.py --scan-call-tables --fail-on-missing`; `./tools/regen_recomp.sh`; `cmake -S . -B build-enemy-trace && cmake --build build-enemy-trace --target LodRecomp`; bounded 20s smoke `/tmp/lod_generalized_smoke.log` | Extended the NI indirect-target audit to locally slice MIPS `jalr` call-table patterns, so raw-data callback/state tables are audited without enabling noisy full-raw pointer scanning. Applied 166 build-safe function-boundary splits in `castlevania2.syms.toml`; regenerated code reaches the normal overlay_system truncation/repair point and the strengthened audit reports no missing targets. `ni118` tiny branch labels and `ni120` result labels are excluded because they make N64Recomp abort early; `ni120` remains covered by its scoped runtime internal-label shim. | Keep regen step 10 running `--scan-call-tables --fail-on-missing`. If future call-table targets appear, run `--fix-syms`, regenerate, and only add an exclusion when a split demonstrably breaks N64Recomp or is already handled by a scoped shim. |

## Open Questions

- What exact signal/address/function context occurs when the intro sequence is allowed to play naturally?
- Does the reported natural intro `Signal 15` match the repeated late `gs=3` `Signal 10`/`0x380800002` crash, or are they separate blockers?
- Does high-resolution mode uniquely reproduce the `0x3880006d0` no-match crash, or can long low-resolution sessions still hit it later? Default builds now report 4MB RDRAM and hard-skip only the `gs=12` selector by requesting `gs=-5` to keep testers on the validated low-resolution path; investigate manually with `-DLOD_SKIP_EXPANSION_PAK_SCREEN=OFF -DLOD_ENABLE_NATIVE_HIGH_RES=ON`.
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

Work item `G6-001`: preserve the now-validated low-resolution natural route into gameplay, then stabilize long-idle gameplay and high-resolution intro/gameplay `gs=3`. The current user save route is validated after the pair-7 splits at `0x0F00162C` and `0x0F010218`; the generalized audit workflow is now in place before push. If another in-range NI table-target crash appears, run `tools/audit_indirect_targets.py --scan-full-raw --section <pair> --target <addr>` first, then split only inspected/validated callable boundaries. Separately, the NI TLB skip-window bug and `0x8E`/`0x8F` mirror dispatch miss have concrete local fixes, and telemetry is available for future KSEG0/no-match crashes: native backtrace, RDRAM-relative fault decoding, DMAMgr snapshot, last-`DMA_ROMCopy` snapshot, and enhanced NI no-match context. Do not keep looping on the guard unless a new crash provides fresh evidence.

Parallel audio work is now a real milestone: generated audio RSP is wired, the libaudio PVoice free-list bug was traced to empty `__osDequeueThread`/`__osEnqueueThread` stubs that must behave as ALLink list helpers, `osPiStartDma` naming fixed sample DMA evidence, and the LoD-local AI pointer wrapper prevents the physical-buffer queue crash. The user confirmed audible game audio, including music. The minimal audible config has now graduated into the default baseline: generated audio RSP on and `alEnvmixerPull` no-op override off. Small visual effects/HUD item textures also have a validated baseline fix: `LOD_FIX_SEGMENT_86_BASES` normalizes RT64 segment bases from `0x86xxxxxx` to `0x80xxxxxx`, restoring assets that previously loaded from empty `0x061A....` addresses. Remaining work is quality/stability/progression triage: high-res/base-NI no-match `0x3880006d0` classification, longer gameplay/manual crash capture, and targeted low-noise crash context for manual audio builds. Use fast-skip for KSEG0 idle loops; reserve full natural prologue runs for intro-stability checks. Keep debug-only input/CFB/audio snapshots as test harnesses and avoid gamestate skips, app-side audio gain, or broad address rewrites as permanent fixes.
