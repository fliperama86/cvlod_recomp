#include <memory>
#include <cstring>

#ifndef HLSL_CPU
#define HLSL_CPU
#endif
#include "hle/rt64_application.h"

#include "ultramodern/ultramodern.hpp"
#include "ultramodern/config.hpp"

#include "lod/lod_render.h"

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
    app->enhancementConfig.f3dex.forceBranch = true;
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

static uint32_t last_displayed_cfb = 0x001DA800; // track the displayed cfb address

// Decompressed NI file table (defined in recomp.cpp)
extern uint32_t ni_decompressed_addrs[1024];
extern int ni_decompressed_count;

// Check if an opcode byte is a valid F3DEX2/RDP command.
static bool is_valid_gbi_opcode(uint8_t op) {
    return op == 0x01 || (op >= 0x04 && op <= 0x07) || op >= 0xD7;
}

void lod::renderer::RT64Context::send_dl(const OSTask* task) {
    uint32_t data_addr = task->t.data_ptr & 0x3FFFFFF;
    uint8_t* rdram = app->core.RDRAM;
    static int dl_n = 0; dl_n++;
    bool do_log = (dl_n <= 5 || (dl_n % 200 == 0));

    // ni_decompressed_addrs/count declared at file scope above

    // === Fix segment 6 values in the DL ===
    // The game sets segment[6] = file_ptr_array[file_id], but file_ptrs are 0
    // (can't set them without breaking overlay/TLB). Instead, we rewrite
    // segment 6 G_MOVEWORD commands to use our decompressed data addresses.
    //
    // Strategy: collect all segment-6 G_DL offsets, cross-validate against
    // decompressed files to find the matching file, then rewrite segment 6.
    {
        // Collect segment-6 G_DL offsets
        uint32_t seg6_offsets[64];
        int seg6_count = 0;
        bool has_seg6_zero = false;

        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            uint8_t opcode = (w0 >> 24) & 0xFF;

            if (opcode == 0xDB && ((w0 >> 16) & 0xFF) == 0x06) {
                int seg_idx = (w0 & 0xFFFF) / 4;
                if (seg_idx == 6 && w1 == 0) has_seg6_zero = true;
            }

            if (opcode == 0xDE && seg6_count < 64) {
                int seg_idx = (w1 >> 24) & 0x0F;
                if (seg_idx == 6) {
                    seg6_offsets[seg6_count++] = w1 & 0x00FFFFFF;
                }
            }
            if (opcode == 0xDF) break;
        }

        // If segment 6 is 0 and there are G_DL branches to it, find the file
        static uint32_t cached_seg6_addr = 0;

        if (has_seg6_zero && seg6_count > 0) {
            if (cached_seg6_addr == 0) {
                // Cross-validate: find the file where ALL offsets have valid data.
                // NI files mix vertex data (interpreted as G_NOOP by RSP) with
                // actual GBI commands. Vertex data has opcode 0x00 (NOOP) with
                // non-zero w1 (coordinates). Real GBI starts at later offsets.
                // Require: all offsets non-zero, AND at least one has a real
                // GBI opcode (0xD7+), AND the second offset has real GBI.
                for (int f = 0; f < ni_decompressed_count && f < 1024; f++) {
                    uint32_t ext = ni_decompressed_addrs[f];
                    if (ext == 0) continue;
                    uint32_t phys = ext & 0x7FFFFFFF;

                    // Validate by checking a GBI SEQUENCE at the second
                    // DL offset (first may be vertex data = NOOPs).
                    // Real GBI has: E2 (SETOTHERMODE_H), D9 (GEOMETRYMODE),
                    // D7 (TEXTURE), etc. Texture data has repeating bytes.
                    bool match = false;
                    if (seg6_count >= 2) {
                        uint32_t addr2 = phys + seg6_offsets[1]; // second DL offset
                        // Check first 4 commands at this offset
                        int gbi_cmds = 0;
                        for (int k = 0; k < 4; k++) {
                            uint32_t w = *(uint32_t*)(rdram + addr2 + k * 8);
                            uint8_t op = (w >> 24) & 0xFF;
                            // Count commands that are definitively GBI state setup
                            if (op == 0xE2 || op == 0xE3 || op == 0xD9 ||
                                op == 0xD7 || op == 0xDA || op == 0xFC) {
                                gbi_cmds++;
                            }
                        }
                        match = (gbi_cmds >= 3); // at least 3 of 4 are state setup
                    }
                    if (match) {
                        cached_seg6_addr = ext;
                        fprintf(stderr, "[DL#%d] Found seg6 file[%d] = 0x%08X (%d offsets validated)\n",
                                dl_n, f, ext, seg6_count);
                        break;
                    }
                }
            }

            // Rewrite segment[6] = 0 → decompressed file address
            if (cached_seg6_addr != 0) {
                for (int i = 0; i < 200; i++) {
                    uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
                    uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
                    uint8_t opcode = (w0 >> 24) & 0xFF;

                    if (opcode == 0xDB && ((w0 >> 16) & 0xFF) == 0x06) {
                        int seg_idx = (w0 & 0xFFFF) / 4;
                        if (seg_idx == 6 && w1 == 0) {
                            *(uint32_t*)(rdram + data_addr + i * 8 + 4) = cached_seg6_addr;
                        }
                    }
                    if (opcode == 0xDF) break;
                }
            }
        }
    }

    if (do_log) {
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            uint8_t opcode = (w0 >> 24) & 0xFF;

            if (opcode == 0xDB && ((w0 >> 16) & 0xFF) == 0x06) {
                int seg_idx = (w0 & 0xFFFF) / 4;
                if (seg_idx < 16) {
                    fprintf(stderr, "[DL#%d cmd%d] G_MOVEWORD segment[%d] = 0x%08X\n",
                            dl_n, i, seg_idx, w1);
                }
            }
            if (opcode == 0xDE) {
                fprintf(stderr, "[DL#%d cmd%d] G_DL target=0x%08X\n", dl_n, i, w1);
            }
            if (opcode == 0xDF) break;
        }
    }

    // Inject extendRDRAM GBI command: RT64 treats bit-31 addresses as extended
    // RDRAM (bypass DMA mask). Needed for decompressed NI files at 0x14XXXXXX.
    *(uint32_t*)(rdram + data_addr - 8) = 0x6400002C;
    *(uint32_t*)(rdram + data_addr - 4) = 0x00000001;
    uint32_t extended_data_addr = data_addr - 8;

    // Track the last SETCIMG address for VI_ORIGIN fixup.
    {
        int cimg_count = 0;
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            if (((w0 >> 24) & 0xFF) == 0xFF) {
                cimg_count++;
                if (cimg_count == 2) {
                    last_displayed_cfb = w1;
                    break;
                }
            }
            if (((w0 >> 24) & 0xFF) == 0xDF) break;
        }
    }

    app->state->rsp->reset();
    app->interpreter->loadUCodeGBI(task->t.ucode & 0x3FFFFFF, task->t.ucode_data & 0x3FFFFFF, true);
    app->processDisplayLists(app->core.RDRAM, extended_data_addr, 0, true);
}

void lod::renderer::RT64Context::update_screen() {
    static int us_n = 0; us_n++;

    // Fix VI_ORIGIN to match the SETCIMG address from the last DL.
    // The game's VI setup produces a mismatched origin (0x700280 vs 0x1DA800).
    // RT64 tracks framebuffers by SETCIMG address. VI_ORIGIN must point to
    // one of those addresses for RT64 to find and display the rendered frame.
    ultramodern::renderer::ViRegs* vi = ultramodern::renderer::get_vi_regs();
    vi->VI_ORIGIN_REG = last_displayed_cfb;

    if (us_n <= 5 || (us_n % 200 == 0)) {
        fprintf(stderr, "[VI_FIX] update_screen #%d VI_ORIGIN forced to 0x%06X\n",
                us_n, last_displayed_cfb);
    }

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
