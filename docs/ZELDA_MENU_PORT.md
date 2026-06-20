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
- Added `LOD_USE_ZELDA_MENU`; it now defaults ON so the ZeldaRecomp menu is the
  default runtime path, while `-DLOD_USE_ZELDA_MENU=OFF` keeps the legacy LoD overlay available.
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
- Windows validation (2026-06-20):
  - Regenerated `RecompiledFuncs/` with the documented Windows N64Recomp
    post-processing chain after stale generated symbols caused a link failure.
  - `python tools\audit_indirect_targets.py --scan-call-tables --fail-on-missing`
    passes.
  - `cmake -S . -B build-win -DLOD_USE_ZELDA_MENU=ON` and
    `cmake --build build-win --config RelWithDebInfo --target LodRecomp` pass.
  - The Zelda renderer now treats DXIL UI shader headers as optional; without
    generated DXIL headers it builds and reports unsupported shader format at
    runtime instead of failing compilation.
- Next step: visually inspect/menu-drive the Zelda settings UI, then wire LoD
  controls and any real audio options.
