# Recomp UI Implementation Tracker

Status: active implementation tracker for `docs/RECOMP_UI_SPEC.md`  
Created: 2026-06-07  
Scope: original RmlUi/RT64 settings overlay and follow-on launcher, controls, audio, and packaging work

## Current baseline

- Existing overlay is a monolithic RmlUi document rendered through the RT64 render hook in `src/ui/lod_ui_overlay.cpp`.
- Overlay starts hidden and is toggled with `F1`.
- Current ROM startup still exits before window creation when no ROM is found; the spec's overlay-only ROM setup requires a separate boot-flow change.
- UI assets are embedded in code for now, so no third-party UI assets or license files are required for the current slice.

## Implementation plan

| Phase | Spec section | Status | Notes / next action |
|---|---|---:|---|
| 1 | Static settings shell | Implemented | Original embedded themed shell with General/Graphics/Controls/Audio tabs and live graphics values. |
| 2 | Clickable graphics options | Implemented (graphics slice) | Clickable Graphics choices stage pending values; Apply/Discard and reset/close prompts are wired. |
| 3 | Keyboard/controller navigation | Partial | SDL events are queued into RmlUi; Esc/B close; basic controller A/B/D-pad maps to UI keys; gameplay input is suppressed while visible. Full focus polish remains. |
| 4 | Controls menu | Partial | Current controller/keyboard mappings are shown read-only; rebinding scanner and persistence remain planned. |
| 5 | Audio and prompts polish | Partial | Reusable prompt host covers graphics close/reset; Audio tab is truthful read-only until volume controls exist. |
| 6 | Launcher/debug/mods | Planned | Missing-ROM setup needs runtime startup restructuring; debug tab should stay developer-gated. |

## Durable decisions

- Keep the first implementation monolithic in `src/ui/lod_ui_overlay.cpp`; split into `recomp_ui_*` modules after behavior stabilizes.
- Use embedded original markup/style for the first interactive slice to avoid packaging regressions; disk-loaded `assets/ui` can be added later with an asset resolver. Current release packages do not need UI asset copies.
- Existing graphics hotkeys remain supported and continue to apply immediately outside the overlay.
- Close behavior with pending graphics changes: open a confirmation prompt; `F1`/Esc while the prompt is open keeps the overlay open unless the user chooses Discard.

## Verification log

| Date | Change | Verification | Result |
|---|---|---|---|
| 2026-06-07 | Tracker created before UI implementation changes. | `cmake --build build --target LodRecomp --parallel`. | Passed after fixing C++ forward declaration. |
| 2026-06-07 | Interactive embedded settings overlay: tabs, clickable Graphics pending/apply/discard, prompt host, SDL event queue, input capture, read-only Controls/Audio. | Bounded `RECOMP_UI_OPEN_ON_START=1` smoke run; CoreGraphics window screenshot `/tmp/lod_ui_smoke_window.png`; no stale `LodRecomp`; UI log only init/first-draw lines. | Passed. |
| 2026-06-07 | Hidden-by-default behavior. | Short bounded smoke run without `RECOMP_UI_OPEN_ON_START`; checked no first-draw log and no stale `LodRecomp`. | Passed. |
