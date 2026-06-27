#include "recomp_ui.h"
#include "recomp_input.h"
#include "zelda_config.h"
#include "zelda_support.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <list>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#if defined(_WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "RmlUi/Core.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Elements/ElementTabSet.h"

#include "elements/ui_element.h"

#include "librecomp/game.hpp"
#include "lod/lod_support.h"
#include "lod/lod_settings.hpp"
#include "promptfont.h"
#include "ultramodern/config.hpp"
#include "ultramodern/ultramodern.hpp"
#include "nfd.h"

extern uint8_t* rdram_ptr_for_debug;
extern "C" uint32_t lod_current_map_overlay_rom();
extern "C" uint32_t lod_current_map_overlay_size();
extern "C" int lod_current_map_overlay_load_count();
extern "C" int lod_ni_overlay_loaded_0f_pair();
extern "C" int lod_ni_overlay_loaded_0e_pair();
void lod_save_controls_bindings_from_ui();

namespace {
constexpr uint32_t kInputNone = static_cast<uint32_t>(recomp::InputType::None);
constexpr uint32_t kInputKeyboard = static_cast<uint32_t>(recomp::InputType::Keyboard);
constexpr uint32_t kInputControllerDigital = static_cast<uint32_t>(recomp::InputType::ControllerDigital);
constexpr uint32_t kInputControllerAnalog = static_cast<uint32_t>(recomp::InputType::ControllerAnalog);

using input_mapping = std::array<recomp::InputField, recomp::bindings_per_input>;
std::array<input_mapping, static_cast<size_t>(recomp::GameInput::COUNT)> g_controller_bindings{};
std::array<input_mapping, static_cast<size_t>(recomp::GameInput::COUNT)> g_keyboard_bindings{};
recomp::BackgroundInputMode g_background_input_mode = recomp::BackgroundInputMode::On;
bool g_debug_enabled = false;
int g_cur_config_index = -1;
int g_scanned_binding_index = -1;
int g_scanned_input_index = -1;
int g_focused_input_index = -1;
recomp::InputDevice g_scanning_device = recomp::InputDevice::COUNT;
recomp::InputDevice g_current_input_device = recomp::InputDevice::Controller;

recompui::ContextId g_launcher_context = recompui::ContextId::null();
recompui::ContextId g_config_context = recompui::ContextId::null();
recompui::ConfigTab g_active_tab = recompui::ConfigTab::General;
Rml::DataModelHandle g_launcher_model;
Rml::DataModelHandle g_config_model;
Rml::DataModelHandle g_controls_model;
Rml::DataModelHandle g_nav_help_model;
bool g_rom_valid = false;
std::string g_rom_status = "Select your Legacy of Darkness (USA) ROM.";
std::string g_rom_file_name = "None selected";
std::string g_version_string;
ultramodern::renderer::GraphicsConfig g_pending_graphics{};
lod::settings::AudioConfig g_audio_config{};
std::string g_config_status = "Graphics changes are staged until Apply.";
std::string g_audio_status = "Volume and mute apply immediately. Audio driver changes require restarting LodRecomp.";
std::string g_config_path_display;

std::u8string lod_game_id() {
    return u8"castlevania2.n64.us";
}

enum class FileDialogMode {
    SingleRom,
    MultipleFiles,
};

struct FileDialogRequest {
    FileDialogMode mode = FileDialogMode::SingleRom;
    std::function<void(bool success, const std::filesystem::path& path)> single_callback;
    std::function<void(bool success, const std::list<std::filesystem::path>& paths)> multiple_callback;
};

struct FileDialogResult {
    bool success = false;
    std::filesystem::path path;
    std::list<std::filesystem::path> paths;
};

struct InputInfo {
    recomp::GameInput input;
    std::string_view enum_name;
    std::string_view display_name;
};

const std::array<InputInfo, static_cast<size_t>(recomp::GameInput::COUNT)> k_input_info{{
    {recomp::GameInput::A, "A", "A Button"},
    {recomp::GameInput::B, "B", "B Button"},
    {recomp::GameInput::L, "L", "L Button"},
    {recomp::GameInput::R, "R", "R Button"},
    {recomp::GameInput::Z, "Z", "Z Button"},
    {recomp::GameInput::START, "START", "Start"},
    {recomp::GameInput::C_LEFT, "C_LEFT", "C Left"},
    {recomp::GameInput::C_RIGHT, "C_RIGHT", "C Right"},
    {recomp::GameInput::C_UP, "C_UP", "C Up"},
    {recomp::GameInput::C_DOWN, "C_DOWN", "C Down"},
    {recomp::GameInput::DPAD_LEFT, "DPAD_LEFT", "D-Pad Left"},
    {recomp::GameInput::DPAD_RIGHT, "DPAD_RIGHT", "D-Pad Right"},
    {recomp::GameInput::DPAD_UP, "DPAD_UP", "D-Pad Up"},
    {recomp::GameInput::DPAD_DOWN, "DPAD_DOWN", "D-Pad Down"},
    {recomp::GameInput::X_AXIS_NEG, "X_AXIS_NEG", "Analog Left"},
    {recomp::GameInput::X_AXIS_POS, "X_AXIS_POS", "Analog Right"},
    {recomp::GameInput::Y_AXIS_POS, "Y_AXIS_POS", "Analog Up"},
    {recomp::GameInput::Y_AXIS_NEG, "Y_AXIS_NEG", "Analog Down"},
    {recomp::GameInput::TOGGLE_MENU, "TOGGLE_MENU", "Toggle Menu"},
    {recomp::GameInput::ACCEPT_MENU, "ACCEPT_MENU", "Accept (Menu)"},
    {recomp::GameInput::APPLY_MENU, "APPLY_MENU", "Apply (Menu)"},
}};

std::mutex g_file_dialog_mutex;
std::deque<FileDialogRequest> g_file_dialog_requests;
bool g_file_dialog_active = false;

std::string default_file_dialog_path() {
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif
    if (home == nullptr) {
        return {};
    }

    std::filesystem::path downloads = std::filesystem::path(home) / "Downloads";
    if (std::filesystem::exists(downloads)) {
        return downloads.string();
    }

    return {};
}

bool enqueue_file_dialog_request(FileDialogRequest&& request) {
    std::lock_guard lock{g_file_dialog_mutex};
    if (g_file_dialog_active || !g_file_dialog_requests.empty()) {
        return false;
    }

    g_file_dialog_requests.emplace_back(std::move(request));
    return true;
}

FileDialogResult run_single_rom_file_dialog() {
    FileDialogResult dialog_result;

    if (NFD_Init() != NFD_OKAY) {
        std::fprintf(stderr, "[UI] NFD_Init failed: %s\n", NFD_GetError());
        return dialog_result;
    }

    nfdchar_t* out_path = nullptr;
    nfdfilteritem_t filters[] = {{"Nintendo 64 ROM", "z64,n64,v64"}};
    std::string default_path = default_file_dialog_path();
    nfdresult_t result = NFD_OpenDialog(&out_path, filters, 1,
        default_path.empty() ? nullptr : default_path.c_str());

    if (result == NFD_OKAY && out_path != nullptr) {
        dialog_result.path = std::filesystem::path(out_path);
        dialog_result.success = true;
        NFD_FreePath(out_path);
    } else if (result == NFD_ERROR) {
        std::fprintf(stderr, "[UI] Native file dialog failed: %s\n", NFD_GetError());
    }

    NFD_Quit();
    return dialog_result;
}

FileDialogResult run_multiple_file_dialog() {
    FileDialogResult dialog_result;

    if (NFD_Init() != NFD_OKAY) {
        std::fprintf(stderr, "[UI] NFD_Init failed: %s\n", NFD_GetError());
        return dialog_result;
    }

    const nfdpathset_t* out_paths = nullptr;
    nfdfilteritem_t filters[] = {{"Mod files", "nrm,zip"}};
    std::string default_path = default_file_dialog_path();
    nfdresult_t result = NFD_OpenDialogMultiple(&out_paths, filters, 1,
        default_path.empty() ? nullptr : default_path.c_str());

    if (result == NFD_OKAY && out_paths != nullptr) {
        nfdpathsetsize_t count = 0;
        if (NFD_PathSet_GetCount(out_paths, &count) == NFD_OKAY) {
            for (nfdpathsetsize_t i = 0; i < count; i++) {
                nfdchar_t* path = nullptr;
                if (NFD_PathSet_GetPath(out_paths, i, &path) == NFD_OKAY && path != nullptr) {
                    dialog_result.paths.emplace_back(path);
                    NFD_PathSet_FreePath(path);
                }
            }
        }
        NFD_PathSet_Free(out_paths);
        dialog_result.success = !dialog_result.paths.empty();
    } else if (result == NFD_ERROR) {
        std::fprintf(stderr, "[UI] Native file dialog failed: %s\n", NFD_GetError());
    }

    NFD_Quit();
    return dialog_result;
}

void queue_file_dialog_completion(FileDialogRequest request, FileDialogResult result) {
    if (request.mode == FileDialogMode::SingleRom && request.single_callback) {
        auto callback = std::move(request.single_callback);
        bool success = result.success;
        std::filesystem::path path = std::move(result.path);
        recompui::queue_ui_thread_callback(
            [callback = std::move(callback), success, path = std::move(path)]() mutable {
                callback(success, path);
            });
        return;
    }

    if (request.mode == FileDialogMode::MultipleFiles && request.multiple_callback) {
        auto callback = std::move(request.multiple_callback);
        bool success = result.success;
        std::list<std::filesystem::path> paths = std::move(result.paths);
        recompui::queue_ui_thread_callback(
            [callback = std::move(callback), success, paths = std::move(paths)]() mutable {
                callback(success, paths);
            });
    }
}

void dirty_launcher() {
    if (!g_launcher_model) {
        return;
    }
    g_launcher_model.DirtyVariable("rom_valid");
    g_launcher_model.DirtyVariable("rom_status");
    g_launcher_model.DirtyVariable("rom_file");
}

void dirty_nav_help() {
    if (!g_nav_help_model) {
        return;
    }
    g_nav_help_model.DirtyVariable("nav_help__navigate");
    g_nav_help_model.DirtyVariable("nav_help__accept");
    g_nav_help_model.DirtyVariable("nav_help__exit");
}

void dirty_controls() {
    if (g_controls_model) {
        g_controls_model.DirtyVariable("inputs");
        g_controls_model.DirtyVariable("input_device_is_keyboard");
        g_controls_model.DirtyVariable("active_binding_input");
        g_controls_model.DirtyVariable("active_binding_slot");
        g_controls_model.DirtyVariable("cur_input_row");
    }
    dirty_nav_help();
    if (g_config_model) {
        g_config_model.DirtyVariable("gfx_help__apply");
    }
}

recomp::InputField keyboard_field(SDL_Scancode scancode) {
    return {kInputKeyboard, static_cast<int32_t>(scancode)};
}

recomp::InputField controller_button_field(SDL_GameControllerButton button) {
    return {kInputControllerDigital, static_cast<int32_t>(button)};
}

recomp::InputField controller_axis_field(SDL_GameControllerAxis axis, bool positive) {
    const int32_t encoded_axis = static_cast<int32_t>(axis) + 1;
    return {kInputControllerAnalog, positive ? encoded_axis : -encoded_axis};
}

input_mapping make_mapping(std::initializer_list<recomp::InputField> fields) {
    input_mapping mapping{};
    size_t index = 0;
    for (const recomp::InputField& field : fields) {
        if (index >= mapping.size()) {
            break;
        }
        mapping[index++] = field;
    }
    return mapping;
}

input_mapping default_binding_for(recomp::InputDevice device, recomp::GameInput input) {
    if (device == recomp::InputDevice::Keyboard) {
        switch (input) {
            case recomp::GameInput::A: return make_mapping({keyboard_field(SDL_SCANCODE_RETURN)});
            case recomp::GameInput::B: return make_mapping({keyboard_field(SDL_SCANCODE_RSHIFT)});
            case recomp::GameInput::L: return make_mapping({keyboard_field(SDL_SCANCODE_Q)});
            case recomp::GameInput::R: return make_mapping({keyboard_field(SDL_SCANCODE_E)});
            case recomp::GameInput::Z: return make_mapping({keyboard_field(SDL_SCANCODE_Z)});
            case recomp::GameInput::START: return make_mapping({keyboard_field(SDL_SCANCODE_SPACE)});
            case recomp::GameInput::C_LEFT: return make_mapping({keyboard_field(SDL_SCANCODE_J)});
            case recomp::GameInput::C_RIGHT: return make_mapping({keyboard_field(SDL_SCANCODE_L)});
            case recomp::GameInput::C_UP: return make_mapping({keyboard_field(SDL_SCANCODE_I)});
            case recomp::GameInput::C_DOWN: return make_mapping({keyboard_field(SDL_SCANCODE_K)});
            case recomp::GameInput::DPAD_LEFT: return make_mapping({keyboard_field(SDL_SCANCODE_LEFT)});
            case recomp::GameInput::DPAD_RIGHT: return make_mapping({keyboard_field(SDL_SCANCODE_RIGHT)});
            case recomp::GameInput::DPAD_UP: return make_mapping({keyboard_field(SDL_SCANCODE_UP)});
            case recomp::GameInput::DPAD_DOWN: return make_mapping({keyboard_field(SDL_SCANCODE_DOWN)});
            case recomp::GameInput::X_AXIS_NEG: return make_mapping({keyboard_field(SDL_SCANCODE_A)});
            case recomp::GameInput::X_AXIS_POS: return make_mapping({keyboard_field(SDL_SCANCODE_D)});
            case recomp::GameInput::Y_AXIS_POS: return make_mapping({keyboard_field(SDL_SCANCODE_W)});
            case recomp::GameInput::Y_AXIS_NEG: return make_mapping({keyboard_field(SDL_SCANCODE_S)});
            case recomp::GameInput::TOGGLE_MENU: return make_mapping({keyboard_field(SDL_SCANCODE_ESCAPE)});
            case recomp::GameInput::ACCEPT_MENU: return make_mapping({keyboard_field(SDL_SCANCODE_RETURN)});
            case recomp::GameInput::APPLY_MENU: return make_mapping({keyboard_field(SDL_SCANCODE_F)});
            case recomp::GameInput::COUNT: break;
        }
        return {};
    }

    switch (input) {
        case recomp::GameInput::A: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_A)});
        case recomp::GameInput::B: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_X)});
        case recomp::GameInput::L: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_LEFTSHOULDER)});
        case recomp::GameInput::R: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)});
        case recomp::GameInput::Z:
            return make_mapping({
                controller_axis_field(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, true),
                controller_axis_field(SDL_CONTROLLER_AXIS_TRIGGERLEFT, true),
            });
        case recomp::GameInput::START: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_START)});
        case recomp::GameInput::C_LEFT: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_Y)});
        case recomp::GameInput::C_RIGHT: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_B)});
        case recomp::GameInput::C_UP: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_Y)});
        case recomp::GameInput::C_DOWN:
            return make_mapping({
                controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTY, true),
                controller_button_field(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
            });
        case recomp::GameInput::DPAD_LEFT:
            return make_mapping({
                controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTX, true),
                controller_button_field(SDL_CONTROLLER_BUTTON_DPAD_LEFT),
            });
        case recomp::GameInput::DPAD_RIGHT:
            return make_mapping({
                controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTX, false),
                controller_button_field(SDL_CONTROLLER_BUTTON_DPAD_RIGHT),
            });
        case recomp::GameInput::DPAD_UP:
            return make_mapping({
                controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTY, false),
                controller_button_field(SDL_CONTROLLER_BUTTON_DPAD_UP),
            });
        case recomp::GameInput::DPAD_DOWN:
            return make_mapping({
                controller_axis_field(SDL_CONTROLLER_AXIS_RIGHTY, true),
                controller_button_field(SDL_CONTROLLER_BUTTON_DPAD_DOWN),
            });
        case recomp::GameInput::X_AXIS_NEG: return make_mapping({controller_axis_field(SDL_CONTROLLER_AXIS_LEFTX, false)});
        case recomp::GameInput::X_AXIS_POS: return make_mapping({controller_axis_field(SDL_CONTROLLER_AXIS_LEFTX, true)});
        case recomp::GameInput::Y_AXIS_POS: return make_mapping({controller_axis_field(SDL_CONTROLLER_AXIS_LEFTY, false)});
        case recomp::GameInput::Y_AXIS_NEG: return make_mapping({controller_axis_field(SDL_CONTROLLER_AXIS_LEFTY, true)});
        case recomp::GameInput::TOGGLE_MENU: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_BACK)});
        case recomp::GameInput::ACCEPT_MENU: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_A)});
        case recomp::GameInput::APPLY_MENU: return make_mapping({controller_button_field(SDL_CONTROLLER_BUTTON_X)});
        case recomp::GameInput::COUNT: break;
    }

    return {};
}

