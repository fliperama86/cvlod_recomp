#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <fstream>

#ifndef HLSL_CPU
#define HLSL_CPU
#endif
#include "hle/rt64_application.h"
#include "hle/rt64_workload.h"
#include "hle/rt64_vi.h"

#include "ultramodern/ultramodern.hpp"
#include "ultramodern/config.hpp"

#include "lod/lod_render.h"
#ifdef LOD_USE_ZELDA_MENU
#include "recomp_ui.h"
#else
#include "lod_ui_overlay.h"
#endif

#ifndef LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
#define LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS 0
#endif

#ifndef LOD_DISABLE_RT64_FORCE_BRANCH
#define LOD_DISABLE_RT64_FORCE_BRANCH 0
#endif

static uint8_t DMEM[0x1000];
static uint8_t IMEM[0x1000];

unsigned int MI_INTR_REG = 0;

unsigned int DPC_START_REG = 0;
unsigned int DPC_END_REG = 0;
unsigned int DPC_CURRENT_REG = 0;
unsigned int DPC_STATUS_REG = 0;
unsigned int DPC_CLOCK_REG = 0;
unsigned int DPC_BUFBUSY_REG = 0;
unsigned int DPC_PIPEBUSY_REG = 0;
unsigned int DPC_TMEM_REG = 0;

void dummy_check_interrupts() {}

RT64::UserConfiguration::AspectRatio to_rt64(ultramodern::renderer::AspectRatio option) {
    switch (option) {
        case ultramodern::renderer::AspectRatio::Original:
            return RT64::UserConfiguration::AspectRatio::Original;
        case ultramodern::renderer::AspectRatio::Expand:
            return RT64::UserConfiguration::AspectRatio::Expand;
        case ultramodern::renderer::AspectRatio::Manual:
            return RT64::UserConfiguration::AspectRatio::Manual;
        case ultramodern::renderer::AspectRatio::OptionCount:
            return RT64::UserConfiguration::AspectRatio::OptionCount;
    }
    return RT64::UserConfiguration::AspectRatio::Original;
}

RT64::UserConfiguration::Antialiasing to_rt64(ultramodern::renderer::Antialiasing option) {
    switch (option) {
        case ultramodern::renderer::Antialiasing::None:
            return RT64::UserConfiguration::Antialiasing::None;
        case ultramodern::renderer::Antialiasing::MSAA2X:
            return RT64::UserConfiguration::Antialiasing::MSAA2X;
        case ultramodern::renderer::Antialiasing::MSAA4X:
            return RT64::UserConfiguration::Antialiasing::MSAA4X;
        case ultramodern::renderer::Antialiasing::MSAA8X:
            return RT64::UserConfiguration::Antialiasing::MSAA8X;
        case ultramodern::renderer::Antialiasing::OptionCount:
            return RT64::UserConfiguration::Antialiasing::OptionCount;
    }
    return RT64::UserConfiguration::Antialiasing::None;
}

RT64::UserConfiguration::RefreshRate to_rt64(ultramodern::renderer::RefreshRate option) {
    switch (option) {
        case ultramodern::renderer::RefreshRate::Original:
            return RT64::UserConfiguration::RefreshRate::Original;
        case ultramodern::renderer::RefreshRate::Display:
            return RT64::UserConfiguration::RefreshRate::Display;
        case ultramodern::renderer::RefreshRate::Manual:
            return RT64::UserConfiguration::RefreshRate::Manual;
        case ultramodern::renderer::RefreshRate::OptionCount:
            return RT64::UserConfiguration::RefreshRate::OptionCount;
    }
    return RT64::UserConfiguration::RefreshRate::Original;
}

RT64::UserConfiguration::InternalColorFormat to_rt64(ultramodern::renderer::HighPrecisionFramebuffer option) {
    switch (option) {
        case ultramodern::renderer::HighPrecisionFramebuffer::Off:
            return RT64::UserConfiguration::InternalColorFormat::Standard;
        case ultramodern::renderer::HighPrecisionFramebuffer::On:
            return RT64::UserConfiguration::InternalColorFormat::High;
        case ultramodern::renderer::HighPrecisionFramebuffer::Auto:
            return RT64::UserConfiguration::InternalColorFormat::Automatic;
        case ultramodern::renderer::HighPrecisionFramebuffer::OptionCount:
            return RT64::UserConfiguration::InternalColorFormat::OptionCount;
    }
    return RT64::UserConfiguration::InternalColorFormat::Standard;
}

