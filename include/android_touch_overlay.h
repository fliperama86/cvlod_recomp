#pragma once

#include <cstddef>
#include <cstdint>

union SDL_Event;

namespace lod::android {

#ifndef ULTRA_A
#define ULTRA_A        0x8000
#define ULTRA_B        0x4000
#define ULTRA_Z        0x2000
#define ULTRA_START    0x1000
#define ULTRA_U_DPAD   0x0800
#define ULTRA_D_DPAD   0x0400
#define ULTRA_L_DPAD   0x0200
#define ULTRA_R_DPAD   0x0100
#define ULTRA_L        0x0020
#define ULTRA_R        0x0010
#define ULTRA_C_UP     0x0008
#define ULTRA_C_DOWN   0x0004
#define ULTRA_C_LEFT   0x0002
#define ULTRA_C_RIGHT  0x0001
#endif

/**
 * One on-screen button.
 *
 * Centres are normalised independently against width and height, but the radius is normalised
 * against SCREEN HEIGHT only. Distances are therefore compared in height-normalised space, which
 * keeps hit areas circular rather than squashing them by the aspect ratio.
 */
struct TouchButtonLayout {
    uint16_t mask;
    float rel_x;
    float rel_y;
    float rel_radius;
    const char* label;
};

struct TouchStickLayout {
    float rel_x;
    float rel_y;
    float rel_radius;
};

struct TouchControlsState {
    uint16_t buttons = 0;
    int8_t stick_x = 0;
    int8_t stick_y = 0;
};

enum class TouchPhase {
    Down,
    Motion,
    Up,
};

/**
 * The overlay menu button. Not an N64 button - it opens the emulator menu, which is otherwise
 * only reachable via TOGGLE_MENU (Escape / controller Back) and so is unreachable on a
 * touch-only device. mask is 0 because it feeds no N64 input.
 */
const TouchButtonLayout& menu_button_layout();

/** True once per press of the menu button; reading it clears the request. */
bool consume_menu_request();

/** For the renderer: whether the menu button is currently held. */
bool menu_button_pressed();

// Layout is the single source of truth: hit-testing and rendering both read it.
size_t button_count();
const TouchButtonLayout& button_layout(size_t index);
TouchStickLayout stick_layout();

void set_touch_controls_enabled(bool enabled);
bool touch_controls_enabled();

/** Clears all pressed state; call when the overlay is hidden so nothing sticks down. */
void reset_touch_controls();

/**
 * Core hit-testing entry point. Coordinates are normalised 0..1 (SDL finger events already are).
 * @param aspect screen_width / screen_height, so hit areas stay circular.
 */
void handle_touch(TouchPhase phase, int64_t finger_id, float x, float y, float aspect);

/** Adapts an SDL_FINGER* event onto handle_touch(). */
void handle_touch_event(const SDL_Event& event, float aspect);

TouchControlsState get_touch_controls_state();

// For the renderer.
bool button_pressed(size_t index);
void stick_thumb_offset(float& dx, float& dy);

} // namespace lod::android
