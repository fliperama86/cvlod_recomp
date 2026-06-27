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

#ifndef LOD_ENABLE_TOWER_RENDER_SUMMARY
#define LOD_ENABLE_TOWER_RENDER_SUMMARY 0
#endif

#ifndef LOD_ENABLE_ISSUE23_TRACE
#define LOD_ENABLE_ISSUE23_TRACE 0
#endif

#ifndef LOD_FIX_TOWER_BLACK_OVERLAY
#define LOD_FIX_TOWER_BLACK_OVERLAY 1
#endif

#if LOD_ENABLE_TOWER_RENDER_SUMMARY || LOD_FIX_TOWER_BLACK_OVERLAY || LOD_ENABLE_ISSUE23_TRACE
#include "hle/rt64_workload_queue.h"
#endif

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

#ifndef LOD_ENABLE_GBI_MISS_TRACE
#define LOD_ENABLE_GBI_MISS_TRACE 0
#endif

extern "C" uint32_t lod_current_map_overlay_rom();
extern "C" uint32_t lod_current_map_overlay_size();
extern "C" int lod_current_map_overlay_load_count();

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
            // Let RT64 choose the platform default. On Windows this resolves to
            // D3D12, which avoids requiring users to have a Vulkan runtime
            // installed. Explicit Vulkan/Metal/D3D12 choices still override it.
            app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Automatic;
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

#if LOD_ENABLE_CFB_SNAPSHOT || LOD_ENABLE_TOWER_RENDER_SUMMARY || LOD_ENABLE_ISSUE23_TRACE
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
#endif

#if LOD_ENABLE_CFB_SNAPSHOT || LOD_ENABLE_ISSUE23_TRACE
static uint16_t lod_rdram_be16(uint8_t* rdram, uint32_t phys) {
    return ((uint16_t)rdram[(phys + 0) ^ 3] << 8) |
           ((uint16_t)rdram[(phys + 1) ^ 3] << 0);
}
#endif

#if LOD_ENABLE_CFB_SNAPSHOT
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

#if LOD_ENABLE_ISSUE23_TRACE
static uint32_t lod_render_phys_u32(uint8_t* rdram, uint32_t phys) {
    return phys <= 0x800000 - 4 ? *(uint32_t*)(rdram + phys) : 0;
}

static void lod_issue23_log_cfb_probe(uint8_t* rdram, int32_t gs, int dl_n, int us_n,
                                      uint32_t cfb, uint32_t width, uint32_t vi_origin) {
    if (width == 0) {
        width = 320;
    }

    const uint32_t height = (width >= 640) ? 480 : 240;
    const uint64_t pixels = (uint64_t)width * (uint64_t)height;
    const uint64_t bytes = pixels * 2ULL;
    if (cfb == 0 || cfb >= 0x800000 || bytes > 0x800000ULL || cfb + bytes > 0x800000ULL) {
        fprintf(stderr,
                "[ISSUE23_CFB] us=%d DL#%d gs=%d map#%d cfb=0x%06X vi=0x%06X width=%u "
                "height=%u invalid=1\n",
                us_n, dl_n, gs, lod_current_map_overlay_load_count(), cfb, vi_origin,
                width, height);
        return;
    }

    const uint64_t step = std::max<uint64_t>(1, pixels / 4096);
    uint64_t samples = 0;
    uint64_t nonzero = 0;
    uint64_t sum = 0;
    uint16_t max_px = 0;
    for (uint64_t i = 0; i < pixels; i += step) {
        const uint32_t phys = cfb + (uint32_t)(i * 2ULL);
        const uint16_t px = lod_rdram_be16(rdram, phys);
        samples++;
        if (px != 0) {
            nonzero++;
        }
        sum += px;
        if (px > max_px) {
            max_px = px;
        }
    }

    fprintf(stderr,
            "[ISSUE23_CFB] us=%d DL#%d gs=%d map#%d cfb=0x%06X vi=0x%06X width=%u height=%u "
            "samples=%llu nonzero=%llu max=0x%04X mean=0x%04llX\n",
            us_n, dl_n, gs, lod_current_map_overlay_load_count(), cfb, vi_origin,
            width, height, (unsigned long long)samples, (unsigned long long)nonzero,
            max_px, (unsigned long long)(samples != 0 ? (sum / samples) : 0));
}
#endif

#if LOD_FIX_TOWER_BLACK_OVERLAY
static bool lod_tower_black_overlay_rect_large(const RT64::FixedRect& rect) {
    return rect.ulx <= 64 && rect.uly <= 64 && rect.lrx >= 1216 && rect.lry >= 896;
}

static bool lod_tower_black_overlay_rgb_black(const interop::float4& color) {
    return color.x <= 0.003f && color.y <= 0.003f && color.z <= 0.003f;
}