void reset_bindings_to_defaults(recomp::InputDevice device) {
    auto& mappings = (device == recomp::InputDevice::Controller) ? g_controller_bindings : g_keyboard_bindings;
    for (const InputInfo& info : k_input_info) {
        mappings[static_cast<size_t>(info.input)] = default_binding_for(device, info.input);
    }
}

void initialise_default_menu_bindings() {
    static bool initialised = false;
    if (initialised) {
        return;
    }
    initialised = true;

    reset_bindings_to_defaults(recomp::InputDevice::Controller);
    reset_bindings_to_defaults(recomp::InputDevice::Keyboard);
}

std::string controller_button_prompt(SDL_GameControllerButton button) {
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A: return PF_GAMEPAD_A;
        case SDL_CONTROLLER_BUTTON_B: return PF_GAMEPAD_B;
        case SDL_CONTROLLER_BUTTON_X: return PF_GAMEPAD_X;
        case SDL_CONTROLLER_BUTTON_Y: return PF_GAMEPAD_Y;
        case SDL_CONTROLLER_BUTTON_BACK: return PF_XBOX_VIEW;
        case SDL_CONTROLLER_BUTTON_GUIDE: return PF_GAMEPAD_HOME;
        case SDL_CONTROLLER_BUTTON_START: return PF_XBOX_MENU;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: return PF_ANALOG_L_CLICK;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return PF_ANALOG_R_CLICK;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return PF_XBOX_LEFT_SHOULDER;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return PF_XBOX_RIGHT_SHOULDER;
        case SDL_CONTROLLER_BUTTON_DPAD_UP: return PF_DPAD_UP;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return PF_DPAD_DOWN;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return PF_DPAD_LEFT;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return PF_DPAD_RIGHT;
#ifdef SDL_CONTROLLER_BUTTON_TOUCHPAD
        case SDL_CONTROLLER_BUTTON_TOUCHPAD: return PF_SONY_TOUCHPAD;
#endif
        default:
            return "Button " + std::to_string(static_cast<int>(button));
    }
}

std::string controller_axis_prompt(int32_t encoded_axis) {
    const bool positive = encoded_axis > 0;
    const SDL_GameControllerAxis axis =
        static_cast<SDL_GameControllerAxis>(std::abs(encoded_axis) - 1);
    switch (axis) {
        case SDL_CONTROLLER_AXIS_LEFTX: return positive ? PF_ANALOG_L_RIGHT : PF_ANALOG_L_LEFT;
        case SDL_CONTROLLER_AXIS_LEFTY: return positive ? PF_ANALOG_L_DOWN : PF_ANALOG_L_UP;
        case SDL_CONTROLLER_AXIS_RIGHTX: return positive ? PF_ANALOG_R_RIGHT : PF_ANALOG_R_LEFT;
        case SDL_CONTROLLER_AXIS_RIGHTY: return positive ? PF_ANALOG_R_DOWN : PF_ANALOG_R_UP;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return positive ? PF_XBOX_LEFT_TRIGGER : PF_XBOX_LEFT_TRIGGER_PULL;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return positive ? PF_XBOX_RIGHT_TRIGGER : PF_XBOX_RIGHT_TRIGGER_PULL;
        default:
            return "Axis " + std::to_string(static_cast<int>(axis)) + (positive ? "+" : "-");
    }
}

