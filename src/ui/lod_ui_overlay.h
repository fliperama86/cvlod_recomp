#pragma once

#include <functional>
#include <string>

#include "ultramodern/config.hpp"

union SDL_Event;

namespace lod::ui {
    using GraphicsApplyCallback = std::function<void(const ultramodern::renderer::GraphicsConfig&, const char*)>;

    void set_render_hooks();
    void set_graphics_apply_callback(GraphicsApplyCallback callback);
    void set_config_path_display(std::string path);
    void set_connected_controller_name(std::string name);

    void toggle_overlay();
    void show_overlay();
    void hide_overlay();
    void set_overlay_visible(bool visible);
    bool overlay_visible();

    bool captures_input();
    bool captures_mouse();

    bool queue_platform_event(const SDL_Event& event);
    void notify_graphics_config_changed();
    void notify_controls_config_changed();
    void notify_audio_config_changed();
}
