#include "lod_ui_overlay.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef HLSL_CPU
#define HLSL_CPU
#endif
#include "rt64_render_hooks.h"
#include "plume_render_interface_builders.h"

#include "RmlUi/Core.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/RenderInterfaceCompatibility.h"

#include "ultramodern/config.hpp"

#include "InterfaceVS.hlsl.spirv.h"
#include "InterfacePS.hlsl.spirv.h"
#ifdef __APPLE__
#include "InterfaceVS.hlsl.metal.h"
#include "InterfacePS.hlsl.metal.h"
#endif

#if defined(__APPLE__)
#define LOD_UI_SHADER_BLOB(name, format) \
    ((format) == RenderShaderFormat::SPIRV ? name##BlobSPIRV : \
     (format) == RenderShaderFormat::METAL ? name##BlobMSL : nullptr)
#define LOD_UI_SHADER_SIZE(name, format) \
    ((format) == RenderShaderFormat::SPIRV ? std::size(name##BlobSPIRV) : \
     (format) == RenderShaderFormat::METAL ? std::size(name##BlobMSL) : 0)
#else
#define LOD_UI_SHADER_BLOB(name, format) \
    ((format) == RenderShaderFormat::SPIRV ? name##BlobSPIRV : nullptr)
#define LOD_UI_SHADER_SIZE(name, format) \
    ((format) == RenderShaderFormat::SPIRV ? std::size(name##BlobSPIRV) : 0)
#endif

namespace {

struct RmlPushConstants {
    Rml::Matrix4f transform;
    Rml::Vector2f translation;
};

struct TextureHandle {
    std::unique_ptr<RenderTexture> texture;
    std::unique_ptr<RenderDescriptorSet> set;
    bool transitioned = false;
};

struct DynamicBuffer {
    std::unique_ptr<RenderBuffer> buffer{};
    uint32_t size = 0;
    uint32_t bytes_used = 0;
    uint8_t* mapped_data = nullptr;
    RenderBufferFlags flags = RenderBufferFlag::NONE;
};

std::atomic_bool g_overlay_visible{false};
std::atomic_bool g_logged_first_draw{false};
bool g_rml_initialised = false;

constexpr RenderFormat kRmlTextureFormat = RenderFormat::R8G8B8A8_UNORM;
constexpr RenderFormat kRmlTextureFormatBgra = RenderFormat::B8G8R8A8_UNORM;
constexpr RenderFormat kSwapChainFormat = RenderFormat::B8G8R8A8_UNORM;
constexpr uint32_t kTextureBytesPerPixel = RenderFormatSize(kRmlTextureFormat);
static_assert(RenderFormatSize(kRmlTextureFormatBgra) == kTextureBytesPerPixel);

void calculate_texture_row_width_padding(uint32_t row_pitch, uint32_t& row_width, uint32_t& row_padding) {
    constexpr uint32_t row_multiple = 256;
    row_width = row_pitch;
    row_padding = (row_width % row_multiple) ? row_multiple - (row_width % row_multiple) : 0;
    row_width += row_padding;
}

const char* resolution_name(ultramodern::renderer::Resolution value) {
    switch (value) {
        case ultramodern::renderer::Resolution::Original: return "Original";
        case ultramodern::renderer::Resolution::Original2x: return "Original2x";
        case ultramodern::renderer::Resolution::Auto: return "Auto";
        default: return "Unknown";
    }
}

const char* window_name(ultramodern::renderer::WindowMode value) {
    switch (value) {
        case ultramodern::renderer::WindowMode::Windowed: return "Windowed";
        case ultramodern::renderer::WindowMode::Fullscreen: return "Fullscreen";
        default: return "Unknown";
    }
}

const char* aspect_name(ultramodern::renderer::AspectRatio value) {
    switch (value) {
        case ultramodern::renderer::AspectRatio::Original: return "Original";
        case ultramodern::renderer::AspectRatio::Expand: return "Expand";
        case ultramodern::renderer::AspectRatio::Manual: return "Manual";
        default: return "Unknown";
    }
}

const char* msaa_name(ultramodern::renderer::Antialiasing value) {
    switch (value) {
        case ultramodern::renderer::Antialiasing::None: return "None";
        case ultramodern::renderer::Antialiasing::MSAA2X: return "MSAA2X";
        case ultramodern::renderer::Antialiasing::MSAA4X: return "MSAA4X";
        case ultramodern::renderer::Antialiasing::MSAA8X: return "MSAA8X";
        default: return "Unknown";
    }
}

const char* refresh_name(ultramodern::renderer::RefreshRate value) {
    switch (value) {
        case ultramodern::renderer::RefreshRate::Original: return "Original";
        case ultramodern::renderer::RefreshRate::Display: return "Display";
        case ultramodern::renderer::RefreshRate::Manual: return "Manual";
        default: return "Unknown";
    }
}

std::string make_options_rml() {
    auto config = ultramodern::renderer::get_graphics_config();
    std::ostringstream out;
    out << "<div class='setting'><div><h2>Window Mode</h2><p>F11 toggles fullscreen.</p></div><button>" << window_name(config.wm_option) << "</button></div>";
    out << "<div class='setting'><div><h2>Internal Resolution</h2><p>F5 cycles render resolution.</p></div><button>" << resolution_name(config.res_option) << "</button></div>";
    out << "<div class='setting'><div><h2>Aspect Ratio</h2><p>F6 cycles viewport behavior.</p></div><button>" << aspect_name(config.ar_option) << "</button></div>";
    out << "<div class='setting'><div><h2>Anti-Aliasing</h2><p>F7 cycles MSAA level.</p></div><button>" << msaa_name(config.msaa_option) << "</button></div>";
    out << "<div class='setting'><div><h2>Refresh Rate</h2><p>F8 cycles timing mode.</p></div><button>" << refresh_name(config.rr_option);
    if (config.rr_option == ultramodern::renderer::RefreshRate::Manual) {
        out << " " << config.rr_manual_value << "Hz";
    }
    out << "</button></div>";
    return out.str();
}

std::string make_document_rml() {
    return R"RML(
<rml>
<head>
<style>
body {
    width: 100%; height: 100%;
    font-family: Arial;
    color: #f2e8cf;
}
#scrim {
    position: absolute;
    left: 0; top: 0; right: 0; bottom: 0;
    background-color: rgba(0, 0, 0, 0.30);
}
#panel {
    position: absolute;
    left: 72px; top: 58px;
    width: 760px;
    background-color: rgba(17, 20, 28, 0.94);
    border: 2px #d1ad63;
    box-shadow: 0 0 28px rgba(0,0,0,0.75);
}
#titlebar {
    height: 68px;
    background-color: #4b2025;
    border-bottom: 2px #d1ad63;
    padding-left: 28px;
    padding-top: 16px;
}
#titlebar h1 {
    margin: 0;
    font-size: 28px;
    color: #f4d58d;
}
#titlebar p {
    margin-top: 4px;
    font-size: 13px;
    color: #c8cbd2;
}
#content {
    display: flex;
    flex-direction: row;
}
#tabs {
    width: 160px;
    background-color: rgba(10, 12, 18, 0.72);
    border-right: 1px #5f5548;
    padding: 18px;
}
.tab {
    padding: 12px 14px;
    margin-bottom: 8px;
    color: #c8cbd2;
    border: 1px transparent;
}
.tab.active {
    color: #fff4cf;
    background-color: rgba(209, 173, 99, 0.16);
    border: 1px #d1ad63;
}
#settings {
    width: 560px;
    padding: 18px 22px 20px 22px;
}
.setting {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
    min-height: 62px;
    padding: 8px 0;
    border-bottom: 1px rgba(255,255,255,0.11);
}
.setting h2 {
    margin: 0 0 3px 0;
    font-size: 17px;
    color: #fff4cf;
}
.setting p {
    margin: 0;
    font-size: 12px;
    color: #aeb4bf;
}
button {
    width: 150px;
    padding: 9px;
    color: #11141c;
    background-color: #d1ad63;
    border: 1px #f2d58d;
    text-align: center;
}
#footer {
    padding: 14px 22px 18px 22px;
    font-size: 12px;
    color: #abb1bf;
}
</style>
</head>
<body>
<div id='scrim'></div>
<div id='panel'>
  <div id='titlebar'>
    <h1>Castlevania: Legacy of Darkness Recompiled</h1>
    <p>Settings overlay prototype - RmlUi / RT64 render hook</p>
  </div>
  <div id='content'>
    <div id='tabs'>
      <div class='tab active'>Graphics</div>
      <div class='tab'>Controls</div>
      <div class='tab'>Audio</div>
      <div class='tab'>Mods</div>
    </div>
    <div id='settings'></div>
  </div>
  <div id='footer'>F1 closes this overlay. Use F5/F6/F7/F8/F11 to change values for now; full Zelda-style controls come next.</div>
