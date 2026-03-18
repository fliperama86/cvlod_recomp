// Minimal RT64 sanity check: red triangle
// No game code, no ultramodern, no librecomp — just SDL + RT64 directly.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

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
#include "gbi/rt64_gbi_rdp.h"
#include "gbi/rt64_gbi_f3dex2.h"

// ── Constants ────────────────────────────────────────────────────────
static constexpr uint32_t SCREEN_W     = 320;
static constexpr uint32_t SCREEN_H     = 240;
static constexpr uint32_t RDRAM_SIZE   = 8 * 1024 * 1024;
static constexpr uint32_t FB_ADDR      = 0x100000;
static constexpr uint32_t DL_ADDR      = 0x200000;
static constexpr uint32_t VP_ADDR      = 0x300000;
static constexpr uint32_t PROJ_ADDR    = 0x300100;
static constexpr uint32_t MODEL_ADDR   = 0x300200;
static constexpr uint32_t VTX_ADDR     = 0x300300;
static constexpr uint32_t TEX_ADDR     = 0x300400;
static constexpr uint32_t TEX_W        = 16;       // 16×16 checkerboard
static constexpr uint32_t TEX_H        = 16;

// ── Dummy N64 registers ──────────────────────────────────────────────
static uint8_t  g_RDRAM[RDRAM_SIZE];
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

// VI registers — NTSC 320×240 16-bit
static uint32_t VI_STATUS_REG     = 0x0000030A;
static uint32_t VI_ORIGIN_REG     = FB_ADDR;
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

// ── Helpers to write N64 data structures into RDRAM ──────────────────
// RT64 reads all data as native-endian structs, but with pair-swapped
// 16-bit fields within each 32-bit word (N64 big-endian legacy).

static void write_cmd(uint32_t addr, int idx, uint32_t w0, uint32_t w1) {
    auto* dl = reinterpret_cast<uint32_t*>(g_RDRAM + addr + idx * 8);
    dl[0] = w0;
    dl[1] = w1;
}

// Write an identity FixedMatrix into RDRAM.
// FixedMatrix = int16_t integer[4][4] + uint16_t frac[4][4] = 64 bytes.
// RT64 reads column j as integer[i][j^1] (pair-swap within 32-bit words).
static void write_identity_matrix(uint32_t addr) {
    auto* mtx = reinterpret_cast<RT64::FixedMatrix*>(g_RDRAM + addr);
    memset(mtx, 0, sizeof(RT64::FixedMatrix));
    // For identity: toFloat(i,j)=1 when i==j.
    // toFloat reads integer[i][j^1], so integer[i][i^1] = 1.
    mtx->integer[0][0 ^ 1] = 1; // [0][1]
    mtx->integer[1][1 ^ 1] = 1; // [1][0]
    mtx->integer[2][2 ^ 1] = 1; // [2][3]
    mtx->integer[3][3 ^ 1] = 1; // [3][2]
}

// Write a Vp_t (viewport) into RDRAM.
// RT64 reads: scale.x = vscale[1]/4, scale.y = vscale[0]/4, scale.z = vscale[3]/range
//             trans.x = vtrans[1]/4,  trans.y = vtrans[0]/4,  trans.z = vtrans[3]/range
static void write_viewport(uint32_t addr, float sx, float sy, float tx, float ty) {
    auto* vp = reinterpret_cast<int16_t*>(g_RDRAM + addr);
    memset(vp, 0, 16); // Vp_t is 16 bytes (8 x int16_t)
    // vscale: [0]=y, [1]=x, [2]=unused, [3]=z (pair-swapped)
    vp[0] = int16_t(sy * 4.0f);  // y scale
    vp[1] = int16_t(sx * 4.0f);  // x scale
    vp[2] = 0;
    vp[3] = int16_t(512);         // z scale (512/1024=0.5 → Z range [0,1])
    // vtrans: [4]=y, [5]=x, [6]=unused, [7]=z
    vp[4] = int16_t(ty * 4.0f);  // y translate
    vp[5] = int16_t(tx * 4.0f);  // x translate
    vp[6] = 0;
    vp[7] = int16_t(512);         // z translate (0.5 → center of depth range)
}

