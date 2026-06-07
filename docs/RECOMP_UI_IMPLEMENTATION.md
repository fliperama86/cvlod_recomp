# Recomp UI Implementation Tracker

Status: active implementation tracker for `docs/RECOMP_UI_SPEC.md`  
Created: 2026-06-07  
Scope: original RmlUi/RT64 settings overlay and follow-on launcher, controls, audio, and packaging work

## Current baseline

- Existing overlay is a monolithic RmlUi document rendered through the RT64 render hook in `src/ui/lod_ui_overlay.cpp`.
- Overlay starts hidden and is toggled with `F1`.
- ROM startup validates discovered/selected ROMs before game launch. Missing or invalid ROMs keep the renderer/UI alive and show an overlay-only ROM setup screen with Select ROM, Retry, Help, and Quit actions.
- UI assets are embedded in code for now, so no third-party UI assets or license files are required for the current slice.
- Completion estimate after the current ROM setup slice: full `RECOMP_UI_SPEC.md` is roughly 50-53% complete; initial settings/graphics overlay is roughly 85% complete; first-run ROM setup is functional but still lacks post-launch Change ROM and prepared-cache metadata UI.

## Implementation plan

| Phase | Spec section | Status | Notes / next action |
|---|---|---:|---|
| 1 | Static settings shell | Implemented | Original embedded themed shell with General/Graphics/Controls/Audio tabs and live graphics values. |
| 2 | Clickable graphics options | Implemented (graphics slice) | Clickable Graphics choices stage pending values; Apply/Discard and reset/close prompts are wired. |
| 3 | Keyboard/controller navigation | Partial+ | SDL events are queued into RmlUi; Esc/B close; controller A/D-pad/left-stick/shoulders map to UI navigation; gameplay input is suppressed while visible. Remaining polish: spatial focus quality and full binding-driven controller semantics. |
| 4 | Controls menu | Partial | Current controller/keyboard mappings plus controller/config metadata are shown read-only; rebinding scanner, duplicate policy, reset/save, and persistence UI remain planned. |
| 5 | Audio and prompts polish | Partial | Reusable prompt host covers graphics close/reset; Audio tab is truthful read-only until volume controls exist. |
| 6 | Launcher/debug/mods | Partial+ | Overlay-only missing/invalid-ROM setup is implemented with native picker validation and persisted selected path after success. Remaining launcher debt: Change ROM from General, prepared-cache metadata/status, and mod management. Developer diagnostics are gated behind `RECOMP_UI_SHOW_DEBUG=1`. |

## Durable decisions

- Keep the first implementation monolithic in `src/ui/lod_ui_overlay.cpp`; split into `recomp_ui_*` modules after behavior stabilizes.
- Use embedded original markup/style for the first interactive slice to avoid packaging regressions; disk-loaded `assets/ui` can be added later with an asset resolver. Current release packages do not need UI asset copies.
- Existing graphics hotkeys remain supported. Outside the overlay they apply immediately; while the overlay is capturing input they mutate pending graphics state and require Apply. If a confirmation prompt is open, graphics hotkeys are consumed without mutating pending settings so prompts remain modal above Settings.
- Close behavior with pending graphics changes: open a confirmation prompt; `F1`/Esc while the prompt is open keeps the overlay open unless the user chooses Discard. The prompt intentionally uses Discard / Keep Editing rather than Apply / Discard so an accidental close path does not apply graphics changes.
- Normal users see General/Graphics/Controls/Audio only. Developer diagnostics are opt-in via `RECOMP_UI_SHOW_DEBUG=1` so compile-time-only debug features are not exposed as live toggles.
- UI capture is gated on renderer/RmlUi readiness. If UI shader/context/document setup fails, gameplay input is not suppressed by an invisible overlay. D3D12/DXIL UI shader blobs are still not implemented, so unsupported renderer shader formats currently disable the overlay gracefully instead of capturing invisibly.
- ROM setup reuses the embedded overlay shell as a separate `ROM Setup` screen. It stays overlay-only until a ROM validates, then starts the game exactly once and hides itself. Automatic discovery avoids plain CV64/reference ROM names and skips decompressed/NI-extended artifacts during resource fallback; explicit command-line/file-picker paths are still validated normally.
- Persist the selected ROM path only after validation succeeds. Current implementation relies on runtime in-memory NI preparation from the validated stock ROM; explicit prepared-cache metadata/status UI is still release debt from spec §41.4.

