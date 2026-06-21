# Definitive Zelda-Style Menu Plan

Date: 2026-06-21

## Purpose

Build a definitive LoD settings and launcher UI using the polished Zelda64Recomp menu framework, while keeping all visible options truthful, LoD-specific, and backed by real runtime behavior.

The current normal build uses the legacy LoD overlay. The experimental `LOD_USE_ZELDA_MENU=ON` path uses the Zelda UI framework, but currently loads a small custom LoD page instead of the full Zelda-style tabbed settings menu. This plan defines how to replace that minimal page with a complete, polished, maintainable LoD menu.

## Current Findings

- `LOD_USE_ZELDA_MENU` defaults to `OFF`.
- The full Zelda64Recomp menu assets are already present:
  - `assets/config_menu.rml`
  - `assets/config_menu/general.rml`
  - `assets/config_menu/graphics.rml`
  - `assets/config_menu/controls.rml`
  - `assets/config_menu/sound.rml`
  - `assets/config_menu/debug.rml`
  - shared RmlUi style/assets under `assets/`
- The current Zelda-mode LoD adapter loads:
  - `assets/lod_launcher.rml`
  - `assets/lod_config.rml`
- `assets/lod_config.rml` is much smaller than the full Zelda config menu.
- `src/ui/lod_zelda_menu.cpp` provides a minimal launcher/config adapter and many stubs:
  - input scanning is stubbed
  - config save is stubbed
  - mods are stubbed
  - debug tab is stubbed
  - config submenu is stubbed
  - tabset helpers are stubbed
- The legacy default overlay has more actual LoD graphics options than the minimal Zelda-mode page, but is monolithic and visually less polished.
- The goal is not to expose Zelda-specific options. The goal is to adopt Zelda's polish and structure with LoD-specific settings.

## Target Architecture

Use Zelda64Recomp's UI framework as the long-term UI foundation.

Keep or adapt:

- `src/ui/ui_renderer.cpp`
- `src/ui/ui_state.cpp`
- `src/ui/ui_prompt.cpp`
- `src/ui/ui_color_hack.cpp`
- `src/ui/ui_rml_hacks.cpp`
- `src/ui/core/*`
- `src/ui/elements/*`
- prompt glyph/footer system
- RmlUi context management
- Zelda-style tabset and prompt flow

Replace or fork:

- Zelda-specific config models
- Zelda-specific launcher text and art
- Zelda-specific General tab options
- Zelda-specific Audio tab options
- Zelda-specific Debug actions

Create LoD-specific modules:

- `src/ui/lod_menu.cpp`
- `src/ui/lod_menu_launcher.cpp`
- `src/ui/lod_menu_config.cpp`
- `src/ui/lod_menu_graphics.cpp`
- `src/ui/lod_menu_controls.cpp`
- `src/ui/lod_menu_audio.cpp`
- `src/ui/lod_menu_debug.cpp`
- `include/lod/lod_menu_config.hpp`
- `include/lod/lod_input_config.hpp`
- `include/lod/lod_audio_config.hpp`

Keep the new path behind `LOD_USE_ZELDA_MENU=ON` until it reaches parity, then flip it to default in a dedicated commit.

## Target Menu Map

### Launcher

The launcher should be LoD-branded and Zelda-polished.

Required entries:

- Start Game
- Select ROM / Change ROM
- Settings
- Controls
- Mods, only if fully functional
- Quit

Required status display:

- Version
- ROM status
- ROM filename only, never full path
- Validation result
- Config path or portable mode status

Required behavior:

- Works when no ROM is present.
- Works when an invalid ROM is selected.
- Valid ROM can be selected from native file picker.
- Valid ROM path is persisted.
- Start Game is disabled or guarded until the ROM is valid.
- No blocking OS dialog should prevent the in-window UI from staying usable.

### General

LoD-specific, truthful settings and status:

- Version
- ROM status
- Change ROM
- Config folder
- Portable mode status
- Background input
- Rumble strength
- Controller Pak/save support status
- Expansion Pak/native high-res build status, read-only unless made runtime-safe
- Debug tab visibility status

Avoid Zelda-only options:

- Autosave
- Targeting mode
- Analog camera
- Aiming camera mode
- Low-health beeps