void set_application_user_config(RT64::Application* application, const ultramodern::renderer::GraphicsConfig& config) {
    switch (config.res_option) {
        default:
        case ultramodern::renderer::Resolution::Auto:
            application->userConfig.resolution = RT64::UserConfiguration::Resolution::WindowIntegerScale;
            application->userConfig.downsampleMultiplier = 1;
            break;
        case ultramodern::renderer::Resolution::Original:
            application->userConfig.resolution = RT64::UserConfiguration::Resolution::Manual;
            application->userConfig.resolutionMultiplier = std::max(config.ds_option, 1);
            application->userConfig.downsampleMultiplier = std::max(config.ds_option, 1);
            break;
        case ultramodern::renderer::Resolution::Original2x:
            application->userConfig.resolution = RT64::UserConfiguration::Resolution::Manual;
            application->userConfig.resolutionMultiplier = 2.0 * std::max(config.ds_option, 1);
            application->userConfig.downsampleMultiplier = std::max(config.ds_option, 1);
            break;
    }

    switch (config.hr_option) {
        default:
        case ultramodern::renderer::HUDRatioMode::Original:
            application->userConfig.extAspectRatio = RT64::UserConfiguration::AspectRatio::Original;
            break;
        case ultramodern::renderer::HUDRatioMode::Clamp16x9:
            application->userConfig.extAspectRatio = RT64::UserConfiguration::AspectRatio::Manual;
            application->userConfig.extAspectTarget = 16.0/9.0;
            break;
        case ultramodern::renderer::HUDRatioMode::Full:
            application->userConfig.extAspectRatio = RT64::UserConfiguration::AspectRatio::Expand;
            break;
    }

    application->userConfig.aspectRatio = to_rt64(config.ar_option);
    application->userConfig.antialiasing = to_rt64(config.msaa_option);
    application->userConfig.refreshRate = to_rt64(config.rr_option);
    application->userConfig.refreshRateTarget = config.rr_manual_value;
    application->userConfig.internalColorFormat = to_rt64(config.hpfb_option);
    application->userConfig.displayBuffering = RT64::UserConfiguration::DisplayBuffering::Triple;
}

ultramodern::renderer::SetupResult map_setup_result(RT64::Application::SetupResult rt64_result) {
    switch (rt64_result) {
        case RT64::Application::SetupResult::Success:
            return ultramodern::renderer::SetupResult::Success;
        case RT64::Application::SetupResult::DynamicLibrariesNotFound:
            return ultramodern::renderer::SetupResult::DynamicLibrariesNotFound;
        case RT64::Application::SetupResult::InvalidGraphicsAPI:
            return ultramodern::renderer::SetupResult::InvalidGraphicsAPI;
        case RT64::Application::SetupResult::GraphicsAPINotFound:
            return ultramodern::renderer::SetupResult::GraphicsAPINotFound;
        case RT64::Application::SetupResult::GraphicsDeviceNotFound:
            return ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
    }
    std::exit(EXIT_FAILURE);
}

ultramodern::renderer::GraphicsApi map_graphics_api(RT64::UserConfiguration::GraphicsAPI api) {
    switch (api) {
        case RT64::UserConfiguration::GraphicsAPI::D3D12:
            return ultramodern::renderer::GraphicsApi::D3D12;
        case RT64::UserConfiguration::GraphicsAPI::Vulkan:
            return ultramodern::renderer::GraphicsApi::Vulkan;
        case RT64::UserConfiguration::GraphicsAPI::Metal:
            return ultramodern::renderer::GraphicsApi::Metal;
        case RT64::UserConfiguration::GraphicsAPI::Automatic:
            return ultramodern::renderer::GraphicsApi::Auto;
    }
    std::exit(EXIT_FAILURE);
}

