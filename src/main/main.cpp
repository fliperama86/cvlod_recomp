#include <cstdio>
#include <cassert>
#include <csignal>
#include <cstdlib>
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <vector>
#include <array>
#include <numeric>
#include <stdexcept>
#include <cinttypes>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <string>
#include <cctype>
#include <limits.h>
#if defined(__APPLE__) || defined(__linux__)
#include <execinfo.h>
#endif

#include "ultramodern/ultra64.h"
#include "ultramodern/ultramodern.hpp"
#define SDL_MAIN_HANDLED
#ifdef _WIN32
#include "SDL.h"
#include "SDL_syswm.h"
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#undef None
#undef Status
#undef LockMask
#undef ControlMask
#undef Success
#undef Always
#endif

#include "lod/lod_render.h"
#include "lod/lod_support.h"
#include "lod/lod_fault_trace.hpp"
#include "lod/lod_paths.hpp"
#include "lod/target_rom.hpp"
#include "lod_ui_overlay.h"
#include "librecomp/game.hpp"
#include "librecomp/overlays.hpp"
#include "librecomp/rsp.hpp"
#include "nfd.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef LOD_ENABLE_AUDIO_TRACE
#define LOD_ENABLE_AUDIO_TRACE 0
#endif

#ifndef LOD_ENABLE_AUDIO_RAW_DUMP
#define LOD_ENABLE_AUDIO_RAW_DUMP 0
#endif

#ifndef LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
#define LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS 0
#endif

template<typename... Ts>
void exit_error(const char* str, Ts ...args) {
    ((void)fprintf(stderr, str, args), ...);
    assert(false);
    ultramodern::error_handling::quick_exit(__FILE__, __LINE__, __FUNCTION__);
}

// ── Graphics setup ──────────────────────────────────────────────────

ultramodern::gfx_callbacks_t::gfx_data_t create_gfx() {
    SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) > 0) {
        exit_error("Failed to initialize SDL2: %s\n", SDL_GetError());
    }

    fprintf(stdout, "SDL Video Driver: %s\n", SDL_GetCurrentVideoDriver());
    return {};
}

static SDL_Window* window = nullptr;
static SDL_GameController* game_controller = nullptr;
static SDL_JoystickID game_controller_instance = -1;
static std::filesystem::path g_config_path;

static std::filesystem::path graphics_config_path() {
    return g_config_path / "graphics.json";
}

static std::filesystem::path controls_config_path() {
    return g_config_path / "controls.json";
}

static constexpr uint16_t N64_BUTTON_A       = 0x8000;
static constexpr uint16_t N64_BUTTON_B       = 0x4000;
static constexpr uint16_t N64_BUTTON_Z       = 0x2000;
static constexpr uint16_t N64_BUTTON_START   = 0x1000;
static constexpr uint16_t N64_DPAD_UP        = 0x0800;
static constexpr uint16_t N64_DPAD_DOWN      = 0x0400;
static constexpr uint16_t N64_DPAD_LEFT      = 0x0200;
static constexpr uint16_t N64_DPAD_RIGHT     = 0x0100;
static constexpr uint16_t N64_BUTTON_L       = 0x0020;
static constexpr uint16_t N64_BUTTON_R       = 0x0010;
static constexpr uint16_t N64_CBUTTON_UP     = 0x0008;
static constexpr uint16_t N64_CBUTTON_DOWN   = 0x0004;
static constexpr uint16_t N64_CBUTTON_LEFT   = 0x0002;
static constexpr uint16_t N64_CBUTTON_RIGHT  = 0x0001;

struct ControlsConfig {
    std::array<uint16_t, SDL_CONTROLLER_BUTTON_MAX> buttons{};
    uint16_t left_trigger = 0;
    uint16_t right_trigger = 0;
    int trigger_threshold = 12000;
    bool right_stick_to_dpad = true;
    bool right_stick_invert_x = true;
    bool right_stick_invert_y = true;
    float right_stick_deadzone = 0.5f;
};

static std::string normalized_config_key(std::string value) {
    for (char& ch : value) {
        if (ch == '-' || ch == ' ' || ch == '/') {
            ch = '_';
        } else {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
    }
    return value;
}

static bool n64_button_from_string(std::string value, uint16_t& out) {
    value = normalized_config_key(std::move(value));
    if (value == "none" || value == "unmapped" || value == "off") { out = 0; return true; }
    if (value == "n64_a" || value == "a") { out = N64_BUTTON_A; return true; }
    if (value == "n64_b" || value == "b") { out = N64_BUTTON_B; return true; }
    if (value == "n64_z" || value == "z") { out = N64_BUTTON_Z; return true; }
    if (value == "n64_start" || value == "start") { out = N64_BUTTON_START; return true; }
    if (value == "n64_d_up" || value == "n64_dpad_up" || value == "dpad_up") { out = N64_DPAD_UP; return true; }
    if (value == "n64_d_down" || value == "n64_dpad_down" || value == "dpad_down") { out = N64_DPAD_DOWN; return true; }
    if (value == "n64_d_left" || value == "n64_dpad_left" || value == "dpad_left") { out = N64_DPAD_LEFT; return true; }
    if (value == "n64_d_right" || value == "n64_dpad_right" || value == "dpad_right") { out = N64_DPAD_RIGHT; return true; }
    if (value == "n64_l" || value == "l") { out = N64_BUTTON_L; return true; }
    if (value == "n64_r" || value == "r") { out = N64_BUTTON_R; return true; }
    if (value == "n64_c_up" || value == "c_up" || value == "cbutton_up") { out = N64_CBUTTON_UP; return true; }
    if (value == "n64_c_down" || value == "c_down" || value == "cbutton_down") { out = N64_CBUTTON_DOWN; return true; }
    if (value == "n64_c_left" || value == "c_left" || value == "cbutton_left") { out = N64_CBUTTON_LEFT; return true; }
    if (value == "n64_c_right" || value == "c_right" || value == "cbutton_right") { out = N64_CBUTTON_RIGHT; return true; }
    return false;
}

static const char* n64_button_to_string(uint16_t value) {
    switch (value) {
        case 0: return "none";
        case N64_BUTTON_A: return "n64_a";
        case N64_BUTTON_B: return "n64_b";
        case N64_BUTTON_Z: return "n64_z";
        case N64_BUTTON_START: return "n64_start";
        case N64_DPAD_UP: return "n64_dpad_up";
        case N64_DPAD_DOWN: return "n64_dpad_down";
        case N64_DPAD_LEFT: return "n64_dpad_left";
        case N64_DPAD_RIGHT: return "n64_dpad_right";
        case N64_BUTTON_L: return "n64_l";
        case N64_BUTTON_R: return "n64_r";
        case N64_CBUTTON_UP: return "n64_c_up";
        case N64_CBUTTON_DOWN: return "n64_c_down";
        case N64_CBUTTON_LEFT: return "n64_c_left";
        case N64_CBUTTON_RIGHT: return "n64_c_right";
        default: return "none";
    }
}

static bool gamepad_button_from_string(std::string value, SDL_GameControllerButton& out) {
    value = normalized_config_key(std::move(value));
    if (value == "a" || value == "cross") { out = SDL_CONTROLLER_BUTTON_A; return true; }
    if (value == "b" || value == "circle") { out = SDL_CONTROLLER_BUTTON_B; return true; }
    if (value == "x" || value == "square") { out = SDL_CONTROLLER_BUTTON_X; return true; }
    if (value == "y" || value == "triangle") { out = SDL_CONTROLLER_BUTTON_Y; return true; }
    if (value == "back" || value == "select") { out = SDL_CONTROLLER_BUTTON_BACK; return true; }
    if (value == "guide" || value == "home") { out = SDL_CONTROLLER_BUTTON_GUIDE; return true; }
    if (value == "start" || value == "options") { out = SDL_CONTROLLER_BUTTON_START; return true; }
    if (value == "left_stick" || value == "left_stick_click" || value == "l3") { out = SDL_CONTROLLER_BUTTON_LEFTSTICK; return true; }
    if (value == "right_stick" || value == "right_stick_click" || value == "r3") { out = SDL_CONTROLLER_BUTTON_RIGHTSTICK; return true; }
    if (value == "left_bumper" || value == "left_shoulder" || value == "l1" || value == "lb") { out = SDL_CONTROLLER_BUTTON_LEFTSHOULDER; return true; }
    if (value == "right_bumper" || value == "right_shoulder" || value == "r1" || value == "rb") { out = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER; return true; }
    if (value == "dpad_up") { out = SDL_CONTROLLER_BUTTON_DPAD_UP; return true; }
    if (value == "dpad_down") { out = SDL_CONTROLLER_BUTTON_DPAD_DOWN; return true; }
    if (value == "dpad_left") { out = SDL_CONTROLLER_BUTTON_DPAD_LEFT; return true; }
    if (value == "dpad_right") { out = SDL_CONTROLLER_BUTTON_DPAD_RIGHT; return true; }
    return false;
}

static const char* gamepad_button_to_string(SDL_GameControllerButton button) {
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A: return "a";
        case SDL_CONTROLLER_BUTTON_B: return "b";
        case SDL_CONTROLLER_BUTTON_X: return "x";
        case SDL_CONTROLLER_BUTTON_Y: return "y";
        case SDL_CONTROLLER_BUTTON_BACK: return "back";
        case SDL_CONTROLLER_BUTTON_GUIDE: return "guide";
        case SDL_CONTROLLER_BUTTON_START: return "start";
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: return "left_stick";
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return "right_stick";
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return "left_bumper";
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return "right_bumper";
        case SDL_CONTROLLER_BUTTON_DPAD_UP: return "dpad_up";
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return "dpad_down";
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return "dpad_left";
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return "dpad_right";
        default: return nullptr;
    }
}