std::string keyboard_prompt(SDL_Scancode scancode) {
    static const std::unordered_map<SDL_Scancode, std::string> scancode_codepoints{
        {SDL_SCANCODE_LEFT, PF_KEYBOARD_LEFT},
        {SDL_SCANCODE_UP, PF_KEYBOARD_RIGHT},
        {SDL_SCANCODE_RIGHT, PF_KEYBOARD_UP},
        {SDL_SCANCODE_DOWN, PF_KEYBOARD_DOWN},
        {SDL_SCANCODE_A, PF_KEYBOARD_A},
        {SDL_SCANCODE_B, PF_KEYBOARD_B},
        {SDL_SCANCODE_C, PF_KEYBOARD_C},
        {SDL_SCANCODE_D, PF_KEYBOARD_D},
        {SDL_SCANCODE_E, PF_KEYBOARD_E},
        {SDL_SCANCODE_F, PF_KEYBOARD_F},
        {SDL_SCANCODE_G, PF_KEYBOARD_G},
        {SDL_SCANCODE_H, PF_KEYBOARD_H},
        {SDL_SCANCODE_I, PF_KEYBOARD_I},
        {SDL_SCANCODE_J, PF_KEYBOARD_J},
        {SDL_SCANCODE_K, PF_KEYBOARD_K},
        {SDL_SCANCODE_L, PF_KEYBOARD_L},
        {SDL_SCANCODE_M, PF_KEYBOARD_M},
        {SDL_SCANCODE_N, PF_KEYBOARD_N},
        {SDL_SCANCODE_O, PF_KEYBOARD_O},
        {SDL_SCANCODE_P, PF_KEYBOARD_P},
        {SDL_SCANCODE_Q, PF_KEYBOARD_Q},
        {SDL_SCANCODE_R, PF_KEYBOARD_R},
        {SDL_SCANCODE_S, PF_KEYBOARD_S},
        {SDL_SCANCODE_T, PF_KEYBOARD_T},
        {SDL_SCANCODE_U, PF_KEYBOARD_U},
        {SDL_SCANCODE_V, PF_KEYBOARD_V},
        {SDL_SCANCODE_W, PF_KEYBOARD_W},
        {SDL_SCANCODE_X, PF_KEYBOARD_X},
        {SDL_SCANCODE_Y, PF_KEYBOARD_Y},
        {SDL_SCANCODE_Z, PF_KEYBOARD_Z},
        {SDL_SCANCODE_1, PF_KEYBOARD_1},
        {SDL_SCANCODE_2, PF_KEYBOARD_2},
        {SDL_SCANCODE_3, PF_KEYBOARD_3},
        {SDL_SCANCODE_4, PF_KEYBOARD_4},
        {SDL_SCANCODE_5, PF_KEYBOARD_5},
        {SDL_SCANCODE_6, PF_KEYBOARD_6},
        {SDL_SCANCODE_7, PF_KEYBOARD_7},
        {SDL_SCANCODE_8, PF_KEYBOARD_8},
        {SDL_SCANCODE_9, PF_KEYBOARD_9},
        {SDL_SCANCODE_0, PF_KEYBOARD_0},
        {SDL_SCANCODE_F1, PF_KEYBOARD_F1},
        {SDL_SCANCODE_F2, PF_KEYBOARD_F2},
        {SDL_SCANCODE_F3, PF_KEYBOARD_F3},
        {SDL_SCANCODE_F4, PF_KEYBOARD_F4},
        {SDL_SCANCODE_F5, PF_KEYBOARD_F5},
        {SDL_SCANCODE_F6, PF_KEYBOARD_F6},
        {SDL_SCANCODE_F7, PF_KEYBOARD_F7},
        {SDL_SCANCODE_F8, PF_KEYBOARD_F8},
        {SDL_SCANCODE_F9, PF_KEYBOARD_F9},
        {SDL_SCANCODE_F10, PF_KEYBOARD_F10},
        {SDL_SCANCODE_F11, PF_KEYBOARD_F11},
        {SDL_SCANCODE_F12, PF_KEYBOARD_F12},
        {SDL_SCANCODE_SPACE, PF_KEYBOARD_SPACE},
        {SDL_SCANCODE_BACKSPACE, PF_KEYBOARD_BACKSPACE},
        {SDL_SCANCODE_TAB, PF_KEYBOARD_TAB},
        {SDL_SCANCODE_RETURN, PF_KEYBOARD_ENTER},
        {SDL_SCANCODE_ESCAPE, PF_KEYBOARD_ESCAPE},
        {SDL_SCANCODE_LSHIFT, "L" PF_KEYBOARD_SHIFT},
        {SDL_SCANCODE_RSHIFT, "R" PF_KEYBOARD_SHIFT},
    };

    auto it = scancode_codepoints.find(scancode);
    if (it != scancode_codepoints.end()) {
        return it->second;
    }
    const char* name = SDL_GetScancodeName(scancode);
    if (name != nullptr && name[0] != '\0') {
        return name;
    }
    return std::to_string(static_cast<int>(scancode));
}

std::string controller_binding_prompt(recomp::GameInput input) {
    initialise_default_menu_bindings();
    std::string result;
    for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
        const recomp::InputField& binding =
            recomp::get_input_binding(input, binding_index, recomp::InputDevice::Controller);
        if (binding.input_type == kInputControllerDigital) {
            result += controller_button_prompt(static_cast<SDL_GameControllerButton>(binding.input_id));
        } else if (binding.input_type == kInputControllerAnalog) {
            result += controller_axis_prompt(binding.input_id);
        }
    }
    return result;
}

const char* rom_validation_message(recomp::RomValidationError error) {
    switch (error) {
        case recomp::RomValidationError::Good: return "ROM ready.";
        case recomp::RomValidationError::FailedToOpen: return "Failed to open ROM.";
        case recomp::RomValidationError::NotARom: return "That file is not a valid Nintendo 64 ROM.";
        case recomp::RomValidationError::IncorrectRom: return "That is not the supported Legacy of Darkness ROM.";
        case recomp::RomValidationError::NotYet: return "That ROM variant is recognized but not supported yet.";
        case recomp::RomValidationError::IncorrectVersion: return "Wrong version/region. Use Legacy of Darkness (USA).";
        case recomp::RomValidationError::OtherError: return "ROM validation failed.";
    }
    return "ROM validation failed.";
}

std::string hex_u32(uint32_t value) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "0x%08X", value);
    return buf;
}

bool show_debug_tab() {
    const char* value = std::getenv("RECOMP_UI_SHOW_DEBUG");
    return value != nullptr && value[0] != '\0' && value[0] != '0';
}

int32_t current_gamestate() {
    uint8_t* rdram = rdram_ptr_for_debug;
    if (rdram == nullptr) {
        return 0;
    }

    uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
    if (gsm_addr == 0) {
        return 0;
    }

    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (gsm_phys + 0x2C > 0x800000) {
        return 0;
    }

    return *(int32_t*)(rdram + gsm_phys + 0x24);
}

uint32_t current_exec_flags() {
    uint8_t* rdram = rdram_ptr_for_debug;
    return rdram != nullptr ? *(uint32_t*)(rdram + 0x001CABC8) : 0;
}

template <typename Enum>
Enum next_enum_option(Enum value) {
    int raw = static_cast<int>(value) + 1;
    int count = static_cast<int>(Enum::OptionCount);
    if (raw >= count) {
        raw = 0;
    }
    return static_cast<Enum>(raw);
}


const char* config_tab_name(recompui::ConfigTab tab) {
    switch (tab) {
        case recompui::ConfigTab::General: return "General";
        case recompui::ConfigTab::Controls: return "Controls";
        case recompui::ConfigTab::Graphics: return "Graphics";
        case recompui::ConfigTab::Sound: return "Audio";
        case recompui::ConfigTab::Mods: return "Mods";
        case recompui::ConfigTab::Debug: return "Debug";
    }
    return "General";
}

bool graphics_changed() {
    return g_pending_graphics != ultramodern::renderer::get_graphics_config();
}

void dirty_config();

void force_original_display_modes(ultramodern::renderer::GraphicsConfig& config) {
    config.hr_option = ultramodern::renderer::HUDRatioMode::Original;
    config.ar_option = ultramodern::renderer::AspectRatio::Original;
    config.rr_option = ultramodern::renderer::RefreshRate::Original;
}

void normalize_pending_display_modes() {
    if (!g_pending_graphics.experimental_display_modes) {
        force_original_display_modes(g_pending_graphics);
    }
}

void set_pending_experimental_display_modes(bool enabled) {
    if (g_pending_graphics.experimental_display_modes == enabled) {
        return;
    }

    g_pending_graphics.experimental_display_modes = enabled;
    normalize_pending_display_modes();
    g_config_status = enabled
        ? "Experimental display modes unlocked. Original is still recommended for normal play."
        : "Experimental display modes locked. Aspect Ratio, HUD Placement, and Refresh Rate were reset to Original.";
    dirty_config();
}

void request_pending_experimental_display_modes(bool enabled) {
    if (!enabled) {
        set_pending_experimental_display_modes(false);
        return;
    }

    if (g_pending_graphics.experimental_display_modes) {
        return;
    }

    g_config_status = "Confirm Experimental Display Modes before enabling.";
    dirty_config();
    recompui::open_choice_prompt(
        "Enable Experimental Display Modes?",
        "These options unlock aspect, HUD, and refresh overrides. They can cause timing-sensitive bugs, text or item hangs, and culling issues. Original is recommended for normal play.",
        "Enable",
        "Cancel",
        []() {
            set_pending_experimental_display_modes(true);
        },
        []() {
            g_config_status = "Experimental Display Modes remain locked.";
            dirty_config();
        },
        recompui::ButtonVariant::Warning,
        recompui::ButtonVariant::Secondary,
        true,
        "lod_exp_display_modes"
    );
}

