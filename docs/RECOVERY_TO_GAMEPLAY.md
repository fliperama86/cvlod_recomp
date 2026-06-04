# Recovery to Gameplay Tracker

This is the live, durable tracker for getting Castlevania: Legacy of Darkness recompilation from the current boot/save-screen state into actual controllable gameplay.

`SCRATCHBOOK.md` is retired. Keep this document focused and update it as recovery work changes the baseline, hypotheses, experiments, milestones, validation status, or next actions.

## Maintenance Rules

- Update this file during recovery work so progress is not lost between sessions.
- Do not append per-turn chatter; record durable technical state only.
- When an experiment is run, record:
  - build/runtime flags used
  - bounded-run duration
  - observed state changes or regressions
  - conclusion and next action
- When a function is identified, also update `FUNCTION_MAP.md`, `castlevania2.syms.toml`, and `symbol_addrs.txt` as appropriate.

## Current Baseline

- Tracker established at: `c5952c1` (`Replace scratchbook with recovery tracker`).
- Last pre-alignment pushed trace commit: `b1ce215` (`Trace save-screen dispatch state`).
- Build status: `cmake --build build` succeeds on macOS with SDK include-order sanitation in `CMakeLists.txt`.
- Launch status: bounded `20s` launch reaches the game without an internal crash; timeout/SIGTERM stops it cleanly.
- Startup overlay status:
  - LoD starts naturally in `gs=4`.
  - Initial map overlay registration and RDRAM bytes are now aligned to `map_ovl_34`
    (`ROM 0x7EF5E0 → RAM 0x802E3B70`, size `0x16690`).
  - This replaces the previous mixed startup state where `map_ovl_00` code was registered while
    `map_ovl_34` data/jump tables were copied.
- Runtime state observed after alignment:
  - Gamestate remains `gs=4`.
  - Save object state advances `255 → 1 → 2 → 3` and then polls state `3`.
  - No `Failed to find function` occurs in validated 20s samples.
  - Input callback is active; debug auto-input can move the save-screen state machine but does not yet reach gameplay.
- Clean baseline policy:
  - No local dirty submodule changes.
  - No editor/debug leftovers.
  - No runtime/gameplay skip enabled by default.
  - Auto-input and handler function-map wrappers are compile-time gated and default off.

## Goal

Reach actual controllable gameplay through the game's natural state flow, not by permanently skipping or forcing gamestates.

## Active Work Item

### G4-001 — Determine What `gs=4` Is Waiting For

Objective:

- Decide whether the current `gs=4` stall is caused by user/menu input, Controller Pak/PFS state, missing gamestate write, or missing object/state dispatch.

Current evidence:

- The fixed parent object at `0x8031AFA4` reaches dispatch state `3`.
- Startup overlay code/data mismatch was a real precondition bug:
  - previous startup registered `map_ovl_00` code at `0x802E3B70` but copied `map_ovl_34` data there;
  - save-screen tables can point at functions such as `0x802EBAF0`, beyond the registered `map_ovl_00` section;
  - aligning startup to `map_ovl_34` removes that missing-function risk.
- The save-screen dispatch model has two adjacent tables:
  - outer table `0x802F7170`, used by `save_screen_outer_dispatch`
  - inner table `0x802F71A8`, used by `save_screen_schedule_dispatch`
- Parent dispatch state `3` maps through the outer table to
  `0x802ECF4C`, now named `save_screen_outer_state3_update`.
- Inner table state `3` maps to `0x802ED804`, now named `save_screen_state3_update`,
  but that is not the fixed parent object's direct state-3 handler.
- `save_screen_outer_state3_update` expects valid object `+0x24` and `+0x34` pointers.
  The fixed parent object currently has both as zero when it reaches outer state `3`.
- Input callback is active.
- Temporary auto-input (`Down`, `A`, `A`) is consumed by the screen:
  - state advances `3 → 4 → 5 → 0`, then loops back through initialization;
  - PFS lookup/status paths run (`contpak_get_inserted_status`, `osPfsFindFile`);
  - gamestate still stays `gs=4`.
- ROM scan found a main-code save/PFS state schedule table at ROM `0xB43C0`.
  It contains `0x8001AF54`, which calls `gamestate_change(3)` and then advances the
  object's current schedule level via `object_curLevel_goToNextFuncAndClearTimer`.
- Temporary exit tracing (`LOD_ENABLE_SAVE_EXIT_TRACE=1`) proved the current tested
  save-screen loop advances through `object_curLevel_goToNextFuncAndClearTimer`
  (`0x80001CE8`) on objects around `0x8031AFA4`, but did not reach `0x8001AF54`
  or the adjacent main-code table handlers in the auto-input path.
