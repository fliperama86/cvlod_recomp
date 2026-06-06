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

# ── RecompiledFuncs ─────────────────────────────────────────────────
add_library(RecompiledFuncs STATIC)

target_compile_options(RecompiledFuncs PRIVATE
    -fno-strict-aliasing
    -Wno-unused-variable
    -Wno-implicit-function-declaration
)

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

option(LOD_ENABLE_BOOT_INPUT_SCRIPT "Enable debug-only scripted controller input for boot/gameplay recovery tests" OFF)
if (LOD_ENABLE_BOOT_INPUT_SCRIPT)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_BOOT_INPUT_SCRIPT=1)
endif()
option(LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT "Enable debug-only aggressive controller input to skip quickly to gameplay/idle recovery tests" OFF)
if (LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT=1)
endif()
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
option(LOD_ENABLE_ITEM_KSEG0_TRACE "Enable debug-only item/model segmented-pointer tracing for item-use KSEG0 faults" OFF)
if (LOD_ENABLE_ITEM_KSEG0_TRACE)
    target_compile_definitions(LodRecomp PRIVATE LOD_ENABLE_ITEM_KSEG0_TRACE=1)
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
    ${LOD_AUDIO_RSP_SOURCE}

    ${CMAKE_SOURCE_DIR}/lib/RmlUi/Backends/RmlUi_Platform_SDL.cpp
)

if (APPLE)
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/src/main/support_apple.mm)
endif()

target_include_directories(LodRecomp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/N64ModernRuntime/N64Recomp/include
    ${CMAKE_SOURCE_DIR}/lib/RmlUi/Include
    ${CMAKE_SOURCE_DIR}/lib/RmlUi/Backends
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib
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
