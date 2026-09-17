#include "android_touch_overlay.h"

#include <algorithm>
#include <array>
#include <cmath>

// LOD_TOUCH_TESTS builds the hit-testing core standalone, without SDL.
#if defined(__ANDROID__) && !defined(LOD_TOUCH_TESTS)
#define LOD_TOUCH_WITH_SDL 1
#include <SDL.h>
#endif

namespace lod::android {

namespace {

constexpr int64_t kNoFinger = -1;

// Full N64 button set. Radii are height-normalised (see TouchButtonLayout).
constexpr std::array<TouchButtonLayout, 10> kButtons{{
    { ULTRA_A,       0.90f, 0.70f, 0.070f, "A" },
    { ULTRA_B,       0.82f, 0.82f, 0.070f, "B" },
    { ULTRA_Z,       0.15f, 0.40f, 0.060f, "Z" },
    { ULTRA_L,       0.08f, 0.15f, 0.060f, "L" },
    { ULTRA_R,       0.92f, 0.15f, 0.060f, "R" },
    { ULTRA_START,   0.50f, 0.10f, 0.050f, "START" },
    { ULTRA_C_UP,    0.78f, 0.30f, 0.050f, "C↑" },
    { ULTRA_C_DOWN,  0.78f, 0.46f, 0.050f, "C↓" },
    { ULTRA_C_LEFT,  0.70f, 0.38f, 0.050f, "C←" },
    { ULTRA_C_RIGHT, 0.86f, 0.38f, 0.050f, "C→" },
}};

constexpr TouchStickLayout kStick{ 0.18f, 0.72f, 0.14f };

// mask 0: opens the emulator menu rather than feeding an N64 button.
constexpr TouchButtonLayout kMenuButton{ 0u, 0.70f, 0.09f, 0.045f, "MENU" };

struct ButtonState {
    bool pressed = false;
    int64_t owner = kNoFinger;
};

struct TouchPad {
    std::array<ButtonState, kButtons.size()> buttons{};
    int64_t stick_owner = kNoFinger;
    float stick_dx = 0.0f; // -1..1
    float stick_dy = 0.0f; // -1..1, positive is up (N64 convention)
    bool enabled = false;
    bool menu_pressed = false;
    int64_t menu_owner = kNoFinger;
    bool menu_requested = false;

    void reset() {
        buttons = {};
        stick_owner = kNoFinger;
        stick_dx = 0.0f;
        stick_dy = 0.0f;
        menu_pressed = false;
        menu_owner = kNoFinger;
        menu_requested = false;
    }

    // Distance in height-normalised units so hit areas are circular on screen.
    static float distance(float px, float py, float cx, float cy, float aspect) {
        const float dx = (px - cx) * aspect;
        const float dy = py - cy;
        return std::sqrt(dx * dx + dy * dy);
    }

    void update_stick(float x, float y, float aspect) {
        const float dx = (x - kStick.rel_x) * aspect;
        const float dy = y - kStick.rel_y;
        const float dist = std::sqrt(dx * dx + dy * dy);

        float nx = dx / kStick.rel_radius;
        float ny = dy / kStick.rel_radius;
        if (dist > kStick.rel_radius && dist > 0.0f) {
            // Clamp to the edge of the gate, preserving direction.
            nx *= kStick.rel_radius / dist;
            ny *= kStick.rel_radius / dist;
        }

        stick_dx = std::clamp(nx, -1.0f, 1.0f);
        stick_dy = std::clamp(-ny, -1.0f, 1.0f); // screen Y grows downward
    }

