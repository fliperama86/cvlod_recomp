#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace recomp {
    enum class GameInput {
        A,
        B,
        L,
        R,
        Z,
        START,
        C_LEFT,
        C_RIGHT,
        C_UP,
        C_DOWN,
        DPAD_LEFT,
        DPAD_RIGHT,
        DPAD_UP,
        DPAD_DOWN,
        X_AXIS_NEG,
        X_AXIS_POS,
        Y_AXIS_POS,
        Y_AXIS_NEG,
        TOGGLE_MENU,
        ACCEPT_MENU,
        APPLY_MENU,
        COUNT,
    };

    enum class InputType : uint32_t {
        None = 0,
        Keyboard,
        Mouse,
        ControllerDigital,
        ControllerAnalog,
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
    InputDevice get_scanning_input_device();

    size_t get_num_inputs();
    const std::string& get_input_name(GameInput input);
    const std::string& get_input_enum_name(GameInput input);
    GameInput get_input_from_enum_name(std::string_view name);

    BackgroundInputMode get_background_input_mode();
    void set_background_input_mode(BackgroundInputMode mode);

    bool game_input_disabled();
    bool all_input_disabled();
}
