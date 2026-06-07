#include "lod_ui_overlay.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifndef HLSL_CPU
#define HLSL_CPU
#endif
#include "rt64_render_hooks.h"
#include "plume_render_interface_builders.h"

#include "RmlUi/Core.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Event.h"
#include "RmlUi/Core/EventListener.h"
#include "RmlUi/Core/ID.h"
#include "RmlUi/Core/RenderInterfaceCompatibility.h"
#include "RmlUi/Core/StringUtilities.h"
#include "RmlUi_Platform_SDL.h"

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



extern uint8_t* rdram_ptr_for_debug;
extern "C" uint32_t lod_current_map_overlay_rom();
extern "C" uint32_t lod_current_map_overlay_size();
extern "C" int lod_current_map_overlay_load_count();
extern "C" int lod_ni_overlay_loaded_0f_pair();
extern "C" int lod_ni_overlay_loaded_0e_pair();

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

enum class UiTab {
    General,
    Graphics,
    Controls,
    Audio,
    Debug,
};

enum class PromptAction {
    None,
    DiscardAndClose,
    ResetGraphicsDefaults,
};

std::atomic_bool g_overlay_visible{false};
std::atomic_bool g_open_requested{false};
std::atomic_bool g_close_requested{false};
std::atomic_bool g_graphics_config_changed{false};
std::atomic_bool g_controls_config_changed{false};
std::atomic_bool g_audio_config_changed{false};
std::atomic_bool g_logged_first_draw{false};
bool g_rml_initialised = false;

std::mutex g_ui_mutex;
std::mutex g_event_mutex;
std::vector<SDL_Event> g_queued_events;

std::mutex g_callback_mutex;
lod::ui::GraphicsApplyCallback g_graphics_apply_callback;
std::string g_config_path_display;
std::string g_connected_controller_name = "None detected";

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

template <typename Enum>
Enum next_enum_option(Enum value) {
    int raw = static_cast<int>(value) + 1;
    int count = static_cast<int>(Enum::OptionCount);
    if (raw >= count) {
        raw = 0;
    }
    return static_cast<Enum>(raw);
}

template <typename Enum>
Enum prev_enum_option(Enum value) {
    int raw = static_cast<int>(value) - 1;
    int count = static_cast<int>(Enum::OptionCount);
    if (raw < 0) {
        raw = count - 1;
    }
    return static_cast<Enum>(raw);
}

ultramodern::renderer::GraphicsConfig make_default_graphics_config() {
    ultramodern::renderer::GraphicsConfig config{};
    config.developer_mode = false;
    config.res_option = ultramodern::renderer::Resolution::Original2x;
    config.wm_option = ultramodern::renderer::WindowMode::Windowed;
    config.hr_option = ultramodern::renderer::HUDRatioMode::Original;
    config.api_option = ultramodern::renderer::GraphicsApi::Auto;
    config.ar_option = ultramodern::renderer::AspectRatio::Original;
    config.msaa_option = ultramodern::renderer::Antialiasing::None;
    config.rr_option = ultramodern::renderer::RefreshRate::Original;
    config.hpfb_option = ultramodern::renderer::HighPrecisionFramebuffer::Auto;
    config.rr_manual_value = 60;
    config.ds_option = 1;
    return config;
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

const char* hpfb_name(ultramodern::renderer::HighPrecisionFramebuffer value) {
    switch (value) {
        case ultramodern::renderer::HighPrecisionFramebuffer::Auto: return "Auto";
        case ultramodern::renderer::HighPrecisionFramebuffer::On: return "On";
        case ultramodern::renderer::HighPrecisionFramebuffer::Off: return "Off";
        default: return "Unknown";
    }
}

const char* api_name(ultramodern::renderer::GraphicsApi value) {
    switch (value) {
        case ultramodern::renderer::GraphicsApi::Auto: return "Auto";
        case ultramodern::renderer::GraphicsApi::D3D12: return "D3D12";
        case ultramodern::renderer::GraphicsApi::Vulkan: return "Vulkan";
        case ultramodern::renderer::GraphicsApi::Metal: return "Metal";
        default: return "Unknown";
    }
}

std::string yes_no(bool value) {
    return value ? "On" : "Off";
}

std::string escape_rml(const std::string& text) {
    return Rml::StringUtilities::EncodeRml(text);
}

std::string hex_u32(uint32_t value) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "0x%08X", value);
    return buf;
}

int32_t current_gamestate() {
    uint8_t* rdram = rdram_ptr_for_debug;
    if (rdram == nullptr) {
        return 0;
    }

    uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
    if (gsm_addr == 0) {
        return 0;
    }

    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (gsm_phys + 0x2C > 0x800000) {
        return 0;
    }

    return *(int32_t*)(rdram + gsm_phys + 0x24);
}

uint32_t current_exec_flags() {
    uint8_t* rdram = rdram_ptr_for_debug;
    return rdram != nullptr ? *(uint32_t*)(rdram + 0x001CABC8) : 0;
}

bool show_debug_tab() {
    const char* value = std::getenv("RECOMP_UI_SHOW_DEBUG");
    return value != nullptr && value[0] != '\0' && value[0] != '0';
}