static bool lod_tower_is_stuck_black_overlay_call(const RT64::GameCall& call,
                                                  RT64::Projection::Type projection_type) {
    const RT64::DrawCall& d = call.callDesc;

    // Tower of Sorcery currently emits a final full-screen black rectangle over
    // otherwise valid world/HUD geometry. Match that exact end-of-workload
    // overlay shape, not generic black UI/fade quads.
    return projection_type == RT64::Projection::Type::Rectangle &&
           d.triangleCount == 2 &&
           d.otherMode.cycleType() == G_CYC_1CYCLE &&
           d.geometryMode == 0x00800000 &&
           d.colorCombiner.L == 0xFCFFFFFF &&
           d.colorCombiner.H == 0xFFFDF6FB &&
           d.otherMode.L == 0x00504344 &&
           d.otherMode.H == 0x00088C2F &&
           lod_tower_black_overlay_rect_large(d.rect) &&
           lod_tower_black_overlay_rect_large(d.scissorRect) &&
           lod_tower_black_overlay_rgb_black(d.rdpParams.primColor) &&
           d.rdpParams.primColor.w >= 0.99f;
}

static bool lod_tower_remove_stuck_black_overlay(RT64::Workload& workload, int dl_n) {
    // Require a real Tower scene workload before removing anything. Small
    // early transition/menu workloads may legitimately be only overlays.
    if (workload.gameCallCount < 80 || workload.fbPairCount == 0) {
        return false;
    }

    uint32_t global_call = workload.gameCallCount;
    for (uint32_t f_rev = workload.fbPairCount; f_rev > 0; f_rev--) {
        RT64::FramebufferPair& fb = workload.fbPairs[f_rev - 1];
        if (fb.gameCallCount == 0) {
            continue;
        }

        for (uint32_t p_rev = fb.projectionCount; p_rev > 0; p_rev--) {
            RT64::Projection& proj = fb.projections[p_rev - 1];
            if (proj.gameCallCount == 0) {
                continue;
            }

            const uint32_t last_call_index = proj.gameCallCount - 1;
            global_call--;
            const RT64::GameCall& call = proj.gameCalls[last_call_index];
            if (!lod_tower_is_stuck_black_overlay_call(call, proj.type)) {
                return false;
            }

            proj.gameCallCount--;
            fb.gameCallCount--;
            workload.gameCallCount--;

            static int suppress_count = 0;
            suppress_count++;
            if (suppress_count <= 12 || (suppress_count % 300) == 0) {
                fprintf(stderr,
                    "[TOWER_BLACK_OVERLAY_FIX] suppressed stuck full-screen black rect #%d "
                    "DL#%d fb=%u proj=%u global=%u remaining_calls=%u\n",
                    suppress_count, dl_n, f_rev - 1, p_rev - 1, global_call,
                    workload.gameCallCount);
            }
            return true;
        }
    }

    return false;
}
#endif

#if LOD_ENABLE_TOWER_RENDER_SUMMARY || LOD_ENABLE_ISSUE23_TRACE
struct LodTowerTopDlStats {
    uint32_t cmds = 0;
    uint32_t end_seen = 0;
    uint32_t set_cimg = 0;
    uint32_t fill_rect = 0;
    uint32_t tex_rect = 0;
    uint32_t branch_dl = 0;
    uint32_t op_counts[256] = {};
    uint32_t first_w0[8] = {};
    uint32_t first_w1[8] = {};
    uint32_t first_count = 0;
};

static LodTowerTopDlStats lod_tower_scan_top_dl(uint8_t* rdram, uint32_t data_addr) {
    LodTowerTopDlStats stats{};
    if (data_addr >= 0x800000) {
        return stats;
    }

    const uint32_t max_cmds = std::min<uint32_t>(512, (0x800000 - data_addr) / 8);
    for (uint32_t i = 0; i < max_cmds; i++) {
        const uint32_t off = data_addr + i * 8;
        uint32_t w0 = *(uint32_t*)(rdram + off + 0);
        uint32_t w1 = *(uint32_t*)(rdram + off + 4);
        const uint32_t op = (w0 >> 24) & 0xFF;
        stats.cmds++;
        stats.op_counts[op]++;
        if (stats.first_count < 8) {
            stats.first_w0[stats.first_count] = w0;
            stats.first_w1[stats.first_count] = w1;
            stats.first_count++;
        }
        if (op == 0xFF) {
            stats.set_cimg++;
        }
        else if (op == 0xF6) {
            stats.fill_rect++;
        }
        else if (op == 0xE4 || op == 0xE5) {
            stats.tex_rect++;
        }
        else if (op == 0xDE) {
            stats.branch_dl++;
        }
        else if (op == 0xDF) {
            stats.end_seen = 1;
            break;
        }
    }

    return stats;
}

