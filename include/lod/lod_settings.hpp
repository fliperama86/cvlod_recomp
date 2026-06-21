#pragma once

#include <filesystem>

#include "lod/lod_audio_config.hpp"
#include "ultramodern/config.hpp"

namespace lod::settings {

std::filesystem::path config_path();
bool portable_mode_enabled();
std::filesystem::path graphics_config_path();
ultramodern::renderer::GraphicsConfig default_graphics_config();
void apply_and_save_graphics_config(const ultramodern::renderer::GraphicsConfig& config,
                                    const char* reason);
void persist_rom_path(const std::filesystem::path& rom_path);

}
