# ZeldaRecomp menu port

Primary UI path: Zelda64Recomp menu framework.

## Intent

Adopt the Zelda64Recomp RmlUi menu framework for LoD's launcher, ROM selection,
settings, and in-game overlay instead of continuing to grow the current
single-file `lod_ui_overlay.cpp` implementation.

## License baseline

The imported menu framework and menu assets come from Zelda64Recomp, which is
licensed under GPL-3.0. This branch therefore treats the combined work as
GPL-3.0 for distribution purposes.

Current imported files:

- `COPYING` — GPL-3.0 license text from Zelda64Recomp.
- `include/recomp_ui.h` — ZeldaRecomp UI public API header.
- `src/ui/ui_*`, `src/ui/core/*`, `src/ui/elements/*`, `src/ui/util/*` —
  ZeldaRecomp menu framework sources.
- `assets/*.rml`, `assets/*.rcss`, `assets/config_menu/*`, `assets/icons/*`,
  `assets/promptfont/*`, and menu fonts — ZeldaRecomp UI assets.
- `include/overloaded.h`, `include/promptfont.h`, `lib/SlotMap/slot_map.h`,
  `lib/concurrentqueue/concurrentqueue.h` — helper headers used by the imported UI.

`LICENSE` and `COPYING` now both contain the GPL-3.0 license text. Before
publishing this branch/release, update README/release notices to call out the
license change explicitly.

## Porting status

- Imported ZeldaRecomp menu framework/assets.
- Added `LOD_USE_ZELDA_MENU`; it now defaults to ON so the ZeldaRecomp menu is
  the primary runtime path. Use `-DLOD_USE_ZELDA_MENU=OFF` for the legacy LoD overlay fallback.
- Added a minimal LoD adapter/launcher/config slice for the Zelda menu framework.
- Added the first real settings slice: Zelda-styled General/Graphics/Controls/Audio shell, with Graphics backed by LoD's active `graphics.json` path and apply/save logic.
- Validation on macOS (2026-06-18):
  - `cmake --build build-zelda-default -j 8` passes with `LOD_USE_ZELDA_MENU=OFF`.
  - `cmake --build build-zelda-menu -j 8` passes with `LOD_USE_ZELDA_MENU=ON`.
  - Bounded smoke run of `build-zelda-menu/LodRecomp` reached startup/game init
    and was terminated by the 20s timeout; no stale process remained.
- Validation after the Graphics settings slice on macOS (2026-06-18):
  - `cmake --build build-zelda-default -j 8` passes with `LOD_USE_ZELDA_MENU=OFF`.
  - `cmake --build build-zelda-menu -j 8` passes with `LOD_USE_ZELDA_MENU=ON`.
  - Bounded smoke run of `build-zelda-menu/LodRecomp` loaded the Zelda UI assets,
    reached startup/game init, and exited by 20s timeout without a stale process.
- Regression recovery on macOS (2026-06-18):
  - Reverted `5a770a1` and `07d3f3d`; the branch tip matches the confirmed-good
    `fb2d1f8` tree plus revert commits.
  - Reintroduced launcher-wait behavior as an opt-in runtime experiment only:
    set `LOD_ZELDA_WAIT_FOR_START=1` to validate/cache a discovered ROM and wait
    for the launcher Start Game action. Default/no-env startup remains the
    confirmed-good auto-start behavior.
  - Added trace-only diagnostics behind `LOD_ZELDA_UI_TRACE=1` for SDL queueing,
    UI event dequeue/dispatch, context capture state, and Rml listener firing.
  - `cmake --build build-zelda-menu -j 8` passes with `LOD_USE_ZELDA_MENU=ON`.
  - `cmake --build build-zelda-default -j 8` passes with `LOD_USE_ZELDA_MENU=OFF`.
- Next step: visually inspect/menu-drive the Zelda settings UI, then wire LoD
  controls and any real audio options.

## 2026-06-21 implementation slice