const char* experimental_display_modes_status() {
    return g_pending_graphics.experimental_display_modes ? "On" : "Off";
}

void dirty_config() {
    if (!g_config_model) {
        return;
    }
    g_config_model.DirtyAllVariables();
}

void reset_pending_graphics_from_active() {
    g_pending_graphics = ultramodern::renderer::get_graphics_config();
    normalize_pending_display_modes();
    g_audio_config = lod::settings::get_audio_config();
    g_config_path_display = lod::settings::config_path().empty()
        ? std::string{"Registered at startup"}
        : lod::settings::config_path().string();
    g_config_status = "Graphics changes are staged until Apply.";
    g_audio_status = "Volume and mute apply immediately. Audio driver changes require restarting LodRecomp.";
    dirty_config();
}

void apply_pending_graphics(const char* reason) {
    lod::settings::apply_and_save_graphics_config(g_pending_graphics, reason);
    g_config_status = "Graphics settings applied and saved.";
    dirty_config();
}

void discard_pending_graphics() {
    reset_pending_graphics_from_active();
    g_config_status = "Graphics changes discarded.";
    dirty_config();
}

void reset_pending_graphics_defaults() {
    g_pending_graphics = lod::settings::default_graphics_config();
    g_config_status = "Graphics defaults staged. Choose Apply to save them.";
    dirty_config();
}

void apply_audio_config_from_ui(const char* reason) {
    g_audio_config.master_volume = std::clamp(g_audio_config.master_volume, 0, 100);
    lod::settings::apply_and_save_audio_config(g_audio_config, reason);
    g_audio_config = lod::settings::get_audio_config();
    g_audio_status = "Audio settings applied and saved.";
    dirty_config();
}

void hide_config_menu() {
    recompui::hide_context(recompui::get_config_context_id());
    if (!ultramodern::is_game_started()) {
        recompui::show_context(recompui::get_launcher_context_id(), "");
    }
}

void close_config_menu() {
    if (!graphics_changed()) {
        hide_config_menu();
        return;
    }

    recompui::open_choice_prompt(
        "Graphics options have changed",
        "Would you like to apply or discard the changes?",
        "Apply",
        "Discard",
        []() {
            apply_pending_graphics("Zelda menu close");
            hide_config_menu();
        },
        []() {
            discard_pending_graphics();
            hide_config_menu();
        },
        recompui::ButtonVariant::Success,
        recompui::ButtonVariant::Error,
        true,
        "config__close-menu-button"
    );
}

const char* resolution_to_string(ultramodern::renderer::Resolution value) {
    switch (value) {
        case ultramodern::renderer::Resolution::Original: return "Original";
        case ultramodern::renderer::Resolution::Original2x: return "Original2x";
        case ultramodern::renderer::Resolution::Auto: return "Auto";
        case ultramodern::renderer::Resolution::OptionCount: break;
    }
    return "Original2x";
}

void set_resolution_from_string(const std::string& value) {
    if (value == "Original") g_pending_graphics.res_option = ultramodern::renderer::Resolution::Original;
    else if (value == "Original2x") g_pending_graphics.res_option = ultramodern::renderer::Resolution::Original2x;
    else if (value == "Auto") g_pending_graphics.res_option = ultramodern::renderer::Resolution::Auto;
}

const char* window_mode_to_string(ultramodern::renderer::WindowMode value) {
    switch (value) {
        case ultramodern::renderer::WindowMode::Windowed: return "Windowed";
        case ultramodern::renderer::WindowMode::Fullscreen: return "Fullscreen";
        case ultramodern::renderer::WindowMode::OptionCount: break;
    }
    return "Windowed";
}

void set_window_mode_from_string(const std::string& value) {
    if (value == "Windowed") g_pending_graphics.wm_option = ultramodern::renderer::WindowMode::Windowed;
    else if (value == "Fullscreen") g_pending_graphics.wm_option = ultramodern::renderer::WindowMode::Fullscreen;
}

const char* aspect_ratio_to_string(ultramodern::renderer::AspectRatio value) {
    switch (value) {
        case ultramodern::renderer::AspectRatio::Original: return "Original";
        case ultramodern::renderer::AspectRatio::Expand: return "Expand";
        case ultramodern::renderer::AspectRatio::Manual: return "Manual";
        case ultramodern::renderer::AspectRatio::OptionCount: break;
    }
    return "Original";
}

void set_aspect_ratio_from_string(const std::string& value) {
    if (!g_pending_graphics.experimental_display_modes && value != "Original") {
        return;
    }
    if (value == "Original") g_pending_graphics.ar_option = ultramodern::renderer::AspectRatio::Original;
    else if (value == "Expand") g_pending_graphics.ar_option = ultramodern::renderer::AspectRatio::Expand;
    else if (value == "Manual") g_pending_graphics.ar_option = ultramodern::renderer::AspectRatio::Manual;
}

const char* msaa_to_string(ultramodern::renderer::Antialiasing value) {
    switch (value) {
        case ultramodern::renderer::Antialiasing::None: return "None";
        case ultramodern::renderer::Antialiasing::MSAA2X: return "MSAA2X";
        case ultramodern::renderer::Antialiasing::MSAA4X: return "MSAA4X";
        case ultramodern::renderer::Antialiasing::MSAA8X: return "MSAA8X";
        case ultramodern::renderer::Antialiasing::OptionCount: break;
    }
    return "None";
}

void set_msaa_from_string(const std::string& value) {
    if (value == "None") g_pending_graphics.msaa_option = ultramodern::renderer::Antialiasing::None;
    else if (value == "MSAA2X") g_pending_graphics.msaa_option = ultramodern::renderer::Antialiasing::MSAA2X;
    else if (value == "MSAA4X") g_pending_graphics.msaa_option = ultramodern::renderer::Antialiasing::MSAA4X;
    else if (value == "MSAA8X") g_pending_graphics.msaa_option = ultramodern::renderer::Antialiasing::MSAA8X;
}

const char* refresh_rate_to_string(ultramodern::renderer::RefreshRate value) {
    switch (value) {
        case ultramodern::renderer::RefreshRate::Original: return "Original";
        case ultramodern::renderer::RefreshRate::Display: return "Display";
        case ultramodern::renderer::RefreshRate::Manual: return "Manual";
        case ultramodern::renderer::RefreshRate::OptionCount: break;
    }
    return "Original";
}

void set_refresh_rate_from_string(const std::string& value) {
    if (!g_pending_graphics.experimental_display_modes && value != "Original") {
        return;
    }
    if (value == "Original") g_pending_graphics.rr_option = ultramodern::renderer::RefreshRate::Original;
    else if (value == "Display") g_pending_graphics.rr_option = ultramodern::renderer::RefreshRate::Display;
    else if (value == "Manual") g_pending_graphics.rr_option = ultramodern::renderer::RefreshRate::Manual;
}

const char* hud_ratio_to_string(ultramodern::renderer::HUDRatioMode value) {
    switch (value) {
        case ultramodern::renderer::HUDRatioMode::Original: return "Original";
        case ultramodern::renderer::HUDRatioMode::Clamp16x9: return "Clamp16x9";
        case ultramodern::renderer::HUDRatioMode::Full: return "Full";
        case ultramodern::renderer::HUDRatioMode::OptionCount: break;
    }
    return "Original";
}

void set_hud_ratio_from_string(const std::string& value) {
    if (!g_pending_graphics.experimental_display_modes && value != "Original") {
        return;
    }
    if (value == "Original") g_pending_graphics.hr_option = ultramodern::renderer::HUDRatioMode::Original;
    else if (value == "Clamp16x9") g_pending_graphics.hr_option = ultramodern::renderer::HUDRatioMode::Clamp16x9;
    else if (value == "Full") g_pending_graphics.hr_option = ultramodern::renderer::HUDRatioMode::Full;
}

const char* hpfb_to_string(ultramodern::renderer::HighPrecisionFramebuffer value) {
    switch (value) {
        case ultramodern::renderer::HighPrecisionFramebuffer::Auto: return "Auto";
        case ultramodern::renderer::HighPrecisionFramebuffer::On: return "On";
        case ultramodern::renderer::HighPrecisionFramebuffer::Off: return "Off";
        case ultramodern::renderer::HighPrecisionFramebuffer::OptionCount: break;
    }
    return "Auto";
}

void set_hpfb_from_string(const std::string& value) {
    if (value == "Auto") g_pending_graphics.hpfb_option = ultramodern::renderer::HighPrecisionFramebuffer::Auto;
    else if (value == "On") g_pending_graphics.hpfb_option = ultramodern::renderer::HighPrecisionFramebuffer::On;
    else if (value == "Off") g_pending_graphics.hpfb_option = ultramodern::renderer::HighPrecisionFramebuffer::Off;
}

const char* graphics_api_to_string(ultramodern::renderer::GraphicsApi value) {
    switch (value) {
        case ultramodern::renderer::GraphicsApi::Auto: return "Auto";
        case ultramodern::renderer::GraphicsApi::D3D12: return "D3D12";
        case ultramodern::renderer::GraphicsApi::Vulkan: return "Vulkan";
        case ultramodern::renderer::GraphicsApi::Metal: return "Metal";
        case ultramodern::renderer::GraphicsApi::OptionCount: break;
    }
    return "Auto";
}

const char* mute_to_string(bool value) {
    return value ? "On" : "Off";
}

void set_mute_from_string(const std::string& value) {
    g_audio_config.mute = (value == "On");
}

void bind_option_string(Rml::DataModelConstructor& constructor,
                        const char* name,
                        const std::function<const char*()>& getter,
                        const std::function<void(const std::string&)>& setter) {
    constructor.BindFunc(name,
        [getter](Rml::Variant& out) {
            out = getter();
        },
        [setter, name](const Rml::Variant& in) {
            setter(in.Get<std::string>());
            g_config_status = "Graphics changes pending.";
            dirty_config();
        });
}

