cmake_minimum_required(VERSION 3.20)

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version")
endif()

project(LodRecomp LANGUAGES C CXX)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (APPLE)
    enable_language(OBJC OBJCXX)
endif()

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ── rt64 ─────────────────────────────────────────────────────────────
set(RT64_STATIC TRUE)
set(RT64_SDL_WINDOW_VULKAN TRUE)
add_compile_definitions(HLSL_CPU)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/rt64 ${CMAKE_BINARY_DIR}/rt64)

# ── lunasvg + RmlUi (required by rt64 build chain) ──────────────────
set(BUILD_SHARED_LIBS OFF)
SET(LUNASVG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/lunasvg)

SET(RMLUI_SVG_PLUGIN ON CACHE BOOL "" FORCE)
SET(RMLUI_TESTS_ENABLED OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/RmlUi)
target_compile_definitions(rmlui_core PRIVATE LUNASVG_BUILD_STATIC)

# ── N64ModernRuntime (ultramodern + librecomp) ──────────────────────
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/N64ModernRuntime)

target_include_directories(rt64 PRIVATE ${CMAKE_BINARY_DIR}/rt64/src)

option(LOD_FIX_SEGMENT_86_BASES "Normalize LoD RT64 segment bases in the invalid 0x86xxxxxx range to 0x80xxxxxx" ON)
if (LOD_FIX_SEGMENT_86_BASES)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_SEGMENT_86_BASES=1)
endif()
option(LOD_ENABLE_RENDER_ADDR_TRACE "Enable throttled RT64 address/display-list diagnostics for LoD rendering glitches" OFF)
if (LOD_ENABLE_RENDER_ADDR_TRACE)
    target_compile_definitions(rt64 PRIVATE LOD_ENABLE_RENDER_ADDR_TRACE=1)
endif()
option(LOD_ENABLE_RENDER_GEOM_TRACE "Enable throttled RT64 geometry/display-list diagnostics for LoD model rendering glitches" OFF)
if (LOD_ENABLE_RENDER_GEOM_TRACE)
    target_compile_definitions(rt64 PRIVATE LOD_ENABLE_RENDER_GEOM_TRACE=1)
endif()
option(LOD_ENABLE_GBI_MISS_TRACE "Enable RT64 graphics microcode/GBI miss diagnostics and skip the null-HLE crash path" OFF)
if (LOD_ENABLE_GBI_MISS_TRACE)
    target_compile_definitions(rt64 PRIVATE LOD_ENABLE_GBI_MISS_TRACE=1)
endif()
option(LOD_ENABLE_RUN_DL_SUSPICIOUS_TRACE "Enable targeted RT64 G_DL parent diagnostics for suspicious NI display-list targets" OFF)
if (LOD_ENABLE_RUN_DL_SUSPICIOUS_TRACE)
    target_compile_definitions(rt64 PRIVATE LOD_ENABLE_RUN_DL_SUSPICIOUS_TRACE=1)
endif()
option(LOD_FIX_INVALID_GBI_LOAD_UCODE "Treat RT64 LOAD_UCODE commands with no RDPHALF_1 data pointer as invalid no-ops" ON)
if (LOD_FIX_INVALID_GBI_LOAD_UCODE)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_INVALID_GBI_LOAD_UCODE=1)
endif()
option(LOD_FIX_INVALID_GBI_LOAD_UCODE_END_DL "End the current RT64 display list when an invalid LOAD_UCODE has no RDPHALF_1 data pointer" ON)
if (LOD_FIX_INVALID_GBI_LOAD_UCODE_END_DL)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_INVALID_GBI_LOAD_UCODE_END_DL=1)
endif()
option(LOD_FIX_PRESERVE_GBI_ON_LOAD_MISS "Preserve the previous RT64 GBI when a mid-display-list LOAD_UCODE cannot be matched" ON)
if (LOD_FIX_PRESERVE_GBI_ON_LOAD_MISS)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_PRESERVE_GBI_ON_LOAD_MISS=1)
endif()
option(LOD_FIX_RUN_DL_USE_GBI_MAP "Validate RT64 sub-display-list targets against the active GBI opcode map instead of a broad numeric opcode range" ON)
if (LOD_FIX_RUN_DL_USE_GBI_MAP)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_RUN_DL_USE_GBI_MAP=1)
endif()
option(LOD_FIX_RUN_DL_NI_BOUNDS "Validate LoD NI 0x8E/0x8F display-list targets against the currently loaded overlay span before RT64 dereferences them" ON)
if (LOD_FIX_RUN_DL_NI_BOUNDS)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_RUN_DL_NI_BOUNDS=1)
endif()
option(LOD_ENABLE_RENDER_CLUSTER_TINT "Tint LoD render geometry clusters selected by render diagnostics" OFF)
if (LOD_ENABLE_RENDER_CLUSTER_TINT)
    target_compile_definitions(rt64 PRIVATE LOD_ENABLE_RENDER_CLUSTER_TINT=1)
