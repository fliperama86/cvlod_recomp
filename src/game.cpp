/**
 * CVLoD Recomp - Game Configuration
 *
 * Registers the Castlevania: Legacy of Darkness game with the runtime.
 */

#include <cstdint>
#include "recomp.h"
#include "librecomp/game.hpp"

// Forward declaration of entrypoint function
// 0x80000460 is the first actual function in the binary
extern "C" void func_80000460(uint8_t* rdram, recomp_context* ctx);

// Game constants
static constexpr uint64_t CVLOD_ROM_HASH = 0; // TODO: Calculate actual ROM hash
static constexpr uint32_t CVLOD_ENTRYPOINT = 0x80000460;

// Game entry configuration
static recomp::GameEntry cvlod_entry = {
    .rom_hash = CVLOD_ROM_HASH,
    .internal_name = "CASTLEVANIA2",
    .game_id = u8"cvlod",
    .mod_game_id = "cvlod",
    .save_type = recomp::SaveType::Eep16k, // CV:LoD uses 16kbit EEPROM
    .is_enabled = true,
    .decompression_routine = nullptr,
    .has_compressed_code = false,
    .entrypoint_address = CVLOD_ENTRYPOINT,
    .entrypoint = nullptr, // Set in register_cvlod_game
    .thread_create_callback = nullptr,
    .on_init_callback = nullptr,
};

void register_cvlod_game() {
    // Set the entrypoint function pointer
    // 0x80000460 is the first function in the recompiled code
    cvlod_entry.entrypoint = func_80000460;

    // Register with runtime
    // recomp::register_game(cvlod_entry);
}

void setup_rsp_callbacks(recomp::rsp::callbacks_t& callbacks) {
    // RSP callbacks for audio/microcode processing
    // These need to be implemented for audio to work

    // For now, use a stub that returns nullptr (no microcode handler)
    callbacks.get_rsp_microcode = nullptr;
}