lod::renderer::RT64Context::RT64Context(uint8_t* rdram, ultramodern::renderer::WindowHandle window_handle, bool debug) {
    static unsigned char dummy_rom_header[0x40];
#ifdef LOD_USE_ZELDA_MENU
    recompui::set_render_hooks();
#else
    lod::ui::set_render_hooks();
#endif

    RT64::Application::Core appCore{};
#if defined(_WIN32)
    appCore.window = window_handle.window;
#elif defined(__linux__) || defined(__ANDROID__)
    appCore.window = window_handle;
#elif defined(__APPLE__)
    appCore.window.window = window_handle.window;
    appCore.window.view = window_handle.view;
#endif

    appCore.checkInterrupts = dummy_check_interrupts;

    appCore.HEADER = dummy_rom_header;
    appCore.RDRAM = rdram;
    appCore.DMEM = DMEM;
    appCore.IMEM = IMEM;

    appCore.MI_INTR_REG = &MI_INTR_REG;

    appCore.DPC_START_REG = &DPC_START_REG;
    appCore.DPC_END_REG = &DPC_END_REG;
    appCore.DPC_CURRENT_REG = &DPC_CURRENT_REG;
    appCore.DPC_STATUS_REG = &DPC_STATUS_REG;
    appCore.DPC_CLOCK_REG = &DPC_CLOCK_REG;
    appCore.DPC_BUFBUSY_REG = &DPC_BUFBUSY_REG;
    appCore.DPC_PIPEBUSY_REG = &DPC_PIPEBUSY_REG;
    appCore.DPC_TMEM_REG = &DPC_TMEM_REG;

    ultramodern::renderer::ViRegs* vi_regs = ultramodern::renderer::get_vi_regs();

    appCore.VI_STATUS_REG = &vi_regs->VI_STATUS_REG;
    appCore.VI_ORIGIN_REG = &vi_regs->VI_ORIGIN_REG;
    appCore.VI_WIDTH_REG = &vi_regs->VI_WIDTH_REG;
    appCore.VI_INTR_REG = &vi_regs->VI_INTR_REG;
    appCore.VI_V_CURRENT_LINE_REG = &vi_regs->VI_V_CURRENT_LINE_REG;
    appCore.VI_TIMING_REG = &vi_regs->VI_TIMING_REG;
    appCore.VI_V_SYNC_REG = &vi_regs->VI_V_SYNC_REG;
    appCore.VI_H_SYNC_REG = &vi_regs->VI_H_SYNC_REG;
    appCore.VI_LEAP_REG = &vi_regs->VI_LEAP_REG;
    appCore.VI_H_START_REG = &vi_regs->VI_H_START_REG;
    appCore.VI_V_START_REG = &vi_regs->VI_V_START_REG;
    appCore.VI_V_BURST_REG = &vi_regs->VI_V_BURST_REG;
    appCore.VI_X_SCALE_REG = &vi_regs->VI_X_SCALE_REG;
    appCore.VI_Y_SCALE_REG = &vi_regs->VI_Y_SCALE_REG;

    RT64::ApplicationConfiguration appConfig;
    appConfig.useConfigurationFile = false;

    app = std::make_unique<RT64::Application>(appCore, appConfig);

    auto& cur_config = ultramodern::renderer::get_graphics_config();
    set_application_user_config(app.get(), cur_config);
    app->userConfig.developerMode = debug;
    app->enhancementConfig.f3dex.forceBranch = !LOD_DISABLE_RT64_FORCE_BRANCH;
#if LOD_DISABLE_RT64_FORCE_BRANCH
    fprintf(stderr, "[RT64-CONFIG] f3dex.forceBranch disabled for LoD render A/B test\n");
#endif
    app->enhancementConfig.textureLOD.scale = true;

    switch (cur_config.api_option) {
        case ultramodern::renderer::GraphicsApi::D3D12:
            app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::D3D12;
            break;
        case ultramodern::renderer::GraphicsApi::Vulkan:
            app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Vulkan;
            break;
        case ultramodern::renderer::GraphicsApi::Metal:
            app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Metal;
            break;
        case ultramodern::renderer::GraphicsApi::Auto:
#ifdef _WIN32
            // Default Windows "Auto" to Vulkan. The settings overlay only ships
            // SPIRV shaders, so it can only initialize under Vulkan; RT64's
            // "Automatic" otherwise resolves to D3D12 on Windows and the overlay
            // never shows. D3D12 remains available by selecting it explicitly.
            app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Vulkan;
#else
            app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Automatic;
#endif
            break;
    }

    uint32_t thread_id = 0;
#ifdef _WIN32
    thread_id = window_handle.thread_id;
#endif
    setup_result = map_setup_result(app->setup(thread_id));
    chosen_api = map_graphics_api(app->chosenGraphicsAPI);
    if (setup_result != ultramodern::renderer::SetupResult::Success) {
        app = nullptr;
        return;
    }

    app->setFullScreen(cur_config.wm_option == ultramodern::renderer::WindowMode::Fullscreen);
}

lod::renderer::RT64Context::~RT64Context() = default;