</div>
</body>
</rml>
)RML";
}

class RmlOverlayRenderer final : public Rml::RenderInterfaceCompatibility {
public:
    void init(RenderInterface* render_interface, RenderDevice* device) {
        interface_ = render_interface;
        device_ = device;
        g_logged_first_draw.store(false, std::memory_order_relaxed);

        vertex_buffer_.flags = RenderBufferFlag::VERTEX;
        index_buffer_.flags = RenderBufferFlag::INDEX;
        resize_dynamic_buffer(vertex_buffer_, 512 * sizeof(Rml::Vertex), false);
        resize_dynamic_buffer(index_buffer_, 1024 * sizeof(int), false);

        RenderSamplerDesc sampler_desc;
        sampler_desc.minFilter = RenderFilter::LINEAR;
        sampler_desc.magFilter = RenderFilter::LINEAR;
        sampler_desc.addressU = RenderTextureAddressMode::CLAMP;
        sampler_desc.addressV = RenderTextureAddressMode::CLAMP;
        sampler_desc.addressW = RenderTextureAddressMode::CLAMP;
        sampler_ = device_->createSampler(sampler_desc);

        RenderShaderFormat shader_format = interface_->getCapabilities().shaderFormat;
        vertex_shader_ = device_->createShader(
            LOD_UI_SHADER_BLOB(InterfaceVS, shader_format),
            LOD_UI_SHADER_SIZE(InterfaceVS, shader_format),
            "VSMain", shader_format);
        pixel_shader_ = device_->createShader(
            LOD_UI_SHADER_BLOB(InterfacePS, shader_format),
            LOD_UI_SHADER_SIZE(InterfacePS, shader_format),
            "PSMain", shader_format);

        RenderDescriptorSetBuilder sampler_set_builder{};
        sampler_set_builder.begin();
        sampler_set_builder.addImmutableSampler(1, sampler_.get());
        sampler_set_builder.addConstantBuffer(3, 1);
        sampler_set_builder.end();
        sampler_set_ = sampler_set_builder.create(device_);

        texture_set_builder_ = std::make_unique<RenderDescriptorSetBuilder>();
        texture_set_builder_->begin();
        texture_descriptor_index_ = texture_set_builder_->addTexture(2);
        texture_set_builder_->end();

        RenderPipelineLayoutBuilder layout_builder{};
        layout_builder.begin(false, true);
        layout_builder.addPushConstant(0, 0, sizeof(RmlPushConstants), RenderShaderStageFlag::VERTEX);
        layout_builder.addDescriptorSet(sampler_set_builder);
        layout_builder.addDescriptorSet(*texture_set_builder_);
        layout_builder.end();
        layout_ = layout_builder.create(device_);

        std::vector<RenderInputElement> vertex_elements{};
        vertex_elements.emplace_back(RenderInputElement{ "POSITION", 0, 0, RenderFormat::R32G32_FLOAT, 0, offsetof(Rml::Vertex, position) });
        vertex_elements.emplace_back(RenderInputElement{ "COLOR", 0, 1, RenderFormat::R8G8B8A8_UNORM, 0, offsetof(Rml::Vertex, colour) });
        vertex_elements.emplace_back(RenderInputElement{ "TEXCOORD", 0, 2, RenderFormat::R32G32_FLOAT, 0, offsetof(Rml::Vertex, tex_coord) });

        RenderGraphicsPipelineDesc pipeline_desc{};
        pipeline_desc.renderTargetBlend[0] = RenderBlendDesc {
            .blendEnabled = true,
            .srcBlend = RenderBlend::SRC_ALPHA,
            .dstBlend = RenderBlend::INV_SRC_ALPHA,
            .blendOp = RenderBlendOperation::ADD,
            .srcBlendAlpha = RenderBlend::ONE,
            .dstBlendAlpha = RenderBlend::INV_SRC_ALPHA,
            .blendOpAlpha = RenderBlendOperation::ADD,
        };
        pipeline_desc.renderTargetFormat[0] = kSwapChainFormat;
        pipeline_desc.renderTargetCount = 1;
        pipeline_desc.cullMode = RenderCullMode::NONE;
        pipeline_desc.inputSlots = &vertex_slot_;
        pipeline_desc.inputSlotsCount = 1;
        pipeline_desc.inputElements = vertex_elements.data();
        pipeline_desc.inputElementsCount = uint32_t(vertex_elements.size());
        pipeline_desc.pipelineLayout = layout_.get();
        pipeline_desc.primitiveTopology = RenderPrimitiveTopology::TRIANGLE_LIST;
        pipeline_desc.vertexShader = vertex_shader_.get();
        pipeline_desc.pixelShader = pixel_shader_.get();
        pipeline_ = device_->createGraphicsPipeline(pipeline_desc);

        copy_command_queue_ = device_->createCommandQueue(RenderCommandListType::COPY);
        copy_command_list_ = copy_command_queue_->createCommandList();
        copy_command_fence_ = device_->createCommandFence();

        const Rml::byte white_pixel[] = {255, 255, 255, 255};
        create_texture(0, white_pixel, Rml::Vector2i{1, 1});
        const Rml::byte transparent_pixel[] = {0, 0, 0, 0};
        create_texture(1, transparent_pixel, Rml::Vector2i{1, 1});

        if (!g_rml_initialised) {
            Rml::SetRenderInterface(GetAdaptedInterface());
            g_rml_initialised = Rml::Initialise();
            if (!g_rml_initialised) {
                std::fprintf(stderr, "[UI] RmlUi initialise failed\n");
                return;
            }

#ifdef __APPLE__
            Rml::LoadFontFace("/System/Library/Fonts/Supplemental/Arial.ttf", true);
            Rml::LoadFontFace("/System/Library/Fonts/Supplemental/Arial Bold.ttf", false, Rml::Style::FontWeight::Bold);
#endif
        }

        std::fprintf(stderr, "[UI] RmlUi RT64 overlay hook initialized\n");
    }