- Baseline capture:
  - `cmake -S . -B build -DLOD_USE_ZELDA_MENU=OFF` and default build passed.
  - Initial `LOD_USE_ZELDA_MENU=ON` build failed before feature work because the RmlUi SDL backend API changed: `SystemInterface_SDL` now requires an `SDL_Window*`, and `RmlSDL::InputEventHandler` now takes `(context, window, event)`.
- Fixed the Zelda-mode build break in `src/ui/ui_state.cpp` by storing the SDL window in `UIState`, constructing `SystemInterface_SDL(window)`, and passing the window to `RmlSDL::InputEventHandler`.
- Expanded the Zelda-mode LoD launcher/status shell:
  - Shows ROM status and filename-only ROM display.
  - Shows config path.
  - Adds Controls launcher entry.
  - Persists a valid ROM chosen through the Zelda-mode picker to `rom_path.txt`.
  - Keeps Mods absent rather than shipping a placeholder.
- Expanded `assets/lod_config.rml` into a fuller LoD-specific settings shell:
  - General: version, ROM status, config folder, portable mode, background input, rumble/Controller Pak/high-res status, debug visibility, Change ROM.
  - Graphics: window mode, internal resolution, downsample factor, aspect ratio, HUD placement, MSAA, refresh rate/manual refresh, high precision framebuffer, read-only API/display refresh, Apply, Discard, Reset Defaults.
  - Controls: intentionally truthful read-only status until the real rebinding backend is implemented.
  - Audio: real Master Volume and Mute controls only, plus read-only SDL/backend/queue/audio RSP status.
  - Debug: developer-gated with `RECOMP_UI_SHOW_DEBUG=1`, read-only gamestate, exec flags, map overlay, NI pair, graphics, and config path diagnostics.
- Added `include/lod/lod_audio_config.hpp` and audio settings APIs in `lod::settings`.
- Added `audio.json` with safe default recovery:
  - `master_volume` 0-100.
  - `mute` boolean.
  - Runtime gain is applied in `queue_samples` through atomics, preserving the old 100% output scale.
- Validation:
  - `cmake --build build-zelda-menu --target LodRecomp --parallel` passes.
  - `cmake -S . -B build -DLOD_USE_ZELDA_MENU=OFF && cmake --build build --target LodRecomp --parallel` passes.
  - Bounded Zelda-mode smoke: `RECOMP_UI_OPEN_ON_START=1 LOD_ZELDA_WAIT_FOR_START=1 timeout 10 build-zelda-menu/LodRecomp`, exited by timeout as expected, loaded Zelda UI assets, no stale `LodRecomp` process, no startup audio-control save spam after setter guards.
  - Bounded default smoke: `RECOMP_UI_OPEN_ON_START=1 timeout 10 build/LodRecomp`, exited by timeout as expected, auto-started ROM, initialized runtime/audio/UI, no stale `LodRecomp` process.
- Remaining major gap: full Controls rebinding backend. Do not present it as complete until keyboard/controller binding arrays, scanning, clear/reset, migration, and `get_n64_input()` integration are implemented and validated.

## 2026-06-21 Zelda polish pass

- Copied the ZeldaRecomp settings-window presentation into `assets/lod_config.rml`:
  - Native `tabset` tabs and panels instead of LoD's temporary button-tab shell.
  - Centered modal backdrop with click-outside close behavior.
  - Zelda-style top-right Quit and Close icon buttons.
  - Footer navigation help labels backed by `nav_help_model`.
- Added input-aware footer prompt bindings in `src/ui/lod_zelda_menu.cpp`:
  - Keyboard shows arrows plus Tab, Enter, and Escape promptfont glyphs.
  - Controller shows D-pad plus the active Accept and Toggle Menu controller bindings as promptfont glyphs.
  - Footer labels dirty when `config_menu_set_cont_or_kb` reports keyboard/controller changes.
