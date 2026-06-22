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
#include <sstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <optional>
#include <algorithm>
#include <cmath>
#include <string>
#include <atomic>
#include <mutex>
#include <initializer_list>
#include <cctype>
#include <limits.h>
#include <system_error>
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
#include "lod/lod_settings.hpp"
#include "lod/target_rom.hpp"
#include "lod_ui_overlay.h"
#include "lod_version.h"
#ifdef LOD_USE_ZELDA_MENU
#include "recomp_input.h"
#include "recomp_ui.h"
#include "zelda_support.h"
#endif
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

#ifndef LOD_ENABLE_AUDIO_LATENCY_GUARD
#define LOD_ENABLE_AUDIO_LATENCY_GUARD 0
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

#ifdef LOD_USE_ZELDA_MENU
SDL_Window* window = nullptr;
#else
static SDL_Window* window = nullptr;
#endif
static SDL_GameController* game_controller = nullptr;
static SDL_JoystickID game_controller_instance = -1;
static std::filesystem::path g_config_path;
static bool g_portable_mode = false;
static std::mutex g_audio_config_mutex;
static lod::settings::AudioConfig g_audio_config{};
static std::atomic<int> g_audio_master_volume_percent{100};
static std::atomic_bool g_audio_muted{false};
static std::atomic_bool g_rom_validation_busy{false};
static std::atomic_bool g_rom_game_started{false};
static int g_argc_for_rom_discovery = 0;
static char** g_argv_for_rom_discovery = nullptr;

static std::filesystem::path graphics_config_path() {
    return g_config_path / "graphics.json";
}

static std::filesystem::path audio_config_path() {
    return g_config_path / "audio.json";
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

static const char* n64_button_display_name(uint16_t value) {
    switch (value) {
        case 0: return "None";
        case N64_BUTTON_A: return "N64 A";
        case N64_BUTTON_B: return "N64 B";
        case N64_BUTTON_Z: return "N64 Z";
        case N64_BUTTON_START: return "Start";
        case N64_DPAD_UP: return "D-pad Up";
        case N64_DPAD_DOWN: return "D-pad Down";
        case N64_DPAD_LEFT: return "D-pad Left";
        case N64_DPAD_RIGHT: return "D-pad Right";
        case N64_BUTTON_L: return "N64 L";
        case N64_BUTTON_R: return "N64 R";
        case N64_CBUTTON_UP: return "C-Up";
        case N64_CBUTTON_DOWN: return "C-Down";
        case N64_CBUTTON_LEFT: return "C-Left";
        case N64_CBUTTON_RIGHT: return "C-Right";
        default: return "Unknown";
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

#ifdef LOD_USE_ZELDA_MENU
static bool n64_button_to_game_input(uint16_t n64_button, recomp::GameInput& out) {
    switch (n64_button) {
        case N64_BUTTON_A: out = recomp::GameInput::A; return true;
        case N64_BUTTON_B: out = recomp::GameInput::B; return true;
        case N64_BUTTON_Z: out = recomp::GameInput::Z; return true;
        case N64_BUTTON_START: out = recomp::GameInput::START; return true;
        case N64_BUTTON_L: out = recomp::GameInput::L; return true;
        case N64_BUTTON_R: out = recomp::GameInput::R; return true;
        case N64_CBUTTON_UP: out = recomp::GameInput::C_UP; return true;
        case N64_CBUTTON_DOWN: out = recomp::GameInput::C_DOWN; return true;
        case N64_CBUTTON_LEFT: out = recomp::GameInput::C_LEFT; return true;
        case N64_CBUTTON_RIGHT: out = recomp::GameInput::C_RIGHT; return true;
        case N64_DPAD_UP: out = recomp::GameInput::DPAD_UP; return true;
        case N64_DPAD_DOWN: out = recomp::GameInput::DPAD_DOWN; return true;
        case N64_DPAD_LEFT: out = recomp::GameInput::DPAD_LEFT; return true;
        case N64_DPAD_RIGHT: out = recomp::GameInput::DPAD_RIGHT; return true;
        default: return false;
    }
}

static bool game_input_is_menu_action(recomp::GameInput input) {
    return input == recomp::GameInput::TOGGLE_MENU ||
        input == recomp::GameInput::ACCEPT_MENU ||
        input == recomp::GameInput::APPLY_MENU;
}

static recomp::InputField zelda_controller_button_field(SDL_GameControllerButton button) {
    return {
        static_cast<uint32_t>(recomp::InputType::ControllerDigital),
        static_cast<int32_t>(button),
    };
}

static recomp::InputField zelda_controller_axis_field(SDL_GameControllerAxis axis, bool positive) {
    const int32_t encoded_axis = static_cast<int32_t>(axis) + 1;
    return {
        static_cast<uint32_t>(recomp::InputType::ControllerAnalog),
        positive ? encoded_axis : -encoded_axis,
    };
}

static void clear_recomp_n64_bindings(recomp::InputDevice device) {
    for (size_t input_index = 0; input_index < recomp::get_num_inputs(); input_index++) {
        const auto input = static_cast<recomp::GameInput>(input_index);
        if (game_input_is_menu_action(input)) {
            continue;
        }
        for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
            recomp::set_input_binding(input, binding_index, device, {});
        }
    }
}

static void add_recomp_binding(recomp::InputDevice device, recomp::GameInput input, recomp::InputField field) {
    if (field.input_type == static_cast<uint32_t>(recomp::InputType::None)) {
        return;
    }

    for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
        recomp::InputField& existing = recomp::get_input_binding(input, binding_index, device);
        if (existing.input_type == static_cast<uint32_t>(recomp::InputType::None)) {
            recomp::set_input_binding(input, binding_index, device, field);
            return;
        }
    }

    recomp::set_input_binding(input, 0, device, field);
}

static void add_legacy_n64_binding(recomp::InputDevice device, uint16_t n64_button, recomp::InputField field) {
    recomp::GameInput input{};
    if (n64_button_to_game_input(n64_button, input)) {
        add_recomp_binding(device, input, field);
    }
}

static void apply_controls_config_to_recomp_bindings(const ControlsConfig& config) {
    clear_recomp_n64_bindings(recomp::InputDevice::Controller);

    for (int button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++) {
        add_legacy_n64_binding(
            recomp::InputDevice::Controller,
            config.buttons[button],
            zelda_controller_button_field(static_cast<SDL_GameControllerButton>(button)));
    }

    add_legacy_n64_binding(
        recomp::InputDevice::Controller,
        config.left_trigger,
        zelda_controller_axis_field(SDL_CONTROLLER_AXIS_TRIGGERLEFT, true));
    add_legacy_n64_binding(
        recomp::InputDevice::Controller,
        config.right_trigger,
        zelda_controller_axis_field(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, true));

    if (config.right_stick_to_dpad) {
        add_recomp_binding(
            recomp::InputDevice::Controller,
            recomp::GameInput::DPAD_LEFT,
            zelda_controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTX, config.right_stick_invert_x));
        add_recomp_binding(
            recomp::InputDevice::Controller,
            recomp::GameInput::DPAD_RIGHT,
            zelda_controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTX, !config.right_stick_invert_x));
        add_recomp_binding(
            recomp::InputDevice::Controller,
            recomp::GameInput::DPAD_UP,
            zelda_controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTY, config.right_stick_invert_y));
        add_recomp_binding(
            recomp::InputDevice::Controller,
            recomp::GameInput::DPAD_DOWN,
            zelda_controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTY, !config.right_stick_invert_y));
    }
}

