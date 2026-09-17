// On-screen touch control rendering for Android.
//
// Draw-only: hit-testing lives in android_touch_overlay.cpp and both read the same layout table, so
// the visuals and the touch targets cannot drift apart. Element geometry is recomputed whenever the
// window size changes because Unit has no viewport units - positions are percentages but diameters
// must be dp derived from the window height, or the circles render as ellipses on a 16:9 screen.

#ifdef __ANDROID__

#include <cstdio>
#include <vector>

#include <SDL.h>

#include "android_touch_overlay.h"
#include "core/ui_context.h"
#include "elements/ui_element.h"
#include "elements/ui_label.h"
#include "recomp_ui.h"
#include "ultramodern/ultramodern.hpp"

namespace {

struct TouchUiState {
    recompui::ContextId context = recompui::ContextId::null();
    std::vector<recompui::Element*> button_elements;
    recompui::Element* stick_base = nullptr;
    recompui::Element* stick_thumb = nullptr;
    recompui::Element* menu_button = nullptr;
    int last_width = 0;
    int last_height = 0;
    bool shown = false;
    bool failed = false;
};

TouchUiState g_ui;

// A dark translucent fill with a light outline stays legible over both the black letterbox bars and
// bright game content; a light fill disappears against pale scenes.
constexpr recompui::Color kIdleFill{ 12, 10, 18, 120 };
constexpr recompui::Color kHeldFill{ 190, 184, 219, 210 };
constexpr recompui::Color kOutline{ 235, 232, 245, 180 };
constexpr recompui::Color kLabel{ 245, 243, 250, 235 };

void style_circle(recompui::Element* element, float diameter_dp) {
    element->set_position(recompui::Position::Absolute);
    element->set_width(diameter_dp, recompui::Unit::Dp);
    element->set_height(diameter_dp, recompui::Unit::Dp);
    element->set_border_radius(diameter_dp * 0.5f, recompui::Unit::Dp);
    element->set_border_width(1.5f, recompui::Unit::Dp);
    element->set_border_color(kOutline);
    element->set_background_color(kIdleFill);
    element->set_display(recompui::Display::Flex);
    element->set_align_items(recompui::AlignItems::Center);
    element->set_justify_content(recompui::JustifyContent::Center);
}

// Centres are placed by percentage, then pulled back by half the pixel diameter so the element is
// centred on the layout point rather than starting there.
void place_centred(recompui::Element* element, float rel_x, float rel_y, float diameter_dp) {
    element->set_left(rel_x * 100.0f, recompui::Unit::Percent);
    element->set_top(rel_y * 100.0f, recompui::Unit::Percent);
    element->set_margin_left(-diameter_dp * 0.5f, recompui::Unit::Dp);
    element->set_margin_top(-diameter_dp * 0.5f, recompui::Unit::Dp);
}

void build(int width, int height) {
    using namespace recompui;

    g_ui.context = create_context();
    if (g_ui.context == ContextId::null()) {
        fprintf(stderr, "[TOUCH] could not create touch overlay context\n");
        g_ui.failed = true;
        return;
    }

    // Purely decorative: it must never swallow game input or gate off get_n64_input().
    g_ui.context.set_captures_input(false);
    g_ui.context.set_captures_mouse(false);

    g_ui.context.open();

    Element* root = g_ui.context.create_element<Element>(g_ui.context.get_root_element());
    root->set_position(Position::Absolute);
    root->set_left(0);
    root->set_top(0);
    root->set_width(100, Unit::Percent);
    root->set_height(100, Unit::Percent);
    root->set_background_color({ 0, 0, 0, 0 });

    const float h = static_cast<float>(height);

    const lod::android::TouchStickLayout stick = lod::android::stick_layout();
    const float stick_d = stick.rel_radius * 2.0f * h;
    g_ui.stick_base = g_ui.context.create_element<Element>(root);
    style_circle(g_ui.stick_base, stick_d);
    place_centred(g_ui.stick_base, stick.rel_x, stick.rel_y, stick_d);

    const float thumb_d = stick_d * 0.45f;
    g_ui.stick_thumb = g_ui.context.create_element<Element>(root);
    style_circle(g_ui.stick_thumb, thumb_d);
    g_ui.stick_thumb->set_background_color(kHeldFill);
    place_centred(g_ui.stick_thumb, stick.rel_x, stick.rel_y, thumb_d);

    // The menu button is drawn from the same table but is not an N64 button.
    {
        const lod::android::TouchButtonLayout& m = lod::android::menu_button_layout();
        const float d = m.rel_radius * 2.0f * h;
        g_ui.menu_button = g_ui.context.create_element<Element>(root);
        style_circle(g_ui.menu_button, d);
        place_centred(g_ui.menu_button, m.rel_x, m.rel_y, d);
        g_ui.context.create_element<Label>(g_ui.menu_button, m.label, LabelStyle::Small)
            ->set_color(kLabel);
    }

    g_ui.button_elements.clear();
    for (size_t i = 0; i < lod::android::button_count(); i++) {
        const lod::android::TouchButtonLayout& layout = lod::android::button_layout(i);
        const float d = layout.rel_radius * 2.0f * h;
        Element* button = g_ui.context.create_element<Element>(root);
        style_circle(button, d);
        place_centred(button, layout.rel_x, layout.rel_y, d);
        g_ui.context.create_element<Label>(button, layout.label, LabelStyle::Small)
            ->set_color(kLabel);
        g_ui.button_elements.push_back(button);
    }

    g_ui.context.close();
    g_ui.last_width = width;
    g_ui.last_height = height;
}

void destroy() {
    if (g_ui.context != recompui::ContextId::null()) {
        recompui::hide_context(g_ui.context);
    }
    g_ui.context = recompui::ContextId::null();
    g_ui.button_elements.clear();
    g_ui.stick_base = nullptr;
    g_ui.stick_thumb = nullptr;
    g_ui.menu_button = nullptr;
    g_ui.shown = false;
}

} // namespace