static ControlsConfig default_controls_config() {
    ControlsConfig config{};
    config.buttons[SDL_CONTROLLER_BUTTON_A] = N64_BUTTON_A;              // jump
    config.buttons[SDL_CONTROLLER_BUTTON_X] = N64_BUTTON_B;              // attack 1 / primary
    config.buttons[SDL_CONTROLLER_BUTTON_Y] = N64_CBUTTON_LEFT;          // attack 2 / secondary
    config.buttons[SDL_CONTROLLER_BUTTON_B] = N64_CBUTTON_RIGHT;         // collect / interact
    config.buttons[SDL_CONTROLLER_BUTTON_START] = N64_BUTTON_START;
    config.buttons[SDL_CONTROLLER_BUTTON_DPAD_UP] = N64_DPAD_UP;
    config.buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = N64_DPAD_DOWN;
    config.buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = N64_DPAD_LEFT;
    config.buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = N64_DPAD_RIGHT;
    config.buttons[SDL_CONTROLLER_BUTTON_LEFTSTICK] = N64_BUTTON_R;      // lock-on
    config.buttons[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = N64_CBUTTON_DOWN; // throw item
    config.left_trigger = N64_BUTTON_Z;
    config.right_trigger = N64_BUTTON_L;
    config.trigger_threshold = 12000;
    config.right_stick_to_dpad = true;
    config.right_stick_invert_x = true;
    config.right_stick_invert_y = true;
    config.right_stick_deadzone = 0.5f;
    return config;
}

static ControlsConfig g_controls_config = default_controls_config();

static nlohmann::json controls_config_to_json(const ControlsConfig& config) {
    nlohmann::json buttons = nlohmann::json::object();
    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
        const char* name = gamepad_button_to_string(static_cast<SDL_GameControllerButton>(i));
        if (name != nullptr) {
            buttons[name] = n64_button_to_string(config.buttons[i]);
        }
    }

    return nlohmann::json{
        {"gamepad", {
            {"buttons", buttons},
            {"axes", {
                {"left_trigger", n64_button_to_string(config.left_trigger)},
                {"right_trigger", n64_button_to_string(config.right_trigger)},
                {"trigger_threshold", config.trigger_threshold},
            }},
            {"right_stick", {
                {"mode", config.right_stick_to_dpad ? "n64_dpad" : "none"},
                {"invert_x", config.right_stick_invert_x},
                {"invert_y", config.right_stick_invert_y},
                {"deadzone", config.right_stick_deadzone},
            }},
        }},
    };
}

static void read_controls_button_binding(const nlohmann::json& buttons,
                                         const char* key,
                                         ControlsConfig& config) {
    auto it = buttons.find(key);
    if (it == buttons.end() || !it->is_string()) {
        return;
    }

    SDL_GameControllerButton button{};
    uint16_t n64_button = 0;
    if (!gamepad_button_from_string(key, button)) {
        fprintf(stderr, "[CONFIG] Ignoring unknown gamepad button key '%s'\n", key);
        return;
    }
    if (!n64_button_from_string(it->get<std::string>(), n64_button)) {
        fprintf(stderr, "[CONFIG] Ignoring invalid N64 button binding '%s' for '%s'\n",
                it->get<std::string>().c_str(), key);
        return;
    }

    config.buttons[button] = n64_button;
}

static void read_controls_trigger_binding(const nlohmann::json& axes,
                                          const char* key,
                                          uint16_t& out) {
    auto it = axes.find(key);
    if (it == axes.end() || !it->is_string()) {
        return;
    }

    uint16_t n64_button = 0;
    if (!n64_button_from_string(it->get<std::string>(), n64_button)) {
        fprintf(stderr, "[CONFIG] Ignoring invalid N64 axis binding '%s' for '%s'\n",
                it->get<std::string>().c_str(), key);
        return;
    }
    out = n64_button;
}

static ControlsConfig controls_config_from_json(const nlohmann::json& json) {
    auto config = default_controls_config();

    auto gamepad_it = json.find("gamepad");
    if (gamepad_it == json.end() || !gamepad_it->is_object()) {
        return config;
    }

    auto buttons_it = gamepad_it->find("buttons");
    if (buttons_it != gamepad_it->end() && buttons_it->is_object()) {
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
            const char* name = gamepad_button_to_string(static_cast<SDL_GameControllerButton>(i));
            if (name != nullptr) {
                read_controls_button_binding(*buttons_it, name, config);
            }
        }
    }

    auto axes_it = gamepad_it->find("axes");
    if (axes_it != gamepad_it->end() && axes_it->is_object()) {
        read_controls_trigger_binding(*axes_it, "left_trigger", config.left_trigger);
        read_controls_trigger_binding(*axes_it, "right_trigger", config.right_trigger);

        auto threshold_it = axes_it->find("trigger_threshold");
        if (threshold_it != axes_it->end() && threshold_it->is_number_integer()) {
            const int value = threshold_it->get<int>();
            if (value >= 0 && value <= 32767) {
                config.trigger_threshold = value;
            }
        }
    }

    auto right_stick_it = gamepad_it->find("right_stick");
    if (right_stick_it != gamepad_it->end() && right_stick_it->is_object()) {
        auto mode_it = right_stick_it->find("mode");
        if (mode_it != right_stick_it->end() && mode_it->is_string()) {
            const std::string mode = normalized_config_key(mode_it->get<std::string>());
            if (mode == "none" || mode == "off") {
                config.right_stick_to_dpad = false;
            } else if (mode == "n64_dpad" || mode == "dpad") {
                config.right_stick_to_dpad = true;
            } else {
                fprintf(stderr, "[CONFIG] Ignoring invalid right_stick mode '%s'\n",
                        mode_it->get<std::string>().c_str());
            }
        }
        if (auto it = right_stick_it->find("invert_x"); it != right_stick_it->end() && it->is_boolean()) {
            config.right_stick_invert_x = it->get<bool>();
        }
        if (auto it = right_stick_it->find("invert_y"); it != right_stick_it->end() && it->is_boolean()) {
            config.right_stick_invert_y = it->get<bool>();
        }
        if (auto it = right_stick_it->find("deadzone"); it != right_stick_it->end() && it->is_number()) {
            const float value = it->get<float>();
            if (value >= 0.0f && value <= 1.0f) {
                config.right_stick_deadzone = value;
            }
        }
    }

    return config;
}

