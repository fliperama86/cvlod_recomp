# Recompilation Runtime UI Specification

Status: standalone architecture and implementation specification  
Created: 2026-06-07  
Audience: engineers implementing an original UI for an N64 recompilation runtime  
Scope: RmlUi + RT64/renderer-hook settings UI, launcher, prompts, input binding, and release packaging

## 1. Purpose

This document specifies a complete, original UI architecture for a game recompilation runtime. It is intended to be self-contained: an engineer should be able to build a similar solution without access to any other project's source code.

The UI should provide:

- A first-run ROM setup screen when no valid ROM is available.
- A polished in-game settings overlay.
- Controller-, keyboard-, and mouse-friendly navigation.
- Graphics, controls, audio, general, and debug settings.
- Prompt and notification dialogs.
- Clean render integration over the game frame.
- Cross-platform behavior on macOS, Windows, and Linux.
- A maintainable separation between rendering, UI state, assets, data binding, events, and project-specific settings.

This is a specification, not a code drop. All implementation code, markup, styles, fonts, and icons should be original to this project or covered by explicitly compatible licenses.

## 2. Design principles

1. **Original implementation** — Do not copy third-party UI code, styles, markup, fonts, icons, or generated assets unless their license is explicitly accepted and recorded.
2. **Small public API** — Game/runtime code should interact with the UI through a small stable interface.
3. **Hidden during gameplay** — The UI should not interfere with boot or gameplay unless the user opens it. The exception is first-run ROM setup: if no valid ROM is available, the app should boot into an overlay-only setup screen instead of exiting.
4. **Controller first, mouse friendly** — Menus must be usable from a gamepad, keyboard, and mouse.
5. **Low-noise diagnostics** — UI logs should be useful and sparse.
6. **Graceful failure** — If UI initialization or asset loading fails, the game should continue running.
7. **Incremental rollout** — Start with a static overlay, then add interactivity, then controls rebinding and dialogs.
8. **Portable release UX** — Release artifacts should include all UI assets or embed them.
9. **No boot-flow hacks in UI** — UI code should not force game state transitions. Runtime compatibility fixes belong in runtime/game integration code.
10. **Live document** — Update this spec as the UI architecture changes.

## 3. Non-goals for the first implementation

The initial UI does not need:

- A full mod manager.
- A launcher-first experience beyond the required ROM setup screen.
- A custom UI component framework.
- A SCSS/build-time style pipeline.
- Runtime editing of compile-time diagnostic flags.
- Fancy icon glyphs or custom fonts.
- Deep per-game debug tools exposed to normal users.

Those can be added later after the settings overlay is stable.

## 4. High-level architecture

A mature recompilation UI should be organized into cooperating layers:

```text
Application / platform loop
  ├─ polls platform and game input
  ├─ queues UI-relevant events
  ├─ handles global UI hotkeys
  └─ asks UI whether gameplay input should be suppressed

Renderer / RT64 context
  ├─ renders the game frame
  └─ invokes UI render hooks after the game frame is available

UI system
  ├─ owns RmlUi initialization and shutdown
  ├─ owns RmlUi context(s) and documents
  ├─ owns UI renderer adapter
  ├─ owns system interface / asset resolver
  ├─ owns data model handles
  ├─ owns event/action dispatch
  ├─ owns active screen, active tab, focus, and capture state
  ├─ drains queued events
  ├─ updates data models
  └─ renders RmlUi

UI documents and assets
  ├─ settings markup
  ├─ prompt markup
  ├─ stylesheets
  ├─ optional fonts
  ├─ optional icons
  └─ license notes for any third-party assets

Project adapters
  ├─ graphics config adapter
  ├─ controls config adapter
  ├─ audio config adapter
  ├─ debug/telemetry adapter
  └─ persistence adapter
```

## 5. Recommended source tree

A robust implementation can use this layout:

```text
src/ui/
  recomp_ui.h                    # public API used by main/render/input code
  recomp_ui.cpp                  # top-level orchestration
  recomp_ui_state.h/.cpp         # state machine, visibility, contexts, event queue
  recomp_ui_renderer.h/.cpp      # RmlUi -> RT64 renderer adapter
  recomp_ui_assets.h/.cpp        # locate/load/embed UI assets
  recomp_ui_events.h/.cpp        # event listener / action dispatch
  recomp_ui_models.h/.cpp        # RmlUi model registration and dirtying
  recomp_ui_graphics.h/.cpp      # graphics settings model + apply/discard
  recomp_ui_controls.h/.cpp      # controls model + input scanning
  recomp_ui_audio.h/.cpp         # audio model
  recomp_ui_prompt.h/.cpp        # prompt/notification helpers
  recomp_ui_navigation.h/.cpp    # focus/controller navigation helpers

assets/ui/
  settings.rml
  prompt.rml
  settings.rcss
  fonts/                         # optional, permissively licensed only
  icons/                         # optional, original/permissively licensed only
  LICENSES.md                    # required if third-party UI assets are used
```

For an incremental first pass, the project may keep a monolithic overlay source file and add the split modules later.

## 6. Public UI API

Game/runtime code should not know about internal RmlUi documents. Use a small public API:

```cpp
namespace recomp::ui {
    void init_render_hooks();
    void shutdown();

    void toggle_overlay();
    void show_overlay();
    void hide_overlay();
    bool overlay_visible();

    bool captures_input();
    bool captures_mouse();

    void notify_graphics_config_changed();
    void notify_controls_config_changed();
    void notify_audio_config_changed();
}
```

If platform events are routed through the UI system:

```cpp
namespace recomp::ui {
    void queue_platform_event(const PlatformEvent& event);
    void frame_update();
}
```

If SDL is an acceptable public dependency, `queue_platform_event` can accept `SDL_Event`. Otherwise, define a smaller event abstraction:

```cpp
struct PlatformEvent {
    enum class Type {
        KeyDown,
        KeyUp,
        TextInput,
        MouseMove,
        MouseDown,
        MouseUp,
        MouseWheel,
        ControllerButtonDown,
        ControllerButtonUp,
        ControllerAxis,
        DropFile,
        WindowResized,
    };

    Type type;
    int key = 0;
    int button = 0;
    int axis = 0;
    float x = 0.0f;
    float y = 0.0f;
    std::string text;
};
```

## 7. UI state machine

The minimum state machine:

```text
Startup
  ├─ valid ROM available -> Hidden + start game
  └─ missing/invalid ROM -> RomSetup(overlay_only = true)

RomSetup
  ├─ select ROM -> RomSetup(validating)
  ├─ valid ROM -> Hidden + start game
  ├─ invalid ROM -> RomSetup(error_message)
  └─ quit -> exit app

Hidden
  └─ toggle/open -> Settings

Settings
  ├─ close -> Hidden
  ├─ switch tab -> Settings(active_tab = X)
  ├─ open prompt -> Prompt(parent = Settings)
  ├─ apply graphics -> Settings
  └─ close with unsaved changes -> Prompt(parent = Settings)

Prompt
  ├─ confirm -> run confirm action -> parent or Hidden
  ├─ cancel -> run cancel action -> parent
  └─ close -> parent
```

Suggested enums:

```cpp
enum class UiScreen {
    Hidden,
    RomSetup,
    Settings,
    Prompt,
};

enum class SettingsTab {
    General,
    Graphics,
    Controls,
    Audio,
    Debug,
};
```

Suggested state:

