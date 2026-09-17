#pragma once

#include <cstddef>
#include <cstdint>

// Cheat engine for Castlevania: Legacy of Darkness (USA).
//
// Each cheat is a small table of guest-address writes re-applied every VI, which is how the
// original GameShark/Action Replay hardware behaved and what "infinite X" codes require - the game
// overwrites the value continuously, so a one-shot poke would not hold.
//
// This module owns only cheat state and application. File persistence lives with the other configs
// in main.cpp, which keeps this translation unit free of JSON and trivially testable.
namespace lod::cheats {

// Order is display order. Persistence keys off CheatInfo::id, never this index, so reordering or
// inserting cheats cannot silently flip a different one in an existing cheats.json.
enum class Cheat : uint32_t {
    Invincibility,
    InfiniteMoney,
    InfiniteRedJewels,
    InfiniteItems,
    MaxPowerups,
    InfiniteBullets,
    Count,
};

struct CheatInfo {
    const char* id;          // stable key used in cheats.json
    const char* label;       // menu row text
    const char* description; // help line
};

/** Number of cheats, i.e. static_cast<size_t>(Cheat::Count). */
size_t count();

/** Metadata for a cheat; index must be < count(). */
const CheatInfo& info(size_t index);

/** Index for a persistence id, or count() if unknown (e.g. a key from a newer build). */
size_t index_for_id(const char* id);

bool enabled(size_t index);
void set_enabled(size_t index, bool on);

/** How many cheats are currently on; drives the launcher warning. */
size_t active_count();

/**
 * Writes every enabled cheat's values into the guest RDRAM image.
 *
 * Call once per VI. Writes are bounds-checked against rdram_size and silently skipped if they would
 * fall outside it or are misaligned, so a bad table entry cannot corrupt memory out of range.
 */
void apply_all(uint8_t* rdram, size_t rdram_size);

} // namespace lod::cheats