static void save_controls_config(const ControlsConfig& config) {
    if (g_config_path.empty()) {
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(g_config_path, ec);

    std::ofstream f(controls_config_path());
    if (!f) {
        fprintf(stderr, "[CONFIG] Failed to write %s\n", controls_config_path().string().c_str());
        return;
    }
    f << controls_config_to_json(config).dump(2) << "\n";
}

static ControlsConfig load_controls_config() {
    auto config = default_controls_config();

    std::ifstream f(controls_config_path());
    if (!f) {
        save_controls_config(config);
        return config;
    }

    try {
        nlohmann::json json;
        f >> json;
        config = controls_config_from_json(json);
    } catch (const std::exception& e) {
        fprintf(stderr, "[CONFIG] Failed to parse %s: %s; using control defaults\n",
                controls_config_path().string().c_str(), e.what());
        config = default_controls_config();
    }

    save_controls_config(config);
    return config;
}

static ultramodern::renderer::GraphicsConfig default_graphics_config() {
    ultramodern::renderer::GraphicsConfig config{};
    config.developer_mode = false;
    config.res_option = ultramodern::renderer::Resolution::Original2x;
    config.wm_option = ultramodern::renderer::WindowMode::Windowed;
    config.hr_option = ultramodern::renderer::HUDRatioMode::Original;
    config.api_option = ultramodern::renderer::GraphicsApi::Auto;
    config.ar_option = ultramodern::renderer::AspectRatio::Original;
    config.msaa_option = ultramodern::renderer::Antialiasing::None;
    config.rr_option = ultramodern::renderer::RefreshRate::Original;
    config.hpfb_option = ultramodern::renderer::HighPrecisionFramebuffer::Auto;
    config.rr_manual_value = 60;
    config.ds_option = 1;
    return config;
}

static nlohmann::json graphics_config_to_json(const ultramodern::renderer::GraphicsConfig& config) {
    return nlohmann::json{
        {"developer_mode", config.developer_mode},
        {"resolution", config.res_option},
        {"window_mode", config.wm_option},
        {"hud_ratio", config.hr_option},
        {"graphics_api", config.api_option},
        {"aspect_ratio", config.ar_option},
        {"antialiasing", config.msaa_option},
        {"refresh_rate", config.rr_option},
        {"high_precision_framebuffer", config.hpfb_option},
        {"refresh_rate_manual", config.rr_manual_value},
        {"downsample", config.ds_option},
    };
}

template <typename Enum>
static void read_graphics_enum(const nlohmann::json& json, const char* key, Enum& out) {
    auto it = json.find(key);
    if (it == json.end()) {
        return;
    }

    try {
        Enum value = it->get<Enum>();
        int raw = static_cast<int>(value);
        if (raw >= 0 && raw < static_cast<int>(Enum::OptionCount)) {
            out = value;
        }
    } catch (const std::exception& e) {
        fprintf(stderr, "[CONFIG] Ignoring invalid graphics option %s: %s\n", key, e.what());
    }
}

static ultramodern::renderer::GraphicsConfig graphics_config_from_json(const nlohmann::json& json) {
    auto config = default_graphics_config();

    if (auto it = json.find("developer_mode"); it != json.end() && it->is_boolean()) {
        config.developer_mode = it->get<bool>();
    }
    read_graphics_enum(json, "resolution", config.res_option);
    read_graphics_enum(json, "window_mode", config.wm_option);
    read_graphics_enum(json, "hud_ratio", config.hr_option);
    read_graphics_enum(json, "graphics_api", config.api_option);
    read_graphics_enum(json, "aspect_ratio", config.ar_option);
    read_graphics_enum(json, "antialiasing", config.msaa_option);
    read_graphics_enum(json, "refresh_rate", config.rr_option);
    read_graphics_enum(json, "high_precision_framebuffer", config.hpfb_option);

    if (auto it = json.find("refresh_rate_manual"); it != json.end() && it->is_number_integer()) {
        int value = it->get<int>();
        if (value >= 20 && value <= 360) {
            config.rr_manual_value = value;
        }
    }
    if (auto it = json.find("downsample"); it != json.end() && it->is_number_integer()) {
        int value = it->get<int>();
        if (value >= 1 && value <= 8) {
            config.ds_option = value;
        }
    }

    return config;
}

static void save_graphics_config(const ultramodern::renderer::GraphicsConfig& config) {
    if (g_config_path.empty()) {
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(g_config_path, ec);

    std::ofstream f(graphics_config_path());
    if (!f) {
        fprintf(stderr, "[CONFIG] Failed to write %s\n", graphics_config_path().string().c_str());
        return;
    }
    f << graphics_config_to_json(config).dump(2) << "\n";
}

static ultramodern::renderer::GraphicsConfig load_graphics_config() {
    auto config = default_graphics_config();

    std::ifstream f(graphics_config_path());
    if (!f) {
        save_graphics_config(config);
        return config;
    }

    try {
        nlohmann::json json;
        f >> json;
        config = graphics_config_from_json(json);
    } catch (const std::exception& e) {
        fprintf(stderr, "[CONFIG] Failed to parse %s: %s; using defaults\n",
                graphics_config_path().string().c_str(), e.what());
        config = default_graphics_config();
    }

    save_graphics_config(config);
    return config;
}

template <typename Enum>
static Enum next_graphics_option(Enum value) {
    int raw = static_cast<int>(value) + 1;
    int count = static_cast<int>(Enum::OptionCount);
    if (raw >= count) {
        raw = 0;
    }
    return static_cast<Enum>(raw);
}

static const char* graphics_resolution_name(ultramodern::renderer::Resolution value) {
    switch (value) {
        case ultramodern::renderer::Resolution::Original: return "Original";
        case ultramodern::renderer::Resolution::Original2x: return "Original2x";
        case ultramodern::renderer::Resolution::Auto: return "Auto";
        default: return "Unknown";
    }
}

static const char* graphics_window_mode_name(ultramodern::renderer::WindowMode value) {
    switch (value) {
        case ultramodern::renderer::WindowMode::Windowed: return "Windowed";
        case ultramodern::renderer::WindowMode::Fullscreen: return "Fullscreen";
        default: return "Unknown";
    }
}

static const char* graphics_aspect_name(ultramodern::renderer::AspectRatio value) {
    switch (value) {
        case ultramodern::renderer::AspectRatio::Original: return "Original";
        case ultramodern::renderer::AspectRatio::Expand: return "Expand";
        case ultramodern::renderer::AspectRatio::Manual: return "Manual";
        default: return "Unknown";
    }
}

static const char* graphics_msaa_name(ultramodern::renderer::Antialiasing value) {
    switch (value) {
        case ultramodern::renderer::Antialiasing::None: return "None";
        case ultramodern::renderer::Antialiasing::MSAA2X: return "MSAA2X";
        case ultramodern::renderer::Antialiasing::MSAA4X: return "MSAA4X";
        case ultramodern::renderer::Antialiasing::MSAA8X: return "MSAA8X";
        default: return "Unknown";
    }
}

static const char* graphics_refresh_name(ultramodern::renderer::RefreshRate value) {
    switch (value) {
        case ultramodern::renderer::RefreshRate::Original: return "Original";
        case ultramodern::renderer::RefreshRate::Display: return "Display";
        case ultramodern::renderer::RefreshRate::Manual: return "Manual";
        default: return "Unknown";
    }
}

static void apply_and_save_graphics_config(const ultramodern::renderer::GraphicsConfig& config,
                                           const char* reason) {
    ultramodern::renderer::set_graphics_config(config);
    save_graphics_config(config);
    fprintf(stderr,
            "[CONFIG] %s: resolution=%s window=%s aspect=%s msaa=%s refresh=%s manual=%d\n",
            reason,
            graphics_resolution_name(config.res_option),
            graphics_window_mode_name(config.wm_option),
            graphics_aspect_name(config.ar_option),
            graphics_msaa_name(config.msaa_option),
            graphics_refresh_name(config.rr_option),
            config.rr_manual_value);
}

static bool handle_graphics_hotkey(SDL_Keycode key) {
    auto config = ultramodern::renderer::get_graphics_config();

    switch (key) {
        case SDLK_F1:
            lod::ui::toggle_overlay();
            fprintf(stderr, "[UI] F1 overlay %s\n", lod::ui::overlay_visible() ? "shown" : "hidden");
            return true;
        case SDLK_F11:
            config.wm_option = config.wm_option == ultramodern::renderer::WindowMode::Fullscreen
                ? ultramodern::renderer::WindowMode::Windowed
                : ultramodern::renderer::WindowMode::Fullscreen;
            apply_and_save_graphics_config(config, "F11");
            lod::ui::notify_graphics_config_changed();
            return true;
        case SDLK_F5:
            config.res_option = next_graphics_option(config.res_option);
            apply_and_save_graphics_config(config, "F5");
            lod::ui::notify_graphics_config_changed();
            return true;
        case SDLK_F6:
            config.ar_option = next_graphics_option(config.ar_option);
            apply_and_save_graphics_config(config, "F6");
            lod::ui::notify_graphics_config_changed();
            return true;
        case SDLK_F7:
            config.msaa_option = next_graphics_option(config.msaa_option);
            apply_and_save_graphics_config(config, "F7");
            lod::ui::notify_graphics_config_changed();
            return true;
        case SDLK_F8:
            config.rr_option = next_graphics_option(config.rr_option);
            apply_and_save_graphics_config(config, "F8");
            lod::ui::notify_graphics_config_changed();
            return true;
        default:
            return false;
    }
}

static void open_first_game_controller() {
    if (game_controller != nullptr) {
        return;
    }

    const int joystick_count = SDL_NumJoysticks();
    for (int i = 0; i < joystick_count; i++) {
        if (!SDL_IsGameController(i)) {
            continue;
        }

        SDL_GameController* opened = SDL_GameControllerOpen(i);
        if (opened == nullptr) {
            fprintf(stderr, "[INPUT] failed to open game controller %d: %s\n", i, SDL_GetError());
            continue;
        }

        SDL_Joystick* joystick = SDL_GameControllerGetJoystick(opened);
        game_controller = opened;
        game_controller_instance = joystick != nullptr ? SDL_JoystickInstanceID(joystick) : -1;
        const char* controller_name = SDL_GameControllerName(opened);
        lod::ui::set_connected_controller_name(controller_name != nullptr ? controller_name : "Unknown controller");
        fprintf(stderr, "[INPUT] opened game controller: %s\n", controller_name != nullptr ? controller_name : "Unknown controller");
        return;
    }
}

static void close_game_controller() {
    if (game_controller != nullptr) {
        fprintf(stderr, "[INPUT] closed game controller\n");
        SDL_GameControllerClose(game_controller);
        game_controller = nullptr;
        game_controller_instance = -1;
        lod::ui::set_connected_controller_name("None detected");
    }
}

ultramodern::renderer::WindowHandle create_window(ultramodern::gfx_callbacks_t::gfx_data_t) {
    uint32_t flags = SDL_WINDOW_RESIZABLE;

#if defined(__APPLE__)
    flags |= SDL_WINDOW_METAL;
#elif defined(RT64_SDL_WINDOW_VULKAN)
    flags |= SDL_WINDOW_VULKAN;
#endif

    window = SDL_CreateWindow("Castlevania: Legacy of Darkness Recompiled",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 960, flags);

    if (window == nullptr) {
        exit_error("Failed to create window: %s\n", SDL_GetError());
    }

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

#if defined(_WIN32)
    return ultramodern::renderer::WindowHandle{ wmInfo.info.win.window, GetCurrentThreadId() };
#elif defined(__linux__) || defined(__ANDROID__)
    return ultramodern::renderer::WindowHandle{ window };
#elif defined(__APPLE__)
    SDL_MetalView view = SDL_Metal_CreateView(window);
    return ultramodern::renderer::WindowHandle{ wmInfo.info.cocoa.window, SDL_Metal_GetLayer(view) };
#else
    static_assert(false && "Unsupported platform");
#endif
}

void update_gfx(void*) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                ultramodern::quit();
                return;
            case SDL_CONTROLLERDEVICEADDED:
                open_first_game_controller();
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                if (event.cdevice.which == game_controller_instance) {
                    close_game_controller();
                    open_first_game_controller();
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.repeat == 0 && handle_graphics_hotkey(event.key.keysym.sym)) {
                    break;
                }
                if (lod::ui::queue_platform_event(event)) {
                    break;
                }
                break;
            case SDL_KEYUP:
            case SDL_TEXTINPUT:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERAXISMOTION:
            case SDL_WINDOWEVENT:
                if (lod::ui::queue_platform_event(event)) {
                    break;
                }
                break;
        }
    }
}

