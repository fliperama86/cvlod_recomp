#ifndef __UI_RENDERER_H__
#define __UI_RENDERER_H__

#include <memory>

#include "rt64_render_hooks.h"
#include "recomp_ui.h"

namespace Rml {
    class RenderInterface;
}

namespace recompui {
    class RmlRenderInterface_RT64_impl;

    class RmlRenderInterface_RT64 {
    private:
        std::unique_ptr<RmlRenderInterface_RT64_impl> impl;
    public:
        RmlRenderInterface_RT64();
        ~RmlRenderInterface_RT64();
        void reset();
        void init(RenderInterface* interface, RenderDevice* device);
        Rml::RenderInterface* get_rml_interface();

        void start(RenderCommandList* list, int image_width, int image_height);
        void end(RenderCommandList* list, RenderFramebuffer* framebuffer);
        void queue_image_from_bytes_file(const std::string &src, const std::vector<char> &bytes);
        void queue_image_from_bytes_rgba32(const std::string &src, const std::vector<char> &bytes, uint32_t width, uint32_t height);
    };
} // namespace recompui

#endif
