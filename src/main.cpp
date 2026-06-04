/**
 * CVLoD Recomp - Castlevania: Legacy of Darkness Recompilation
 *
 * Main entry point for the recompiled game runtime.
 */

#include <atomic>
#include <cassert>
#include <chrono>
#include <csetjmp>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <sys/mman.h>
#include <thread>
#include <vector>
#include <cxxabi.h>
#include <execinfo.h>

// Global terminate handler to catch unhandled exceptions
static void custom_terminate_handler() {
    fprintf(stderr, "\n=== TERMINATE HANDLER CALLED ===\n");
    fflush(stderr);

    // Print current exception if any
    std::exception_ptr eptr = std::current_exception();
    if (eptr) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception& e) {
            fprintf(stderr, "Unhandled std::exception: %s\n", e.what());
        } catch (...) {
            fprintf(stderr, "Unhandled unknown exception\n");
        }
    }

    // Print backtrace
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
    fprintf(stderr, "Backtrace (%d frames):\n", frames);
    for (int i = 0; i < frames; i++) {
        fprintf(stderr, "  %s\n", strs[i]);
    }
    free(strs);
    fflush(stderr);

    std::abort();
}

#ifndef MOODYCAMEL_DELETE_FUNCTION
#define MOODYCAMEL_DELETE_FUNCTION = delete
#endif
#include "lightweightsemaphore.h"

#include "hardware.h"
#include "recomp.h"
#include "ultramodern/renderer_context.hpp"
#include "ultramodern/ultramodern.hpp"
#include <SDL.h>

// Global exit flag and semaphore required by ultramodern
std::atomic_bool exited{false};
moodycamel::LightweightSemaphore graphics_shutdown_ready;

// Forward declarations from funcs.cpp
extern void init_function_table();
extern void add_address_aliases();
extern recomp_func_t *cvlod_get_function(int32_t vram);
extern void set_trace_lookups(bool enable);
extern void set_libultra_wrapper(int32_t vram, recomp_func_t *wrapper);

// Forward declarations for libultra wrapper functions (from libultra_wrappers.cpp)
extern void install_libultra_wrappers();
extern "C" void func_8008A690(uint8_t *rdram, recomp_context *ctx);  // osViSwapBuffer
extern "C" void func_80083120(uint8_t *rdram, recomp_context *ctx);  // osViSetMode
extern "C" void func_8008BD70(uint8_t *rdram, recomp_context *ctx);  // osViBlack
extern "C" void func_8007FFC0(uint8_t *rdram, recomp_context *ctx);  // osSpTaskLoad
extern "C" void func_800802B0(uint8_t *rdram, recomp_context *ctx);  // osSpTaskStartGo
extern "C" void func_80089EC0(uint8_t *rdram, recomp_context *ctx);  // osCreateMesgQueue
extern "C" void func_8007F2E0(uint8_t *rdram, recomp_context *ctx);  // osSendMesg
extern "C" void func_8007F860(uint8_t *rdram, recomp_context *ctx);  // osRecvMesg
extern "C" void func_80081D80(uint8_t *rdram, recomp_context *ctx);  // osInitialize
extern "C" void func_800812C0(uint8_t *rdram, recomp_context *ctx);  // osCreateThread
extern "C" void func_80090490(uint8_t *rdram, recomp_context *ctx);  // osStartThread
extern "C" void L_8008C4E0(uint8_t *rdram, recomp_context *ctx);     // osViSetEvent

// Forward declaration from game_thread_stubs.cpp
extern void install_game_thread_stubs();

// Forward declarations from rt64_context.cpp
extern "C" void init_graphics_system();
extern "C" void setup_rt64_with_rdram(uint8_t *rdram);
extern "C" void cleanup_graphics_system();
extern "C" void process_frame();
extern "C" bool is_graphics_running();
extern "C" void *get_native_window_handle();