bool stage_graphics_function_key(Rml::Input::KeyIdentifier key) {
    switch (key) {
        case Rml::Input::KeyIdentifier::KI_F11:
            g_pending_graphics.wm_option =
                g_pending_graphics.wm_option == ultramodern::renderer::WindowMode::Fullscreen
                    ? ultramodern::renderer::WindowMode::Windowed
                    : ultramodern::renderer::WindowMode::Fullscreen;
            break;
        case Rml::Input::KeyIdentifier::KI_F5:
            g_pending_graphics.res_option = next_enum_option(g_pending_graphics.res_option);
            break;
        case Rml::Input::KeyIdentifier::KI_F6:
            if (!g_pending_graphics.experimental_display_modes) {
                g_pending_graphics.ar_option = ultramodern::renderer::AspectRatio::Original;
                g_config_status = "Enable Experimental Display Modes to change Aspect Ratio.";
                dirty_config();
                return true;
            }
            g_pending_graphics.ar_option = next_enum_option(g_pending_graphics.ar_option);
            break;
        case Rml::Input::KeyIdentifier::KI_F7:
            g_pending_graphics.msaa_option = next_enum_option(g_pending_graphics.msaa_option);
            break;
        case Rml::Input::KeyIdentifier::KI_F8:
            if (!g_pending_graphics.experimental_display_modes) {
                g_pending_graphics.rr_option = ultramodern::renderer::RefreshRate::Original;
                g_config_status = "Enable Experimental Display Modes to change Refresh Rate.";
                dirty_config();
                return true;
            }
            g_pending_graphics.rr_option = next_enum_option(g_pending_graphics.rr_option);
            break;
        default:
            return false;
    }

    g_config_status = "Graphics hotkey staged. Choose Apply to save.";
    dirty_config();
    return true;
}

std::string audio_config_path_display() {
    const std::filesystem::path path = lod::settings::audio_config_path();
    return path.empty() ? "audio.json" : path.string();
}

std::string audio_backend_status() {
    const lod::settings::AudioStatus status = lod::settings::audio_status();
    std::ostringstream out;
    out << status.sdl_driver
        << "; input " << status.input_sample_rate << " Hz"
        << "; output " << status.output_sample_rate << " Hz"
        << "; channels " << status.output_channels
        << "; device " << (status.device_open ? "open" : "closed");
    return out.str();
}

std::string graphics_summary(const ultramodern::renderer::GraphicsConfig& config) {
    std::ostringstream out;
    out << "res " << resolution_to_string(config.res_option)
        << ", window " << window_mode_to_string(config.wm_option)
        << ", aspect " << aspect_ratio_to_string(config.ar_option)
        << ", MSAA " << msaa_to_string(config.msaa_option)
        << ", refresh " << refresh_rate_to_string(config.rr_option)
        << ", HPFB " << hpfb_to_string(config.hpfb_option);
    return out.str();
}

void select_rom() {
    g_rom_status = "Opening ROM picker...";
    dirty_launcher();

    zelda64::open_file_dialog([](bool success, const std::filesystem::path& path) {
        if (!success) {
            g_rom_status = "ROM selection cancelled.";
            dirty_launcher();
            return;
        }

        g_rom_file_name = path.filename().string();
        g_rom_status = "Validating " + g_rom_file_name + "...";
        dirty_launcher();

        std::u8string game_id = lod_game_id();
        recomp::RomValidationError result = recomp::select_rom(path, game_id);
        g_rom_valid = (result == recomp::RomValidationError::Good);
        if (g_rom_valid) {
            lod::settings::persist_rom_path(path);
            g_rom_status = "ROM ready: " + g_rom_file_name;
        } else {
            g_rom_status = rom_validation_message(result);
        }
        dirty_launcher();
    });
}

class LodLauncherMenu final : public recompui::MenuController {
public:
    void load_document() override {
        g_launcher_context = recompui::create_context(zelda64::get_asset_path("lod_launcher.rml"));
    }

    void register_events(recompui::UiEventListenerInstancer& listener) override {
        recompui::register_event(listener, "select_rom", [](const std::string&, Rml::Event&) {
            select_rom();
        });
        recompui::register_event(listener, "start_game", [](const std::string&, Rml::Event&) {
            std::u8string game_id = lod_game_id();
            if (!g_rom_valid && !recomp::is_rom_valid(game_id)) {
                g_rom_status = "Select a valid Legacy of Darkness ROM first.";
                dirty_launcher();
                return;
            }
            recomp::start_game(game_id);
            recompui::hide_all_contexts();
            recompui::process_game_started();
        });
        recompui::register_event(listener, "open_settings", [](const std::string&, Rml::Event&) {
            reset_pending_graphics_from_active();
            recompui::set_config_tab(recompui::ConfigTab::General);
            recompui::hide_all_contexts();
            recompui::show_context(recompui::get_config_context_id(), "");
        });
        recompui::register_event(listener, "open_controls", [](const std::string&, Rml::Event&) {
            reset_pending_graphics_from_active();
            recompui::set_config_tab(recompui::ConfigTab::Controls);
            recompui::hide_all_contexts();
            recompui::show_context(recompui::get_config_context_id(), "");
        });
        recompui::register_event(listener, "open_mods", [](const std::string&, Rml::Event&) {
            recompui::set_config_tab(recompui::ConfigTab::Mods);
            recompui::hide_all_contexts();
            recompui::show_context(recompui::get_config_context_id(), "");
        });
        recompui::register_event(listener, "exit_game", [](const std::string&, Rml::Event&) {
            zelda64::open_quit_game_prompt();
        });
    }

    void make_bindings(Rml::Context* context) override {
        initialise_default_menu_bindings();
        std::u8string game_id = lod_game_id();
        g_rom_valid = recomp::is_rom_valid(game_id);
        if (g_rom_valid) {
            g_rom_status = "ROM ready.";
            if (g_rom_file_name == "None selected") {
                g_rom_file_name = "Validated cached ROM";
            }
        }
        g_config_path_display = lod::settings::config_path().empty()
            ? std::string{"Registered at startup"}
            : lod::settings::config_path().string();

        Rml::DataModelConstructor constructor = context->CreateDataModel("launcher_model");
        constructor.Bind("rom_valid", &g_rom_valid);
        constructor.Bind("rom_status", &g_rom_status);
        constructor.Bind("rom_file", &g_rom_file_name);
        constructor.Bind("config_path", &g_config_path_display);
        g_version_string = recomp::get_project_version().to_string();
        constructor.Bind("version_number", &g_version_string);
        g_launcher_model = constructor.GetModelHandle();
    }
};

class LodConfigMenu final : public recompui::MenuController {
public:
    void load_document() override {
        g_config_context = recompui::create_context(zelda64::get_asset_path("lod_config.rml"));
        g_config_context.set_captures_input(true);
        g_config_context.set_captures_mouse(true);
    }

    void register_events(recompui::UiEventListenerInstancer& listener) override {
        recompui::register_event(listener, "close_config_menu_backdrop", [](const std::string&, Rml::Event& event) {
            if (event.GetPhase() == Rml::EventPhase::Target) {
                close_config_menu();
            }
        });
        recompui::register_event(listener, "close_config_menu", [](const std::string&, Rml::Event&) {
            close_config_menu();
        });
        recompui::register_event(listener, "open_quit_game_prompt", [](const std::string&, Rml::Event&) {
            zelda64::open_quit_game_prompt();
        });
        recompui::register_event(listener, "toggle_input_device", [](const std::string&, Rml::Event&) {
            g_current_input_device = g_current_input_device == recomp::InputDevice::Controller
                ? recomp::InputDevice::Keyboard
                : recomp::InputDevice::Controller;
            dirty_controls();
        });
        recompui::register_event(listener, "apply_graphics", [](const std::string&, Rml::Event&) {
            apply_pending_graphics("Zelda menu apply");
        });
        recompui::register_event(listener, "discard_graphics", [](const std::string&, Rml::Event&) {
            discard_pending_graphics();
        });
        recompui::register_event(listener, "reset_graphics", [](const std::string&, Rml::Event&) {
            reset_pending_graphics_defaults();
        });
        recompui::register_event(listener, "toggle_experimental_display_modes", [](const std::string&, Rml::Event&) {
            request_pending_experimental_display_modes(!g_pending_graphics.experimental_display_modes);
        });
        recompui::register_event(listener, "select_rom", [](const std::string&, Rml::Event&) {
            select_rom();
        });
        recompui::register_event(listener, "show_general_tab", [](const std::string&, Rml::Event&) {
            recompui::set_config_tab(recompui::ConfigTab::General);
        });
        recompui::register_event(listener, "show_graphics_tab", [](const std::string&, Rml::Event&) {
            recompui::set_config_tab(recompui::ConfigTab::Graphics);
        });
        recompui::register_event(listener, "show_controls_tab", [](const std::string&, Rml::Event&) {
            recompui::set_config_tab(recompui::ConfigTab::Controls);
        });
        recompui::register_event(listener, "show_audio_tab", [](const std::string&, Rml::Event&) {
            recompui::set_config_tab(recompui::ConfigTab::Sound);
        });
        recompui::register_event(listener, "show_debug_tab", [](const std::string&, Rml::Event&) {
            if (show_debug_tab()) {
                recompui::set_config_tab(recompui::ConfigTab::Debug);
            }
        });
        recompui::register_event(listener, "config_keydown", [](const std::string&, Rml::Event& event) {
            if (recompui::is_prompt_open() || event.GetId() != Rml::EventId::Keydown) {
                return;
            }
            const auto key = event.GetParameter<Rml::Input::KeyIdentifier>(
                "key_identifier",
                Rml::Input::KeyIdentifier::KI_UNKNOWN);
            if (key == Rml::Input::KeyIdentifier::KI_ESCAPE) {
                close_config_menu();
            } else if (stage_graphics_function_key(key)) {
                return;
            } else if (key == Rml::Input::KeyIdentifier::KI_F) {
                apply_pending_graphics("Zelda menu key apply");
            }
        });
    }