std::string make_document_rml() {
    return R"RML(
<rml>
<head>
<style>
body {
    width: 100%; height: 100%;
    font-family: Arial;
    color: #f7ead0;
}
button {
    min-width: 84px;
    padding: 8px 12px;
    color: #160f10;
    background-color: #d6af63;
    border: 1px #f5d58b;
    text-align: center;
}
button:hover, button:focus {
    background-color: #f0ce82;
    border: 1px #fff3c6;
}
button.danger {
    color: #fff0ea;
    background-color: #8f2633;
    border: 1px #d65b66;
}
button.secondary {
    color: #efe3ca;
    background-color: #352d31;
    border: 1px #7d6a52;
}
button.choice {
    width: 118px;
}
button.choice.changed {
    color: #211400;
    background-color: #f3c15a;
}
.recomp-root {
    position: absolute;
    left: 0; top: 0; right: 0; bottom: 0;
}
.recomp-backdrop {
    position: absolute;
    left: 0; top: 0; right: 0; bottom: 0;
    background-color: rgba(0, 0, 0, 0.55);
}
.recomp-panel {
    position: absolute;
    left: 64px; top: 48px;
    width: 1060px;
    min-height: 0;
    background-color: #121015;
    border: 2px #caa35f;
}
.recomp-header {
    height: 82px;
    padding: 18px 26px 0 26px;
    background-color: #4a161f;
    border-bottom: 2px #caa35f;
}
.recomp-title {
    margin: 0;
    font-size: 28px;
    color: #f7d98f;
}
.recomp-subtitle {
    margin-top: 5px;
    font-size: 13px;
    color: #d5c6ad;
}
.recomp-body {
    display: flex;
    flex-direction: row;
}
.recomp-tabs {
    width: 190px;
    min-height: 610px;
    padding: 18px 18px;
    background-color: #08090d;
    border-right: 1px #594d42;
}
.recomp-tab {
    display: block;
    width: 154px;
    margin-bottom: 9px;
    padding: 12px 14px;
    color: #d4c8b7;
    background-color: transparent;
    border: 1px transparent;
    text-align: left;
}
.recomp-tab:hover, .recomp-tab:focus {
    color: #fff5d2;
    background-color: rgba(202, 163, 95, 0.13);
    border: 1px #7d6a52;
}
.recomp-tab.active {
    color: #fff6d4;
    background-color: rgba(202, 163, 95, 0.22);
    border: 1px #caa35f;
}
.recomp-content {
    width: 810px;
    padding: 16px 24px 20px 24px;
}
.recomp-page-title {
    display: block;
    margin: 0 0 4px 0;
    font-size: 22px;
    color: #ffe8a6;
}
.recomp-page-help {
    display: block;
    margin: 0 0 10px 0;
    font-size: 11px;
    color: #b7afa6;
}
.recomp-setting-row {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
    min-height: 50px;
    padding: 5px 0;
    border-bottom: 1px rgba(255,255,255,0.10);
}
.recomp-setting-row.changed {
    background-color: rgba(243, 193, 90, 0.08);
}
.recomp-setting-row h2 {
    display: block;
    margin: 0 0 3px 0;
    font-size: 15px;
    color: #fff3cf;
}
.recomp-setting-row p {
    display: block;
    margin: 0;
    font-size: 11px;
    color: #aaa49d;
}
.recomp-setting-copy {
    width: 450px;
}
.recomp-setting-control {
    display: flex;
    flex-direction: row;
    justify-content: flex-end;
    align-items: center;
    width: 340px;
}
.recomp-value {
    min-width: 124px;
    padding: 8px 10px;
    color: #f4e6c9;
    background-color: #242027;
    border: 1px #645748;
    text-align: center;
}
.recomp-value.changed {
    color: #fff0bd;
    border: 1px #d6af63;
}
.recomp-actions {
    display: flex;
    flex-direction: row;
    justify-content: flex-end;
    gap: 10px;
    margin-top: 16px;
}
.recomp-footer {
    padding: 13px 24px 16px 24px;
    font-size: 11px;
    color: #bab1a6;
    border-top: 1px #4d423c;
}
.recomp-pill {
    display: inline-block;
    padding: 3px 8px;
    color: #241708;
    background-color: #d6af63;
    border: 1px #f5d58b;
}
.recomp-muted {
    color: #aaa49d;
}
.recomp-prompt-scrim {
    position: absolute;
    left: 0; top: 0; right: 0; bottom: 0;
    background-color: rgba(0,0,0,0.42);
}
.recomp-prompt {
    position: absolute;
    left: 310px; top: 220px;
    width: 520px;
    padding: 24px;
    background-color: #171217;
    border: 2px #d6af63;
}
.recomp-prompt h1 {
    display: block;
    margin: 0 0 10px 0;
    font-size: 24px;
    color: #ffe2a0;
}
.recomp-prompt p {
    display: block;
    margin: 0 0 18px 0;
    color: #d6cbbc;
    font-size: 14px;
}
.recomp-prompt-actions {
    display: flex;
    flex-direction: row;
    justify-content: flex-end;
    gap: 10px;
}
</style>
</head>
<body>
<div class='recomp-root'>
  <div class='recomp-backdrop'></div>
  <div class='recomp-panel'>
    <div class='recomp-header'>
      <h1 class='recomp-title'>Castlevania: Legacy of Darkness Recompiled</h1>
      <p class='recomp-subtitle' id='subtitle'>Settings overlay</p>
    </div>
    <div class='recomp-body'>
      <div class='recomp-tabs' id='tabs'></div>
      <div class='recomp-content' id='content'></div>
    </div>
    <div class='recomp-footer' id='footer'>F1 toggles settings. Esc/B closes. Tab and arrow keys move focus. Enter/A activates.</div>
  </div>
  <div id='prompt_host'></div>
</div>
</body>
</rml>
)RML";
}