endif()
option(LOD_FIX_DIRECT_SEGMENT_86_ADDRS "Interpret direct LoD 0x86xxxxxx display-list operands as segment-6 references" OFF)
if (LOD_FIX_DIRECT_SEGMENT_86_ADDRS)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_DIRECT_SEGMENT_86_ADDRS=1)
endif()
option(LOD_FIX_NI_RDP_EXTENDED_ADDRS "Preserve LoD NI 0x8E/0x8F addresses in RDP image/texture state" OFF)
if (LOD_FIX_NI_RDP_EXTENDED_ADDRS)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_NI_RDP_EXTENDED_ADDRS=1)
endif()
option(LOD_FIX_NI_RSP_EXTENDED_ADDRS "Preserve LoD NI 0x8E/0x8F addresses in RSP physical-data address paths" OFF)
if (LOD_FIX_NI_RSP_EXTENDED_ADDRS)
    target_compile_definitions(rt64 PRIVATE LOD_FIX_NI_RSP_EXTENDED_ADDRS=1)
endif()

# ── RecompiledFuncs ─────────────────────────────────────────────────
add_library(RecompiledFuncs STATIC)

target_compile_options(RecompiledFuncs PRIVATE
    -fno-strict-aliasing
    -Wno-unused-variable
    -Wno-implicit-function-declaration
)

option(LOD_ENABLE_FIGURE_SEG_TRACE "Enable generated-code diagnostics for figure/model links that still contain segmented addresses" OFF)
if (LOD_ENABLE_FIGURE_SEG_TRACE)
    target_compile_definitions(RecompiledFuncs PRIVATE LOD_ENABLE_FIGURE_SEG_TRACE=1)
endif()

target_include_directories(RecompiledFuncs PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/N64ModernRuntime/ultramodern/include
    ${CMAKE_SOURCE_DIR}/lib/N64ModernRuntime/librecomp/include
    ${CMAKE_SOURCE_DIR}/lib/N64ModernRuntime/N64Recomp/include
)