    void make_controls_bindings(Rml::Context* context) {
        Rml::DataModelConstructor constructor = context->CreateDataModel("controls_model");
        if (!constructor) {
            throw std::runtime_error("Failed to make RmlUi data model for the controls config menu");
        }

        constructor.BindFunc("input_count", [](Rml::Variant& out) {
            out = static_cast<uint64_t>(recomp::get_num_inputs());
        });
        constructor.BindFunc("input_device_is_keyboard", [](Rml::Variant& out) {
            out = g_current_input_device == recomp::InputDevice::Keyboard;
        });

        constructor.RegisterTransformFunc("get_input_name", [](const Rml::VariantList& inputs) {
            return Rml::Variant{
                recomp::get_input_name(static_cast<recomp::GameInput>(inputs.at(0).Get<size_t>()))
            };
        });

        constructor.RegisterTransformFunc("get_input_enum_name", [](const Rml::VariantList& inputs) {
            return Rml::Variant{
                recomp::get_input_enum_name(static_cast<recomp::GameInput>(inputs.at(0).Get<size_t>()))
            };
        });

        constructor.BindEventCallback("set_input_binding",
            [](Rml::DataModelHandle model_handle, Rml::Event&, const Rml::VariantList& inputs) {
                if (inputs.size() < 2) {
                    return;
                }
                g_scanned_input_index = static_cast<int>(inputs.at(0).Get<size_t>());
                g_scanned_binding_index = static_cast<int>(inputs.at(1).Get<size_t>());
                recomp::start_scanning_input(g_current_input_device);
                model_handle.DirtyVariable("active_binding_input");
                model_handle.DirtyVariable("active_binding_slot");
            });

        constructor.BindEventCallback("reset_input_bindings_to_defaults",
            [](Rml::DataModelHandle model_handle, Rml::Event&, const Rml::VariantList&) {
                if (g_current_input_device == recomp::InputDevice::Controller) {
                    zelda64::reset_cont_input_bindings();
                } else {
                    zelda64::reset_kb_input_bindings();
                }
                zelda64::save_config();
                model_handle.DirtyAllVariables();
                dirty_controls();
            });

        constructor.BindEventCallback("clear_input_bindings",
            [](Rml::DataModelHandle model_handle, Rml::Event&, const Rml::VariantList& inputs) {
                if (inputs.empty()) {
                    return;
                }
                const auto input = static_cast<recomp::GameInput>(inputs.at(0).Get<size_t>());
                for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
                    recomp::set_input_binding(input, binding_index, g_current_input_device, recomp::InputField{});
                }
                zelda64::save_config();
                model_handle.DirtyVariable("inputs");
                dirty_controls();
            });

        constructor.BindEventCallback("reset_single_input_binding_to_default",
            [](Rml::DataModelHandle model_handle, Rml::Event&, const Rml::VariantList& inputs) {
                if (inputs.empty()) {
                    return;
                }
                const auto input = static_cast<recomp::GameInput>(inputs.at(0).Get<size_t>());
                zelda64::reset_single_input_binding(g_current_input_device, input);
                zelda64::save_config();
                model_handle.DirtyVariable("inputs");
                dirty_controls();
            });

        constructor.BindEventCallback("set_input_row_focus",
            [](Rml::DataModelHandle model_handle, Rml::Event& event, const Rml::VariantList& inputs) {
                if (inputs.empty()) {
                    return;
                }
                const int input_index = inputs.at(0).Get<int>();
                if (input_index == -1 &&
                    event.GetType() == "mouseout" &&
                    event.GetCurrentElement() != event.GetTargetElement()) {
                    return;
                }
                g_focused_input_index = input_index;
                model_handle.DirtyVariable("cur_input_row");
            });

        struct InputFieldVariableDefinition : public Rml::VariableDefinition {
            InputFieldVariableDefinition() : Rml::VariableDefinition(Rml::DataVariableType::Scalar) {}

            bool Get(void* ptr, Rml::Variant& variant) override {
                variant = reinterpret_cast<recomp::InputField*>(ptr)->to_string();
                return true;
            }
            bool Set(void*, const Rml::Variant&) override {
                return false;
            }
        };
        static InputFieldVariableDefinition input_field_definition_instance{};

        struct BindingContainerVariableDefinition : public Rml::VariableDefinition {
            BindingContainerVariableDefinition() : Rml::VariableDefinition(Rml::DataVariableType::Array) {}

            bool Get(void*, Rml::Variant&) override {
                return false;
            }
            bool Set(void*, const Rml::Variant&) override {
                return false;
            }

            int Size(void*) override {
                return static_cast<int>(recomp::bindings_per_input);
            }
            Rml::DataVariable Child(void* ptr, const Rml::DataAddressEntry& address) override {
                const auto input = static_cast<recomp::GameInput>(
                    static_cast<size_t>(reinterpret_cast<uintptr_t>(ptr)));
                return Rml::DataVariable{
                    &input_field_definition_instance,
                    &recomp::get_input_binding(input, address.index, g_current_input_device)
                };
            }
        };
        static BindingContainerVariableDefinition binding_container_var_instance{};

        struct BindingArrayContainerVariableDefinition : public Rml::VariableDefinition {
            BindingArrayContainerVariableDefinition() : Rml::VariableDefinition(Rml::DataVariableType::Array) {}

            bool Get(void*, Rml::Variant&) override {
                return false;
            }
            bool Set(void*, const Rml::Variant&) override {
                return false;
            }

            int Size(void*) override {
                return static_cast<int>(recomp::get_num_inputs());
            }
            Rml::DataVariable Child(void*, const Rml::DataAddressEntry& address) override {
                return Rml::DataVariable(
                    &binding_container_var_instance,
                    reinterpret_cast<void*>(static_cast<uintptr_t>(address.index))
                );
            }
        };
        static BindingArrayContainerVariableDefinition binding_array_var_instance{};

        struct InputContainerVariableDefinition : public Rml::VariableDefinition {
            InputContainerVariableDefinition() : Rml::VariableDefinition(Rml::DataVariableType::Struct) {}

            bool Get(void*, Rml::Variant&) override {
                return true;
            }
            bool Set(void*, const Rml::Variant&) override {
                return false;
            }

            int Size(void*) override {
                return static_cast<int>(recomp::get_num_inputs());
            }
            Rml::DataVariable Child(void*, const Rml::DataAddressEntry& address) override {
                if (address.name == "array") {
                    return Rml::DataVariable(&binding_array_var_instance, nullptr);
                }

                const recomp::GameInput input = recomp::get_input_from_enum_name(address.name);
                if (input != recomp::GameInput::COUNT) {
                    return Rml::DataVariable(
                        &binding_container_var_instance,
                        reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<size_t>(input)))
                    );
                }

