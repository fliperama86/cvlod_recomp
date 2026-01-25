/**
 * CVLoD Recomp - Castlevania: Legacy of Darkness Recompilation
 *
 * Main entry point for the recompiled game runtime.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <csignal>
#include <csetjmp>
#include <sys/mman.h>

#include "recomp.h"
#include "hardware.h"

// Forward declarations from funcs.cpp
extern void init_function_table();
extern recomp_func_t* cvlod_get_function(int32_t vram);
extern void set_trace_lookups(bool enable);

// Forward declaration from libultra_wrappers.cpp
extern void install_libultra_wrappers();

// Forward declarations for game functions
extern "C" void func_80000460(uint8_t* rdram, recomp_context* ctx);
extern "C" void game_main(uint8_t* rdram, recomp_context* ctx);
extern "C" void func_8001783C(uint8_t* rdram, recomp_context* ctx); // Main thread entry (0x800186DC)

// Global RDRAM allocation
static uint8_t* g_rdram = nullptr;
static constexpr size_t RDRAM_SIZE = 8 * 1024 * 1024; // 8MB RDRAM

// ROM data
static std::vector<uint8_t> g_rom;

// Jump buffer for recovery
static sigjmp_buf g_jump_buf;
static volatile bool g_in_game_code = false;
static volatile int g_crash_count = 0;
static constexpr int MAX_CRASHES = 100;

// Signal handler for debugging crashes
static void signal_handler(int sig, siginfo_t* info, void* context) {
    g_crash_count++;

    fprintf(stderr, "\n*** Signal %d received (crash #%d) ***\n", sig, g_crash_count);

    if (info) {
        void* fault_addr = info->si_addr;
        fprintf(stderr, "Fault address: %p\n", fault_addr);

        // Check if this looks like an N64 hardware address
        uint64_t addr = (uint64_t)(uintptr_t)fault_addr;

        // The MEM_* macros calculate: rdram + (n64_addr - 0xFFFFFFFF80000000)
        // So fault_addr = rdram + offset, where offset = n64_addr - 0x80000000
        // If offset is huge (> 8MB), it's likely a hardware access

        ptrdiff_t offset = (uint8_t*)fault_addr - g_rdram;
        if (offset < 0 || offset > 0x800000) {
            // Calculate what N64 address this would be
            uint64_t n64_addr = 0x80000000ULL + offset;
            fprintf(stderr, "Likely N64 address: 0x%08llX\n", (unsigned long long)(n64_addr & 0xFFFFFFFF));

            if ((n64_addr & 0xFF000000) == 0xA4000000) {
                fprintf(stderr, "This is a HARDWARE REGISTER access!\n");
                if ((n64_addr & 0x00F00000) == 0x00000000) fprintf(stderr, "  -> RSP registers\n");
                else if ((n64_addr & 0x00F00000) == 0x00100000) fprintf(stderr, "  -> RDP registers\n");
                else if ((n64_addr & 0x00F00000) == 0x00300000) fprintf(stderr, "  -> MI registers\n");
                else if ((n64_addr & 0x00F00000) == 0x00400000) fprintf(stderr, "  -> VI registers\n");
                else if ((n64_addr & 0x00F00000) == 0x00500000) fprintf(stderr, "  -> AI registers\n");
                else if ((n64_addr & 0x00F00000) == 0x00600000) fprintf(stderr, "  -> PI registers\n");
                else if ((n64_addr & 0x00F00000) == 0x00700000) fprintf(stderr, "  -> RI registers\n");
                else if ((n64_addr & 0x00F00000) == 0x00800000) fprintf(stderr, "  -> SI registers\n");
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

bool load_rom(const char* path) {
    printf("Loading ROM: %s\n", path);

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        fprintf(stderr, "Failed to open ROM file\n");
        return false;
    }

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    g_rom.resize(size);
    file.read(reinterpret_cast<char*>(g_rom.data()), size);

    printf("ROM loaded: %zu bytes (%.1f MB)\n", size, size / (1024.0 * 1024.0));

    // Verify N64 ROM header
    if (size >= 4) {
        uint32_t magic = (g_rom[0] << 24) | (g_rom[1] << 16) | (g_rom[2] << 8) | g_rom[3];
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

void copy_rom_to_rdram(uint32_t rdram_addr, uint32_t rom_offset, size_t length) {
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
constexpr uint32_t BSS_SIZE = 0x80350;  // 524,112 bytes
constexpr uint32_t STACK_TOP = 0x800C3680;

void init_bss() {
    // Clear BSS section like the boot code does
    uint32_t bss_offset = BSS_START & 0x7FFFFF;
    size_t clear_size = std::min((size_t)BSS_SIZE, RDRAM_SIZE - bss_offset);
    memset(g_rdram + bss_offset, 0, clear_size);
    printf("Cleared BSS: 0x%08X - 0x%08X (%zu bytes)\n",
           BSS_START, BSS_START + (uint32_t)clear_size, clear_size);
}

void init_boot_context(recomp_context* ctx) {
    memset(ctx, 0, sizeof(recomp_context));

    // Set up initial register state matching N64 boot code
    // Stack pointer from boot code: lui $sp, 0x800C; addiu $sp, 0x3680
    ctx->r29 = 0xFFFFFFFF80000000ULL | STACK_TOP; // $sp - sign-extended
    ctx->r31 = 0;                                  // $ra - return address (none at boot)

    // Boot parameters (a0-a3)
    ctx->r4 = 1;    // $a0 = TV type (1 = NTSC)
    ctx->r5 = 0;    // $a1 = device type
    ctx->r6 = 0;    // $a2 = reset type
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
        printf("Copied %zu bytes from ROM to RDRAM at 0x80000400\n", boot_size - 0x1000);
    }

    // Clear BSS section (what the boot stub at 0x80000400 does)
    init_bss();

    // Initialize context
    recomp_context ctx;
    init_boot_context(&ctx);

    printf("Initial register state:\n");
    printf("  $sp (r29) = 0x%016llX (stack at 0x%08X)\n",
           (unsigned long long)ctx.r29, STACK_TOP);
    printf("  $a0 (r4)  = %lld (TV type)\n", (long long)ctx.r4);
    printf("  $a3 (r7)  = 0x%llX (RAM size)\n", (unsigned long long)ctx.r7);
    printf("  RDRAM base = %p\n", (void*)g_rdram);

    // Install signal handlers with sigaction for better info
    struct sigaction sa;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);

    // Set up jump buffer for crash recovery
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
    // This initializes OS and creates threads (all stubbed)
    game_main(g_rdram, &ctx);
    printf("game_main returned (OS init complete, threads stubbed)\n");

    printf("\nCalling main thread entry func_8001783C (0x800186DC)...\n");
    printf("(This is what osStartThread would run)\n\n");

    // Call the main thread entry function directly
    // This bypasses the thread system and runs the game logic
    func_8001783C(g_rdram, &ctx);

    g_in_game_code = false;
    printf("Main thread returned!\n");
}

void print_usage(const char* prog) {
    printf("Usage: %s [options] [rom_file]\n", prog);
    printf("\nOptions:\n");
    printf("  --test     Run in test mode (don't execute game)\n");
    printf("  --help     Show this help\n");
    printf("\nIf no ROM file is specified, will try 'game.z64' or 'game.n64'\n");
}

int main(int argc, char* argv[]) {
    printf("CVLoD Recomp - Castlevania: Legacy of Darkness\n");
    printf("================================================\n\n");

    bool test_mode = false;
    const char* rom_path = nullptr;

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

    // Initialize function lookup table
    init_function_table();
    printf("Function table initialized\n");

    // Install libultra wrappers to replace stubbed functions with ultramodern
    install_libultra_wrappers();
    printf("libultra wrappers installed\n");

    // Try to load ROM
    if (rom_path) {
        if (!load_rom(rom_path)) {
            fprintf(stderr, "Failed to load ROM\n");
        }
    } else {
        // Try default paths
        const char* default_paths[] = {
            "game.z64",
            "game.n64",
            "../game.z64",
            "../resources/castlevania_legacy_of_darkness.z64",
            nullptr
        };

        for (const char** p = default_paths; *p; p++) {
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

    // Cleanup
    delete[] g_rdram;
    g_rdram = nullptr;

    return 0;
}

// Implementation of get_function from recomp.h
// This is called by LOOKUP_FUNC macro in recompiled code
recomp_func_t* get_function(int32_t vram) {
    return cvlod_get_function(vram);
}

// Stub implementations for functions called by recompiled code
extern "C" {
    void cop0_status_write(recomp_context* ctx, gpr value) {
        ctx->status_reg = (uint32_t)value;
    }

    gpr cop0_status_read(recomp_context* ctx) {
        return ctx->status_reg;
    }

    void switch_error(const char* func, uint32_t vram, uint32_t jtbl) {
        fprintf(stderr, "Switch error in %s at 0x%08X (jump table 0x%08X)\n",
                func, vram, jtbl);
    }

    void do_break(uint32_t vram) {
        fprintf(stderr, "Break instruction at 0x%08X\n", vram);
    }

    void recomp_syscall_handler(uint8_t* rdram, recomp_context* ctx, int32_t instruction_vram) {
        fprintf(stderr, "Syscall at 0x%08X\n", instruction_vram);
    }

    // Note: pause_self is provided by ultramodern/scheduling.cpp

    // Section addresses for relocations
    int32_t* section_addresses = nullptr;

    // osYieldThread - stub (ultramodern declares but doesn't define it)
    void osYieldThread(uint8_t* rdram) {
        printf("[STUB] osYieldThread() called\n");
        // For now, this is a no-op since we don't have proper thread scheduling yet
    }
}

// Symbols required by ultramodern
#include <atomic>
#ifndef MOODYCAMEL_DELETE_FUNCTION
#define MOODYCAMEL_DELETE_FUNCTION = delete
#endif
#include "lightweightsemaphore.h"

std::atomic_bool exited{false};
moodycamel::LightweightSemaphore graphics_shutdown_ready;

namespace ultramodern {
    bool is_game_started() {
        // Always return false for now since we're not fully running the game loop
        return false;
    }
}

// Function to run a thread's entry point
// This is called by ultramodern when starting a thread
void run_thread_function(uint8_t* rdram, uint64_t addr, uint64_t sp, uint64_t arg) {
    printf("[THREAD] run_thread_function(addr=0x%08llX, sp=0x%08llX, arg=0x%08llX)\n",
           (unsigned long long)addr, (unsigned long long)sp, (unsigned long long)arg);

    // Get the function pointer from our function table
    recomp_func_t* func = cvlod_get_function((int32_t)addr);
    if (func) {
        // Set up a minimal context
        recomp_context ctx;
        memset(&ctx, 0, sizeof(ctx));
        ctx.r29 = sp;  // Stack pointer
        ctx.r4 = arg;  // First argument (a0)

        // Call the function
        func(rdram, &ctx);
    } else {
        fprintf(stderr, "[THREAD] Function not found at 0x%08llX!\n", (unsigned long long)addr);
    }
}