class RmlOverlayRenderer;

class ActionListener final : public Rml::EventListener {
public:
    ActionListener(RmlOverlayRenderer& renderer, std::string action, std::string param)
        : renderer_(renderer), action_(std::move(action)), param_(std::move(param)) {}

    void ProcessEvent(Rml::Event& event) override;

private:
    RmlOverlayRenderer& renderer_;
    std::string action_;
    std::string param_;
};

class RmlOverlayRenderer final : public Rml::RenderInterfaceCompatibility {
public:
    void init(RenderInterface* render_interface, RenderDevice* device) {
        std::lock_guard<std::mutex> lock(g_ui_mutex);
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
        const void* vertex_blob = LOD_UI_SHADER_BLOB(InterfaceVS, shader_format);
        const void* pixel_blob = LOD_UI_SHADER_BLOB(InterfacePS, shader_format);
        const size_t vertex_blob_size = LOD_UI_SHADER_SIZE(InterfaceVS, shader_format);
        const size_t pixel_blob_size = LOD_UI_SHADER_SIZE(InterfacePS, shader_format);
        if (vertex_blob == nullptr || pixel_blob == nullptr || vertex_blob_size == 0 || pixel_blob_size == 0) {
            std::fprintf(stderr, "[UI] Unsupported renderer shader format for RmlUi overlay\n");
            return;
        }

        vertex_shader_ = device_->createShader(vertex_blob, vertex_blob_size, "VSMain", shader_format);
        pixel_shader_ = device_->createShader(pixel_blob, pixel_blob_size, "PSMain", shader_format);

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
            system_interface_ = std::make_unique<SystemInterface_SDL>();
            Rml::SetSystemInterface(system_interface_.get());
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
        std::lock_guard<std::mutex> lock(g_ui_mutex);
        if (document_) {
            document_->Close();
            document_ = nullptr;
        }
        action_listeners_.clear();
        if (context_) {
            Rml::RemoveContext(context_->GetName());
            context_ = nullptr;
        }
        if (g_rml_initialised) {
            Rml::Shutdown();
            g_rml_initialised = false;
        }
        system_interface_.reset();
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
        std::lock_guard<std::mutex> lock(g_ui_mutex);
        if (!g_overlay_visible.load(std::memory_order_relaxed) || !pipeline_ || !g_rml_initialised) {
            was_visible_ = false;
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
        if (!document_) {
            return;
        }

        if (g_open_requested.exchange(false, std::memory_order_relaxed) || !was_visible_) {
            open_settings();
        }
        was_visible_ = true;

        if (g_graphics_config_changed.exchange(false, std::memory_order_relaxed)) {
            sync_from_active_graphics(false);
        }
        if (g_controls_config_changed.exchange(false, std::memory_order_relaxed) ||
            g_audio_config_changed.exchange(false, std::memory_order_relaxed)) {
            dirty_document_ = true;
        }
        if (g_close_requested.exchange(false, std::memory_order_relaxed)) {
            request_close_from_ui();
        }

        drain_platform_events();
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

    bool has_pending_graphics_changes() const {
        return pending_graphics_ != active_graphics_;
    }

    void handle_action(const std::string& action, const std::string& param) {
        if (action == "set_tab") {
            if (param == "general") active_tab_ = UiTab::General;
            else if (param == "graphics") active_tab_ = UiTab::Graphics;
            else if (param == "controls") active_tab_ = UiTab::Controls;
            else if (param == "audio") active_tab_ = UiTab::Audio;
            else if (param == "debug" && show_debug_tab()) active_tab_ = UiTab::Debug;
            pending_focus_id_ = first_focus_for_tab(active_tab_);
            dirty_document_ = true;
            return;
        }

        if (action == "close_settings") {
            request_close_from_ui();
            return;
        }

        if (action == "prompt_confirm") {
            confirm_prompt();
            return;
        }

        if (action == "prompt_cancel") {
            close_prompt();
            return;
        }

        if (action == "apply_graphics") {
            apply_graphics();
            return;
        }

        if (action == "discard_graphics") {
            discard_graphics();
            return;
        }

        if (action == "reset_graphics_defaults") {
            open_prompt("Reset graphics defaults?",
                        "This sets the pending graphics values back to the project defaults. Apply is still required to save them.",
                        "Reset", "Cancel", PromptAction::ResetGraphicsDefaults, true);
            return;
        }

        if (action == "cycle_graphics") {
            cycle_graphics(param, true);
            return;
        }

        if (action == "cycle_graphics_prev") {
            cycle_graphics(param, false);
            return;
        }

        std::fprintf(stderr, "[UI] Ignoring unknown action '%s' param '%s'\n", action.c_str(), param.c_str());
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
        dirty_document_ = true;
    }

    void open_settings() {
        active_tab_ = UiTab::Graphics;
        close_prompt();
        sync_from_active_graphics(true);
        pending_focus_id_ = first_focus_for_tab(active_tab_);
        dirty_document_ = true;
    }

    void sync_from_active_graphics(bool reset_pending) {
        active_graphics_ = ultramodern::renderer::get_graphics_config();
        if (reset_pending || !has_pending_graphics_changes()) {
            pending_graphics_ = active_graphics_;
        }
        dirty_document_ = true;
    }

    void request_close_from_ui() {
        if (prompt_open_) {
            close_prompt();
            return;
        }
        if (has_pending_graphics_changes()) {
            open_prompt("Discard unsaved graphics changes?",
                        "You have pending graphics edits that have not been applied. Discard them and return to gameplay?",
                        "Discard", "Keep Editing", PromptAction::DiscardAndClose, true);
            return;
        }
        g_overlay_visible.store(false, std::memory_order_relaxed);
        was_visible_ = false;
    }

    void open_prompt(std::string title, std::string body, std::string confirm_label,
                     std::string cancel_label, PromptAction action, bool destructive) {
        prompt_open_ = true;
        prompt_title_ = std::move(title);
        prompt_body_ = std::move(body);
        prompt_confirm_label_ = std::move(confirm_label);
        prompt_cancel_label_ = std::move(cancel_label);
        prompt_action_ = action;
        prompt_destructive_ = destructive;
        pending_focus_id_ = destructive ? "prompt_cancel" : "prompt_confirm";
        dirty_document_ = true;
    }

    void close_prompt() {
        prompt_open_ = false;
        prompt_title_.clear();
        prompt_body_.clear();
        prompt_confirm_label_.clear();
        prompt_cancel_label_.clear();
        prompt_action_ = PromptAction::None;
        prompt_destructive_ = false;
        pending_focus_id_ = first_focus_for_tab(active_tab_);
        dirty_document_ = true;
    }

    void confirm_prompt() {
        const PromptAction action = prompt_action_;
        close_prompt();
        switch (action) {
            case PromptAction::DiscardAndClose:
                discard_graphics();
                g_overlay_visible.store(false, std::memory_order_relaxed);
                was_visible_ = false;
                break;
            case PromptAction::ResetGraphicsDefaults:
                pending_graphics_ = make_default_graphics_config();
                dirty_document_ = true;
                break;
            case PromptAction::None:
                break;
        }
    }

    void apply_graphics() {
        if (!has_pending_graphics_changes()) {
            return;
        }

        lod::ui::GraphicsApplyCallback callback;
        {
            std::lock_guard<std::mutex> lock(g_callback_mutex);
            callback = g_graphics_apply_callback;
        }

        if (callback) {
            callback(pending_graphics_, "UI");
        } else {
            ultramodern::renderer::set_graphics_config(pending_graphics_);
        }
        active_graphics_ = pending_graphics_;
        dirty_document_ = true;
    }

    void discard_graphics() {
        pending_graphics_ = active_graphics_;
        dirty_document_ = true;
    }

    void cycle_graphics(const std::string& field, bool forward) {
        if (field == "window_mode") {
            pending_graphics_.wm_option = forward ? next_enum_option(pending_graphics_.wm_option) : prev_enum_option(pending_graphics_.wm_option);
        } else if (field == "resolution") {
            pending_graphics_.res_option = forward ? next_enum_option(pending_graphics_.res_option) : prev_enum_option(pending_graphics_.res_option);
        } else if (field == "aspect_ratio") {
            pending_graphics_.ar_option = forward ? next_enum_option(pending_graphics_.ar_option) : prev_enum_option(pending_graphics_.ar_option);
        } else if (field == "antialiasing") {
            pending_graphics_.msaa_option = forward ? next_enum_option(pending_graphics_.msaa_option) : prev_enum_option(pending_graphics_.msaa_option);
        } else if (field == "refresh_rate") {
            pending_graphics_.rr_option = forward ? next_enum_option(pending_graphics_.rr_option) : prev_enum_option(pending_graphics_.rr_option);
        } else if (field == "high_precision_framebuffer") {
            pending_graphics_.hpfb_option = forward ? next_enum_option(pending_graphics_.hpfb_option) : prev_enum_option(pending_graphics_.hpfb_option);
        } else if (field == "downsample") {
            pending_graphics_.ds_option += forward ? 1 : -1;
            pending_graphics_.ds_option = std::clamp(pending_graphics_.ds_option, 1, 8);
        } else if (field == "refresh_rate_manual") {
            pending_graphics_.rr_manual_value += forward ? 5 : -5;
            pending_graphics_.rr_manual_value = std::clamp(pending_graphics_.rr_manual_value, 20, 360);
        } else {
            std::fprintf(stderr, "[UI] Ignoring unknown graphics field '%s'\n", field.c_str());
            return;
        }
        dirty_document_ = true;
    }

    std::string first_focus_for_tab(UiTab tab) const {
        switch (tab) {
            case UiTab::General: return "tab_general";
            case UiTab::Graphics: return "window_mode_next";
            case UiTab::Controls: return "tab_controls";
            case UiTab::Audio: return "tab_audio";
            case UiTab::Debug: return "tab_debug";
        }
        return "tab_graphics";
    }

    std::vector<std::string> focus_order() const {
        if (prompt_open_) {
            return {"prompt_cancel", "prompt_confirm"};
        }

        std::vector<std::string> ids{
            "tab_general", "tab_graphics", "tab_controls", "tab_audio",
        };
        if (show_debug_tab()) {
            ids.emplace_back("tab_debug");
        }

        if (active_tab_ == UiTab::Graphics) {
            const char* fields[] = {
                "window_mode", "resolution", "downsample", "aspect_ratio",
                "antialiasing", "refresh_rate", "refresh_rate_manual",
                "high_precision_framebuffer",
            };
            for (const char* field : fields) {
                ids.emplace_back(std::string(field) + "_prev");
                ids.emplace_back(std::string(field) + "_next");
            }
            ids.emplace_back("reset_graphics");
            ids.emplace_back("discard_graphics");
            ids.emplace_back("apply_graphics");
            ids.emplace_back("close_settings_graphics");
        } else {
            switch (active_tab_) {
                case UiTab::General: ids.emplace_back("close_settings_general"); break;
                case UiTab::Controls: ids.emplace_back("close_settings_controls"); break;
                case UiTab::Audio: ids.emplace_back("close_settings_audio"); break;
                case UiTab::Debug: ids.emplace_back("close_settings_debug"); break;
                case UiTab::Graphics: break;
            }
        }
        return ids;
    }

    bool focus_element_by_id(const std::string& id) {
        if (!document_ || id.empty()) {
            return false;
        }
        Rml::Element* element = document_->GetElementById(id);
        if (!element) {
            return false;
        }
        element->Focus(true);
        return true;
    }

    void apply_pending_focus() {
        if (!pending_focus_id_.empty()) {
            if (focus_element_by_id(pending_focus_id_)) {
                pending_focus_id_.clear();
                return;
            }
            pending_focus_id_.clear();
        }
        if (!context_->GetFocusElement()) {
            focus_element_by_id(first_focus_for_tab(active_tab_));
        }
    }

    void move_focus(int delta) {
        std::vector<std::string> ids = focus_order();
        if (ids.empty()) {
            return;
        }

        std::string focused_id;
        if (Rml::Element* focused = context_->GetFocusElement()) {
            focused_id = focused->GetId();
        }

        int index = 0;
        auto it = std::find(ids.begin(), ids.end(), focused_id);
        if (it != ids.end()) {
            index = int(std::distance(ids.begin(), it));
        }
        index = (index + delta) % int(ids.size());
        if (index < 0) {
            index += int(ids.size());
        }
        focus_element_by_id(ids[size_t(index)]);
    }

    void click_focused() {
        if (Rml::Element* focused = context_->GetFocusElement()) {
            focused->Click();
        } else {
            focus_element_by_id(first_focus_for_tab(active_tab_));
        }
    }

    void select_adjacent_tab(int delta) {
        std::vector<UiTab> tabs{UiTab::General, UiTab::Graphics, UiTab::Controls, UiTab::Audio};
        if (show_debug_tab()) {
            tabs.emplace_back(UiTab::Debug);
        }

        auto it = std::find(tabs.begin(), tabs.end(), active_tab_);
        int index = it != tabs.end() ? int(std::distance(tabs.begin(), it)) : 1;
        index = (index + delta) % int(tabs.size());
        if (index < 0) {
            index += int(tabs.size());
        }
        active_tab_ = tabs[size_t(index)];
        pending_focus_id_ = first_focus_for_tab(active_tab_);
        dirty_document_ = true;
    }

    void drain_platform_events() {
        std::vector<SDL_Event> events;
        {
            std::lock_guard<std::mutex> lock(g_event_mutex);
            events.swap(g_queued_events);
        }

        for (SDL_Event& event : events) {
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        request_close_from_ui();
                        continue;
                    case SDLK_UP:
                        move_focus(-1);
                        continue;
                    case SDLK_DOWN:
                        move_focus(1);
                        continue;
                    case SDLK_LEFT:
                        move_focus(-1);
                        continue;
                    case SDLK_RIGHT:
                        move_focus(1);
                        continue;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                    case SDLK_SPACE:
                        click_focused();
                        continue;
                    default:
                        break;
                }
            }

            if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                switch (event.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_A: click_focused(); continue;
                    case SDL_CONTROLLER_BUTTON_B: request_close_from_ui(); continue;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP: move_focus(-1); continue;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN: move_focus(1); continue;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT: move_focus(-1); continue;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: move_focus(1); continue;
                    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: select_adjacent_tab(-1); continue;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: select_adjacent_tab(1); continue;
                    default: continue;
                }
            }

            if (event.type == SDL_CONTROLLERAXISMOTION &&
                (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX || event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)) {
                constexpr int kAxisThreshold = 21000;
                constexpr uint32_t kRepeatMs = 180;
                const uint32_t now = SDL_GetTicks();
                if (now >= next_axis_nav_ms_ && (event.caxis.value <= -kAxisThreshold || event.caxis.value >= kAxisThreshold)) {
                    next_axis_nav_ms_ = now + kRepeatMs;
                    if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                        move_focus(event.caxis.value < 0 ? -1 : 1);
                    } else {
                        move_focus(event.caxis.value < 0 ? -1 : 1);
                    }
                }
                continue;
            }