```cpp
struct UiState {
    UiScreen screen = UiScreen::Hidden;
    SettingsTab active_tab = SettingsTab::Graphics;
    bool mouse_active = false;
    bool controller_active = true;
    bool captures_input = false;
    bool captures_mouse = false;
    bool pending_graphics_changes = false;
};
```

Rules:

- Hidden UI must not capture input or mouse.
- Settings captures menu/navigation input.
- Prompt captures above settings.
- Closing a prompt should restore focus to the element that opened it, when possible.
- A global toggle key while a prompt is open may close the prompt first or close the whole UI; choose one behavior and document it.

Recommended default behavior:

```text
F1          Open/close settings
Escape/B    Close prompt, back out of submenu, or close settings
Enter/A     Activate focused control
Arrows      Navigate or adjust values
Tab         Next focus
Shift+Tab   Previous focus
Mouse       Click/hover controls
```

## 8. Render-hook lifecycle

The UI should be drawn after the game frame.

### Startup

1. Register renderer hooks.
2. Do not initialize RmlUi until the renderer/device/window are available.

### Hook init

1. Create UI renderer adapter.
2. Create or set RmlUi system interface.
3. Set RmlUi render interface.
4. Initialize RmlUi.
5. Load/register fonts if any.
6. Create RmlUi context using current pixel dimensions.
7. Register event/action dispatcher.
8. Register data models.
9. Load documents.
10. Hide documents by default.

### Every frame

1. Return early if UI is not initialized or hidden.
2. Update RmlUi context dimensions if the window changed size.
3. Drain queued platform events into RmlUi.
4. Refresh dirty data models.
5. Call `context->Update()`.
6. Begin UI render pass.
7. Call `context->Render()`.
8. End UI render pass and composite to swapchain framebuffer.

### Shutdown

1. Unload/hide documents.
2. Destroy RmlUi context(s).
3. Release render resources.
4. Shutdown RmlUi if this module owns it.

## 9. RmlUi render adapter specification

A render adapter bridges RmlUi draw calls into the runtime renderer.

It must handle:

- RmlUi vertex format:
  - position: two floats.
  - color: RGBA8.
  - texture coordinate: two floats.
- Dynamic vertex buffers.
- Dynamic index buffers.
- Texture upload for fonts/images.
- Texture descriptors / descriptor sets.
- Scissor rectangles.
- Transform/projection constants.
- Alpha blending.
- Render target layout transitions/barriers.
- Shader format selection:
  - SPIR-V for Vulkan-style paths.
  - Metal shader blobs for macOS.
  - DXIL shader blobs for Windows D3D12 paths.
- Resize handling.
- Safe resource lifetime for in-flight frames.

A first implementation can reuse the existing project overlay renderer if it already draws RmlUi correctly. Extract into a dedicated renderer class only when needed.

## 10. System interface and asset resolver

RmlUi needs a system interface for time, logging, and optionally clipboard/cursor/file loading. The project should also provide an asset resolver.

Asset search order should support:

### Development checkout

```text
./assets/ui
```

### macOS app bundle

```text
<app>/Contents/Resources/ui
```

### Windows portable zip

```text
<exe-dir>/ui
<exe-dir>/assets/ui
```

### Linux install/run folder

```text
./assets/ui
./ui
<prefix>/share/<app>/ui
```

If required assets are missing:

- Log the selected search paths.
- Log the missing file.
- Disable UI gracefully or show a minimal built-in fallback.
- Do not crash the game.

## 11. Asset loading strategies

### 11.1 Load from disk

Best for early development.

Pros:

- Fast iteration.
- Easy to inspect and edit.
- No asset generator needed.

Cons:

- Packaging must include assets.
- Missing files can break UI if not handled.

### 11.2 Embed as generated byte arrays

Best for stable releases.

Pros:

- Fewer release packaging mistakes.
- Single binary or app-bundle code path.

Cons:

- Requires generator script.
- Rebuild required after UI edits.

### 11.3 Hybrid

Recommended long term:

- Development loads from disk.
- Release falls back to embedded assets.
- Environment variable can override asset root for debugging.

Suggested environment variable:

```text
RECOMP_UI_ASSET_ROOT=/path/to/assets/ui
```

## 12. Release packaging requirements

If assets are disk-loaded, packaging must include them.

### macOS

Expected layout:

```text
AppName.app/
  Contents/
    MacOS/AppName
    Resources/ui/settings.rml
    Resources/ui/settings.rcss
    Resources/ui/prompt.rml
```

Package script should copy assets before signing the app bundle.

### Windows

Expected zip layout:

```text
AppName.exe
SDL2.dll
...
ui/settings.rml
ui/settings.rcss
ui/prompt.rml
README.txt
```

### Linux

For portable builds:

```text
AppName
ui/settings.rml
ui/settings.rcss
```

For installed builds:

```text
bin/AppName
share/AppName/ui/settings.rml
share/AppName/ui/settings.rcss
```

## 13. Data model architecture

Use RmlUi data models to keep markup declarative and C++ state authoritative.

Recommended models:

```text
nav_model
settings_model
graphics_model
controls_model
audio_model
debug_model
prompt_model
```

### 13.1 `nav_model`

Purpose: footer/help text and active input device.

Variables:

```text
active_device
help_navigate
help_accept
help_back
help_apply
```

Example values:

```text
active_device = "controller"
help_navigate = "D-pad / Left Stick"
help_accept = "A"
help_back = "B"
help_apply = "Y"
```

### 13.2 `settings_model`

Purpose: active tab and global settings-window actions.

Variables:

```text
active_tab
version_string
show_debug_tab
rom_status
```

Callbacks:

```text
set_tab(tab_name)
close_settings()
open_quit_prompt()
```

### 13.3 `rom_model`

Purpose: ROM discovery, first-run setup, validation status, and launch control.

Variables:

```text
rom_available
rom_status
rom_status_detail
rom_display_name
rom_path_display
rom_error
rom_busy
rom_can_start
rom_can_replace
rom_setup_required
```

Recommended `rom_status` values:

```text
missing
selecting
validating
preparing
ready
invalid
failed
```

Callbacks:

```text
select_rom_file()
clear_rom_path()
retry_rom_load()
start_game_from_rom()
open_rom_help()
```

Rules:

- If no valid ROM is available during startup, the app must still create the window and UI, then show `RomSetup`.
- `RomSetup` is overlay-only: it captures all gameplay input because gameplay has not started.
- Selecting a ROM should use the platform-native file picker when available.
- Accepted file extensions: `.z64`, `.n64`, `.v64`.
- Validation and preparation should update `rom_status` and `rom_status_detail`.
- A valid selected ROM path should be persisted so the user is not asked again on the next launch.
- Invalid or missing paths should never crash or exit immediately; return to `RomSetup` with an actionable error.
- Do not show full filesystem paths by default; expose only basename plus an optional “Show path” or tooltip for debugging.

### 13.4 `graphics_model`

Purpose: renderer settings with pending/apply/discard behavior.

State:

```cpp
struct GraphicsUiState {
    GraphicsConfig active;
    GraphicsConfig pending;
    bool has_pending_changes;
    bool msaa2x_supported;
    bool msaa4x_supported;
    bool msaa8x_supported;
    int display_refresh_rate;
};
```

Variables:

```text
resolution
resolution_options
window_mode
window_mode_options
aspect_ratio
aspect_ratio_options
msaa
msaa_options
refresh_rate
refresh_rate_options
refresh_rate_manual_value
options_changed
native_high_res_status
native_high_res_explanation
```

Callbacks:

