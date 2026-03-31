#include <cstdio>
#include <cassert>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <array>
#include <numeric>
#include <stdexcept>
#include <cinttypes>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>

#include "ultramodern/ultra64.h"
#include "ultramodern/ultramodern.hpp"
#define SDL_MAIN_HANDLED
#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#undef None
#undef Status
#undef LockMask
#undef ControlMask
#undef Success
#undef Always
#endif

#include "lod/lod_render.h"
#include "lod/lod_support.h"
#include "lod/target_rom.hpp"
#include "librecomp/game.hpp"
#include "librecomp/overlays.hpp"
#include "librecomp/rsp.hpp"

template<typename... Ts>
void exit_error(const char* str, Ts ...args) {
    ((void)fprintf(stderr, str, args), ...);
    assert(false);
    ultramodern::error_handling::quick_exit(__FILE__, __LINE__, __FUNCTION__);
}

// ── Graphics setup ──────────────────────────────────────────────────

ultramodern::gfx_callbacks_t::gfx_data_t create_gfx() {
    SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) > 0) {
        exit_error("Failed to initialize SDL2: %s\n", SDL_GetError());
    }

    fprintf(stdout, "SDL Video Driver: %s\n", SDL_GetCurrentVideoDriver());
    return {};
}

static SDL_Window* window = nullptr;

ultramodern::renderer::WindowHandle create_window(ultramodern::gfx_callbacks_t::gfx_data_t) {
    uint32_t flags = SDL_WINDOW_RESIZABLE;

#if defined(__APPLE__)
    flags |= SDL_WINDOW_METAL;
#elif defined(RT64_SDL_WINDOW_VULKAN)
    flags |= SDL_WINDOW_VULKAN;
#endif

    window = SDL_CreateWindow("Castlevania: Legacy of Darkness Recompiled",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 960, flags);

    if (window == nullptr) {
        exit_error("Failed to create window: %s\n", SDL_GetError());
    }

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

#if defined(_WIN32)
    return ultramodern::renderer::WindowHandle{ wmInfo.info.win.window, GetCurrentThreadId() };
#elif defined(__linux__) || defined(__ANDROID__)
    return ultramodern::renderer::WindowHandle{ window };
#elif defined(__APPLE__)
    SDL_MetalView view = SDL_Metal_CreateView(window);
    return ultramodern::renderer::WindowHandle{ wmInfo.info.cocoa.window, SDL_Metal_GetLayer(view) };
#else
    static_assert(false && "Unsupported platform");
#endif
}

void update_gfx(void*) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                ultramodern::quit();
                return;
        }
    }
}

// ── Audio ───────────────────────────────────────────────────────────

static SDL_AudioDeviceID audio_device = 0;
static SDL_AudioCVT audio_convert;
static uint32_t sample_rate = 48000;
static uint32_t output_sample_rate = 48000;
constexpr uint32_t input_channels = 2;
static uint32_t output_channels = 2;
constexpr uint32_t duplicated_input_frames = 4;
static uint32_t discarded_output_frames;
constexpr uint32_t bytes_per_frame = input_channels * sizeof(float);

void update_audio_converter() {
    SDL_BuildAudioCVT(&audio_convert, AUDIO_F32, input_channels, sample_rate,
                       AUDIO_F32, output_channels, output_sample_rate);
    discarded_output_frames = duplicated_input_frames * output_sample_rate / sample_rate;
}

void queue_samples(int16_t* audio_data, size_t sample_count) {
    static std::vector<float> swap_buffer;
    static std::array<float, duplicated_input_frames * input_channels> duplicated_sample_buffer;

    size_t resampled_sample_count = sample_count + duplicated_input_frames * input_channels;
    size_t max_sample_count = std::max(resampled_sample_count, resampled_sample_count * audio_convert.len_mult);
    if (max_sample_count > swap_buffer.size()) {
        swap_buffer.resize(max_sample_count);
    }

    for (size_t i = 0; i < duplicated_input_frames * input_channels; i++) {
        swap_buffer[i] = duplicated_sample_buffer[i];
    }

    for (size_t i = 0; i < sample_count; i += input_channels) {
        swap_buffer[i + 0 + duplicated_input_frames * input_channels] = audio_data[i + 1] * (0.5f / 32768.0f);
        swap_buffer[i + 1 + duplicated_input_frames * input_channels] = audio_data[i + 0] * (0.5f / 32768.0f);
    }

    assert(sample_count > duplicated_input_frames * input_channels);

    for (size_t i = 0; i < duplicated_input_frames * input_channels; i++) {
        duplicated_sample_buffer[i] = swap_buffer[i + sample_count];
    }

    audio_convert.buf = reinterpret_cast<Uint8*>(swap_buffer.data());
    audio_convert.len = (sample_count + duplicated_input_frames * input_channels) * sizeof(swap_buffer[0]);
    SDL_ConvertAudio(&audio_convert);

    uint32_t num_bytes_to_queue = audio_convert.len_cvt - output_channels * discarded_output_frames * sizeof(swap_buffer[0]);
    float* samples_to_queue = swap_buffer.data() + output_channels * discarded_output_frames / 2;

    SDL_QueueAudio(audio_device, samples_to_queue, num_bytes_to_queue);
}