            RmlSDL::InputEventHandler(context_, event);
        }
    }

    std::string tab_button(const char* id, const char* label, UiTab tab) const {
        std::ostringstream out;
        out << "<button id='" << id << "' class='recomp-tab" << (active_tab_ == tab ? " active" : "") << "'>"
            << label << "</button>";
        return out.str();
    }

    std::string make_tabs_rml() const {
        std::ostringstream out;
        out << tab_button("tab_general", "General", UiTab::General);
        out << tab_button("tab_graphics", "Graphics", UiTab::Graphics);
        out << tab_button("tab_controls", "Controls", UiTab::Controls);
        out << tab_button("tab_audio", "Audio", UiTab::Audio);
        if (show_debug_tab()) {
            out << tab_button("tab_debug", "Debug", UiTab::Debug);
        }
        return out.str();
    }

    std::string make_readonly_row(const std::string& title, const std::string& description, const std::string& value) const {
        std::ostringstream out;
        out << "<div class='recomp-setting-row'><div class='recomp-setting-copy'><h2>" << escape_rml(title) << "</h2><p>"
            << escape_rml(description) << "</p></div><div class='recomp-value'>" << escape_rml(value) << "</div></div>";
        return out.str();
    }

    std::string make_graphics_row(const char* field, const std::string& title, const std::string& description,
                                  const std::string& value, bool changed) const {
        std::ostringstream out;
        out << "<div class='recomp-setting-row" << (changed ? " changed" : "") << "'><div class='recomp-setting-copy'><h2>"
            << escape_rml(title) << "</h2><p>" << escape_rml(description)
            << "</p></div><div class='recomp-setting-control'>"
            << "<button id='" << field << "_prev' class='choice secondary'>&lt;</button>"
            << "<button id='" << field << "_next' class='choice" << (changed ? " changed" : "") << "'>"
            << escape_rml(value) << "</button>"
            << "</div></div>";
        return out.str();
    }

    std::string make_general_page() const {
        std::ostringstream out;
        out << "<h1 class='recomp-page-title'>General</h1>";
        out << "<p class='recomp-page-help'>Runtime status and release-facing paths. ROM setup is tracked separately because the current app exits before UI startup when no ROM is found.</p>";
        out << make_readonly_row("Version", "Current project version from main configuration.", "0.1.0");
        out << make_readonly_row("ROM", "Normal releases look for a legally dumped LoD ROM named rom.z64 beside the app.", "Loaded before runtime start");
        out << make_readonly_row("Settings Overlay", "The overlay starts hidden and does not affect gameplay until opened.", "F1 toggles");
        out << make_readonly_row("Config Folder", "Graphics and controls settings are stored in the platform config folder.", g_config_path_display.empty() ? "Registered at startup" : g_config_path_display);
        out << make_readonly_row("Debug Tab", "Developer-only diagnostics are hidden unless explicitly enabled.", show_debug_tab() ? "Visible" : "Set RECOMP_UI_SHOW_DEBUG=1");
        out << "<div class='recomp-actions'><button id='close_settings_general' class='secondary'>Close</button></div>";
        return out.str();
    }

    std::string make_graphics_page() const {
        std::ostringstream out;
        out << "<h1 class='recomp-page-title'>Graphics</h1>";
        out << "<p class='recomp-page-help'>Changes are staged here first. Choose Apply to write graphics.json, or Discard to return to active renderer values.</p>";
        out << make_graphics_row("window_mode", "Window Mode", "Fullscreen may recreate or resize the renderer surface.", window_name(pending_graphics_.wm_option), pending_graphics_.wm_option != active_graphics_.wm_option);
        out << make_graphics_row("resolution", "Internal Resolution", "Original2x is the current baseline default.", resolution_name(pending_graphics_.res_option), pending_graphics_.res_option != active_graphics_.res_option);
        out << make_graphics_row("downsample", "Downsample Factor", "Integer scale factor used by supported high-resolution modes.", std::to_string(pending_graphics_.ds_option), pending_graphics_.ds_option != active_graphics_.ds_option);
        out << make_graphics_row("aspect_ratio", "Aspect Ratio", "Manual currently maps to the renderer's widescreen target.", aspect_name(pending_graphics_.ar_option), pending_graphics_.ar_option != active_graphics_.ar_option);
        out << make_graphics_row("antialiasing", "Anti-Aliasing", "MSAA level requested from the renderer.", msaa_name(pending_graphics_.msaa_option), pending_graphics_.msaa_option != active_graphics_.msaa_option);
        out << make_graphics_row("refresh_rate", "Refresh Rate", "Original preserves game timing; Manual uses the value below.", refresh_name(pending_graphics_.rr_option), pending_graphics_.rr_option != active_graphics_.rr_option);
        out << make_graphics_row("refresh_rate_manual", "Manual Refresh Rate", "Step between 20 and 360 Hz in 5 Hz increments.", std::to_string(pending_graphics_.rr_manual_value) + " Hz", pending_graphics_.rr_manual_value != active_graphics_.rr_manual_value);
        out << make_graphics_row("high_precision_framebuffer", "High Precision Framebuffer", "Renderer framebuffer precision preference.", hpfb_name(pending_graphics_.hpfb_option), pending_graphics_.hpfb_option != active_graphics_.hpfb_option);
        out << make_readonly_row("Graphics API", "Startup-sensitive backend; read-only for now.", api_name(pending_graphics_.api_option));
        out << "<div class='recomp-actions'>";
        out << "<button id='reset_graphics' class='secondary'>Reset Defaults</button>";
        out << "<button id='discard_graphics' class='secondary'>Discard</button>";
        out << "<button id='apply_graphics'>Apply</button>";
        out << "<button id='close_settings_graphics' class='secondary'>Close</button>";
        out << "</div>";
        if (has_pending_graphics_changes()) {
            out << "<p class='recomp-page-help'><span class='recomp-pill'>Unsaved</span> Pending graphics changes are staged locally.</p>";
        }
        return out.str();
    }

    std::string make_controls_page() const {
        std::ostringstream out;
        out << "<h1 class='recomp-page-title'>Controls</h1>";
        out << "<p class='recomp-page-help'>Read-only first pass showing the current LoD-specific controller and keyboard defaults. Rebinding will come after the persistence model is extended.</p>";
        out << make_readonly_row("Connected Controller", "First SDL GameController opened by the runtime.", g_connected_controller_name);
        out << make_readonly_row("Controls Config", "Controller mappings are persisted in controls.json.", g_config_path_display.empty() ? "controls.json" : (g_config_path_display + "/controls.json"));
        out << make_readonly_row("Jump", "Controller A / Cross maps to N64 A.", "A / Cross → N64 A");
        out << make_readonly_row("Attack 1", "Controller X / Square maps to N64 B.", "X / Square → N64 B");
        out << make_readonly_row("Attack 2", "Controller Y / Triangle maps to N64 C-Left.", "Y / Triangle → C-Left");
        out << make_readonly_row("Collect / Interact", "Controller B / Circle maps to N64 C-Right.", "B / Circle → C-Right");
        out << make_readonly_row("Throw Item", "Right shoulder maps to N64 C-Down.", "R1/RB → C-Down");
        out << make_readonly_row("Lock On", "Left stick click maps to N64 R.", "L3 → N64 R");
        out << make_readonly_row("Triggers", "Left trigger maps to Z; right trigger maps to L.", "L2/LT → Z, R2/RT → L");
        out << make_readonly_row("Keyboard", "Enter/Right Shift/Z/Space and arrow/WASD mappings remain available.", "Hardcoded currently");
        out << "<div class='recomp-actions'><button id='close_settings_controls' class='secondary'>Close</button></div>";
        return out.str();
    }

    std::string make_audio_page() const {
        std::ostringstream out;
        out << "<h1 class='recomp-page-title'>Audio</h1>";
        out << "<p class='recomp-page-help'>The audio backend currently exposes status through runtime configuration, but not separate volume sliders. This page stays truthful instead of showing fake controls.</p>";
        out << make_readonly_row("Audio Backend", "SDL output is initialized by the main runtime.", "Enabled at startup");
        out << make_readonly_row("Audio RSP", "Generated audio RSP is the normal baseline path.", "Enabled by default");
        out << make_readonly_row("Master Volume", "No runtime volume control is exposed yet.", "Not available");
        out << make_readonly_row("Music / SFX Volume", "Per-channel mixer controls require backend support first.", "Not available");
        out << "<div class='recomp-actions'><button id='close_settings_audio' class='secondary'>Close</button></div>";
        return out.str();
    }

    std::string make_debug_page() const {
        std::ostringstream out;
        out << "<h1 class='recomp-page-title'>Debug</h1>";
        out << "<p class='recomp-page-help'>Developer-only read-only runtime diagnostics. Compile-time features are intentionally not exposed as live toggles.</p>";
        out << make_readonly_row("Gamestate", "Current game-state manager value, if RDRAM is initialized.", std::to_string(current_gamestate()));
        out << make_readonly_row("Exec Flags", "Runtime execution flags snapshot.", hex_u32(current_exec_flags()));
        out << make_readonly_row("Map Overlay ROM", "Most recent map overlay ROM offset tracked by runtime hooks.", hex_u32(lod_current_map_overlay_rom()));
        out << make_readonly_row("Map Overlay Size", "Most recent map overlay size.", hex_u32(lod_current_map_overlay_size()));
        out << make_readonly_row("Map Overlay Loads", "Map overlay load counter.", std::to_string(lod_current_map_overlay_load_count()));
        out << make_readonly_row("NI 0F Pair", "Currently loaded NI pair in 0x0F segment.", std::to_string(lod_ni_overlay_loaded_0f_pair()));
        out << make_readonly_row("NI 0E Pair", "Currently loaded NI pair in 0x0E segment.", std::to_string(lod_ni_overlay_loaded_0e_pair()));
        out << make_readonly_row("Reported RDRAM", "Default builds report 4MB to stay on the validated low-resolution route.", "0x00400000");
        out << "<div class='recomp-actions'><button id='close_settings_debug' class='secondary'>Close</button></div>";
        return out.str();
    }

    std::string make_content_rml() const {
        switch (active_tab_) {
            case UiTab::General: return make_general_page();
            case UiTab::Graphics: return make_graphics_page();
            case UiTab::Controls: return make_controls_page();
            case UiTab::Audio: return make_audio_page();
            case UiTab::Debug: return show_debug_tab() ? make_debug_page() : make_general_page();
        }
        return make_graphics_page();
    }

    std::string make_prompt_rml() const {
        if (!prompt_open_) {
            return "";
        }
        std::ostringstream out;
        out << "<div class='recomp-prompt-scrim'></div>";
        out << "<div class='recomp-prompt'><h1>" << escape_rml(prompt_title_) << "</h1><p>"
            << escape_rml(prompt_body_) << "</p><div class='recomp-prompt-actions'>";
        out << "<button id='prompt_cancel' class='secondary'>" << escape_rml(prompt_cancel_label_) << "</button>";
        out << "<button id='prompt_confirm'" << (prompt_destructive_ ? " class='danger'" : "") << ">"
            << escape_rml(prompt_confirm_label_) << "</button>";
        out << "</div></div>";
        return out.str();
    }

    void update_values_if_needed() {
        if (!document_ || !dirty_document_) {
            return;
        }

        if (Rml::Element* tabs = document_->GetElementById("tabs")) {
            tabs->SetInnerRML(make_tabs_rml());
        }
        if (Rml::Element* content = document_->GetElementById("content")) {
            content->SetInnerRML(make_content_rml());
        }
        if (Rml::Element* subtitle = document_->GetElementById("subtitle")) {
            subtitle->SetInnerRML(has_pending_graphics_changes() ? "Settings overlay — pending graphics changes" : "Settings overlay");
        }
        if (Rml::Element* prompt = document_->GetElementById("prompt_host")) {
            prompt->SetInnerRML(make_prompt_rml());
        }

        bind_actions();
        dirty_document_ = false;
    }

    void bind_action(const char* element_id, const char* action, const char* param = "") {
        if (!document_) {
            return;
        }
        Rml::Element* element = document_->GetElementById(element_id);
        if (!element) {
            return;
        }
        auto listener = std::make_unique<ActionListener>(*this, action, param);
        element->AddEventListener(Rml::EventId::Click, listener.get());
        action_listeners_.emplace_back(std::move(listener));
    }

    void bind_graphics_field(const char* field) {
        std::string prev = std::string(field) + "_prev";
        std::string next = std::string(field) + "_next";
        bind_action(prev.c_str(), "cycle_graphics_prev", field);
        bind_action(next.c_str(), "cycle_graphics", field);
    }

    void bind_actions() {
        action_listeners_.clear();
        bind_action("tab_general", "set_tab", "general");
        bind_action("tab_graphics", "set_tab", "graphics");
        bind_action("tab_controls", "set_tab", "controls");
        bind_action("tab_audio", "set_tab", "audio");
        bind_action("tab_debug", "set_tab", "debug");

        bind_action("close_settings_general", "close_settings");
        bind_action("close_settings_graphics", "close_settings");
        bind_action("close_settings_controls", "close_settings");
        bind_action("close_settings_audio", "close_settings");
        bind_action("close_settings_debug", "close_settings");

        bind_graphics_field("window_mode");
        bind_graphics_field("resolution");
        bind_graphics_field("downsample");
        bind_graphics_field("aspect_ratio");
        bind_graphics_field("antialiasing");
        bind_graphics_field("refresh_rate");
        bind_graphics_field("refresh_rate_manual");
        bind_graphics_field("high_precision_framebuffer");
        bind_action("apply_graphics", "apply_graphics");
        bind_action("discard_graphics", "discard_graphics");
        bind_action("reset_graphics", "reset_graphics_defaults");

        bind_action("prompt_confirm", "prompt_confirm");
        bind_action("prompt_cancel", "prompt_cancel");
        apply_pending_focus();
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
    std::unique_ptr<SystemInterface_SDL> system_interface_{};
    std::vector<std::unique_ptr<ActionListener>> action_listeners_{};

    UiTab active_tab_ = UiTab::Graphics;
    ultramodern::renderer::GraphicsConfig active_graphics_ = make_default_graphics_config();
    ultramodern::renderer::GraphicsConfig pending_graphics_ = make_default_graphics_config();
    bool dirty_document_ = true;
    bool was_visible_ = false;
    uint32_t next_axis_nav_ms_ = 0;
    std::string pending_focus_id_{};

    bool prompt_open_ = false;
    bool prompt_destructive_ = false;
    PromptAction prompt_action_ = PromptAction::None;
    std::string prompt_title_{};
    std::string prompt_body_{};
    std::string prompt_confirm_label_{};
    std::string prompt_cancel_label_{};
};