    void reset() {
        if (document_) {
            document_->Close();
            document_ = nullptr;
        }
        if (context_) {
            Rml::RemoveContext(context_->GetName());
            context_ = nullptr;
        }
        if (g_rml_initialised) {
            Rml::Shutdown();
            g_rml_initialised = false;
        }
        textures_.clear();
        stale_buffers_.clear();
        copy_buffer_.reset();
        copy_command_fence_.reset();
        copy_command_list_.reset();
        copy_command_queue_.reset();
        pipeline_.reset();
        layout_.reset();
        texture_set_builder_.reset();
        sampler_set_.reset();
        pixel_shader_.reset();
        vertex_shader_.reset();
        sampler_.reset();
        index_buffer_.buffer.reset();
        vertex_buffer_.buffer.reset();
        context_ = nullptr;
        document_ = nullptr;
    }

    void draw(RenderCommandList* list, RenderFramebuffer* framebuffer) {
        if (!g_overlay_visible.load(std::memory_order_relaxed) || !pipeline_ || !g_rml_initialised) {
            return;
        }

        const int width = static_cast<int>(framebuffer->getWidth());
        const int height = static_cast<int>(framebuffer->getHeight());
        if (width <= 0 || height <= 0) {
            return;
        }

        if (!context_) {
            context_ = Rml::CreateContext("lod_overlay", Rml::Vector2i(width, height), GetAdaptedInterface());
            if (!context_) {
                std::fprintf(stderr, "[UI] Failed to create RmlUi context\n");
                return;
            }
        }

        if (last_width_ != width || last_height_ != height) {
            context_->SetDimensions(Rml::Vector2i(width, height));
            last_width_ = width;
            last_height_ = height;
        }

        ensure_document();
        update_values_if_needed();

        list_ = list;
        window_width_ = width;
        window_height_ = height;
        projection_mtx_ = Rml::Matrix4f::ProjectOrtho(0.0f, float(width), float(height), 0.0f, -10000.0f, 10000.0f);
        recalculate_mvp();
        stale_buffers_.clear();
        reset_dynamic_buffer(vertex_buffer_);
        reset_dynamic_buffer(index_buffer_);

        list_->setPipeline(pipeline_.get());
        list_->setGraphicsPipelineLayout(layout_.get());
        list_->setGraphicsDescriptorSet(sampler_set_.get(), 0);

        context_->Update();
        context_->Render();

        end_dynamic_buffer(vertex_buffer_);
        end_dynamic_buffer(index_buffer_);
        list_ = nullptr;

        if (!g_logged_first_draw.exchange(true, std::memory_order_relaxed)) {
            std::fprintf(stderr, "[UI] RmlUi overlay first draw %dx%d\n", width, height);
        }
    }

