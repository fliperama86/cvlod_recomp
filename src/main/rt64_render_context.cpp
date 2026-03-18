#include <memory>
#include <cstring>
#include <cstdio>
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

static uint32_t last_displayed_cfb = 0x00200000; // track the displayed cfb address (default: FB0)
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

    // No DL overrides — game commands pass through unmodified.

    // === Game state diagnostic ===
    if (dl_n <= 5 || dl_n % 100 == 0) {
        uint32_t root_obj = 0x0031AC78; // 0x8031AC78 & 0x7FFFFF
        uint32_t scene_arg = *(uint32_t*)(rdram + root_obj + 0x24);
        uint32_t exec_flags = *(uint32_t*)(rdram + 0x001CABC8);
        uint32_t ni_sys_ptr = *(uint32_t*)(rdram + 0x001CAC1C);
        uint32_t guard_val = *(uint32_t*)(rdram + 0x000C671C); // event_struct+0x89C
        fprintf(stderr, "[STATE] DL#%d scene_arg=0x%08X exec_flags=0x%08X ni_sys=0x%08X guard=0x%08X\n",
                dl_n, scene_arg, exec_flags, ni_sys_ptr, guard_val);
    }

    // === Read game input state (diagnostic only, no injection) ===
    // The game reads input from 3 locations:
    //   1. PIF buffer at 0x8013ED70 (raw SI DMA data)
    //   2. Parsed pad data at 0x800EFBA0 (extracted by func_80098BD4)
    //   3. Game input state at 0x801C87F4 (+2=prev_buttons, +4=new_buttons, +6=stick)
    // We inject at all 3 levels + handle button transition detection.
    {
        // Read game input state (diagnostic only — no write)
        uint32_t pad_parsed = 0x000EFBA0; // parsed pad data
        uint32_t input_state = 0x001C87F4; // game input state
        uint16_t parsed_buttons = *(uint16_t*)(rdram + pad_parsed);
        uint16_t game_current = *(uint16_t*)(rdram + input_state + 2);
        uint16_t game_new = *(uint16_t*)(rdram + input_state + 4);

        if (dl_n <= 10 || dl_n == 20 || dl_n == 50 || dl_n == 100) {
            fprintf(stderr, "[PAD] DL#%d parsed=0x%04X game_cur=0x%04X game_new=0x%04X\n",
                    dl_n, parsed_buttons, game_current, game_new);
        }
    }

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

        if (dl_n <= 5) {
            fprintf(stderr, "[SEG6] DL#%d has_seg6_zero=%d seg6_count=%d cached=0x%08X\n",
                    dl_n, has_seg6_zero, seg6_count, cached_seg6_addr);
        }
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
                        if (f == 97) {
                            fprintf(stderr, "[SEG6] file[97] phys=0x%08X addr2=0x%08X gbi_cmds=%d ops:",
                                    phys, addr2, gbi_cmds);
                            for (int k = 0; k < 4; k++) {
                                uint32_t w = *(uint32_t*)(rdram + addr2 + k * 8);
                                fprintf(stderr, " 0x%02X", (w >> 24) & 0xFF);
                            }
                            fprintf(stderr, "\n");
                        }
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

    // Validate G_DL branches: allow any target that points to valid GBI data.
    // Previous approach (whitelist 2 KSEG0 addresses) was too restrictive and
    // caused all geometry to collapse to screen center (missing matrix sub-DLs).
    for (int i = 0; i < 200; i++) {
        uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
        uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
        uint8_t opcode = (w0 >> 24) & 0xFF;
        if (opcode == 0xDE) {
            bool safe = false;
            uint8_t first_op = 0;

            // KSEG0 addresses (0x80XXXXXX) → strip prefix, check valid RDRAM range
            if (!safe && (w1 & 0xFF000000) == 0x80000000) {
                uint32_t phys = w1 & 0x00FFFFFF;
                if (phys < 0x00800000) {
                    first_op = (*(uint32_t*)(rdram + phys) >> 24) & 0xFF;
                    safe = is_valid_gbi_opcode(first_op);
                }
            }
            // Physical RDRAM addresses (< 8MB)
            if (!safe && w1 > 0 && w1 < 0x00800000) {
                first_op = (*(uint32_t*)(rdram + w1) >> 24) & 0xFF;
                safe = is_valid_gbi_opcode(first_op);
            }
            // Segment 6 sub-DLs (0x06XXXXXX) — resolve via cached segment base
            if (!safe && ((w1 >> 24) & 0x0F) == 0x06 && cached_seg6_addr != 0) {
                uint32_t phys = (cached_seg6_addr & 0x7FFFFFFF) + (w1 & 0x00FFFFFF);
                first_op = (*(uint32_t*)(rdram + phys) >> 24) & 0xFF;
                safe = is_valid_gbi_opcode(first_op);
            }

            if (!safe) {
                if (dl_n <= 10) {
                    fprintf(stderr, "[DL#%d cmd%d] NOP G_DL target=0x%08X (first_op=0x%02X)\n",
                            dl_n, i, w1, first_op);
                }
                *(uint32_t*)(rdram + data_addr + i * 8) = 0x00000000;
                *(uint32_t*)(rdram + data_addr + i * 8 + 4) = 0x00000000;
            } else if (dl_n <= 5) {
                fprintf(stderr, "[DL#%d cmd%d] ALLOW G_DL target=0x%08X (first_op=0x%02X)\n",
                        dl_n, i, w1, first_op);
            }
        }
        if (opcode == 0xDF) break;
    }

    // Log original SETCIMG addresses (before any patching).
    if (dl_n <= 10) {
        int cimg_count = 0;
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            uint8_t op = (w0 >> 24) & 0xFF;
            if (op == 0xFF) {
                cimg_count++;
                fprintf(stderr, "[DL#%d] ORIGINAL SETCIMG #%d addr=0x%08X\n", dl_n, cimg_count, w1);
            }
            if (op == 0xDF) break;
        }
    }

    // No DL overrides — game's original commands pass through unmodified.

    // === DISABLED: SETCOMBINE patching in seg6 sub-DLs ===
    // Was forcing PRIMITIVE color output, hiding all real textures.
    // Let the game's original combiners through for real rendering.

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

    // Track SETCIMG addresses for VI_ORIGIN fixup.
    // In Castlevania LoD, the DL structure is:
    //   1st SETCIMG = Z-buffer address (0x1DA800) — used for depth clear
    //   2nd SETCIMG = color buffer address (0x200000) — actual scene render
    // We want to display the color buffer (2nd SETCIMG).
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

    // ── Projection Matrix Dump (DL#4 only) ─────────────────────────
    // The DL loads projection matrix via MTX command. Dump the matrix data
    // to understand why vertices collapse to screen center.
    if (dl_n == 4) {
        // Scan DL for first MTX projection command: 0xDA38000[57] (bit2=projection)
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            uint8_t op = (w0 >> 24) & 0xFF;
            if (op == 0xDA) {
                uint8_t params = w0 & 0xFF;
                bool is_proj = (params & 0x04) != 0;
                uint32_t mtx_addr = w1 & 0x3FFFFF; // strip KSEK0
                if (is_proj && mtx_addr < 0x00800000) {
                    auto* mtx = reinterpret_cast<RT64::FixedMatrix*>(rdram + mtx_addr);
                    auto m = mtx->toMatrix4x4();
                    fprintf(stderr, "[MTX] DL#4 PROJECTION at rdram+0x%06X (raw addr 0x%08X):\n", mtx_addr, w1);
                    for (int r = 0; r < 4; r++) {
                        fprintf(stderr, "[MTX]   [%d] %12.6f %12.6f %12.6f %12.6f\n",
                                r, (float)m[r][0], (float)m[r][1], (float)m[r][2], (float)m[r][3]);
                    }
                    // Also dump first modelview matrix (next MTX with bit2=0)
                    break;
                }
            }
            if (op == 0xDF) break;
        }
        // Dump first modelview matrix
        for (int i = 0; i < 200; i++) {
            uint32_t w0 = *(uint32_t*)(rdram + data_addr + i * 8);
            uint32_t w1 = *(uint32_t*)(rdram + data_addr + i * 8 + 4);
            uint8_t op = (w0 >> 24) & 0xFF;
            if (op == 0xDA) {
                uint8_t params = w0 & 0xFF;
                bool is_proj = (params & 0x04) != 0;
                uint32_t mtx_addr = w1 & 0x3FFFFF;
                if (!is_proj && mtx_addr < 0x00800000) {
                    auto* mtx = reinterpret_cast<RT64::FixedMatrix*>(rdram + mtx_addr);
                    auto m = mtx->toMatrix4x4();
                    fprintf(stderr, "[MTX] DL#4 MODELVIEW at rdram+0x%06X (raw addr 0x%08X):\n", mtx_addr, w1);
                    for (int r = 0; r < 4; r++) {
                        fprintf(stderr, "[MTX]   [%d] %12.6f %12.6f %12.6f %12.6f\n",
                                r, (float)m[r][0], (float)m[r][1], (float)m[r][2], (float)m[r][3]);
                    }
                    break;
                }
            }
            if (op == 0xDF) break;
        }
    }

    // ── DL Binary Dump (first 20 frames) ─────────────────────────────
    if (dl_n <= 20) {
        // Count commands
        int cmd_count = 0;
        for (int c = 0; c < 200; c++) {
            uint8_t op = (*(uint32_t*)(rdram + data_addr + c * 8) >> 24) & 0xFF;
            cmd_count++;
            if (op == 0xDF) break;
        }
        char path[256];
        snprintf(path, sizeof(path), "/tmp/lod_dl_%03d.bin", dl_n);
        FILE* f = fopen(path, "wb");
        if (f) {
            // Header: "LOD_DL\0\0" + dl_n + data_addr + cmd_count
            const char magic[8] = {'L','O','D','_','D','L',0,0};
            fwrite(magic, 1, 8, f);
            uint32_t hdr[3] = { (uint32_t)dl_n, data_addr, (uint32_t)cmd_count };
            fwrite(hdr, 4, 3, f);
            // Raw command bytes
            fwrite(rdram + data_addr, 8, cmd_count, f);
            fclose(f);
            if (dl_n <= 5) fprintf(stderr, "[DUMP] DL#%d → %s (%d cmds)\n", dl_n, path, cmd_count);
        }
    }

    app->state->rsp->reset();
    app->interpreter->loadUCodeGBI(task->t.ucode & 0x3FFFFFF, task->t.ucode_data & 0x3FFFFFF, true);
    app->state->extended.extendRDRAM = true;  // Force extended RDRAM for NI file access
    app->processDisplayLists(app->core.RDRAM, extended_data_addr, 0, true);

    // ── FB RDRAM Pixel Inspection ────────────────────────────────────
    // Check if the FB region in RDRAM has any non-zero pixels.
    // Note: RT64 renders to GPU, so RDRAM FB may not reflect GPU output.
    // But the game may write to FB directly for 2D effects.
    if (dl_n <= 20) {
        uint32_t fb_addr = 0x1DA800; // FB1
        int nonzero = 0;
        uint16_t sample_colors[8] = {};
        int sample_idx = 0;
        for (int p = 0; p < 320 * 240; p++) {
            uint16_t pixel = *(uint16_t*)(rdram + fb_addr + p * 2);
            if (pixel != 0) {
                nonzero++;
                if (sample_idx < 8) sample_colors[sample_idx++] = pixel;
            }
        }
        if (dl_n <= 10 || nonzero > 0) {
            fprintf(stderr, "[FB#%d] RDRAM FB1 non-zero pixels: %d/%d",
                    dl_n, nonzero, 320 * 240);
            if (nonzero > 0) {
                fprintf(stderr, "  samples:");
                for (int s = 0; s < sample_idx && s < 4; s++)
                    fprintf(stderr, " 0x%04X", sample_colors[s]);
            }
            fprintf(stderr, "\n");
        }
    }

    // Dump workload state + vertex screen positions after DL processing
    if (dl_n <= 20) {
        auto& wq = *app->workloadQueue;
        auto& workload = wq.workloads[wq.previousWriteCursor()];
        auto& dd = workload.drawData;
        fprintf(stderr, "[WL#%d] fbPairs=%d faceIdx=%zu posFloats=%zu viewXforms=%zu\n",
                dl_n, workload.fbPairCount, dd.faceIndices.size(), dd.posFloats.size(), dd.viewTransforms.size());

        // Also dump to file for offline analysis
        char wl_path[256];
        snprintf(wl_path, sizeof(wl_path), "/tmp/lod_workload_%03d.txt", dl_n);
        FILE* wl_file = fopen(wl_path, "w");

        for (int fp = 0; fp < workload.fbPairCount && fp < 8; fp++) {
            auto& fbp = workload.fbPairs[fp];
            fprintf(stderr, "[WL#%d]   fb[%d] colorAddr=0x%06X projCount=%d calls=%d fillOnly=%d\n",
                    dl_n, fp, fbp.colorImage.address, fbp.projectionCount, fbp.gameCallCount, (int)fbp.fillRectOnly);
            fprintf(stderr, "[WL#%d]   fb[%d] colorRect=(%d,%d,%d,%d) empty=%d\n",
                    dl_n, fp, fbp.drawColorRect.ulx, fbp.drawColorRect.uly,
                    fbp.drawColorRect.lrx, fbp.drawColorRect.lry, (int)fbp.drawColorRect.isEmpty());
            if (wl_file) {
                fprintf(wl_file, "fb[%d] colorAddr=0x%06X projCount=%d calls=%d fillOnly=%d\n",
                        fp, fbp.colorImage.address, fbp.projectionCount, fbp.gameCallCount, (int)fbp.fillRectOnly);
                fprintf(wl_file, "fb[%d] colorRect=(%d,%d,%d,%d) empty=%d\n",
                        fp, fbp.drawColorRect.ulx, fbp.drawColorRect.uly,
                        fbp.drawColorRect.lrx, fbp.drawColorRect.lry, (int)fbp.drawColorRect.isEmpty());
            }
            for (int pj = 0; pj < fbp.projectionCount && pj < 6; pj++) {
                auto& proj = fbp.projections[pj];
                fprintf(stderr, "[WL#%d]     proj[%d] type=%d xformIdx=%d calls=%d\n",
                        dl_n, pj, (int)proj.type, proj.transformsIndex, proj.gameCallCount);
                if (wl_file) {
                    fprintf(wl_file, "  proj[%d] type=%d xformIdx=%d calls=%d\n",
                            pj, (int)proj.type, proj.transformsIndex, proj.gameCallCount);
                }
            }
        }
        // Dump vertex data for geometry DLs (DL#4+ have geometry from seg6)
        if (dd.posFloats.size() > 0) {
            size_t max_verts = std::min((size_t)10, dd.posFloats.size() / 3);
            for (size_t vi = 0; vi < max_verts; vi++) {
                fprintf(stderr, "[WL#%d] posFloats[%zu]=(%.1f, %.1f, %.1f)\n",
                        dl_n, vi, dd.posFloats[vi*3], dd.posFloats[vi*3+1], dd.posFloats[vi*3+2]);
            }
        }
        if (dd.posScreen.size() > 0) {
            size_t max_screen = std::min((size_t)10, dd.posScreen.size());
            for (size_t vi = 0; vi < max_screen; vi++) {
                auto& ps = dd.posScreen[vi];
                fprintf(stderr, "[WL#%d] posScreen[%zu]=(%.1f, %.1f, %.1f)\n",
                        dl_n, vi, (float)ps.x, (float)ps.y, (float)ps.z);
            }
        }
        if (wl_file) {
            fprintf(wl_file, "drawData faceIndices=%zu posFloats=%zu posScreen=%zu viewXforms=%zu\n",
                    dd.faceIndices.size(), dd.posFloats.size(), dd.posScreen.size(), dd.viewTransforms.size());
            fclose(wl_file);
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
    // Use game's VI_ORIGIN from osViSwapBuffer, but ensure NTSC registers are set.
    // RT64 tracks framebuffers by SETCIMG address. VI_ORIGIN must point to
    // one of those addresses + width*2 for RT64 to find the rendered frame.
    uint32_t game_origin = vi->VI_ORIGIN_REG & 0xFFFFFF;

    // Hardcode NTSC timing registers (game may not set these correctly early on).
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

    // Use the last SETCIMG from the DL as VI_ORIGIN (+ 1 row offset).
    // This ensures RT64 finds the matching framebuffer pair.
    // Fall back to game_origin if no SETCIMG was tracked.
    if (last_displayed_cfb != 0) {
        vi->VI_ORIGIN_REG = last_displayed_cfb + 640;
    } else if (game_origin > 0) {
        vi->VI_ORIGIN_REG = game_origin;
    } else {
        vi->VI_ORIGIN_REG = 0x1DA800 + 640;
    }
    if (us_n <= 5 || us_n % 200 == 0) {
        fprintf(stderr, "[VI#%d] game_origin=0x%06X displayed_cfb=0x%06X VI_ORIGIN=0x%06X\n",
                us_n, game_origin, last_displayed_cfb, vi->VI_ORIGIN_REG);
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