void ActionListener::ProcessEvent(Rml::Event& event) {
    event.StopPropagation();
    renderer_.handle_action(action_, param_);
}

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

void lod::ui::set_graphics_apply_callback(GraphicsApplyCallback callback) {
    std::lock_guard<std::mutex> lock(g_callback_mutex);
    g_graphics_apply_callback = std::move(callback);
}

void lod::ui::set_config_path_display(std::string path) {
    std::lock_guard<std::mutex> lock(g_ui_mutex);
    g_config_path_display = std::move(path);
}

void lod::ui::set_connected_controller_name(std::string name) {
    std::lock_guard<std::mutex> lock(g_ui_mutex);
    g_connected_controller_name = name.empty() ? "None detected" : std::move(name);
    g_controls_config_changed.store(true, std::memory_order_relaxed);
}

void lod::ui::toggle_overlay() {
    if (g_overlay_visible.load(std::memory_order_relaxed)) {
        g_close_requested.store(true, std::memory_order_relaxed);
    } else {
        show_overlay();
    }
}

void lod::ui::show_overlay() {
    g_overlay_visible.store(true, std::memory_order_relaxed);
    g_open_requested.store(true, std::memory_order_relaxed);
}

void lod::ui::hide_overlay() {
    g_close_requested.store(false, std::memory_order_relaxed);
    g_overlay_visible.store(false, std::memory_order_relaxed);
}

void lod::ui::set_overlay_visible(bool visible) {
    if (visible) {
        show_overlay();
    } else {
        hide_overlay();
    }
}

bool lod::ui::overlay_visible() {
    return g_overlay_visible.load(std::memory_order_relaxed);
}

bool lod::ui::captures_input() {
    return overlay_visible();
}

bool lod::ui::captures_mouse() {
    return overlay_visible();
}

bool lod::ui::queue_platform_event(const SDL_Event& event) {
    if (!overlay_visible()) {
        return false;
    }
    std::lock_guard<std::mutex> lock(g_event_mutex);
    g_queued_events.push_back(event);
    return true;
}

void lod::ui::notify_graphics_config_changed() {
    g_graphics_config_changed.store(true, std::memory_order_relaxed);
}

void lod::ui::notify_controls_config_changed() {
    g_controls_config_changed.store(true, std::memory_order_relaxed);
}

void lod::ui::notify_audio_config_changed() {
    g_audio_config_changed.store(true, std::memory_order_relaxed);
}
