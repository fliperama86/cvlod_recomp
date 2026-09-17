// Android on-screen touch control hit-testing tests.
//
// Standalone: the hit-testing core takes normalised coordinates and needs no SDL, no window and no
// renderer, so it can be exercised directly.

#include "android_touch_overlay.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace lod::android;

namespace {

int g_failures = 0;
int g_checks = 0;

void check(bool condition, const std::string& what) {
    g_checks++;
    if (!condition) {
        g_failures++;
        std::printf("  FAIL: %s\n", what.c_str());
    }
}

// A 16:9 landscape screen, the case where a naive normalised-distance test squashes hit areas.
constexpr float kAspect = 1920.0f / 1080.0f;

void begin(const char* name) {
    std::printf("%s\n", name);
    set_touch_controls_enabled(false);
    set_touch_controls_enabled(true);
    reset_touch_controls();
}

size_t index_of(uint16_t mask) {
    for (size_t i = 0; i < button_count(); i++) {
        if (button_layout(i).mask == mask) {
            return i;
        }
    }
    return button_count();
}

void test_centre_of_each_button() {
    begin("centre of each button presses exactly that button");
    for (size_t i = 0; i < button_count(); i++) {
        reset_touch_controls();
        const TouchButtonLayout& b = button_layout(i);
        handle_touch(TouchPhase::Down, 1, b.rel_x, b.rel_y, kAspect);
        const uint16_t got = get_touch_controls_state().buttons;
        check(got == b.mask, "button " + std::string(b.label) + " expected mask " +
                                 std::to_string(b.mask) + " got " + std::to_string(got));
        handle_touch(TouchPhase::Up, 1, b.rel_x, b.rel_y, kAspect);
    }
}

void test_gap_presses_nothing() {
    begin("a touch in empty space presses nothing");
    // Middle of the screen is deliberately clear of every control.
    handle_touch(TouchPhase::Down, 1, 0.50f, 0.55f, kAspect);
    check(get_touch_controls_state().buttons == 0, "expected no buttons pressed");
}

void test_two_fingers_two_buttons() {
    begin("two fingers press two buttons simultaneously");
    const TouchButtonLayout& a = button_layout(index_of(ULTRA_A));
    const TouchButtonLayout& b = button_layout(index_of(ULTRA_B));
    handle_touch(TouchPhase::Down, 1, a.rel_x, a.rel_y, kAspect);
    handle_touch(TouchPhase::Down, 2, b.rel_x, b.rel_y, kAspect);
    const uint16_t got = get_touch_controls_state().buttons;
    check(got == (ULTRA_A | ULTRA_B), "expected A|B, got " + std::to_string(got));

    // Releasing one finger must not release the other button.
    handle_touch(TouchPhase::Up, 1, a.rel_x, a.rel_y, kAspect);
    check(get_touch_controls_state().buttons == ULTRA_B, "expected only B after releasing A");
}

void test_sliding_off_releases() {
    begin("sliding a finger off a held button releases it");
    const TouchButtonLayout& a = button_layout(index_of(ULTRA_A));
    handle_touch(TouchPhase::Down, 1, a.rel_x, a.rel_y, kAspect);
    check(get_touch_controls_state().buttons == ULTRA_A, "A should be held");
    // Move well clear of the button without lifting.
    handle_touch(TouchPhase::Motion, 1, 0.50f, 0.55f, kAspect);
    check(get_touch_controls_state().buttons == 0, "A should release when the finger slides off");
}

void test_stick_finger_does_not_press_buttons() {
    begin("the stick finger does not trigger buttons it slides over");
    const TouchStickLayout s = stick_layout();
    handle_touch(TouchPhase::Down, 1, s.rel_x, s.rel_y, kAspect);
    // Drag right across the screen, passing under the C cluster and A/B.
    for (float x = s.rel_x; x <= 0.95f; x += 0.01f) {
        handle_touch(TouchPhase::Motion, 1, x, s.rel_y, kAspect);
    }
    check(get_touch_controls_state().buttons == 0, "dragging the stick must not press buttons");
}

void test_stick_range_and_clamp() {
    begin("stick reports full range and clamps to the gate");
    const TouchStickLayout s = stick_layout();

    reset_touch_controls();
    handle_touch(TouchPhase::Down, 1, s.rel_x, s.rel_y, kAspect);
    TouchControlsState st = get_touch_controls_state();
    check(st.stick_x == 0 && st.stick_y == 0, "centre should be neutral");

    // Push far past the gate to the right; x should saturate positive, y stay neutral.
    handle_touch(TouchPhase::Motion, 1, s.rel_x + 0.9f, s.rel_y, kAspect);
    st = get_touch_controls_state();
    check(st.stick_x > 120, "far right should saturate stick_x, got " + std::to_string(st.stick_x));
    check(std::abs((int)st.stick_y) <= 1, "far right should leave stick_y neutral");

    // Screen Y grows downward, so a touch above centre must read as positive (up).
    handle_touch(TouchPhase::Motion, 1, s.rel_x, s.rel_y - 0.9f, kAspect);
    st = get_touch_controls_state();
    check(st.stick_y > 120, "upward should be positive stick_y, got " + std::to_string(st.stick_y));

    // Diagonal must never exceed unit magnitude.
    handle_touch(TouchPhase::Motion, 1, s.rel_x + 0.9f, s.rel_y - 0.9f, kAspect);
    st = get_touch_controls_state();
    const float mag = std::sqrt(float(st.stick_x) * st.stick_x + float(st.stick_y) * st.stick_y) / 127.0f;
    check(mag <= 1.02f, "diagonal magnitude should clamp to <=1, got " + std::to_string(mag));

    handle_touch(TouchPhase::Up, 1, s.rel_x, s.rel_y, kAspect);
    st = get_touch_controls_state();
    check(st.stick_x == 0 && st.stick_y == 0, "releasing should recentre the stick");
}

void test_hit_area_is_circular_not_squashed() {
    begin("hit areas are circular on screen, not squashed by aspect ratio");
    const TouchButtonLayout& b = button_layout(index_of(ULTRA_START));
    // A point one radius directly above the centre is on the edge: inside.
    handle_touch(TouchPhase::Down, 1, b.rel_x, b.rel_y - b.rel_radius * 0.9f, kAspect);
    check(get_touch_controls_state().buttons == b.mask, "0.9r above centre should hit");
    handle_touch(TouchPhase::Up, 1, 0, 0, kAspect);

    reset_touch_controls();
    // The same *screen* distance horizontally is a smaller normalised x offset by 1/aspect.
    handle_touch(TouchPhase::Down, 2, b.rel_x + (b.rel_radius * 0.9f) / kAspect, b.rel_y, kAspect);
    check(get_touch_controls_state().buttons == b.mask, "0.9r right of centre should also hit");
    handle_touch(TouchPhase::Up, 2, 0, 0, kAspect);

    reset_touch_controls();
    // Beyond the radius horizontally must miss.
    handle_touch(TouchPhase::Down, 3, b.rel_x + (b.rel_radius * 1.6f) / kAspect, b.rel_y, kAspect);
    check(get_touch_controls_state().buttons == 0, "1.6r right of centre should miss");
}

void test_disabled_ignores_input() {
    begin("input is ignored while the overlay is disabled");
    set_touch_controls_enabled(false);
    const TouchButtonLayout& a = button_layout(index_of(ULTRA_A));
    handle_touch(TouchPhase::Down, 1, a.rel_x, a.rel_y, kAspect);
    check(get_touch_controls_state().buttons == 0, "disabled overlay must not report presses");
}

void test_menu_button_requests_menu_without_n64_input() {
    begin("the menu button latches a request and feeds no N64 buttons");
    const TouchButtonLayout& m = menu_button_layout();
    check(m.mask == 0, "menu button carries no N64 mask");
    (void)consume_menu_request(); // clear anything stale
    handle_touch(TouchPhase::Down, 1, m.rel_x, m.rel_y, kAspect);
    check(get_touch_controls_state().buttons == 0, "menu press must not produce N64 buttons");
    check(menu_button_pressed(), "menu button reads as held for the renderer");
    check(consume_menu_request(), "request is latched");
    check(!consume_menu_request(), "request is cleared once consumed");
    handle_touch(TouchPhase::Up, 1, m.rel_x, m.rel_y, kAspect);
    check(!menu_button_pressed(), "released after finger up");
}

void test_menu_button_does_not_overlap_other_controls() {
    begin("the menu button does not overlap any other control");
    const TouchButtonLayout& m = menu_button_layout();
    // Pressing dead centre of the menu button must claim nothing else.
    handle_touch(TouchPhase::Down, 1, m.rel_x, m.rel_y, kAspect);
    check(get_touch_controls_state().buttons == 0, "no N64 button claimed at menu centre");
    const TouchControlsState st = get_touch_controls_state();
    check(st.stick_x == 0 && st.stick_y == 0, "stick not claimed at menu centre");
    handle_touch(TouchPhase::Up, 1, m.rel_x, m.rel_y, kAspect);

    // And every N64 button centre must be outside the menu button, in screen space.
    for (size_t i = 0; i < button_count(); i++) {
        const TouchButtonLayout& b = button_layout(i);
        const float dx = (b.rel_x - m.rel_x) * kAspect;
        const float dy = b.rel_y - m.rel_y;
        const float dist = std::sqrt(dx * dx + dy * dy);
        check(dist > b.rel_radius + m.rel_radius,
              std::string("menu clear of ") + b.label + " (gap " + std::to_string(dist) + ")");
    }
}

void test_sliding_off_menu_button_does_not_refire() {
    begin("sliding off the menu button releases it without a second request");
    const TouchButtonLayout& m = menu_button_layout();
    (void)consume_menu_request();
    handle_touch(TouchPhase::Down, 1, m.rel_x, m.rel_y, kAspect);
    check(consume_menu_request(), "first press requests once");
    handle_touch(TouchPhase::Motion, 1, 0.50f, 0.55f, kAspect);
    check(!menu_button_pressed(), "released when the finger slides off");
    check(!consume_menu_request(), "sliding off does not request again");
}
} // namespace

int main() {
    test_centre_of_each_button();
    test_gap_presses_nothing();
    test_two_fingers_two_buttons();
    test_sliding_off_releases();
    test_stick_finger_does_not_press_buttons();
    test_stick_range_and_clamp();
    test_hit_area_is_circular_not_squashed();
    test_disabled_ignores_input();
    test_menu_button_requests_menu_without_n64_input();
    test_menu_button_does_not_overlap_other_controls();
    test_sliding_off_menu_button_does_not_refire();

    std::printf("\n%d checks, %d failure(s)\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
