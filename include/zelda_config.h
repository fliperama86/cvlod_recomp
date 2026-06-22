#pragma once

#include "recomp_input.h"

#include <string_view>

namespace zelda64 {
    constexpr std::u8string_view program_id = u8"LodRecomp";
    constexpr std::string_view program_name = "Castlevania: Legacy of Darkness Recompiled";

    void save_config();
    void reset_input_bindings();
    void reset_cont_input_bindings();
    void reset_kb_input_bindings();
    void reset_single_input_binding(recomp::InputDevice device, recomp::GameInput input);
    bool get_debug_mode_enabled();
    void set_debug_mode_enabled(bool enabled);
    void open_quit_game_prompt();
}
