//
// Created by andy on 4/17/2025.
//

#pragma once
#include "render_system.hpp"


#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace neuron {

    struct FrameSyncInfo {
        vk::raii::Semaphore imageAvailable, renderFinished;

        explicit FrameSyncInfo(const vk::raii::Device &device);
        explicit FrameSyncInfo(const std::shared_ptr<RenderSystem> &renderSystem);

        FrameSyncInfo(const FrameSyncInfo &other)                = delete;
        FrameSyncInfo(FrameSyncInfo &&other) noexcept            = default;
        FrameSyncInfo &operator=(const FrameSyncInfo &other)     = delete;
        FrameSyncInfo &operator=(FrameSyncInfo &&other) noexcept = default;
    };

    class RenderTarget {
      public:
        virtual ~RenderTarget() = default;

        struct FrameInfo {
            vk::Extent2D  extent;
            vk::Format    format;
            vk::Image     image;
            vk::ImageView imageView;
            uint32_t      imageIndex;
        };

        virtual FrameInfo beginFrame(const FrameSyncInfo &frameSyncInfo)                    = 0;
        virtual void      endFrame(const FrameSyncInfo &frameSyncInfo, FrameInfo frameInfo) = 0;
    };

} // namespace neuron
