// Cheat engine tests.
//
// Standalone: apply_all() writes into a plain buffer, so no game, RDRAM allocation or renderer is
// needed. The important properties are that values land at the correct byte-swizzled offsets, that
// disabled cheats write nothing at all, and that a bad table entry cannot write out of bounds.

#include "lod/lod_cheats.hpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace lod::cheats;

namespace {

int g_checks = 0;
int g_failures = 0;

void check(bool condition, const std::string& what) {
    g_checks++;
    if (!condition) {
        g_failures++;
        std::printf("  FAIL: %s\n", what.c_str());
    }
}

constexpr size_t kRdramSize = 0x00800000; // 8 MiB, as the runtime uses

std::vector<uint8_t> fresh_rdram() {
    return std::vector<uint8_t>(kRdramSize, 0u);
}

void all_off() {
    for (size_t i = 0; i < count(); i++) {
        set_enabled(i, false);
    }
}

size_t index_of(const char* id) {
    return index_for_id(id);
}

// Reads back using the same convention the game side uses.
uint8_t guest_u8(const std::vector<uint8_t>& rdram, uint32_t phys) {
    return rdram[phys ^ 3u];
}

uint16_t guest_u16(const std::vector<uint8_t>& rdram, uint32_t phys) {
    return static_cast<uint16_t>((static_cast<uint16_t>(guest_u8(rdram, phys)) << 8) |
                                 guest_u8(rdram, phys + 1));
}

size_t nonzero_bytes(const std::vector<uint8_t>& rdram) {
    size_t n = 0;
    for (uint8_t b : rdram) {
        if (b != 0u) {
            n++;
        }
    }
    return n;
}

void begin(const char* name) {
    std::printf("%s\n", name);
    all_off();
}

void test_ids_are_stable_and_unique() {
    begin("every cheat has a unique, resolvable persistence id");
    for (size_t i = 0; i < count(); i++) {
        const char* id = info(i).id;
        check(id != nullptr && id[0] != '\0', "cheat " + std::to_string(i) + " has a non-empty id");
        check(index_for_id(id) == i, std::string("id '") + id + "' resolves back to its own index");
    }
    check(index_for_id("not_a_cheat") == count(), "unknown id resolves to count()");
    check(index_for_id(nullptr) == count(), "null id resolves to count()");
}

void test_disabled_writes_nothing() {
    begin("no cheat enabled writes nothing at all");
    auto rdram = fresh_rdram();
    apply_all(rdram.data(), rdram.size());
    check(nonzero_bytes(rdram) == 0, "RDRAM untouched with all cheats off");
}

void test_invincibility_is_a_halfword_at_the_right_offset() {
    begin("invincibility writes 0x2AF8 as a guest halfword at 0x1CAB3A");
    auto rdram = fresh_rdram();
    set_enabled(index_of("invincibility"), true);
    apply_all(rdram.data(), rdram.size());

    check(guest_u16(rdram, 0x1CAB3Au) == 0x2AF8u,
          "guest halfword reads back 0x2AF8, got 0x" +
              std::to_string(guest_u16(rdram, 0x1CAB3Au)));
    // Exactly two bytes should have changed - a wrong swizzle would smear into neighbours.
    check(nonzero_bytes(rdram) == 2, "exactly 2 bytes written, got " +
                                         std::to_string(nonzero_bytes(rdram)));
}

void test_money_is_a_halfword() {
    begin("infinite money writes 0xFFFF at 0x1CAB42");
    auto rdram = fresh_rdram();
    set_enabled(index_of("infinite_money"), true);
    apply_all(rdram.data(), rdram.size());
    check(guest_u16(rdram, 0x1CAB42u) == 0xFFFFu, "money halfword is 0xFFFF");
    check(nonzero_bytes(rdram) == 2, "exactly 2 bytes written");
}

void test_items_cover_all_nine_slots() {
    begin("infinite items fills all nine consumable slots with 0x0A");
    auto rdram = fresh_rdram();
    set_enabled(index_of("infinite_items"), true);
    apply_all(rdram.data(), rdram.size());

    for (uint32_t addr = 0x1CAB47u; addr <= 0x1CAB4Fu; addr++) {
        check(guest_u8(rdram, addr) == 0x0Au,
              "slot 0x" + std::to_string(addr) + " is 0x0A");
    }
    check(nonzero_bytes(rdram) == 9, "exactly 9 bytes written, got " +
                                         std::to_string(nonzero_bytes(rdram)));
    // The neighbouring red-jewel byte must not be touched by the items group.
    check(guest_u8(rdram, 0x1CAB45u) == 0u, "red jewels byte untouched by the items group");
}

void test_single_byte_cheats() {
    begin("byte-wide cheats land on their own single byte");
    struct Case { const char* id; uint32_t phys; uint8_t value; };
    const Case cases[] = {
        { "infinite_red_jewels", 0x1CAB45u, 0x68u },
        { "max_powerups",        0x1CAE23u, 0x02u },
        { "infinite_bullets",    0x1D3DA3u, 0x06u },
    };
    for (const Case& c : cases) {
        all_off();
        auto rdram = fresh_rdram();
        set_enabled(index_of(c.id), true);
        apply_all(rdram.data(), rdram.size());
        check(guest_u8(rdram, c.phys) == c.value, std::string(c.id) + " wrote its value");
        check(nonzero_bytes(rdram) == 1, std::string(c.id) + " wrote exactly 1 byte");
    }
}

void test_cheats_compose() {
    begin("multiple cheats apply together without interfering");
    auto rdram = fresh_rdram();
    set_enabled(index_of("invincibility"), true);
    set_enabled(index_of("infinite_money"), true);
    set_enabled(index_of("infinite_items"), true);
    apply_all(rdram.data(), rdram.size());
    check(guest_u16(rdram, 0x1CAB3Au) == 0x2AF8u, "energy still correct");
    check(guest_u16(rdram, 0x1CAB42u) == 0xFFFFu, "money still correct");
    check(guest_u8(rdram, 0x1CAB4Fu) == 0x0Au, "last item slot still correct");
    check(nonzero_bytes(rdram) == 2 + 2 + 9, "2+2+9 bytes written");
}

void test_active_count_tracks_toggles() {
    begin("active_count reflects the enabled set");
    check(active_count() == 0, "starts at zero");
    set_enabled(index_of("invincibility"), true);
    check(active_count() == 1, "one after enabling one");
    set_enabled(index_of("max_powerups"), true);
    check(active_count() == 2, "two after enabling two");
    set_enabled(index_of("invincibility"), false);
    check(active_count() == 1, "back to one after disabling");
}

void test_out_of_range_is_skipped() {
    begin("a short RDRAM image is never written past its end");
    // Every real cheat address is well past 64KiB, so with a tiny buffer all writes must be skipped
    // rather than running off the end. Guards against a future typo'd table entry.
    std::vector<uint8_t> tiny(0x1000, 0u);
    for (size_t i = 0; i < count(); i++) {
        set_enabled(i, true);
    }
    apply_all(tiny.data(), tiny.size());
    check(nonzero_bytes(tiny) == 0, "no writes landed in the undersized buffer");
}

void test_null_rdram_is_safe() {
    begin("a null RDRAM pointer is ignored");
    for (size_t i = 0; i < count(); i++) {
        set_enabled(i, true);
    }
    apply_all(nullptr, kRdramSize); // must not crash
    check(true, "apply_all(nullptr) returned without crashing");
}

} // namespace

int main() {
    test_ids_are_stable_and_unique();
    test_disabled_writes_nothing();
    test_invincibility_is_a_halfword_at_the_right_offset();
    test_money_is_a_halfword();
    test_items_cover_all_nine_slots();
    test_single_byte_cheats();
    test_cheats_compose();
    test_active_count_tracks_toggles();
    test_out_of_range_is_skipped();
    test_null_rdram_is_safe();

    std::printf("\n%d checks, %d failure(s)\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