// Write an N64 Vertex into RDRAM.
// Vertex struct: {y, x, flag, z, t, s, color{a, b, g, r}} = 16 bytes.
// Fields are pair-swapped within 32-bit words.
// s, t are in 10.5 fixed point (pixel_coord << 5).
static void write_vertex(uint32_t addr, int idx,
                         int16_t x, int16_t y, int16_t z,
                         int16_t s, int16_t t,
                         uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    auto* v = reinterpret_cast<uint8_t*>(g_RDRAM + addr + idx * 16);
    auto* s16 = reinterpret_cast<int16_t*>(v);
    auto* u16 = reinterpret_cast<uint16_t*>(v);
    s16[0] = y;     // y (pair-swapped: [0] = y, [1] = x)
    s16[1] = x;     // x
    u16[2] = 0;     // flag (pair-swapped: [2] = flag, [3] = z)
    s16[3] = z;     // z
    s16[4] = t;     // t (pair-swapped: [4] = t, [5] = s)
    s16[5] = s;     // s
    v[12] = a;       // color: a, b, g, r (byte-swapped within 32-bit word)
    v[13] = b;
    v[14] = g;
    v[15] = r;
}

// Write a 16×16 RGBA16 checkerboard texture into RDRAM.
// Each pixel is 16 bits: RRRR RGGG GGBB BBBA.
static void write_checkerboard_rgba16(uint32_t addr) {
    auto* tex = reinterpret_cast<uint16_t*>(g_RDRAM + addr);
    for (uint32_t row = 0; row < TEX_H; row++) {
        for (uint32_t col = 0; col < TEX_W; col++) {
            bool white = ((row / 4) ^ (col / 4)) & 1;
            // RGBA16: R5 G5 B5 A1
            tex[row * TEX_W + col] = white ? 0xFFFF : 0xF801; // white or red, alpha=1
        }
    }
}

