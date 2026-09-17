# Cheat engine tests
# Standalone — apply_all() writes into a plain buffer, no runtime dependencies.

add_executable(CheatsTest main.cpp ${CMAKE_SOURCE_DIR}/src/main/lod_cheats.cpp)
target_compile_features(CheatsTest PRIVATE cxx_std_17)
target_include_directories(CheatsTest PRIVATE ${CMAKE_SOURCE_DIR}/include)
