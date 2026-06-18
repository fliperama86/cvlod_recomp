#ifndef __LOD_SUPPORT_H__
#define __LOD_SUPPORT_H__

#include <functional>
#include <filesystem>
#include <optional>

namespace lod {
    std::filesystem::path get_program_path();
    void show_error_message_box(const char *title, const char *message);

#ifdef __APPLE__
    void dispatch_on_ui_thread(std::function<void()> func);
    std::optional<std::filesystem::path> get_application_support_directory();
    std::filesystem::path get_bundle_resource_directory();
    std::filesystem::path get_bundle_directory();
#endif
}

#endif
