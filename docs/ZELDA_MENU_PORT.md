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
- Matched Zelda launcher startup behavior for `LOD_USE_ZELDA_MENU`: an
  auto-discovered ROM is validated/cached at launch, but emulation waits until
  the player presses Start Game in the launcher.
- Launcher/config RML events are registered in the LoD adapter and log concise
  event traces while this branch is experimental.
- On macOS, native ROM selection is dispatched synchronously to the main thread
  so AppKit/NFD is not opened from RT64's render thread.
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
- Validation after launcher wait/event wiring on macOS (2026-06-18):
  - `cmake --build build-zelda-menu -j 8` passes with `LOD_USE_ZELDA_MENU=ON`.
  - `cmake --build build-zelda-default -j 8` passes with `LOD_USE_ZELDA_MENU=OFF`.
  - Interactive launch of `build-zelda-menu/LodRecomp` validates the discovered
    ROM and logs that it is waiting for the launcher Start Game action; no game
    init occurs before the launcher action.
- Next step: visually inspect/menu-drive the Zelda settings UI, then wire LoD
  controls and any real audio options.