// Forward declarations for game functions
extern "C" void func_80000460(uint8_t *rdram, recomp_context *ctx);
extern "C" void game_main(uint8_t *rdram, recomp_context *ctx);
extern "C" void func_8001783C(
    uint8_t *rdram,
    recomp_context *ctx); // Idle thread (creates main thread, then loops)
extern "C" void
func_800178D0(uint8_t *rdram,
              recomp_context *ctx); // Main game loop thread entry

// Global RDRAM allocation
static uint8_t *g_rdram = nullptr;
static constexpr size_t RDRAM_SIZE = 8 * 1024 * 1024; // 8MB RDRAM

static std::atomic<bool> g_game_started{false};

// ROM data
static std::vector<uint8_t> g_rom;

// Jump buffer for recovery
static sigjmp_buf g_jump_buf;
static volatile bool g_in_game_code = false;
static volatile int g_crash_count = 0;
static constexpr int MAX_CRASHES = 100;

// Signal handler for debugging crashes
static void signal_handler(int sig, siginfo_t *info, void *context) {
  g_crash_count++;

  fprintf(stderr, "\n*** Signal %d received (crash #%d) ***\n", sig,
          g_crash_count);

  if (info) {
    void *fault_addr = info->si_addr;
    fprintf(stderr, "Fault address: %p\n", fault_addr);

    // Check if this looks like an N64 hardware address
    uint64_t addr = (uint64_t)(uintptr_t)fault_addr;

    // The MEM_* macros calculate: rdram + (n64_addr - 0xFFFFFFFF80000000)
    // So fault_addr = rdram + offset, where offset = n64_addr - 0x80000000
    // If offset is huge (> 8MB), it's likely a hardware access

    ptrdiff_t offset = (uint8_t *)fault_addr - g_rdram;
    if (offset < 0 || offset > 0x800000) {
      // Calculate what N64 address this would be
      uint64_t n64_addr = 0x80000000ULL + offset;
      fprintf(stderr, "Likely N64 address: 0x%08llX\n",
              (unsigned long long)(n64_addr & 0xFFFFFFFF));

      if ((n64_addr & 0xFF000000) == 0xA4000000) {
        fprintf(stderr, "This is a HARDWARE REGISTER access!\n");
        if ((n64_addr & 0x00F00000) == 0x00000000)
          fprintf(stderr, "  -> RSP registers\n");
        else if ((n64_addr & 0x00F00000) == 0x00100000)
          fprintf(stderr, "  -> RDP registers\n");
        else if ((n64_addr & 0x00F00000) == 0x00300000)
          fprintf(stderr, "  -> MI registers\n");
        else if ((n64_addr & 0x00F00000) == 0x00400000)
          fprintf(stderr, "  -> VI registers\n");
        else if ((n64_addr & 0x00F00000) == 0x00500000)
          fprintf(stderr, "  -> AI registers\n");
        else if ((n64_addr & 0x00F00000) == 0x00600000)
          fprintf(stderr, "  -> PI registers\n");
        else if ((n64_addr & 0x00F00000) == 0x00700000)
          fprintf(stderr, "  -> RI registers\n");
        else if ((n64_addr & 0x00F00000) == 0x00800000)
          fprintf(stderr, "  -> SI registers\n");
      } else if ((n64_addr & 0xF0000000) == 0xB0000000) {
        fprintf(stderr, "This is a CARTRIDGE ROM access!\n");
      }
    }
  }

  if (g_in_game_code && g_crash_count < MAX_CRASHES) {
    fprintf(stderr, "Attempting to continue...\n\n");
    siglongjmp(g_jump_buf, 1);
  }

  fprintf(stderr, "Cannot continue. Exiting.\n");
  exit(1);
}