// ── Audio ───────────────────────────────────────────────────────────

static SDL_AudioDeviceID audio_device = 0;
static SDL_AudioCVT audio_convert;
static uint32_t sample_rate = 48000;
static uint32_t output_sample_rate = 48000;
constexpr uint32_t input_channels = 2;
static uint32_t output_channels = 2;
constexpr uint32_t duplicated_input_frames = 4;
static uint32_t discarded_output_frames;
constexpr uint32_t bytes_per_frame = input_channels * sizeof(float);

void update_audio_converter() {
    SDL_BuildAudioCVT(&audio_convert, AUDIO_F32, input_channels, sample_rate,
                       AUDIO_F32, output_channels, output_sample_rate);
    discarded_output_frames = duplicated_input_frames * output_sample_rate / sample_rate;
}

void queue_samples(int16_t* audio_data, size_t sample_count) {
    static std::vector<float> swap_buffer;
    static std::array<float, duplicated_input_frames * input_channels> duplicated_sample_buffer;

#if LOD_ENABLE_AUDIO_TRACE
    static uint32_t queue_count = 0;
    queue_count++;
    int16_t min_sample = 0;
    int16_t max_sample = 0;
    size_t nonzero_count = 0;
    if (sample_count > 0) {
        min_sample = audio_data[0];
        max_sample = audio_data[0];
        for (size_t i = 0; i < sample_count; i++) {
            if (audio_data[i] < min_sample) min_sample = audio_data[i];
            if (audio_data[i] > max_sample) max_sample = audio_data[i];
            if (audio_data[i] != 0) nonzero_count++;
        }
    }
    bool should_log = queue_count <= 40 || (queue_count % 120) == 0 || nonzero_count == 0 ||
                      min_sample < -100 || max_sample > 100;
    if (should_log) {
        fprintf(stderr,
                "[AUDIO] queue_samples#%u samples=%zu frames=%zu freq=%u out=%u "
                "min=%d max=%d nonzero=%zu queued_before=%u device=%u\n",
                queue_count, sample_count, sample_count / input_channels,
                sample_rate, output_sample_rate, (int)min_sample, (int)max_sample,
                nonzero_count, SDL_GetQueuedAudioSize(audio_device), audio_device);
    }
#endif

#if LOD_ENABLE_AUDIO_RAW_DUMP
    {
        static FILE* raw_dump = []() -> FILE* {
            (void)std::remove("/tmp/lod_audio_queue_s16le.raw");
            return std::fopen("/tmp/lod_audio_queue_s16le.raw", "ab");
        }();
        if (raw_dump != nullptr && sample_count > 0) {
            std::fwrite(audio_data, sizeof(int16_t), sample_count, raw_dump);
            std::fflush(raw_dump);
        }
    }
#endif

    size_t resampled_sample_count = sample_count + duplicated_input_frames * input_channels;
    size_t max_sample_count = std::max(resampled_sample_count, resampled_sample_count * audio_convert.len_mult);
    if (max_sample_count > swap_buffer.size()) {
        swap_buffer.resize(max_sample_count);
    }

    for (size_t i = 0; i < duplicated_input_frames * input_channels; i++) {
        swap_buffer[i] = duplicated_sample_buffer[i];
    }

    for (size_t i = 0; i < sample_count; i += input_channels) {
        swap_buffer[i + 0 + duplicated_input_frames * input_channels] = audio_data[i + 1] * (0.5f / 32768.0f);
        swap_buffer[i + 1 + duplicated_input_frames * input_channels] = audio_data[i + 0] * (0.5f / 32768.0f);
    }

    assert(sample_count > duplicated_input_frames * input_channels);

    for (size_t i = 0; i < duplicated_input_frames * input_channels; i++) {
        duplicated_sample_buffer[i] = swap_buffer[i + sample_count];
    }

    audio_convert.buf = reinterpret_cast<Uint8*>(swap_buffer.data());
    audio_convert.len = (sample_count + duplicated_input_frames * input_channels) * sizeof(swap_buffer[0]);
    SDL_ConvertAudio(&audio_convert);

    uint32_t num_bytes_to_queue = audio_convert.len_cvt - output_channels * discarded_output_frames * sizeof(swap_buffer[0]);
    float* samples_to_queue = swap_buffer.data() + output_channels * discarded_output_frames / 2;

    SDL_QueueAudio(audio_device, samples_to_queue, num_bytes_to_queue);
#if LOD_ENABLE_AUDIO_TRACE
    if (should_log) {
        fprintf(stderr,
                "[AUDIO] queue_samples#%u queued_bytes=%u queued_after=%u\n",
                queue_count, num_bytes_to_queue, SDL_GetQueuedAudioSize(audio_device));
    }
#endif
}