static uint32_t last_displayed_cfb = 0x00200000;
static int g_dl_n = 0;

#ifndef LOD_ENABLE_CFB_SNAPSHOT
#define LOD_ENABLE_CFB_SNAPSHOT 0
#endif

#if LOD_ENABLE_CFB_SNAPSHOT
static int32_t lod_render_current_gamestate(uint8_t* rdram) {
    uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
    if (gsm_addr == 0) {
        return 0;
    }

    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (gsm_phys + 0x2C > 0x800000) {
        return 0;
    }

    return *(int32_t*)(rdram + gsm_phys + 0x24);
}

static uint16_t lod_rdram_be16(uint8_t* rdram, uint32_t phys) {
    return ((uint16_t)rdram[(phys + 0) ^ 3] << 8) |
           ((uint16_t)rdram[(phys + 1) ^ 3] << 0);
}

static void lod_dump_cfb_ppm(uint8_t* rdram, int32_t gs, int dl_n, int us_n, uint32_t cfb, uint32_t width) {
    if (width == 0) {
        width = 320;
    }

    uint32_t height = (width >= 640) ? 480 : 240;
    uint64_t bytes = (uint64_t)width * (uint64_t)height * 2ULL;
    if (cfb == 0 || cfb >= 0x800000 || bytes > 0x800000ULL || cfb + bytes > 0x800000ULL) {
        fprintf(stderr,
            "[CFB] skip snapshot gs=%d dl=%d us=%d cfb=0x%06X width=%u height=%u bytes=0x%llX\n",
            gs, dl_n, us_n, cfb, width, height, (unsigned long long)bytes);
        return;
    }

    char path[256];
    std::snprintf(path, sizeof(path), "/tmp/lod_cfb_gs%d_us%d_dl%d_cfb%06X.ppm", gs, us_n, dl_n, cfb);
    FILE* f = std::fopen(path, "wb");
    if (f == nullptr) {
        fprintf(stderr, "[CFB] failed to open %s\n", path);
        return;
    }

    std::fprintf(f, "P6\n%u %u\n255\n", width, height);

    uint64_t nonzero = 0;
    uint64_t sum_r = 0;
    uint64_t sum_g = 0;
    uint64_t sum_b = 0;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t phys = cfb + ((y * width + x) * 2);
            uint16_t px = lod_rdram_be16(rdram, phys);
            uint8_t r = (uint8_t)((((px >> 11) & 0x1F) * 255) / 31);
            uint8_t g = (uint8_t)((((px >> 6) & 0x1F) * 255) / 31);
            uint8_t b = (uint8_t)((((px >> 1) & 0x1F) * 255) / 31);
            uint8_t rgb[3] = { r, g, b };
            if (px != 0) {
                nonzero++;
            }
            sum_r += r;
            sum_g += g;
            sum_b += b;
            std::fwrite(rgb, 1, sizeof(rgb), f);
        }
    }

    std::fclose(f);

    uint64_t pixels = (uint64_t)width * (uint64_t)height;
    fprintf(stderr,
        "[CFB] wrote %s gs=%d dl=%d us=%d cfb=0x%06X %ux%u nonzero=%llu/%llu mean_rgb=(%llu,%llu,%llu)\n",
        path, gs, dl_n, us_n, cfb, width, height,
        (unsigned long long)nonzero, (unsigned long long)pixels,
        (unsigned long long)(sum_r / pixels),
        (unsigned long long)(sum_g / pixels),
        (unsigned long long)(sum_b / pixels));
}
#endif