bool load_rom(const char *path) {
  printf("Loading ROM: %s\n", path);

  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open ROM file\n");
    return false;
  }

  size_t size = file.tellg();
  file.seekg(0, std::ios::beg);

  g_rom.resize(size);
  file.read(reinterpret_cast<char *>(g_rom.data()), size);

  printf("ROM loaded: %zu bytes (%.1f MB)\n", size, size / (1024.0 * 1024.0));

  // Verify N64 ROM header
  if (size >= 4) {
    uint32_t magic =
        (g_rom[0] << 24) | (g_rom[1] << 16) | (g_rom[2] << 8) | g_rom[3];
    if (magic == 0x80371240) {
      printf("ROM format: Big-endian (z64)\n");
    } else if (magic == 0x40123780) {
      printf("ROM format: Byte-swapped (v64) - may need conversion\n");
    } else if (magic == 0x37804012) {
      printf("ROM format: Little-endian (n64) - may need conversion\n");
    } else {
      printf("ROM format: Unknown (magic: 0x%08X)\n", magic);
    }
  }

  return true;
}

void copy_rom_to_rdram(uint32_t rdram_addr, uint32_t rom_offset,
                       size_t length) {
  // Simulate DMA from ROM to RDRAM
  if (rom_offset + length > g_rom.size()) {
    fprintf(stderr, "ROM read out of bounds\n");
    return;
  }

  uint32_t offset = rdram_addr & 0x7FFFFF; // Mask to RDRAM size
  if (offset + length > RDRAM_SIZE) {
    fprintf(stderr, "RDRAM write out of bounds\n");
    return;
  }

  memcpy(g_rdram + offset, g_rom.data() + rom_offset, length);
}

// BSS region from the boot code
constexpr uint32_t BSS_START = 0x800C1520;
constexpr uint32_t BSS_SIZE = 0x80350; // 524,112 bytes
constexpr uint32_t STACK_TOP = 0x800C3680;

void init_bss() {
  // Clear BSS section like the boot code does
  uint32_t bss_offset = BSS_START & 0x7FFFFF;
  size_t clear_size = std::min((size_t)BSS_SIZE, RDRAM_SIZE - bss_offset);
  memset(g_rdram + bss_offset, 0, clear_size);
  printf("Cleared BSS: 0x%08X - 0x%08X (%zu bytes)\n", BSS_START,
         BSS_START + (uint32_t)clear_size, clear_size);
}

void init_boot_context(recomp_context *ctx) {
  memset(ctx, 0, sizeof(recomp_context));

  // Set up initial register state matching N64 boot code
  // Stack pointer from boot code: lui $sp, 0x800C; addiu $sp, 0x3680
  ctx->r29 = 0xFFFFFFFF80000000ULL | STACK_TOP; // $sp - sign-extended
  ctx->r31 = 0; // $ra - return address (none at boot)

  // Boot parameters (a0-a3)
  ctx->r4 = 1;          // $a0 = TV type (1 = NTSC)
  ctx->r5 = 0;          // $a1 = device type
  ctx->r6 = 0;          // $a2 = reset type
  ctx->r7 = RDRAM_SIZE; // $a3 = memory size

  // Enable MIPS III float mode
  ctx->mips3_float_mode = 1;

  // Status register - enable FPU, kernel mode
  ctx->status_reg = 0x34000000;
}