size_t get_frames_remaining() {
    constexpr float buffer_offset_frames = 1.0f;
    uint64_t buffered_byte_count = SDL_GetQueuedAudioSize(audio_device);
    buffered_byte_count = buffered_byte_count * 2 * sample_rate / output_sample_rate / output_channels;

    uint32_t frames_per_vi = (sample_rate / 60);
    if (buffered_byte_count > (buffer_offset_frames * bytes_per_frame * frames_per_vi)) {
        buffered_byte_count -= (buffer_offset_frames * bytes_per_frame * frames_per_vi);
    } else {
        buffered_byte_count = 0;
    }
    return static_cast<uint32_t>(buffered_byte_count / bytes_per_frame);
}

void set_frequency(uint32_t freq) {
    sample_rate = freq;
    update_audio_converter();
#if LOD_ENABLE_AUDIO_TRACE
    static uint32_t set_frequency_count = 0;
    set_frequency_count++;
    if (set_frequency_count <= 40 || (set_frequency_count % 120) == 0) {
        fprintf(stderr,
                "[AUDIO] set_frequency#%u freq=%u output=%u len_mult=%d discarded=%u\n",
                set_frequency_count, sample_rate, output_sample_rate,
                audio_convert.len_mult, discarded_output_frames);
    }
#endif
}

void reset_audio(uint32_t output_freq) {
    SDL_AudioSpec spec_desired{
        .freq = (int)output_freq,
        .format = AUDIO_F32,
        .channels = (Uint8)output_channels,
        .silence = 0,
        .samples = 0x100,
        .padding = 0,
        .size = 0,
        .callback = nullptr,
        .userdata = nullptr
    };

    audio_device = SDL_OpenAudioDevice(nullptr, false, &spec_desired, nullptr, 0);
    if (audio_device == 0) {
        fprintf(stderr, "[WARN] SDL error opening audio device: %s (continuing without audio)\n", SDL_GetError());
    } else {
        SDL_PauseAudioDevice(audio_device, 0);
#if LOD_ENABLE_AUDIO_TRACE
        fprintf(stderr,
                "[AUDIO] opened SDL audio device id=%u output_freq=%u channels=%u\n",
                audio_device, output_freq, output_channels);
#endif
    }

    output_sample_rate = output_freq;
    update_audio_converter();
#if LOD_ENABLE_AUDIO_TRACE
    fprintf(stderr,
            "[AUDIO] reset_audio output=%u len_mult=%d discarded=%u device=%u\n",
            output_sample_rate, audio_convert.len_mult, discarded_output_frames, audio_device);
#endif
}

// ── RSP ─────────────────────────────────────────────────────────────

extern RspUcodeFunc aspMain;

RspUcodeFunc* get_rsp_microcode(const OSTask* task) {
    switch (task->t.type) {
    case M_AUDTASK:
        return aspMain;
    default:
        fprintf(stderr, "Unknown RSP task type: %" PRIu32 "\n", task->t.type);
        return nullptr;
    }
}

// ── Input (minimal stubs) ───────────────────────────────────────────

void poll_inputs() {
    // SDL event polling happens in update_gfx. Lazily open here too so a
    // controller already connected before boot is available immediately.
    open_first_game_controller();
}

static float normalize_axis(Sint16 value) {
    constexpr float deadzone = 8000.0f;
    const float v = static_cast<float>(value);
    const float abs_v = std::fabs(v);
    if (abs_v <= deadzone) {
        return 0.0f;
    }

    float normalized = (abs_v - deadzone) / (32767.0f - deadzone);
    if (normalized > 1.0f) {
        normalized = 1.0f;
    }
    return v < 0.0f ? -normalized : normalized;
}