Add LoD equivalents only after the corresponding runtime feature exists.

### Graphics

This tab must cover every current LoD graphics setting, with the Zelda UI's polish.

Required options:

- Window Mode
- Internal Resolution
- Downsample Factor
- Aspect Ratio
- HUD Placement
- MS Anti-Aliasing
- Refresh Rate
- Manual Refresh Rate
- High Precision Framebuffer
- Graphics API, read-only or restart-required
- Display refresh rate, read-only
- Supported MSAA indicators
- Apply
- Discard
- Reset Defaults

Required behavior:

- Defaults remain conservative.
- Aspect Ratio defaults to Original.
- Refresh Rate defaults to Original.
- Users can override Aspect Ratio and Refresh Rate.
- Opening settings copies active renderer config to pending config.
- UI edits mutate pending config only.
- Apply writes renderer config and persists `graphics.json`.
- Discard restores active renderer values.
- Close with pending changes opens a prompt.
- F5/F6/F7/F8/F11 outside the menu still apply immediately and save.
- F5/F6/F7/F8/F11 inside the menu stage pending changes.
- Manual refresh slider is disabled unless Refresh Rate is Manual.
- Downsample controls are disabled or explained when resolution mode makes them irrelevant.

### Controls

Controls must become fully interactive, not a read-only summary.

Required pages/features:

- Controller/keyboard device toggle
- Rebind N64 A
- Rebind N64 B
- Rebind N64 Z
- Rebind N64 Start
- Rebind N64 L
- Rebind N64 R
- Rebind C-Up, C-Down, C-Left, C-Right
- Rebind D-pad directions
- Rebind analog stick directions
- Rebind menu actions:
  - Toggle Menu
  - Accept
  - Apply
- Clear binding
- Reset one binding
- Reset all bindings
- Trigger threshold
- Right stick to D-pad mode
- Right stick invert X
- Right stick invert Y
- Right stick deadzone
- Connected controller name
- Binding conflict policy
- Scan cancel path

Required backend work:

- Replace hardcoded keyboard mappings in `get_n64_input()`.
- Convert current `ControlsConfig` into binding arrays compatible with the Zelda UI controls page.
- Support keyboard and controller binding persistence.
- Preserve current defaults.
- Keep reading the current `controls.json` format for migration.
- Write a versioned controls schema after save.
- Implement real input scanning:
  - `recomp::start_scanning_input`
  - `recomp::stop_scanning_input`
  - `recomp::finish_scanning_input`
  - `recomp::cancel_scanning_input`
  - `recomp::get_scanned_input`
  - `recomp::get_scanned_input_index`
  - `recomp::get_input_binding`
  - `recomp::set_input_binding`

### Audio

Only expose real controls.

Minimum viable controls:

- Master Volume
- Mute

Read-only status:

- SDL audio driver
- Output sample rate
- Output channels
- Queue/backend status
- Audio RSP path status

Do not expose fake BGM/SFX sliders. Add them only after LoD has real mixer or voice-group hooks.

Required backend work:

- Add `audio.json`.
- Add master volume 0-100.
- Add mute.
- Apply attenuation in `queue_samples`.
- Persist audio settings.
- Recover safely from invalid audio config.

### Mods

Choose one of two outcomes:

1. Fully wire and validate the imported mod UI.
2. Hide the Mods tab entirely.

Do not ship a dead or placeholder Mods tab.

If fully wired, adapt and validate:

- `src/ui/ui_mod_menu.cpp`
- `src/ui/ui_mod_details_panel.cpp`
- `src/ui/ui_mod_installer.cpp`
- `src/ui/ui_api.cpp`
- `src/ui/ui_api_events.cpp`
- `src/ui/ui_api_images.cpp`

Validation must cover:

- mod list loading
- enable/disable
- configure
- install/import flow, if supported
- persistence
- no ROM path or private data leaks

### Debug

Developer-gated only.

Gate with:

- `RECOMP_UI_SHOW_DEBUG=1`
- optional compile-time debug flags for risky features

Show read-only diagnostics first:

- gamestate
- exec flags
- map overlay ROM offset
- map overlay size
- map overlay load count
- current renderer backend
- current graphics config
- current config path
- ROM validation/cache status
- build/version info
- selected compile-time feature flags

