/**
 * CVLoD Recomp - RT64 Graphics Integration
 *
 * This file implements the bridge between the ultramodern runtime and the RT64
 * renderer. Based on Zelda64Recomp's rt64_render_context.cpp
 */

#ifdef WITH_RT64

#ifndef HLSL_CPU
#define HLSL_CPU
#endif

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <thread>

#include "ultramodern/error_handling.hpp"
#include "ultramodern/events.hpp"
#include "ultramodern/input.hpp"
#include "ultramodern/renderer_context.hpp"
#include "ultramodern/rsp.hpp"
#include "ultramodern/threads.hpp"
#include "ultramodern/ultramodern.hpp"

// RT64 headers
#include "hardware.h"
#include "hle/rt64_application.h"

using namespace interop;

// SDL for window creation
#include <SDL.h>
#include <SDL_syswm.h>

static uint8_t *g_rdram = nullptr;
static SDL_Window *g_window = nullptr;
static std::atomic<bool> g_running{true};
static ultramodern::renderer::WindowHandle g_window_handle{};

// Global pointer to the current renderer context
static ultramodern::renderer::RendererContext *g_active_renderer = nullptr;

// RSP/DPC register storage (for non-VI registers)
static uint8_t DMEM[0x1000];
static uint8_t IMEM[0x1000];
static unsigned int MI_INTR_REG = 0;
static unsigned int DPC_START_REG = 0;
static unsigned int DPC_END_REG = 0;
static unsigned int DPC_CURRENT_REG = 0;
static unsigned int DPC_STATUS_REG = 0;
static unsigned int DPC_CLOCK_REG = 0;
static unsigned int DPC_BUFBUSY_REG = 0;
static unsigned int DPC_PIPEBUSY_REG = 0;
static unsigned int DPC_TMEM_REG = 0;

static void dummy_check_interrupts() {}

// Real RT64 RendererContext - following Zelda64Recomp's pattern
class RT64RendererContext : public ultramodern::renderer::RendererContext {
public:
  std::unique_ptr<RT64::Application> app;

  RT64RendererContext(uint8_t *rdram,
                      ultramodern::renderer::WindowHandle window_handle,
                      bool developer_mode) {
    printf("[GFX] Creating RT64RendererContext (rdram=%p, window=%p, view=%p)\n",
           (void*)rdram, window_handle.window, window_handle.view);
    g_rdram = rdram;
    g_active_renderer = this;

    if (!window_handle.window || !window_handle.view) {
      fprintf(stderr, "[GFX] ERROR: Invalid window handle!\n");
      setup_result = ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
      return;
    }

    // Use a dummy ROM header (like Zelda64Recomp)
    static unsigned char dummy_rom_header[0x40] = {0};

    // Set up the RT64 application core fields
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

    // MI/DPC registers
    appCore.MI_INTR_REG = &MI_INTR_REG;
    appCore.DPC_START_REG = &DPC_START_REG;
    appCore.DPC_END_REG = &DPC_END_REG;
    appCore.DPC_CURRENT_REG = &DPC_CURRENT_REG;
    appCore.DPC_STATUS_REG = &DPC_STATUS_REG;
    appCore.DPC_CLOCK_REG = &DPC_CLOCK_REG;
    appCore.DPC_BUFBUSY_REG = &DPC_BUFBUSY_REG;
    appCore.DPC_PIPEBUSY_REG = &DPC_PIPEBUSY_REG;
    appCore.DPC_TMEM_REG = &DPC_TMEM_REG;

    // VI registers - CRITICAL: Use ultramodern's VI registers!
    // ultramodern's VI thread updates these registers based on osViSetMode/osViSwapBuffer calls.
    // RT64 reads these to know the framebuffer address and screen state.
    ultramodern::renderer::ViRegs *vi_regs = ultramodern::renderer::get_vi_regs();
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

    // Set up the RT64 application configuration
    RT64::ApplicationConfiguration appConfig;
    appConfig.appId = "cvlod_recomp";
    appConfig.useConfigurationFile = false;

    // Create the RT64 application
    printf("[GFX] Creating RT64::Application...\n");
    fflush(stdout);
    try {
      app = std::make_unique<RT64::Application>(appCore, appConfig);
    } catch (const std::exception &e) {
      fprintf(stderr, "[GFX] Exception creating RT64::Application: %s\n", e.what());
      fflush(stderr);
      setup_result = ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
      return;
    } catch (...) {
      fprintf(stderr, "[GFX] Unknown exception creating RT64::Application\n");
      fflush(stderr);
      setup_result = ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
      return;
    }
    printf("[GFX] RT64::Application created successfully\n");
    fflush(stdout);

    // Set graphics API based on platform
#ifdef __APPLE__
    printf("[GFX] Setting Metal API\n");
    fflush(stdout);
    app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Metal;
#elif defined(_WIN32)
    app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::D3D12;
#else
    app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Vulkan;
#endif

    app->userConfig.developerMode = developer_mode;

    // Set up the RT64 application
    uint32_t thread_id = 0;
#ifdef _WIN32
    thread_id = window_handle.thread_id;
#endif

    printf("[GFX] Calling RT64 setup (thread_id=%u)...\n", thread_id);
    fflush(stdout);
    try {
      auto result = app->setup(thread_id);
      if (result != RT64::Application::SetupResult::Success) {
        fprintf(stderr, "[GFX] RT64 setup failed with error code %d\n", (int)result);
        setup_result = ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
        app = nullptr;
        return;
      }
      printf("[GFX] RT64 setup complete\n");
      setup_result = ultramodern::renderer::SetupResult::Success;
      chosen_api = ultramodern::renderer::GraphicsApi::Metal;
    } catch (const std::exception &e) {
      fprintf(stderr, "[GFX] Exception during RT64 setup: %s\n", e.what());
      setup_result = ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
      app = nullptr;
      return;
    } catch (...) {
      fprintf(stderr, "[GFX] Unknown exception during RT64 setup\n");
      setup_result = ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
      app = nullptr;
      return;
    }
  }