```text
set_resolution(value)
set_window_mode(value)
set_aspect_ratio(value)
set_msaa(value)
set_refresh_rate(value)
set_refresh_rate_manual_value(value)
apply_graphics()
discard_graphics()
reset_graphics_defaults()
```

Behavior:

1. Opening settings copies active renderer config to pending config.
2. UI edits mutate pending config only.
3. `options_changed` is true when pending != active.
4. Apply writes pending config to renderer and persists it.
5. Discard copies active config back to pending.
6. Closing with pending changes prompts the user.

### 13.5 `controls_model`

Purpose: display and edit input bindings.

State:

```cpp
struct ControlBindingView {
    std::string action_id;
    std::string display_name;
    std::string binding_1;
    std::string binding_2;
    bool scanning_binding_1;
    bool scanning_binding_2;
};

struct ControlsUiState {
    InputDevice device;
    std::vector<ControlBindingView> bindings;
    int active_action_index;
    int active_binding_slot;
    bool scanning;
};
```

Variables:

```text
device
bindings
active_action_index
active_binding_slot
scanning
connected_controller_name
controls_file_path
```

Callbacks:

```text
select_device(controller|keyboard)
begin_scan(action_id, slot)
cancel_scan()
clear_binding(action_id, slot)
reset_binding(action_id)
reset_all_bindings()
save_controls()
```

Behavior:

- While scanning, gameplay input is suppressed.
- First valid scanned input becomes the binding.
- Escape/B cancels scanning.
- Reset all requires confirmation.
- Duplicate binding policy must be explicit: allow with warning, or reject.

### 13.6 `audio_model`

Purpose: audio status and volume options.

Variables:

```text
audio_available
audio_enabled
master_volume
music_volume
sfx_volume
```

Callbacks:

```text
set_audio_enabled(value)
set_master_volume(value)
set_music_volume(value)
set_sfx_volume(value)
reset_audio_defaults()
```

If no volume control exists yet, show read-only audio status instead of fake sliders.

### 13.7 `debug_model`

Purpose: development-only diagnostics.

Variables:

```text
current_gamestate
current_map_overlay_rom
current_map_overlay_size
current_map_overlay_load_count
ni_0f_pair
ni_0e_pair
os_mem_size_reported
native_high_res_skip_enabled
last_fault_summary
```

Callbacks:

```text
copy_debug_summary_to_clipboard()
open_logs_folder()
```

Do not expose compile-time-only features as live toggles.

## 14. Event/action dispatch

The markup should call named actions. C++ should map action names to handlers.

Conceptual API:

```cpp
using UiAction = std::function<void(std::string_view param, Rml::Event& event)>;

class UiActionRegistry {
public:
    void register_action(std::string name, UiAction action);
    bool dispatch(std::string_view name, std::string_view param, Rml::Event& event);
};
```

Recommended actions:

```text
close_settings
set_tab
select_rom_file
clear_rom_path
retry_rom_load
start_game_from_rom
open_rom_help
apply_graphics
discard_graphics
reset_graphics_defaults
begin_input_scan
cancel_input_scan
clear_input_binding
reset_input_binding
reset_all_input_bindings
prompt_confirm
prompt_cancel
```

Invalid action names or invalid parameters should log once and be ignored.

## 15. RML document structure

A clean settings document should conceptually contain:

```text
root
  rom_setup_screen            # visible only when RomSetup is active
    title
    explanation
    status card
    select ROM button
    retry button
    help button
    quit button
  backdrop
  panel
    header
      title
      subtitle/status
      close button
    body
      tab rail
        General tab
        Graphics tab
        Controls tab
        Audio tab
        Debug tab
      content
        general page
        graphics page
        controls page
        audio page
        debug page
    footer
      navigation help
```

Use project-specific class names, for example:

```text
recomp-root
recomp-backdrop
recomp-panel
recomp-header
recomp-title
recomp-subtitle
recomp-tabs
recomp-tab
recomp-page
recomp-setting-row
recomp-setting-title
recomp-setting-description
recomp-setting-control
recomp-footer
recomp-prompt
```

Each setting row should include:

- A title.
- Optional description.
- One control.
- Disabled/unsupported state if applicable.
- Dirty/changed state when pending value differs from active value.

## 16. Styling specification

The style should be original and game-appropriate.

### 16.1 Global

- Fullscreen root.
- Readable default font.
- Warm high-contrast text.
- Consistent spacing.
- Clear focus outlines.

### 16.2 Overlay

- Semi-transparent backdrop.
- Main panel with strong border.
- Responsive dimensions.
- Avoid obscuring too much of the game when possible.

### 16.3 Tabs

States:

- normal.
- hover.
- focus.
- selected.
- disabled.

### 16.4 Setting rows

States:

- normal.
- hover.
- focus.
- changed.
- disabled.

### 16.5 Controls

Need style coverage for:

- Button.
- Segmented choice/radio group.
- Slider/range.
- Checkbox/toggle.
- Dropdown/select, if used.
- Text/value label.

### 16.6 Prompt

- Centered above settings.
- Clear confirm/cancel actions.
- Destructive action is visually distinct.
- Focus defaults to safest action for destructive prompts.

## 17. Visual direction for this project

Suggested original theme:

- Dark gothic panels.
- Antique gold borders.
- Burgundy/black background surfaces.
- Warm ivory primary text.
- Muted gray secondary text.
- Crimson destructive actions.
- Muted green success actions.

Avoid third-party art unless licensed and recorded.

## 18. Navigation behavior

### 18.1 Keyboard

```text
F1          Toggle settings
Esc         Back/close/cancel
Enter       Accept/activate
Space       Activate focused control
Arrows      Navigate or adjust selected slider/value
Tab         Next focus
Shift+Tab   Previous focus
```

### 18.2 Controller

```text
Menu/Start or configured UI toggle   Toggle settings, if enabled
A / accept binding                   Accept/activate
B / back binding                     Back/close/cancel
D-pad / left stick                   Navigate
Shoulders                            Optional tab switching
```

### 18.3 Mouse

- Click activates controls.
- Hover highlights rows.
- Moving mouse marks mouse as active.
- Controller/keyboard navigation marks mouse inactive for focus styling.

## 19. Focus management

Rules:

1. Opening settings focuses the last focused element if still valid.
2. If no previous element is valid, focus the active tab or first option.
3. Switching tabs focuses the first option in the new tab.
4. Opening a prompt stores the return focus element.
5. Closing prompt restores return focus if possible.
6. Disabled elements are skipped.
7. Hidden inactive tab contents are not focusable.

Initial implementation can rely mostly on RmlUi focus behavior and add helpers only where needed.

## 20. Prompt/dialog system

Suggested API:

```cpp
struct PromptButton {
    std::string label;
    enum class Role { Confirm, Cancel, Destructive, Neutral } role;
    std::function<void()> action;
};

void open_choice_prompt(
    std::string title,
    std::string body,
    PromptButton confirm,
    PromptButton cancel,
    bool focus_cancel_by_default
);

void open_info_prompt(
    std::string title,
    std::string body,
    PromptButton ok
);

void close_prompt();
bool prompt_open();
```

Prompt model variables:

```text
prompt_title
prompt_body
confirm_label
cancel_label
confirm_role
cancel_role
has_cancel
```

Prompt callbacks:

```text
prompt_confirm()
prompt_cancel()
prompt_close()
```

Use prompts for:

- Applying/discarding pending graphics changes.
- Resetting controls.
- Invalid ROM selection.
- Replacing the currently configured ROM path.
- ROM preparation/import failure.
- Quitting game.
- Informational errors.