bool get_n64_input(int controller_num, uint16_t* buttons, float* x, float* y) {
#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
    {
        static int input_calls = 0; input_calls++;
        if (input_calls <= 3 || (input_calls % 1000 == 0))
            fprintf(stderr, "[INPUT] get_n64_input called #%d, controller=%d\n", input_calls, controller_num);
    }
#endif
    if (controller_num != 0) return false;

    *buttons = 0;
    *x = 0.0f;
    *y = 0.0f;

    if (lod::ui::captures_input()) {
        return true;
    }

    const uint8_t* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_RETURN])  *buttons |= N64_BUTTON_A;
    if (keys[SDL_SCANCODE_RSHIFT])  *buttons |= N64_BUTTON_B;
    if (keys[SDL_SCANCODE_Z])       *buttons |= N64_BUTTON_Z;
    if (keys[SDL_SCANCODE_SPACE])   *buttons |= N64_BUTTON_START;
    if (keys[SDL_SCANCODE_UP])      *buttons |= N64_DPAD_UP;
    if (keys[SDL_SCANCODE_DOWN])    *buttons |= N64_DPAD_DOWN;
    if (keys[SDL_SCANCODE_LEFT])    *buttons |= N64_DPAD_LEFT;
    if (keys[SDL_SCANCODE_RIGHT])   *buttons |= N64_DPAD_RIGHT;

    // Analog stick via WASD
    if (keys[SDL_SCANCODE_W]) *y += 1.0f;
    if (keys[SDL_SCANCODE_S]) *y -= 1.0f;
    if (keys[SDL_SCANCODE_A]) *x -= 1.0f;
    if (keys[SDL_SCANCODE_D]) *x += 1.0f;

    if (game_controller != nullptr) {
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
            if (g_controls_config.buttons[i] != 0 &&
                SDL_GameControllerGetButton(game_controller, static_cast<SDL_GameControllerButton>(i))) {
                *buttons |= g_controls_config.buttons[i];
            }
        }

        if (g_controls_config.left_trigger != 0 &&
            SDL_GameControllerGetAxis(game_controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > g_controls_config.trigger_threshold) {
            *buttons |= g_controls_config.left_trigger;
        }
        if (g_controls_config.right_trigger != 0 &&
            SDL_GameControllerGetAxis(game_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > g_controls_config.trigger_threshold) {
            *buttons |= g_controls_config.right_trigger;
        }

        if (g_controls_config.right_stick_to_dpad) {
            float camera_x = normalize_axis(SDL_GameControllerGetAxis(game_controller, SDL_CONTROLLER_AXIS_RIGHTX));
            float camera_y = normalize_axis(SDL_GameControllerGetAxis(game_controller, SDL_CONTROLLER_AXIS_RIGHTY));
            if (g_controls_config.right_stick_invert_x) {
                camera_x = -camera_x;
            }
            if (g_controls_config.right_stick_invert_y) {
                camera_y = -camera_y;
            }
            const float deadzone = g_controls_config.right_stick_deadzone;
            if (camera_y < -deadzone) *buttons |= N64_DPAD_UP;
            if (camera_y >  deadzone) *buttons |= N64_DPAD_DOWN;
            if (camera_x < -deadzone) *buttons |= N64_DPAD_LEFT;
            if (camera_x >  deadzone) *buttons |= N64_DPAD_RIGHT;
        }

        const float pad_x = normalize_axis(SDL_GameControllerGetAxis(game_controller, SDL_CONTROLLER_AXIS_LEFTX));
        const float pad_y = -normalize_axis(SDL_GameControllerGetAxis(game_controller, SDL_CONTROLLER_AXIS_LEFTY));
        if (std::fabs(pad_x) > std::fabs(*x)) {
            *x = pad_x;
        }
        if (std::fabs(pad_y) > std::fabs(*y)) {
            *y = pad_y;
        }
    }

    return true;
}

void set_rumble(int controller_num, bool rumble) {
    if (controller_num != 0 || game_controller == nullptr) {
        return;
    }

    SDL_GameControllerRumble(game_controller, rumble ? 0x6000 : 0, rumble ? 0xFFFF : 0, rumble ? 200 : 0);
}

ultramodern::input::connected_device_info_t get_connected_device_info(int controller_num) {
    if (controller_num == 0) {
        return { ultramodern::input::Device::Controller, ultramodern::input::Pak::ControllerPak };
    }
    return { ultramodern::input::Device::None, ultramodern::input::Pak::None };
}

// ── Game entry ──────────────────────────────────────────────────────

extern "C" void recomp_entrypoint(uint8_t* rdram, recomp_context* ctx);
gpr get_entrypoint_address();
extern void lod_register_overlays();
extern void lod_on_init(uint8_t* rdram, recomp_context* ctx);

std::vector<recomp::GameEntry> supported_games = {
    {
        .rom_hash = lod::target_rom::kXxh3_64,
        .internal_name = std::string(lod::target_rom::kInternalName),
        .game_id = u8"castlevania2.n64.us",
        .mod_game_id = "",  // Disabled — mod system not yet set up for LoD
        .save_type = recomp::SaveType::Sram, // 32KB — used for virtual Controller Pak
        .is_enabled = false,
        .entrypoint_address = get_entrypoint_address(),
        .entrypoint = recomp_entrypoint,
        .on_init_callback = lod_on_init,
    },
};

std::string get_game_thread_name(const OSThread* t) {
    std::string name = "[Game] ";
    switch (t->id) {
        case 1: name += "IDLE"; break;
        case 3: name += "MAIN"; break;
        case 4: name += "GRAPH"; break;
        case 5: name += "SCHED"; break;
        case 7: name += "PADMGR"; break;
        case 10: name += "AUDIOMGR"; break;
        default: name += std::to_string(t->id); break;
    }
    return name;
}

void message_box_stub(const char* msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
    // Don't show modal dialog — it blocks startup and the game window
    // may not be visible yet. Just log to stderr.
}

// ── ROM loading ─────────────────────────────────────────────────────

namespace lod {
std::filesystem::path get_runtime_rom_path() {
    std::filesystem::path cwd_rom = "rom.z64";
    if (std::filesystem::exists(cwd_rom)) {
        return cwd_rom;
    }

#ifdef __APPLE__
    // When launched as LodRecomp.app, prefer a user-provided ROM next to the
    // .app bundle: LodRecomp.app and rom.z64 in the same folder.
    if (CFBundleRef bundle = CFBundleGetMainBundle()) {
        if (CFURLRef bundle_url = CFBundleCopyBundleURL(bundle)) {
            char path_buf[PATH_MAX] = {};
            if (CFURLGetFileSystemRepresentation(bundle_url, true,
                                                 reinterpret_cast<UInt8*>(path_buf),
                                                 sizeof(path_buf))) {
                std::filesystem::path bundle_path(path_buf);
                std::filesystem::path beside_app = bundle_path.parent_path() / "rom.z64";
                if (std::filesystem::exists(beside_app)) {
                    CFRelease(bundle_url);
                    return beside_app;
                }

                std::filesystem::path bundled_rom =
                    bundle_path / "Contents" / "Resources" / "rom.z64";
                if (std::filesystem::exists(bundled_rom)) {
                    CFRelease(bundle_url);
                    return bundled_rom;
                }
            }
            CFRelease(bundle_url);
        }
    }
#endif

    return {};
}
}

static std::filesystem::path stored_rom_path_file(const std::filesystem::path& config_path) {
    return config_path / "rom_path.txt";
}

static std::filesystem::path read_stored_rom_path(const std::filesystem::path& config_path) {
    std::ifstream f(stored_rom_path_file(config_path));
    if (!f) {
        return {};
    }

    std::string path;
    std::getline(f, path);
    if (path.empty()) {
        return {};
    }

    std::filesystem::path stored(path);
    if (std::filesystem::exists(stored)) {
        return stored;
    }

    fprintf(stderr, "[LodRecomp] Stored ROM path no longer exists: %s\n", path.c_str());
    return {};
}

static void write_stored_rom_path(const std::filesystem::path& config_path,
                                  const std::filesystem::path& rom_path) {
    std::ofstream f(stored_rom_path_file(config_path));
    if (!f) {
        fprintf(stderr, "[LodRecomp] Failed to save ROM path in %s\n",
                config_path.string().c_str());
        return;
    }
    f << rom_path.string() << "\n";
}

static std::filesystem::path prompt_for_rom_path(const std::filesystem::path& config_path) {
    fprintf(stderr,
            "[LodRecomp] No ROM found automatically. Asking user to select a stock LoD ROM.\n");

    if (NFD_Init() != NFD_OKAY) {
        fprintf(stderr, "[LodRecomp] Failed to initialize native file dialog: %s\n",
                NFD_GetError());
        return {};
    }

    nfdchar_t* out_path = nullptr;
    nfdfilteritem_t filters[] = {
        { "Nintendo 64 ROM", "z64,n64,v64" },
    };

    std::string default_path;
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif
    if (home) {
        std::filesystem::path downloads = std::filesystem::path(home) / "Downloads";
        if (std::filesystem::exists(downloads)) {
            default_path = downloads.string();
        }
    }

    nfdresult_t result = NFD_OpenDialog(&out_path, filters, 1,
        default_path.empty() ? nullptr : default_path.c_str());

    std::filesystem::path selected;
    if (result == NFD_OKAY) {
        selected = std::filesystem::path(out_path);
        NFD_FreePath(out_path);
        if (std::filesystem::exists(selected)) {
            write_stored_rom_path(config_path, selected);
            fprintf(stderr, "[LodRecomp] Selected ROM: %s\n", selected.string().c_str());
        } else {
            fprintf(stderr, "[LodRecomp] Selected ROM does not exist: %s\n",
                    selected.string().c_str());
            selected.clear();
        }
    } else if (result == NFD_CANCEL) {
        fprintf(stderr, "[LodRecomp] ROM selection cancelled.\n");
    } else {
        fprintf(stderr, "[LodRecomp] Native file dialog failed: %s\n", NFD_GetError());
    }

    NFD_Quit();
    return selected;
}