static void lod_tower_format_top_ops(const LodTowerTopDlStats& stats, char* out, size_t out_size) {
    if (out_size == 0) {
        return;
    }
    out[0] = '\0';
    size_t used = 0;
    for (uint32_t rank = 0; rank < 8; rank++) {
        uint32_t best_op = 0;
        uint32_t best_count = 0;
        for (uint32_t op = 0; op < 256; op++) {
            uint32_t count = stats.op_counts[op];
            if (count == 0) {
                continue;
            }
            bool already_used = false;
            for (uint32_t prev = 0; prev < rank; prev++) {
                char needle[8];
                std::snprintf(needle, sizeof(needle), "%02X:", op);
                if (std::strstr(out, needle) != nullptr) {
                    already_used = true;
                    break;
                }
            }
            if (already_used) {
                continue;
            }
            if (count > best_count) {
                best_count = count;
                best_op = op;
            }
        }
        if (best_count == 0) {
            break;
        }
        int wrote = std::snprintf(out + used, out_size - used, "%s%02X:%u",
                                  (used == 0) ? "" : ",", best_op, best_count);
        if (wrote <= 0) {
            break;
        }
        if ((size_t)wrote >= out_size - used) {
            out[out_size - 1] = '\0';
            break;
        }
        used += (size_t)wrote;
    }
}

static const char* lod_tower_fb_flush_reason_name(RT64::FramebufferPair::FlushReason reason) {
    switch (reason) {
        case RT64::FramebufferPair::FlushReason::None: return "none";
        case RT64::FramebufferPair::FlushReason::SamplingFromColorImage: return "sample-color";
        case RT64::FramebufferPair::FlushReason::SamplingFromDepthImage: return "sample-depth";
        case RT64::FramebufferPair::FlushReason::ColorImageChanged: return "color-change";
        case RT64::FramebufferPair::FlushReason::DepthImageChanged: return "depth-change";
        case RT64::FramebufferPair::FlushReason::ProcessDisplayListsEnd: return "process-end";
    }
    return "?";
}

static const char* lod_tower_projection_type_name(RT64::Projection::Type type) {
    switch (type) {
        case RT64::Projection::Type::None: return "none";
        case RT64::Projection::Type::Perspective: return "persp";
        case RT64::Projection::Type::Orthographic: return "ortho";
        case RT64::Projection::Type::Rectangle: return "rect";
        case RT64::Projection::Type::Triangle: return "tri";
    }
    return "?";
}

static const char* lod_tower_cycle_name(uint32_t cycle) {
    switch (cycle) {
        case G_CYC_1CYCLE: return "1cyc";
        case G_CYC_2CYCLE: return "2cyc";
        case G_CYC_COPY: return "copy";
        case G_CYC_FILL: return "fill";
    }
    return "?";
}

static int lod_tower_color_byte(float value) {
    value = std::max(0.0f, std::min(1.0f, value));
    return (int)(value * 255.0f + 0.5f);
}

static bool lod_tower_color_rgb_black(const interop::float4& color) {
    return (color.x <= 0.003f) && (color.y <= 0.003f) && (color.z <= 0.003f);
}

static bool lod_tower_rect_large(const RT64::FixedRect& rect) {
    return rect.ulx <= 64 && rect.uly <= 64 && rect.lrx >= 1216 && rect.lry >= 896;
}

