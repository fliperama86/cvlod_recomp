# Changelog

## v0.2.29 - 2026-09-28

### Android
- **Fixed ROM browser not working** — tapping "Select ROM" now correctly opens the system file picker and stays in a "Waiting..." state while the picker is open. Previously, the launcher immediately showed "ROM selection cancelled" before the file picker result arrived, making it impossible to load a ROM through the UI.
- **Fixed ROM not found on cold-start after first pick** — the app now checks its private storage (`filesDir/rom.z64`) during startup ROM discovery, so a previously picked ROM is found without requiring a re-pick every launch.

Users who previously could not get past the launcher on Android should now be able to tap "Select ROM", choose their `.z64` file from Downloads or any file manager location, and have the game start automatically.

## v0.2.28 - 2026-09-16

- Fixed Issue #28: Added custom gamepad and keyboard binding support for toggling the config menu (`recomp::GameInput::TOGGLE_MENU`).
- Fixed Issue #29: Resolved host crash (`0xC0000005` at `0x80960008`) at Carrie Stage 4 Vampire boss battle by expanding KSEG0 mirror to 256MB and adding RDRAM post-guard range.
- Fixed Issues #27, #31, #33, #35, #36: Fixed Henry transition freezes, black screens, invisible platforms/NPCs (Edward), rock crusher floor, and disabled pause with overlay Pair 129 lifecycle tracking and automatic transition lock release watchdog.
- Fixed RT64 stuck full-screen black overlay during active gameplay transitions.
- Native MSVC and Android compilation support.

## v0.2.27 - 2026-08-22

- Added official Android port with touch controls, on-screen layout customization, cheats menu, splash screen, and audio.

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