static std::filesystem::path find_rom_path(int argc, char** argv,
                                           const std::filesystem::path& config_path) {
    // Check command line argument first
    if (argc > 1) {
        return std::filesystem::path(argv[1]);
    }

    // Release/default path: keep runtime setup simple for testers.
    // Put the prepared LoD ROM next to the executable/run directory as rom.z64.
    std::filesystem::path default_rom = lod::get_runtime_rom_path();
    if (!default_rom.empty()) {
        return default_rom;
    }

    // macOS may launch quarantined/ad-hoc apps through App Translocation after
    // Security Settings authorization. In that case the app bundle's apparent
    // path is a randomized temp mount, so rom.z64 beside the original app is not
    // visible. Remembering a previously selected ROM path keeps future launches
    // working without asking again.
    std::filesystem::path stored_rom = read_stored_rom_path(config_path);
    if (!stored_rom.empty()) {
        return stored_rom;
    }

    // Look for ROM in current directory
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        auto ext = entry.path().extension().string();
        if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
            auto name = entry.path().stem().string();
            // Match common names for Castlevania LoD
            if (name.find("castlevania") != std::string::npos ||
                name.find("legacy") != std::string::npos ||
                name.find("Castlevania") != std::string::npos ||
                name.find("Legacy") != std::string::npos ||
                name.find("ND4E") != std::string::npos) {
                return entry.path();
            }
        }
    }

    // Fall back to any .z64 file
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        auto ext = entry.path().extension().string();
        if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
            return entry.path();
        }
    }

    // Also search in resources/ subdirectory
    auto search_dir = [](const std::filesystem::path& dir) -> std::filesystem::path {
        // Prefer castlevania-named ROMs
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            auto ext = entry.path().extension().string();
            if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
                auto name = entry.path().stem().string();
                if (name.find("castlevania") != std::string::npos ||
                    name.find("Castlevania") != std::string::npos ||
                    name.find("legacy") != std::string::npos ||
                    name.find("ND4E") != std::string::npos) {
                    return entry.path();
                }
            }
        }
        // Fall back to any ROM
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            auto ext = entry.path().extension().string();
            if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
                return entry.path();
            }
        }
        return {};
    };

    if (std::filesystem::exists("resources")) {
        auto p = search_dir("resources");
        if (!p.empty()) return p;
    }

    return prompt_for_rom_path(config_path);
}

static void auto_start_game(const std::filesystem::path& rom_path) {
    // Give the runtime a moment to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::u8string game_id = u8"castlevania2.n64.us";

    // Try the decompressed ROM first (NI files pre-decompressed for direct DMA)
    std::filesystem::path decomp_rom = "resources/castlevania2_decompressed.z64";
    bool using_decomp = false;
    if (rom_path.filename() != std::filesystem::path("rom.z64") &&
        std::filesystem::exists(decomp_rom) && std::filesystem::file_size(decomp_rom) > 0) {
        auto result = recomp::select_rom(decomp_rom, game_id);
        if (result == recomp::RomValidationError::Good) {
            fprintf(stderr, "[LodRecomp] Using decompressed ROM\n");
            using_decomp = true;
        } else {
            // Force-load the decompressed ROM even if hash doesn't match
            std::ifstream decomp_file(decomp_rom, std::ios::binary);
            if (decomp_file) {
                std::vector<uint8_t> decomp_data((std::istreambuf_iterator<char>(decomp_file)),
                                                  std::istreambuf_iterator<char>());
                recomp::set_rom_contents(std::move(decomp_data));
                fprintf(stderr, "[LodRecomp] Force-loaded decompressed ROM (%zu bytes)\n", recomp::get_rom().size());
                using_decomp = true;
            }
        }
    }
    if (!using_decomp) {
        auto result = recomp::select_rom(rom_path, game_id);
        if (result != recomp::RomValidationError::Good) {
            fprintf(stderr, "ROM validation failed (error %d) for: %s\n", (int)result, rom_path.string().c_str());
            std::ifstream rom_file(rom_path, std::ios::binary);
            if (rom_file) {
                std::vector<uint8_t> rom_data((std::istreambuf_iterator<char>(rom_file)),
                                              std::istreambuf_iterator<char>());
                recomp::set_rom_contents(std::move(rom_data));
                fprintf(stderr, "[LodRecomp] Force-loaded runtime ROM (%zu bytes)\n", recomp::get_rom().size());
            } else if (!recomp::load_stored_rom(game_id)) {
                fprintf(stderr, "No stored ROM found either, cannot start game.\n");
                return;
            } else {
                fprintf(stderr, "[LodRecomp] Using previously stored ROM\n");
            }
        }
    }

    fprintf(stderr, "[LodRecomp] ROM loaded successfully, starting game...\n");
    recomp::start_game(game_id);
}

// ── Signal handling ─────────────────────────────────────────────────

// Declared in overlays.cpp
extern uint32_t trace_ring[];
extern int trace_ring_pos;
extern int trace_total;
extern uint8_t* rdram_ptr_for_debug;

extern "C" {
LodKseg0FaultTraceSnapshot lod_kseg0_fault_trace_snapshot = {};
}

static void report_crash_details(void* fault_addr) {
    if (rdram_ptr_for_debug != nullptr && fault_addr != nullptr) {
        uintptr_t fault = reinterpret_cast<uintptr_t>(fault_addr);
        uintptr_t base = reinterpret_cast<uintptr_t>(rdram_ptr_for_debug);
        if (fault >= base) {
            uint64_t off = static_cast<uint64_t>(fault - base);
            fprintf(stderr, "  Fault offset from rdram base: 0x%llX\n",
                    static_cast<unsigned long long>(off));
            if (off >= 0x80000000ULL && off < 0x90000000ULL) {
                uint64_t kseg0_phys = off - 0x80000000ULL;
                fprintf(stderr, "  KSEG0 mirror access: host_off=0x%llX phys=0x%llX%s\n",
                        static_cast<unsigned long long>(off),
                        static_cast<unsigned long long>(kseg0_phys),
                        kseg0_phys >= 0x800000ULL ? " (past 8MB RDRAM)" : "");
            }
        }
    }
    if (lod_kseg0_fault_trace_snapshot.magic == 0x4B534730) {
        const auto& s = lod_kseg0_fault_trace_snapshot;
        fprintf(stderr,
                "  KSEG0 trace #%u func=0x%08X gs=%u map#%u rom=0x%08X size=0x%X "
                "ra=0x%08X sp=0x%08X a={0x%08X,0x%08X,0x%08X,0x%08X} "
                "v={0x%08X,0x%08X} s0=0x%08X t={0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}\n",
                s.count, s.func_vram, s.gs, s.map_load_count, s.map_rom, s.map_size,
                s.ra, s.sp, s.a0, s.a1, s.a2, s.a3, s.v0, s.v1, s.s0,
                s.t0, s.t1, s.t2, s.t3, s.t4, s.t5, s.t6, s.t7, s.t8, s.t9);
        fprintf(stderr,
                "  KSEG0 trace chunk=0x%08X phys=0x%08X slot=%u pending=%u "
                "entry={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X user=0x%08X}\n",
                s.chunk_addr, s.chunk_phys, s.slot, s.pending,
                s.entry_rom, s.entry_ram, s.entry_size, s.entry_file_id, s.entry_user_ptr);
        fprintf(stderr,
                "  KSEG0 trace cur={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X} "
                "ram_ok=%u active_ram_ok=%u\n",
                s.cur_rom, s.cur_ram, s.cur_size, s.cur_file_id,
                s.cur_ram_ok, s.entry_ram_ok);
        if (s.romcopy_count != 0) {
            fprintf(stderr,
                    "  KSEG0 trace last_dma_romcopy#%u gs=%u rom=0x%08X ram=0x%08X "
                    "size=0x%08X phys=0x%08X ra=0x%08X ram_ok=%u\n",
                    s.romcopy_count, s.romcopy_gs, s.romcopy_rom, s.romcopy_ram,
                    s.romcopy_size, s.romcopy_ram_phys, s.romcopy_ra,
                    s.romcopy_ram_ok);
        }
    }
    // Print last function calls from trace ring
    int count = trace_total < 32 ? trace_total : 32;
    if (count > 0) {
        fprintf(stderr, "  Last %d function lookups:\n", count);
        for (int i = 0; i < count; i++) {
            int idx = (trace_ring_pos - count + i + 32) % 32;
            fprintf(stderr, "    [-%d] 0x%08X\n", count - i, trace_ring[idx]);
        }
    }
}

