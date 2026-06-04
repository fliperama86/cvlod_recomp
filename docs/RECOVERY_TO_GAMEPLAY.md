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

- Code baseline commit: `18a56c5` (`Sanitize recovery baseline`).
- Tracker established at: `c5952c1` (`Replace scratchbook with recovery tracker`).
- Build status: `cmake --build build` succeeds on macOS with SDK include-order sanitation in `CMakeLists.txt`.
- Launch status: bounded `20s` launch reaches the game without an internal crash; timeout/SIGTERM stops it cleanly.
- Runtime state observed after baseline:
  - Gamestate remains `gs=4`.
  - Save object state advances `255 → 1 → 2 → 3`.
  - NI overlay cycle continues around pairs `105 / 120 / 101`.
  - Input callback is active.
  - Max RSS in the latest 20s bounded run was about `861 MB`.
- Clean baseline policy:
  - No local dirty submodule changes.
  - No editor/debug leftovers.
  - No runtime/gameplay skip enabled by default.

## Goal

Reach actual controllable gameplay through the game's natural state flow, not by permanently skipping or forcing gamestates.

## Active Work Item

### G4-001 — Determine What `gs=4` Is Waiting For

Objective:

- Decide whether the current `gs=4` stall is caused by user/menu input, Controller Pak/PFS state, missing gamestate write, or missing object/state dispatch.

Current evidence:

- The fixed parent object at `0x8031AFA4` reaches dispatch state `3`.
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
- PFS path can initialize/read/write pak image and reports no existing CASTLEVANIA2 file through the current find-file path.
- The runtime continues cycling NI overlays `105 / 120 / 101`.
- `exec`, `ni`, `gate`, and `sel` telemetry stayed flat in previous samples.

Latest bounded trace (`G4-001`, 20s, default flags):

- Build succeeds.
- Final schedule-aware sample timed out cleanly (`exit=124`) with no stale `LodRecomp` process.
- One intermediate precondition sample hit `SIGBUS` (`exit=138`), not reproduced in the final sample.
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

1. Determine whether the fixed parent object is supposed to execute
   `save_screen_outer_state3_update`, or whether `0x8031AFA4` is only a container/sentinel
   and the real state owner is a nearby child object.
2. Trace the actual object passed into outer state `2`/`3` handlers:
   - `save_screen_schedule_dispatch` (`0x802ECA84`)
   - `save_screen_outer_state3_update` (`0x802ECF4C`)
3. Identify the writer/initializer for object `+0x24`, `+0x34`, and `+0x70` in the
   save-screen object chain.
4. Only after the owner/precondition issue is explained, decide whether PFS/input status is
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

- Add the narrowest next trace around the actual outer handler owner:
  - object pointer passed to `save_screen_outer_dispatch`
  - object pointer passed to `save_screen_schedule_dispatch`
  - object pointer passed to `save_screen_outer_state3_update`
  - object `+0x24/+0x34/+0x70` at entry
  - whether `object_curLevel_goToNextFunc` is requested for states `3` or `9`

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
| `LOD_ENABLE_SAVE_AUTO_INPUT` | off | Debug-only; should stay off by default | Use only for explicit input-flow experiments |
| `LOD_ENABLE_BOOT_GS_SKIP` | off | Debug-only; should stay off by default | Use only to compare downstream behavior, never as permanent fix |

## Experiment Log

Record durable experiments here. Keep entries concise and technical.

| ID | Flags / build | Runtime bound | Result | Conclusion |
|----|---------------|---------------|--------|------------|
| BASE-001 | default baseline at `18a56c5` | 20s | Built, launched, reached `gs=4`; save object `255 → 1 → 2 → 3`; NI loop `105 / 120 / 101`; no internal crash; timeout stopped it | Clean baseline is viable; primary blocker is natural transition out of `gs=4` |
| BUILD-001 | `LOD_OVERRIDE_CONTPAK_INSERTED_STATUS=0`, `LOD_OVERRIDE_FUNC_8001D398=1` compile-only check | n/a | `src/main/ignored_func_stubs.cpp` compiled successfully | Review-reported dependent override-gate compile failure is fixed |
| G4-001A | default flags + schedule-aware save dispatch trace | 20s | Built; final run timed out cleanly; fixed parent `0x8031AFA4` reached outer dispatch state `3` → `save_screen_outer_state3_update` while `+0x24/+0x34/+0x70` were zero; nearby child objects had valid-looking pointers | The state-3 blocker is now an object ownership/precondition problem, not simply "inner table state 3 waiting for input" |

## Open Questions

- Which object actually owns save-screen outer state `3`: fixed parent `0x8031AFA4` or a nearby child?
- Why does fixed parent `0x8031AFA4` reach outer state `3` with `+0x24/+0x34/+0x70 == 0`?
- Which fields represent the current save-screen selection/cursor?
- Does state `3` expect a button press that is not being delivered/consumed?
- Does state `3` consume `osPfsFindFile`/Pak status in a way that keeps it in the loop?
- Which function should write the next gamestate after a valid selection?
- Are NI pairs `105 / 120 / 101` the expected idle loop for the save screen, or evidence of an overlay dispatch mismatch?

## Dependency Posture

Do not wholesale update dependencies until the `gs=4` blocker is explained.

Only consider dependency cherry-picks if they directly affect:

- Controller Pak/PIF behavior.
- Overlay loading.
- Scheduling/message queue correctness.

## Current Next Step

Work item `G4-001`: trace the actual object pointer passed to `save_screen_outer_state3_update`
and identify why the fixed parent reaches outer state `3` with missing `+0x24/+0x34/+0x70`
preconditions. Do not add a gameplay skip or force a gamestate transition yet.