- Routed launcher Quit and settings Quit through the Zelda choice prompt instead of exiting immediately.
- Implemented LoD's `get_config_tabset()` and active-tab routing so launcher Settings/Controls and tab clicks use the real RmlUi tabset.
- Validation:
  - `cmake --build build-zelda-menu --target LodRecomp --parallel` passes.
  - `cmake -S . -B build -DLOD_USE_ZELDA_MENU=OFF && cmake --build build --target LodRecomp --parallel` passes.
  - Bounded Zelda-mode smoke: `RECOMP_UI_OPEN_ON_START=1 LOD_ZELDA_WAIT_FOR_START=1 timeout 10 build-zelda-menu/LodRecomp`, exited with status 124 from timeout, loaded Zelda UI assets and PromptFont, no stale `LodRecomp` process.
  - Bounded default smoke: `RECOMP_UI_OPEN_ON_START=1 timeout 10 build/LodRecomp`, exited with status 124 from timeout, auto-started the ROM and initialized runtime/audio/UI, no stale `LodRecomp` process.

## 2026-06-21 macOS app-bundle launch fix

- Root cause for `open ./build/LodRecomp.app` not showing the Zelda launcher:
  - The local bundle had been packaged from `build/LodRecomp`, a default `LOD_USE_ZELDA_MENU=OFF` binary, instead of `build-zelda-menu/LodRecomp`.
  - `.app` launches do not preserve the shell cwd, so Zelda menu assets were looked up under the launch cwd instead of the bundle resources folder.
  - ROM discovery did not search the active portable/config folder for LoD-named ROMs such as `build/castlevania2.n64.us.z64`.
- Fixes:
  - `make_app.sh` now bundles the Zelda UI asset subset under `Contents/Resources/assets`.
  - Zelda asset lookup now prefers `Contents/Resources/assets` on macOS, then falls back to `cwd/assets`.
  - ROM discovery now searches the active config/portable directory and the macOS bundle parent for LoD-named ROMs before cwd/resources fallback.
  - Zelda-menu builds now wait at the launcher by default. Set `LOD_ZELDA_WAIT_FOR_START=0` to opt back into auto-start.
- Validation:
  - Rebuilt `build-zelda-menu/LodRecomp`.
  - Packaged `build/LodRecomp.app` from the Zelda binary.
  - Bounded bundle smoke from `/tmp` without env vars found portable config at `build/`, found `build/castlevania2.n64.us.z64`, loaded fonts from `build/LodRecomp.app/Contents/Resources/assets`, waited at the Zelda launcher, and exited with timeout status 124 with no stale process.

## 2026-06-21 launcher polish slice

- `assets/lod_launcher.rml` now follows the imported Zelda launcher presentation more closely:
  - Adds the animated/faded right-side background wrapper using the LoD app icon.
  - Uses Zelda-style launcher labels: `Start game`, `Setup controls`, and `Exit`.
  - Follow-up release polish removed the LoD-specific ROM status, selected ROM filename, Change ROM, and config path readouts so the launcher matches ZeldaRecomp's minimal presentation more closely.
- `make_app.sh` now bundles PNG assets into `Contents/Resources/assets` so the launcher background is available in `.app` launches.
- `make_app.sh` strips extended attributes before signing, preventing copied PNG metadata from breaking ad-hoc codesign.
- Validation:
  - Regenerated `build/LodRecomp.app` from `build-zelda-menu/LodRecomp`.
  - `codesign --verify --verbose=2 build/LodRecomp.app` passes.
  - Bounded bundle smoke from `/tmp` exits with timeout status 124, finds the build portable ROM, loads bundled Zelda assets, and leaves no stale process.

## 2026-06-21 full visual-copy settings pass

- `assets/lod_config.rml` now copies the imported Zelda config panel structure much more closely while keeping LoD-specific truthful settings:
  - Each tab uses Zelda-style `config__form`, `config__hz-wrapper`, `config__wrapper`, `config-option`, hover/focus `set_cur_config_index`, and right-side description panes.
  - General, Controls, Graphics, Sound, and Debug panels use the same visual classes and tab/window/footer chrome as ZeldaRecomp.
  - The Sound tab label now matches ZeldaRecomp naming.
  - Graphics keeps LoD's real renderer settings and staged Apply flow, with a Zelda-style single secondary Apply footer button and an input-aware Apply prompt glyph.
