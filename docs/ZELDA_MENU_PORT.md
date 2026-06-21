# ZeldaRecomp menu port

Experimental branch: `experiment/zelda-menu`.

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
- Added `LOD_USE_ZELDA_MENU` (default OFF) so the existing LoD overlay remains
  the default runtime path.
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
