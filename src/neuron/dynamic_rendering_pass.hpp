//
// Created by andy on 4/17/2025.
//

#pragma once
#include "command_recorder.hpp"

namespace neuron {

    class DynamicRenderingPass {
      public:
        DynamicRenderingPass(const std::vector<std::optional<vk::ClearColorValue>> &colorClearValues, vk::Extent2D extent);

        void setClearColor(uint32_t attachment, std::optional<vk::ClearColorValue> colorValue);
        void setRenderArea(const vk::Rect2D &renderArea);

        [[nodiscard]] const std::vector<std::optional<vk::ClearColorValue>> &colorClearValues() const { return m_ColorClearValues; }

        [[nodiscard]] const vk::Rect2D &renderArea() const { return m_RenderArea; }

      private:
        std::vector<std::optional<vk::ClearColorValue>> m_ColorClearValues;
        vk::Rect2D                                      m_RenderArea;
    };

    class DynamicRenderingPassRecorder final : public ICommandRecorder {
      public:
        explicit DynamicRenderingPassRecorder(const CommandRecorder &recorder, const DynamicRenderingPass &renderingPass, const std::vector<vk::ImageView> &attachments);

        ~DynamicRenderingPassRecorder() override;

        inline const vk::raii::CommandBuffer &commandBuffer() const override { return recorder.commandBuffer(); }

        void bindGraphicsPipeline(const vk::raii::Pipeline &pipeline) const;
        void draw(uint32_t firstVertex, uint32_t firstInstance, uint32_t vertexCount, uint32_t instanceCount) const;

      private:
        const CommandRecorder &recorder;
    };

} // namespace neuron