    void handle(TouchPhase phase, int64_t finger, float x, float y, float aspect) {
        if (!enabled) {
            return;
        }

        if (phase == TouchPhase::Up) {
            if (stick_owner == finger) {
                stick_owner = kNoFinger;
                stick_dx = 0.0f;
                stick_dy = 0.0f;
            }
            for (ButtonState& b : buttons) {
                if (b.owner == finger) {
                    b.pressed = false;
                    b.owner = kNoFinger;
                }
            }
            if (menu_owner == finger) {
                menu_pressed = false;
                menu_owner = kNoFinger;
            }
            return;
        }

        if (phase == TouchPhase::Down) {
            // A press inside the stick gate claims the stick, and that finger does nothing else.
            if (stick_owner == kNoFinger &&
                distance(x, y, kStick.rel_x, kStick.rel_y, aspect) <= kStick.rel_radius * 1.5f) {
                stick_owner = finger;
                update_stick(x, y, aspect);
                return;
            }

            if (menu_owner == kNoFinger &&
                distance(x, y, kMenuButton.rel_x, kMenuButton.rel_y, aspect) <= kMenuButton.rel_radius) {
                menu_owner = finger;
                menu_pressed = true;
                menu_requested = true; // latched; consumed by the frame loop
                return;
            }

            // Buttons are only claimed on the initial press, never by a finger sliding over them,
            // so dragging the stick can't fire buttons it passes under.
            for (size_t i = 0; i < buttons.size(); i++) {
                if (buttons[i].owner != kNoFinger) {
                    continue;
                }
                if (distance(x, y, kButtons[i].rel_x, kButtons[i].rel_y, aspect) <= kButtons[i].rel_radius) {
                    buttons[i].pressed = true;
                    buttons[i].owner = finger;
                    return;
                }
            }
            return;
        }

        // Motion.
        if (stick_owner == finger) {
            update_stick(x, y, aspect);
            return;
        }
        if (menu_owner == finger) {
            if (distance(x, y, kMenuButton.rel_x, kMenuButton.rel_y, aspect) > kMenuButton.rel_radius) {
                menu_pressed = false;
                menu_owner = kNoFinger;
            }
            return;
        }
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].owner != finger) {
                continue;
            }
            // Sliding off a held button releases it (and only it).
            const bool inside =
                distance(x, y, kButtons[i].rel_x, kButtons[i].rel_y, aspect) <= kButtons[i].rel_radius;
            if (!inside) {
                buttons[i].pressed = false;
                buttons[i].owner = kNoFinger;
            }
        }
    }

    uint16_t mask() const {
        uint16_t m = 0;
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].pressed) {
                m |= kButtons[i].mask;
            }
        }
        return m;
    }
};

TouchPad g_pad;

} // namespace

size_t button_count() {
    return kButtons.size();
}

const TouchButtonLayout& button_layout(size_t index) {
    return kButtons[std::min(index, kButtons.size() - 1)];
}

TouchStickLayout stick_layout() {
    return kStick;
}

void set_touch_controls_enabled(bool enabled) {
    if (g_pad.enabled == enabled) {
        return;
    }
    g_pad.enabled = enabled;
    g_pad.reset();
}

bool touch_controls_enabled() {
    return g_pad.enabled;
}

void reset_touch_controls() {
    g_pad.reset();
}

void handle_touch(TouchPhase phase, int64_t finger_id, float x, float y, float aspect) {
    g_pad.handle(phase, finger_id, x, y, aspect);
}

#ifdef LOD_TOUCH_WITH_SDL
void handle_touch_event(const SDL_Event& event, float aspect) {
    TouchPhase phase;
    switch (event.type) {
        case SDL_FINGERDOWN:   phase = TouchPhase::Down;   break;
        case SDL_FINGERMOTION: phase = TouchPhase::Motion; break;
        case SDL_FINGERUP:     phase = TouchPhase::Up;     break;
        default: return;
    }
    // SDL reports finger coordinates already normalised to 0..1.
    handle_touch(phase, static_cast<int64_t>(event.tfinger.fingerId),
                 event.tfinger.x, event.tfinger.y, aspect);
}
#endif

TouchControlsState get_touch_controls_state() {
    TouchControlsState state;
    state.buttons = g_pad.mask();
    state.stick_x = static_cast<int8_t>(std::lround(g_pad.stick_dx * 127.0f));
    state.stick_y = static_cast<int8_t>(std::lround(g_pad.stick_dy * 127.0f));
    return state;
}

bool button_pressed(size_t index) {
    return index < g_pad.buttons.size() && g_pad.buttons[index].pressed;
}

const TouchButtonLayout& menu_button_layout() {
    return kMenuButton;
}

bool consume_menu_request() {
    const bool requested = g_pad.menu_requested;
    g_pad.menu_requested = false;
    return requested;
}

bool menu_button_pressed() {
    return g_pad.menu_pressed;
}

void stick_thumb_offset(float& dx, float& dy) {
    dx = g_pad.stick_dx;
    dy = g_pad.stick_dy;
}

} // namespace lod::android
