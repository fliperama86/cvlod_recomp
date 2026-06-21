#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace lod::settings {

struct AudioConfig {
    int master_volume = 100;
    bool mute = false;
};

struct AudioStatus {
    std::string sdl_driver;
    uint32_t input_sample_rate = 0;
    uint32_t output_sample_rate = 0;
    uint32_t output_channels = 0;
    uint32_t queued_bytes = 0;
    bool device_open = false;
};

std::filesystem::path audio_config_path();
AudioConfig default_audio_config();
AudioConfig get_audio_config();
void apply_and_save_audio_config(const AudioConfig& config, const char* reason);
AudioStatus audio_status();

} // namespace lod::settings
