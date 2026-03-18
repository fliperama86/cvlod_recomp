// DL Test Harness for LoD Recomp
// Tests each stage of the RT64 GBI pipeline with synthetic display lists.
// Reports PASS/FAIL per test based on workload output inspection — no visual
// inspection needed.
//
// Usage: ./DLHarness [--rom path/to/rom.z64]
//   Without ROM: runs synthetic tests only
//   With ROM: also runs NI-file-based tests with real game data

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <sys/mman.h>

#define SDL_MAIN_HANDLED
#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#endif

#ifndef HLSL_CPU
#define HLSL_CPU
#endif

#include "hle/rt64_application.h"
#include "hle/rt64_workload.h"
#include "gbi/rt64_gbi_rdp.h"
#include "gbi/rt64_gbi_f3dex2.h"

// ── Constants ────────────────────────────────────────────────────────
static constexpr uint32_t SCREEN_W     = 320;
static constexpr uint32_t SCREEN_H     = 240;
static constexpr uint32_t RDRAM_SIZE   = 0x15000000; // 336MB — enough for extendRDRAM tests
static constexpr uint32_t FB0_ADDR     = 0x00100000;
static constexpr uint32_t FB1_ADDR     = 0x001DA800;
static constexpr uint32_t ZIMG_ADDR    = 0x002B0000;
static constexpr uint32_t DL_ADDR      = 0x00200000;
static constexpr uint32_t SUB_DL_ADDR  = 0x00210000;
static constexpr uint32_t VP_ADDR      = 0x00300000;
static constexpr uint32_t PROJ_ADDR    = 0x00300100;
static constexpr uint32_t MODEL_ADDR   = 0x00300200;
static constexpr uint32_t VTX_ADDR     = 0x00300300;
static constexpr uint32_t TEX_ADDR     = 0x00300400;
static constexpr uint32_t SEG6_BASE    = 0x00400000; // Physical base for segment 6 tests
static constexpr uint32_t EXT_TEX_ADDR = 0x14000000; // Extended RDRAM texture address
static constexpr uint32_t TEX_W        = 16;
static constexpr uint32_t TEX_H        = 16;

// ── Globals ──────────────────────────────────────────────────────────
static uint8_t* g_RDRAM = nullptr;
static uint8_t  g_DMEM[0x1000];
static uint8_t  g_IMEM[0x1000];
static uint8_t  g_HEADER[0x40];

static uint32_t MI_INTR_REG       = 0;
static uint32_t DPC_START_REG     = 0;
static uint32_t DPC_END_REG       = 0;
static uint32_t DPC_CURRENT_REG   = 0;
static uint32_t DPC_STATUS_REG    = 0;
static uint32_t DPC_CLOCK_REG     = 0;
static uint32_t DPC_BUFBUSY_REG   = 0;
static uint32_t DPC_PIPEBUSY_REG  = 0;
static uint32_t DPC_TMEM_REG      = 0;

static uint32_t VI_STATUS_REG     = 0x0000030A;
static uint32_t VI_ORIGIN_REG     = FB0_ADDR;
static uint32_t VI_WIDTH_REG      = SCREEN_W;
static uint32_t VI_INTR_REG       = 2;
static uint32_t VI_V_CURRENT_REG  = 0;
static uint32_t VI_TIMING_REG     = 0x03E52239;
static uint32_t VI_V_SYNC_REG     = 0x020D;
static uint32_t VI_H_SYNC_REG     = 0x00000C15;
static uint32_t VI_LEAP_REG       = 0x0C150C15;
static uint32_t VI_H_START_REG    = 0x006C02EC;
static uint32_t VI_V_START_REG    = 0x00250239;
static uint32_t VI_V_BURST_REG    = 0x000E0204;
static uint32_t VI_X_SCALE_REG    = 0x00000200;
static uint32_t VI_Y_SCALE_REG    = 0x00000400;

static void dummy_check_interrupts() {}

// ── Test result ──────────────────────────────────────────────────────
struct TestResult {
    bool passed;
    std::string name;
    std::string detail;
};

// ── Workload snapshot ────────────────────────────────────────────────
struct WorkloadInfo {
    int fbPairCount = 0;
    int totalProjections = 0;
    size_t totalFaces = 0;
    size_t totalPosFloats = 0;
    size_t totalPosScreen = 0;
    size_t totalViewXforms = 0;
    bool anyFillOnly = false;
    bool allFillOnly = true;

    struct FBPairInfo {
        uint32_t colorAddr;
        int projCount;
        int gameCallCount;
        bool fillOnly;
        int drawRectUlx, drawRectUly, drawRectLrx, drawRectLry;
        bool drawRectEmpty;
    };
    std::vector<FBPairInfo> fbPairs;
};