// ── Build display list: black fill-rect clear + red triangle ─────────
static int build_triangle_dl() {
    // Viewport: scale=(80,60) so NDC ±1 → screen range 80..240 x, 60..180 y
    // translate=(160,120) = screen center
    write_viewport(VP_ADDR, 80.0f, 60.0f, 160.0f, 120.0f);
    write_identity_matrix(PROJ_ADDR);
    write_identity_matrix(MODEL_ADDR);

    // With identity MVP, viewport scale=(80,60), translate=(160,120):
    //   screen_x = clip_x * 80 + 160,  screen_y = clip_y * (-60) + 120
    // Vertices in NDC:  v0=(0,-1) → screen(160,180),
    //                   v1=(-1,1) → screen(80,60),
    //                   v2=(1,1)  → screen(240,60)
    // S/T in 10.5 fixed: 16 pixels = 16<<5 = 512
    int16_t texMax = TEX_W << 5;   // 512 in 10.5 fixed
    write_vertex(VTX_ADDR, 0,  0, -1, 0,  texMax/2, texMax,  255, 0,   0,   255);  // bottom center (red)
    write_vertex(VTX_ADDR, 1, -1,  1, 0,  0,        0,       0,   255, 0,   255);  // top left (green)
    write_vertex(VTX_ADDR, 2,  1,  1, 0,  texMax,   0,       0,   0,   255, 255);  // top right (blue)

    // Write checkerboard texture
    write_checkerboard_rgba16(TEX_ADDR);

    int i = 0;

    // === Phase 1: Fill mode — clear to black, establishes the FB pair ===
    write_cmd(DL_ADDR, i++, 0xFF10013F, FB_ADDR);                  // SETCIMG
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);      // SETSCISSOR
    write_cmd(DL_ADDR, i++, 0xE3000A01, 3 << 20);                  // SETOTHERMODE_H: FILL
    write_cmd(DL_ADDR, i++, 0xF7000000, 0x001F001F);               // SETFILLCOLOR: blue
    uint32_t xl = 319 << 2, yl = 239 << 2;
    write_cmd(DL_ADDR, i++, 0xF6000000 | (xl << 12) | yl, 0);     // FILLRECT full screen

    // === Phase 2: 1-cycle mode — draw textured triangle ===
    // (same SETCIMG = same FB pair, triangle overlays the cleared buffer)
    write_cmd(DL_ADDR, i++, 0xED000000, (1280 << 12) | 960);      // SETSCISSOR

    // SETOTHERMODE_H: 1-CYCLE + bilinear filter (G_TF_BILERP at bits[13:12])
    // Default H=0x080CFF already has G_CYC_1CYCLE. Set explicitly for clarity:
    // bits[21:20]=00 (1CYCLE), bits[19]=1 (persp correct), bits[13:12]=10 (bilerp)
    write_cmd(DL_ADDR, i++, 0xE3000A01, 0);                        // SETOTHERMODE_H: 1-CYCLE
    write_cmd(DL_ADDR, i++, 0xE200001F, 0x0C084000);               // SETOTHERMODE_L

    // SETCOMBINE: TEXEL0 * SHADE (texture modulated by vertex color)
    // Color: (TEXEL0 - 0) * SHADE + 0  →  a=TEXEL0(1), b=0, c=SHADE(4), d=0
    // Alpha: same
    // w0: a0=1@[23:20], c0=4@[19:15], Aa0=1@[14:12], Ac0=4@[11:9], a1/c1 same @[4:0]
    // w1: b=d=0 (COMBINED) → all zero
    write_cmd(DL_ADDR, i++, 0xFC121824, 0x00000000);               // SETCOMBINE: TEXEL0*SHADE

    // SETGEOMETRYMODE: G_SHADE | G_SHADING_SMOOTH
    write_cmd(DL_ADDR, i++, 0xD9000000, 0x00200004);

    // G_TEXTURE: enable texturing, tile 0, scale s/t = 0xFFFF (1:1)
    // F3DEX2 G_TEXTURE = 0xD7, format: w0 = 0xD7000002 (on=1, tile=0, level=0)
    //                                  w1 = (scaleS << 16) | scaleT
    write_cmd(DL_ADDR, i++, 0xD7000002, 0xFFFFFFFF);               // G_TEXTURE on

    // Load the 16×16 RGBA16 texture into TMEM
    // SETTIMG: fmt=RGBA, siz=16b, width=16, addr=TEX_ADDR
    write_cmd(DL_ADDR, i++, 0xFD100000 | (TEX_W - 1), TEX_ADDR);  // SETTIMG

    // SETTILE (for load): fmt=RGBA, siz=16b, line=0, tmem=0, tile=7 (load tile)
    // w0 = 0xF5 | (fmt<<21) | (siz<<19) | (line<<9) | (tmem<<0)
    //    = 0xF5 | (0<<21) | (2<<19) | (0<<9) | 0 = 0xF5100000
    // w1 = tile7 (palette=0, etc) = (7<<24)
    write_cmd(DL_ADDR, i++, 0xF5100000, 0x07000000);               // SETTILE load (tile 7)

    // LOADBLOCK: tile=7, uls=0, ult=0, lrs=TEX_W*TEX_H-1, dxt
    // dxt = (1<<11) / (width_in_64bit_words) = 2048 / (16*2/8) = 512
    {
        uint32_t texels = TEX_W * TEX_H - 1;  // 255
        uint32_t dxt = 2048 / (TEX_W * 2 / 8); // = 512
        write_cmd(DL_ADDR, i++, 0xF3000000, (7 << 24) | (texels << 12) | dxt); // LOADBLOCK
    }

    // SETTILE (for render): fmt=RGBA, siz=16b, line=TEX_W/4=4, tmem=0, tile=0
    // w0 = 0xF5 | (0<<21) | (2<<19) | (line<<9) | 0
    {
        uint32_t line = TEX_W * 2 / 8;  // 16*2/8 = 4 (64-bit words per row)
        write_cmd(DL_ADDR, i++, 0xF5100000 | (line << 9), 0x00000000); // SETTILE render (tile 0)
    }

    // SETTILESIZE: tile=0, uls=0, ult=0, lrs=(TEX_W-1)<<2, lrt=(TEX_H-1)<<2
    {
        uint32_t lrs = (TEX_W - 1) << 2;
        uint32_t lrt = (TEX_H - 1) << 2;
        write_cmd(DL_ADDR, i++, 0xF2000000, (lrs << 12) | lrt);       // SETTILESIZE
    }

    write_cmd(DL_ADDR, i++, 0xDC080008, VP_ADDR);                  // MOVEMEM VP
    write_cmd(DL_ADDR, i++, 0xDA380007, PROJ_ADDR);                // MTX: projection
    write_cmd(DL_ADDR, i++, 0xDA380003, MODEL_ADDR);               // MTX: modelview
    write_cmd(DL_ADDR, i++, 0x01003006, VTX_ADDR);                 // VTX: 3 verts at 0
    write_cmd(DL_ADDR, i++, 0x05000204, 0x00000000);               // TRI1: (0,1,2)

    write_cmd(DL_ADDR, i++, 0xE9000000, 0x00000000);               // FULLSYNC
    write_cmd(DL_ADDR, i++, 0xDF000000, 0x00000000);               // ENDDL

    return i;
}

