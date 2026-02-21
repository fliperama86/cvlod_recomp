#include <cinttypes>
#include <cstdio>

#include "lod/target_rom.hpp"

#ifdef LOD_ENABLE_RECOMP_RUNTIME
#include "librecomp/game.hpp"
#endif

#ifdef LOD_ENABLE_RECOMP_RUNTIME
recomp::GameEntry make_lod_game_entry_stub() {
    return recomp::GameEntry{
        .rom_hash = lod::target_rom::kXxh3_64,
        .internal_name = std::string{lod::target_rom::kInternalName},
        .game_id = u8"lod.n64.us.1.0",
        .mod_game_id = "lod",
        .save_type = recomp::SaveType::AllowAll,
        .is_enabled = true,
        .decompression_routine = nullptr,
        .has_compressed_code = false,
        .entrypoint_address = 0,
        .entrypoint = nullptr,
    };
}
#endif

int main() {
    std::printf("Legacy of Darkness Recomp Bootstrap\n");
    std::printf("Target internal name: %.*s\n", static_cast<int>(lod::target_rom::kInternalName.size()), lod::target_rom::kInternalName.data());
    std::printf("Target game code: %.*s\n", static_cast<int>(lod::target_rom::kGameCode.size()), lod::target_rom::kGameCode.data());
    std::printf("Target XXH3_64 (GameEntry.rom_hash): 0x%016" PRIX64 "\n", lod::target_rom::kXxh3_64);
    std::printf("SHA1: %.*s\n", static_cast<int>(lod::target_rom::kSha1.size()), lod::target_rom::kSha1.data());
    std::printf("SHA256: %.*s\n", static_cast<int>(lod::target_rom::kSha256.size()), lod::target_rom::kSha256.data());
    std::printf("MD5: %.*s\n", static_cast<int>(lod::target_rom::kMd5.size()), lod::target_rom::kMd5.data());

    std::printf("\nNext integration step:\n");
    std::printf("- Replace this bootstrap executable with N64Recomp + N64ModernRuntime wiring.\n");
    std::printf("- Register GameEntry using the locked constants from include/lod/target_rom.hpp.\n");
#ifdef LOD_ENABLE_RECOMP_RUNTIME
    auto lod_game_entry = make_lod_game_entry_stub();
    std::printf("- Experimental runtime flag enabled; stub game id: %s\n",
        reinterpret_cast<const char*>(lod_game_entry.game_id.c_str()));
#else
    std::printf("- Experimental runtime flag is OFF (default).\n");
#endif

    return 0;
}