                return Rml::DataVariable{};
            }
        };

        struct InputContainer {};
        constructor.RegisterCustomDataVariableDefinition<InputContainer>(
            Rml::MakeUnique<InputContainerVariableDefinition>());

        static InputContainer dummy_container;
        constructor.Bind("inputs", &dummy_container);

        constructor.BindFunc("cur_input_row", [](Rml::Variant& out) {
            if (g_focused_input_index == -1) {
                out = "NONE";
            } else {
                out = recomp::get_input_enum_name(static_cast<recomp::GameInput>(g_focused_input_index));
            }
        });

        constructor.BindFunc("active_binding_input", [](Rml::Variant& out) {
            if (g_scanned_input_index == -1) {
                out = "NONE";
            } else {
                out = recomp::get_input_enum_name(static_cast<recomp::GameInput>(g_scanned_input_index));
            }
        });

        constructor.Bind<int>("active_binding_slot", &g_scanned_binding_index);

        g_controls_model = constructor.GetModelHandle();
    }

    void make_bindings(Rml::Context* context) override {
        reset_pending_graphics_from_active();
        Rml::DataModelConstructor constructor = context->CreateDataModel("config_model");
        constructor.BindEventCallback("set_cur_config_index",
            [](Rml::DataModelHandle model_handle, Rml::Event& event, const Rml::VariantList& inputs) {
                if (inputs.empty()) {
                    return;
                }

                int option_index = inputs.at(0).Get<int>();
                if (option_index == -1 &&
                    event.GetType() == "mouseout" &&
                    event.GetCurrentElement() != event.GetTargetElement()) {
                    return;
                }

                g_cur_config_index = option_index;
                model_handle.DirtyVariable("cur_config_index");
            });
        constructor.Bind("cur_config_index", &g_cur_config_index);
        constructor.BindFunc("active_tab", [](Rml::Variant& out) {
            out = config_tab_name(g_active_tab);
        });
        constructor.BindFunc("options_changed", [](Rml::Variant& out) {
            out = graphics_changed();
        });
        constructor.BindFunc("gfx_help__apply", [](Rml::Variant& out) {
            if (recompui::get_cont_active()) {
                const std::string prompt = controller_binding_prompt(recomp::GameInput::APPLY_MENU);
                out = prompt.empty() ? std::string{PF_XBOX_X} : prompt;
            } else {
                out = " " PF_KEYBOARD_F;
            }
        });
        if (g_version_string.empty()) {
            g_version_string = recomp::get_project_version().to_string();
        }
        constructor.Bind("version_number", &g_version_string);
        constructor.Bind("rom_status", &g_rom_status);
        constructor.Bind("rom_file", &g_rom_file_name);
        constructor.Bind("config_status", &g_config_status);
        constructor.Bind("config_path", &g_config_path_display);
        constructor.Bind("audio_status", &g_audio_status);
        constructor.BindFunc("debug_visible", [](Rml::Variant& out) {
            out = show_debug_tab();
        });
        constructor.BindFunc("portable_mode", [](Rml::Variant& out) {
            out = lod::settings::portable_mode_enabled() ? "On" : "Off";
        });
        constructor.BindFunc("background_input_status", [](Rml::Variant& out) {
            out = recomp::get_background_input_mode() == recomp::BackgroundInputMode::On ? "On" : "Off";
        });
        constructor.BindFunc("debug_visibility_status", [](Rml::Variant& out) {
            out = show_debug_tab() ? "Visible via RECOMP_UI_SHOW_DEBUG" : "Hidden";
        });
        constructor.BindFunc("display_refresh_rate", [](Rml::Variant& out) {
            out = ultramodern::get_display_refresh_rate();
        });
        constructor.BindFunc("experimental_display_modes", [](Rml::Variant& out) {
            out = g_pending_graphics.experimental_display_modes;
        });
        constructor.BindFunc("experimental_display_modes_status", [](Rml::Variant& out) {
            out = experimental_display_modes_status();
        });
        constructor.BindFunc("experimental_display_modes_option",
            [](Rml::Variant& out) {
                out = experimental_display_modes_status();
            },
            [](const Rml::Variant& in) {
                request_pending_experimental_display_modes(in.Get<std::string>() == "On");
            });
        bind_option_string(constructor, "res_option",
            []() { return resolution_to_string(g_pending_graphics.res_option); },
            [](const std::string& value) { set_resolution_from_string(value); });
        bind_option_string(constructor, "wm_option",
            []() { return window_mode_to_string(g_pending_graphics.wm_option); },
            [](const std::string& value) { set_window_mode_from_string(value); });
        bind_option_string(constructor, "ar_option",
            []() { return aspect_ratio_to_string(g_pending_graphics.ar_option); },
            [](const std::string& value) { set_aspect_ratio_from_string(value); });
        bind_option_string(constructor, "msaa_option",
            []() { return msaa_to_string(g_pending_graphics.msaa_option); },
            [](const std::string& value) { set_msaa_from_string(value); });
        bind_option_string(constructor, "rr_option",
            []() { return refresh_rate_to_string(g_pending_graphics.rr_option); },
            [](const std::string& value) { set_refresh_rate_from_string(value); });
        bind_option_string(constructor, "hr_option",
            []() { return hud_ratio_to_string(g_pending_graphics.hr_option); },
            [](const std::string& value) { set_hud_ratio_from_string(value); });
        bind_option_string(constructor, "hpfb_option",
            []() { return hpfb_to_string(g_pending_graphics.hpfb_option); },
            [](const std::string& value) { set_hpfb_from_string(value); });
        constructor.BindFunc("ds_option",
            [](Rml::Variant& out) {
                out = g_pending_graphics.ds_option;
            },
            [](const Rml::Variant& in) {
                g_pending_graphics.ds_option = std::clamp(in.Get<int>(), 1, 8);
                g_config_status = "Graphics changes pending.";
                dirty_config();
            });
        constructor.BindFunc("rr_manual_value",
            [](Rml::Variant& out) {
                out = g_pending_graphics.rr_manual_value;
            },
            [](const Rml::Variant& in) {
                g_pending_graphics.rr_manual_value = std::clamp(in.Get<int>(), 20, 360);
                g_config_status = "Graphics changes pending.";
                dirty_config();
            });
        constructor.BindFunc("manual_refresh_enabled", [](Rml::Variant& out) {
            out = g_pending_graphics.experimental_display_modes &&
                  g_pending_graphics.rr_option == ultramodern::renderer::RefreshRate::Manual;
        });
        constructor.BindFunc("graphics_api", [](Rml::Variant& out) {
            out = graphics_api_to_string(g_pending_graphics.api_option);
        });
        constructor.BindFunc("graphics_summary", [](Rml::Variant& out) {
            out = graphics_summary(ultramodern::renderer::get_graphics_config());
        });
        constructor.BindFunc("audio_master_volume",
            [](Rml::Variant& out) {
                out = g_audio_config.master_volume;
            },
            [](const Rml::Variant& in) {
                const int new_volume = std::clamp(in.Get<int>(), 0, 100);
                if (new_volume == g_audio_config.master_volume) {
                    return;
                }
                g_audio_config.master_volume = new_volume;
                apply_audio_config_from_ui("Zelda menu audio volume");
            });
        constructor.BindFunc("audio_mute_mode",
            [](Rml::Variant& out) {
                out = mute_to_string(g_audio_config.mute);
            },
            [](const Rml::Variant& in) {
                const std::string value = in.Get<std::string>();
                const bool old_mute = g_audio_config.mute;
                set_mute_from_string(value);
                if (old_mute == g_audio_config.mute) {
                    return;
                }
                apply_audio_config_from_ui("Zelda menu audio mute");
            });
        constructor.BindFunc("audio_driver_mode",
            [](Rml::Variant& out) {
                out = lod::settings::normalize_audio_driver_setting(g_audio_config.sdl_driver);
            },
            [](const Rml::Variant& in) {
                const std::string old_driver = lod::settings::normalize_audio_driver_setting(g_audio_config.sdl_driver);
                const std::string new_driver = lod::settings::normalize_audio_driver_setting(in.Get<std::string>());
                if (old_driver == new_driver) {
                    return;
                }
                g_audio_config.sdl_driver = new_driver;
                apply_audio_config_from_ui("Zelda menu audio driver");
                g_audio_status = "Audio driver saved. Restart LodRecomp for the new backend to take effect.";
                dirty_config();
            });
        constructor.BindFunc("audio_driver_wasapi_supported", [](Rml::Variant& out) {
            out = lod::settings::audio_driver_setting_supported("wasapi");
        });
        constructor.BindFunc("audio_driver_directsound_supported", [](Rml::Variant& out) {
            out = lod::settings::audio_driver_setting_supported("directsound");
        });
        constructor.BindFunc("audio_driver_pulseaudio_supported", [](Rml::Variant& out) {
            out = lod::settings::audio_driver_setting_supported("pulseaudio");
        });
        constructor.BindFunc("audio_driver_pipewire_supported", [](Rml::Variant& out) {
            out = lod::settings::audio_driver_setting_supported("pipewire");
        });
        constructor.BindFunc("audio_driver_alsa_supported", [](Rml::Variant& out) {
            out = lod::settings::audio_driver_setting_supported("alsa");
        });
        constructor.BindFunc("audio_driver_coreaudio_supported", [](Rml::Variant& out) {
            out = lod::settings::audio_driver_setting_supported("coreaudio");
        });
        constructor.BindFunc("audio_backend_status", [](Rml::Variant& out) {
            out = audio_backend_status();
        });
        constructor.BindFunc("audio_config_path", [](Rml::Variant& out) {
            out = audio_config_path_display();
        });
        constructor.BindFunc("audio_queue_status", [](Rml::Variant& out) {
            const lod::settings::AudioStatus status = lod::settings::audio_status();
            out = std::to_string(status.queued_bytes) + " bytes queued";
        });
        constructor.BindFunc("debug_gamestate", [](Rml::Variant& out) {
            out = std::to_string(current_gamestate());
        });
        constructor.BindFunc("debug_exec_flags", [](Rml::Variant& out) {
            out = hex_u32(current_exec_flags());
        });
        constructor.BindFunc("debug_map_rom", [](Rml::Variant& out) {
            out = hex_u32(lod_current_map_overlay_rom());
        });
        constructor.BindFunc("debug_map_size", [](Rml::Variant& out) {
            out = hex_u32(lod_current_map_overlay_size());
        });
        constructor.BindFunc("debug_map_loads", [](Rml::Variant& out) {
            out = std::to_string(lod_current_map_overlay_load_count());
        });
        constructor.BindFunc("debug_ni_0f", [](Rml::Variant& out) {
            out = std::to_string(lod_ni_overlay_loaded_0f_pair());
        });
        constructor.BindFunc("debug_ni_0e", [](Rml::Variant& out) {
            out = std::to_string(lod_ni_overlay_loaded_0e_pair());
        });
        g_config_model = constructor.GetModelHandle();

        make_controls_bindings(context);

        Rml::DataModelConstructor nav_constructor = context->CreateDataModel("nav_help_model");
        if (!nav_constructor) {
            throw std::runtime_error("Failed to make RmlUi data model for LoD nav help");
        }

        nav_constructor.BindFunc("nav_help__navigate", [](Rml::Variant& out) {
            out = recompui::get_cont_active()
                ? PF_DPAD
                : PF_KEYBOARD_ARROWS PF_KEYBOARD_TAB;
        });

        nav_constructor.BindFunc("nav_help__accept", [](Rml::Variant& out) {
            if (recompui::get_cont_active()) {
                const std::string prompt = controller_binding_prompt(recomp::GameInput::ACCEPT_MENU);
                out = prompt.empty() ? std::string{PF_XBOX_A} : prompt;
            } else {
                out = PF_KEYBOARD_ENTER;
            }
        });

        nav_constructor.BindFunc("nav_help__exit", [](Rml::Variant& out) {
            if (recompui::get_cont_active()) {
                const std::string prompt = controller_binding_prompt(recomp::GameInput::TOGGLE_MENU);
                out = prompt.empty() ? std::string{PF_XBOX_B} : prompt;
            } else {
                out = PF_KEYBOARD_ESCAPE;
            }
        });

        g_nav_help_model = nav_constructor.GetModelHandle();
    }
};
} // namespace

std::string recomp::InputField::to_string() const {
    switch (static_cast<recomp::InputType>(input_type)) {
        case recomp::InputType::None:
            return "";
        case recomp::InputType::ControllerDigital:
            return controller_button_prompt(static_cast<SDL_GameControllerButton>(input_id));
        case recomp::InputType::ControllerAnalog:
            return controller_axis_prompt(input_id);
        case recomp::InputType::Keyboard:
            return keyboard_prompt(static_cast<SDL_Scancode>(input_id));
        case recomp::InputType::Mouse:
            return "Mouse " + std::to_string(input_id);
    }
    return std::to_string(input_type) + "," + std::to_string(input_id);
}

recomp::InputField& recomp::get_input_binding(GameInput input, size_t binding_index, InputDevice device) {
    initialise_default_menu_bindings();
    auto& mappings = (device == InputDevice::Controller) ? g_controller_bindings : g_keyboard_bindings;
    if (static_cast<size_t>(input) >= mappings.size() || binding_index >= bindings_per_input) {
        static InputField dummy{};
        return dummy;
    }
    return mappings[static_cast<size_t>(input)][binding_index];
}