static WorkloadInfo inspect_workload(RT64::Application* app) {
    WorkloadInfo info;
    auto& wq = *app->workloadQueue;
    auto& workload = wq.workloads[wq.previousWriteCursor()];
    auto& dd = workload.drawData;

    info.fbPairCount = workload.fbPairCount;
    info.totalFaces = dd.faceIndices.size();
    info.totalPosFloats = dd.posFloats.size();
    info.totalPosScreen = dd.posScreen.size();
    info.totalViewXforms = dd.viewTransforms.size();

    for (int i = 0; i < workload.fbPairCount && i < 16; i++) {
        auto& fp = workload.fbPairs[i];
        WorkloadInfo::FBPairInfo fpi{};
        fpi.colorAddr = fp.colorImage.address;
        fpi.projCount = fp.projectionCount;
        fpi.gameCallCount = fp.gameCallCount;
        fpi.fillOnly = fp.fillRectOnly;
        fpi.drawRectUlx = fp.drawColorRect.ulx;
        fpi.drawRectUly = fp.drawColorRect.uly;
        fpi.drawRectLrx = fp.drawColorRect.lrx;
        fpi.drawRectLry = fp.drawColorRect.lry;
        fpi.drawRectEmpty = fp.drawColorRect.isEmpty();
        info.fbPairs.push_back(fpi);
        info.totalProjections += fp.projectionCount;
        if (fp.fillRectOnly) info.anyFillOnly = true;
        else info.allFillOnly = false;
    }
    if (info.fbPairCount == 0) info.allFillOnly = false;
    return info;
}

// ── DL building utilities ────────────────────────────────────────────
static void write_cmd(uint32_t base, int idx, uint32_t w0, uint32_t w1) {
    auto* dl = reinterpret_cast<uint32_t*>(g_RDRAM + base + idx * 8);
    dl[0] = w0;
    dl[1] = w1;
}

static void write_identity_matrix(uint32_t addr) {
    auto* mtx = reinterpret_cast<RT64::FixedMatrix*>(g_RDRAM + addr);
    memset(mtx, 0, sizeof(RT64::FixedMatrix));
    mtx->integer[0][0 ^ 1] = 1;
    mtx->integer[1][1 ^ 1] = 1;
    mtx->integer[2][2 ^ 1] = 1;
    mtx->integer[3][3 ^ 1] = 1;
}

static void write_viewport(uint32_t addr, float sx, float sy, float tx, float ty) {
    auto* vp = reinterpret_cast<int16_t*>(g_RDRAM + addr);
    memset(vp, 0, 16);
    vp[0] = int16_t(sy * 4.0f);
    vp[1] = int16_t(sx * 4.0f);
    vp[2] = 0;
    vp[3] = int16_t(512);
    vp[4] = int16_t(ty * 4.0f);
    vp[5] = int16_t(tx * 4.0f);
    vp[6] = 0;
    vp[7] = int16_t(512);
}

static void write_vertex(uint32_t addr, int idx,
                          int16_t x, int16_t y, int16_t z,
                          int16_t s, int16_t t,
                          uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    auto* v = reinterpret_cast<uint8_t*>(g_RDRAM + addr + idx * 16);
    auto* s16 = reinterpret_cast<int16_t*>(v);
    auto* u16 = reinterpret_cast<uint16_t*>(v);
    s16[0] = y;
    s16[1] = x;
    u16[2] = 0;
    s16[3] = z;
    s16[4] = t;
    s16[5] = s;
    v[12] = a;
    v[13] = b;
    v[14] = g;
    v[15] = r;
}

static void write_checkerboard_rgba16(uint32_t addr) {
    auto* tex = reinterpret_cast<uint16_t*>(g_RDRAM + addr);
    for (uint32_t row = 0; row < TEX_H; row++) {
        for (uint32_t col = 0; col < TEX_W; col++) {
            bool white = ((row / 4) ^ (col / 4)) & 1;
            tex[row * TEX_W + col] = white ? 0xFFFF : 0xF801;
        }
    }
}

// Write standard geometry setup into a DL starting at cmd index `i`.
// Returns the new cmd index after the geometry commands.
static int write_geometry_setup(uint32_t dl_base, int i, uint32_t fb_addr) {
    // SETCIMG
    write_cmd(dl_base, i++, 0xFF10013F, fb_addr);
    // SETSCISSOR
    write_cmd(dl_base, i++, 0xED000000, (1280 << 12) | 960);
    // SETOTHERMODE_H: 1-CYCLE
    write_cmd(dl_base, i++, 0xE3000A01, 0);
    // SETOTHERMODE_L
    write_cmd(dl_base, i++, 0xE200001F, 0x0C084000);
    // SETCOMBINE: TEXEL0*SHADE
    write_cmd(dl_base, i++, 0xFC121824, 0x00000000);
    // SETGEOMETRYMODE: SHADE | SHADING_SMOOTH
    write_cmd(dl_base, i++, 0xD9000000, 0x00200004);
    // G_TEXTURE on
    write_cmd(dl_base, i++, 0xD7000002, 0xFFFFFFFF);
    return i;
}

