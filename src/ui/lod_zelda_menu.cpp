#include "recomp_ui.h"
#include "recomp_input.h"
#include "zelda_config.h"
#include "zelda_support.h"

#include <algorithm>
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
#include "lod/lod_support.h"
#include "lod/lod_settings.hpp"
#include "ultramodern/config.hpp"
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
ultramodern::renderer::GraphicsConfig g_pending_graphics{};
std::string g_config_status = "Graphics changes are staged until Apply.";
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
    g_config_path_display = lod::settings::config_path().empty()
        ? std::string{"Registered at startup"}
        : lod::settings::config_path().string();
    g_config_status = "Graphics changes are staged until Apply.";
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

void select_rom() {
    std::fprintf(stderr, "[UI] launcher event: select_rom\n");
    zelda64::open_file_dialog([](bool success, const std::filesystem::path& path) {
        if (!success) {
            std::fprintf(stderr, "[UI] ROM selection cancelled\n");
            g_rom_status = "ROM selection cancelled.";
            dirty_launcher();
            return;
        }

        g_rom_status = "Validating " + path.filename().string() + "...";
        dirty_launcher();

        std::u8string game_id = lod_game_id();
        std::fprintf(stderr, "[UI] Validating selected ROM: %s\n", path.string().c_str());
        recomp::RomValidationError result = recomp::select_rom(path, game_id);
        g_rom_valid = (result == recomp::RomValidationError::Good);
        if (g_rom_valid) {
            std::fprintf(stderr, "[UI] Selected ROM validated\n");
            g_rom_status = "ROM ready: " + path.filename().string();
        } else {
            std::fprintf(stderr, "[UI] Selected ROM validation failed (%d)\n", static_cast<int>(result));
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
            std::fprintf(stderr, "[UI] launcher event: start_game\n");
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
            std::fprintf(stderr, "[UI] launcher event: open_settings\n");
            reset_pending_graphics_from_active();
            recompui::set_config_tab(recompui::ConfigTab::General);
            recompui::hide_all_contexts();
            recompui::show_context(recompui::get_config_context_id(), "");
        });
        recompui::register_event(listener, "open_controls", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] launcher event: open_controls\n");
            reset_pending_graphics_from_active();
            recompui::set_config_tab(recompui::ConfigTab::Controls);
            recompui::hide_all_contexts();
            recompui::show_context(recompui::get_config_context_id(), "");
        });
        recompui::register_event(listener, "open_mods", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] launcher event: open_mods\n");
            recompui::set_config_tab(recompui::ConfigTab::Mods);
            recompui::hide_all_contexts();
            recompui::show_context(recompui::get_config_context_id(), "");
        });
        recompui::register_event(listener, "exit_game", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] launcher event: exit_game\n");
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
            std::fprintf(stderr, "[UI] config event: close_config_menu\n");
            close_config_menu();
        });
        recompui::register_event(listener, "apply_graphics", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] config event: apply_graphics\n");
            apply_pending_graphics("Zelda menu apply");
        });
        recompui::register_event(listener, "discard_graphics", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] config event: discard_graphics\n");
            discard_pending_graphics();
        });
        recompui::register_event(listener, "show_general_tab", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] config event: show_general_tab\n");
            recompui::set_config_tab(recompui::ConfigTab::General);
        });
        recompui::register_event(listener, "show_graphics_tab", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] config event: show_graphics_tab\n");
            recompui::set_config_tab(recompui::ConfigTab::Graphics);
        });
        recompui::register_event(listener, "show_controls_tab", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] config event: show_controls_tab\n");
            recompui::set_config_tab(recompui::ConfigTab::Controls);
        });
        recompui::register_event(listener, "show_audio_tab", [](const std::string&, Rml::Event&) {
            std::fprintf(stderr, "[UI] config event: show_audio_tab\n");
            recompui::set_config_tab(recompui::ConfigTab::Sound);
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
            } else if (key == Rml::Input::KeyIdentifier::KI_F) {
                apply_pending_graphics("Zelda menu key apply");
            }
        });
    }

    void make_bindings(Rml::Context* context) override {
        reset_pending_graphics_from_active();
        Rml::DataModelConstructor constructor = context->CreateDataModel("config_model");
        constructor.BindFunc("active_tab", [](Rml::Variant& out) {
            out = config_tab_name(g_active_tab);
        });
        constructor.BindFunc("options_changed", [](Rml::Variant& out) {
            out = graphics_changed();
        });
        constructor.Bind("config_status", &g_config_status);
        constructor.Bind("config_path", &g_config_path_display);
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
        constructor.BindFunc("rr_manual_value",
            [](Rml::Variant& out) {
                out = g_pending_graphics.rr_manual_value;
            },
            [](const Rml::Variant& in) {
                g_pending_graphics.rr_manual_value = std::clamp(in.Get<int>(), 20, 360);
                g_config_status = "Graphics changes pending.";
                dirty_config();
            });
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
    bool success = false;
    std::filesystem::path selected;

    auto run_dialog = [&]() {
        if (NFD_Init() != NFD_OKAY) {
            std::fprintf(stderr, "[UI] NFD_Init failed: %s\n", NFD_GetError());
            return;
        }

        nfdchar_t* out_path = nullptr;
        nfdfilteritem_t filters[] = {{"Nintendo 64 ROM", "z64,n64,v64"}};
        nfdresult_t result = NFD_OpenDialog(&out_path, filters, 1, nullptr);
        if (result == NFD_OKAY) {
            selected = std::filesystem::path(out_path);
            success = true;
            NFD_FreePath(out_path);
        } else if (result == NFD_ERROR) {
            std::fprintf(stderr, "[UI] Native file dialog failed: %s\n", NFD_GetError());
        }

        NFD_Quit();
    };

#ifdef __APPLE__
    lod::run_on_ui_thread_sync(run_dialog);
#else
    run_dialog();
#endif

    callback(success, selected);
}

void zelda64::open_file_dialog_multiple(std::function<void(bool success, const std::list<std::filesystem::path>& paths)> callback) {
    callback(false, {});
}

void zelda64::show_error_message_box(const char* title, const char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
}

#ifdef __APPLE__
void zelda64::dispatch_on_ui_thread(std::function<void()> func) {
    lod::dispatch_on_ui_thread(std::move(func));
}

std::optional<std::filesystem::path> zelda64::get_application_support_directory() {
    return lod::get_application_support_directory();
}

std::filesystem::path zelda64::get_bundle_resource_directory() {
    return lod::get_bundle_resource_directory();
}

std::filesystem::path zelda64::get_bundle_directory() {
    return lod::get_bundle_directory();
}
#endif

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
    if (!recompui::is_context_shown(g_config_context)) {
        reset_pending_graphics_from_active();
    }
    g_active_tab = tab;
    dirty_config();
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
