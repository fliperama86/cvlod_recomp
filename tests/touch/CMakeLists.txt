# Android on-screen touch control hit-testing tests.
# Standalone — the hit-testing core needs no SDL, window or renderer.

add_executable(TouchTest main.cpp ${CMAKE_SOURCE_DIR}/src/ui/android_touch_overlay.cpp)
target_compile_features(TouchTest PRIVATE cxx_std_17)
target_compile_definitions(TouchTest PRIVATE LOD_TOUCH_TESTS=1)
target_include_directories(TouchTest PRIVATE ${CMAKE_SOURCE_DIR}/include)