static void lod_tower_log_call_detail(const char* tag, int tower_task, int dl_n,
                                      const RT64::Workload& workload,
                                      uint32_t fb_index, uint32_t proj_index,
                                      RT64::Projection::Type proj_type,
                                      uint32_t local_call_index, uint32_t global_call_index,
                                      const RT64::GameCall& call) {
    const RT64::DrawCall& d = call.callDesc;
    const interop::float4& prim = d.rdpParams.primColor;
    const interop::float4& env = d.rdpParams.envColor;
    const interop::float4& fog = d.rdpParams.fogColor;
    const interop::float4& blend = d.rdpParams.blendColor;
    const uint32_t cycle = d.otherMode.cycleType();
    const bool black_rgb =
        lod_tower_color_rgb_black(prim) &&
        lod_tower_color_rgb_black(env) &&
        lod_tower_color_rgb_black(fog);
    const bool large_rect = lod_tower_rect_large(d.rect) || lod_tower_rect_large(d.scissorRect);

    fprintf(stderr,
            "[TOWER_RENDER_CALL] %s task=%d DL#%d fb=%u proj=%u/%s call=%u global=%u "
            "tri=%u cycle=%s tex=%u tile=%u load=%u geom=0x%08X cc=0x%08X/%08X om=0x%08X/%08X "
            "fill=0x%08X rect=(%d,%d,%d,%d) scissor=(%d,%d,%d,%d) "
            "prim=(%d,%d,%d,%d) env=(%d,%d,%d,%d) fog=(%d,%d,%d,%d) blend=(%d,%d,%d,%d) "
            "blackRGB=%u largeRect=%u z=%u/%u alphaCmp=0x%X forceBl=%u\n",
            tag, tower_task, dl_n, fb_index, proj_index, lod_tower_projection_type_name(proj_type),
            local_call_index, global_call_index,
            d.triangleCount, lod_tower_cycle_name(cycle), d.textureOn ? 1U : 0U,
            d.tileCount, d.loadCount, d.geometryMode, d.colorCombiner.L, d.colorCombiner.H,
            d.otherMode.L, d.otherMode.H, d.fillColor,
            d.rect.ulx, d.rect.uly, d.rect.lrx, d.rect.lry,
            d.scissorRect.ulx, d.scissorRect.uly, d.scissorRect.lrx, d.scissorRect.lry,
            lod_tower_color_byte(prim.x), lod_tower_color_byte(prim.y),
            lod_tower_color_byte(prim.z), lod_tower_color_byte(prim.w),
            lod_tower_color_byte(env.x), lod_tower_color_byte(env.y),
            lod_tower_color_byte(env.z), lod_tower_color_byte(env.w),
            lod_tower_color_byte(fog.x), lod_tower_color_byte(fog.y),
            lod_tower_color_byte(fog.z), lod_tower_color_byte(fog.w),
            lod_tower_color_byte(blend.x), lod_tower_color_byte(blend.y),
            lod_tower_color_byte(blend.z), lod_tower_color_byte(blend.w),
            black_rgb ? 1U : 0U, large_rect ? 1U : 0U,
            d.otherMode.zCmp() ? 1U : 0U, d.otherMode.zUpd() ? 1U : 0U,
            d.otherMode.alphaCompare(), d.otherMode.forceBlend() ? 1U : 0U);

    const uint32_t max_tiles = std::min<uint32_t>(d.tileCount, 3);
    for (uint32_t t = 0; t < max_tiles; t++) {
        const uint32_t tile_index = d.tileIndex + t;
        if (tile_index >= workload.drawData.callTiles.size()) {
            break;
        }
        const RT64::DrawCallTile& tile = workload.drawData.callTiles[tile_index];
        fprintf(stderr,
                "[TOWER_RENDER_TILE] %s task=%d DL#%d global=%u t=%u idx=%u valid=%u "
                "hash=0x%016llX sample=%ux%u line=%u tlut=%u raw=%u copy=%u copySize=%ux%u reinterpret=%u/%u/%u texcoord=(%d,%d)-(%d,%d)\n",
                tag, tower_task, dl_n, global_call_index, t, tile_index,
                tile.valid ? 1U : 0U,
                (unsigned long long)tile.tmemHashOrID,
                tile.sampleWidth, tile.sampleHeight, tile.lineWidth, tile.tlut,
                tile.rawTMEM ? 1U : 0U, tile.tileCopyUsed ? 1U : 0U,
                tile.tileCopyWidth, tile.tileCopyHeight,
                tile.reinterpretTile ? 1U : 0U, tile.reinterpretSiz, tile.reinterpretFmt,
                tile.minTexcoord.x, tile.minTexcoord.y,
                tile.maxTexcoord.x, tile.maxTexcoord.y);
    }
}