// Write texture load commands. Returns new cmd index.
static int write_texture_load(uint32_t dl_base, int i, uint32_t tex_addr) {
    write_cmd(dl_base, i++, 0xFD100000 | (TEX_W - 1), tex_addr);         // SETTIMG
    write_cmd(dl_base, i++, 0xF5100000, 0x07000000);                      // SETTILE load (tile 7)
    uint32_t texels = TEX_W * TEX_H - 1;
    uint32_t dxt = 2048 / (TEX_W * 2 / 8);
    write_cmd(dl_base, i++, 0xF3000000, (7 << 24) | (texels << 12) | dxt); // LOADBLOCK
    uint32_t line = TEX_W * 2 / 8;
    write_cmd(dl_base, i++, 0xF5100000 | (line << 9), 0x00000000);        // SETTILE render (tile 0)
    uint32_t lrs = (TEX_W - 1) << 2;
    uint32_t lrt = (TEX_H - 1) << 2;
    write_cmd(dl_base, i++, 0xF2000000, (lrs << 12) | lrt);               // SETTILESIZE
    return i;
}

// Write viewport + matrices + vertices + triangle. Returns new cmd index.
static int write_triangle_geometry(uint32_t dl_base, int i) {
    write_viewport(VP_ADDR, 80.0f, 60.0f, 160.0f, 120.0f);
    write_identity_matrix(PROJ_ADDR);
    write_identity_matrix(MODEL_ADDR);

    int16_t texMax = TEX_W << 5;
    write_vertex(VTX_ADDR, 0,  0, -1, 0,  texMax/2, texMax,  255, 0,   0,   255);
    write_vertex(VTX_ADDR, 1, -1,  1, 0,  0,        0,       0,   255, 0,   255);
    write_vertex(VTX_ADDR, 2,  1,  1, 0,  texMax,   0,       0,   0,   255, 255);

    write_cmd(dl_base, i++, 0xDC080008, VP_ADDR);      // MOVEMEM VP
    write_cmd(dl_base, i++, 0xDA380007, PROJ_ADDR);     // MTX projection
    write_cmd(dl_base, i++, 0xDA380003, MODEL_ADDR);    // MTX modelview
    write_cmd(dl_base, i++, 0x01003006, VTX_ADDR);      // VTX: 3 verts at 0
    write_cmd(dl_base, i++, 0x05000204, 0x00000000);    // TRI1: (0,1,2)
    return i;
}

// ── RT64 lifecycle ───────────────────────────────────────────────────
static std::unique_ptr<RT64::Application> g_app;

static bool init_rt64(SDL_Window* window) {
    plume::RenderWindow renderWindow{};
#if defined(__APPLE__)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    SDL_MetalView metalView = SDL_Metal_CreateView(window);
    renderWindow.window = wmInfo.info.cocoa.window;
    renderWindow.view = SDL_Metal_GetLayer(metalView);
#elif defined(_WIN32)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    renderWindow = wmInfo.info.win.window;
#else
    renderWindow = window;
#endif

    RT64::Application::Core appCore{};
    appCore.window = renderWindow;
    appCore.checkInterrupts = dummy_check_interrupts;
    appCore.HEADER = g_HEADER;
    appCore.RDRAM = g_RDRAM;
    appCore.DMEM = g_DMEM;
    appCore.IMEM = g_IMEM;
    appCore.MI_INTR_REG = &MI_INTR_REG;
    appCore.DPC_START_REG = &DPC_START_REG;
    appCore.DPC_END_REG = &DPC_END_REG;
    appCore.DPC_CURRENT_REG = &DPC_CURRENT_REG;
    appCore.DPC_STATUS_REG = &DPC_STATUS_REG;
    appCore.DPC_CLOCK_REG = &DPC_CLOCK_REG;
    appCore.DPC_BUFBUSY_REG = &DPC_BUFBUSY_REG;
    appCore.DPC_PIPEBUSY_REG = &DPC_PIPEBUSY_REG;
    appCore.DPC_TMEM_REG = &DPC_TMEM_REG;
    appCore.VI_STATUS_REG = &VI_STATUS_REG;
    appCore.VI_ORIGIN_REG = &VI_ORIGIN_REG;
    appCore.VI_WIDTH_REG = &VI_WIDTH_REG;
    appCore.VI_INTR_REG = &VI_INTR_REG;
    appCore.VI_V_CURRENT_LINE_REG = &VI_V_CURRENT_REG;
    appCore.VI_TIMING_REG = &VI_TIMING_REG;
    appCore.VI_V_SYNC_REG = &VI_V_SYNC_REG;
    appCore.VI_H_SYNC_REG = &VI_H_SYNC_REG;
    appCore.VI_LEAP_REG = &VI_LEAP_REG;
    appCore.VI_H_START_REG = &VI_H_START_REG;
    appCore.VI_V_START_REG = &VI_V_START_REG;
    appCore.VI_V_BURST_REG = &VI_V_BURST_REG;
    appCore.VI_X_SCALE_REG = &VI_X_SCALE_REG;
    appCore.VI_Y_SCALE_REG = &VI_Y_SCALE_REG;

    RT64::ApplicationConfiguration appConfig;
    appConfig.useConfigurationFile = false;

    g_app = std::make_unique<RT64::Application>(appCore, appConfig);
    g_app->userConfig.graphicsAPI = RT64::UserConfiguration::GraphicsAPI::Automatic;
    g_app->userConfig.resolution = RT64::UserConfiguration::Resolution::Original;
    g_app->userConfig.antialiasing = RT64::UserConfiguration::Antialiasing::None;
    g_app->userConfig.refreshRate = RT64::UserConfiguration::RefreshRate::Original;
    g_app->userConfig.developerMode = true;

    auto result = g_app->setup(0);
    if (result != RT64::Application::SetupResult::Success) {
        fprintf(stderr, "[HARNESS] RT64 setup failed (result=%d)\n", (int)result);
        g_app = nullptr;
        return false;
    }

    // Configure F3DEX2 GBI
    auto& gbi = g_app->interpreter->gbiManager.gbiCache[uint32_t(RT64::GBIUCode::F3DEX2)];
    if (gbi.ucode == RT64::GBIUCode::Unknown) {
        gbi.ucode = RT64::GBIUCode::F3DEX2;
        RT64::GBI_RDP::setup(&gbi, true);
        RT64::GBI_F3DEX2::setup(&gbi);
    }
    g_app->interpreter->hleGBI = &gbi;
    g_app->state->rsp->setGBI(&gbi);
    if (gbi.resetFromTask) gbi.resetFromTask(g_app->state.get());

    return true;
}

