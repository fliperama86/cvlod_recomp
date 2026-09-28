# Changelog

## v0.2.27 - 2026-09-28

- Fixed geometry clipping and popping that appeared with widescreen or high framerate enabled, depending on the camera heading.
- Fixed enemies and items disappearing near the left and right edges of the screen in widescreen.
- Fixed a startup crash on systems without a hardware GPU driver (D3D12 software rendering).
- Updated the RT64 renderer and the N64 runtime to their latest upstream versions, including Metal fixes and fixes for crashes and stalls around exit and timers.
- Added optional testing cheats, configurable in `cheats.cfg` next to the other config files (all off by default).

## v0.2.26 - 2026-07-02

- Fixed the Henry Outer Walls elevator black screen reported in issue #26.
- Generalized the pair-126 transition-lock cleanup so init paths that do not start a fade can recover without map-specific checks.
- Documented the validated issue #26 trace and release behavior.

## v0.2.17 - 2026-06-22

- Replaced the temporary Controls panel with the ZeldaRecomp-style button assignment menu.
- Added keyboard/controller binding display, input scanning, clear/reset actions, and promptfont labels.
- Made gameplay input use the Zelda-style binding arrays in the primary Zelda-menu build.
- Kept `controls.json` backward-compatible while adding a versioned `bindings` section.
- Updated the built-in default controls to match the current tested app controls.
- Kept Toggle Menu on Select/Back only, not controller Start.