## Verification log

| Date | Change | Verification | Result |
|---|---|---|---|
| 2026-06-07 | Tracker created before UI implementation changes. | `cmake --build build --target LodRecomp --parallel`. | Passed after fixing C++ forward declaration. |
| 2026-06-07 | Interactive embedded settings overlay: tabs, clickable Graphics pending/apply/discard, prompt host, SDL event queue, input capture, read-only Controls/Audio. | Bounded `RECOMP_UI_OPEN_ON_START=1` smoke run; CoreGraphics window screenshot `/tmp/lod_ui_smoke_window.png`; no stale `LodRecomp`; UI log only init/first-draw lines. | Passed. |
| 2026-06-07 | Hidden-by-default behavior. | Short bounded smoke run without `RECOMP_UI_OPEN_ON_START`; checked no first-draw log and no stale `LodRecomp`. | Passed. |
| 2026-06-07 | Review follow-up: tracker clarified full-spec progress, controller navigation expanded, metadata pages improved, developer-gated Debug tab added. | `git diff --check`; `cmake --build build --target LodRecomp --parallel`; bounded `RECOMP_UI_OPEN_ON_START=1` screenshots with and without `RECOMP_UI_SHOW_DEBUG=1`; hidden-by-default smoke run; no stale `LodRecomp`. | Passed. |
| 2026-06-07 | Review follow-up: invisible-capture failure guard, visible-overlay graphics hotkey staging, live controls config summaries, clearer F1 logging. | `git diff --check`; `cmake --build build --target LodRecomp --parallel`; bounded overlay screenshot smoke; hidden-by-default smoke; F5-with-overlay-open staging smoke confirmed `[UI] staged graphics hotkey in overlay` and no `[CONFIG] F5`; no stale `LodRecomp`. | Passed. |
| 2026-06-07 | Review follow-up: prompts consume graphics hotkeys without hidden mutations; Apply/Discard are disabled when clean; Manual Refresh Rate is disabled unless Refresh Rate is Manual; D3D12/DXIL shader gap and close-prompt choice documented. | `git diff --check`; `cmake --build build --target LodRecomp --parallel`; bounded overlay screenshot smoke with visual disabled-state check; hidden-by-default smoke; F5-with-overlay-open handling smoke confirmed `[UI] handled graphics hotkey in overlay` and no `[CONFIG] F5`; no stale `LodRecomp`. | Passed. |
| 2026-06-07 | Review follow-up: disabled UI elements are filtered out of keyboard/controller focus order, cannot be focused programmatically, and are ignored by focused-click activation. | `git diff --check`; `cmake --build build --target LodRecomp --parallel`; bounded overlay screenshot smoke with disabled-state visual check; hidden-by-default smoke; no stale `LodRecomp`. | Passed. |
| 2026-06-07 | Review follow-up: refused overlay opens after UI init failure clear visible/open-requested state; F1 logging reflects post-toggle visibility; focus retargets when the current focused element is disabled or no longer in focus order; disabled controls skip click listener binding. | `git diff --check`; `cmake --build build --target LodRecomp --parallel`; bounded overlay screenshot smoke; hidden-by-default smoke; F5 overlay hotkey handling smoke; bounded F1-open smoke confirmed `[UI] F1 overlay shown` and first draw; no stale `LodRecomp`. | Passed. |
| 2026-06-07 | Full ROM setup slice: startup no longer exits for missing/invalid ROMs; overlay-only ROM setup screen, native picker request flow, validation status, retry/help/quit actions, and post-validation path persistence were added. | `cmake --build build --target LodRecomp --parallel`; bounded normal startup smoke validated stored ROM and started game; bounded invalid CLI ROM smoke showed validation failure and kept app alive until timeout; no stale `LodRecomp`. | Passed. |