## 21. Input suppression

The UI must coordinate with gameplay input.

Rules:

- `RomSetup` captures all input except platform/window shortcuts; gameplay has not started.
- Hidden UI passes all input normally.
- Visible interactive UI captures menu/navigation input.
- Gameplay input should be suppressed while settings or prompt is active.
- Input scanning suppresses gameplay input until scan completes or is cancelled.
- Non-interactive notifications may avoid capture if desired.

Implementation:

- Input path queries `recomp::ui::captures_input()`.
- Mouse/camera input path queries `recomp::ui::captures_mouse()`.
- UI still receives events while capturing.

## 22. Configuration persistence

UI should call existing project config APIs. Do not duplicate persistence.

Graphics:

1. Read active renderer config.
2. Maintain pending config while menu is open.
3. Apply using renderer config setter.
4. Save using existing config save path.

Controls:

1. Read controls config.
2. Update binding model.
3. Write `controls.json` or existing controls persistence file.
4. Notify runtime input system.

Audio:

1. Read audio config/backend state.
2. Apply through audio subsystem.
3. Persist if supported.

ROM:

1. Check command-line ROM argument first.
2. Check portable release locations such as `./rom.z64`, a ROM beside the app bundle/executable, and bundled/resource paths.
3. Check the persisted user-selected ROM path.
4. If no valid ROM is found, show `RomSetup` instead of exiting.
5. Persist a successfully validated user-selected path in the existing config folder.
6. If ROM preparation/import is required, cache generated artifacts under the config/cache folder rather than modifying the user’s source ROM.

## 23. Logging and diagnostics

Log once for:

- UI initialization success/failure.
- Asset root selected.
- Missing required asset.
- First successful draw.
- ROM setup status transitions: missing, selected, validated, prepared, failed.
- Unsupported renderer/shader format.

Avoid logging:

- every frame.
- every focus change.
- every RmlUi event.
- every data-model update.

Optional debug flag:

```text
RECOMP_ENABLE_UI_TRACE=0/1
```

Trace categories:

- asset loading.
- data model changes.
- input capture decisions.
- renderer resize/recreate.
- prompt open/close.

## 24. Error handling

If UI initialization fails:

- Log a clear reason.
- Disable UI.
- Continue game boot.

If assets fail to load:

- Log path attempted.
- Try fallback path or embedded fallback.
- Continue without UI if no fallback works.

If callback parameter is invalid:

- Log once.
- Ignore.

If config value is invalid:

- Clamp or reset to default.
- Dirty the model so UI reflects actual value.

## 25. Platform notes

### macOS

- Use pixel dimensions for high-DPI windows.
- Copy UI assets into `.app/Contents/Resources/ui` before signing.
- Sign the final app bundle after all resources are present.

### Windows

- Package UI assets in the zip.
- Include required renderer and SDL DLLs.
- Ensure the UI renderer supports the active shader format, especially DXIL for D3D12.
- Unsigned builds may trigger SmartScreen; release README should explain.

### Linux

- Support both portable and installed asset paths.
- Fullscreen may require SDL-specific handling depending on renderer backend.
- Avoid hardcoded macOS/Windows paths.

## 26. Minimal pseudocode

### Initialization

```cpp
void UiSystem::init(RenderInterface* render_interface, RenderDevice* device, WindowHandle window) {
    asset_root = find_asset_root();

    renderer.init(render_interface, device);
    system_interface.init(window, asset_root);

    Rml::SetRenderInterface(renderer.rml_interface());
    Rml::SetSystemInterface(system_interface.rml_interface());

    if (!Rml::Initialise()) {
        log_error("RmlUi initialization failed");
        enabled = false;
        return;
    }

    load_fonts_if_any();

    auto [width, height] = query_window_pixel_size(window);
    context = Rml::CreateContext("recomp_ui", {width, height});
    if (!context) {
        log_error("Failed to create RmlUi context");
        enabled = false;
        return;
    }

    actions.register_defaults();
    models.register_all(context);

    settings_doc = load_document("settings.rml");
    prompt_doc = load_document("prompt.rml");

    hide_all_documents();
    enabled = true;
}
```

### Draw

```cpp
void UiSystem::draw(RenderCommandList* list, RenderFramebuffer* framebuffer) {
    if (!enabled || state.screen == UiScreen::Hidden) {
        return;
    }

    update_context_size_if_needed();
    drain_events_to_rml();
    refresh_dirty_models();

    context->Update();

    renderer.begin(list, window_width, window_height);
    context->Render();
    renderer.end(list, framebuffer);
}
```

### Toggle overlay

```cpp
void UiSystem::toggle_overlay() {
    if (!enabled) {
        return;
    }

    if (state.screen == UiScreen::Hidden) {
        open_settings();
    } else {
        close_all();
    }
}
```

### Apply graphics

```cpp
void GraphicsUiModel::apply() {
    if (!has_pending_changes()) {
        return;
    }

    renderer_config_set(pending);
    save_config();
    active = pending;
    dirty_all_graphics_variables();
}
```

## 27. Implementation phases

### Phase 1 — Static settings shell

Deliverable:

- Original settings UI appears with F1.
- Tabs are visible.
- Graphics tab shows current config values.
- No interaction beyond close/toggle is required.

Acceptance:

- Builds locally.
- UI starts hidden.
- No game boot/gameplay regression.
- No log spam.
- No third-party copied UI files.

### Phase 2 — Clickable graphics options

Deliverable:

- Mouse-clickable graphics options.
- Apply/discard behavior.
- Existing hotkeys still work.

Acceptance:

- Resolution/aspect/MSAA/refresh/window mode can be changed from UI.
- Closing with pending changes prompts.
- UI reflects config changes.

### Phase 3 — Keyboard/controller navigation

Deliverable:

- Focusable tabs/options.
- D-pad/left stick/arrow navigation.
- A/Enter accepts.
- B/Esc backs out.

Acceptance:

- UI usable without mouse.
- Gameplay input is suppressed while UI is open.
- Input restored after close.

### Phase 4 — Controls menu

Deliverable:

- Show current bindings.
- Rebinding scanner.
- Reset defaults.
- Save controls.

Acceptance:

- Controller and keyboard bindings are editable.
- Config persists.
- Scanning cancels cleanly.

### Phase 5 — Audio and prompts polish

Deliverable:

- Audio settings if backend supports them.
- Reusable prompt system.
- Polished theme/assets.

Acceptance:

- Prompts are controller navigable.
- Any new assets have license documentation.
- Release packages include UI assets.

### Phase 6 — Optional launcher/debug/mods

Deliverable:

- Launcher or advanced debug UI if needed.
- Optional mod management only after baseline stability.

Acceptance:

- Normal users are not exposed to unstable debug controls by default.
- Debug tools are gated.

## 28. Acceptance checklist for repository changes

Before committing UI implementation:

- [ ] New files are original to this repo or have compatible license notes.
- [ ] UI starts hidden by default.
- [ ] F1 opens/closes UI.
- [ ] Game input works normally after closing UI.
- [ ] Build succeeds locally.
- [ ] Packaging includes UI assets or assets are embedded.
- [ ] Missing assets fail gracefully.
- [ ] No per-frame logging.
- [ ] Windows/macOS CI packaging updated if asset layout changed.
- [ ] README documents UI hotkeys and release asset expectations.

## 29. Suggested first task for this project

Implement Phase 1:

1. Create original `assets/ui/settings.rml`.
2. Create original `assets/ui/settings.rcss`.
3. Modify current overlay code to load the document/style or embed a simple original document.
4. Keep existing render hook and F1 behavior.
5. Show live graphics config values.
6. Build/sign locally.
7. Ask for visual testing.
8. Do not commit until requested.

---

## 30. Concrete menu map for Castlevania: Legacy of Darkness Recompiled

This section maps the intended UI exactly: menu names, tabs, options, value lists, existing hotkeys, and current LoD-specific keybindings. It should be updated whenever runtime options or default bindings change.

### 30.1 Top-level UI entry points

| Entry point | Trigger | Opens | Notes |
|---|---:|---|---|
| ROM Setup | Missing or invalid ROM at startup | Overlay-only ROM setup screen | Window/UI still launches; game thread does not start until a valid ROM is selected. |
| Settings Overlay | `F1` | Settings menu | Existing behavior. Overlay starts hidden. |
| Close Settings | `F1`, `Esc`, UI close button, controller Back/Cancel | Hidden/gameplay | If pending changes exist, open Apply/Discard prompt first. |
| Apply Graphics Changes | UI Apply button; optionally controller/menu Apply binding | Applies pending graphics config | Existing hotkeys apply immediately; UI should support pending/apply semantics. |
| Discard Graphics Changes | UI Discard button or close prompt choice | Restores pending graphics values from active config | Only relevant once interactive UI exists. |

### 30.2 Settings menu tabs

Tabs should appear in this order:

| Tab ID | Menu label | Default visibility | Purpose |
|---|---|---:|---|
| `general` | General | On | Version, ROM/config paths, basic app behavior. |
| `graphics` | Graphics | On | Renderer/window/display settings. First interactive target. |
| `controls` | Controls | On | Gamepad/keyboard mapping display and later rebinding. |
| `audio` | Audio | On | Audio status and volume options once exposed. |
| `debug` | Debug | Off by default or developer-only | Runtime diagnostics; should be hidden from normal release users unless explicitly enabled. |

Default focused tab for the first implementation: `graphics`.

## 31. General tab option map

| Option ID | Label | Control type | Values | Default/current source | Persistence | Notes |
|---|---|---|---|---|---|---|
| `version` | Version | read-only text | commit/tag/build string | build metadata | none | Show tag on release builds, short SHA on dev builds. |
| `rom_status` | ROM | read-only text | loaded/missing/path basename | runtime ROM loader | none | Do not show full user path unless useful; avoid clutter. |
| `select_rom` | Select ROM | button | action | native file dialog | persisted ROM path after validation | Visible when ROM is missing; also available as “Change ROM” when loaded. |
| `rom_help` | ROM Help | button/link | action | help/documentation | none | Explain accepted file types and that the app can prepare/cache the ROM internally. |
| `config_path` | Config Folder | read-only text + button | platform config path | `g_config_path` | none | Optional “Open Folder” button later. |
| `ui_visibility` | Settings Overlay | help text | `F1` toggles | fixed hotkey | none | Explain overlay starts hidden. |
| `portable_rom_name` | ROM Filename | read-only text | `rom.z64` | release UX | none | Explain release builds prefer `./rom.z64`. |
| `reset_all_settings` | Reset All Settings | button | action | config adapters | writes config files | Later; destructive prompt required. |

General tab first-pass content can be read-only except close/open-folder actions.

## 32. Graphics tab option map

Graphics values come from `ultramodern::renderer::GraphicsConfig` and the existing `graphics.json` persistence path.

Current default config:

| Field | Default |
|---|---|
| `developer_mode` | `false` |
| `res_option` | `Original2x` |
| `wm_option` | `Windowed` |
| `hr_option` | `Original` |
| `api_option` | `Auto` |
| `ar_option` | `Original` |
| `msaa_option` | `None` |
| `rr_option` | `Original` |
| `hpfb_option` | `Auto` |
| `rr_manual_value` | `60` |
| `ds_option` | `1` |

### 32.1 Graphics tab primary options

| Option ID | Label | Control type | Values | Default | Existing hotkey | Persistence key | Notes |
|---|---|---|---|---|---|---|---|
| `window_mode` | Window Mode | segmented/radio | `Windowed`, `Fullscreen` | `Windowed` | `F11` toggles | `window_mode` | Apply should call renderer fullscreen update. |
| `resolution` | Internal Resolution | segmented/radio | `Original`, `Original2x`, `Auto` | `Original2x` | `F5` cycles | `resolution` | `Original2x` is current default and desired baseline. |
| `downsample` | Downsample Factor | slider/stepper | integer `1..8` | `1` | none | `downsample` | Only meaningful for supported resolution modes; document in UI. |
| `aspect_ratio` | Aspect Ratio | segmented/radio | `Original`, `Expand`, `Manual` | `Original` | `F6` cycles | `aspect_ratio` | `Manual` currently maps to 16:9 target in renderer. |
| `antialiasing` | Anti-Aliasing | segmented/radio | `None`, `MSAA2X`, `MSAA4X`, `MSAA8X` | `None` | `F7` cycles | `antialiasing` | Disable unsupported values if renderer reports support. |
| `refresh_rate` | Refresh Rate | segmented/radio | `Original`, `Display`, `Manual` | `Original` | `F8` cycles | `refresh_rate` | Manual uses `refresh_rate_manual`. |
| `refresh_rate_manual` | Manual Refresh Rate | numeric/slider | integer `20..360` | `60` | none | `refresh_rate_manual` | Enabled only when `refresh_rate=Manual`. |
| `high_precision_framebuffer` | High Precision Framebuffer | segmented/radio | `Auto`, `On`, `Off` | `Auto` | none | `high_precision_framebuffer` | Present after interaction layer exists. |
| `graphics_api` | Graphics API | read-only or dropdown | `Auto`, `D3D12`, `Vulkan`, `Metal` | `Auto` | none | `graphics_api` | Runtime/backend-sensitive; changing may require restart, so initially read-only. |
| `hud_ratio` | HUD Ratio | hidden/advanced | `Original`, `Clamp16x9`, `Full` | `Original` | none | `hud_ratio` | Expose later only if LoD HUD behavior is validated. |
| `native_high_res_status` | Native High-Res Mode | read-only info | skipped/disabled/enabled | skipped by default | none | build flags | Explain native Expansion Pak high-res route is not normal baseline. |

### 32.2 Graphics action buttons

| Action ID | Label | Enabled when | Behavior |
|---|---|---|---|
| `apply_graphics` | Apply | pending config differs from active | Apply pending config, save `graphics.json`, clear dirty state. |
| `discard_graphics` | Discard | pending config differs from active | Reset pending config from active config, clear dirty state. |
| `reset_graphics_defaults` | Reset Defaults | always | Prompt, then set pending config to defaults. |

### 32.3 Graphics hotkey compatibility

Existing hotkeys must remain supported even after the UI becomes interactive:

| Hotkey | Current behavior | UI sync requirement |
|---:|---|---|
| `F1` | Toggle overlay | Show/hide UI and update capture state. |
| `F5` | Cycle `resolution` and save immediately | If UI is visible, refresh graphics model. |
| `F6` | Cycle `aspect_ratio` and save immediately | If UI is visible, refresh graphics model. |
| `F7` | Cycle `antialiasing` and save immediately | If UI is visible, refresh graphics model. |
| `F8` | Cycle `refresh_rate` and save immediately | If UI is visible, refresh graphics model. |
| `F11` | Toggle `window_mode` and save immediately | If UI is visible, refresh graphics model. |

