# DL Test Harness — validates RT64 GBI pipeline stages
# Built as part of the main project (shares rt64 target)

add_executable(DLHarness main.cpp)

target_include_directories(DLHarness PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/rt64/src
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/contrib/hlslpp/include
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/rhi
    ${CMAKE_SOURCE_DIR}/lib/rt64/src/render
    ${CMAKE_BINARY_DIR}/rt64/src
)

target_compile_definitions(DLHarness PRIVATE HLSL_CPU)
target_compile_options(DLHarness PRIVATE -fno-strict-aliasing -fms-extensions)

# SDL2
if (APPLE)
    find_package(SDL2 REQUIRED)
    target_include_directories(DLHarness PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(DLHarness PRIVATE SDL2::SDL2)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(SDL2 REQUIRED)
    target_include_directories(DLHarness PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(DLHarness PRIVATE SDL2::SDL2)
elseif (WIN32)
    target_include_directories(DLHarness PRIVATE ${sdl2_SOURCE_DIR}/include)
    target_link_directories(DLHarness PRIVATE ${sdl2_SOURCE_DIR}/lib/x64)
    target_link_libraries(DLHarness PRIVATE SDL2)
endif()

target_link_libraries(DLHarness PRIVATE rt64)

if (APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(DLHarness PRIVATE ${CMAKE_DL_LIBS} Threads::Threads)

    set(ENTITLEMENTS_FILE "${CMAKE_SOURCE_DIR}/.github/macos/entitlements.plist")
    if (EXISTS ${ENTITLEMENTS_FILE})
        add_custom_command(TARGET DLHarness POST_BUILD
            COMMAND codesign --force --sign - --entitlements "${ENTITLEMENTS_FILE}" "$<TARGET_FILE:DLHarness>"
            COMMENT "Ad-hoc codesigning DLHarness"
        )
    endif()
endif()