static void lod_tower_log_workload_summary(const char* tag, int tower_task, int dl_n,
                                           uint32_t cursor, const RT64::Workload& workload) {
    const uint32_t face_tri = (uint32_t)(workload.drawData.faceIndices.size() / 3);
    const uint32_t raw_tri = (uint32_t)(workload.drawData.rawTriVertexCount() / 3);
    uint32_t proj_counts[5] = {};
    uint32_t cycle_1 = 0;
    uint32_t cycle_2 = 0;
    uint32_t cycle_copy = 0;
    uint32_t cycle_fill = 0;
    uint32_t rect_calls = 0;
    uint32_t persp_calls = 0;
    uint32_t large_rect_calls = 0;
    uint32_t black_rgb_calls = 0;
    for (uint32_t f = 0; f < workload.fbPairCount; f++) {
        const RT64::FramebufferPair& fb = workload.fbPairs[f];
        for (uint32_t p = 0; p < fb.projectionCount; p++) {
            const RT64::Projection& proj = fb.projections[p];
            const uint32_t type_index = std::min<uint32_t>((uint32_t)proj.type, 4);
            proj_counts[type_index]++;
            for (uint32_t c = 0; c < proj.gameCallCount; c++) {
                const RT64::GameCall& call = proj.gameCalls[c];
                const RT64::DrawCall& d = call.callDesc;
                const uint32_t cycle = d.otherMode.cycleType();
                if (cycle == G_CYC_1CYCLE) { cycle_1++; }
                else if (cycle == G_CYC_2CYCLE) { cycle_2++; }
                else if (cycle == G_CYC_COPY) { cycle_copy++; }
                else if (cycle == G_CYC_FILL) { cycle_fill++; }
                if (proj.type == RT64::Projection::Type::Rectangle) { rect_calls++; }
                if (proj.type == RT64::Projection::Type::Perspective) { persp_calls++; }
                if (lod_tower_rect_large(d.rect) || lod_tower_rect_large(d.scissorRect)) { large_rect_calls++; }
                if (lod_tower_color_rgb_black(d.rdpParams.primColor) &&
                    lod_tower_color_rgb_black(d.rdpParams.envColor) &&
                    lod_tower_color_rgb_black(d.rdpParams.fogColor)) {
                    black_rgb_calls++;
                }
            }
        }
    }
    fprintf(stderr,
            "[TOWER_RENDER] %s task=%d DL#%d cursor=%u wid=%llu calls=%u fbPairs=%u submitted=%u "
            "faceTri=%u rawTri=%u vertices=%u posFloats=%zu rdp=%zu render=%zu loads=%zu tiles=%zu "
            "xforms=%zu viewProj=%zu warnings=%zu cfb=0x%06X "
            "proj(n/p/o/r/t)=%u/%u/%u/%u/%u cycles(1/2/copy/fill)=%u/%u/%u/%u "
            "calls(rect/persp/large/blackRGB)=%u/%u/%u/%u\n",
            tag, tower_task, dl_n, cursor, (unsigned long long)workload.workloadId,
            workload.gameCallCount, workload.fbPairCount, workload.fbPairSubmitted,
            face_tri, raw_tri, workload.drawData.vertexCount(), workload.drawData.posFloats.size(),
            workload.drawData.rdpParams.size(), workload.drawData.renderParams.size(),
            workload.drawData.loadOperations.size(), workload.drawData.callTiles.size(),
            workload.drawData.worldTransforms.size(), workload.drawData.viewProjTransforms.size(),
            workload.commandWarnings.size(), last_displayed_cfb,
            proj_counts[0], proj_counts[1], proj_counts[2], proj_counts[3], proj_counts[4],
            cycle_1, cycle_2, cycle_copy, cycle_fill,
            rect_calls, persp_calls, large_rect_calls, black_rgb_calls);

    const uint32_t fb_count = std::min<uint32_t>(workload.fbPairCount, 4);
    for (uint32_t i = 0; i < fb_count; i++) {
        const RT64::FramebufferPair& fb = workload.fbPairs[i];
        fprintf(stderr,
                "[TOWER_RENDER_FB] %s task=%d DL#%d fb=%u color=0x%06X fmt=%u siz=%u width=%u "
                "depth=0x%06X calls=%u proj=%u fillOnly=%u sync=%u depthRW=%u/%u flush=%s "
                "draw=(%d,%d,%d,%d) scissor=(%d,%d,%d,%d) ops=%zu/%zu discards=%zu\n",
                tag, tower_task, dl_n, i,
                fb.colorImage.address, fb.colorImage.fmt, fb.colorImage.siz, fb.colorImage.width,
                fb.depthImage.address, fb.gameCallCount, fb.projectionCount,
                fb.fillRectOnly ? 1U : 0U, fb.syncRequired ? 1U : 0U,
                fb.depthRead ? 1U : 0U, fb.depthWrite ? 1U : 0U,
                lod_tower_fb_flush_reason_name(fb.flushReason),
                fb.drawColorRect.ulx, fb.drawColorRect.uly, fb.drawColorRect.lrx, fb.drawColorRect.lry,
                fb.scissorRect.ulx, fb.scissorRect.uly, fb.scissorRect.lrx, fb.scissorRect.lry,
                fb.startFbOperations.size(), fb.endFbOperations.size(), fb.startFbDiscards.size());
    }

    if (std::strcmp(tag, "prev") == 0) {
        const uint32_t max_detail_per_summary = 128;
        uint32_t emitted = 0;
        uint32_t global_call = 0;
        for (uint32_t f = 0; f < workload.fbPairCount && emitted < max_detail_per_summary; f++) {
            const RT64::FramebufferPair& fb = workload.fbPairs[f];
            for (uint32_t p = 0; p < fb.projectionCount && emitted < max_detail_per_summary; p++) {
                const RT64::Projection& proj = fb.projections[p];
                for (uint32_t c = 0; c < proj.gameCallCount && emitted < max_detail_per_summary; c++, global_call++) {
                    const RT64::GameCall& call = proj.gameCalls[c];
                    const RT64::DrawCall& d = call.callDesc;
                    const bool selected =
                        (global_call < 8) ||
                        (global_call + 24 >= workload.gameCallCount) ||
                        ((proj.type == RT64::Projection::Type::Perspective) && ((c % 16) == 0)) ||
                        (proj.type == RT64::Projection::Type::Rectangle) ||
                        (d.otherMode.cycleType() == G_CYC_FILL) ||
                        (d.otherMode.cycleType() == G_CYC_COPY);
                    if (selected) {
                        lod_tower_log_call_detail(tag, tower_task, dl_n, workload, f, p, proj.type, c, global_call, call);
                        emitted++;
                    }
                }
            }
        }
    }
}
#endif