Implementation note: once UI has pending/apply semantics, decide whether hotkeys should continue immediate-apply or mutate pending values while the menu is open. Recommended: outside UI = immediate apply; inside UI = mutate pending values and show dirty state.

## 33. Controls tab option and binding map

Current controls config file: `controls.json` under the platform config path.

Current N64 button constants:

| N64 input | Bit value | Config string |
|---|---:|---|
| A | `0x8000` | `n64_a` |
| B | `0x4000` | `n64_b` |
| Z | `0x2000` | `n64_z` |
| Start | `0x1000` | `n64_start` |
| D-pad Up | `0x0800` | `n64_dpad_up` |
| D-pad Down | `0x0400` | `n64_dpad_down` |
| D-pad Left | `0x0200` | `n64_dpad_left` |
| D-pad Right | `0x0100` | `n64_dpad_right` |
| L | `0x0020` | `n64_l` |
| R | `0x0010` | `n64_r` |
| C-Up | `0x0008` | `n64_c_up` |
| C-Down | `0x0004` | `n64_c_down` |
| C-Left | `0x0002` | `n64_c_left` |
| C-Right | `0x0001` | `n64_c_right` |
| None | `0x0000` | `none` |

### 33.1 Current gamepad defaults

These defaults are LoD-specific and should be presented in the Controls tab.

| Control ID | UI label | SDL controller input | Maps to N64 input | Intended LoD action / note | Config path |
|---|---|---|---|---|---|
| `jump` | Jump | `A` / Cross | `n64_a` | Jump / confirm in some contexts. | `gamepad.buttons.a` |
| `attack_primary` | Attack 1 | `X` / Square | `n64_b` | Primary attack. | `gamepad.buttons.x` |
| `attack_secondary` | Attack 2 | `Y` / Triangle | `n64_c_left` | Secondary attack. | `gamepad.buttons.y` |
| `interact` | Collect / Interact | `B` / Circle | `n64_c_right` | Pick up items / interact. | `gamepad.buttons.b` |
| `throw_item` | Throw Item | `Right Shoulder` / R1/RB | `n64_c_down` | Use/throw item. | `gamepad.buttons.right_bumper` |
| `lock_on` | Lock On | Left Stick Click / L3 | `n64_r` | Lock-on / target. | `gamepad.buttons.left_stick` |
| `n64_l` | N64 L | Right Trigger / R2/RT | `n64_l` | Currently mapped to N64 L. | `gamepad.axes.right_trigger` |
| `n64_z` | N64 Z | Left Trigger / L2/LT | `n64_z` | Currently mapped to N64 Z. | `gamepad.axes.left_trigger` |
| `start` | Start / Pause | Start / Options | `n64_start` | Pause/menu. | `gamepad.buttons.start` |
| `dpad_up` | D-pad Up | Controller D-pad Up | `n64_dpad_up` | N64 D-pad direct mapping. | `gamepad.buttons.dpad_up` |
| `dpad_down` | D-pad Down | Controller D-pad Down | `n64_dpad_down` | N64 D-pad direct mapping. | `gamepad.buttons.dpad_down` |
| `dpad_left` | D-pad Left | Controller D-pad Left | `n64_dpad_left` | N64 D-pad direct mapping. | `gamepad.buttons.dpad_left` |
| `dpad_right` | D-pad Right | Controller D-pad Right | `n64_dpad_right` | N64 D-pad direct mapping. | `gamepad.buttons.dpad_right` |
| `analog_move` | Move | Left Stick | analog X/Y | Movement. Left stick Y is inverted in code to N64-positive-up. | implicit axes |
| `camera_dpad` | Camera / D-pad Emulation | Right Stick | N64 D-pad | Right stick maps to D-pad when enabled. | `gamepad.right_stick.*` |

### 33.2 Current right-stick settings

| Option ID | Label | Control type | Values | Default | Config path | Notes |
|---|---|---|---|---|---|---|
| `right_stick_mode` | Right Stick Mode | segmented/radio | `N64 D-pad`, `Off` | `N64 D-pad` | `gamepad.right_stick.mode` | Current code accepts `n64_dpad` or `none`. |
| `right_stick_invert_x` | Invert Right Stick X | toggle | `On`, `Off` | `On` | `gamepad.right_stick.invert_x` | Current user-requested mapping has X inverted. |
| `right_stick_invert_y` | Invert Right Stick Y | toggle | `On`, `Off` | `On` | `gamepad.right_stick.invert_y` | Current user-requested mapping has Y inverted. |
| `right_stick_deadzone` | Right Stick Deadzone | slider | `0.0..1.0` | `0.5` | `gamepad.right_stick.deadzone` | Used before emitting D-pad bits. |

### 33.3 Trigger settings

| Option ID | Label | Control type | Values | Default | Config path | Notes |
|---|---|---|---|---|---|---|
| `left_trigger_binding` | Left Trigger | binding selector | any N64 button or none | `n64_z` | `gamepad.axes.left_trigger` | L2/LT. |
| `right_trigger_binding` | Right Trigger | binding selector | any N64 button or none | `n64_l` | `gamepad.axes.right_trigger` | R2/RT. |
| `trigger_threshold` | Trigger Threshold | slider | `0..32767` | `12000` | `gamepad.axes.trigger_threshold` | Axis threshold before emitting mapped N64 button. |

### 33.4 Keyboard defaults

Keyboard mappings are currently hardcoded in `get_n64_input` and are not yet persisted through `controls.json`.

| UI label | Keyboard input | Maps to N64 input | Notes |
|---|---|---|---|
| A / Jump | Enter | `n64_a` | Also menu accept in-game. |
| B / Attack | Right Shift | `n64_b` | Keyboard primary attack. |
| Z | Z | `n64_z` | N64 Z. |
| Start | Space | `n64_start` | Pause/start. |
| D-pad Up | Up Arrow | `n64_dpad_up` | Direct D-pad. |
| D-pad Down | Down Arrow | `n64_dpad_down` | Direct D-pad. |
| D-pad Left | Left Arrow | `n64_dpad_left` | Direct D-pad. |
| D-pad Right | Right Arrow | `n64_dpad_right` | Direct D-pad. |
| Analog Up | W | analog Y +1 | Movement. |
| Analog Down | S | analog Y -1 | Movement. |
| Analog Left | A key | analog X -1 | Movement. |
| Analog Right | D key | analog X +1 | Movement. |

Controls UI first pass should show keyboard mappings read-only. Rebinding keyboard can be added later after the persistence format supports it.

### 33.5 Controls tab actions

| Action ID | Label | Enabled when | Behavior |
|---|---|---|---|
| `select_controller_bindings` | Controller | always | Shows controller binding table. |
| `select_keyboard_bindings` | Keyboard | always | Shows keyboard binding table; initially read-only. |
| `begin_scan_binding` | Rebind | controller binding selected | Enters scanning mode for selected binding. |
| `cancel_scan_binding` | Cancel | scanning | Exits scanning without changes. |
| `clear_binding` | Clear | binding selected | Sets selected mapping to `none`. |
| `reset_binding` | Reset Row | binding selected | Restores one default mapping. |
| `reset_all_bindings` | Reset All | always | Prompt, then restore defaults and save. |
| `save_controls` | Save | controls dirty | Writes `controls.json`. |
| `open_controls_file` | Open Config File | platform supports opening folder | Opens config folder or copies path. |

