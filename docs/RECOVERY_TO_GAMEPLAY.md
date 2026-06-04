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
- Last pushed baseline before this update: `13c0a12` (`Trace NI pair 120 save reset caller`).
- Build status: default `cmake --build build --target LodRecomp` succeeds on macOS.
- Launch discipline:
  - codesign before every macOS launch;
  - bounded `timeout` runs only;
  - verify no stale `LodRecomp` process after each run.
- Default runtime flags:
  - no boot gamestate skip;
  - no auto-input;
  - pair-120 internal-label dispatch fix is default-on;
  - stale direct `func_8009F400`/`osPfsFindFile` shim is default-off.
- Validated default behavior:
  - With an existing virtual Controller Pak save note, default launch reaches `gs=12` cleanly from the natural boot/save flow.
  - With a truly empty virtual Controller Pak and no input, default launch formats the Pak and remains in `gs=4` waiting for selection/input; this is expected menu behavior, not a crash.
  - With a truly empty virtual Controller Pak plus debug auto-input (`Down`, `A`, `A`), the game creates/uses the note and reaches `gs=12` cleanly.
  - User visual confirmation: `gs=12` is the Expansion Pak screen.
  - A longer default existing-save run naturally advanced `gs=12 → -5 → 5`, then crashed in the next flow after loading NI pair `104`.
  - Current crash signature: `Signal 11` at `0x4001cad9c`, no `Failed to find function`, no stale process; last function lookups include NI pair `104` entries `0x0F000000` and `0x0F000130`, then main-code handlers around `0x8001B4C0/0x8001B530`.

## Goal

Reach actual controllable gameplay through the game's natural state flow, not by permanently skipping screens or forcing gamestates.

## Current Active Work Item

### G5-001 — Fix Transition After Expansion Pak Screen

Objective:

- Fix the natural transition after the Expansion Pak screen so the game continues through intro/title toward gameplay.
- Preserve the now-working save/Pak path and avoid a forced gamestate skip.

Current evidence:

- `gs=4` is no longer the primary blocker:
  - it is the save/Controller Pak screen;
  - the game can leave it naturally when a valid note exists;
  - on an empty Pak it waits until the user chooses the create/use path;
  - debug auto-input can exercise that path and reach `gs=12`.
- `gs=12` is the Expansion Pak screen (confirmed visually by the user).
- In a default existing-save 45s run, `gs=12` advanced naturally to `gs=-5` and then `gs=5`.
- `gs=5` currently crashes after NI overlay pair `104` loads at `0x0F000000`.
- Crash sample `/tmp/lod_default_existing_45s_gs12.log`:
  - exit code `139`;
  - `[CRASH] Signal 11 at address 0x4001cad9c`;
  - no missing-function lookup;
  - no stale `LodRecomp` process;
  - last lookups include `0x0F000000`, `0x0F000130`, `0x8001B4C0`, and `0x8001B530`.
- Static inspection identified and renamed `0x0F000000` in NI pair `104` to
  `ni_ovl_104_schedule_dispatch`; it dispatches table `0x0F0026F8`.

Next action:

1. Add a default-off trace around the `gs=5` transition and NI pair `104` entry (`ni_ovl_104_schedule_dispatch`, `0x0F000130`) to capture object pointers/arguments before the crash.
2. Inspect and name the main-code functions in the crash path (`0x8001B4C0`, `0x8001B530`, and nearby `0x8001A5BC/0x8001AA48/0x8001AB28`) once their role is established.
3. Determine whether the crash is an NI pair-104 logic bug, bad object/scene data, stale overlay function mapping, or a renderer/display-list side effect.
4. Keep the default runtime path natural: no permanent skip from `gs=12` or `gs=5`.

## Resolved Work Item

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

Status: active.

Success condition:

- The game survives `gs=5` after the Expansion Pak screen and reaches the next title/intro state through normal scheduling.

### Milestone 4 — Reach Controllable Gameplay

Status: pending.

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

## Open Questions

- What exact flow is `gs=5` in LoD after the Expansion Pak screen: intro cutscene, title intro, or another boot screen?
- Why does NI pair `104` crash after `gs=5` starts?
- Are `0x0F000000` / `0x0F000130` in pair `104` being called with bad object arguments, or is the crash later in main-code handlers?
- What are the roles of `0x8001B4C0`, `0x8001B530`, `0x8001A5BC`, `0x8001AA48`, and `0x8001AB28` in this transition?
- Are remaining default-on PFS/Pak overrides still required after the direct `func_8009F400` shim retirement?
- Are NI pairs `101 / 105 / 120` expected around the save/boot UI after the pair-120 fix, or do they indicate another overlay mismatch later?

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
- Running N64Recomp directly instead of `./tools/regen_recomp.sh`.
- Long unbounded runs during leak-prone experiments.

## Current Next Step

Work item `G5-001`: add a default-off trace for the `gs=5` transition and NI pair `104`, then reproduce the crash with object/argument context. Do not reintroduce a gamestate skip; the natural path now reaches the next blocker.