- PFS path can initialize/read/write pak image and reports no existing CASTLEVANIA2 file through the current find-file path.
- The runtime continues cycling NI overlays `105 / 120 / 101`.
- `exec`, `ni`, `gate`, and `sel` telemetry stayed flat in previous samples.

Latest bounded traces (`G4-001`, 20s):

- Default sanitized build succeeds.
- Final sanitized default sample timed out cleanly (`exit=124`) with no stale `LodRecomp` process.
- Init logs confirm aligned startup overlay:
  - `Registered and copied map_ovl_34 for initial gs=4 at 0x802E3B70`
  - `Refreshed initial map_ovl_34 bytes: ROM 0x7EF5E0 → rdram+0x2E3B70 (91792 bytes)`
- No `Failed to find function`, no `[CRASH]`, no auto-input, no handler-wrapper install in the default sample.
- Temporary auto-input sample (`LOD_ENABLE_SAVE_AUTO_INPUT=1`) timed out cleanly and proved baked controller input reaches the game.
- Temporary exit-trace sample (`LOD_ENABLE_SAVE_AUTO_INPUT=1`, `LOD_ENABLE_SAVE_EXIT_TRACE=1`) timed out cleanly in the first sample:
  - wrappers installed for `0x80001C20`, `0x80001CE8`, `0x8001ACB0`, `0x8001AF54`,
    `0x8001AF90`, `0x8001AFF0`, and `0x8001B0B4`;
  - 40 traced `object_curLevel_goToNextFuncAndClearTimer` calls occurred for save-screen objects;
  - no `0x8001AF54` exit-candidate hit and no adjacent main-code table-handler hit occurred;
  - auto-input fired all three baked inputs and the screen looped `3 → 4 → 5 → 1 → 2 → 3`;
  - no `Failed to find function` and no internal crash before the timeout signal.
- A second loosened trace sample reproduced the same lack of main-code table/exit hits before an intermittent `SIGBUS` after the first baked `DPad-Down`; treat this as non-baseline until reproduced without temporary trace.
- One older intermediate precondition sample hit `SIGBUS` (`exit=138`), not reproduced after final validation.
- Parent object at `0x8031AFA4`:
  - `state09=3`, `func_id=-1`, `dispatch_state=3`
  - outer handler `0x802ECF4C` / inner handler `0x802ED804`
  - `fig0=0`, `alloc0=0`, `alloc15=0`
  - `disp_flags=0`, `map6FE4=0`, input/status flags `0x801CAB18=0`
  - `pfs_status=1`, `contpak_uninserted[0]=0`
- Nearby object window at parent state-3 entry:
  - `0x8031B08C`: child of parent, `dispatch_state=1`, outer handler `0x802EC550`,
    `fig0=0x802ECCE8`, `alloc0=0x802A9770`, `alloc1=0x802A3B88`
  - `0x8031B100`: child of `0x8031B018`, `dispatch_state=2`, outer handler
    `save_screen_schedule_dispatch`, `fig0=0x802ECC30`, `alloc0=0`, `alloc1=0x80324920`

Next action:

1. Focus the next trace on map_ovl_34 state handlers for states `4` and `5`, plus the
   fields they read to decide whether to loop or transition.
2. Determine why the tested loop does not reach the main-code `0x8001AF54`
   `gamestate_change(3)` table target. Current evidence says this is either a different
   branch/owner table or a later PFS/menu result not reached by the baked inputs.
3. Determine whether fixed parent `0x8031AFA4` is supposed to execute
   `save_screen_outer_state3_update`, or whether it is only a container/sentinel and the real
   state owner is a nearby child object.
4. Identify the writer/initializer for object `+0x24`, `+0x34`, and `+0x70` in the
   save-screen object chain.
5. Trace which field/button/PFS result should write the next gamestate after state `5`.
6. Only after the owner/precondition issue is explained, decide whether PFS/input status is
   blocking the transition or merely a symptom.

## Milestones

### Milestone 1 — Explain the `gs=4` Stall

Determine why the save/Controller Pak screen does not naturally transition onward.

Known facts:

- LoD intentionally starts at `gs=4` on real hardware/emulator.
- `gs=4` is the save/Controller Pak management screen.
- Current recomp reaches this screen path and advances the fixed parent object to outer
  dispatch state `3`.
- Current recomp continues cycling NI overlays `105 / 120 / 101` without transitioning.
- The outer/inner save-screen dispatch tables are now decoded and documented in
  `FUNCTION_MAP.md`.

Questions to answer:

- Is `gs=4` waiting for user input/menu selection?
- Is it blocked by a Controller Pak/PFS result?
- Is it failing to write the next gamestate?
- Is an object/state-machine dispatch path missing?

Next recommended experiment:

- Add the narrowest next trace around map_ovl_34 state `4`/`5` handlers and the
  loop/exit decision fields:
  - state/function slots that drive objects `0x8031AFA4`, `0x8031B018`, `0x8031B08C`, `0x8031B100`;
  - object `+0x24/+0x34/+0x70` writes before state `3`;
  - global PFS/menu decision fields read by outer-table states `4` and `5`;
  - why state `5` advances back to state `0/1` instead of selecting an exit path;
  - whether the main-code `0x8001AF54` table is an alternate branch or an unreachable intended exit.

### Milestone 2 — Burn Down or Justify Remaining Overrides

Each override should be either removed or documented as a required compatibility shim.

Test one at a time:

1. `LOD_OVERRIDE_FUNC_8009E480=0` — audio-chain no-op.
2. `LOD_OVERRIDE_FUNC_8001D398=0` — Controller Pak inserted recheck.
3. `LOD_OVERRIDE_FUNC_8001C93C=0` — Pak validation gate.
4. `LOD_OVERRIDE_FUNC_8009F400=0` — PFS find-file override.
5. `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS=0` — Pak inserted-status wrapper.

Rules:

- Change only one variable per run.
- Keep experiments bounded.
- Record whether the result is build failure, crash, regression, or no behavioral change.
- If keeping an override, document the specific runtime contract it compensates for.

### Milestone 3 — Validate Controller Pak Path

Use `docs/controller-pak-approaches.md` for detailed PFS notes.

Questions:

- Does the game expect an existing CASTLEVANIA2 note, a newly created note, or a “continue without saving” path?
- Which PFS result should lead from `gs=4` to the next gamestate?
- Are current PIF/PFS return codes faithful enough for this screen?

### Milestone 4 — Transition Past Save Screen Naturally

Expected original flow reference:

1. `gs=4` — save/Controller Pak screen.
2. Next intro/title/game states, eventually into actual gameplay.

Success condition:

- The game leaves `gs=4` without default-on forced gamestate skip.

### Milestone 5 — Reach Controllable Gameplay

Success condition:

- Player reaches an in-game controllable scene.
- Input works.
- Renderer remains stable.
- RSS remains bounded.
- No stale process remains after bounded run.

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
- Reintroducing dirty submodule-only runtime changes.
- Running N64Recomp directly instead of `./tools/regen_recomp.sh`.
- Long unbounded runs during leak-prone experiments.

## Symbol Naming Queue

Keep this synchronized with `FUNCTION_MAP.md`, `castlevania2.syms.toml`, and `symbol_addrs.txt`.

Priority naming targets:

- Save object update function.
- Save object state handlers.
- Gamestate transition setter(s).
- NI overlay pair `101`, `105`, and `120` owner functions.
- PFS wrapper callsites involved in the save screen.

## Override Matrix

The current baseline keeps several old bring-up overrides default-on to preserve behavior. Each one needs an explicit disposition.

| Override | Default | Current disposition | Next test |
|----------|---------|---------------------|-----------|
| `LOD_OVERRIDE_FUNC_8009E480` | on | Unknown; audio-chain no-op may mask a real issue | Build/run with `=0` after `gs=4` tracing is understood |
| `LOD_OVERRIDE_FUNC_8001D398` | on | Known build-gate issue fixed; runtime necessity still unknown | Build/run with `=0` |
| `LOD_OVERRIDE_FUNC_8001C93C` | on | Unknown; Pak validation gate | Build/run with `=0` |
| `LOD_OVERRIDE_FUNC_8009F400` | on | Unknown; PFS find-file shim | Build/run with `=0` |
| `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS` | on | Dedicated compile combo passed with this disabled while `FUNC_8001D398` stayed enabled | Build/run with `=0` |
| `LOD_ENABLE_SAVE_AUTO_INPUT` | off | Debug-only; tested and confirmed to move the save-screen state machine but not leave `gs=4` | Use only for explicit input-flow experiments |
| `LOD_ENABLE_SAVE_HANDLER_TRACE` | off | Debug-only function-map wrapper probe; default-off to avoid hiding dispatch bugs | Enable only for targeted handler-entry confirmation |
| `LOD_ENABLE_SAVE_EXIT_TRACE` | off | Debug-only wrapper probe for main-code save/PFS exit candidates and schedule advances | Use to confirm whether `0x8001AF54` or adjacent table handlers are reached |
| `LOD_ENABLE_BOOT_GS_SKIP` | off | Debug-only; should stay off by default | Use only to compare downstream behavior, never as permanent fix |