void try_execute_game() {
  printf("\n=== Attempting Game Execution ===\n");

  // Initialize hardware registers
  hw_init();
  printf("Hardware registers initialized\n");

  // Copy ROM data to RDRAM (simulating N64 boot loader)
  // The N64 IPL copies the first ~1MB of ROM to 0x80000400
  if (!g_rom.empty()) {
    size_t boot_size = std::min(g_rom.size(), (size_t)(1024 * 1024));
    copy_rom_to_rdram(0x80000400, 0x1000, boot_size - 0x1000);
    printf("Copied %zu bytes from ROM to RDRAM at 0x80000400\n",
           boot_size - 0x1000);
  }

  // Clear BSS section (what the boot stub at 0x80000400 does)
  init_bss();

  // Initialize context
  // Using a static context to ensure it lives as long as the background thread
  static recomp_context ctx;
  init_boot_context(&ctx);

  printf("Initial register state:\n");
  printf("  $sp (r29) = 0x%016llX (stack at 0x%08X)\n",
         (unsigned long long)ctx.r29, STACK_TOP);
  printf("  $a0 (r4)  = %lld (TV type)\n", (long long)ctx.r4);
  printf("  $a3 (r7)  = 0x%llX (RAM size)\n", (unsigned long long)ctx.r7);
  printf("  RDRAM base = %p\n", (void *)g_rdram);

  // Install signal handlers with sigaction for better info
  struct sigaction sa;
  sa.sa_sigaction = signal_handler;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGSEGV, &sa, nullptr);
  sigaction(SIGBUS, &sa, nullptr);
  sigaction(SIGFPE, &sa, nullptr);

  // Run game code in a separate thread to keep the main thread free for SDL
  // events (required on macOS)
  std::thread game_thread([&]() {
    try {
      // Set up jump buffer for crash recovery in this thread
      g_in_game_code = true;
      if (sigsetjmp(g_jump_buf, 1) != 0) {
        // We returned here from a crash
        printf("\n=== Execution halted after crash ===\n");
        printf("Total crashes: %d\n", g_crash_count);
        g_in_game_code = false;
        return;
      }

      printf("\nCalling game_main (0x80018678)...\n");
      // Call game_main - the actual main function after boot
      // This initializes OS and creates threads
      g_game_started = true;
      game_main(g_rdram, &ctx);
      printf("game_main returned (OS init complete, threads created)\n");

      g_in_game_code = false;
      printf("Game thread (initial) finished. Game logic continues in ultramodern pthreads.\n");
    } catch (const std::exception &e) {
      fprintf(stderr, "Exception in game thread: %s\n", e.what());
    } catch (...) {
      fprintf(stderr, "Unknown exception in game thread!\n");
    }
  });

  // Main loop for SDL events and rendering (on the main thread)
  printf("Entering main loop...\n");

  bool graphics_active = false;
#ifdef WITH_RT64
  graphics_active = true;
#endif

  if (graphics_active) {
    while (is_graphics_running()) {
      process_frame();
      // Yield to avoid 100% CPU usage
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    printf("Graphics system stopped.\n");
  } else {
    printf("Graphics disabled, waiting for game thread (Ctrl+C to exit)...\n");
    if (game_thread.joinable()) {
      game_thread.join();
    }
  }

  // If we're here, either graphics stopped or game thread finished
  // We can't easily kill the game_thread if it's stuck, so we'll just exit
  printf("Exiting try_execute_game.\n");
}

void print_usage(const char *prog) {
  printf("Usage: %s [options] [rom_file]\n", prog);
  printf("\nOptions:\n");
  printf("  --test     Run in test mode (don't execute game)\n");
  printf("  --help     Show this help\n");
  printf("\nIf no ROM file is specified, will try 'game.z64' or 'game.n64'\n");
}

int main(int argc, char *argv[]) {
  // Install custom terminate handler to catch unhandled exceptions
  std::set_terminate(custom_terminate_handler);

  // Disable output buffering for immediate feedback
  setvbuf(stdout, nullptr, _IONBF, 0);
  setvbuf(stderr, nullptr, _IONBF, 0);

  printf("CVLoD Recomp - Castlevania: Legacy of Darkness\n");
  printf("================================================\n\n");

  bool test_mode = false;
  const char *rom_path = nullptr;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--test") == 0) {
      test_mode = true;
    } else if (strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return 0;
    } else if (argv[i][0] != '-') {
      rom_path = argv[i];
    }
  }

  // Allocate RDRAM
  g_rdram = new uint8_t[RDRAM_SIZE];
  if (!g_rdram) {
    fprintf(stderr, "Failed to allocate RDRAM\n");
    return 1;
  }
  memset(g_rdram, 0, RDRAM_SIZE);
  printf("Allocated %zu MB RDRAM\n", RDRAM_SIZE / (1024 * 1024));

  // Initialize graphics system (SDL + RT64 Application creation)
  init_graphics_system();

  // Finalize RT64 setup with the allocated RDRAM
  setup_rt64_with_rdram(g_rdram);

  // Pre-initialize ultramodern with the window handle
  void *handle_ptr = get_native_window_handle();
