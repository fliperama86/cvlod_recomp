#include "recomp_ui.h"
#include "recomp_input.h"
#include "zelda_config.h"
#include "zelda_support.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
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

namespace {
constexpr uint32_t kInputNone = 0;
constexpr uint32_t kInputControllerDigital = 3;

using input_mapping = std::array<recomp::InputField, recomp::bindings_per_input>;
std::array<input_mapping, static_cast<size_t>(recomp::GameInput::COUNT)> g_controller_bindings{};
std::array<input_mapping, static_cast<size_t>(recomp::GameInput::COUNT)> g_keyboard_bindings{};
recomp::BackgroundInputMode g_background_input_mode = recomp::BackgroundInputMode::On;
bool g_debug_enabled = false;
int g_cur_config_index = -1;

recompui::ContextId g_launcher_context = recompui::ContextId::null();
recompui::ContextId g_config_context = recompui::ContextId::null();
recompui::ConfigTab g_active_tab = recompui::ConfigTab::General;
Rml::DataModelHandle g_launcher_model;
Rml::DataModelHandle g_config_model;
Rml::DataModelHandle g_nav_help_model;
bool g_rom_valid = false;
std::string g_rom_status = "Select your Legacy of Darkness (USA) ROM.";
std::string g_rom_file_name = "None selected";
std::string g_version_string;
ultramodern::renderer::GraphicsConfig g_pending_graphics{};
lod::settings::AudioConfig g_audio_config{};
std::string g_config_status = "Graphics changes are staged until Apply.";
std::string g_audio_status = "Audio changes apply immediately and save to audio.json.";
std::string g_config_path_display;

std::u8string lod_game_id() {
    return u8"castlevania2.n64.us";
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

void initialise_default_menu_bindings() {
    static bool initialised = false;
    if (initialised) {
        return;
    }
    initialised = true;

    g_controller_bindings[static_cast<size_t>(recomp::GameInput::ACCEPT_MENU)][0] = {
        kInputControllerDigital,
        SDL_CONTROLLER_BUTTON_A,
    };
    g_controller_bindings[static_cast<size_t>(recomp::GameInput::APPLY_MENU)][0] = {
        kInputControllerDigital,
        SDL_CONTROLLER_BUTTON_X,
    };
    g_controller_bindings[static_cast<size_t>(recomp::GameInput::TOGGLE_MENU)][0] = {
        kInputControllerDigital,
        SDL_CONTROLLER_BUTTON_BACK,
    };
    g_controller_bindings[static_cast<size_t>(recomp::GameInput::TOGGLE_MENU)][1] = {
        kInputControllerDigital,
        SDL_CONTROLLER_BUTTON_START,
    };
}

std::string controller_button_prompt(uint32_t input_id) {
    switch (input_id) {
        case SDL_CONTROLLER_BUTTON_A: return PF_XBOX_A;
        case SDL_CONTROLLER_BUTTON_B: return PF_XBOX_B;
        case SDL_CONTROLLER_BUTTON_X: return PF_XBOX_X;
        case SDL_CONTROLLER_BUTTON_Y: return PF_XBOX_Y;
        case SDL_CONTROLLER_BUTTON_BACK: return PF_XBOX_VIEW;
        case SDL_CONTROLLER_BUTTON_START: return PF_XBOX_MENU;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            return PF_DPAD;
        default:
            return "Button " + std::to_string(input_id);
    }
}

std::string controller_binding_prompt(recomp::GameInput input) {
    initialise_default_menu_bindings();
    std::string result;
    for (size_t binding_index = 0; binding_index < recomp::bindings_per_input; binding_index++) {
        const recomp::InputField& binding =
            recomp::get_input_binding(input, binding_index, recomp::InputDevice::Controller);
        if (binding.input_type == kInputControllerDigital) {
            result += controller_button_prompt(binding.input_id);
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

void dirty_config() {
    if (!g_config_model) {
        return;
    }
    g_config_model.DirtyAllVariables();
}

void reset_pending_graphics_from_active() {
    g_pending_graphics = ultramodern::renderer::get_graphics_config();
    g_audio_config = lod::settings::get_audio_config();
    g_config_path_display = lod::settings::config_path().empty()
        ? std::string{"Registered at startup"}
        : lod::settings::config_path().string();
    g_config_status = "Graphics changes are staged until Apply.";
    g_audio_status = "Audio changes apply immediately and save to audio.json.";
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
            g_pending_graphics.ar_option = next_enum_option(g_pending_graphics.ar_option);
            break;
        case Rml::Input::KeyIdentifier::KI_F7:
            g_pending_graphics.msaa_option = next_enum_option(g_pending_graphics.msaa_option);
            break;
        case Rml::Input::KeyIdentifier::KI_F8:
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
        recompui::register_event(listener, "apply_graphics", [](const std::string&, Rml::Event&) {
            apply_pending_graphics("Zelda menu apply");
        });
        recompui::register_event(listener, "discard_graphics", [](const std::string&, Rml::Event&) {
            discard_pending_graphics();
        });
        recompui::register_event(listener, "reset_graphics", [](const std::string&, Rml::Event&) {
            reset_pending_graphics_defaults();
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
            out = g_pending_graphics.rr_option == ultramodern::renderer::RefreshRate::Manual;
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
    if (input_type == kInputNone) {
        return "";
    }
    if (input_type == kInputControllerDigital) {
        switch (input_id) {
            case SDL_CONTROLLER_BUTTON_A: return "A";
            case SDL_CONTROLLER_BUTTON_B: return "B";
            case SDL_CONTROLLER_BUTTON_X: return "X";
            case SDL_CONTROLLER_BUTTON_Y: return "Y";
            case SDL_CONTROLLER_BUTTON_BACK: return "Back";
            case SDL_CONTROLLER_BUTTON_START: return "Start";
            default: return "Button " + std::to_string(input_id);
        }
    }
    return std::to_string(input_id);
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

void recomp::start_scanning_input(InputDevice) {}
void recomp::stop_scanning_input() {}
void recomp::finish_scanning_input(InputField) {}
void recomp::cancel_scanning_input() {}
void recomp::config_menu_set_cont_or_kb(bool) {
    dirty_nav_help();
    if (g_config_model) {
        g_config_model.DirtyVariable("gfx_help__apply");
    }
}
recomp::InputField recomp::get_scanned_input() { return {}; }
int recomp::get_scanned_input_index() { return -1; }
recomp::BackgroundInputMode recomp::get_background_input_mode() { return g_background_input_mode; }
void recomp::set_background_input_mode(BackgroundInputMode mode) { g_background_input_mode = mode; }
bool recomp::game_input_disabled() { return recompui::is_context_capturing_input(); }
bool recomp::all_input_disabled() { return false; }

std::filesystem::path zelda64::get_program_path() {
#if defined(__APPLE__)
    return lod::get_bundle_resource_directory();
#else
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

    return std::filesystem::current_path() / "assets" / asset;
}

void zelda64::open_file_dialog(std::function<void(bool success, const std::filesystem::path& path)> callback) {
    if (NFD_Init() != NFD_OKAY) {
        std::fprintf(stderr, "[UI] NFD_Init failed: %s\n", NFD_GetError());
        callback(false, {});
        return;
    }

    nfdchar_t* out_path = nullptr;
    nfdfilteritem_t filters[] = {{"Nintendo 64 ROM", "z64,n64,v64"}};
    nfdresult_t result = NFD_OpenDialog(&out_path, filters, 1, nullptr);
    if (result == NFD_OKAY) {
        std::filesystem::path selected(out_path);
        NFD_FreePath(out_path);
        NFD_Quit();
        callback(true, selected);
    } else {
        if (result == NFD_ERROR) {
            std::fprintf(stderr, "[UI] Native file dialog failed: %s\n", NFD_GetError());
        }
        NFD_Quit();
        callback(false, {});
    }
}

void zelda64::open_file_dialog_multiple(std::function<void(bool success, const std::list<std::filesystem::path>& paths)> callback) {
    callback(false, {});
}

void zelda64::show_error_message_box(const char* title, const char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
}

void zelda64::save_config() {}
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
