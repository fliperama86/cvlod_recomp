# Changelog

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