  ~RT64RendererContext() override { shutdown(); }

  bool valid() override { return app != nullptr; }

  bool update_config(
      const ultramodern::renderer::GraphicsConfig &old_config,
      const ultramodern::renderer::GraphicsConfig &new_config) override {
    return true;
  }

  void enable_instant_present() override {
    if (app) {
      // Enable the present early presentation mode for minimal latency
      app->enhancementConfig.presentation.mode =
          RT64::EnhancementConfiguration::Presentation::Mode::PresentEarly;
      app->updateEnhancementConfig();
    }
  }

  void send_dl(const OSTask *task) override {
    if (!app)
      return;

    printf("[GFX] send_dl: type=%d, ucode=0x%08X, data_ptr=0x%08X\n",
           task->t.type, (uint32_t)(uintptr_t)task->t.ucode,
           (uint32_t)(uintptr_t)task->t.data_ptr);

    try {
      // Reset RSP state before processing (critical for correct rendering)
      app->state->rsp->reset();

      // Load microcode and process display list
      // Use 0x3FFFFFF mask for proper N64 address translation (like Zelda64Recomp)
      app->interpreter->loadUCodeGBI(
          task->t.ucode & 0x3FFFFFF,
          task->t.ucode_data & 0x3FFFFFF,
          true);
      app->processDisplayLists(
          app->core.RDRAM,
          task->t.data_ptr & 0x3FFFFFF,
          0, true);
    } catch (const std::exception &e) {
      fprintf(stderr, "[GFX] Exception during processDisplayLists: %s\n",
              e.what());
    } catch (...) {
      fprintf(stderr, "[GFX] Unknown exception during processDisplayLists!\n");
    }
  }

  void update_screen() override {
    if (!app)
      return;

    static int update_count = 0;
    update_count++;
    if (update_count <= 5 || update_count % 300 == 0) {
      printf("[GFX] update_screen (call #%d)\n", update_count);
    }
    try {
      app->updateScreen();
    } catch (const std::exception &e) {
      fprintf(stderr, "[GFX] Exception during updateScreen: %s\n", e.what());
    } catch (...) {
      fprintf(stderr, "[GFX] Unknown exception during updateScreen!\n");
    }
  }

  void shutdown() override {
    g_running = false;
    g_active_renderer = nullptr;
    if (app) {
      app->end();
      app.reset();
    }
  }

  uint32_t get_display_framerate() const override {
    if (app) {
      return app->presentQueue->ext.sharedResources->swapChainRate;
    }
    return 60;
  }

  float get_resolution_scale() const override { return 1.0f; }
};

// Stub renderer context for when RT64 fails
class StubRendererContext : public ultramodern::renderer::RendererContext {
public:
  StubRendererContext() {
    setup_result = ultramodern::renderer::SetupResult::GraphicsDeviceNotFound;
    chosen_api = ultramodern::renderer::GraphicsApi::Auto;
  }
  bool valid() override { return false; }
  bool update_config(const ultramodern::renderer::GraphicsConfig&,
                     const ultramodern::renderer::GraphicsConfig&) override { return false; }
  void enable_instant_present() override {}
  void send_dl(const OSTask*) override {}
  void update_screen() override {}
  void shutdown() override {}
  uint32_t get_display_framerate() const override { return 60; }
  float get_resolution_scale() const override { return 1.0f; }
};