### 33.6 Controls scanning rules

- While scanning, UI captures input and game input is suppressed.
- First valid controller button/trigger input becomes the binding.
- The scanner should ignore UI navigation inputs for a short debounce window after pressing Rebind.
- `Esc` / controller Back cancels scanning.
- If a binding already exists elsewhere, policy must be explicit:
  - recommended first policy: allow duplicate with a warning icon/text.
  - stricter future policy: prompt to move binding.

## 34. Audio tab option map

Audio currently works by default through the recompiled audio RSP path, but exposed runtime volume controls may not exist yet. The UI should not show fake controls as functional.

| Option ID | Label | Control type | Values | Default/current source | Persistence | Notes |
|---|---|---|---|---|---|---|
| `audio_status` | Audio Status | read-only text | available/unavailable | audio subsystem init | none | Show whether audio device/backend is active. |
| `audio_backend` | Audio Backend | read-only text | backend/device name if available | audio subsystem | none | Optional. |
| `master_volume` | Master Volume | slider | `0..100` | TBD | TBD | Only enable if runtime exposes volume. |
| `music_volume` | Music Volume | slider | `0..100` | TBD | TBD | Only enable if music/effects split exists. |
| `sfx_volume` | SFX Volume | slider | `0..100` | TBD | TBD | Only enable if music/effects split exists. |
| `mute_when_unfocused` | Mute When Unfocused | toggle | `On`, `Off` | TBD | TBD | Optional future feature. |
| `reset_audio_defaults` | Reset Audio Defaults | button | action | audio config | writes config | Prompt required. |

First-pass Audio tab should show read-only status plus placeholders marked “not yet configurable” if controls are not wired.

## 35. Debug tab option map

Debug tab must be hidden in normal releases unless a developer/debug setting enables it.

| Option ID | Label | Control type | Values/source | Notes |
|---|---|---|---|
| `build_version` | Build Version | read-only text | tag/SHA | Useful for reports. |
| `current_gamestate` | Game State | read-only text | current game state if accessible | Example: `gs=3`. |
| `map_overlay_rom` | Map Overlay ROM | read-only hex | `lod_current_map_overlay_rom()` | Include size/load count if available. |
| `map_overlay_size` | Map Overlay Size | read-only hex | `lod_current_map_overlay_size()` | Useful for overlay bugs. |
| `map_overlay_load_count` | Map Overlay Loads | read-only integer | `lod_current_map_overlay_load_count()` | Useful for comparing logs. |
| `ni_pair_0f` | NI 0F Pair | read-only integer | `lod_ni_overlay_loaded_0f_pair()` | Current 0x0F overlay pair. |
| `ni_pair_0e` | NI 0E Pair | read-only integer | `lod_ni_overlay_loaded_0e_pair()` | Current 0x0E overlay pair. |
| `reported_os_mem_size` | Reported RDRAM | read-only text | native high-res flag/config | Show 4MB/8MB reporting state. |
| `expansion_pak_skip` | Expansion Pak Screen Skip | read-only text | build flag/runtime flag | Default baseline skips via `gs=-5`. |
| `copy_diagnostics` | Copy Diagnostics | button | action | Copies concise debug summary to clipboard/log. |
| `open_log_folder` | Open Log Folder | button | action | Optional. |

Do not expose compile-time trace flags as live toggles unless the runtime actually supports changing them live.

## 36. Prompt map

| Prompt ID | Trigger | Title | Confirm | Cancel | Default focus | Notes |
|---|---|---|---|---|---|---|
| `graphics_apply_discard` | Closing settings with pending graphics changes | Graphics options have changed | Apply | Discard | Apply or Discard; choose safer UX | Confirm applies pending config; cancel/discard restores. |
| `reset_graphics` | Reset Graphics Defaults | Reset graphics settings? | Reset | Cancel | Cancel | Destructive. |
| `reset_controls_all` | Reset All Controls | Reset all controls? | Reset | Cancel | Cancel | Destructive. |
| `scan_binding_cancel` | Leaving while scanning | Stop rebinding? | Stop | Continue | Continue | Optional. |
| `rom_invalid` | Selected file fails validation | ROM not supported | Choose Another | Cancel | Choose Another | Remain in ROM setup; do not exit. |
| `rom_prepare_failed` | ROM import/preparation fails | ROM setup failed | Try Again | Cancel | Try Again | Show concise reason and keep selected path editable. |
| `replace_rom` | Change ROM while one is configured | Change configured ROM? | Change | Cancel | Cancel | Stops pending launch; does not mutate existing cached ROM until new selection validates. |
| `quit_game` | Quit button | Quit game? | Quit | Cancel | Cancel | Later. |
| `missing_assets` | UI asset load failure | UI assets missing | OK | none | OK | Fallback/minimal prompt only if UI can render. |

## 37. UI navigation binding map

These are UI-level actions, separate from N64 gameplay inputs.

| UI action | Keyboard default | Controller default | Behavior |
|---|---|---|---|
| `toggle_menu` | `F1` | optional Start/Menu combo; initially none | Open/close settings overlay. |
| `accept` | `Enter`, `Space` | A / Cross | Activate focused element. |
| `back` | `Esc` | B / Circle | Close prompt/submenu/settings. |
| `navigate_up` | Up Arrow | D-pad Up / Left Stick Up | Move focus up. |
| `navigate_down` | Down Arrow | D-pad Down / Left Stick Down | Move focus down. |
| `navigate_left` | Left Arrow | D-pad Left / Left Stick Left | Move focus left or decrease value. |
| `navigate_right` | Right Arrow | D-pad Right / Left Stick Right | Move focus right or increase value. |
| `next_tab` | `]` or `Tab` when on tab rail | R1/RB optional | Next tab. |
| `previous_tab` | `[` or Shift+Tab when on tab rail | L1/LB optional | Previous tab. |
| `apply` | optional `Y` keyboard binding or UI button only | Y / Triangle optional | Apply pending graphics changes if enabled. |

Avoid binding controller Start to the UI overlay until it is proven not to conflict with in-game pause/start expectations. F1-only is safest for the first release.

## 38. Settings document exact page layout

### 38.1 Header

| Element ID | Text/source | Notes |
|---|---|---|
| `settings_title` | `Legacy of Darkness Recompiled` | Main title. |
| `settings_subtitle` | active tab subtitle | Example: `Graphics Settings`. |
| `settings_close` | `Close` or `×` | Calls `close_settings`. |

### 38.2 Tab rail

| Element ID | Label | Action |
|---|---|---|
| `tab_general` | General | `set_tab(general)` |
| `tab_graphics` | Graphics | `set_tab(graphics)` |
| `tab_controls` | Controls | `set_tab(controls)` |
| `tab_audio` | Audio | `set_tab(audio)` |
| `tab_debug` | Debug | `set_tab(debug)`; hidden unless enabled |

### 38.3 Footer

| Element ID | Text/source | Example |
|---|---|---|
| `footer_navigate` | `nav_model.help_navigate` | `D-pad / Arrows: Navigate` |
| `footer_accept` | `nav_model.help_accept` | `A / Enter: Select` |
| `footer_back` | `nav_model.help_back` | `B / Esc: Back` |
| `footer_apply` | `nav_model.help_apply` | `Y: Apply` or hidden if not relevant |

## 39. Config file mapping summary

### 39.1 `graphics.json`