namespace lod::android {

void update_touch_overlay_ui(int width, int height) {
    if (g_ui.failed || width <= 0 || height <= 0) {
        return;
    }

    // update_gfx() starts pumping before the UI system is initialised, and creating a context then
    // dereferences a null ui_state. Gating on the game having started covers that and is also the
    // behaviour we want: on-screen controls belong in gameplay, not over the launcher. They also
    // step aside whenever a menu is capturing input.
    const bool want = touch_controls_enabled() &&
                      ultramodern::is_game_started() &&
                      !recompui::is_context_capturing_input();

    if (!want) {
        if (g_ui.shown) {
            destroy();
            reset_touch_controls();
        }
        return;
    }

    // Rebuild on first show and whenever the surface is resized or rotated, since diameters are dp.
    if (g_ui.context == recompui::ContextId::null() ||
        width != g_ui.last_width || height != g_ui.last_height) {
        destroy();
        build(width, height);
        if (g_ui.failed) {
            return;
        }
    }

    if (!g_ui.shown) {
        recompui::show_context(g_ui.context, "");
        g_ui.shown = true;
    }

    // Per-frame feedback: held buttons brighten, and the thumb follows the reported stick offset.
    g_ui.context.open();
    for (size_t i = 0; i < g_ui.button_elements.size(); i++) {
        g_ui.button_elements[i]->set_background_color(button_pressed(i) ? kHeldFill : kIdleFill);
    }

    if (g_ui.menu_button != nullptr) {
        g_ui.menu_button->set_background_color(menu_button_pressed() ? kHeldFill : kIdleFill);
    }

    float dx = 0.0f;
    float dy = 0.0f;
    stick_thumb_offset(dx, dy);
    const TouchStickLayout stick = stick_layout();
    const float thumb_d = stick.rel_radius * 2.0f * static_cast<float>(g_ui.last_height) * 0.45f;
    // dy is positive-up from the pad; screen space grows downward.
    place_centred(g_ui.stick_thumb,
                  stick.rel_x + dx * stick.rel_radius / (static_cast<float>(g_ui.last_width) /
                                                         static_cast<float>(g_ui.last_height)),
                  stick.rel_y - dy * stick.rel_radius,
                  thumb_d);
    g_ui.context.close();
}

void shutdown_touch_overlay_ui() {
    destroy();
}

} // namespace lod::android

#endif // __ANDROID__
