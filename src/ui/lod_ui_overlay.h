#pragma once

#include <functional>
#include <string>

#include "ultramodern/config.hpp"

struct SDL_Window;
union SDL_Event;

namespace lod::ui {
    using GraphicsApplyCallback = std::function<void(const ultramodern::renderer::GraphicsConfig&, const char*)>;

    enum class RomSetupRequest {
        None,
        SelectFile,
        Retry,
        Help,
        Quit,
    };

    void set_render_hooks();
    void set_sdl_window(SDL_Window* window);
    void set_graphics_apply_callback(GraphicsApplyCallback callback);
    void set_config_path_display(std::string path);
    void set_connected_controller_name(std::string name);
    void set_controls_config_summary(std::string buttons, std::string triggers, std::string right_stick);

    void toggle_overlay();
    void show_overlay();
    void hide_overlay();
    void set_overlay_visible(bool visible);
    bool overlay_visible();

    bool captures_input();
    bool captures_mouse();

    void show_rom_setup();
    void hide_rom_setup();
    bool rom_setup_visible();
    void set_rom_setup_status(std::string status, std::string detail, std::string file, bool busy);
    RomSetupRequest consume_rom_setup_request();

    bool handle_graphics_hotkey(int key);
    bool queue_platform_event(const SDL_Event& event);
    void notify_graphics_config_changed();
    void notify_controls_config_changed();
    void notify_audio_config_changed();
}
