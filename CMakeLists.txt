cmake_minimum_required(VERSION 3.20)

project(LegacyOfDarknessRecomp LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LOD_ENABLE_RECOMP_RUNTIME "Enable experimental N64ModernRuntime wiring." OFF)
set(N64MODERNRUNTIME_DIR "" CACHE PATH "Path to N64ModernRuntime source root (required when LOD_ENABLE_RECOMP_RUNTIME=ON)")

add_executable(lod_recomp_bootstrap
    src/main/main.cpp
)

target_include_directories(lod_recomp_bootstrap PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

if (LOD_ENABLE_RECOMP_RUNTIME)
    if (NOT EXISTS "${N64MODERNRUNTIME_DIR}/librecomp/include/librecomp/game.hpp")
        message(FATAL_ERROR
            "LOD_ENABLE_RECOMP_RUNTIME=ON requires N64MODERNRUNTIME_DIR to point to a valid N64ModernRuntime tree "
            "(missing librecomp/game.hpp).")
    endif()
    target_compile_definitions(lod_recomp_bootstrap PRIVATE LOD_ENABLE_RECOMP_RUNTIME=1)
    target_include_directories(lod_recomp_bootstrap PRIVATE
        ${N64MODERNRUNTIME_DIR}/librecomp/include
        ${N64MODERNRUNTIME_DIR}/N64Recomp/include
        ${N64MODERNRUNTIME_DIR}/ultramodern/include
        ${N64MODERNRUNTIME_DIR}/thirdparty
        ${N64MODERNRUNTIME_DIR}/thirdparty/sse2neon
        ${N64MODERNRUNTIME_DIR}/thirdparty/concurrentqueue
    )
endif()