// Factory function for creating the render context
std::unique_ptr<ultramodern::renderer::RendererContext>
create_render_context(uint8_t *rdram,
                      ultramodern::renderer::WindowHandle window_handle,
                      bool developer_mode) {
  try {
    return std::make_unique<RT64RendererContext>(rdram, window_handle,
                                                 developer_mode);
  } catch (const std::exception &e) {
    fprintf(stderr, "[GFX] Exception in create_render_context: %s\n", e.what());
    return std::make_unique<StubRendererContext>();
  } catch (...) {
    fprintf(stderr, "[GFX] Unknown exception in create_render_context\n");
    return std::make_unique<StubRendererContext>();
  }
}

// RSP callbacks
// Note: Graphics tasks are routed via ultramodern's action_queue and handled by
// the GFX thread, which calls renderer_context->send_dl(). This callback is only
// for non-graphics tasks (audio, etc.)
void rsp_init_stub() { printf("[RSP] RT64 RSP init\n"); }

bool rsp_run_task_stub(uint8_t *rdram, const OSTask *task) {
  // GFX tasks are handled by ultramodern's gfx_thread via the action queue.
  // This callback is only invoked for non-GFX tasks (audio microcode, etc.)
  if (task->t.type == M_GFXTASK) {
    // This shouldn't happen - GFX tasks go through the action queue
    fprintf(stderr, "[RSP] WARNING: GFX task in run_task callback (should go through action queue)\n");
    return true;
  }

  // For audio tasks, we'd run the audio microcode here
  // For now, just acknowledge the task
  printf("[RSP] Non-GFX task type=%d\n", task->t.type);
  return true;
}

// Input poll helper
void input_poll_stub() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        exit(0);
      }
      break;
    }
  }
}

// Initialization function - only creates SDL window
extern "C" void init_graphics_system() {
  printf("[GFX] Initializing graphics system (SDL window)...\n");

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
    printf("[GFX] SDL_Init failed: %s\n", SDL_GetError());
    return;
  }

  g_window =
      SDL_CreateWindow("Castlevania: Legacy of Darkness - RT64",
                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
                           SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_METAL);

  if (!g_window) {
    printf("[GFX] SDL_CreateWindow failed: %s\n", SDL_GetError());
    return;
  }

#ifdef __APPLE__
  SDL_SysWMinfo wm_info;
  SDL_VERSION(&wm_info.version);
  SDL_GetWindowWMInfo(g_window, &wm_info);
  void *ns_window = wm_info.info.cocoa.window;

  g_window_handle.window = ns_window;
  SDL_MetalView view = SDL_Metal_CreateView(g_window);
  g_window_handle.view = SDL_Metal_GetLayer(view);
#elif defined(_WIN32)
  SDL_SysWMinfo wm_info;
  SDL_VERSION(&wm_info.version);
  if (SDL_GetWindowWMInfo(g_window, &wm_info)) {
    g_window_handle.window = wm_info.info.win.window;
    g_window_handle.thread_id = GetCurrentThreadId();
  }
#endif

  // Register callbacks for ultramodern
  ultramodern::rsp::callbacks_t rsp_callbacks{};
  rsp_callbacks.init = rsp_init_stub;
  rsp_callbacks.run_task = rsp_run_task_stub;

  ultramodern::renderer::callbacks_t renderer_callbacks{};
  renderer_callbacks.create_render_context = create_render_context;

  ultramodern::set_callbacks(rsp_callbacks, renderer_callbacks, {}, {}, {}, {},
                             {}, {});

  g_running = true;
  printf("[GFX] Graphics system initialized (SDL window created)\n");
}

extern "C" void setup_rt64_with_rdram(uint8_t *rdram) {
  printf("[GFX] Setting RDRAM pointer for RT64: %p\n", (void *)rdram);
  g_rdram = rdram;
}

extern "C" void cleanup_graphics_system() {
  if (g_window) {
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
  }
  SDL_Quit();
}

extern "C" void process_frame() { input_poll_stub(); }

extern "C" bool is_graphics_running() { return g_running; }

extern "C" void *get_native_window_handle() { return (void *)&g_window_handle; }

extern "C" void set_framebuffer_address(uint32_t addr) {}

#else // !WITH_RT64

#include <cstdio>
#include <cstdint>

extern "C" void init_graphics_system() { printf("[GFX] RT64 disabled\n"); }
extern "C" void setup_rt64_with_rdram(uint8_t *rdram) { (void)rdram; }
extern "C" void cleanup_graphics_system() {}
extern "C" void process_frame() {}
extern "C" bool is_graphics_running() { return false; }
extern "C" void *get_native_window_handle() { return nullptr; }
extern "C" void set_framebuffer_address(uint32_t addr) { (void)addr; }

#endif // WITH_RT64