#ifdef _WIN32
  ultramodern::renderer::WindowHandle win_handle{(HWND)handle_ptr};
#else
  ultramodern::renderer::WindowHandle win_handle =
      *(ultramodern::renderer::WindowHandle *)handle_ptr;
#endif
  // ultramodern::preinit() initializes the event system internally, including:
  // - VI thread (sends screen updates every ~16.67ms)
  // - GFX thread (calls renderer_context->send_dl() and update_screen())
  // - Task thread (handles RSP tasks)
  //
  // On macOS, RT64's Metal setup may need the main thread's runloop to be active.
  // To avoid deadlock, we run preinit in a background thread while pumping SDL events.
  printf("Initializing ultramodern (event system, timers, audio)...\n");
  std::atomic<bool> preinit_done{false};
  std::atomic<bool> preinit_failed{false};
  std::thread preinit_thread([&]() {
    try {
      ultramodern::preinit(g_rdram, win_handle);
      preinit_done = true;
    } catch (const std::exception &e) {
      fprintf(stderr, "Failed to initialize ultramodern: %s\n", e.what());
      preinit_failed = true;
    }
  });

  // Pump SDL events while waiting for preinit to complete
  // This ensures Metal can initialize properly on macOS
  while (!preinit_done && !preinit_failed) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        fprintf(stderr, "Quit requested during initialization\n");
        exit(1);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  preinit_thread.join();
  if (preinit_failed) {
    fprintf(stderr, "ultramodern initialization failed\n");
    return 1;
  }
  printf("ultramodern initialized successfully\n");

  // Initialize function lookup table
  init_function_table();
  printf("Function table initialized\n");

  // Add address aliases for thread entry points (ELF/ROM address offset fix)
  add_address_aliases();

  // Register libultra wrapper functions in the lookup table
  // These override the recompiled stubs with our native implementations
  printf("Registering libultra wrappers in lookup table...\n");
  set_libultra_wrapper(0x8008A690, func_8008A690);  // osViSwapBuffer
  set_libultra_wrapper(0x80083120, func_80083120);  // osViSetMode
  set_libultra_wrapper(0x8008BD70, func_8008BD70);  // osViBlack
  set_libultra_wrapper(0x8007FFC0, func_8007FFC0);  // osSpTaskLoad
  set_libultra_wrapper(0x800802B0, func_800802B0);  // osSpTaskStartGo
  set_libultra_wrapper(0x80089EC0, func_80089EC0);  // osCreateMesgQueue
  set_libultra_wrapper(0x8007F2E0, func_8007F2E0);  // osSendMesg
  set_libultra_wrapper(0x8007F860, func_8007F860);  // osRecvMesg
  set_libultra_wrapper(0x80081D80, func_80081D80);  // osInitialize
  set_libultra_wrapper(0x800812C0, func_800812C0);  // osCreateThread
  set_libultra_wrapper(0x80090490, func_80090490);  // osStartThread
  set_libultra_wrapper(0x8008C4E0, L_8008C4E0);     // osViSetEvent
  printf("libultra wrappers registered\n");

  // Install libultra wrappers to replace stubbed functions with ultramodern
  install_libultra_wrappers();
  printf("libultra wrappers installed\n");

  // Install game-specific thread system stubs
  install_game_thread_stubs();

  // Disable verbose lookup tracing for debugging
  set_trace_lookups(true);

  // Try to load ROM
  if (rom_path) {
    if (!load_rom(rom_path)) {
      fprintf(stderr, "Failed to load ROM\n");
    }
  } else {
    // Try default paths
    const char *default_paths[] = {
        "game.z64", "game.n64", "../game.z64",
        "../resources/castlevania_legacy_of_darkness.z64", nullptr};

    for (const char **p = default_paths; *p; p++) {
      std::ifstream test(*p);
      if (test.good()) {
        test.close();
        load_rom(*p);
        break;
      }
    }

    if (g_rom.empty()) {
      printf("No ROM file found. Specify with: %s <rom_file>\n", argv[0]);
    }
  }

  if (test_mode) {
    printf("\nTest mode - skipping game execution\n");
    printf("Runtime initialization complete.\n");
  } else if (!g_rom.empty()) {
    try_execute_game();
  } else {
    printf("\nNo ROM loaded - cannot execute game\n");
    printf("Runtime initialization complete.\n");
  }

  // Signal exit and cleanup
  exited.store(true);

  // Signal graphics shutdown and join event threads