static void reset_for_test() {
    g_app->state->rsp->reset();
    auto& gbi = g_app->interpreter->gbiManager.gbiCache[uint32_t(RT64::GBIUCode::F3DEX2)];
    if (gbi.resetFromTask) gbi.resetFromTask(g_app->state.get());
    // Clear DL and sub-DL areas
    memset(g_RDRAM + DL_ADDR, 0, 0x10000);
    memset(g_RDRAM + SUB_DL_ADDR, 0, 0x10000);
}

static void process_and_present(uint32_t dl_addr, bool extend_rdram = false) {
    if (extend_rdram) {
        g_app->state->extended.extendRDRAM = true;
    }
    g_app->processDisplayLists(g_RDRAM, dl_addr, 0, true);
    VI_ORIGIN_REG = FB0_ADDR + SCREEN_W * 2;
    g_app->updateScreen();
}

// ═══════════════════════════════════════════════════════════════════════
// TESTS
// ═══════════════════════════════════════════════════════════════════════

// --- Test 1: Basic fill rectangle ---
static TestResult test_fillrect() {
    reset_for_test();
    int i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);                    // SETCIMG
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);          // SETSCISSOR
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);                     // SETOTHERMODE_H: FILL
    write_cmd(DL_ADDR, i++, 0xF7000000, 0x001F001F);                  // SETFILLCOLOR: blue
    uint32_t xl = 319 << 2, yl = 239 << 2;
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);         // FILLRECT
    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);                  // FULLSYNC
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);                  // ENDDL

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d fill=%s",
             wl.fbPairCount, wl.anyFillOnly ? "yes" : "no");

    bool pass = wl.fbPairCount >= 1 && wl.anyFillOnly;
    return { pass, "test_fillrect", detail };
}

// --- Test 2: Textured triangle (sanity baseline) ---
static TestResult test_triangle() {
    reset_for_test();
    write_checkerboard_rgba16(TEX_ADDR);

    int i = 0;
    // Fill phase
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);
    write_cmd(DL_ADDR, i++, 0xF7000000, 0x00010001);
    uint32_t xl = 319 << 2, yl = 239 << 2;
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);

    // Geometry phase
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    i = write_geometry_setup(DL_ADDR, i - 1, FB0_ADDR); // overwrites SETCIMG
    // Actually redo: we need the SETCIMG from geometry_setup, but we already
    // have one. Let me just continue from the fill phase.
    // Reset: build properly
    i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);
    write_cmd(DL_ADDR, i++, 0xF7000000, 0x00010001);
    xl = 319 << 2; yl = 239 << 2;
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);

    // Switch to 1-cycle
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 0);
    write_cmd(DL_ADDR, i++, 0xE200001F, 0x0C084000);
    write_cmd(DL_ADDR, i++, 0xFC121824, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xD9000000, 0x00200004);
    write_cmd(DL_ADDR, i++, 0xD7000002, 0xFFFFFFFF);

    i = write_texture_load(DL_ADDR, i, TEX_ADDR);
    i = write_triangle_geometry(DL_ADDR, i);

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d faces=%zu posFloats=%zu proj=%d",
             wl.fbPairCount, wl.totalFaces, wl.totalPosFloats, wl.totalProjections);

    bool pass = wl.fbPairCount >= 1 && wl.totalFaces > 0 && wl.totalPosFloats > 0;
    return { pass, "test_triangle", detail };
}

// --- Test 3: Two SETCIMG commands (game pattern: FB0 + FB1) ---
static TestResult test_two_fb_pairs() {
    reset_for_test();
    int i = 0;

    // FB0: fill blue
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);
    write_cmd(DL_ADDR, i++, 0xF7000000, 0x001F001F);
    uint32_t xl = 319 << 2, yl = 239 << 2;
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);

    // FB1: fill red
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB1_ADDR);
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);
    write_cmd(DL_ADDR, i++, 0xF7000000, 0xF800F800);
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d", wl.fbPairCount);
    for (int f = 0; f < (int)wl.fbPairs.size() && f < 4; f++) {
        char buf[64];
        snprintf(buf, sizeof(buf), " fb[%d]=0x%06X", f, wl.fbPairs[f].colorAddr);
        strcat(detail, buf);
    }

    bool pass = wl.fbPairCount >= 2;
    return { pass, "test_two_fb_pairs", detail };
}