// ── Main ─────────────────────────────────────────────────────────────
int main(int /*argc*/, char* /*argv*/[]) {
    fprintf(stderr, "[sanity] Starting RT64 sanity check — red triangle...\n");

    // ── SDL init ─────────────────────────────────────────────────────
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "[sanity] SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    uint32_t flags = SDL_WINDOW_RESIZABLE;
#if defined(__APPLE__)
    flags |= SDL_WINDOW_METAL;
#elif defined(RT64_SDL_WINDOW_VULKAN)
    flags |= SDL_WINDOW_VULKAN;
#endif

    SDL_Window* window = SDL_CreateWindow(
        "RT64 Sanity Check — Triangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, flags
    );
    if (!window) {
        fprintf(stderr, "[sanity] SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }
    fprintf(stderr, "[sanity] Window created.\n");

    // ── Platform window handle ───────────────────────────────────────
    plume::RenderWindow renderWindow{};

#if defined(_WIN32)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    renderWindow = wmInfo.info.win.window;
#elif defined(__APPLE__)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    SDL_MetalView metalView = SDL_Metal_CreateView(window);
    renderWindow.window = wmInfo.info.cocoa.window;
    renderWindow.view   = SDL_Metal_GetLayer(metalView);
#else
    renderWindow = window;
#endif

    // ── Clear RDRAM ──────────────────────────────────────────────────
    memset(g_RDRAM, 0, RDRAM_SIZE);
    memset(g_DMEM, 0, sizeof(g_DMEM));
    memset(g_IMEM, 0, sizeof(g_IMEM));
    memset(g_HEADER, 0, sizeof(g_HEADER));

    // ── Build RT64::Application::Core ────────────────────────────────
    RT64::Application::Core appCore{};
    appCore.window          = renderWindow;
    appCore.checkInterrupts = dummy_check_interrupts;
    appCore.HEADER          = g_HEADER;
    appCore.RDRAM           = g_RDRAM;
    appCore.DMEM            = g_DMEM;
    appCore.IMEM            = g_IMEM;

    appCore.MI_INTR_REG      = &MI_INTR_REG;
    appCore.DPC_START_REG    = &DPC_START_REG;
    appCore.DPC_END_REG      = &DPC_END_REG;
    appCore.DPC_CURRENT_REG  = &DPC_CURRENT_REG;
    appCore.DPC_STATUS_REG   = &DPC_STATUS_REG;
    appCore.DPC_CLOCK_REG    = &DPC_CLOCK_REG;
    appCore.DPC_BUFBUSY_REG  = &DPC_BUFBUSY_REG;
    appCore.DPC_PIPEBUSY_REG = &DPC_PIPEBUSY_REG;
    appCore.DPC_TMEM_REG     = &DPC_TMEM_REG;

    appCore.VI_STATUS_REG         = &VI_STATUS_REG;
    appCore.VI_ORIGIN_REG         = &VI_ORIGIN_REG;
    appCore.VI_WIDTH_REG          = &VI_WIDTH_REG;
    appCore.VI_INTR_REG           = &VI_INTR_REG;
    appCore.VI_V_CURRENT_LINE_REG = &VI_V_CURRENT_REG;
    appCore.VI_TIMING_REG         = &VI_TIMING_REG;
    appCore.VI_V_SYNC_REG         = &VI_V_SYNC_REG;
    appCore.VI_H_SYNC_REG         = &VI_H_SYNC_REG;
    appCore.VI_LEAP_REG           = &VI_LEAP_REG;
    appCore.VI_H_START_REG        = &VI_H_START_REG;
    appCore.VI_V_START_REG        = &VI_V_START_REG;
    appCore.VI_V_BURST_REG        = &VI_V_BURST_REG;
    appCore.VI_X_SCALE_REG        = &VI_X_SCALE_REG;
    appCore.VI_Y_SCALE_REG        = &VI_Y_SCALE_REG;

    // ── Create RT64 application ──────────────────────────────────────
    RT64::ApplicationConfiguration appConfig;
    appConfig.useConfigurationFile = false;

    fprintf(stderr, "[sanity] Creating RT64::Application...\n");
    auto app = std::make_unique<RT64::Application>(appCore, appConfig);

    app->userConfig.graphicsAPI  = RT64::UserConfiguration::GraphicsAPI::Automatic;
    app->userConfig.resolution   = RT64::UserConfiguration::Resolution::Original;
    app->userConfig.antialiasing = RT64::UserConfiguration::Antialiasing::None;
    app->userConfig.refreshRate  = RT64::UserConfiguration::RefreshRate::Original;
    app->userConfig.developerMode = true;

    fprintf(stderr, "[sanity] Calling app->setup()...\n");
    auto result = app->setup(0);
    if (result != RT64::Application::SetupResult::Success) {
        fprintf(stderr, "[sanity] RT64 setup failed (result=%d)\n", (int)result);
        return 1;
    }
    fprintf(stderr, "[sanity] RT64 setup OK!\n");

    // ── Manually configure F3DEX2 GBI ────────────────────────────────
    {
        auto& gbi = app->interpreter->gbiManager.gbiCache[uint32_t(RT64::GBIUCode::F3DEX2)];
        if (gbi.ucode == RT64::GBIUCode::Unknown) {
            gbi.ucode = RT64::GBIUCode::F3DEX2;
            RT64::GBI_RDP::setup(&gbi, true);
            RT64::GBI_F3DEX2::setup(&gbi);
        }
        app->interpreter->hleGBI = &gbi;
        app->state->rsp->setGBI(&gbi);
        if (gbi.resetFromTask) {
            gbi.resetFromTask(app->state.get());
        }
        fprintf(stderr, "[sanity] F3DEX2 GBI configured manually.\n");
    }

    // ── Build display list ───────────────────────────────────────────
    int dl_cmds = build_triangle_dl();

    // Debug: verify data structures in RDRAM
    {
        auto* proj = reinterpret_cast<RT64::FixedMatrix*>(g_RDRAM + PROJ_ADDR);
        auto pm = proj->toMatrix4x4();
        fprintf(stderr, "[debug] Proj matrix:\n");
        for (int r = 0; r < 4; r++)
            fprintf(stderr, "  [%d] %8.4f %8.4f %8.4f %8.4f\n", r,
                    (float)pm[r][0], (float)pm[r][1], (float)pm[r][2], (float)pm[r][3]);

        auto* model = reinterpret_cast<RT64::FixedMatrix*>(g_RDRAM + MODEL_ADDR);
        auto mm = model->toMatrix4x4();
        fprintf(stderr, "[debug] Model matrix:\n");
        for (int r = 0; r < 4; r++)
            fprintf(stderr, "  [%d] %8.4f %8.4f %8.4f %8.4f\n", r,
                    (float)mm[r][0], (float)mm[r][1], (float)mm[r][2], (float)mm[r][3]);

        auto* vp = reinterpret_cast<int16_t*>(g_RDRAM + VP_ADDR);
        fprintf(stderr, "[debug] Viewport: vscale=[%d,%d,%d,%d] vtrans=[%d,%d,%d,%d]\n",
                vp[0], vp[1], vp[2], vp[3], vp[4], vp[5], vp[6], vp[7]);

        for (int v = 0; v < 3; v++) {
            auto* vtx = reinterpret_cast<int16_t*>(g_RDRAM + VTX_ADDR + v * 16);
            auto* col = reinterpret_cast<uint8_t*>(g_RDRAM + VTX_ADDR + v * 16 + 12);
            fprintf(stderr, "[debug] Vtx[%d]: y=%d x=%d z=%d  rgba=(%d,%d,%d,%d)\n",
                    v, vtx[0], vtx[1], vtx[3], col[3], col[2], col[1], col[0]);
        }
    }
    fprintf(stderr, "[sanity] Display list built: %d commands at RDRAM 0x%06X\n", dl_cmds, DL_ADDR);

    // ── Main loop ────────────────────────────────────────────────────
    fprintf(stderr, "[sanity] Entering main loop. Close window or press Escape to exit.\n");
    bool running = true;
    int frame = 0;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) running = false;
            app->sdlEventFilter(&ev);
        }

        app->state->rsp->reset();
        {
            auto& gbi = app->interpreter->gbiManager.gbiCache[uint32_t(RT64::GBIUCode::F3DEX2)];
            if (gbi.resetFromTask) gbi.resetFromTask(app->state.get());
        }
        app->processDisplayLists(g_RDRAM, DL_ADDR, 0, true);

        // VI_ORIGIN points 1 row past FB start (N64 convention).
        // RT64's VI::fbAddress() = origin - width*2, so this yields FB_ADDR.
        VI_ORIGIN_REG = FB_ADDR + SCREEN_W * 2;
        app->updateScreen();

        frame++;
        if (frame <= 3 || frame % 60 == 0) {
            fprintf(stderr, "[sanity] Frame %d presented.\n", frame);
        }
    }

    fprintf(stderr, "[sanity] Shutting down...\n");
    app->end();
    app.reset();
#if defined(__APPLE__)
    // Metal cleanup: destroy view before window
    SDL_Metal_DestroyView(metalView);
#endif
    SDL_DestroyWindow(window);
    SDL_Quit();
    fprintf(stderr, "[sanity] Done.\n");
    return 0;
}
