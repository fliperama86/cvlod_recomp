#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace recomp {
    enum class GameInput {
        TOGGLE_MENU,
        ACCEPT_MENU,
        APPLY_MENU,
        COUNT,
    };

    struct InputField {
        uint32_t input_type = 0;
        int32_t input_id = 0;
        std::string to_string() const;
        auto operator<=>(const InputField& rhs) const = default;
    };

    enum class InputDevice {
        Controller,
        Keyboard,
        COUNT,
    };

    enum class BackgroundInputMode {
        On,
        Off,
        OptionCount,
    };

    constexpr size_t bindings_per_input = 2;

    InputField& get_input_binding(GameInput input, size_t binding_index, InputDevice device);
    void set_input_binding(GameInput input, size_t binding_index, InputDevice device, InputField value);

    void start_scanning_input(InputDevice device);
    void stop_scanning_input();
    void finish_scanning_input(InputField scanned_field);
    void cancel_scanning_input();
    void config_menu_set_cont_or_kb(bool cont_interacted);
    InputField get_scanned_input();
    int get_scanned_input_index();

    BackgroundInputMode get_background_input_mode();
    void set_background_input_mode(BackgroundInputMode mode);

    bool game_input_disabled();
    bool all_input_disabled();
}