size_t get_frames_remaining() {
    constexpr float buffer_offset_frames = 1.0f;
    uint64_t buffered_byte_count = SDL_GetQueuedAudioSize(audio_device);
    buffered_byte_count = buffered_byte_count * 2 * sample_rate / output_sample_rate / output_channels;

    uint32_t frames_per_vi = (sample_rate / 60);
    if (buffered_byte_count > (buffer_offset_frames * bytes_per_frame * frames_per_vi)) {
        buffered_byte_count -= (buffer_offset_frames * bytes_per_frame * frames_per_vi);
    } else {
        buffered_byte_count = 0;
    }
    return static_cast<uint32_t>(buffered_byte_count / bytes_per_frame);
}

void set_frequency(uint32_t freq) {
    sample_rate = freq;
    update_audio_converter();
}

void reset_audio(uint32_t output_freq) {
    SDL_AudioSpec spec_desired{
        .freq = (int)output_freq,
        .format = AUDIO_F32,
        .channels = (Uint8)output_channels,
        .silence = 0,
        .samples = 0x100,
        .padding = 0,
        .size = 0,
        .callback = nullptr,
        .userdata = nullptr
    };

    audio_device = SDL_OpenAudioDevice(nullptr, false, &spec_desired, nullptr, 0);
    if (audio_device == 0) {
        fprintf(stderr, "[WARN] SDL error opening audio device: %s (continuing without audio)\n", SDL_GetError());
    } else {
        SDL_PauseAudioDevice(audio_device, 0);
    }

    output_sample_rate = output_freq;
    update_audio_converter();
}

// ── RSP ─────────────────────────────────────────────────────────────

extern RspUcodeFunc aspMain;

RspUcodeFunc* get_rsp_microcode(const OSTask* task) {
    switch (task->t.type) {
    case M_AUDTASK:
        return aspMain;
    default:
        fprintf(stderr, "Unknown RSP task type: %" PRIu32 "\n", task->t.type);
        return nullptr;
    }
}

// ── Input (minimal stubs) ───────────────────────────────────────────

void poll_inputs() {
    // SDL event polling happens in update_gfx
}

bool get_n64_input(int controller_num, uint16_t* buttons, float* x, float* y) {
    {
        static int input_calls = 0; input_calls++;
        if (input_calls <= 3 || (input_calls % 1000 == 0))
            fprintf(stderr, "[INPUT] get_n64_input called #%d, controller=%d\n", input_calls, controller_num);
    }
    if (controller_num != 0) return false;

    *buttons = 0;
    *x = 0.0f;
    *y = 0.0f;

    const uint8_t* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_RETURN])  *buttons |= 0x8000; // A
    if (keys[SDL_SCANCODE_RSHIFT])  *buttons |= 0x4000; // B
    if (keys[SDL_SCANCODE_Z])       *buttons |= 0x2000; // Z
    if (keys[SDL_SCANCODE_SPACE])   *buttons |= 0x1000; // Start
    if (keys[SDL_SCANCODE_UP])      *buttons |= 0x0800; // D-Up
    if (keys[SDL_SCANCODE_DOWN])    *buttons |= 0x0400; // D-Down
    if (keys[SDL_SCANCODE_LEFT])    *buttons |= 0x0200; // D-Left
    if (keys[SDL_SCANCODE_RIGHT])   *buttons |= 0x0100; // D-Right

    // Analog stick via WASD
    if (keys[SDL_SCANCODE_W]) *y += 1.0f;
    if (keys[SDL_SCANCODE_S]) *y -= 1.0f;
    if (keys[SDL_SCANCODE_A]) *x -= 1.0f;
    if (keys[SDL_SCANCODE_D]) *x += 1.0f;


    return true;
}

void set_rumble(int, bool) {}

ultramodern::input::connected_device_info_t get_connected_device_info(int controller_num) {
    if (controller_num == 0) {
        return { ultramodern::input::Device::Controller, ultramodern::input::Pak::None };
    }
    return { ultramodern::input::Device::None, ultramodern::input::Pak::None };
}

// ── Game entry ──────────────────────────────────────────────────────

extern "C" void recomp_entrypoint(uint8_t* rdram, recomp_context* ctx);
gpr get_entrypoint_address();
extern void lod_register_overlays();
extern void lod_on_init(uint8_t* rdram, recomp_context* ctx);