- `src/ui/lod_zelda_menu.cpp` now binds `cur_config_index`, `set_cur_config_index`, and `gfx_help__apply` for Zelda-style descriptions and input-aware Apply labels.
- Controls remain intentionally read-only and truthful until the real rebinding backend is ported. Do not present rebinding as complete until the versioned binding arrays, scanning, clear/reset actions, persistence migration, and `get_n64_input()` integration are implemented and validated.
- Validation:
  - `git diff --check` passes.
  - `cmake --build build-zelda-menu --target LodRecomp --parallel` passes.
  - Regenerated `build/LodRecomp.app` from `build-zelda-menu/LodRecomp`; `codesign --verify --verbose=2 build/LodRecomp.app` passes.
  - Bounded bundle smoke from `/tmp` exits with timeout status 124, finds the portable build ROM, loads bundled Zelda assets and PromptFont, waits at the Zelda launcher, and leaves no stale `LodRecomp` process.
  - Bounded config smoke with `RECOMP_UI_OPEN_ON_START=1 LOD_ZELDA_WAIT_FOR_START=1 timeout 10 build-zelda-menu/LodRecomp` exits with timeout status 124, loads Zelda menu assets and PromptFont, and leaves no stale `LodRecomp` process.
  - `cmake -S . -B build -DLOD_USE_ZELDA_MENU=OFF -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake --build build --target LodRecomp --parallel` passes.
- 2026-06-21 follow-up: the Graphics footer was tightened to a single Apply button because the extra Discard and Reset Defaults buttons stacked vertically at runtime instead of matching ZeldaRecomp's footer layout.

## 2026-06-21 release polish pass

- Removed release-facing ROM/status clutter from the Zelda-mode LoD shell while keeping internal ROM discovery, validation, and persistence intact:
  - Launcher now follows the ZeldaRecomp list shape: Select ROM only when no valid ROM is cached, Start game when valid, Setup controls, Settings, Exit.
  - General settings no longer shows ROM status, ROM filename, Change ROM, config folder, or ROM-specific descriptions.
- Cleaned release-facing settings copy:
  - Controls no longer says `Not wired yet`; it now presents controller, keyboard, and automatic prompt status without exposing unfinished rebinding internals.
  - Sound no longer shows audio config path, backend, queue, or save-status diagnostics.
  - Debug remains hidden unless `RECOMP_UI_SHOW_DEBUG=1` is set.
- Added `tools/smoke_zelda_release.sh` for repeatable local release validation. It configures the primary default build, builds `LodRecomp`, regenerates `build/LodRecomp.app`, verifies codesign, runs bounded bundle and config-open smokes, and checks for stale `LodRecomp` processes.
- Validation:
  - `git diff --check` passes.
  - `tools/smoke_zelda_release.sh` passes: primary default build succeeds, app bundle signs and verifies, bundle smoke exits with timeout status 124, config-open smoke exits with timeout status 124, and no stale `LodRecomp` process remains.

## 2026-06-21 primary-build default flip

- `LOD_USE_ZELDA_MENU` now defaults to `ON` in `CMakeLists.txt`, making the ZeldaRecomp-style menu the primary build path for release packaging.
- The legacy LoD overlay remains available for fallback/debug builds with `-DLOD_USE_ZELDA_MENU=OFF`.
- README and the definitive menu plan were updated to describe the ZeldaRecomp menu as primary instead of experimental.
- `tools/smoke_zelda_release.sh` now validates the primary default `build/` path and clears stale `LOD_USE_ZELDA_MENU` cache entries before configuring so local old OFF caches do not mask the new default.
- Keep release notes explicit about known gaps: controls rebinding is still not the full ZeldaRecomp binding editor.
- Validation:
  - `git diff --check` passes.
  - `tools/smoke_zelda_release.sh` passes against the default `build/` path; `cmake -LAH build` reports `LOD_USE_ZELDA_MENU:BOOL=ON`; bundled app and config-open smokes both exit with timeout status 124 and leave no stale process.
  - Legacy fallback build passes with `cmake -S . -B build-legacy-overlay -DLOD_USE_ZELDA_MENU=OFF -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake --build build-legacy-overlay --target LodRecomp --parallel`.
