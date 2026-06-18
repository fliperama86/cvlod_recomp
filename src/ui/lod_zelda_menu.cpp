#include "recomp_ui.h"
#include "recomp_input.h"
#include "zelda_config.h"
#include "zelda_support.h"

#include <array>
#include <cstdio>
#include <filesystem>
#include <list>
#include <memory>
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
#include "ultramodern/ultramodern.hpp"
#include "nfd.h"

namespace {
constexpr uint32_t kInputNone = 0;
constexpr uint32_t kInputControllerDigital = 3;

using input_mapping = std::array<recomp::InputField, recomp::bindings_per_input>;
std::array<input_mapping, static_cast<size_t>(recomp::GameInput::COUNT)> g_controller_bindings{};
std::array<input_mapping, static_cast<size_t>(recomp::GameInput::COUNT)> g_keyboard_bindings{};
recomp::BackgroundInputMode g_background_input_mode = recomp::BackgroundInputMode::On;
bool g_debug_enabled = false;

recompui::ContextId g_launcher_context = recompui::ContextId::null();
recompui::ContextId g_config_context = recompui::ContextId::null();
recompui::ConfigTab g_active_tab = recompui::ConfigTab::General;
Rml::DataModelHandle g_launcher_model;
Rml::DataModelHandle g_config_model;
bool g_rom_valid = false;
std::string g_rom_status = "Select your Legacy of Darkness (USA) ROM.";
std::string g_version_string;

std::u8string lod_game_id() {
    return u8"castlevania2.n64.us";
}

void dirty_launcher() {
    if (!g_launcher_model) {
        return;
    }
    g_launcher_model.DirtyVariable("rom_valid");
    g_launcher_model.DirtyVariable("rom_status");
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

void select_rom() {
    zelda64::open_file_dialog([](bool success, const std::filesystem::path& path) {
        if (!success) {
            g_rom_status = "ROM selection cancelled.";
            dirty_launcher();
            return;
        }

        g_rom_status = "Validating " + path.filename().string() + "...";
        dirty_launcher();

        std::u8string game_id = lod_game_id();
        recomp::RomValidationError result = recomp::select_rom(path, game_id);
        g_rom_valid = (result == recomp::RomValidationError::Good);
        if (g_rom_valid) {
            g_rom_status = "ROM ready: " + path.filename().string();
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
            recompui::set_config_tab(recompui::ConfigTab::General);
            recompui::hide_all_contexts();
            recompui::show_context(recompui::get_config_context_id(), "");
        });
        recompui::register_event(listener, "open_controls", [](const std::string&, Rml::Event&) {
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
            ultramodern::quit();
        });
    }

    void make_bindings(Rml::Context* context) override {
        initialise_default_menu_bindings();
        std::u8string game_id = lod_game_id();
        g_rom_valid = recomp::is_rom_valid(game_id);
        if (g_rom_valid) {
            g_rom_status = "ROM ready.";
        }

        Rml::DataModelConstructor constructor = context->CreateDataModel("launcher_model");
        constructor.Bind("rom_valid", &g_rom_valid);
        constructor.Bind("rom_status", &g_rom_status);
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
        recompui::register_event(listener, "close_config_menu", [](const std::string&, Rml::Event&) {
            recompui::hide_context(recompui::get_config_context_id());
            if (!ultramodern::is_game_started()) {
                recompui::show_context(recompui::get_launcher_context_id(), "");
            }
        });
    }

    void make_bindings(Rml::Context* context) override {
        Rml::DataModelConstructor constructor = context->CreateDataModel("config_model");
        g_config_model = constructor.GetModelHandle();
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
void recomp::config_menu_set_cont_or_kb(bool) {}
recomp::InputField recomp::get_scanned_input() { return {}; }
int recomp::get_scanned_input_index() { return -1; }
recomp::BackgroundInputMode recomp::get_background_input_mode() { return g_background_input_mode; }
void recomp::set_background_input_mode(BackgroundInputMode mode) { g_background_input_mode = mode; }
bool recomp::game_input_disabled() { return recompui::is_context_capturing_input(); }
bool recomp::all_input_disabled() { return false; }

std::filesystem::path zelda64::get_program_path() {
    return std::filesystem::current_path();
}

std::filesystem::path zelda64::get_asset_path(const char* asset) {
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
void zelda64::open_quit_game_prompt() { ultramodern::quit(); }

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
    g_active_tab = tab;
}

int recompui::config_tab_to_index(ConfigTab tab) {
    return static_cast<int>(tab);
}

Rml::ElementTabSet* recompui::get_config_tabset() {
    return nullptr;
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