Avoid runtime toggles for risky compile-time experiments unless they are truly runtime-safe.

## Implementation Phases

### Phase 1: Baseline Capture

Build and capture the current state before changing behavior.

Commands:

```sh
cmake -S . -B build -DLOD_USE_ZELDA_MENU=OFF
cmake --build build --target LodRecomp --parallel

cmake -S . -B build-zelda-menu -DLOD_USE_ZELDA_MENU=ON
cmake --build build-zelda-menu --target LodRecomp --parallel
```

Capture screenshots for:

- default overlay launcher/no-ROM path
- default overlay settings
- current Zelda-mode launcher
- current Zelda-mode settings
- prompts
- ROM setup states

Deliverable:

- Update `docs/ZELDA_MENU_PORT.md` with a concise visual and functional gap checklist.

Acceptance:

- Both menu paths build.
- Current gaps are documented before implementation begins.

### Phase 2: Full LoD Zelda Menu Shell

Replace `assets/lod_config.rml` as the primary Zelda-mode settings page with a full tabbed config shell.

Tasks:

- Create LoD-specific full config RML or fork `assets/config_menu.rml`.
- Use tabset navigation.
- Use footer nav help.
- Use standard Zelda-style prompts.
- Add LoD tabs:
  - General
  - Graphics
  - Controls
  - Audio
  - Mods, only if functional
  - Debug, developer-gated
- Compile the required full framework sources in `CMakeLists.txt`.
- Replace `LodConfigMenu` stubs with real tabset helpers.

Acceptance:

- `LOD_USE_ZELDA_MENU=ON` opens a polished tabbed LoD settings menu.
- No Zelda-specific labels remain visible.
- Existing ROM setup still works.

### Phase 3: Graphics Parity

Wire the Graphics tab completely.

Tasks:

- Port current default overlay graphics options into the Zelda-style `graphics_model`.
- Bind all renderer options to a pending `GraphicsConfig`.
- Add descriptions and disabled states.
- Add Apply, Discard, Reset Defaults.
- Keep Aspect Ratio and Refresh Rate override behavior from v0.2.11.
- Add `graphics.json` save through the existing `lod::settings::apply_and_save_graphics_config`.
- Add pending-state hotkey behavior while the menu is open.

Acceptance:

- Zelda-mode Graphics has every option from the default overlay.
- Saved config survives restart.
- Defaults remain conservative.
- User overrides are preserved.

### Phase 4: Controls Rebinding Backend

Implement real controls rebinding.

Tasks:

- Define LoD action names for every N64 input.
- Implement keyboard and controller `InputField` support.
- Migrate current `ControlsConfig` to the binding-array model.
- Keep backward compatibility for existing `controls.json`.
- Implement input scanning.
- Rework `get_n64_input()` to use configured bindings.
- Keep current defaults exactly unless intentionally changed.

Acceptance:

- Users can rebind every game input.
- Users can rebind menu inputs.
- Reset one and reset all work.
- Clear binding works.
- Invalid config recovers to defaults.
- Existing `controls.json` still loads.

### Phase 5: Audio Settings

Add real, minimal audio controls.

Tasks:

- Add `AudioConfig`.
- Add `audio.json`.
- Add Master Volume.
- Add Mute.
- Apply gain in `queue_samples`.
- Expose audio backend read-only status.
- Add invalid-config recovery.

Acceptance:

- Master volume audibly changes output.
- Mute works.
- Settings persist.
- No fake BGM/SFX sliders are shown.

### Phase 6: Launcher and ROM Setup Polish

Bring the launcher and ROM setup up to Zelda-level polish.

Tasks:

- Improve LoD-branded launcher layout.
- Add config path and version status.
- Add ROM status panel.
- Add Change ROM from General.
- Keep filename-only display.
- Keep validation nonblocking.
- Preserve first-run no-ROM behavior.
- Add help prompt with exact ROM requirements.

Acceptance:

- No-ROM, invalid-ROM, and valid-ROM flows are polished and usable.
- Valid ROM can be selected and persisted.
- Start Game is clearly gated by ROM validity.

### Phase 7: Mods Decision

Decide whether the Mods tab is functional enough to ship.