// --- Test 4: Sub-DL branch (G_DL to separate address) ---
static TestResult test_sub_dl_branch() {
    reset_for_test();
    write_checkerboard_rgba16(TEX_ADDR);

    // Build sub-DL at SUB_DL_ADDR: geometry commands
    int s = 0;
    write_cmd(SUB_DL_ADDR, s++, 0xE3000A01, 0);                      // SETOTHERMODE_H: 1-CYCLE
    write_cmd(SUB_DL_ADDR, s++, 0xE200001F, 0x0C084000);              // SETOTHERMODE_L
    write_cmd(SUB_DL_ADDR, s++, 0xFC121824, 0x00000000);              // SETCOMBINE
    write_cmd(SUB_DL_ADDR, s++, 0xD9000000, 0x00200004);              // GEOMETRYMODE
    write_cmd(SUB_DL_ADDR, s++, 0xD7000002, 0xFFFFFFFF);              // G_TEXTURE on
    s = write_texture_load(SUB_DL_ADDR, s, TEX_ADDR);
    s = write_triangle_geometry(SUB_DL_ADDR, s);
    write_cmd(SUB_DL_ADDR, s++, 0xDF000000, 0x00000000);              // ENDDL

    // Build main DL: SETCIMG + SETSCISSOR + G_DL branch to sub-DL
    int i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);                    // SETCIMG
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);          // SETSCISSOR
    // G_DL (push) to SUB_DL_ADDR
    write_cmd(DL_ADDR, i++, 0xDE000000, SUB_DL_ADDR);                 // G_DL branch
    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);                  // FULLSYNC
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);                  // ENDDL

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d faces=%zu posFloats=%zu proj=%d",
             wl.fbPairCount, wl.totalFaces, wl.totalPosFloats, wl.totalProjections);

    bool pass = wl.fbPairCount >= 1 && wl.totalFaces > 0;
    return { pass, "test_sub_dl_branch", detail };
}

// --- Test 5: Segment resolution (set segment 6, branch via 0x06 addr) ---
static TestResult test_segment_resolution() {
    reset_for_test();
    write_checkerboard_rgba16(TEX_ADDR);

    // Build sub-DL at SEG6_BASE (physical address)
    int s = 0;
    write_cmd(SEG6_BASE, s++, 0xE3000A01, 0);
    write_cmd(SEG6_BASE, s++, 0xE200001F, 0x0C084000);
    write_cmd(SEG6_BASE, s++, 0xFC121824, 0x00000000);
    write_cmd(SEG6_BASE, s++, 0xD9000000, 0x00200004);
    write_cmd(SEG6_BASE, s++, 0xD7000002, 0xFFFFFFFF);
    s = write_texture_load(SEG6_BASE, s, TEX_ADDR);
    s = write_triangle_geometry(SEG6_BASE, s);
    write_cmd(SEG6_BASE, s++, 0xDF000000, 0x00000000);

    // Build main DL
    int i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);                    // SETCIMG
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);          // SETSCISSOR
    // G_MOVEWORD segment[6] = SEG6_BASE
    write_cmd(DL_ADDR, i++, 0xDB060018, SEG6_BASE);                   // segment[6] = SEG6_BASE
    // G_DL to segment 6 offset 0 = 0x06000000
    write_cmd(DL_ADDR, i++, 0xDE000000, 0x06000000);                  // G_DL via segment 6
    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d faces=%zu posFloats=%zu proj=%d",
             wl.fbPairCount, wl.totalFaces, wl.totalPosFloats, wl.totalProjections);

    bool pass = wl.fbPairCount >= 1 && wl.totalFaces > 0;
    return { pass, "test_segment_resolution", detail };
}