void lod::renderer::RT64Context::send_dl(const OSTask* task) {
    uint32_t data_addr = task->t.data_ptr & 0x3FFFFFF;
    uint8_t* rdram = app->core.RDRAM;
    g_dl_n++;

#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
    // === Game state diagnostic (read-only) ===
    {
        uint32_t exec_flags = *(uint32_t*)(rdram + 0x001CABC8);
        uint32_t ni_sys_ptr = *(uint32_t*)(rdram + 0x001CAC1C);
        // Save-screen constructor gate + selector globals.
        uint32_t save_gate_bits = *(uint32_t*)(rdram + 0x001CAA60);
        uint32_t save_selector = *(uint32_t*)(rdram + 0x001C833C);
        // Read current gamestate from GSM
        uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
        int32_t cur_gs = 0;
        if (gsm_addr != 0) {
            uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
            cur_gs = *(int32_t*)(rdram + gsm_phys + 0x24);
        }
        static uint32_t prev_exec = 0, prev_ni = 0, prev_save_gate = 0, prev_save_sel = 0;
        static int32_t prev_gs = -1;
        bool changed = (exec_flags != prev_exec ||
                        ni_sys_ptr != prev_ni ||
                        save_gate_bits != prev_save_gate ||
                        save_selector != prev_save_sel ||
                        cur_gs != prev_gs);
        if (g_dl_n <= 10 || g_dl_n % 100 == 0 || changed) {
            fprintf(stderr, "[STATE] DL#%d gs=%d exec=0x%08X ni=0x%08X gate=0x%08X sel=0x%08X%s\n",
                    g_dl_n, cur_gs, exec_flags, ni_sys_ptr, save_gate_bits, save_selector,
                    changed ? " ← CHANGED" : "");
            prev_exec = exec_flags;
            prev_ni = ni_sys_ptr;
            prev_save_gate = save_gate_bits;
            prev_save_sel = save_selector;
            prev_gs = cur_gs;
        }
    }
#endif

    // Track 2nd SETCIMG for VI_ORIGIN (1st is Z-buffer, 2nd is color buffer)
    {
        int cimg_count = 0;
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            if (((w0 >> 24) & 0xFF) == 0xFF) {
                cimg_count++;
                if (cimg_count == 2) { last_displayed_cfb = w1; break; }
            }
            if (((w0 >> 24) & 0xFF) == 0xDF) break;
        }
    }

    // Pass DL to RT64 unmodified — no filtering, no rewriting.
    app->state->rsp->reset();
    app->interpreter->loadUCodeGBI(task->t.ucode & 0x3FFFFFF, task->t.ucode_data & 0x3FFFFFF, true);
    app->state->extended.extendRDRAM = true;
    app->processDisplayLists(app->core.RDRAM, data_addr, 0, true);
}

void lod::renderer::RT64Context::update_screen() {
    static int us_n = 0; us_n++;

    // Fix VI_ORIGIN to match the SETCIMG address from the last DL.
    // The game's VI setup produces a mismatched origin (0x700280 vs 0x1DA800).
    // RT64 tracks framebuffers by SETCIMG address. VI_ORIGIN must point to
    // one of those addresses for RT64 to find and display the rendered frame.
    ultramodern::renderer::ViRegs* vi = ultramodern::renderer::get_vi_regs();
    uint32_t game_origin = vi->VI_ORIGIN_REG & 0xFFFFFF;

    // Let the game's osViSetMode set VI registers naturally.
    // Only override if the game hasn't set them (zero = uninitialized).
    if (vi->VI_STATUS_REG == 0) {
        vi->VI_STATUS_REG    = 0x0000030A;
    }
    // Detect video width. The game calls osViSetMode which stores the mode ptr
    // but the ultramodern runtime may not write VI_WIDTH_REG immediately.
    // Read directly from the last SETCIMG command: if it uses a high address
    // (>0x400000), the game is in high-res mode (640x480).
    uint32_t width = vi->VI_WIDTH_REG;
    if (width == 0 || width == 320) {
        // Check if the last DL used a high-res framebuffer
        if (last_displayed_cfb >= 0x400000) {
            width = 640;
        } else {
            width = 320;
        }
        vi->VI_WIDTH_REG = width;
    }
    // Only set timing registers if uninitialized
    if (vi->VI_V_SYNC_REG == 0) {
        vi->VI_INTR_REG      = 2;
        vi->VI_TIMING_REG    = 0x03E52239;
        vi->VI_V_SYNC_REG    = 0x020D;
        vi->VI_H_SYNC_REG    = 0x00000C15;
        vi->VI_LEAP_REG      = 0x0C150C15;
        vi->VI_H_START_REG   = 0x006C02EC;
        vi->VI_V_START_REG   = 0x00250239;
        vi->VI_V_BURST_REG   = 0x000E0204;
        vi->VI_X_SCALE_REG   = 0x00000200;
        vi->VI_Y_SCALE_REG   = 0x00000400;
    }

    // Use the last SETCIMG from the DL as VI_ORIGIN (+ 1 row offset).
    // Row offset = width * 2 bytes (16bpp).
    uint32_t row_offset = width * 2;
    if (last_displayed_cfb != 0) {
        vi->VI_ORIGIN_REG = last_displayed_cfb + row_offset;
    } else if (game_origin > 0) {
        vi->VI_ORIGIN_REG = game_origin;
    } else {
        vi->VI_ORIGIN_REG = 0x1DA800 + row_offset;
    }
#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
    if (us_n <= 5 || us_n % 5000 == 0) {
        fprintf(stderr, "[VI#%d] game_origin=0x%06X displayed_cfb=0x%06X VI_ORIGIN=0x%06X width=%d\n",
                us_n, game_origin, last_displayed_cfb, vi->VI_ORIGIN_REG, width);
    }
#endif

#if LOD_ENABLE_CFB_SNAPSHOT
    {
        int32_t gs = lod_render_current_gamestate(app->core.RDRAM);
        static int32_t prev_snapshot_gs = INT32_MIN;
        static int gs_frame = 0;
        if (gs != prev_snapshot_gs) {
            prev_snapshot_gs = gs;
            gs_frame = 0;
        }
        gs_frame++;

        bool interesting_gs = (gs == 1 || gs == 12 || gs == 5 || gs == 6 || gs == 8 || gs == 3);
        bool one_shot = interesting_gs && (gs_frame == 1);
        bool gs5_probe = (gs == 5) &&
                         (gs_frame == 30 || gs_frame == 120 || gs_frame == 300 ||
                          gs_frame == 600 || gs_frame == 1200 || gs_frame == 2400);
        bool gs3_probe = (gs == 3) &&
                         (gs_frame == 30 || gs_frame == 120 || gs_frame == 300 ||
                          gs_frame == 360 || gs_frame == 420 || gs_frame == 480 ||
                          gs_frame == 540 || gs_frame == 600 || gs_frame == 720 ||
                          gs_frame == 900 || gs_frame == 1200 || gs_frame == 2400);
        bool gs12_probe = (gs == 12) && (gs_frame == 30 || gs_frame == 120);
        bool menu_probe = (gs == 6 || gs == 8) &&
                          (gs_frame == 30 || gs_frame == 120 || gs_frame == 300 ||
                           gs_frame == 1080 || gs_frame == 1120 || gs_frame == 1160 ||
                           gs_frame == 1200 || gs_frame == 1260 || gs_frame == 1320);

        if (one_shot || gs5_probe || gs3_probe || gs12_probe || menu_probe) {
            lod_dump_cfb_ppm(app->core.RDRAM, gs, g_dl_n, us_n, last_displayed_cfb, width);
        }
    }
#endif

    app->updateScreen();
}

