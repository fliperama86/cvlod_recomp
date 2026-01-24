cmake_minimum_required(VERSION 3.20)

project(CVLoD_Recomp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Options
option(BUILD_WITH_RT64 "Build with RT64 graphics backend" ON)

# =============================================================================
# Dependencies
# =============================================================================

# N64ModernRuntime (ultramodern + librecomp)
add_subdirectory(lib/N64ModernRuntime)

# RT64 graphics backend
if(BUILD_WITH_RT64)
    add_subdirectory(lib/rt64)
endif()

# =============================================================================
# Recompiled Game Code
# =============================================================================

# Collect all recompiled C files
file(GLOB RECOMPILED_SOURCES "recompiled/functions/*.c")

add_library(cvlod_recompiled STATIC ${RECOMPILED_SOURCES})

target_include_directories(cvlod_recompiled PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/recompiled/functions"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/N64ModernRuntime/N64Recomp/include"
)

# Disable warnings for generated code
target_compile_options(cvlod_recompiled PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-w>
    $<$<CXX_COMPILER_ID:Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/w>
)

# =============================================================================
# Game Runtime
# =============================================================================

add_executable(cvlod_recomp
    src/main.cpp
    src/game.cpp
    src/funcs.cpp
)

target_include_directories(cvlod_recomp PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/recompiled/functions"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/N64ModernRuntime/librecomp/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/N64ModernRuntime/ultramodern/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/N64ModernRuntime/N64Recomp/include"
)

target_link_libraries(cvlod_recomp PRIVATE
    cvlod_recompiled
    librecomp
    ultramodern
)

if(BUILD_WITH_RT64)
    target_link_libraries(cvlod_recomp PRIVATE rt64)
    target_compile_definitions(cvlod_recomp PRIVATE USE_RT64=1)
endif()

# Platform-specific settings
if(WIN32)
    target_compile_definitions(cvlod_recomp PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

# =============================================================================
# Installation
# =============================================================================

install(TARGETS cvlod_recomp DESTINATION bin)