std::vector<recomp::GameEntry> supported_games = {
    {
        .rom_hash = lod::target_rom::kXxh3_64,
        .internal_name = std::string(lod::target_rom::kInternalName),
        .game_id = u8"castlevania2.n64.us",
        .mod_game_id = "",  // Disabled — mod system not yet set up for LoD
        .save_type = recomp::SaveType::Sram, // 32KB — used for virtual Controller Pak
        .is_enabled = false,
        .entrypoint_address = get_entrypoint_address(),
        .entrypoint = recomp_entrypoint,
        .on_init_callback = lod_on_init,
    },
};

std::string get_game_thread_name(const OSThread* t) {
    std::string name = "[Game] ";
    switch (t->id) {
        case 1: name += "IDLE"; break;
        case 3: name += "MAIN"; break;
        case 4: name += "GRAPH"; break;
        case 5: name += "SCHED"; break;
        case 7: name += "PADMGR"; break;
        case 10: name += "AUDIOMGR"; break;
        default: name += std::to_string(t->id); break;
    }
    return name;
}

void message_box_stub(const char* msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
    // Don't show modal dialog — it blocks startup and the game window
    // may not be visible yet. Just log to stderr.
}

// ── ROM loading ─────────────────────────────────────────────────────

static std::filesystem::path find_rom_path(int argc, char** argv) {
    // Check command line argument first
    if (argc > 1) {
        return std::filesystem::path(argv[1]);
    }

    // Look for ROM in current directory
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        auto ext = entry.path().extension().string();
        if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
            auto name = entry.path().stem().string();
            // Match common names for Castlevania LoD
            if (name.find("castlevania") != std::string::npos ||
                name.find("legacy") != std::string::npos ||
                name.find("Castlevania") != std::string::npos ||
                name.find("Legacy") != std::string::npos ||
                name.find("ND4E") != std::string::npos) {
                return entry.path();
            }
        }
    }

    // Fall back to any .z64 file
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        auto ext = entry.path().extension().string();
        if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
            return entry.path();
        }
    }

    // Also search in resources/ subdirectory
    auto search_dir = [](const std::filesystem::path& dir) -> std::filesystem::path {
        // Prefer castlevania-named ROMs
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            auto ext = entry.path().extension().string();
            if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
                auto name = entry.path().stem().string();
                if (name.find("castlevania") != std::string::npos ||
                    name.find("Castlevania") != std::string::npos ||
                    name.find("legacy") != std::string::npos ||
                    name.find("ND4E") != std::string::npos) {
                    return entry.path();
                }
            }
        }
        // Fall back to any ROM
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            auto ext = entry.path().extension().string();
            if (ext == ".z64" || ext == ".n64" || ext == ".v64") {
                return entry.path();
            }
        }
        return {};
    };

    if (std::filesystem::exists("resources")) {
        auto p = search_dir("resources");
        if (!p.empty()) return p;
    }

    return {};
}

static void auto_start_game(const std::filesystem::path& rom_path) {
    // Give the runtime a moment to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::u8string game_id = u8"castlevania2.n64.us";

    // Try the decompressed ROM first (NI files pre-decompressed for direct DMA)
    std::filesystem::path decomp_rom = "resources/castlevania2_decompressed.z64";
    bool using_decomp = false;
    if (std::filesystem::exists(decomp_rom) && std::filesystem::file_size(decomp_rom) > 0) {
        auto result = recomp::select_rom(decomp_rom, game_id);
        if (result == recomp::RomValidationError::Good) {
            fprintf(stderr, "[LodRecomp] Using decompressed ROM\n");
            using_decomp = true;
        } else {
            // Force-load the decompressed ROM even if hash doesn't match
            std::ifstream decomp_file(decomp_rom, std::ios::binary);
            if (decomp_file) {
                std::vector<uint8_t> decomp_data((std::istreambuf_iterator<char>(decomp_file)),
                                                  std::istreambuf_iterator<char>());
                recomp::set_rom_contents(std::move(decomp_data));
                fprintf(stderr, "[LodRecomp] Force-loaded decompressed ROM (%zu bytes)\n", recomp::get_rom().size());
                using_decomp = true;
            }
        }
    }
    if (!using_decomp) {
        auto result = recomp::select_rom(rom_path, game_id);
        if (result != recomp::RomValidationError::Good) {
            fprintf(stderr, "ROM validation failed (error %d) for: %s\n", (int)result, rom_path.string().c_str());
            if (!recomp::load_stored_rom(game_id)) {
                fprintf(stderr, "No stored ROM found either, cannot start game.\n");
                return;
            }
            fprintf(stderr, "[LodRecomp] Using previously stored ROM\n");
        }
    }

    fprintf(stderr, "[LodRecomp] ROM loaded successfully, starting game...\n");
    recomp::start_game(game_id);
}