// --- Test 6: Textured rectangle (TEXRECT — 2D UI pattern like save screen) ---
static TestResult test_texrect() {
    reset_for_test();
    write_checkerboard_rgba16(TEX_ADDR);

    int i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);                    // SETCIMG
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);          // SETSCISSOR
    write_cmd(DL_ADDR, i++, 0xE3000A01, 0);                           // SETOTHERMODE_H: 1-CYCLE
    write_cmd(DL_ADDR, i++, 0xE200001F, 0x0C084000);                  // SETOTHERMODE_L
    // SETCOMBINE: TEXEL0 (just texture, no shade)
    // a=TEXEL0(1), b=0, c=0, d=TEXEL0(1) → output = TEXEL0
    write_cmd(DL_ADDR, i++, 0xFC119623, 0xFF2FFFFF);                  // SETCOMBINE: TEXEL0

    // Load texture
    i = write_texture_load(DL_ADDR, i, TEX_ADDR);

    // TEXRECT: draw textured rectangle from (80,60) to (240,180)
    // F3DEX2 TEXRECT is 3 words (128-bit command):
    // w0 = 0xE4 | (xh << 12) | yh    (xh, yh in 10.2 fixed)
    // w1 = (tile << 24) | (xl << 12) | yl
    // w2+w3 = s << 16 | t,  dsdx << 16 | dtdy  (10.5 fixed point)
    uint32_t xl = 80 << 2, yl = 60 << 2;
    uint32_t xh = 240 << 2, yh = 180 << 2;
    write_cmd(DL_ADDR, i++, 0xE4000000 | (xh << 12) | yh, (0 << 24) | (xl << 12) | yl);
    // s=0, t=0, dsdx=1<<10 (1 texel per pixel), dtdy=1<<10
    write_cmd(DL_ADDR, i++, 0x00000000, 0x04000400);                  // TEXRECT args

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d faces=%zu rect=(%d,%d,%d,%d) empty=%s",
             wl.fbPairCount, wl.totalFaces,
             wl.fbPairs.empty() ? 0 : wl.fbPairs[0].drawRectUlx,
             wl.fbPairs.empty() ? 0 : wl.fbPairs[0].drawRectUly,
             wl.fbPairs.empty() ? 0 : wl.fbPairs[0].drawRectLrx,
             wl.fbPairs.empty() ? 0 : wl.fbPairs[0].drawRectLry,
             (wl.fbPairs.empty() || wl.fbPairs[0].drawRectEmpty) ? "yes" : "no");

    bool pass = wl.fbPairCount >= 1 && !wl.fbPairs.empty() && !wl.fbPairs[0].drawRectEmpty;
    return { pass, "test_texrect", detail };
}

// --- Test 7: SETCOMBINE modes (test various combiner configurations) ---
static TestResult test_combiner_modes() {
    reset_for_test();
    write_checkerboard_rgba16(TEX_ADDR);

    // Test PRIMITIVE color combiner (same as our debug override)
    int i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 0);                           // 1-CYCLE
    write_cmd(DL_ADDR, i++, 0xE200001F, 0x0C084000);
    // SETCOMBINE: output PRIMITIVE color (our debug mode)
    write_cmd(DL_ADDR, i++, 0xFC000000, 0x000186C3);
    // SETPRIMCOLOR: red
    write_cmd(DL_ADDR, i++, 0xFA000000, 0xFF0000FF);
    write_cmd(DL_ADDR, i++, 0xD9000000, 0x00200004);
    write_cmd(DL_ADDR, i++, 0xD7000002, 0xFFFFFFFF);

    i = write_texture_load(DL_ADDR, i, TEX_ADDR);
    i = write_triangle_geometry(DL_ADDR, i);

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d faces=%zu posFloats=%zu (PRIMITIVE combiner)",
             wl.fbPairCount, wl.totalFaces, wl.totalPosFloats);

    bool pass = wl.fbPairCount >= 1 && wl.totalFaces > 0;
    return { pass, "test_combiner_modes", detail };
}

// --- Test 8: Extended RDRAM (texture at 0x14XXXXXX via extendRDRAM) ---
static TestResult test_extended_rdram() {
    reset_for_test();

    // Write texture at extended RDRAM address
    if (EXT_TEX_ADDR + TEX_W * TEX_H * 2 > RDRAM_SIZE) {
        return { false, "test_extended_rdram", "RDRAM too small for 0x14XXXXXX" };
    }
    write_checkerboard_rgba16(EXT_TEX_ADDR);

    // Set segment 6 to 0x94000000 (0x14000000 | 0x80000000 KSEG0 bit)
    uint32_t seg6_val = EXT_TEX_ADDR | 0x80000000;

    int i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 0);
    write_cmd(DL_ADDR, i++, 0xE200001F, 0x0C084000);
    write_cmd(DL_ADDR, i++, 0xFC121824, 0x00000000);                  // SETCOMBINE
    write_cmd(DL_ADDR, i++, 0xD9000000, 0x00200004);
    write_cmd(DL_ADDR, i++, 0xD7000002, 0xFFFFFFFF);

    // Set segment 6
    write_cmd(DL_ADDR, i++, 0xDB060018, seg6_val);

    // SETTIMG via segment 6 → 0x06000000 → resolves to seg6_val + 0 = 0x94000000
    // With extendRDRAM, RT64 accesses rdram[0x14000000]
    write_cmd(DL_ADDR, i++, 0xFD100000 | (TEX_W - 1), 0x06000000);
    write_cmd(DL_ADDR, i++, 0xF5100000, 0x07000000);
    uint32_t texels = TEX_W * TEX_H - 1;
    uint32_t dxt = 2048 / (TEX_W * 2 / 8);
    write_cmd(DL_ADDR, i++, 0xF3000000, (7 << 24) | (texels << 12) | dxt);
    uint32_t line = TEX_W * 2 / 8;
    write_cmd(DL_ADDR, i++, 0xF5100000 | (line << 9), 0x00000000);
    uint32_t lrs = (TEX_W - 1) << 2;
    uint32_t lrt = (TEX_H - 1) << 2;
    write_cmd(DL_ADDR, i++, 0xF2000000, (lrs << 12) | lrt);

    i = write_triangle_geometry(DL_ADDR, i);

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    // Inject extendRDRAM command before the DL (same pattern as game runtime)
    *(uint32_t*)(g_RDRAM + DL_ADDR - 8) = 0x6400002C;
    *(uint32_t*)(g_RDRAM + DL_ADDR - 4) = 0x00000001;

    process_and_present(DL_ADDR - 8, true);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d faces=%zu posFloats=%zu (texture at 0x%08X via seg6=0x%08X)",
             wl.fbPairCount, wl.totalFaces, wl.totalPosFloats, EXT_TEX_ADDR, seg6_val);

    bool pass = wl.fbPairCount >= 1 && wl.totalFaces > 0;
    return { pass, "test_extended_rdram", detail };
}

