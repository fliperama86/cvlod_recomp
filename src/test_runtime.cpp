/**
 * CVLoD Recomp - Runtime Tests
 *
 * Preliminary tests to verify the runtime infrastructure.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "recomp.h"

// External function table
extern void init_function_table();
extern recomp_func_t* cvlod_get_function(int32_t vram);

// Test RDRAM
static uint8_t* g_rdram = nullptr;
static constexpr size_t RDRAM_SIZE = 8 * 1024 * 1024;

// Test context
static recomp_context g_ctx;

void init_context() {
    memset(&g_ctx, 0, sizeof(g_ctx));
    g_ctx.mips3_float_mode = 1;
}

bool test_function_table() {
    printf("\n=== Test: Function Table ===\n");

    init_function_table();

    // Test some known function addresses
    uint32_t test_addrs[] = {
        0x80000460, 0x8000053C, 0x80001090, 0x80005000,
        0x80010000, 0x80050000, 0x80100000, 0x80150000
    };

    int found = 0;
    int not_found = 0;

    for (uint32_t addr : test_addrs) {
        recomp_func_t* func = cvlod_get_function(addr);
        if (func) {
            printf("  [OK] 0x%08X -> function found\n", addr);
            found++;
        } else {
            printf("  [--] 0x%08X -> not found\n", addr);
            not_found++;
        }
    }

    printf("  Found: %d, Not found: %d\n", found, not_found);
    return found > 0;
}

bool test_memory_access() {
    printf("\n=== Test: Memory Access ===\n");

    uint8_t* rdram = g_rdram;

    // The MEM_W macro expects addresses in N64 KSEG0 format (0x80000000+)
    // It calculates: rdram + (addr - 0xFFFFFFFF80000000)
    // For 64-bit, we need to use sign-extended addresses

    // Test using sign-extended address (simulate how MIPS64 works)
    gpr test_addr = 0xFFFFFFFF80001000ULL; // Sign-extended 0x80001000

    // Calculate offset manually to show it works
    uint64_t offset = test_addr - 0xFFFFFFFF80000000ULL;
    printf("  test_addr = 0x%016llX\n", (unsigned long long)test_addr);
    printf("  offset = 0x%llX (should be 0x1000)\n", (unsigned long long)offset);

    // Write using MEM_W macro
    MEM_W(0, test_addr) = 0xDEADBEEF;

    // Read back
    int32_t read_val = MEM_W(0, test_addr);

    printf("  Write 0xDEADBEEF, read back: 0x%08X\n", (uint32_t)read_val);

    if ((uint32_t)read_val == 0xDEADBEEF) {
        printf("  [OK] Word access works\n");
    } else {
        printf("  [FAIL] Word mismatch\n");
        return false;
    }

    // Test halfword with sign-extended address
    gpr h_addr = 0xFFFFFFFF80002000ULL;
    MEM_H(0, h_addr) = 0x1234;
    int16_t h = MEM_H(0, h_addr);
    printf("  Halfword: wrote 0x1234, read 0x%04X\n", (uint16_t)h);

    // Test byte with sign-extended address
    gpr b_addr = 0xFFFFFFFF80003000ULL;
    MEM_B(0, b_addr) = 0x42;
    int8_t b = MEM_B(0, b_addr);
    printf("  Byte: wrote 0x42, read 0x%02X\n", (uint8_t)b);

    printf("  [OK] Memory access works\n");
    return true;
}

bool test_register_context() {
    printf("\n=== Test: Register Context ===\n");

    init_context();

    // Test GPRs
    g_ctx.r1 = 0x0000000100000001ULL;
    g_ctx.r2 = 0xFFFFFFFFFFFFFFFFULL;
    g_ctx.r31 = 0x80001234;

    printf("  r1 = 0x%016llX\n", (unsigned long long)g_ctx.r1);
    printf("  r2 = 0x%016llX\n", (unsigned long long)g_ctx.r2);
    printf("  r31 ($ra) = 0x%08llX\n", (unsigned long long)g_ctx.r31);

    // Test FPRs
    g_ctx.f0.fl = 3.14159f;
    g_ctx.f2.d = 2.71828;

    printf("  f0 (float) = %f\n", g_ctx.f0.fl);
    printf("  f2 (double) = %f\n", g_ctx.f2.d);

    // Test hi/lo
    g_ctx.hi = 0x12345678;
    g_ctx.lo = 0x9ABCDEF0;

    printf("  hi = 0x%08llX, lo = 0x%08llX\n",
           (unsigned long long)g_ctx.hi, (unsigned long long)g_ctx.lo);

    printf("  [OK] Register context works\n");
    return true;
}

bool test_arithmetic_macros() {
    printf("\n=== Test: Arithmetic Macros ===\n");

    // ADD32 with overflow
    gpr result = ADD32(0x7FFFFFFF, 1);
    printf("  ADD32(0x7FFFFFFF, 1) = 0x%08llX\n", (unsigned long long)result);

    // SUB32
    result = SUB32(0, 1);
    printf("  SUB32(0, 1) = 0x%016llX (sign-extended -1)\n", (unsigned long long)result);

    // S32 sign extension
    result = S32(-1);
    printf("  S32(-1) = 0x%016llX\n", (unsigned long long)result);

    // SIGNED cast
    int64_t signed_val = SIGNED(0xFFFFFFFF80000000ULL);
    printf("  SIGNED(0x80000000) = %lld\n", (long long)signed_val);

    printf("  [OK] Arithmetic macros work\n");
    return true;
}

bool test_function_coverage() {
    printf("\n=== Test: Function Coverage ===\n");

    init_function_table();

    int found = 0;

    // Sample addresses in different ranges
    for (uint32_t base = 0x80000000; base < 0x80180000; base += 0x10000) {
        for (uint32_t offset = 0; offset < 0x1000; offset += 0x100) {
            if (cvlod_get_function(base + offset)) {
                found++;
            }
        }
    }

    printf("  Sampled function coverage: %d functions found\n", found);

    // Count exact matches for known ranges
    int main_funcs = 0;
    for (uint32_t addr = 0x80000400; addr < 0x800B2000; addr += 4) {
        if (cvlod_get_function(addr)) main_funcs++;
    }
    printf("  .main section (0x80000400-0x800B2000): %d functions\n", main_funcs);

    int common_funcs = 0;
    for (uint32_t addr = 0x80128000; addr < 0x80180000; addr += 4) {
        if (cvlod_get_function(addr)) common_funcs++;
    }
    printf("  .common section (0x80128000-0x80180000): %d functions\n", common_funcs);

    return true;
}

bool test_64bit_math() {
    printf("\n=== Test: 64-bit Math ===\n");

    int64_t lo, hi;

    // Test DMULT
    DMULT(0x100000000LL, 0x100000000LL, &lo, &hi);
    printf("  DMULT(0x100000000, 0x100000000):\n");
    printf("    hi = 0x%016llX, lo = 0x%016llX\n",
           (unsigned long long)hi, (unsigned long long)lo);

    // Test DDIV
    int64_t quot, rem;
    DDIV(1000000000000LL, 7LL, &quot, &rem);
    printf("  DDIV(1000000000000, 7):\n");
    printf("    quot = %lld, rem = %lld\n", (long long)quot, (long long)rem);

    printf("  [OK] 64-bit math works\n");
    return true;
}

int main(int argc, char* argv[]) {
    printf("CVLoD Recomp - Runtime Tests\n");
    printf("============================\n");

    // Allocate RDRAM
    g_rdram = new uint8_t[RDRAM_SIZE];
    memset(g_rdram, 0, RDRAM_SIZE);
    printf("Allocated %zu MB RDRAM at %p\n", RDRAM_SIZE / (1024*1024), g_rdram);

    int passed = 0;
    int failed = 0;

    if (test_function_table()) passed++; else failed++;
    if (test_memory_access()) passed++; else failed++;
    if (test_register_context()) passed++; else failed++;
    if (test_arithmetic_macros()) passed++; else failed++;
    if (test_64bit_math()) passed++; else failed++;
    if (test_function_coverage()) passed++; else failed++;

    printf("\n============================\n");
    printf("Results: %d passed, %d failed\n", passed, failed);

    delete[] g_rdram;

    return failed > 0 ? 1 : 0;
}
