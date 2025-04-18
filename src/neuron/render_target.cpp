//
// Created by andy on 4/17/2025.
//

#include "render_target.hpp"

namespace neuron {
    FrameSyncInfo::FrameSyncInfo(const vk::raii::Device &device) : imageAvailable(device, vk::SemaphoreCreateInfo()), renderFinished(device, vk::SemaphoreCreateInfo()) {}

    FrameSyncInfo::FrameSyncInfo(const std::shared_ptr<RenderSystem> &renderSystem) : imageAvailable(renderSystem->device(), vk::SemaphoreCreateInfo()), renderFinished(renderSystem->device(), vk::SemaphoreCreateInfo()) {}
} // neuron