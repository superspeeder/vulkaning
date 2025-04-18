//
// Created by andy on 4/17/2025.
//

#include "command_recorder.hpp"

namespace neuron {
    CommandRecorder::CommandRecorder(const vk::raii::CommandBuffer &commandBuffer, const vk::CommandBufferBeginInfo &beginInfo) : m_CommandBuffer(commandBuffer) {
        commandBuffer.reset({});
        commandBuffer.begin(beginInfo);
    }

    CommandRecorder::~CommandRecorder() {
        m_CommandBuffer.end();
    }
} // neuron