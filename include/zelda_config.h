#pragma once

#include <string_view>

namespace zelda64 {
    constexpr std::u8string_view program_id = u8"LodRecomp";
    constexpr std::string_view program_name = "Castlevania: Legacy of Darkness Recompiled";

    void save_config();
    bool get_debug_mode_enabled();
    void set_debug_mode_enabled(bool enabled);
    void open_quit_game_prompt();
}