Option A: hide Mods.

- Remove launcher Mods entry.
- Remove tab.
- Leave no placeholder.

Option B: fully wire Mods.

- Compile mod UI sources.
- Wire to librecomp mod APIs.
- Validate list, enable, configure, install.
- Add persistence tests.

Acceptance:

- Mods is either absent or fully functional.

### Phase 8: LoD Debug Tab

Add a developer-only Debug tab.

Tasks:

- Gate with `RECOMP_UI_SHOW_DEBUG=1`.
- Add read-only runtime diagnostics.
- Add safe refresh behavior.
- Avoid risky runtime toggles.

Acceptance:

- Debug tab is hidden for normal users.
- Debug tab is useful for developers.
- No unsafe compile-time experiment is exposed as a live toggle.

### Phase 9: Visual Polish

Bring the UI to the expected quality level.

Tasks:

- Polish tab spacing, page hierarchy, and descriptions.
- Add correct focus order for keyboard.
- Add correct controller navigation.
- Add disabled-state styling.
- Add scroll behavior for small windows.
- Add consistent prompt glyphs.
- Test 4:3 and 16:9 windows.
- Test high-DPI window sizes.
- Remove all placeholder copy and warning hacks.

Acceptance:

- Screenshots look coherent across tabs and window sizes.
- Keyboard/controller/mouse navigation all feel intentional.

### Phase 10: Flip Default

Only after parity and validation:

- Flip `LOD_USE_ZELDA_MENU` default to `ON`.
- Keep legacy overlay as fallback for one release.
- Update README and UI docs.
- Release with explicit notes.

Acceptance:

- Default build uses the definitive Zelda-style LoD menu.
- Legacy overlay fallback still builds.
- Release package works on Windows, Linux, and macOS.

## Validation Matrix

Run for each major phase:

```sh
git diff --check
cmake --build build --target LodRecomp --parallel
cmake -S . -B build-zelda-menu -DLOD_USE_ZELDA_MENU=ON
cmake --build build-zelda-menu --target LodRecomp --parallel
```

Runtime validation:

- no-ROM startup
- invalid-ROM selection
- valid-ROM selection
- launcher Start Game
- open settings from launcher
- open settings during gameplay
- close with no changes
- close with pending changes
- Apply graphics
- Discard graphics
- Reset graphics defaults
- F5/F6/F7/F8/F11 with menu closed
- F5/F6/F7/F8/F11 with menu open
- keyboard navigation
- controller navigation
- mouse interaction
- input rebind scan
- input rebind cancel
- input clear
- input reset one
- input reset all
- corrupt `graphics.json`
- corrupt `controls.json`
- corrupt `audio.json`
- fullscreen/windowed transition
- aspect override persistence
- refresh override persistence
- audio volume persistence
- debug tab hidden by default
- debug tab visible with env flag
- no stale `LodRecomp` process after bounded runs

macOS runtime rule:

```sh
codesign -s - --entitlements .github/macos/entitlements.plist -f build/LodRecomp
timeout 20 build/LodRecomp
pgrep -la LodRecomp
```

## Suggested Commit Sequence

1. `Add full LoD Zelda-menu shell`
2. `Port graphics settings to Zelda menu`
3. `Add LoD controls rebinding model`
4. `Add audio settings model`
5. `Polish launcher and ROM setup`
6. `Add LoD debug tab`
7. `Enable Zelda menu by default`

Each commit should build independently and keep unrelated changes out.

## Open Decisions

- Should Mods be hidden until proven functional, or should mod support be part of the first definitive UI milestone?
- Should Graphics API remain read-only, or become a restart-required setting?
- Should old `lod_ui_overlay.cpp` remain as a fallback after the default flips?
- Should audio expose only Master Volume initially, or include separate future placeholders hidden behind backend capability checks?
- Should controller Pak/save status be read-only, or include future save-management UI?

## Initial Recommendation

Do the work in this order:

1. Full tabbed shell with LoD labels.
2. Graphics parity.
3. Controls rebinding.
4. Audio master volume.
5. Launcher/ROM polish.
6. Debug tab.
7. Decide Mods.
8. Flip default.

This order gets the visual polish and the highest-impact settings online early, while avoiding fake configuration pages.