    void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices,
                        Rml::TextureHandle texture, const Rml::Vector2f& translation) override {
        if (!textures_.contains(texture)) {
            texture = 0;
        }

        const uint32_t vertex_size_bytes = num_vertices * sizeof(*vertices);
        const uint32_t index_size_bytes = num_indices * sizeof(*indices);
        const uint32_t vertex_buffer_offset = allocate_dynamic_data(vertex_buffer_, vertex_size_bytes);
        const uint32_t index_buffer_offset = allocate_dynamic_data(index_buffer_, index_size_bytes);
        std::memcpy(vertex_buffer_.mapped_data + vertex_buffer_offset, vertices, vertex_size_bytes);
        std::memcpy(index_buffer_.mapped_data + index_buffer_offset, indices, index_size_bytes);

        list_->setViewports(RenderViewport{0, 0, float(window_width_), float(window_height_)});
        if (scissor_enabled_) {
            list_->setScissors(RenderRect{
                scissor_x_,
                scissor_y_,
                scissor_x_ + scissor_width_,
                scissor_y_ + scissor_height_});
        } else {
            list_->setScissors(RenderRect{0, 0, window_width_, window_height_});
        }

        RenderIndexBufferView index_view{index_buffer_.buffer->at(index_buffer_offset), index_size_bytes, RenderFormat::R32_UINT};
        list_->setIndexBuffer(&index_view);
        RenderVertexBufferView vertex_view{vertex_buffer_.buffer->at(vertex_buffer_offset), vertex_size_bytes};
        list_->setVertexBuffers(0, &vertex_view, 1, &vertex_slot_);

        TextureHandle& texture_handle = textures_.at(texture);
        if (!texture_handle.transitioned) {
            list_->barriers(RenderBarrierStage::GRAPHICS,
                            RenderTextureBarrier(texture_handle.texture.get(), RenderTextureLayout::SHADER_READ));
            texture_handle.transitioned = true;
        }

        list_->setGraphicsDescriptorSet(texture_handle.set.get(), 1);
        RmlPushConstants constants{
            .transform = mvp_,
            .translation = translation,
        };
        list_->setGraphicsPushConstants(0, &constants);
        list_->drawIndexedInstanced(num_indices, 1, 0, 0, 0);
    }

    void EnableScissorRegion(bool enable) override {
        scissor_enabled_ = enable;
    }

    void SetScissorRegion(int x, int y, int width, int height) override {
        scissor_x_ = x;
        scissor_y_ = y;
        scissor_width_ = width;
        scissor_height_ = height;
    }

    bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String&) override {
        texture_handle = 1;
        texture_dimensions = Rml::Vector2i{1, 1};
        return true;
    }

    bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source,
                         const Rml::Vector2i& source_dimensions) override {
        texture_handle = next_texture_handle_++;
        return create_texture(texture_handle, source, source_dimensions);
    }

    void ReleaseTexture(Rml::TextureHandle texture) override {
        textures_.erase(texture);
    }

    void SetTransform(const Rml::Matrix4f* transform) override {
        transform_ = transform ? *transform : Rml::Matrix4f::Identity();
        recalculate_mvp();
    }