static nlohmann::json input_field_to_json(const recomp::InputField& field) {
    const auto type = static_cast<recomp::InputType>(field.input_type);
    const char* type_name = "none";
    switch (type) {
        case recomp::InputType::None: type_name = "none"; break;
        case recomp::InputType::Keyboard: type_name = "keyboard"; break;
        case recomp::InputType::Mouse: type_name = "mouse"; break;
        case recomp::InputType::ControllerDigital: type_name = "controller_button"; break;
        case recomp::InputType::ControllerAnalog: type_name = "controller_axis"; break;
    }
    return nlohmann::json{
        {"type", type_name},
        {"id", field.input_id},
    };
}

static bool input_field_from_json(const nlohmann::json& json, recomp::InputField& out) {
    if (!json.is_object()) {
        return false;
    }

    auto type_it = json.find("type");
    if (type_it == json.end() || !type_it->is_string()) {
        return false;
    }

    std::string type = normalized_config_key(type_it->get<std::string>());
    int id = 0;
    if (auto id_it = json.find("id"); id_it != json.end() && id_it->is_number_integer()) {
        id = id_it->get<int>();
    }

    if (type == "none" || type == "empty" || type == "off") {
        out = {};
        return true;
    }
    if (type == "keyboard" || type == "key") {
        out = {static_cast<uint32_t>(recomp::InputType::Keyboard), id};
        return true;
    }
    if (type == "mouse") {
        out = {static_cast<uint32_t>(recomp::InputType::Mouse), id};
        return true;
    }
    if (type == "controller_button" || type == "controller_digital" || type == "button") {
        out = {static_cast<uint32_t>(recomp::InputType::ControllerDigital), id};
        return true;
    }
    if (type == "controller_axis" || type == "controller_analog" || type == "axis") {
        out = {static_cast<uint32_t>(recomp::InputType::ControllerAnalog), id};
        return true;
    }

    return false;
}

static nlohmann::json controls_device_bindings_to_json(recomp::InputDevice device) {
    nlohmann::json bindings = nlohmann::json::object();
    for (size_t input_index = 0; input_index < recomp::get_num_inputs(); input_index++) {
        const auto input = static_cast<recomp::GameInput>(input_index);
        nlohmann::json input_bindings = nlohmann::json::array();
        for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
            input_bindings.push_back(input_field_to_json(recomp::get_input_binding(input, binding_index, device)));
        }
        bindings[recomp::get_input_enum_name(input)] = input_bindings;
    }
    return bindings;
}

static nlohmann::json controls_bindings_to_json() {
    return nlohmann::json{
        {"version", 1},
        {"controller", controls_device_bindings_to_json(recomp::InputDevice::Controller)},
        {"keyboard", controls_device_bindings_to_json(recomp::InputDevice::Keyboard)},
    };
}

static void apply_controls_device_bindings_json(const nlohmann::json& json, recomp::InputDevice device) {
    if (!json.is_object()) {
        return;
    }

    for (auto it = json.begin(); it != json.end(); ++it) {
        const recomp::GameInput input = recomp::get_input_from_enum_name(it.key());
        if (input == recomp::GameInput::COUNT) {
            fprintf(stderr, "[CONFIG] Ignoring unknown input binding key '%s'\n", it.key().c_str());
            continue;
        }

        for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
            recomp::set_input_binding(input, binding_index, device, {});
        }

        if (!it->is_array()) {
            recomp::InputField field{};
            if (input_field_from_json(*it, field)) {
                recomp::set_input_binding(input, 0, device, field);
            }
            continue;
        }

        for (size_t binding_index = 0;
             binding_index < recomp::bindings_per_input && binding_index < it->size();
             binding_index++) {
            recomp::InputField field{};
            if (input_field_from_json(it->at(binding_index), field)) {
                recomp::set_input_binding(input, binding_index, device, field);
            }
        }
    }
}

static void apply_controls_bindings_json_to_recomp(const nlohmann::json& json) {
    auto bindings_it = json.find("bindings");
    if (bindings_it == json.end() || !bindings_it->is_object()) {
        return;
    }

    if (auto controller_it = bindings_it->find("controller");
        controller_it != bindings_it->end()) {
        apply_controls_device_bindings_json(*controller_it, recomp::InputDevice::Controller);
    }
    if (auto keyboard_it = bindings_it->find("keyboard");
        keyboard_it != bindings_it->end()) {
        apply_controls_device_bindings_json(*keyboard_it, recomp::InputDevice::Keyboard);
    }
}
#endif

