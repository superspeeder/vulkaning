//
// Created by andy on 4/17/2025.
//

#include "dynamic_rendering_pass.hpp"

namespace neuron {

    DynamicRenderingPass::DynamicRenderingPass(const std::vector<std::optional<vk::ClearColorValue>> &colorClearValues, vk::Extent2D extent)
        : m_ColorClearValues(colorClearValues), m_RenderArea({0, 0}, extent) {}

    void DynamicRenderingPass::setClearColor(const uint32_t attachment, std::optional<vk::ClearColorValue> colorValue) {
        m_ColorClearValues[attachment] = std::move(colorValue);
    }

    void DynamicRenderingPass::setRenderArea(const vk::Rect2D &renderArea) {
        m_RenderArea = renderArea;
    }

    DynamicRenderingPassRecorder::DynamicRenderingPassRecorder(const CommandRecorder &recorder, const DynamicRenderingPass &renderingPass,
                                                               const std::vector<vk::ImageView> &attachments)
        : recorder(recorder) {

        std::vector<vk::RenderingAttachmentInfo> attachmentInfo;
        uint32_t                                 i = 0;
        for (const auto &attachment : attachments) {
            auto           loadOp = vk::AttachmentLoadOp::eLoad;
            vk::ClearValue clearValue{};
            if (renderingPass.colorClearValues()[i].has_value()) {
                clearValue.color = renderingPass.colorClearValues()[i].value();
                loadOp           = vk::AttachmentLoadOp::eClear;
            }

            attachmentInfo.emplace_back(attachment, vk::ImageLayout::eColorAttachmentOptimal, vk::ResolveModeFlagBits::eNone, nullptr, vk::ImageLayout::eUndefined, loadOp,
                                        vk::AttachmentStoreOp::eStore, clearValue);

            ++i;
        }

        vk::RenderingInfo renderingInfo{};
        renderingInfo.setColorAttachments(attachmentInfo).setViewMask(0).setRenderArea(renderingPass.renderArea()).setLayerCount(1);

        recorder->beginRendering(renderingInfo);
    }

    DynamicRenderingPassRecorder::~DynamicRenderingPassRecorder() {
        recorder->endRendering();
    }

    void DynamicRenderingPassRecorder::bindGraphicsPipeline(const vk::raii::Pipeline &pipeline) const {
        recorder->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
    }

    void DynamicRenderingPassRecorder::draw(const uint32_t vertexCount, const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance) const {
        recorder->draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }
} // namespace neuron