void recomp::set_input_binding(GameInput input, size_t binding_index, InputDevice device, InputField value) {
    initialise_default_menu_bindings();
    auto& mappings = (device == InputDevice::Controller) ? g_controller_bindings : g_keyboard_bindings;
    if (static_cast<size_t>(input) < mappings.size() && binding_index < bindings_per_input) {
        mappings[static_cast<size_t>(input)][binding_index] = value;
    }
}

void recomp::start_scanning_input(InputDevice device) {
    g_scanning_device = device;
}

void recomp::stop_scanning_input() {
    g_scanning_device = InputDevice::COUNT;
}

void recomp::finish_scanning_input(InputField scanned_field) {
    initialise_default_menu_bindings();
    if (g_scanned_input_index >= 0 &&
        g_scanned_binding_index >= 0 &&
        g_scanned_binding_index < static_cast<int>(bindings_per_input) &&
        g_scanning_device != InputDevice::COUNT) {
        set_input_binding(
            static_cast<GameInput>(g_scanned_input_index),
            static_cast<size_t>(g_scanned_binding_index),
            g_scanning_device,
            scanned_field);
        zelda64::save_config();
    }

    g_scanning_device = InputDevice::COUNT;
    g_scanned_input_index = -1;
    g_scanned_binding_index = -1;
    dirty_controls();
}

void recomp::cancel_scanning_input() {
    g_scanning_device = InputDevice::COUNT;
    g_scanned_input_index = -1;
    g_scanned_binding_index = -1;
    dirty_controls();
}

void recomp::config_menu_set_cont_or_kb(bool) {
    dirty_nav_help();
    if (g_config_model) {
        g_config_model.DirtyVariable("gfx_help__apply");
    }
}
recomp::InputField recomp::get_scanned_input() { return {}; }
int recomp::get_scanned_input_index() { return g_scanned_input_index; }
recomp::InputDevice recomp::get_scanning_input_device() { return g_scanning_device; }

size_t recomp::get_num_inputs() {
    return static_cast<size_t>(GameInput::COUNT);
}

const std::string& recomp::get_input_name(GameInput input) {
    static const std::array<std::string, static_cast<size_t>(GameInput::COUNT)> input_names = [] {
        std::array<std::string, static_cast<size_t>(GameInput::COUNT)> names{};
        for (const InputInfo& info : k_input_info) {
            names[static_cast<size_t>(info.input)] = std::string{info.display_name};
        }
        return names;
    }();
    return input_names.at(static_cast<size_t>(input));
}

const std::string& recomp::get_input_enum_name(GameInput input) {
    static const std::array<std::string, static_cast<size_t>(GameInput::COUNT)> enum_names = [] {
        std::array<std::string, static_cast<size_t>(GameInput::COUNT)> names{};
        for (const InputInfo& info : k_input_info) {
            names[static_cast<size_t>(info.input)] = std::string{info.enum_name};
        }
        return names;
    }();
    return enum_names.at(static_cast<size_t>(input));
}

recomp::GameInput recomp::get_input_from_enum_name(std::string_view name) {
    for (const InputInfo& info : k_input_info) {
        if (info.enum_name == name) {
            return info.input;
        }
    }
    return GameInput::COUNT;
}

recomp::BackgroundInputMode recomp::get_background_input_mode() { return g_background_input_mode; }
void recomp::set_background_input_mode(BackgroundInputMode mode) { g_background_input_mode = mode; }
bool recomp::game_input_disabled() { return recompui::is_context_capturing_input(); }
bool recomp::all_input_disabled() { return g_scanning_device != InputDevice::COUNT; }

std::filesystem::path zelda64::get_program_path() {
#if defined(__APPLE__)
    return lod::get_bundle_resource_directory();
#else
    char* base_path = SDL_GetBasePath();
    if (base_path != nullptr) {
        std::filesystem::path path(base_path);
        SDL_free(base_path);
        return path;
    }
    return std::filesystem::current_path();
#endif
}

std::filesystem::path zelda64::get_asset_path(const char* asset) {
#if defined(__APPLE__)
    std::filesystem::path bundled_asset = lod::get_bundle_resource_directory() / "assets" / asset;
    if (std::filesystem::exists(bundled_asset)) {
        return bundled_asset;
    }
#endif

    std::filesystem::path executable_asset = zelda64::get_program_path() / "assets" / asset;
    if (std::filesystem::exists(executable_asset)) {
        return executable_asset;
    }

    return std::filesystem::current_path() / "assets" / asset;
}

void zelda64::open_file_dialog(std::function<void(bool success, const std::filesystem::path& path)> callback) {
    FileDialogRequest request;
    request.mode = FileDialogMode::SingleRom;
    request.single_callback = std::move(callback);

    if (!enqueue_file_dialog_request(std::move(request))) {
        recompui::queue_ui_thread_callback(
            [callback = std::move(request.single_callback)]() mutable {
                if (callback) {
                    callback(false, {});
                }
            });
    }
}

void zelda64::open_file_dialog_multiple(std::function<void(bool success, const std::list<std::filesystem::path>& paths)> callback) {
    FileDialogRequest request;
    request.mode = FileDialogMode::MultipleFiles;
    request.multiple_callback = std::move(callback);

    if (!enqueue_file_dialog_request(std::move(request))) {
        recompui::queue_ui_thread_callback(
            [callback = std::move(request.multiple_callback)]() mutable {
                if (callback) {
                    callback(false, {});
                }
            });
    }
}

void zelda64::process_pending_file_dialogs() {
    FileDialogRequest request;
    {
        std::lock_guard lock{g_file_dialog_mutex};
        if (g_file_dialog_active || g_file_dialog_requests.empty()) {
            return;
        }

        request = std::move(g_file_dialog_requests.front());
        g_file_dialog_requests.pop_front();
        g_file_dialog_active = true;
    }

    FileDialogResult result = request.mode == FileDialogMode::MultipleFiles
        ? run_multiple_file_dialog()
        : run_single_rom_file_dialog();

    {
        std::lock_guard lock{g_file_dialog_mutex};
        g_file_dialog_active = false;
    }

    queue_file_dialog_completion(std::move(request), std::move(result));
}

void zelda64::show_error_message_box(const char* title, const char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
}

void zelda64::save_config() {
    lod_save_controls_bindings_from_ui();
}

void zelda64::reset_input_bindings() {
    reset_bindings_to_defaults(recomp::InputDevice::Controller);
    reset_bindings_to_defaults(recomp::InputDevice::Keyboard);
    dirty_controls();
}

void zelda64::reset_cont_input_bindings() {
    reset_bindings_to_defaults(recomp::InputDevice::Controller);
    dirty_controls();
}

void zelda64::reset_kb_input_bindings() {
    reset_bindings_to_defaults(recomp::InputDevice::Keyboard);
    dirty_controls();
}

void zelda64::reset_single_input_binding(recomp::InputDevice device, recomp::GameInput input) {
    recomp::set_input_binding(input, 0, device, {});
    recomp::set_input_binding(input, 1, device, {});
    const input_mapping defaults = default_binding_for(device, input);
    for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
        recomp::set_input_binding(input, binding_index, device, defaults[binding_index]);
    }
    dirty_controls();
}

bool zelda64::get_debug_mode_enabled() { return g_debug_enabled; }
void zelda64::set_debug_mode_enabled(bool enabled) { g_debug_enabled = enabled; }
void zelda64::open_quit_game_prompt() {
    recompui::open_choice_prompt(
        "Quit game?",
        "Are you sure you want to quit?",
        "Quit",
        "Cancel",
        []() {
            ultramodern::quit();
        },
        []() {},
        recompui::ButtonVariant::Error,
        recompui::ButtonVariant::Secondary,
        true,
        "config__quit-game-button"
    );
}

std::unique_ptr<recompui::MenuController> recompui::create_launcher_menu() {
    return std::make_unique<LodLauncherMenu>();
}

std::unique_ptr<recompui::MenuController> recompui::create_config_menu() {
    return std::make_unique<LodConfigMenu>();
}

recompui::ContextId recompui::get_launcher_context_id() {
    return g_launcher_context;
}

recompui::ContextId recompui::get_config_context_id() {
    return g_config_context;
}

recompui::ContextId recompui::get_config_sub_menu_context_id() {
    return recompui::ContextId::null();
}

void recompui::set_config_tab(ConfigTab tab) {
    if (!recompui::is_context_shown(g_config_context)) {
        reset_pending_graphics_from_active();
    }
    g_active_tab = tab;
    g_cur_config_index = -1;
    dirty_config();

    Rml::ElementTabSet* tabset = recompui::get_config_tabset();
    if (tabset != nullptr) {
        tabset->SetActiveTab(config_tab_to_index(tab));
    }
}

int recompui::config_tab_to_index(ConfigTab tab) {
    switch (tab) {
        case ConfigTab::General: return 0;
        case ConfigTab::Controls: return 1;
        case ConfigTab::Graphics: return 2;
        case ConfigTab::Sound: return 3;
        case ConfigTab::Mods: return 0;
        case ConfigTab::Debug: return 4;
    }
    return 0;
}

Rml::ElementTabSet* recompui::get_config_tabset() {
    if (g_config_context == recompui::ContextId::null()) {
        return nullptr;
    }

    ContextId old_context = recompui::try_close_current_context();
    Rml::ElementDocument* doc = g_config_context.get_document();
    Rml::ElementTabSet* tabset = nullptr;

    if (doc != nullptr) {
        Rml::Element* tabset_el = doc->GetElementById("config_tabset");
        tabset = rmlui_dynamic_cast<Rml::ElementTabSet*>(tabset_el);
    }

    if (old_context != ContextId::null()) {
        old_context.open();
    }

    return tabset;
}

Rml::Element* recompui::get_mod_tab() {
    return nullptr;
}

void recompui::set_config_tabset_mod_nav() {}
void recompui::focus_mod_configure_button() {}
void recompui::update_mod_list(bool) {}
void recompui::process_game_started() {}
void recompui::toggle_fullscreen() {}

void recompui::set_cursor_visible(bool visible) {
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
}

void recompui::queue_ui_callback(recompui::ResourceId, const recompui::Event&, const recompui::UICallback&) {}
