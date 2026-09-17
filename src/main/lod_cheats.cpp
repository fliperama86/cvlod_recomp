#include "lod/lod_cheats.hpp"

#include <array>
#include <cstring>

namespace lod::cheats {

namespace {

// One GameShark write. `width` is 1 or 2 bytes, matching the 80.../81... code prefixes.
struct CodeWrite {
    uint32_t guest_addr;
    uint8_t width;
    uint16_t value;
};

// Verified codes for Castlevania: Legacy of Darkness (USA). The player status block is contiguous
// from 0x801CAB3A, which is why the item writes are consecutive addresses.
constexpr CodeWrite kInvincibility[] = {
    { 0x801CAB3Au, 2, 0x2AF8u }, // 811CAB3A 2AF8 - infinite energy
};

constexpr CodeWrite kInfiniteMoney[] = {
    { 0x801CAB42u, 2, 0xFFFFu }, // 811CAB42 FFFF
};

constexpr CodeWrite kInfiniteRedJewels[] = {
    { 0x801CAB45u, 1, 0x68u },   // 801CAB45 0068
};

// Every consumable slot, grouped behind one toggle.
constexpr CodeWrite kInfiniteItems[] = {
    { 0x801CAB47u, 1, 0x0Au }, // Special 1
    { 0x801CAB48u, 1, 0x0Au }, // Special 2
    { 0x801CAB49u, 1, 0x0Au }, // Special 3
    { 0x801CAB4Au, 1, 0x0Au }, // Roast Chicken
    { 0x801CAB4Bu, 1, 0x0Au }, // Roast Beef
    { 0x801CAB4Cu, 1, 0x0Au }, // Healing Kit
    { 0x801CAB4Du, 1, 0x0Au }, // Purifying
    { 0x801CAB4Eu, 1, 0x0Au }, // Cure Ampoule
    { 0x801CAB4Fu, 1, 0x0Au }, // Power up
};

constexpr CodeWrite kMaxPowerups[] = {
    { 0x801CAE23u, 1, 0x02u },   // 801CAE23 0002
};

constexpr CodeWrite kInfiniteBullets[] = {
    { 0x801D3DA3u, 1, 0x06u },   // 801D3DA3 0006 - Henry only
};

struct CheatDef {
    CheatInfo info;
    const CodeWrite* writes;
    size_t write_count;
};

template <size_t N>
constexpr CheatDef make_def(const char* id, const char* label, const char* description,
                           const CodeWrite (&writes)[N]) {
    return CheatDef{ { id, label, description }, writes, N };
}

const std::array<CheatDef, static_cast<size_t>(Cheat::Count)> kCheats{{
    make_def("invincibility", "Invincibility", "Keeps your energy pinned at maximum.", kInvincibility),
    make_def("infinite_money", "Infinite Money", "Holds gold at the maximum value.", kInfiniteMoney),
    make_def("infinite_red_jewels", "Infinite Red Jewels", "Keeps red jewels topped up.", kInfiniteRedJewels),
    make_def("infinite_items", "Infinite Items", "Refills every consumable: specials, food, kits and cures.", kInfiniteItems),
    make_def("max_powerups", "Max Power-ups", "Holds your weapon at its upgraded form.", kMaxPowerups),
    make_def("infinite_bullets", "Infinite Bullets", "Keeps Henry's ammunition full. No effect as other characters.", kInfiniteBullets),
}};

std::array<bool, static_cast<size_t>(Cheat::Count)> g_enabled{};

// The guest is big-endian while RDRAM is stored as byte-swapped 32-bit words, so a guest byte at
// `phys` lives at host `phys ^ 3` and an aligned guest halfword at host `phys ^ 2`. This matches
// lod_rdram_u8/s16 in ignored_func_stubs.cpp; getting it wrong writes to a neighbouring field.
void write_u8(uint8_t* rdram, size_t rdram_size, uint32_t phys, uint8_t value) {
    if (phys >= rdram_size) {
        return;
    }
    rdram[phys ^ 3u] = value;
}

void write_u16(uint8_t* rdram, size_t rdram_size, uint32_t phys, uint16_t value) {
    // Only aligned halfwords can use the ^2 swizzle.
    if ((phys & 1u) != 0u || phys + sizeof(uint16_t) > rdram_size) {
        return;
    }
    const uint32_t host = phys ^ 2u;
    std::memcpy(rdram + host, &value, sizeof(value));
}

} // namespace

size_t count() {
    return kCheats.size();
}

const CheatInfo& info(size_t index) {
    return kCheats[index < kCheats.size() ? index : 0].info;
}

size_t index_for_id(const char* id) {
    if (id == nullptr) {
        return kCheats.size();
    }
    for (size_t i = 0; i < kCheats.size(); i++) {
        if (std::strcmp(kCheats[i].info.id, id) == 0) {
            return i;
        }
    }
    return kCheats.size();
}

bool enabled(size_t index) {
    return index < g_enabled.size() && g_enabled[index];
}

void set_enabled(size_t index, bool on) {
    if (index < g_enabled.size()) {
        g_enabled[index] = on;
    }
}

size_t active_count() {
    size_t total = 0;
    for (bool on : g_enabled) {
        if (on) {
            total++;
        }
    }
    return total;
}

void apply_all(uint8_t* rdram, size_t rdram_size) {
    if (rdram == nullptr || rdram_size == 0) {
        return;
    }

    for (size_t i = 0; i < kCheats.size(); i++) {
        if (!g_enabled[i]) {
            continue;
        }
        const CheatDef& def = kCheats[i];
        for (size_t w = 0; w < def.write_count; w++) {
            const CodeWrite& code = def.writes[w];
            const uint32_t phys = code.guest_addr & 0x1FFFFFFFu;
            if (code.width == 2) {
                write_u16(rdram, rdram_size, phys, code.value);
            } else {
                write_u8(rdram, rdram_size, phys, static_cast<uint8_t>(code.value));
            }
        }
    }
}

} // namespace lod::cheats