#if LOD_ENABLE_ISSUE23_TRACE
static void lod_issue23_log_workload_summary(const char* tag, int issue_task, int dl_n,
                                             uint32_t cursor, const RT64::Workload& workload) {
    const uint32_t face_tri = (uint32_t)(workload.drawData.faceIndices.size() / 3);
    const uint32_t raw_tri = (uint32_t)(workload.drawData.rawTriVertexCount() / 3);
    uint32_t proj_counts[5] = {};
    uint32_t cycle_1 = 0;
    uint32_t cycle_2 = 0;
    uint32_t cycle_copy = 0;
    uint32_t cycle_fill = 0;
    uint32_t rect_calls = 0;
    uint32_t persp_calls = 0;
    uint32_t large_rect_calls = 0;
    uint32_t black_rgb_calls = 0;
    for (uint32_t f = 0; f < workload.fbPairCount; f++) {
        const RT64::FramebufferPair& fb = workload.fbPairs[f];
        for (uint32_t p = 0; p < fb.projectionCount; p++) {
            const RT64::Projection& proj = fb.projections[p];
            const uint32_t type_index = std::min<uint32_t>((uint32_t)proj.type, 4);
            proj_counts[type_index]++;
            for (uint32_t c = 0; c < proj.gameCallCount; c++) {
                const RT64::GameCall& call = proj.gameCalls[c];
                const RT64::DrawCall& d = call.callDesc;
                const uint32_t cycle = d.otherMode.cycleType();
                if (cycle == G_CYC_1CYCLE) { cycle_1++; }
                else if (cycle == G_CYC_2CYCLE) { cycle_2++; }
                else if (cycle == G_CYC_COPY) { cycle_copy++; }
                else if (cycle == G_CYC_FILL) { cycle_fill++; }
                if (proj.type == RT64::Projection::Type::Rectangle) { rect_calls++; }
                if (proj.type == RT64::Projection::Type::Perspective) { persp_calls++; }
                if (lod_tower_rect_large(d.rect) || lod_tower_rect_large(d.scissorRect)) { large_rect_calls++; }
                if (lod_tower_color_rgb_black(d.rdpParams.primColor) &&
                    lod_tower_color_rgb_black(d.rdpParams.envColor) &&
                    lod_tower_color_rgb_black(d.rdpParams.fogColor)) {
                    black_rgb_calls++;
                }
            }
        }
    }

    fprintf(stderr,
            "[ISSUE23_RENDER] %s task=%d DL#%d cursor=%u wid=%llu calls=%u fbPairs=%u submitted=%u "
            "faceTri=%u rawTri=%u vertices=%u loads=%zu warnings=%zu cfb=0x%06X "
            "proj(n/p/o/r/t)=%u/%u/%u/%u/%u cycles(1/2/copy/fill)=%u/%u/%u/%u "
            "calls(rect/persp/large/blackRGB)=%u/%u/%u/%u\n",
            tag, issue_task, dl_n, cursor, (unsigned long long)workload.workloadId,
            workload.gameCallCount, workload.fbPairCount, workload.fbPairSubmitted,
            face_tri, raw_tri, workload.drawData.vertexCount(),
            workload.drawData.loadOperations.size(), workload.commandWarnings.size(),
            last_displayed_cfb,
            proj_counts[0], proj_counts[1], proj_counts[2], proj_counts[3], proj_counts[4],
            cycle_1, cycle_2, cycle_copy, cycle_fill,
            rect_calls, persp_calls, large_rect_calls, black_rgb_calls);

    const uint32_t fb_count = std::min<uint32_t>(workload.fbPairCount, 2);
    for (uint32_t i = 0; i < fb_count; i++) {
        const RT64::FramebufferPair& fb = workload.fbPairs[i];
        fprintf(stderr,
                "[ISSUE23_RENDER_FB] %s task=%d DL#%d fb=%u color=0x%06X fmt=%u siz=%u width=%u "
                "depth=0x%06X calls=%u proj=%u fillOnly=%u flush=%s "
                "draw=(%d,%d,%d,%d) scissor=(%d,%d,%d,%d)\n",
                tag, issue_task, dl_n, i,
                fb.colorImage.address, fb.colorImage.fmt, fb.colorImage.siz, fb.colorImage.width,
                fb.depthImage.address, fb.gameCallCount, fb.projectionCount,
                fb.fillRectOnly ? 1U : 0U,
                lod_tower_fb_flush_reason_name(fb.flushReason),
                fb.drawColorRect.ulx, fb.drawColorRect.uly, fb.drawColorRect.lrx, fb.drawColorRect.lry,
                fb.scissorRect.ulx, fb.scissorRect.uly, fb.scissorRect.lrx, fb.scissorRect.lry);
    }
}
#endif

