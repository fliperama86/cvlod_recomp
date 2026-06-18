#pragma once

#include <filesystem>

#include "ultramodern/config.hpp"

namespace lod::settings {

std::filesystem::path config_path();
std::filesystem::path graphics_config_path();
void apply_and_save_graphics_config(const ultramodern::renderer::GraphicsConfig& config,
                                    const char* reason);

}
