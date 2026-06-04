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

- Baseline commit: `18a56c5` (`Sanitize recovery baseline`).
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

## Milestones

### Milestone 1 — Explain the `gs=4` Stall

Determine why the save/Controller Pak screen does not naturally transition onward.

Known facts:

- LoD intentionally starts at `gs=4` on real hardware/emulator.
- `gs=4` is the save/Controller Pak management screen.
- Current recomp reaches this screen path and advances the save object to state `3`.
- Current recomp continues cycling NI overlays `105 / 120 / 101` without transitioning.

Questions to answer:

- Is `gs=4` waiting for user input/menu selection?
- Is it blocked by a Controller Pak/PFS result?
- Is it failing to write the next gamestate?
- Is an object/state-machine dispatch path missing?

Next recommended experiment:

- Run a longer bounded trace with targeted logs only, focused on:
  - current game state
  - exiting game state
  - save object state
  - selected save/menu option
  - consumed input bits
  - latest PFS result
  - functions writing gamestate fields

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

## Dependency Posture

Do not wholesale update dependencies until the `gs=4` blocker is explained.

Only consider dependency cherry-picks if they directly affect:

- Controller Pak/PIF behavior.
- Overlay loading.
- Scheduling/message queue correctness.

## Current Next Step

Instrument the `gs=4` transition path narrowly enough to answer whether the screen is waiting on input, PFS state, or a missing gamestate write.
