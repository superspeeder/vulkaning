#pragma once
#include "render_system.hpp"


#include <filesystem>
#include <memory>

namespace neuron {
    class ShaderModule {
      public:
        ShaderModule(const std::shared_ptr<RenderSystem> &renderSystem, const std::filesystem::path &path);
        ShaderModule(const std::shared_ptr<RenderSystem> &renderSystem, const std::vector<uint32_t> &code);

        inline const vk::raii::ShaderModule &handle() const { return m_ShaderModule; };

      private:
        vk::raii::ShaderModule m_ShaderModule;
    };

    struct VertexInputLayout {
        std::vector<vk::VertexInputBindingDescription>   bindings;
        std::vector<vk::VertexInputAttributeDescription> attributes;
    };

    struct InputAssemblerState {
        vk::PrimitiveTopology topology              = vk::PrimitiveTopology::eTriangleList;
        bool                  allowPrimitiveRestart = false;
    };

    struct RasterizerDepthBias {
        float constantFactor = 0.0f;
        float slopeFactor    = 0.0f;
        float clamp          = 0.0f;
    };

    struct RasterizerState {
        vk::PolygonMode                    polygonMode   = vk::PolygonMode::eFill;
        vk::CullModeFlags                  cullMode      = vk::CullModeFlagBits::eBack;
        vk::FrontFace                      frontFace     = vk::FrontFace::eClockwise;
        float                              lineWidth     = 1.0f;
        bool                               clampDepth    = false;
        bool                               discardOutput = false;
        std::optional<RasterizerDepthBias> depthBias     = std::nullopt;
    };

    struct TessellatorState {
        uint32_t patchControlPoints = 1;
    };

    struct RenderingCompatibility {
        std::vector<vk::Format> colorFormats;
        vk::Format              depthFormat   = vk::Format::eUndefined;
        vk::Format              stencilFormat = vk::Format::eUndefined;
    };

    struct MultisamplingState {
        vk::SampleCountFlagBits     rasterizationSamples  = vk::SampleCountFlagBits::e1;
        float                       minSampleShading      = 0.0f;
        bool                        enableSampleShading   = false;
        std::vector<vk::SampleMask> sampleMask            = {};
        bool                        enableAlphaToOne      = false;
        bool                        enableAlphaToCoverage = false;
    };

    struct ColorBlendingState {
        std::vector<vk::PipelineColorBlendAttachmentState> attachments;

        std::array<float, 4>       blendConstants = {0.0f, 0.0f, 0.0f, 0.0f};
        std::optional<vk::LogicOp> logicOp        = std::nullopt;
    };

    class GraphicsPipeline {
      public:
        struct Description {
            std::vector<std::tuple<const ShaderModule *, vk::ShaderStageFlagBits, std::string>> shaderStages;
            RenderingCompatibility                                                              renderingCompatibility;

            std::vector<std::pair<vk::Viewport, vk::Rect2D>> viewports;
            std::vector<vk::DynamicState>                    dynamicStates;

            VertexInputLayout   vertexInputLayout{};
            InputAssemblerState inputAssembler{};
            RasterizerState     rasterizerState{};
            TessellatorState    tessellatorState{};
            MultisamplingState  multisamplingState{};
            ColorBlendingState  colorBlendingState{};
        };

        GraphicsPipeline(const std::shared_ptr<RenderSystem> &renderSystem, const Description &description);

      private:
    };
} // namespace neuron