## Experiment Log

Record durable experiments here. Keep entries concise and technical.

| ID | Flags / build | Runtime bound | Result | Conclusion |
|----|---------------|---------------|--------|------------|
| BASE-001 | default baseline at `18a56c5` | 20s | Built, launched, reached `gs=4`; save object `255 → 1 → 2 → 3`; NI loop `105 / 120 / 101`; no internal crash; timeout stopped it | Clean baseline is viable; primary blocker is natural transition out of `gs=4` |
| BUILD-001 | `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS=0`, `LOD_OVERRIDE_FUNC_8001D398=1` compile-only check | n/a | `src/main/ignored_func_stubs.cpp` compiled successfully | Review-reported dependent override-gate compile failure is fixed |
| G4-001A | default flags + schedule-aware save dispatch trace | 20s | Built; final run timed out cleanly; fixed parent `0x8031AFA4` reached outer dispatch state `3` → `save_screen_outer_state3_update` while `+0x24/+0x34/+0x70` were zero; nearby child objects had valid-looking pointers | The state-3 blocker is now an object ownership/precondition problem, not simply "inner table state 3 waiting for input" |
| G4-001B | default flags + initial `map_ovl_34` code/data alignment | 20s | Built; launched; no `Failed to find function`; save-screen state still reaches/polls parent dispatch state `3` | Overlay alignment is a real precondition fix, not a gameplay skip; remaining blocker is later state/owner/PFS flow |
| G4-001C | temporary auto-input build (`LOD_ENABLE_SAVE_AUTO_INPUT=1`; handler wrapper probe available as `LOD_ENABLE_SAVE_HANDLER_TRACE=1`) | 20s | Baked `Down`, `A`, `A` fired; state moved `3 → 4 → 5 → 0`; `contpak_get_inserted_status` and `osPfsFindFile` ran; game stayed `gs=4`; no handler-entry wrapper hits | Input is reaching the game; the menu/PFS path loops instead of transitioning, so the next target is the state owner/result that should leave `gs=4` |
| G4-001D | sanitized default flags after gating auto-input and handler wrappers off | 20s | Build/run clean; init confirms `map_ovl_34`; no auto-input; no handler wrapper install; no missing-function crash; timeout stopped cleanly | Default baseline is clean while retaining optional probes for targeted experiments |
| G4-001E | temporary exit-trace build (`LOD_ENABLE_SAVE_AUTO_INPUT=1`, `LOD_ENABLE_SAVE_EXIT_TRACE=1`) | 20s | First sample timed out cleanly; wrappers installed; 40 `0x80001CE8` schedule advances logged for save-screen objects; no `0x8001AF54` or adjacent main-code table-handler hit; no missing function/crash before timeout | The active tested loop is map_ovl_34 object scheduling, not the main-code `gamestate_change(3)` table target |
| G4-001F | same temporary exit-trace build with less-filtered table logs | <20s | Built; crashed with `SIGBUS` after first baked `DPad-Down`; before crash, still no main-code table-handler or `0x8001AF54` hit | Treat as trace-only/non-baseline until reproduced without temporary trace |

## Open Questions

- Which object actually owns save-screen outer state `3`: fixed parent `0x8031AFA4` or a nearby child?
- Why does fixed parent `0x8031AFA4` reach outer state `3` with `+0x24/+0x34/+0x70 == 0`?
- Which fields represent the current save-screen selection/cursor?
- Which specific state/selection result should follow the consumed `A` press and state `3 → 4 → 5 → 0` loop?
- Does state `3` consume `osPfsFindFile`/Pak status in a way that keeps it in the loop?
- Which function should write the next gamestate after a valid selection?
- Is main-code `0x8001AF54` an alternate save/PFS branch, or is the current map_ovl_34 path failing to select it?
- Are NI pairs `105 / 120 / 101` the expected idle loop for the save screen, or evidence of an overlay dispatch mismatch?

## Dependency Posture

Do not wholesale update dependencies until the `gs=4` blocker is explained.

Only consider dependency cherry-picks if they directly affect:

- Controller Pak/PIF behavior.
- Overlay loading.
- Scheduling/message queue correctness.

## Current Next Step

Work item `G4-001`: trace the object/GSS executor path that advances the save-screen object chain,
especially around objects `0x8031AFA4`, `0x8031B018`, `0x8031B08C`, and `0x8031B100`. Identify
which owner/state/PFS result should leave `gs=4`. Do not add a gameplay skip or force a gamestate
transition yet.