void lod::renderer::RT64Context::shutdown() {
    if (app != nullptr) {
        app->end();
    }
}

bool lod::renderer::RT64Context::update_config(const ultramodern::renderer::GraphicsConfig& old_config, const ultramodern::renderer::GraphicsConfig& new_config) {
    if (old_config == new_config) {
        return false;
    }

    if (new_config.wm_option != old_config.wm_option) {
        app->setFullScreen(new_config.wm_option == ultramodern::renderer::WindowMode::Fullscreen);
    }

    set_application_user_config(app.get(), new_config);
    app->updateUserConfig(true);

    if (new_config.msaa_option != old_config.msaa_option) {
        app->updateMultisampling();
    }
    return true;
}

void lod::renderer::RT64Context::enable_instant_present() {
    app->enhancementConfig.presentation.mode = RT64::EnhancementConfiguration::Presentation::Mode::PresentEarly;
    app->updateEnhancementConfig();
}

uint32_t lod::renderer::RT64Context::get_display_framerate() const {
    return app->presentQueue->ext.sharedResources->swapChainRate;
}

float lod::renderer::RT64Context::get_resolution_scale() const {
    constexpr int ReferenceHeight = 240;
    switch (app->userConfig.resolution) {
        case RT64::UserConfiguration::Resolution::WindowIntegerScale:
            if (app->sharedQueueResources->swapChainHeight > 0) {
                return std::max(float((app->sharedQueueResources->swapChainHeight + ReferenceHeight - 1) / ReferenceHeight), 1.0f);
            }
            return 1.0f;
        case RT64::UserConfiguration::Resolution::Manual:
            return float(app->userConfig.resolutionMultiplier);
        case RT64::UserConfiguration::Resolution::Original:
        default:
            return 1.0f;
    }
}

std::unique_ptr<ultramodern::renderer::RendererContext> lod::renderer::create_render_context(uint8_t* rdram, ultramodern::renderer::WindowHandle window_handle, bool developer_mode) {
    return std::make_unique<lod::renderer::RT64Context>(rdram, window_handle, developer_mode);
}
