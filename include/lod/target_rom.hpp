#ifndef LOD_TARGET_ROM_HPP
#define LOD_TARGET_ROM_HPP

#include <cstdint>
#include <string_view>

namespace lod::target_rom {

constexpr std::string_view kInternalName = "CASTLEVANIA2";
constexpr std::string_view kGameCode = "ND4E";

constexpr std::string_view kSha1 = "879ead98f197fd05edda867655da5b1ce25aa5b8";
constexpr std::string_view kSha256 = "89e15df6042defddc48c61f7408f99d06fffdb845e2422cf8b5ba8e73d4d70fb";
constexpr std::string_view kMd5 = "25258460f98f567497b24844abe3a05b";

// Runtime GameEntry.rom_hash value (XXH3_64 over canonical big-endian ROM bytes).
constexpr std::uint64_t kXxh3_64 = 0x0BF827D4AC27A631ULL;

} // namespace lod::target_rom

#endif