| JSON key | UI option | Type | Valid values/range |
|---|---|---|---|
| `developer_mode` | Debug/developer mode | bool | `true/false` |
| `resolution` | Internal Resolution | enum string | `Original`, `Original2x`, `Auto` |
| `window_mode` | Window Mode | enum string | `Windowed`, `Fullscreen` |
| `hud_ratio` | HUD Ratio | enum string | `Original`, `Clamp16x9`, `Full` |
| `graphics_api` | Graphics API | enum string | `Auto`, `D3D12`, `Vulkan`, `Metal` |
| `aspect_ratio` | Aspect Ratio | enum string | `Original`, `Expand`, `Manual` |
| `antialiasing` | Anti-Aliasing | enum string | `None`, `MSAA2X`, `MSAA4X`, `MSAA8X` |
| `refresh_rate` | Refresh Rate | enum string | `Original`, `Display`, `Manual` |
| `high_precision_framebuffer` | High Precision Framebuffer | enum string | `Auto`, `On`, `Off` |
| `refresh_rate_manual` | Manual Refresh Rate | int | `20..360` |
| `downsample` | Downsample Factor | int | `1..8` |

### 39.2 `controls.json`

| JSON path | UI option | Type | Valid values/range |
|---|---|---|---|
| `gamepad.buttons.a` | Jump | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.x` | Attack 1 | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.y` | Attack 2 | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.b` | Collect / Interact | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.start` | Start / Pause | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.left_stick` | Lock On | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.right_bumper` | Throw Item | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.dpad_up` | D-pad Up | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.dpad_down` | D-pad Down | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.dpad_left` | D-pad Left | N64 button string | any N64 input string or `none` |
| `gamepad.buttons.dpad_right` | D-pad Right | N64 button string | any N64 input string or `none` |
| `gamepad.axes.left_trigger` | Left Trigger | N64 button string | any N64 input string or `none` |
| `gamepad.axes.right_trigger` | Right Trigger | N64 button string | any N64 input string or `none` |
| `gamepad.axes.trigger_threshold` | Trigger Threshold | int | `0..32767` |
| `gamepad.right_stick.mode` | Right Stick Mode | string | `n64_dpad`, `none` |
| `gamepad.right_stick.invert_x` | Invert Right Stick X | bool | `true/false` |
| `gamepad.right_stick.invert_y` | Invert Right Stick Y | bool | `true/false` |
| `gamepad.right_stick.deadzone` | Right Stick Deadzone | float | `0.0..1.0` |

## 40. Minimum first implementation menu content

For the first check-in, implement at least this UI content:

### General

- Version.
- ROM status.
- Config folder.
- F1 help.

### Graphics

- Window Mode.
- Internal Resolution.
- Aspect Ratio.
- Anti-Aliasing.
- Refresh Rate.
- Native High-Res status note.

### Controls

- Connected controller name.
- Read-only current gamepad default mapping table.
- Read-only keyboard mapping table.
- Controls config path.

### Audio

- Audio status.
- “Volume controls not yet implemented” note if volume APIs are absent.

### Debug

- Hidden unless debug UI is enabled.
- If visible, show current gamestate and overlay/NI info where available.

## 41. ROM setup / overlay-only boot flow

The app must not silently do nothing or exit when the user launches without a ROM. If no valid ROM is available, it should boot the renderer and UI into a dedicated overlay-only screen that explains the issue and lets the user choose a ROM.

### 41.1 Startup order

Required startup order:

1. Initialize platform/window/render context enough to display UI.
2. Initialize the UI system and load the ROM setup document.
3. Load graphics, controls, audio, and UI config.
4. Try to resolve a ROM path:
   1. command-line argument.
   2. portable release path: `./rom.z64`.
   3. ROM beside the executable or beside the `.app` bundle on macOS.
   4. ROM inside bundled/resource paths, if intentionally packaged.
   5. persisted user-selected path from the config folder.
   6. optional local-directory scan for compatible extensions.
5. If the resolved ROM is valid, start the game automatically and keep the settings overlay hidden.
6. If no ROM is valid, show `RomSetup` and do not start the game thread.

### 41.2 ROM setup screen

Screen name: `ROM Setup`

Document ID: `rom_setup`

Mode: overlay-only, full-window, no gameplay behind it.

Required visible copy:

```text
Castlevania: Legacy of Darkness Recompiled
ROM required
Select your ROM file to start the game.
```

Required controls:

| Control ID | Label | Type | Enabled when | Behavior |
|---|---|---|---|---|
| `rom_select_file` | Select ROM | primary button | not busy | Opens native file picker for `.z64`, `.n64`, `.v64`. |
| `rom_retry` | Retry | secondary button | missing/failed/invalid | Re-runs discovery and validation. |
| `rom_help` | Help | secondary button/link | always | Shows concise ROM instructions. |
| `rom_quit` | Quit | secondary button | always | Opens quit prompt or exits app. |

Status card fields:

| Field ID | Label | Values |
|---|---|---|
| `rom_status_label` | Status | Missing, Selecting, Validating, Preparing, Ready, Invalid, Failed |
| `rom_file_label` | Selected file | basename only, or “None selected” |
| `rom_detail_label` | Detail | concise actionable message |

### 41.3 File picker behavior

- Use a platform-native picker when available.
- Filter to Nintendo 64 ROM-like files: `.z64`, `.n64`, `.v64`.
- Default starting directory should prefer Downloads if available, then user home.
- Cancelling the picker returns to `ROM Setup`; it is not an error.
- Selecting the same invalid file repeatedly should log sparingly and keep the user on the setup screen.

### 41.4 Validation and preparation

The user-facing flow should accept the user’s source ROM and do any required preparation internally.

Rules:

- Validate that the selected file is the supported game/region before starting the game.
- If byte-order conversion or runtime-specific preparation is required, do it inside the app.
- Cache prepared artifacts in the app config/cache folder.
- Never modify the user’s original ROM file.
- Persist the original selected path only after validation succeeds.
- Persist prepared cache metadata separately so cache invalidation is explicit.
- If preparation fails, show `rom_prepare_failed` with a short explanation and a retry path.

First implementation fallback:

- If full internal preparation is not wired yet, the ROM setup screen may require an already-prepared ROM, but the UI must state that clearly.
- This fallback should be treated as temporary release debt; the target UX is “select supported ROM, app prepares what it needs.”

### 41.5 Transition to gameplay

When a ROM becomes ready:

1. Update `rom_status = ready`.
2. Persist the selected path/cache metadata.
3. Hide `ROM Setup`.
4. Start the game exactly once.
5. Ignore duplicate start requests while `rom_busy` or game-running is true.

The settings overlay should then use normal gameplay behavior: hidden by default, toggle with `F1`.

### 41.6 Changing ROM after setup

Changing ROM from the General tab should be possible but guarded:

- Button label: `Change ROM`.
- Prompt ID: `replace_rom`.
- If confirmed, open the same native picker.
- New selection must validate before replacing stored path/cache metadata.
- Do not interrupt a running game unless the runtime explicitly supports reset/relaunch. If it does not, show “Restart required” and save for next launch.

### 41.7 Acceptance checklist

- Launching without `rom.z64` opens a visible app window.
- The ROM setup screen is usable with mouse, keyboard, and controller.
- Selecting a valid ROM starts gameplay.
- Cancelling selection keeps the app open.
- Selecting an invalid ROM shows an actionable error and allows retry.
- Relaunching after a valid selection starts the game without asking again.
- Deleting/moving the stored ROM path returns to ROM setup instead of exiting.
- Logs contain a small number of ROM setup lines, not per-frame spam.
