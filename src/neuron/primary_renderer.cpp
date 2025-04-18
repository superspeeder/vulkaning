//
// Created by andy on 4/18/2025.
//

#include "primary_renderer.hpp"

namespace neuron {
    PrimaryRenderer::PrimaryRenderer(std::shared_ptr<RenderSystem> renderSystem) : m_RenderSystem(std::move(renderSystem)), m_CommandBuffers(nullptr) {
        m_CommandBuffers = m_RenderSystem->allocateCommandBuffers(maxFramesInFlight);

        m_InFlightFences.reserve(maxFramesInFlight);
        m_SyncInfos.reserve(maxFramesInFlight);
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            m_InFlightFences.emplace_back(m_RenderSystem->device(), vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
            m_SyncInfos.emplace_back(m_RenderSystem);
        }
    }

    void PrimaryRenderer::renderFrame(RenderTarget *renderTarget, const std::function<void(const CommandRecorder &, const RenderTarget::FrameInfo &)> &f) {
        [[maybe_unused]] auto _ = m_RenderSystem->device().waitForFences(*m_InFlightFences[m_CurrentFrame], true, UINT64_MAX);
        m_RenderSystem->device().resetFences(*m_InFlightFences[m_CurrentFrame]);

        const auto &frameSync = m_SyncInfos[m_CurrentFrame];
        const auto  frameInfo = renderTarget->beginFrame(frameSync);


        const auto &commandBuffer = m_CommandBuffers[m_CurrentFrame];
        {
            const auto             commandRecorder = CommandRecorder(commandBuffer);
            vk::ImageMemoryBarrier imb1{vk::AccessFlagBits::eNone,
                                        vk::AccessFlagBits::eColorAttachmentWrite,
                                        vk::ImageLayout::eUndefined,
                                        vk::ImageLayout::eColorAttachmentOptimal,
                                        0,
                                        0,
                                        frameInfo.image,
                                        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

            vk::ImageMemoryBarrier imb2{vk::AccessFlagBits::eColorAttachmentWrite,
                                        vk::AccessFlagBits::eNone,
                                        vk::ImageLayout::eColorAttachmentOptimal,
                                        vk::ImageLayout::ePresentSrcKHR,
                                        0,
                                        0,
                                        frameInfo.image,
                                        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

            commandRecorder->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput, {}, {}, {}, imb1);
            f(commandRecorder, frameInfo);
            commandRecorder->pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe, {}, {}, {}, imb2);
        }

        vk::SubmitInfo         submitInfo{};
        vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eTopOfPipe;
        submitInfo.setWaitSemaphores(*frameSync.imageAvailable);
        submitInfo.setSignalSemaphores(*frameSync.renderFinished);
        submitInfo.setCommandBuffers(*commandBuffer);
        submitInfo.setWaitDstStageMask(waitStage);
        m_RenderSystem->queues().main.submit(submitInfo, *m_InFlightFences[m_CurrentFrame]);

        renderTarget->endFrame(frameSync, frameInfo);

        m_CurrentFrame = (m_CurrentFrame + 1) % maxFramesInFlight;
    }
} // namespace neuron
