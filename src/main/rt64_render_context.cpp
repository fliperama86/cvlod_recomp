#include <memory>
#include <cstring>

#ifndef HLSL_CPU
#define HLSL_CPU
#endif
#include "hle/rt64_application.h"
#include "hle/rt64_workload.h"
#include "hle/rt64_vi.h"

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
static uint32_t cached_seg6_addr = 0;             // segment 6 base for NI file data

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
        // (cached_seg6_addr is at file scope for use in injected sub-DL)

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

    // NOP all G_DL branches except the two known-safe KSEG0 targets that
    // appear in every DL (0x800B0090 and 0x800AE958). Everything else
    // (segment 6 sub-DLs, segment 0 sub-DL at 0x000B2F80) can hang.
    for (int i = 0; i < 200; i++) {
        uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
        uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
        uint8_t opcode = (w0 >> 24) & 0xFF;
        if (opcode == 0xDE) {
            // Allow known-safe sub-DLs. With extendRDRAM forced, segment 6 DLs
            // starting with valid GBI opcodes should work now.
            bool safe = (w1 == 0x800B0090 || w1 == 0x800AE958);
            // Allow segment 6 sub-DLs that start with valid state-setup opcodes
            if (!safe && ((w1 >> 24) & 0x0F) == 0x06 && cached_seg6_addr != 0) {
                uint32_t phys = (cached_seg6_addr & 0x7FFFFFFF) + (w1 & 0x00FFFFFF);
                uint8_t first_op = (*(uint32_t*)(rdram + phys) >> 24) & 0xFF;
                safe = (first_op == 0xE2 || first_op == 0xE3 || first_op == 0xE7 || first_op == 0xD9);
            }
            if (!safe) {
                if (dl_n <= 5) {
                    fprintf(stderr, "[DL#%d cmd%d] NOP G_DL target=0x%08X\n", dl_n, i, w1);
                }
                *(uint32_t*)(rdram + data_addr + i * 8) = 0x00000000;
                *(uint32_t*)(rdram + data_addr + i * 8 + 4) = 0x00000000;
            }
        }
        if (opcode == 0xDF) break;
    }

    // Redirect 2nd SETCIMG to FB1 + override 2nd fill color to blue.
    {
        int cimg_count = 0, f7_count = 0;
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint8_t op = (w0 >> 24) & 0xFF;
            if (op == 0xFF) {
                cimg_count++;
                if (cimg_count == 2) {
                    *(uint32_t*)(rdram + data_addr + i * 8 + 4) = 0x1DA800;
                }
            }
            if (op == 0xF7) {
                f7_count++;
                if (f7_count == 2) {
                    *(uint32_t*)(rdram + data_addr + i * 8 + 4) = 0x001F001F; // blue
                }
            }
            // Override SETPRIMCOLOR to red + SETCOMBINE to output PRIMITIVE
            if (op == 0xFA) {
                *(uint32_t*)(rdram + data_addr + i * 8 + 4) = 0xFF0000FF;
            }
            if (op == 0xFC) {
                *(uint32_t*)(rdram + data_addr + i * 8) = 0xFC000000;
                *(uint32_t*)(rdram + data_addr + i * 8 + 4) = 0x000186C3;
            }
            if (op == 0xDF) break;
        }
    }

    // Patch SETCOMBINE inside segment 6 sub-DLs to output solid PRIMITIVE color.
    // Only patch when seg6 is resolved (DL#4+).
    if (cached_seg6_addr != 0) {
        uint32_t seg6_phys = cached_seg6_addr & 0x7FFFFFFF;
        // Patch all segment 6 G_DL targets that start with valid GBI opcodes
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            uint8_t op = (w0 >> 24) & 0xFF;
            if (op == 0xDE && ((w1 >> 24) & 0x0F) == 0x06) {
                uint32_t phys = seg6_phys + (w1 & 0x00FFFFFF);
                // Scan up to 64 commands in this sub-DL
                for (int j = 0; j < 64; j++) {
                    uint32_t* cmd = reinterpret_cast<uint32_t*>(rdram + phys + j * 8);
                    uint8_t sub_op = (cmd[0] >> 24) & 0xFF;
                    if (sub_op == 0xFC) {
                        cmd[0] = 0xFC000000;
                        cmd[1] = 0x000186C3;  // PRIMITIVE
                        if (dl_n <= 5) fprintf(stderr, "[PATCH] DL#%d patched SETCOMBINE at phys 0x%X+%d\n", dl_n, phys, j);
                    }
                    if (sub_op == 0xDF) break;
                }
            }
            if (op == 0xDF) break;
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
    app->state->extended.extendRDRAM = true;  // Force extended RDRAM for NI file access
    app->processDisplayLists(app->core.RDRAM, extended_data_addr, 0, true);

    // Dump workload state + vertex screen positions after DL processing
    if (dl_n <= 5) {
        auto& wq = *app->workloadQueue;
        auto& workload = wq.workloads[wq.previousWriteCursor()];
        auto& dd = workload.drawData;
        fprintf(stderr, "[WL#%d] fbPairs=%d faceIdx=%zu posFloats=%zu viewXforms=%zu\n",
                dl_n, workload.fbPairCount, dd.faceIndices.size(), dd.posFloats.size(), dd.viewTransforms.size());
        for (int fp = 0; fp < workload.fbPairCount && fp < 4; fp++) {
            auto& fbp = workload.fbPairs[fp];
            fprintf(stderr, "[WL#%d]   fb[%d] colorAddr=0x%06X projCount=%d calls=%d fillOnly=%d\n",
                    dl_n, fp, fbp.colorImage.address, fbp.projectionCount, fbp.gameCallCount, (int)fbp.fillRectOnly);
            fprintf(stderr, "[WL#%d]   fb[%d] colorRect=(%d,%d,%d,%d) empty=%d\n",
                    dl_n, fp, fbp.drawColorRect.ulx, fbp.drawColorRect.uly,
                    fbp.drawColorRect.lrx, fbp.drawColorRect.lry, (int)fbp.drawColorRect.isEmpty());
            for (int pj = 0; pj < fbp.projectionCount && pj < 6; pj++) {
                auto& proj = fbp.projections[pj];
                fprintf(stderr, "[WL#%d]     proj[%d] type=%d xformIdx=%d calls=%d\n",
                        dl_n, pj, (int)proj.type, proj.transformsIndex, proj.gameCallCount);
            }
        }
        // Dump posFloats (raw vertex positions as sent to RT64) and posScreen
        if (dl_n == 4) {
            for (size_t vi = 0; vi < 3 && vi*3+2 < dd.posFloats.size(); vi++) {
                fprintf(stderr, "[WL#%d] posFloats[%zu]=(%.1f, %.1f, %.1f)\n",
                        dl_n, vi, dd.posFloats[vi*3], dd.posFloats[vi*3+1], dd.posFloats[vi*3+2]);
            }
        }
        if (dl_n == 4) {
            for (size_t vi = 0; vi < dd.posScreen.size() && vi < 6; vi++) {
                auto& ps = dd.posScreen[vi];
                fprintf(stderr, "[WL#%d] posScreen[%zu]=(%.1f, %.1f, %.1f)\n",
                        dl_n, vi, (float)ps.x, (float)ps.y, (float)ps.z);
            }
        }
    }
}

void lod::renderer::RT64Context::update_screen() {
    static int us_n = 0; us_n++;

    // Fix VI_ORIGIN to match the SETCIMG address from the last DL.
    // The game's VI setup produces a mismatched origin (0x700280 vs 0x1DA800).
    // RT64 tracks framebuffers by SETCIMG address. VI_ORIGIN must point to
    // one of those addresses for RT64 to find and display the rendered frame.
    ultramodern::renderer::ViRegs* vi = ultramodern::renderer::get_vi_regs();
    // Snapshot game's origin, then hardcode ALL registers and call
    // app->updateScreen() (the approach that worked for FB1).
    uint32_t game_origin = vi->VI_ORIGIN_REG & 0xFFFFFF;

    vi->VI_STATUS_REG    = 0x0000030A;
    vi->VI_WIDTH_REG     = 320;
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
    // Always display FB1 — both fill-rects and game geometry render here now.
    vi->VI_ORIGIN_REG    = 0x1DA800 + 640;

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