static nlohmann::json controls_config_to_json(const ControlsConfig& config) {
    nlohmann::json buttons = nlohmann::json::object();
    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
        const char* name = gamepad_button_to_string(static_cast<SDL_GameControllerButton>(i));
        if (name != nullptr) {
            buttons[name] = n64_button_to_string(config.buttons[i]);
        }
    }

    nlohmann::json root = nlohmann::json{
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
#ifdef LOD_USE_ZELDA_MENU
    root["bindings"] = controls_bindings_to_json();
#endif
    return root;
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

static std::string control_pair_summary(const ControlsConfig& config,
                                        std::initializer_list<std::pair<const char*, SDL_GameControllerButton>> entries) {
    std::ostringstream out;
    bool first = true;
    for (const auto& [label, button] : entries) {
        if (!first) {
            out << "; ";
        }
        first = false;
        out << label << " → " << n64_button_display_name(config.buttons[button]);
    }
    return out.str();
}

static void update_ui_controls_config_summary(const ControlsConfig& config) {
    const std::string buttons = control_pair_summary(config, {
        {"A/Cross", SDL_CONTROLLER_BUTTON_A},
        {"B/Circle", SDL_CONTROLLER_BUTTON_B},
        {"X/Square", SDL_CONTROLLER_BUTTON_X},
        {"Y/Triangle", SDL_CONTROLLER_BUTTON_Y},
        {"Start", SDL_CONTROLLER_BUTTON_START},
        {"L3", SDL_CONTROLLER_BUTTON_LEFTSTICK},
        {"R1/RB", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
    });

    std::ostringstream triggers;
    triggers << "L2/LT → " << n64_button_display_name(config.left_trigger)
             << "; R2/RT → " << n64_button_display_name(config.right_trigger)
             << "; threshold " << config.trigger_threshold;

    std::ostringstream right_stick;
    right_stick << (config.right_stick_to_dpad ? "N64 D-pad" : "Off")
                << "; invert X " << (config.right_stick_invert_x ? "On" : "Off")
                << "; invert Y " << (config.right_stick_invert_y ? "On" : "Off")
                << "; deadzone " << config.right_stick_deadzone;

    lod::ui::set_controls_config_summary(buttons, triggers.str(), right_stick.str());
}

#ifdef LOD_USE_ZELDA_MENU
void lod_save_controls_bindings_from_ui() {
    save_controls_config(g_controls_config);
    update_ui_controls_config_summary(g_controls_config);
}
#endif

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
#ifdef LOD_USE_ZELDA_MENU
        apply_controls_config_to_recomp_bindings(config);
        apply_controls_bindings_json_to_recomp(json);
#endif
    } catch (const std::exception& e) {
        fprintf(stderr, "[CONFIG] Failed to parse %s: %s; using control defaults\n",
                controls_config_path().string().c_str(), e.what());
        config = default_controls_config();
#ifdef LOD_USE_ZELDA_MENU
        apply_controls_config_to_recomp_bindings(config);
#endif
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

std::filesystem::path lod::settings::config_path() {
    return g_config_path;
}

std::filesystem::path lod::settings::graphics_config_path() {
    return ::graphics_config_path();
}

void lod::settings::apply_and_save_graphics_config(const ultramodern::renderer::GraphicsConfig& config,
                                                   const char* reason) {
    ::apply_and_save_graphics_config(config, reason);
}

bool lod::settings::portable_mode_enabled() {
    return g_portable_mode;
}

ultramodern::renderer::GraphicsConfig lod::settings::default_graphics_config() {
    return ::default_graphics_config();
}

void lod::settings::persist_rom_path(const std::filesystem::path& rom_path) {
    if (g_config_path.empty() || rom_path.empty()) {
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(g_config_path, ec);

    std::ofstream f(g_config_path / "rom_path.txt");
    if (!f) {
        fprintf(stderr, "[LodRecomp] Failed to save ROM path in %s\n",
                g_config_path.string().c_str());
        return;
    }
    f << rom_path.string() << "\n";
}

static lod::settings::AudioConfig default_audio_config() {
    return {};
}

static nlohmann::json audio_config_to_json(const lod::settings::AudioConfig& config) {
    return nlohmann::json{
        {"version", 1},
        {"master_volume", std::clamp(config.master_volume, 0, 100)},
        {"mute", config.mute},
    };
}

static lod::settings::AudioConfig audio_config_from_json(const nlohmann::json& json) {
    auto config = default_audio_config();

    if (auto it = json.find("master_volume"); it != json.end() && it->is_number_integer()) {
        config.master_volume = std::clamp(it->get<int>(), 0, 100);
    }
    if (auto it = json.find("mute"); it != json.end() && it->is_boolean()) {
        config.mute = it->get<bool>();
    }

    return config;
}

static void set_audio_runtime_config(const lod::settings::AudioConfig& raw_config) {
    lod::settings::AudioConfig config = raw_config;
    config.master_volume = std::clamp(config.master_volume, 0, 100);

    {
        std::lock_guard<std::mutex> lock(g_audio_config_mutex);
        g_audio_config = config;
    }

    g_audio_master_volume_percent.store(config.master_volume, std::memory_order_relaxed);
    g_audio_muted.store(config.mute, std::memory_order_relaxed);
}

static void save_audio_config(const lod::settings::AudioConfig& config) {
    if (g_config_path.empty()) {
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(g_config_path, ec);

    std::ofstream f(audio_config_path());
    if (!f) {
        fprintf(stderr, "[CONFIG] Failed to write %s\n", audio_config_path().string().c_str());
        return;
    }
    f << audio_config_to_json(config).dump(2) << "\n";
}

static lod::settings::AudioConfig load_audio_config() {
    auto config = default_audio_config();

    std::ifstream f(audio_config_path());
    if (!f) {
        set_audio_runtime_config(config);
        save_audio_config(config);
        return config;
    }

    try {
        nlohmann::json json;
        f >> json;
        config = audio_config_from_json(json);
    } catch (const std::exception& e) {
        fprintf(stderr, "[CONFIG] Failed to parse %s: %s; using audio defaults\n",
                audio_config_path().string().c_str(), e.what());
        config = default_audio_config();
    }

    set_audio_runtime_config(config);
    save_audio_config(config);
    return config;
}

static void apply_and_save_audio_config(const lod::settings::AudioConfig& config,
                                        const char* reason) {
    set_audio_runtime_config(config);
    lod::settings::AudioConfig saved = lod::settings::get_audio_config();
    save_audio_config(saved);
    fprintf(stderr, "[CONFIG] %s: audio master=%d mute=%s\n",
            reason,
            saved.master_volume,
            saved.mute ? "on" : "off");
}

std::filesystem::path lod::settings::audio_config_path() {
    return ::audio_config_path();
}

lod::settings::AudioConfig lod::settings::default_audio_config() {
    return ::default_audio_config();
}

lod::settings::AudioConfig lod::settings::get_audio_config() {
    std::lock_guard<std::mutex> lock(g_audio_config_mutex);
    return g_audio_config;
}

void lod::settings::apply_and_save_audio_config(const lod::settings::AudioConfig& config, const char* reason) {
    ::apply_and_save_audio_config(config, reason);
}

static bool handle_graphics_hotkey(SDL_Keycode key) {
    switch (key) {
        case SDLK_F1:
        {
#ifdef LOD_USE_ZELDA_MENU
            recompui::ContextId config_context = recompui::get_config_context_id();
            if (config_context != recompui::ContextId::null() && recompui::is_context_shown(config_context)) {
                recompui::hide_context(config_context);
                fprintf(stderr, "[UI] F1 Zelda settings hidden\n");
            } else if (config_context != recompui::ContextId::null()) {
                recompui::hide_all_contexts();
                recompui::set_config_tab(recompui::ConfigTab::Graphics);
                recompui::show_context(config_context, "");
                fprintf(stderr, "[UI] F1 Zelda settings shown\n");
            }
#else
            const bool was_visible = lod::ui::overlay_visible();
            lod::ui::toggle_overlay();
            const bool is_visible = lod::ui::overlay_visible();
            fprintf(stderr, "[UI] F1 overlay %s\n",
                    was_visible
                        ? (lod::ui::rom_setup_visible() ? "ROM setup active" : (is_visible ? "close requested" : "hidden"))
                        : (is_visible ? "shown" : "hidden"));
#endif
            return true;
        }
        default:
            break;
    }

#ifndef LOD_USE_ZELDA_MENU
    if (lod::ui::handle_graphics_hotkey(key)) {
        fprintf(stderr, "[UI] handled graphics hotkey in overlay\n");
        return true;
    }
#else
    if (recompui::is_context_capturing_input()) {
        return false;
    }
#endif

    auto config = ultramodern::renderer::get_graphics_config();

    switch (key) {
        case SDLK_F11:
            config.wm_option = config.wm_option == ultramodern::renderer::WindowMode::Fullscreen
                ? ultramodern::renderer::WindowMode::Windowed
                : ultramodern::renderer::WindowMode::Fullscreen;
            apply_and_save_graphics_config(config, "F11");
#ifndef LOD_USE_ZELDA_MENU
            lod::ui::notify_graphics_config_changed();
#endif
            return true;
        case SDLK_F5:
            config.res_option = next_graphics_option(config.res_option);
            apply_and_save_graphics_config(config, "F5");
#ifndef LOD_USE_ZELDA_MENU
            lod::ui::notify_graphics_config_changed();
#endif
            return true;
        case SDLK_F6:
            config.ar_option = next_graphics_option(config.ar_option);
            apply_and_save_graphics_config(config, "F6");
#ifndef LOD_USE_ZELDA_MENU
            lod::ui::notify_graphics_config_changed();
#endif
            return true;
        case SDLK_F7:
            config.msaa_option = next_graphics_option(config.msaa_option);
            apply_and_save_graphics_config(config, "F7");
#ifndef LOD_USE_ZELDA_MENU
            lod::ui::notify_graphics_config_changed();
#endif
            return true;
        case SDLK_F8:
            config.rr_option = next_graphics_option(config.rr_option);
            apply_and_save_graphics_config(config, "F8");
#ifndef LOD_USE_ZELDA_MENU
            lod::ui::notify_graphics_config_changed();
#endif
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
    lod::ui::set_sdl_window(window);

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

static void handle_rom_setup_requests(int argc, char** argv, const std::filesystem::path& config_path);

#ifdef LOD_USE_ZELDA_MENU
static bool zelda_ui_trace_enabled() {
    const char* value = std::getenv("LOD_ZELDA_UI_TRACE");
    return value != nullptr && value[0] != '\0' && value[0] != '0';
}

static const char* zelda_sdl_event_name(uint32_t type) {
    switch (type) {
        case SDL_KEYDOWN: return "KEYDOWN";
        case SDL_KEYUP: return "KEYUP";
        case SDL_TEXTINPUT: return "TEXTINPUT";
        case SDL_MOUSEMOTION: return "MOUSEMOTION";
        case SDL_MOUSEBUTTONDOWN: return "MOUSEBUTTONDOWN";
        case SDL_MOUSEBUTTONUP: return "MOUSEBUTTONUP";
        case SDL_MOUSEWHEEL: return "MOUSEWHEEL";
        case SDL_CONTROLLERBUTTONDOWN: return "CONTROLLERBUTTONDOWN";
        case SDL_CONTROLLERBUTTONUP: return "CONTROLLERBUTTONUP";
        case SDL_CONTROLLERAXISMOTION: return "CONTROLLERAXISMOTION";
        case SDL_WINDOWEVENT: return "WINDOWEVENT";
        default: return "OTHER";
    }
}

static void trace_zelda_queued_event(const SDL_Event& event) {
    if (!zelda_ui_trace_enabled()) {
        return;
    }

    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            fprintf(stderr, "[UI_TRACE] queued SDL %s scancode=%d sym=%d repeat=%d\n",
                zelda_sdl_event_name(event.type),
                static_cast<int>(event.key.keysym.scancode),
                static_cast<int>(event.key.keysym.sym),
                static_cast<int>(event.key.repeat));
            break;
        case SDL_MOUSEMOTION:
            fprintf(stderr, "[UI_TRACE] queued SDL MOUSEMOTION x=%d y=%d rel=%d,%d\n",
                event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            fprintf(stderr, "[UI_TRACE] queued SDL %s button=%d x=%d y=%d\n",
                zelda_sdl_event_name(event.type),
                static_cast<int>(event.button.button),
                event.button.x,
                event.button.y);
            break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            fprintf(stderr, "[UI_TRACE] queued SDL %s button=%d\n",
                zelda_sdl_event_name(event.type),
                static_cast<int>(event.cbutton.button));
            break;
        case SDL_CONTROLLERAXISMOTION:
            fprintf(stderr, "[UI_TRACE] queued SDL CONTROLLERAXISMOTION axis=%d value=%d\n",
                static_cast<int>(event.caxis.axis),
                static_cast<int>(event.caxis.value));
            break;
        case SDL_WINDOWEVENT:
            fprintf(stderr, "[UI_TRACE] queued SDL WINDOWEVENT event=%d\n",
                static_cast<int>(event.window.event));
            break;
        default:
            fprintf(stderr, "[UI_TRACE] queued SDL %s type=%u\n",
                zelda_sdl_event_name(event.type),
                event.type);
            break;
    }
}

static void queue_zelda_ui_event(const SDL_Event& event) {
    recompui::queue_event(event);
    trace_zelda_queued_event(event);
}
#endif

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
#ifdef LOD_USE_ZELDA_MENU
                queue_zelda_ui_event(event);
                break;
#else
                if (lod::ui::queue_platform_event(event)) {
                    break;
                }
#endif
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
#ifdef LOD_USE_ZELDA_MENU
                queue_zelda_ui_event(event);
                break;
#else
                if (lod::ui::queue_platform_event(event)) {
                    break;
                }
#endif
                break;
        }
    }

#ifdef LOD_USE_ZELDA_MENU
    zelda64::process_pending_file_dialogs();
#endif

    handle_rom_setup_requests(g_argc_for_rom_discovery, g_argv_for_rom_discovery, g_config_path);
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

const char* audio_format_name(SDL_AudioFormat format) {
    switch (format) {
        case AUDIO_U8: return "U8";
        case AUDIO_S8: return "S8";
        case AUDIO_U16LSB: return "U16LSB";
        case AUDIO_S16LSB: return "S16LSB";
        case AUDIO_U16MSB: return "U16MSB";
        case AUDIO_S16MSB: return "S16MSB";
        case AUDIO_S32LSB: return "S32LSB";
        case AUDIO_S32MSB: return "S32MSB";
        case AUDIO_F32LSB: return "F32LSB";
        case AUDIO_F32MSB: return "F32MSB";
        default: return "unknown";
    }
}

void log_audio_spec(const char* label, const SDL_AudioSpec& spec) {
    fprintf(stderr,
            "[AUDIO] %s freq=%d format=%s(0x%04X) channels=%u samples=%u size=%u\n",
            label, spec.freq, audio_format_name(spec.format), spec.format,
            (unsigned)spec.channels, (unsigned)spec.samples, spec.size);
}

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

    const float master_gain = g_audio_muted.load(std::memory_order_relaxed)
        ? 0.0f
        : std::clamp(g_audio_master_volume_percent.load(std::memory_order_relaxed), 0, 100) / 100.0f;
    const float sample_scale = master_gain * (1.0f / 32768.0f);

    for (size_t i = 0; i < sample_count; i += input_channels) {
        swap_buffer[i + 0 + duplicated_input_frames * input_channels] = audio_data[i + 1] * sample_scale;
        swap_buffer[i + 1 + duplicated_input_frames * input_channels] = audio_data[i + 0] * sample_scale;
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

#if LOD_ENABLE_AUDIO_LATENCY_GUARD
    const uint64_t queued_microseconds =
        uint64_t(SDL_GetQueuedAudioSize(audio_device)) / bytes_per_frame * 1000000 / sample_rate;
    uint32_t skip_factor = static_cast<uint32_t>(queued_microseconds / 100000);
    if (skip_factor > 8) {
        skip_factor = 8;
    }
    if (skip_factor != 0) {
        const uint32_t skip_ratio = 1u << skip_factor;
        const uint32_t frame_size = output_channels * sizeof(swap_buffer[0]);
        const uint32_t original_frame_count = num_bytes_to_queue / frame_size;
        const uint32_t decimated_frame_count = original_frame_count / skip_ratio;

        for (uint32_t frame = 0; frame < decimated_frame_count; frame++) {
            for (uint32_t channel = 0; channel < output_channels; channel++) {
                samples_to_queue[frame * output_channels + channel] =
                    samples_to_queue[frame * skip_ratio * output_channels + channel];
            }
        }
        num_bytes_to_queue = decimated_frame_count * frame_size;

#if LOD_ENABLE_AUDIO_TRACE
        fprintf(stderr,
                "[AUDIO] latency_guard queued_us=%" PRIu64 " skip_factor=%u"
                " skip_ratio=%u frames=%u->%u bytes=%u\n",
                queued_microseconds, skip_factor, skip_ratio,
                original_frame_count, decimated_frame_count, num_bytes_to_queue);
#endif
    }
#endif

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
    const char* audio_driver = SDL_GetCurrentAudioDriver();
    fprintf(stderr, "[AUDIO] SDL audio driver: %s\n", audio_driver != nullptr ? audio_driver : "(none)");

    SDL_AudioSpec spec_preferred{};
    if (SDL_GetDefaultAudioInfo(nullptr, &spec_preferred, 0) == 0) {
        log_audio_spec("default-output", spec_preferred);
    } else {
        fprintf(stderr, "[AUDIO] SDL_GetDefaultAudioInfo failed: %s\n", SDL_GetError());
    }

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
    log_audio_spec("desired", spec_desired);

    SDL_AudioSpec spec_obtained{};
    audio_device = SDL_OpenAudioDevice(nullptr, false, &spec_desired, &spec_obtained, 0);
    if (audio_device == 0) {
        fprintf(stderr, "[WARN] SDL error opening audio device: %s (continuing without audio)\n", SDL_GetError());
    } else {
        log_audio_spec("obtained", spec_obtained);
        SDL_PauseAudioDevice(audio_device, 0);
#if LOD_ENABLE_AUDIO_TRACE
        fprintf(stderr,
                "[AUDIO] opened SDL audio device id=%u output=%u channels=%u\n",
                audio_device, output_sample_rate, output_channels);
#endif
    }

    output_sample_rate = output_freq;
    update_audio_converter();
    fprintf(stderr,
            "[AUDIO] reset_audio output=%u len_mult=%d discarded=%u device=%u\n",
            output_sample_rate, audio_convert.len_mult, discarded_output_frames, audio_device);
}

lod::settings::AudioStatus lod::settings::audio_status() {
    lod::settings::AudioStatus status{};
    const char* driver = SDL_GetCurrentAudioDriver();
    status.sdl_driver = driver != nullptr ? driver : "(none)";
    status.input_sample_rate = sample_rate;
    status.output_sample_rate = output_sample_rate;
    status.output_channels = output_channels;
    status.device_open = audio_device != 0;
    status.queued_bytes = status.device_open ? SDL_GetQueuedAudioSize(audio_device) : 0;
    return status;
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

#ifdef LOD_USE_ZELDA_MENU
static float zelda_input_field_value(const recomp::InputField& field) {
    switch (static_cast<recomp::InputType>(field.input_type)) {
        case recomp::InputType::None:
            return 0.0f;

        case recomp::InputType::Keyboard: {
            int key_count = 0;
            const uint8_t* keys = SDL_GetKeyboardState(&key_count);
            if (field.input_id < 0 || field.input_id >= key_count) {
                return 0.0f;
            }
            return keys[field.input_id] ? 1.0f : 0.0f;
        }

        case recomp::InputType::ControllerDigital:
            if (game_controller == nullptr) {
                return 0.0f;
            }
            return SDL_GameControllerGetButton(
                game_controller,
                static_cast<SDL_GameControllerButton>(field.input_id)) ? 1.0f : 0.0f;

        case recomp::InputType::ControllerAnalog: {
            if (game_controller == nullptr || field.input_id == 0) {
                return 0.0f;
            }
            const int encoded_axis = field.input_id;
            const auto axis = static_cast<SDL_GameControllerAxis>(std::abs(encoded_axis) - 1);
            float value = normalize_axis(SDL_GameControllerGetAxis(game_controller, axis));
            if (encoded_axis < 0) {
                value = -value;
            }
            return std::max(value, 0.0f);
        }

        case recomp::InputType::Mouse:
            return 0.0f;
    }

    return 0.0f;
}

static float zelda_input_value(recomp::GameInput input, recomp::InputDevice device) {
    float value = 0.0f;
    for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
        value = std::max(
            value,
            zelda_input_field_value(recomp::get_input_binding(input, binding_index, device)));
    }
    return value;
}

static float zelda_input_value(recomp::GameInput input) {
    return std::max(
        zelda_input_value(input, recomp::InputDevice::Keyboard),
        zelda_input_value(input, recomp::InputDevice::Controller));
}

static bool zelda_input_pressed(recomp::GameInput input) {
    return zelda_input_value(input) > 0.5f;
}

static uint16_t n64_button_for_game_input(recomp::GameInput input) {
    switch (input) {
        case recomp::GameInput::A: return N64_BUTTON_A;
        case recomp::GameInput::B: return N64_BUTTON_B;
        case recomp::GameInput::Z: return N64_BUTTON_Z;
        case recomp::GameInput::START: return N64_BUTTON_START;
        case recomp::GameInput::L: return N64_BUTTON_L;
        case recomp::GameInput::R: return N64_BUTTON_R;
        case recomp::GameInput::C_UP: return N64_CBUTTON_UP;
        case recomp::GameInput::C_DOWN: return N64_CBUTTON_DOWN;
        case recomp::GameInput::C_LEFT: return N64_CBUTTON_LEFT;
        case recomp::GameInput::C_RIGHT: return N64_CBUTTON_RIGHT;
        case recomp::GameInput::DPAD_UP: return N64_DPAD_UP;
        case recomp::GameInput::DPAD_DOWN: return N64_DPAD_DOWN;
        case recomp::GameInput::DPAD_LEFT: return N64_DPAD_LEFT;
        case recomp::GameInput::DPAD_RIGHT: return N64_DPAD_RIGHT;
        case recomp::GameInput::X_AXIS_NEG:
        case recomp::GameInput::X_AXIS_POS:
        case recomp::GameInput::Y_AXIS_POS:
        case recomp::GameInput::Y_AXIS_NEG:
        case recomp::GameInput::TOGGLE_MENU:
        case recomp::GameInput::ACCEPT_MENU:
        case recomp::GameInput::APPLY_MENU:
        case recomp::GameInput::COUNT:
            return 0;
    }
    return 0;
}
#endif

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

#ifdef LOD_USE_ZELDA_MENU
    if (recompui::is_context_capturing_input()) {
        return true;
    }
#else
    if (lod::ui::captures_input()) {
        return true;
    }
#endif

#ifdef LOD_USE_ZELDA_MENU
    for (size_t input_index = 0; input_index < recomp::get_num_inputs(); input_index++) {
        const auto input = static_cast<recomp::GameInput>(input_index);
        const uint16_t n64_button = n64_button_for_game_input(input);
        if (n64_button != 0 && zelda_input_pressed(input)) {
            *buttons |= n64_button;
        }
    }

    *x = std::clamp(
        zelda_input_value(recomp::GameInput::X_AXIS_POS) -
            zelda_input_value(recomp::GameInput::X_AXIS_NEG),
        -1.0f,
        1.0f);
    *y = std::clamp(
        zelda_input_value(recomp::GameInput::Y_AXIS_POS) -
            zelda_input_value(recomp::GameInput::Y_AXIS_NEG),
        -1.0f,
        1.0f);

    return true;
#else
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
#endif
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

static std::string rom_basename(const std::filesystem::path& rom_path) {
    return rom_path.empty() ? "None selected" : rom_path.filename().string();
}

static std::string ascii_lower(std::string value) {
    for (char& c : value) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return value;
}

static bool is_n64_rom_path(const std::filesystem::path& path) {
    std::string ext = ascii_lower(path.extension().string());
    return ext == ".z64" || ext == ".n64" || ext == ".v64";
}

static bool is_lod_named_rom_path(const std::filesystem::path& path) {
    std::string name = ascii_lower(path.stem().string());
    if (name.find("decompressed") != std::string::npos ||
        name.find("extended") != std::string::npos ||
        name.find("ni_") != std::string::npos ||
        name.find("ni-") != std::string::npos) {
        return false;
    }
    return name.find("legacy") != std::string::npos ||
           name.find("lod") != std::string::npos ||
           name.find("castlevania2") != std::string::npos ||
           name.find("nd4e") != std::string::npos;
}

static std::filesystem::path find_rom_in_directory(const std::filesystem::path& dir) {
    std::error_code ec;
    if (dir.empty() || !std::filesystem::exists(dir, ec) || !std::filesystem::is_directory(dir, ec)) {
        return {};
    }

    // Prefer LoD-named ROMs. This repository may also contain the original
    // CV64 reference ROM, so avoid matching plain "castlevania" here.
    for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
        if (ec) {
            return {};
        }
        if (is_n64_rom_path(entry.path()) && is_lod_named_rom_path(entry.path())) {
            return entry.path();
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
        if (ec) {
            return {};
        }
        if (is_n64_rom_path(entry.path())) {
            return entry.path();
        }
    }

    return {};
}

static const char* rom_validation_error_name(recomp::RomValidationError error) {
    switch (error) {
        case recomp::RomValidationError::Good: return "Ready";
        case recomp::RomValidationError::FailedToOpen: return "Failed to open the selected file.";
        case recomp::RomValidationError::NotARom: return "The selected file is not a recognized Nintendo 64 ROM.";
        case recomp::RomValidationError::IncorrectRom: return "The selected ROM is not the supported Legacy of Darkness ROM.";
        case recomp::RomValidationError::NotYet: return "This ROM variant is recognized but not supported by this build yet.";
        case recomp::RomValidationError::IncorrectVersion: return "This appears to be the wrong version or region of the game.";
        case recomp::RomValidationError::OtherError: return "ROM validation failed due to an internal error.";
    }
    return "ROM validation failed.";
}

static std::filesystem::path prompt_for_rom_path() {
    fprintf(stderr,
            "[LodRecomp] Asking user to select a stock LoD ROM.\n");

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

static std::filesystem::path discover_rom_path(int argc, char** argv,
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

    // Portable and selected-ROM setups often keep the ROM in the active config
    // folder. This is especially important for macOS .app launches, where
    // Launch Services does not preserve the shell working directory.
    std::filesystem::path config_rom = find_rom_in_directory(config_path);
    if (!config_rom.empty()) {
        return config_rom;
    }

#ifdef __APPLE__
    std::filesystem::path bundle_parent_rom = find_rom_in_directory(lod::get_bundle_directory().parent_path());
    if (!bundle_parent_rom.empty()) {
        return bundle_parent_rom;
    }
#endif

    // Look for ROM in current directory.
    std::filesystem::path cwd_rom = find_rom_in_directory(".");
    if (!cwd_rom.empty()) {
        return cwd_rom;
    }

    // Also search in resources/ subdirectory.
    if (std::filesystem::exists("resources")) {
        auto p = find_rom_in_directory("resources");
        if (!p.empty()) {
            return p;
        }
    }

    return {};
}

static void validate_and_start_rom(std::filesystem::path rom_path, bool persist_selected_path, const char* source_label) {
    if (rom_path.empty()) {
        lod::ui::set_rom_setup_status("Missing",
            "No ROM file was selected. Select your ROM file to start the game.",
            "None selected", false);
        lod::ui::show_rom_setup();
        g_rom_validation_busy.store(false, std::memory_order_relaxed);
        return;
    }

    bool expected = false;
    if (!g_rom_validation_busy.compare_exchange_strong(expected, true, std::memory_order_relaxed)) {
        return;
    }

    lod::ui::set_rom_setup_status("Validating",
        std::string("Checking ") + rom_basename(rom_path) + "...",
        rom_basename(rom_path), true);
    fprintf(stderr, "[LodRecomp] Validating ROM from %s: %s\n", source_label, rom_path.string().c_str());

    std::u8string game_id = u8"castlevania2.n64.us";
    auto result = recomp::select_rom(rom_path, game_id);
    if (result != recomp::RomValidationError::Good) {
        fprintf(stderr, "[LodRecomp] ROM validation failed (%d) for: %s\n",
                static_cast<int>(result), rom_path.string().c_str());
        lod::ui::set_rom_setup_status("Invalid",
            rom_validation_error_name(result),
            rom_basename(rom_path), false);
        lod::ui::show_rom_setup();
        g_rom_validation_busy.store(false, std::memory_order_relaxed);
        return;
    }

    if (persist_selected_path) {
        write_stored_rom_path(g_config_path, rom_path);
    }

    lod::ui::set_rom_setup_status("Ready",
        "ROM validated. Starting game...",
        rom_basename(rom_path), true);
    fprintf(stderr, "[LodRecomp] ROM validated successfully, starting game...\n");
    g_rom_game_started.store(true, std::memory_order_relaxed);
    recomp::start_game(game_id);
    lod::ui::hide_rom_setup();
    g_rom_validation_busy.store(false, std::memory_order_relaxed);
}

static void start_rom_validation_thread(std::filesystem::path rom_path, bool persist_selected_path, const char* source_label) {
    std::thread([rom_path = std::move(rom_path), persist_selected_path, label = std::string(source_label)]() mutable {
        validate_and_start_rom(std::move(rom_path), persist_selected_path, label.c_str());
    }).detach();
}

static void retry_rom_discovery(int argc, char** argv, const std::filesystem::path& config_path) {
    if (g_rom_validation_busy.load(std::memory_order_relaxed) ||
        g_rom_game_started.load(std::memory_order_relaxed)) {
        return;
    }

    lod::ui::set_rom_setup_status("Searching",
        "Checking command-line, portable, stored, and local ROM paths...",
        "None selected", true);
    std::filesystem::path rom_path = discover_rom_path(argc, argv, config_path);
    if (rom_path.empty()) {
        lod::ui::set_rom_setup_status("Missing",
            "No valid ROM path was found automatically. Select your ROM file to start.",
            "None selected", false);
        return;
    }

    start_rom_validation_thread(std::move(rom_path), false, "retry discovery");
}

static void handle_rom_setup_requests(int argc, char** argv, const std::filesystem::path& config_path) {
    switch (lod::ui::consume_rom_setup_request()) {
        case lod::ui::RomSetupRequest::None:
            return;
        case lod::ui::RomSetupRequest::SelectFile:
        {
            if (g_rom_validation_busy.load(std::memory_order_relaxed) ||
                g_rom_game_started.load(std::memory_order_relaxed)) {
                return;
            }
            lod::ui::set_rom_setup_status("Selecting",
                "Choose a .z64, .n64, or .v64 ROM file.",
                "None selected", true);
            std::filesystem::path selected = prompt_for_rom_path();
            if (selected.empty()) {
                lod::ui::set_rom_setup_status("Missing",
                    "ROM selection cancelled. Select your ROM file to start the game.",
                    "None selected", false);
                return;
            }
            start_rom_validation_thread(std::move(selected), true, "file picker");
            return;
        }
        case lod::ui::RomSetupRequest::Retry:
            retry_rom_discovery(argc, argv, config_path);
            return;
        case lod::ui::RomSetupRequest::Help:
            return;
        case lod::ui::RomSetupRequest::Quit:
            ultramodern::quit();
            return;
    }
}

static void auto_start_game(const std::filesystem::path& rom_path) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    validate_and_start_rom(rom_path, false, "startup discovery");
}

#ifdef LOD_USE_ZELDA_MENU
static bool zelda_wait_for_start_enabled() {
    const char* value = std::getenv("LOD_ZELDA_WAIT_FOR_START");
    if (value == nullptr || value[0] == '\0') {
        return true;
    }

    std::string normalized = ascii_lower(value);
    return normalized != "0" && normalized != "false" && normalized != "off" && normalized != "no";
}

static void validate_rom_for_zelda_launcher(const std::filesystem::path& rom_path) {
    std::u8string game_id = u8"castlevania2.n64.us";
    fprintf(stderr, "[LodRecomp] Validating ROM for Zelda launcher: %s\n", rom_path.string().c_str());
    auto result = recomp::select_rom(rom_path, game_id);
    if (result == recomp::RomValidationError::Good) {
        fprintf(stderr, "[LodRecomp] ROM validated; waiting for launcher Start Game action.\n");
    } else {
        fprintf(stderr, "[LodRecomp] ROM validation failed (%d); Zelda launcher will wait for ROM selection.\n",
            static_cast<int>(result));
    }
}
#endif

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

static bool lod_path_exists(const std::filesystem::path& path) {
    std::error_code ec;
    return std::filesystem::exists(path, ec);
}

static std::optional<std::filesystem::path> find_portable_config_path(int argc, char** argv) {
    std::vector<std::filesystem::path> candidate_dirs;

    std::error_code ec;
    candidate_dirs.emplace_back(std::filesystem::current_path(ec));

    if (argc > 0 && argv != nullptr && argv[0] != nullptr && argv[0][0] != '\0') {
        std::filesystem::path exe_path(argv[0]);
        if (exe_path.is_relative()) {
            exe_path = std::filesystem::current_path(ec) / exe_path;
        }
        candidate_dirs.emplace_back(exe_path.lexically_normal().parent_path());
    }

#ifdef __APPLE__
    candidate_dirs.emplace_back(lod::get_bundle_directory().parent_path());
#endif

    for (auto dir : candidate_dirs) {
        if (dir.empty()) {
            continue;
        }
        dir = dir.lexically_normal();
        if (lod_path_exists(dir / "portable.txt")) {
            return dir;
        }
    }

    return std::nullopt;
}

int main(int argc, char** argv) {
    g_argc_for_rom_discovery = argc;
    g_argv_for_rom_discovery = argv;

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
    recomp::Version::from_string(LOD_RECOMP_VERSION, project_version);

    // Set up config path for ROM storage. If portable.txt exists beside the
    // executable/app (or in the launch directory), keep config and saves there.
    std::filesystem::path config_path;
    if (auto portable_path = find_portable_config_path(argc, argv)) {
        config_path = *portable_path;
        g_portable_mode = true;
        fprintf(stderr, "[CONFIG] Portable mode enabled: %s\n",
                config_path.string().c_str());
    } else {
        g_portable_mode = false;
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
    }
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
    update_ui_controls_config_summary(g_controls_config);
    fprintf(stderr, "[CONFIG] Loaded controls config: %s\n",
            controls_config_path().string().c_str());

    auto audio_config = load_audio_config();
    fprintf(stderr, "[CONFIG] Loaded audio config: master=%d mute=%s path=%s\n",
            audio_config.master_volume,
            audio_config.mute ? "on" : "off",
            audio_config_path().string().c_str());

#ifdef _WIN32
    // Prefer WASAPI on Windows; SDL queued audio can crackle with DirectSound.
    if (SDL_getenv("SDL_AUDIODRIVER") == nullptr) {
        SDL_setenv("SDL_AUDIODRIVER", "wasapi", false);
    }
#endif

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    reset_audio(48000);

    for (const auto& game : supported_games) {
        recomp::register_game(game);
    }

    lod_register_overlays();

#ifdef LOD_USE_ZELDA_MENU
    const bool zelda_wait_for_start = zelda_wait_for_start_enabled();
#endif

    // Find the ROM without blocking on a native picker. If discovery fails, boot
    // the renderer/UI into overlay-only ROM setup so users can choose a ROM.
    std::filesystem::path rom_path = discover_rom_path(argc, argv, config_path);
    if (rom_path.empty()) {
#ifdef LOD_USE_ZELDA_MENU
        if (zelda_wait_for_start) {
            fprintf(stderr, "[LodRecomp] No ROM found automatically; Zelda launcher will wait for ROM selection.\n");
        } else
#endif
        {
        fprintf(stderr, "[LodRecomp] No ROM found automatically; starting ROM setup UI.\n");
        lod::ui::set_rom_setup_status("Missing",
            "No ROM was found automatically. Select your ROM file to start the game.",
            "None selected", false);
        lod::ui::show_rom_setup();
        }
    } else {
        fprintf(stderr, "[LodRecomp] Candidate ROM: %s\n", rom_path.string().c_str());
#ifdef LOD_USE_ZELDA_MENU
        if (zelda_wait_for_start) {
            validate_rom_for_zelda_launcher(rom_path);
        }
#endif
    }

    if (std::getenv("RECOMP_UI_OPEN_ON_START") != nullptr) {
        lod::ui::show_overlay();
        fprintf(stderr, "[UI] RECOMP_UI_OPEN_ON_START requested; overlay will open after renderer init\n");
    }

    if (!rom_path.empty()) {
#ifdef LOD_USE_ZELDA_MENU
        if (zelda_wait_for_start) {
            fprintf(stderr, "[LodRecomp] LOD_ZELDA_WAIT_FOR_START enabled; waiting for launcher Start Game action.\n");
        } else
#endif
        {
        // Launch auto-start thread (will validate ROM and start game after runtime initializes).
        std::thread auto_start_thread(auto_start_game, rom_path);
        auto_start_thread.detach();
        }
    }

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