#ifdef _WIN32
static LONG WINAPI crash_handler(EXCEPTION_POINTERS* ep) {
    const EXCEPTION_RECORD* rec = ep->ExceptionRecord;
    void* fault_addr = nullptr;
    if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2) {
        fault_addr = reinterpret_cast<void*>(rec->ExceptionInformation[1]);
    }
    fprintf(stderr, "\n[CRASH] Exception 0x%08lX at address %p (ip=%p)\n",
            static_cast<unsigned long>(rec->ExceptionCode), fault_addr, rec->ExceptionAddress);
    report_crash_details(fault_addr);
    _exit(127);
    return EXCEPTION_EXECUTE_HANDLER;
}
#else
static void crash_handler(int sig, siginfo_t* info, void* ctx) {
    fprintf(stderr, "\n[CRASH] Signal %d at address %p (code=%d)\n",
            sig, info->si_addr, info->si_code);
#if defined(__APPLE__) || defined(__linux__)
    {
        void* frames[64];
        int frame_count = backtrace(frames, 64);
        fprintf(stderr, "  Native backtrace (%d frames):\n", frame_count);
        backtrace_symbols_fd(frames, frame_count, STDERR_FILENO);
    }
#endif
    report_crash_details(info->si_addr);
    _exit(128 + sig);
}
#endif

static void signal_handler(int sig) {
    fprintf(stderr, "\n[LodRecomp] Caught signal %d, shutting down...\n", sig);
    ultramodern::quit();
    // Give threads a moment to clean up, then force exit
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    _exit(0);
}

// ── Main ────────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    // Catch SIGSEGV/SIGBUS (or the SEH equivalent) to get fault address
#ifdef _WIN32
    SetUnhandledExceptionFilter(crash_handler);
#else
    struct sigaction sa = {};
    sa.sa_sigaction = crash_handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
#endif
    recomp::Version project_version{};
    recomp::Version::from_string("0.1.0", project_version);

    // Set up config path for ROM storage
    std::filesystem::path config_path;
#ifdef __APPLE__
    auto app_support = lod::get_application_support_directory();
    if (app_support) {
        config_path = *app_support / "LodRecomp";
    } else {
        config_path = std::filesystem::path(getenv("HOME")) / ".lodrecomp";
    }
#elif defined(_WIN32)
    if (const char* appdata = std::getenv("APPDATA")) {
        config_path = std::filesystem::path(appdata) / "LodRecomp";
    } else {
        config_path = std::filesystem::path(std::getenv("USERPROFILE") ? std::getenv("USERPROFILE") : ".") / ".lodrecomp";
    }
#else
    config_path = std::filesystem::path(getenv("HOME")) / ".lodrecomp";
#endif
    std::filesystem::create_directories(config_path);
    recomp::register_config_path(config_path);
    g_config_path = config_path;
    lod::ui::set_config_path_display(g_config_path.string());

    auto graphics_config = load_graphics_config();
    ultramodern::renderer::set_graphics_config(graphics_config);
    lod::ui::set_graphics_apply_callback(apply_and_save_graphics_config);
    fprintf(stderr,
            "[CONFIG] Loaded graphics config: resolution=%s window=%s aspect=%s msaa=%s refresh=%s manual=%d\n",
            graphics_resolution_name(graphics_config.res_option),
            graphics_window_mode_name(graphics_config.wm_option),
            graphics_aspect_name(graphics_config.ar_option),
            graphics_msaa_name(graphics_config.msaa_option),
            graphics_refresh_name(graphics_config.rr_option),
            graphics_config.rr_manual_value);

    g_controls_config = load_controls_config();
    fprintf(stderr, "[CONFIG] Loaded controls config: %s\n",
            controls_config_path().string().c_str());

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    reset_audio(48000);

    for (const auto& game : supported_games) {
        recomp::register_game(game);
    }

    lod_register_overlays();

    // Find the ROM
    std::filesystem::path rom_path = find_rom_path(argc, argv, config_path);
    if (rom_path.empty()) {
        fprintf(stderr, "No ROM file found. Pass a .z64 file as an argument, place one in the current directory, or select one when prompted.\n");
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[LodRecomp] Using ROM: %s\n", rom_path.string().c_str());

    if (std::getenv("RECOMP_UI_OPEN_ON_START") != nullptr) {
        lod::ui::show_overlay();
        fprintf(stderr, "[UI] RECOMP_UI_OPEN_ON_START requested; overlay will open after renderer init\n");
    }

    // Launch auto-start thread (will select ROM and start game after runtime initializes)
    std::thread auto_start_thread(auto_start_game, rom_path);
    auto_start_thread.detach();

    recomp::rsp::callbacks_t rsp_callbacks{
        .get_rsp_microcode = get_rsp_microcode,
    };

    ultramodern::renderer::callbacks_t renderer_callbacks{
        .create_render_context = lod::renderer::create_render_context,
    };

    ultramodern::gfx_callbacks_t gfx_callbacks{
        .create_gfx = create_gfx,
        .create_window = create_window,
        .update_gfx = update_gfx,
    };

    ultramodern::audio_callbacks_t audio_callbacks{
        .queue_samples = queue_samples,
        .get_frames_remaining = get_frames_remaining,
        .set_frequency = set_frequency,
    };

    ultramodern::input::callbacks_t input_callbacks{
        .poll_input = poll_inputs,
        .get_input = get_n64_input,
        .set_rumble = set_rumble,
        .get_connected_device_info = get_connected_device_info,
    };

    ultramodern::events::callbacks_t events_callbacks{
        .vi_callback = nullptr,
        .gfx_init_callback = nullptr,
    };

    ultramodern::error_handling::callbacks_t error_handling_callbacks{
        .message_box = message_box_stub,
    };

    ultramodern::threads::callbacks_t threads_callbacks{
        .get_game_thread_name = get_game_thread_name,
    };

    recomp::Configuration config{
        .project_version = project_version,
        .window_handle = {},
        .rsp_callbacks = rsp_callbacks,
        .renderer_callbacks = renderer_callbacks,
        .audio_callbacks = audio_callbacks,
        .input_callbacks = input_callbacks,
        .gfx_callbacks = gfx_callbacks,
        .events_callbacks = events_callbacks,
        .error_handling_callbacks = error_handling_callbacks,
        .threads_callbacks = threads_callbacks,
        .message_queue_control = { .requeue_timer = false, .requeue_sp = true, .requeue_si = true, .requeue_dp = true },
    };

    fprintf(stderr, "[LodRecomp] Calling recomp::start()...\n");
    recomp::start(config);

    fprintf(stderr, "[LodRecomp] recomp::start() returned\n");
    return EXIT_SUCCESS;
}
