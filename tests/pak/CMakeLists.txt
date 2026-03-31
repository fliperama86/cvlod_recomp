# Controller Pak format + operations tests
# Standalone — no runtime dependencies, just validates pak image format
# against the N64 PFS specification.

add_executable(PakTest main.cpp)
target_compile_features(PakTest PRIVATE cxx_std_17)