#ifdef WITH_RT64
  graphics_shutdown_ready.signal();
  ultramodern::join_event_threads();
#endif

  cleanup_graphics_system();
  delete[] g_rdram;
  g_rdram = nullptr;

  return 0;
}

// Implementation of get_function from recomp.h
// This is called by LOOKUP_FUNC macro in recompiled code
static int get_function_call_count = 0;
recomp_func_t *get_function(int32_t vram) {
  get_function_call_count++;
  if (get_function_call_count <= 20 || get_function_call_count % 1000 == 0) {
    printf("[get_function] call #%d: vram=0x%08X\n", get_function_call_count, (uint32_t)vram);
    fflush(stdout);
  }
  return cvlod_get_function(vram);
}

// Stub implementations for functions called by recompiled code
extern "C" {
void cop0_status_write(recomp_context *ctx, gpr value) {
  ctx->status_reg = (uint32_t)value;
}

gpr cop0_status_read(recomp_context *ctx) { return ctx->status_reg; }

void switch_error(const char *func, uint32_t vram, uint32_t jtbl) {
  fprintf(stderr, "Switch error in %s at 0x%08X (jump table 0x%08X)\n", func,
          vram, jtbl);
}

void do_break(uint32_t vram) {
  fprintf(stderr, "Break instruction at 0x%08X\n", vram);
}

void recomp_syscall_handler(uint8_t *rdram, recomp_context *ctx,
                            int32_t instruction_vram) {
  fprintf(stderr, "Syscall at 0x%08X\n", instruction_vram);
}

// Note: pause_self is provided by ultramodern/scheduling.cpp

// Section addresses for relocations
int32_t *section_addresses = nullptr;

// osYieldThread - stub (ultramodern declares but doesn't define it)
void osYieldThread(uint8_t *rdram) {
  printf("[STUB] osYieldThread() called\n");
  // For now, this is a no-op since we don't have proper thread scheduling yet
}
}

namespace ultramodern {
bool is_game_started() {
  return g_game_started.load();
}
} // namespace ultramodern

// Function to run a thread's entry point
// This is called by ultramodern when starting a thread
void run_thread_function(uint8_t *rdram, uint64_t addr, uint64_t sp,
                         uint64_t arg) {
  printf("[THREAD] run_thread_function(addr=0x%08llX, sp=0x%08llX, "
         "arg=0x%08llX)\n",
         (unsigned long long)addr, (unsigned long long)sp,
         (unsigned long long)arg);

  // Get the function pointer from our function table
  recomp_func_t *func = cvlod_get_function((int32_t)addr);
  if (func) {
    // Set up a minimal context
    recomp_context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.r29 = sp; // Stack pointer
    ctx.r4 = arg; // First argument (a0)

    // Call the function
    func(rdram, &ctx);
  } else {
    fprintf(stderr, "[THREAD] Function not found at 0x%08llX!\n",
            (unsigned long long)addr);
  }
}