// ── Signal handling ─────────────────────────────────────────────────

// Declared in overlays.cpp
extern uint32_t trace_ring[];
extern int trace_ring_pos;
extern int trace_total;

static void crash_handler(int sig, siginfo_t* info, void* ctx) {
    fprintf(stderr, "\n[CRASH] Signal %d at address %p (code=%d)\n",
            sig, info->si_addr, info->si_code);
    // Print last function calls from trace ring
    int count = trace_total < 32 ? trace_total : 32;
    if (count > 0) {
        fprintf(stderr, "  Last %d function lookups:\n", count);
        for (int i = 0; i < count; i++) {
            int idx = (trace_ring_pos - count + i + 32) % 32;
            fprintf(stderr, "    [-%d] 0x%08X\n", count - i, trace_ring[idx]);
        }
    }
    _exit(128 + sig);
}

static void signal_handler(int sig) {
    fprintf(stderr, "\n[LodRecomp] Caught signal %d, shutting down...\n", sig);
    ultramodern::quit();
    // Give threads a moment to clean up, then force exit
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    _exit(0);
}

// ── Main ────────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    // Catch SIGSEGV/SIGBUS to get fault address
    struct sigaction sa = {};
    sa.sa_sigaction = crash_handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    recomp::Version project_version{};
    recomp::Version::from_string("0.1.0", project_version);

    // Set up config path for ROM storage
    std::filesystem::path config_path;
#ifdef __APPLE__
    auto app_support = lod::get_application_support_directory();
    if (app_support) {
        config_path = *app_support / "LodRecomp";
    } else {
        config_path = std::filesystem::path(getenv("HOME")) / ".lodrecomp";
    }
#else
    config_path = std::filesystem::path(getenv("HOME")) / ".lodrecomp";
#endif
    std::filesystem::create_directories(config_path);
    recomp::register_config_path(config_path);

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    reset_audio(48000);

    for (const auto& game : supported_games) {
        recomp::register_game(game);
    }

    lod_register_overlays();

    // Find the ROM
    std::filesystem::path rom_path = find_rom_path(argc, argv);
    if (rom_path.empty()) {
        fprintf(stderr, "No ROM file found. Pass a .z64 file as an argument or place one in the current directory.\n");
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[LodRecomp] Using ROM: %s\n", rom_path.string().c_str());

    // Launch auto-start thread (will select ROM and start game after runtime initializes)
    std::thread auto_start_thread(auto_start_game, rom_path);
    auto_start_thread.detach();

    recomp::rsp::callbacks_t rsp_callbacks{
        .get_rsp_microcode = get_rsp_microcode,
    };

    ultramodern::renderer::callbacks_t renderer_callbacks{
        .create_render_context = lod::renderer::create_render_context,
    };

    ultramodern::gfx_callbacks_t gfx_callbacks{
        .create_gfx = create_gfx,
        .create_window = create_window,
        .update_gfx = update_gfx,
    };

    ultramodern::audio_callbacks_t audio_callbacks{
        .queue_samples = queue_samples,
        .get_frames_remaining = get_frames_remaining,
        .set_frequency = set_frequency,
    };

    ultramodern::input::callbacks_t input_callbacks{
        .poll_input = poll_inputs,
        .get_input = get_n64_input,
        .set_rumble = set_rumble,
        .get_connected_device_info = get_connected_device_info,
    };

    ultramodern::events::callbacks_t events_callbacks{
        .vi_callback = nullptr,
        .gfx_init_callback = nullptr,
    };

    ultramodern::error_handling::callbacks_t error_handling_callbacks{
        .message_box = message_box_stub,
    };

    ultramodern::threads::callbacks_t threads_callbacks{
        .get_game_thread_name = get_game_thread_name,
    };

    recomp::Configuration config{
        .project_version = project_version,
        .window_handle = {},
        .rsp_callbacks = rsp_callbacks,
        .renderer_callbacks = renderer_callbacks,
        .audio_callbacks = audio_callbacks,
        .input_callbacks = input_callbacks,
        .gfx_callbacks = gfx_callbacks,
        .events_callbacks = events_callbacks,
        .error_handling_callbacks = error_handling_callbacks,
        .threads_callbacks = threads_callbacks,
        .message_queue_control = { .requeue_timer = false, .requeue_sp = true, .requeue_si = true, .requeue_dp = true },
    };

    fprintf(stderr, "[LodRecomp] Calling recomp::start()...\n");
    recomp::start(config);

    fprintf(stderr, "[LodRecomp] recomp::start() returned\n");
    return EXIT_SUCCESS;
}