private:
    void ensure_document() {
        if (document_) {
            return;
        }
        document_ = context_->LoadDocumentFromMemory(make_document_rml());
        if (!document_) {
            std::fprintf(stderr, "[UI] Failed to load RmlUi overlay document\n");
            return;
        }
        document_->Show();
        last_options_rml_.clear();
    }

    void update_values_if_needed() {
        if (!document_) {
            return;
        }
        std::string options = make_options_rml();
        if (options == last_options_rml_) {
            return;
        }
        if (Rml::Element* settings = document_->GetElementById("settings")) {
            settings->SetInnerRML(options);
            last_options_rml_ = std::move(options);
        }
    }

    void recalculate_mvp() {
        mvp_ = projection_mtx_ * transform_;
    }

    void resize_dynamic_buffer(DynamicBuffer& dynamic_buffer, uint32_t new_size, bool map = true) {
        if (dynamic_buffer.mapped_data) {
            dynamic_buffer.buffer->unmap();
        }
        if (dynamic_buffer.buffer) {
            stale_buffers_.emplace_back(std::move(dynamic_buffer.buffer));
        }
        dynamic_buffer.buffer = device_->createBuffer(RenderBufferDesc::UploadBuffer(new_size, dynamic_buffer.flags));
        dynamic_buffer.size = new_size;
        dynamic_buffer.bytes_used = 0;
        dynamic_buffer.mapped_data = map ? reinterpret_cast<uint8_t*>(dynamic_buffer.buffer->map()) : nullptr;
    }

    void reset_dynamic_buffer(DynamicBuffer& dynamic_buffer) {
        assert(dynamic_buffer.mapped_data == nullptr);
        dynamic_buffer.bytes_used = 0;
        dynamic_buffer.mapped_data = reinterpret_cast<uint8_t*>(dynamic_buffer.buffer->map());
    }

    void end_dynamic_buffer(DynamicBuffer& dynamic_buffer) {
        assert(dynamic_buffer.mapped_data != nullptr);
        dynamic_buffer.buffer->unmap();
        dynamic_buffer.mapped_data = nullptr;
    }

    uint32_t allocate_dynamic_data(DynamicBuffer& dynamic_buffer, uint32_t num_bytes) {
        uint32_t total_bytes = num_bytes + dynamic_buffer.bytes_used;
        if (total_bytes > dynamic_buffer.size) {
            resize_dynamic_buffer(dynamic_buffer, total_bytes + total_bytes / 2);
        }
        uint32_t offset = dynamic_buffer.bytes_used;
        dynamic_buffer.bytes_used += num_bytes;
        return offset;
    }

    bool create_texture(Rml::TextureHandle texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {
        std::unique_ptr<RenderTexture> texture =
            device_->createTexture(RenderTextureDesc::Texture2D(source_dimensions.x, source_dimensions.y, 1, kRmlTextureFormat));
        if (!texture) {
            return false;
        }

        const uint32_t row_pitch = source_dimensions.x * kTextureBytesPerPixel;
        uint32_t row_byte_width, row_byte_padding;
        calculate_texture_row_width_padding(row_pitch, row_byte_width, row_byte_padding);
        const uint32_t row_width = row_byte_width / kTextureBytesPerPixel;
        const uint32_t uploaded_size_bytes = row_byte_width * source_dimensions.y;

        if (uploaded_size_bytes > copy_buffer_size_) {
            copy_buffer_size_ = (uploaded_size_bytes * 3) / 2 + 256;
            copy_buffer_ = device_->createBuffer(RenderBufferDesc::UploadBuffer(copy_buffer_size_));
        }

        uint8_t* dst = static_cast<uint8_t*>(copy_buffer_->map());
        const Rml::byte* src = source;
        for (int row = 0; row < source_dimensions.y; row++) {
            std::memcpy(dst, src, row_pitch);
            src += row_pitch;
            dst += row_byte_width;
        }
        (void)row_byte_padding;
        copy_buffer_->unmap();

        copy_command_list_->begin();
        copy_command_list_->barriers(RenderBarrierStage::COPY,
            RenderTextureBarrier(texture.get(), RenderTextureLayout::COPY_DEST));
        copy_command_list_->copyTextureRegion(
            RenderTextureCopyLocation::Subresource(texture.get()),
            RenderTextureCopyLocation::PlacedFootprint(copy_buffer_.get(), kRmlTextureFormat,
                source_dimensions.x, source_dimensions.y, 1, row_width));
        copy_command_list_->end();
        copy_command_queue_->executeCommandLists(copy_command_list_.get(), copy_command_fence_.get());
        copy_command_queue_->waitForCommandFence(copy_command_fence_.get());

        std::unique_ptr<RenderDescriptorSet> set = texture_set_builder_->create(device_);
        set->setTexture(texture_descriptor_index_, texture.get(), RenderTextureLayout::SHADER_READ);
        textures_.emplace(texture_handle, TextureHandle{std::move(texture), std::move(set), false});
        return true;
    }

    RenderInterface* interface_ = nullptr;
    RenderDevice* device_ = nullptr;
    RenderInputSlot vertex_slot_{0, sizeof(Rml::Vertex)};
    DynamicBuffer vertex_buffer_{};
    DynamicBuffer index_buffer_{};
    std::vector<std::unique_ptr<RenderBuffer>> stale_buffers_{};
    std::unique_ptr<RenderSampler> sampler_{};
    std::unique_ptr<RenderShader> vertex_shader_{};
    std::unique_ptr<RenderShader> pixel_shader_{};
    std::unique_ptr<RenderDescriptorSet> sampler_set_{};
    std::unique_ptr<RenderDescriptorSetBuilder> texture_set_builder_{};
    std::unique_ptr<RenderPipelineLayout> layout_{};
    std::unique_ptr<RenderPipeline> pipeline_{};
    uint32_t texture_descriptor_index_ = 0;
    std::unordered_map<Rml::TextureHandle, TextureHandle> textures_{};
    Rml::TextureHandle next_texture_handle_ = 2;
    std::unique_ptr<RenderCommandQueue> copy_command_queue_{};
    std::unique_ptr<RenderCommandList> copy_command_list_{};
    std::unique_ptr<RenderCommandFence> copy_command_fence_{};
    std::unique_ptr<RenderBuffer> copy_buffer_{};
    uint64_t copy_buffer_size_ = 0;
    RenderCommandList* list_ = nullptr;
    bool scissor_enabled_ = false;
    int scissor_x_ = 0;
    int scissor_y_ = 0;
    int scissor_width_ = 0;
    int scissor_height_ = 0;
    int window_width_ = 0;
    int window_height_ = 0;
    int last_width_ = 0;
    int last_height_ = 0;
    Rml::Matrix4f projection_mtx_ = Rml::Matrix4f::Identity();
    Rml::Matrix4f transform_ = Rml::Matrix4f::Identity();
    Rml::Matrix4f mvp_ = Rml::Matrix4f::Identity();
    Rml::Context* context_ = nullptr;
    Rml::ElementDocument* document_ = nullptr;
    std::string last_options_rml_{};
};

RmlOverlayRenderer g_renderer;

void overlay_hook_init(RenderInterface* render_interface, RenderDevice* device) {
    g_renderer.init(render_interface, device);
}

void overlay_hook_draw(RenderCommandList* list, RenderFramebuffer* swap_chain_framebuffer) {
    g_renderer.draw(list, swap_chain_framebuffer);
}

void overlay_hook_deinit() {
    g_renderer.reset();
}

} // namespace

void lod::ui::set_render_hooks() {
    RT64::SetRenderHooks(overlay_hook_init, overlay_hook_draw, overlay_hook_deinit);
}

void lod::ui::toggle_overlay() {
    g_overlay_visible.store(!g_overlay_visible.load(std::memory_order_relaxed), std::memory_order_relaxed);
}

void lod::ui::set_overlay_visible(bool visible) {
    g_overlay_visible.store(visible, std::memory_order_relaxed);
}

bool lod::ui::overlay_visible() {
    return g_overlay_visible.load(std::memory_order_relaxed);
}
