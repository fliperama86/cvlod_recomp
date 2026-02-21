#include "lod/lod_support.h"
#include <SDL.h>

namespace lod {
    std::filesystem::path get_program_path() {
#if defined(__APPLE__)
        return get_bundle_resource_directory();
#else
        return "";
#endif
    }

    void show_error_message_box(const char *title, const char *message) {
#ifdef __APPLE__
        std::string title_copy(title);
        std::string message_copy(message);
        dispatch_on_ui_thread([title_copy, message_copy] {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title_copy.c_str(), message_copy.c_str(), nullptr);
        });
#else
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
#endif
    }
}
