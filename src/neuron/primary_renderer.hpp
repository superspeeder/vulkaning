//
// Created by andy on 4/18/2025.
//

#pragma once

#include "command_recorder.hpp"
#include "render_system.hpp"
#include "render_target.hpp"

#include <functional>

#include <cstdlib>

namespace neuron {

    class PrimaryRenderer {
      public:
        static constexpr std::size_t maxFramesInFlight = 2;

        explicit PrimaryRenderer(std::shared_ptr<RenderSystem> renderSystem);
        ~PrimaryRenderer() = default;

        void renderFrame(RenderTarget *renderTarget, const std::function<void(const CommandRecorder &, const RenderTarget::FrameInfo &)> &f);

        inline void renderFrame(RenderTarget &renderTarget, const std::function<void(const CommandRecorder &, const RenderTarget::FrameInfo &)> &f) {
            renderFrame(&renderTarget, f);
        }

        inline void renderFrame(const std::shared_ptr<RenderTarget> &renderTarget, const std::function<void(const CommandRecorder &, const RenderTarget::FrameInfo &)> &f) {
            renderFrame(renderTarget.get(), f);
        }

        inline void renderFrame(const std::unique_ptr<RenderTarget> &renderTarget, const std::function<void(const CommandRecorder &, const RenderTarget::FrameInfo &)> &f) {
            renderFrame(renderTarget.get(), f);
        }

      private:
        std::shared_ptr<RenderSystem> m_RenderSystem;
        vk::raii::CommandBuffers      m_CommandBuffers;
        std::vector<vk::raii::Fence>  m_InFlightFences;
        std::vector<FrameSyncInfo>    m_SyncInfos;
        uint32_t                      m_CurrentFrame = 0;
    };

} // namespace neuron