void lod::renderer::RT64Context::send_dl(const OSTask* task) {
    uint32_t data_addr = task->t.data_ptr & 0x3FFFFFF;
    uint8_t* rdram = app->core.RDRAM;
    g_dl_n++;

#if LOD_ENABLE_TOWER_RENDER_SUMMARY || LOD_FIX_TOWER_BLACK_OVERLAY
    const bool tower_render_active = (lod_current_map_overlay_rom() == 0x007D9790);
#endif
#if LOD_ENABLE_ISSUE23_TRACE
    const bool issue23_render_active = (lod_current_map_overlay_rom() == 0x0082E330);
    static int issue23_render_task_count = 0;
    int issue23_render_task = 0;
    bool issue23_render_log = false;
    if (issue23_render_active) {
        issue23_render_task = ++issue23_render_task_count;
        issue23_render_log = (issue23_render_task <= 80) || ((issue23_render_task % 60) == 0);
        if (issue23_render_log) {
            const int32_t gs = lod_render_current_gamestate(rdram);
            const uint32_t exec_flags = lod_render_phys_u32(rdram, 0x001CABC8);
            const uint32_t ni_sys_ptr = lod_render_phys_u32(rdram, 0x001CAC1C);
            const LodTowerTopDlStats top_stats = lod_tower_scan_top_dl(rdram, data_addr);
            char top_ops[128];
            lod_tower_format_top_ops(top_stats, top_ops, sizeof(top_ops));
            fprintf(stderr,
                    "[ISSUE23_RENDER_TASK] pre task=%d DL#%d gs=%d exec=0x%08X ni=0x%08X "
                    "map#%d map=0x%08X type=%u flags=0x%X data=0x%06X size=0x%X "
                    "ucode=0x%06X/%u udata=0x%06X/%u top_cmds=%u end=%u setcimg=%u "
                    "fill=%u texrect=%u gdl=%u top_ops=%s first=",
                    issue23_render_task, g_dl_n, gs, exec_flags, ni_sys_ptr,
                    lod_current_map_overlay_load_count(), lod_current_map_overlay_rom(),
                    task->t.type, task->t.flags, data_addr, task->t.data_size,
                    task->t.ucode & 0x3FFFFFF, task->t.ucode_size,
                    task->t.ucode_data & 0x3FFFFFF, task->t.ucode_data_size,
                    top_stats.cmds, top_stats.end_seen, top_stats.set_cimg,
                    top_stats.fill_rect, top_stats.tex_rect, top_stats.branch_dl,
                    top_ops[0] != '\0' ? top_ops : "-");
            for (uint32_t i = 0; i < top_stats.first_count; i++) {
                fprintf(stderr, "%s%08X/%08X", (i == 0) ? "" : ",",
                        top_stats.first_w0[i], top_stats.first_w1[i]);
            }
            fprintf(stderr, "\n");
        }
    }
#endif
#if LOD_ENABLE_TOWER_RENDER_SUMMARY
    static int tower_render_task_count = 0;
    int tower_render_task = 0;
    bool tower_render_log = false;
    if (tower_render_active) {
        tower_render_task = ++tower_render_task_count;
        tower_render_log = (tower_render_task <= 48) || ((tower_render_task % 30) == 0);
        if (tower_render_log) {
            const int32_t gs = lod_render_current_gamestate(rdram);
            const LodTowerTopDlStats top_stats = lod_tower_scan_top_dl(rdram, data_addr);
            char top_ops[128];
            lod_tower_format_top_ops(top_stats, top_ops, sizeof(top_ops));
            fprintf(stderr,
                    "[TOWER_RENDER_TASK] pre task=%d DL#%d gs=%d map#%d map=0x%08X "
                    "type=%u flags=0x%X data=0x%06X size=0x%X ucode=0x%06X/%u udata=0x%06X/%u "
                    "top_cmds=%u end=%u setcimg=%u fill=%u texrect=%u gdl=%u top_ops=%s first=",
                    tower_render_task, g_dl_n, gs,
                    lod_current_map_overlay_load_count(), lod_current_map_overlay_rom(),
                    task->t.type, task->t.flags, data_addr, task->t.data_size,
                    task->t.ucode & 0x3FFFFFF, task->t.ucode_size,
                    task->t.ucode_data & 0x3FFFFFF, task->t.ucode_data_size,
                    top_stats.cmds, top_stats.end_seen, top_stats.set_cimg,
                    top_stats.fill_rect, top_stats.tex_rect, top_stats.branch_dl,
                    top_ops[0] != '\0' ? top_ops : "-");
            for (uint32_t i = 0; i < top_stats.first_count; i++) {
                fprintf(stderr, "%s%08X/%08X", (i == 0) ? "" : ",",
                        top_stats.first_w0[i], top_stats.first_w1[i]);
            }
            fprintf(stderr, "\n");
        }
    }
#endif

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
#if LOD_ENABLE_GBI_MISS_TRACE
    if (app->interpreter->hleGBI == nullptr) {
        static int gbi_miss_task_count = 0;
        gbi_miss_task_count++;

        int32_t cur_gs = 0;
        uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
        if (gsm_addr != 0) {
            uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
            cur_gs = *(int32_t*)(rdram + gsm_phys + 0x24);
        }

        uint32_t dl0 = *(uint32_t*)(rdram + data_addr + 0);
        uint32_t dl1 = *(uint32_t*)(rdram + data_addr + 4);
        uint32_t uc_text = task->t.ucode & 0x3FFFFFF;
        uint32_t uc_data = task->t.ucode_data & 0x3FFFFFF;
        uint32_t text0 = *(uint32_t*)(rdram + uc_text + 0);
        uint32_t text1 = *(uint32_t*)(rdram + uc_text + 4);
        uint32_t data0 = *(uint32_t*)(rdram + uc_data + 0);
        uint32_t data1 = *(uint32_t*)(rdram + uc_data + 4);
        uint32_t data_str0 = 0;
        uint32_t data_str1 = 0;
        for (uint32_t i = 0; i < 4; i++) {
            data_str0 = (data_str0 << 8) | rdram[(uc_data + i) ^ 0x3];
            data_str1 = (data_str1 << 8) | rdram[(uc_data + 4 + i) ^ 0x3];
        }

        fprintf(stderr,
                "[GBI_MISS_TASK] #%d DL#%d gs=%d map#%d map_rom=0x%08X map_size=0x%X "
                "type=%u flags=0x%X data=0x%08X size=0x%X ucode=0x%08X size=0x%X "
                "ucode_data=0x%08X size=0x%X dl0=%08X/%08X text0=%08X/%08X "
                "data0=%08X/%08X data_be0=%08X/%08X\n",
                gbi_miss_task_count, g_dl_n, cur_gs,
                lod_current_map_overlay_load_count(),
                lod_current_map_overlay_rom(),
                lod_current_map_overlay_size(),
                task->t.type, task->t.flags,
                data_addr, task->t.data_size,
                uc_text, task->t.ucode_size,
                uc_data, task->t.ucode_data_size,
                dl0, dl1, text0, text1, data0, data1, data_str0, data_str1);
    }
#endif
    app->state->extended.extendRDRAM = true;
    app->processDisplayLists(app->core.RDRAM, data_addr, 0, true);

#if LOD_FIX_TOWER_BLACK_OVERLAY
    if (tower_render_active && app->workloadQueue != nullptr) {
        RT64::WorkloadQueue* queue = app->workloadQueue.get();
        const uint32_t prev = queue->previousWriteCursor();
        lod_tower_remove_stuck_black_overlay(queue->workloads[prev], g_dl_n);
    }
#endif

#if LOD_ENABLE_TOWER_RENDER_SUMMARY
    if (tower_render_active && tower_render_log && app->workloadQueue != nullptr) {
        const RT64::WorkloadQueue* queue = app->workloadQueue.get();
        const uint32_t cur = (uint32_t)queue->writeCursor;
        const uint32_t prev = queue->previousWriteCursor();
        lod_tower_log_workload_summary("cur", tower_render_task, g_dl_n, cur, queue->workloads[cur]);
        lod_tower_log_workload_summary("prev", tower_render_task, g_dl_n, prev, queue->workloads[prev]);
    }
#endif
#if LOD_ENABLE_ISSUE23_TRACE
    if (issue23_render_active && issue23_render_log && app->workloadQueue != nullptr) {
        const RT64::WorkloadQueue* queue = app->workloadQueue.get();
        const uint32_t cur = (uint32_t)queue->writeCursor;
        const uint32_t prev = queue->previousWriteCursor();
        lod_issue23_log_workload_summary("cur", issue23_render_task, g_dl_n, cur, queue->workloads[cur]);
        lod_issue23_log_workload_summary("prev", issue23_render_task, g_dl_n, prev, queue->workloads[prev]);
    }
#endif
}

void lod::renderer::RT64Context::update_screen() {
    static int us_n = 0; us_n++;

#ifdef LOD_USE_ZELDA_MENU
    // Match ZeldaRecomp's launcher behavior before emulation starts: let the
    // runtime's dummy VI drive UI-only frames without LoD's gameplay CFB fixups.
    if (!ultramodern::is_game_started()) {
        app->updateScreen();
        return;
    }
#endif

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

#if LOD_ENABLE_ISSUE23_TRACE
    if (lod_current_map_overlay_rom() == 0x0082E330) {
        static int issue23_us_count = 0;
        issue23_us_count++;
        if (issue23_us_count <= 80 || (issue23_us_count % 60) == 0) {
            lod_issue23_log_cfb_probe(app->core.RDRAM, lod_render_current_gamestate(app->core.RDRAM),
                                      g_dl_n, us_n, last_displayed_cfb, width, vi->VI_ORIGIN_REG);
        }
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