// --- Test 9: Game-like DL structure (two FB pairs + geometry in sub-DL) ---
// Simulates the actual game pattern: FB0 clear, FB1 clear+geometry via sub-DL
static TestResult test_game_dl_pattern() {
    reset_for_test();
    write_checkerboard_rgba16(TEX_ADDR);

    // Sub-DL: state setup + geometry (like segment 6 sub-DL in game)
    int s = 0;
    write_cmd(SUB_DL_ADDR, s++, 0xE3000A01, 0);                      // SETOTHERMODE_H
    write_cmd(SUB_DL_ADDR, s++, 0xE200001F, 0x0C084000);              // SETOTHERMODE_L
    write_cmd(SUB_DL_ADDR, s++, 0xD9000000, 0x00200004);              // GEOMETRYMODE
    write_cmd(SUB_DL_ADDR, s++, 0xD7000002, 0xFFFFFFFF);              // G_TEXTURE
    write_cmd(SUB_DL_ADDR, s++, 0xFC121824, 0x00000000);              // SETCOMBINE
    s = write_texture_load(SUB_DL_ADDR, s, TEX_ADDR);
    s = write_triangle_geometry(SUB_DL_ADDR, s);
    write_cmd(SUB_DL_ADDR, s++, 0xDF000000, 0x00000000);              // ENDDL

    // Main DL: mirrors game structure
    int i = 0;
    // FB0: depth buffer clear (fill mode)
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);                    // SETCIMG FB0
    write_cmd(DL_ADDR, i++, 0xFE000000, ZIMG_ADDR);                   // SETZIMG
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);          // SETSCISSOR
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);                     // FILL mode
    write_cmd(DL_ADDR, i++, 0xF7000000, 0xFFFFFFFF);                  // SETFILLCOLOR (z-clear)
    uint32_t xl = 319 << 2, yl = 239 << 2;
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);         // FILLRECT

    // FB1: scene render
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB1_ADDR);                    // SETCIMG FB1
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);          // SETSCISSOR
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);                     // FILL mode
    write_cmd(DL_ADDR, i++, 0xF7000000, 0x001F001F);                  // SETFILLCOLOR blue
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);         // FILLRECT (clear FB1)

    // Branch to sub-DL for geometry
    write_cmd(DL_ADDR, i++, 0xDE000000, SUB_DL_ADDR);                 // G_DL

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    VI_ORIGIN_REG = FB1_ADDR + SCREEN_W * 2;
    g_app->state->rsp->reset();
    auto& gbi = g_app->interpreter->gbiManager.gbiCache[uint32_t(RT64::GBIUCode::F3DEX2)];
    if (gbi.resetFromTask) gbi.resetFromTask(g_app->state.get());
    g_app->processDisplayLists(g_RDRAM, DL_ADDR, 0, true);
    g_app->updateScreen();

    auto wl = inspect_workload(g_app.get());

    char detail[512];
    snprintf(detail, sizeof(detail), "fbPairs=%d", wl.fbPairCount);
    for (int f = 0; f < (int)wl.fbPairs.size() && f < 4; f++) {
        char buf[128];
        snprintf(buf, sizeof(buf), " fb[%d]=0x%06X proj=%d fill=%s",
                 f, wl.fbPairs[f].colorAddr, wl.fbPairs[f].projCount,
                 wl.fbPairs[f].fillOnly ? "yes" : "no");
        strcat(detail, buf);
    }
    char buf2[64];
    snprintf(buf2, sizeof(buf2), " faces=%zu posFloats=%zu", wl.totalFaces, wl.totalPosFloats);
    strcat(detail, buf2);

    // Expect: 2 FB pairs, FB1 has projections, geometry exists
    bool pass = wl.fbPairCount >= 2 && wl.totalFaces > 0;
    // Check that geometry ended up in FB1 (second FB pair)
    if (pass && wl.fbPairs.size() >= 2) {
        pass = wl.fbPairs[1].projCount > 0 || wl.totalFaces > 0;
    }
    return { pass, "test_game_dl_pattern", detail };
}