file(GLOB FUNC_C_SOURCES ${CMAKE_SOURCE_DIR}/RecompiledFuncs/*.c)
file(GLOB FUNC_CXX_SOURCES ${CMAKE_SOURCE_DIR}/RecompiledFuncs/*.cpp)
target_sources(RecompiledFuncs PRIVATE ${FUNC_C_SOURCES} ${FUNC_CXX_SOURCES})

# ── Main executable ─────────────────────────────────────────────────
add_executable(LodRecomp)

option(LOD_FIX_NI_PERSIST_OVERLAY_DATA "Persist mutable NI overlay segment data across pair swaps" ON)
if (LOD_FIX_NI_PERSIST_OVERLAY_DATA)
    target_compile_definitions(LodRecomp PRIVATE LOD_FIX_NI_PERSIST_OVERLAY_DATA=1)
endif()
option(LOD_FIX_NI_SEG6_CPU_ALIAS "Mirror active NI overlay segment data into the CPU 0x06xxxxxx alias region for segmented model pointers" OFF)
if (LOD_FIX_NI_SEG6_CPU_ALIAS)
    target_compile_definitions(LodRecomp PRIVATE LOD_FIX_NI_SEG6_CPU_ALIAS=1)
endif()
option(LOD_ENABLE_TLB_SEG6_TRACE "Enable targeted osMapTLB diagnostics for segment-6 model/asset mappings" OFF)
if (LOD_ENABLE_TLB_SEG6_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_TLB_SEG6_TRACE=1)
endif()
option(LOD_FIX_NI_TLB_SKIP_ACTUAL_SPAN "For NI overlay TLB maps, skip normal page copies only across the actual loaded overlay span instead of the legacy 64KB minimum" OFF)
if (LOD_FIX_NI_TLB_SKIP_ACTUAL_SPAN)
    target_compile_definitions(LodRecomp PRIVATE LOD_FIX_NI_TLB_SKIP_ACTUAL_SPAN=1)
endif()

option(LOD_DISABLE_RT64_FORCE_BRANCH "Disable RT64 global F3DEX BranchZ/W forcing for LoD render A/B tests" OFF)
if (LOD_DISABLE_RT64_FORCE_BRANCH)
    target_compile_definitions(LodRecomp PRIVATE LOD_DISABLE_RT64_FORCE_BRANCH=1)
endif()
if (LOD_ENABLE_GBI_MISS_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_GBI_MISS_TRACE=1)
endif()

option(LOD_ENABLE_NATIVE_HIGH_RES "Report Expansion Pak/8MB RDRAM to the game, enabling native high-resolution mode" OFF)
if (LOD_ENABLE_NATIVE_HIGH_RES)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_NATIVE_HIGH_RES=1)
else()
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_NATIVE_HIGH_RES=0)
endif()

option(LOD_SKIP_EXPANSION_PAK_SCREEN "Hard-skip the game's Expansion Pak / native high-resolution selector via the normal post-selector transition" ON)
if (LOD_SKIP_EXPANSION_PAK_SCREEN)
    target_compile_definitions(LodRecomp PRIVATE LOD_SKIP_EXPANSION_PAK_SCREEN=1)
else()
    target_compile_definitions(LodRecomp PRIVATE LOD_SKIP_EXPANSION_PAK_SCREEN=0)
endif()

option(LOD_AUTO_ADVANCE_EXPANSION_PAK_SCREEN "Auto-advance the game's Expansion Pak / native high-resolution selector using normal controller input" OFF)
if (LOD_AUTO_ADVANCE_EXPANSION_PAK_SCREEN)
    target_compile_definitions(LodRecomp PRIVATE LOD_AUTO_ADVANCE_EXPANSION_PAK_SCREEN=1)
else()
    target_compile_definitions(LodRecomp PRIVATE LOD_AUTO_ADVANCE_EXPANSION_PAK_SCREEN=0)
endif()

option(LOD_ENABLE_BOOT_INPUT_SCRIPT "Enable debug-only scripted controller input for boot/gameplay recovery tests" OFF)
if (LOD_ENABLE_BOOT_INPUT_SCRIPT)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_BOOT_INPUT_SCRIPT=1)
endif()
option(LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT "Enable debug-only aggressive controller input to skip quickly to gameplay/idle recovery tests" OFF)
if (LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1)
endif()
option(LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT "Enable debug-only scripted Start/menu input after reaching gameplay" OFF)
if (LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT=1)
endif()
set(LOD_ITEM_MENU_SAVE_SLOT "1" CACHE STRING "Debug-only save slot selected by LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT (1-3)")
target_compile_definitions(LodRecomp PRIVATE LOD_ITEM_MENU_SAVE_SLOT=${LOD_ITEM_MENU_SAVE_SLOT})
set(LOD_POST_RDRAM_GUARD_SIZE "0x10000" CACHE STRING "Size of zero-filled post-RDRAM diagnostic/compatibility guard at rdram+0x80800000")
target_compile_definitions(LodRecomp PRIVATE LOD_POST_RDRAM_GUARD_SIZE=${LOD_POST_RDRAM_GUARD_SIZE})
option(LOD_ENABLE_SAVE_AUTO_INPUT "Enable debug-only save-screen controller input for recovery tests" OFF)
if (LOD_ENABLE_SAVE_AUTO_INPUT)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_SAVE_AUTO_INPUT=1)
endif()
option(LOD_ENABLE_SAVE_HANDLER_TRACE "Enable debug-only save-screen handler tracing for gs=4 recovery" OFF)
if (LOD_ENABLE_SAVE_HANDLER_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_SAVE_HANDLER_TRACE=1)
endif()
option(LOD_ENABLE_SAVE_EXIT_TRACE "Enable debug-only save-screen exit/scheduler tracing for gs=4 recovery" OFF)
if (LOD_ENABLE_SAVE_EXIT_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_SAVE_EXIT_TRACE=1)
endif()
option(LOD_ENABLE_SAVE_STATE45_TRACE "Enable debug-only save-screen state 4/5 tracing for gs=4 recovery" OFF)
if (LOD_ENABLE_SAVE_STATE45_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_SAVE_STATE45_TRACE=1)
endif()
option(LOD_ENABLE_CFB_SNAPSHOT "Enable debug-only internal VI color framebuffer PPM snapshots" OFF)
if (LOD_ENABLE_CFB_SNAPSHOT)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_CFB_SNAPSHOT=1)
endif()
option(LOD_ENABLE_GS3_ANIM_TRACE "Enable debug-only DMAMgr/animation resource tracing for gameplay/intro recovery" OFF)
if (LOD_ENABLE_GS3_ANIM_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_GS3_ANIM_TRACE=1)
endif()
option(LOD_ENABLE_KSEG0_FAULT_TRACE "Enable debug-only lightweight context snapshots for KSEG0/post-RDRAM faults" OFF)
if (LOD_ENABLE_KSEG0_FAULT_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_KSEG0_FAULT_TRACE=1)
endif()
option(LOD_ENABLE_B2_ASSET_TRACE "Enable narrow file-0xB2/ROM-0x00C3FE4E asset trace for the missing boss/enemy investigation" OFF)
if (LOD_ENABLE_B2_ASSET_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_B2_ASSET_TRACE=1)
    target_compile_definitions(librecomp PRIVATE LOD_ENABLE_B2_ASSET_TRACE=1)
    target_compile_definitions(rt64 PRIVATE LOD_ENABLE_B2_ASSET_TRACE=1)
endif()
option(LOD_ENABLE_ITEM_KSEG0_TRACE "Enable debug-only item/model segmented-pointer tracing for item-use KSEG0 faults" OFF)
if (LOD_ENABLE_ITEM_KSEG0_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_ITEM_KSEG0_TRACE=1)
endif()
option(LOD_ENABLE_PAUSE_ITEM_TRACE "Enable debug-only pause/item menu NI overlay tracing" OFF)
if (LOD_ENABLE_PAUSE_ITEM_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_PAUSE_ITEM_TRACE=1)
endif()
option(LOD_FIX_NULL_OBJECT_DISPATCH_CHILD "Treat object_dispatchChild(NULL, ...) as a no-op" ON)
if (LOD_FIX_NULL_OBJECT_DISPATCH_CHILD)
    target_compile_definitions(LodRecomp PRIVATE LOD_FIX_NULL_OBJECT_DISPATCH_CHILD=1)
else()
    target_compile_definitions(LodRecomp PRIVATE LOD_FIX_NULL_OBJECT_DISPATCH_CHILD=0)
endif()
option(LOD_ENABLE_INPUT_TRACE "Enable debug-only raw/PIF/internal controller tracing for gameplay input diagnosis" OFF)
if (LOD_ENABLE_INPUT_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_INPUT_TRACE=1)
endif()
option(LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE "Enable debug-only Fog Lake enemy/platform trigger tracing" OFF)
if (LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE=1)
endif()
option(LOD_ENABLE_NI24_NULL_DISPATCH_TRACE "Enable debug-only NI pair 24 null-dispatch tracing for Fog Lake trigger diagnosis" OFF)
if (LOD_ENABLE_NI24_NULL_DISPATCH_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_NI24_NULL_DISPATCH_TRACE=1)
endif()
option(LOD_ENABLE_NI24_WRITER_TRACE "Enable debug-only object schedule writer tracing for NI pair 24 Fog Lake diagnosis" OFF)
if (LOD_ENABLE_NI24_WRITER_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_NI24_WRITER_TRACE=1)
endif()
option(LOD_FIX_NI_PAIR24_INTERNAL_LABELS "Enable scoped fix for NI pair 24 dispatch-table internal labels" ON)
if (LOD_FIX_NI_PAIR24_INTERNAL_LABELS)
    target_compile_definitions(LodRecomp PRIVATE LOD_FIX_NI_PAIR24_INTERNAL_LABELS=1)
else()
    target_compile_definitions(LodRecomp PRIVATE LOD_FIX_NI_PAIR24_INTERNAL_LABELS=0)
endif()
option(LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS "Enable routine frame/SI/TLB heartbeat logs that are normally too noisy for focused debugging" OFF)
if (LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS=1)
endif()
option(LOD_ENABLE_AUDIO_TRACE "Enable debug-only audio callback tracing" OFF)
if (LOD_ENABLE_AUDIO_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_AUDIO_TRACE=1)
endif()
option(LOD_ENABLE_AUDIO_RAW_DUMP "Enable debug-only raw AI queue dump to /tmp/lod_audio_queue_s16le.raw" OFF)
if (LOD_ENABLE_AUDIO_RAW_DUMP)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_AUDIO_RAW_DUMP=1)
endif()
option(LOD_ENABLE_AUDIO_LATENCY_GUARD "Enable Zelda-style SDL queued-audio latency catch-up guard" OFF)
if (LOD_ENABLE_AUDIO_LATENCY_GUARD)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_AUDIO_LATENCY_GUARD=1)
endif()
option(LOD_ENABLE_RSP_AUDIO_TRACE "Enable debug-only RSP audio task tracing and first-ucode dump" OFF)
if (LOD_ENABLE_RSP_AUDIO_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_RSP_AUDIO_TRACE=1)
endif()
option(LOD_ENABLE_AUDIO_PULL_TRACE "Enable debug-only CPU audio pull-chain tracing" OFF)
if (LOD_ENABLE_AUDIO_PULL_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_AUDIO_PULL_TRACE=1)
    target_compile_definitions(RecompiledFuncs PRIVATE LOD_ENABLE_AUDIO_PULL_TRACE=1)
endif()
option(LOD_USE_RECOMPILED_AUDIO_RSP "Use generated audio RSP microcode instead of the timing-only stub" ON)
if (LOD_USE_RECOMPILED_AUDIO_RSP)
    target_compile_definitions(LodRecomp PRIVATE LOD_USE_RECOMPILED_AUDIO_RSP=1)
endif()
option(LOD_OVERRIDE_ALENVMIXERPULL "Replace alEnvmixerPull with a no-op compatibility shim; disables audible audio" OFF)
if (LOD_OVERRIDE_ALENVMIXERPULL)
    target_compile_definitions(LodRecomp PRIVATE LOD_OVERRIDE_ALENVMIXERPULL=1)
else()
    target_compile_definitions(LodRecomp PRIVATE LOD_OVERRIDE_ALENVMIXERPULL=0)
endif()
option(LOD_ENABLE_GS5_NI_TRACE "Enable debug-only NI pair 53/104/149 scheduler tracing for gs=5 recovery" OFF)
if (LOD_ENABLE_GS5_NI_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_GS5_NI_TRACE=1)
endif()
option(LOD_USE_ZELDA_MENU "Use the experimental Zelda64Recomp menu framework instead of the current LoD overlay hooks" OFF)
if (LOD_USE_ZELDA_MENU)
    target_compile_definitions(LodRecomp PRIVATE LOD_USE_ZELDA_MENU=1 LOD_ZELDA_MENU_MINIMAL=1)
endif()
option(LOD_ENABLE_BGSTATE_TRACE "Enable debug-only background-state tracing for gs=5 recovery" OFF)
if (LOD_ENABLE_BGSTATE_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_BGSTATE_TRACE=1)
endif()
set(LOD_AUDIO_RSP_SOURCE ${CMAKE_SOURCE_DIR}/rsp/aspMain.cpp)
if (LOD_USE_RECOMPILED_AUDIO_RSP)
    set(LOD_AUDIO_RSP_SOURCE ${CMAKE_SOURCE_DIR}/rsp/aspMain_audio_recomp.cpp)
endif()

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main/main.cpp
    ${CMAKE_SOURCE_DIR}/src/main/support.cpp
    ${CMAKE_SOURCE_DIR}/src/main/register_overlays.cpp
    ${CMAKE_SOURCE_DIR}/src/main/rt64_render_context.cpp
    ${CMAKE_SOURCE_DIR}/src/main/ignored_func_stubs.cpp
    ${CMAKE_SOURCE_DIR}/src/main/tlb_segment.cpp
    ${CMAKE_SOURCE_DIR}/src/main/lod_init.cpp
    ${CMAKE_SOURCE_DIR}/src/main/ni_overlay_loader.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/lod_ui_overlay.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/shaders/InterfaceVS.hlsl.spirv.c
    ${CMAKE_SOURCE_DIR}/src/ui/shaders/InterfacePS.hlsl.spirv.c
    ${CMAKE_SOURCE_DIR}/src/ui/shaders/InterfaceVS.hlsl.metal.c
    ${CMAKE_SOURCE_DIR}/src/ui/shaders/InterfacePS.hlsl.metal.c
    ${LOD_AUDIO_RSP_SOURCE}

    ${CMAKE_SOURCE_DIR}/lib/RmlUi/Backends/RmlUi_Platform_SDL.cpp
)

if (LOD_USE_ZELDA_MENU)
    list(APPEND SOURCES
        ${CMAKE_SOURCE_DIR}/src/ui/lod_zelda_menu.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/ui_renderer.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/ui_state.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/ui_prompt.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/ui_color_hack.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/ui_rml_hacks.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/ui_elements.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/ui_utils.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/util/hsv.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/core/ui_context.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_button.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_clickable.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_container.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_element.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_image.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_label.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_radio.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_scroll_container.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_slider.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_span.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_style.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_text_input.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/elements/ui_toggle.cpp
    )
endif()

if (APPLE)
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/src/main/support_apple.mm)
endif()

target_include_directories(LodRecomp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/ui/shaders
    ${CMAKE_SOURCE_DIR}/lib/concurrentqueue
    ${CMAKE_SOURCE_DIR}/lib/SlotMap
    ${CMAKE_SOURCE_DIR}/lib/N64ModernRuntime/N64Recomp/include
    ${CMAKE_SOURCE_DIR}/lib/RmlUi/Include
    ${CMAKE_SOURCE_DIR}/lib/RmlUi/Backends
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib/plume
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib/hlslpp/include
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib/dxc/inc
    ${CMAKE_SOURCE_DIR}/lib/rt64/src
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/rhi
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/render
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib/nativefiledialog-extended/src/include
    ${CMAKE_BINARY_DIR}/shaders
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(LodRecomp PRIVATE
    -fno-strict-aliasing
    -fms-extensions
)

if (APPLE)
    # Use ld64 wrapper for writable+executable memory (required for mod patching)
    target_link_options(LodRecomp PRIVATE
        "-fuse-ld=${CMAKE_SOURCE_DIR}/.github/macos/ld64"
    )
endif()

# ── Platform: macOS ─────────────────────────────────────────────────
if (APPLE)
    find_package(SDL2 REQUIRED)
    target_include_directories(LodRecomp PRIVATE ${SDL2_INCLUDE_DIRS})

    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

    target_link_libraries(LodRecomp PRIVATE ${CMAKE_DL_LIBS} Threads::Threads SDL2::SDL2)

    # Ad-hoc codesign with entitlements after linking.
    # Required because the recomp runtime uses RWX memory (JIT/mmap).
    # Without this, macOS kills the binary with SIGKILL (Code Signature Invalid).
    set(ENTITLEMENTS_FILE "${CMAKE_SOURCE_DIR}/.github/macos/entitlements.plist")
    if (EXISTS ${ENTITLEMENTS_FILE})
        add_custom_command(TARGET LodRecomp POST_BUILD
            COMMAND codesign --force --sign - --entitlements "${ENTITLEMENTS_FILE}" "$<TARGET_FILE:LodRecomp>"
            COMMENT "Ad-hoc codesigning LodRecomp with entitlements"
        )
    endif()
endif()

# ── Platform: Linux ─────────────────────────────────────────────────
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(SDL2 REQUIRED)
    add_compile_definitions("RT64_SDL_WINDOW_VULKAN")

    target_include_directories(LodRecomp PRIVATE ${SDL2_INCLUDE_DIRS})

    find_package(Freetype REQUIRED)
    include_directories(${FREETYPE_LIBRARIES})
    target_link_libraries(LodRecomp PRIVATE ${FREETYPE_LIBRARIES} SDL2::SDL2)

    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

    target_link_libraries(LodRecomp PRIVATE "-latomic -static-libstdc++" ${CMAKE_DL_LIBS} Threads::Threads)

    # librecomp's RSP vector unit (rsp_vu_impl.hpp) uses SSSE3/SSE4.1 intrinsics.
    # On x86_64, clang gates these behind target features, so they must be
    # enabled explicitly; on arm64 sse2neon provides them with no -m flags.
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        target_compile_options(LodRecomp PRIVATE -mssse3 -msse4.1)
    endif()
endif()

# ── Platform: Windows ───────────────────────────────────────────────
if (WIN32)
    include(FetchContent)

    if (DEFINED ENV{SDL2_VERSION})
        set(SDL2_VERSION $ENV{SDL2_VERSION})
    else()
        set(SDL2_VERSION "2.30.3")
    endif()

    FetchContent_Declare(
        sdl2
        URL https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-VC.zip
    )
    FetchContent_MakeAvailable(sdl2)
    target_include_directories(LodRecomp PRIVATE ${sdl2_SOURCE_DIR}/include)
    target_link_directories(LodRecomp PRIVATE ${sdl2_SOURCE_DIR}/lib/x64)

    add_custom_command(TARGET LodRecomp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll"
            "${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib/dxc/bin/x64/dxil.dll"
            "${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib/dxc/bin/x64/dxcompiler.dll"
            $<TARGET_FILE_DIR:LodRecomp>)

    target_link_libraries(LodRecomp PRIVATE SDL2 Winmm.lib)

    # librecomp's RSP vector unit (rsp_vu_impl.hpp) uses SSSE3/SSE4.1 intrinsics;
    # clang-cl gates intrinsics behind target features (MSVC cl does not).
    target_compile_options(LodRecomp PRIVATE -mssse3 -msse4.1)

    # COFF has no ELF-style weak-symbol interposition, so the strong override
    # shims in src/main would collide with the weak RECOMP_FUNC definitions in
    # RecompiledFuncs. Keep the first definition seen instead: the executable's
    # own objects precede the RecompiledFuncs archive on the link line, so the
    # overrides win — matching the macOS/Linux weak-interposition semantics.
    target_link_options(LodRecomp PRIVATE /FORCE:MULTIPLE)

    # Embed version metadata and an explicit asInvoker manifest. Bare exes with
    # no VERSIONINFO/manifest score worse with SmartScreen and AV heuristics.
    target_sources(LodRecomp PRIVATE
        ${CMAKE_SOURCE_DIR}/src/main/lod_recomp.rc
        ${CMAKE_SOURCE_DIR}/src/main/lod_recomp.manifest
    )

    add_compile_definitions(NOMINMAX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR")
endif()

# ── Link everything ─────────────────────────────────────────────────
find_package(ZLIB REQUIRED)
if (APPLE AND CMAKE_OSX_SYSROOT AND TARGET ZLIB::ZLIB)
    # The macOS SDK's usr/include path is already implied by -isysroot. If the
    # imported ZLIB target adds it explicitly, clang can search the SDK C headers
    # before libc++'s wrapper headers and break <cstdio>/<cmath> includes.
    get_target_property(ZLIB_INTERFACE_INCLUDES ZLIB::ZLIB INTERFACE_INCLUDE_DIRECTORIES)
    if (ZLIB_INTERFACE_INCLUDES)
        list(REMOVE_ITEM ZLIB_INTERFACE_INCLUDES "${CMAKE_OSX_SYSROOT}/usr/include")
        set_target_properties(ZLIB::ZLIB PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INTERFACE_INCLUDES}")
    endif()
endif()
target_link_libraries(LodRecomp PRIVATE
    RecompiledFuncs
    librecomp
    ultramodern
    rt64
    RmlUi::Core
    RmlUi::Debugger
    nfd
    lunasvg
    ZLIB::ZLIB
)

target_sources(LodRecomp PRIVATE ${SOURCES})

set_property(TARGET LodRecomp PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# ── Tests ────────────────────────────────────────────────────────────
add_subdirectory(${CMAKE_SOURCE_DIR}/tests/sanity ${CMAKE_BINARY_DIR}/tests/sanity)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests/dl_harness ${CMAKE_BINARY_DIR}/tests/dl_harness)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests/pak ${CMAKE_BINARY_DIR}/tests/pak)

# ── DXC shader compilation (required by rt64) ───────────────────────
set(DXC_COMMON_OPTS "-I${PROJECT_SOURCE_DIR}/src")
set(DXC_DXIL_OPTS "-Wno-ignored-attributes")
set(DXC_SPV_OPTS "-spirv" "-fspv-target-env=vulkan1.0" "-fvk-use-dx-layout")
set(DXC_PS_OPTS "${DXC_COMMON_OPTS}" "-E" "PSMain" "-T ps_6_0" "-D DYNAMIC_RENDER_PARAMS")
set(DXC_VS_OPTS "${DXC_COMMON_OPTS}" "-E" "VSMain" "-T vs_6_0" "-D DYNAMIC_RENDER_PARAMS" "-fvk-invert-y")
set(DXC_CS_OPTS "${DXC_COMMON_OPTS}" "-E" "CSMain" "-T cs_6_0")
set(DXC_GS_OPTS "${DXC_COMMON_OPTS}" "-E" "GSMain" "-T gs_6_0")
set(DXC_RT_OPTS "${DXC_COMMON_OPTS}" "-D" "RT_SHADER" "-T" "lib_6_3" "-fspv-target-env=vulkan1.1spirv1.4" "-fspv-extension=SPV_KHR_ray_tracing" "-fspv-extension=SPV_EXT_descriptor_indexing")

if (${WIN32})
    set(DXC "${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/dxc/bin/x64/dxc.exe")
elseif (APPLE)
    set(DXC "DYLD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/dxc/lib/arm64" "${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/dxc/bin/arm64/dxc-macos")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(SPIRVCROSS "DYLD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/spirv-cross/lib/x64" "${PROJECT_SOURCE_DIR}/lib/rt64//src/contrib/spirv-cross/bin/x64/spirv-cross")
    else()
        set(SPIRVCROSS "DYLD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/spirv-cross/lib/arm64" "${PROJECT_SOURCE_DIR}/lib/rt64//src/contrib/spirv-cross/bin/x64/spirv-cross")
    endif()
else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(DXC "LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/dxc/lib/x64" "${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/dxc/bin/x64/dxc-linux")
    else()
        set(DXC "LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/dxc/lib/arm64" "${PROJECT_SOURCE_DIR}/lib/rt64/src/contrib/dxc/bin/arm64/dxc-linux")
    endif()
endif()
