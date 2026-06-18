#ifndef RECOMPUI_ELEMENTS_H
#define RECOMPUI_ELEMENTS_H

#include "recomp_ui.h"
#include "RmlUi/Core/Element.h"

#ifndef LOD_ZELDA_MENU_MINIMAL
#include "ui_mod_menu.h"
#include "ui_config_sub_menu.h"
#endif

namespace recompui {
    void register_custom_elements();

    Rml::ElementInstancer* get_custom_element_instancer(std::string tag);
}

#endif