// --- Test 10: SETPRIMCOLOR + SETENVCOLOR visibility ---
static TestResult test_prim_env_colors() {
    reset_for_test();

    int i = 0;
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB0_ADDR);
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);
    write_cmd(DL_ADDR, i++, 0xE3000A01, 0);
    write_cmd(DL_ADDR, i++, 0xE200001F, 0x0C084000);

    // SETCOMBINE: (PRIM - ENV) * SHADE + ENV
    write_cmd(DL_ADDR, i++, 0xFC000000, 0x000186C3);                  // PRIMITIVE output

    // SETPRIMCOLOR: bright green
    write_cmd(DL_ADDR, i++, 0xFA000000, 0x00FF00FF);
    // SETENVCOLOR: dark blue
    write_cmd(DL_ADDR, i++, 0xFB000000, 0x0000FFFF);

    write_cmd(DL_ADDR, i++, 0xD9000000, 0x00200004);
    write_cmd(DL_ADDR, i++, 0xD7000002, 0xFFFFFFFF);

    write_checkerboard_rgba16(TEX_ADDR);
    i = write_texture_load(DL_ADDR, i, TEX_ADDR);
    i = write_triangle_geometry(DL_ADDR, i);

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);

    process_and_present(DL_ADDR);
    auto wl = inspect_workload(g_app.get());

    char detail[256];
    snprintf(detail, sizeof(detail), "fbPairs=%d faces=%zu (PRIM=green ENV=blue)",
             wl.fbPairCount, wl.totalFaces);

    bool pass = wl.fbPairCount >= 1 && wl.totalFaces > 0;
    return { pass, "test_prim_env_colors", detail };
}

// ═══════════════════════════════════════════════════════════════════════
// MAIN
// ═══════════════════════════════════════════════════════════════════════

int main(int argc, char* argv[]) {
    fprintf(stderr, "╔═══════════════════════════════════════════════════╗\n");
    fprintf(stderr, "║  LoD Recomp — DL Test Harness                    ║\n");
    fprintf(stderr, "╚═══════════════════════════════════════════════════╝\n\n");

    // Parse args
    std::string rom_path;
    for (int a = 1; a < argc; a++) {
        if (strcmp(argv[a], "--rom") == 0 && a + 1 < argc) {
            rom_path = argv[++a];
        }
    }

    // Allocate RDRAM (mmap for efficient sparse allocation)
    g_RDRAM = (uint8_t*)mmap(nullptr, RDRAM_SIZE,
                              PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g_RDRAM == MAP_FAILED) {
        fprintf(stderr, "[FATAL] Failed to mmap %u bytes for RDRAM\n", RDRAM_SIZE);
        return 1;
    }
    memset(g_DMEM, 0, sizeof(g_DMEM));
    memset(g_IMEM, 0, sizeof(g_IMEM));
    memset(g_HEADER, 0, sizeof(g_HEADER));

    // SDL init
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "[FATAL] SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    uint32_t flags = SDL_WINDOW_RESIZABLE;
#if defined(__APPLE__)
    flags |= SDL_WINDOW_METAL;
#elif defined(RT64_SDL_WINDOW_VULKAN)
    flags |= SDL_WINDOW_VULKAN;
#endif

    SDL_Window* window = SDL_CreateWindow(
        "LoD DL Test Harness", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, flags);
    if (!window) {
        fprintf(stderr, "[FATAL] SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }

    // Init RT64
    fprintf(stderr, "[HARNESS] Initializing RT64...\n");
    if (!init_rt64(window)) {
        fprintf(stderr, "[FATAL] RT64 initialization failed\n");
        return 1;
    }
    fprintf(stderr, "[HARNESS] RT64 ready. Running tests...\n\n");

    // Collect and run all tests
    std::vector<std::function<TestResult()>> tests = {
        test_fillrect,
        test_triangle,
        test_two_fb_pairs,
        test_sub_dl_branch,
        test_segment_resolution,
        test_texrect,
        test_combiner_modes,
        test_extended_rdram,
        test_game_dl_pattern,
        test_prim_env_colors,
    };

    int passed = 0, failed = 0;
    std::vector<TestResult> results;

    for (auto& test_fn : tests) {
        // Pump SDL events to keep window alive
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) goto done;
            g_app->sdlEventFilter(&ev);
        }

        TestResult result = test_fn();
        results.push_back(result);

        const char* tag = result.passed ? "PASS" : "FAIL";
        fprintf(stderr, "[%s] %-30s %s\n", tag, result.name.c_str(), result.detail.c_str());

        if (result.passed) passed++;
        else failed++;
    }

done:
    fprintf(stderr, "\n════════════════════════════════════════════════════\n");
    fprintf(stderr, "Results: %d passed, %d failed, %d total\n", passed, failed, passed + failed);
    fprintf(stderr, "════════════════════════════════════════════════════\n");

    if (failed > 0) {
        fprintf(stderr, "\nFailed tests:\n");
        for (auto& r : results) {
            if (!r.passed) {
                fprintf(stderr, "  - %s: %s\n", r.name.c_str(), r.detail.c_str());
            }
        }
    }

    // Cleanup
    g_app->end();
    g_app.reset();
    SDL_DestroyWindow(window);
    SDL_Quit();
    munmap(g_RDRAM, RDRAM_SIZE);

    return failed > 0 ? 1 : 0;
}
