//
// Created by andy on 4/17/2025.
//

#pragma once
#include <vulkan/vulkan_raii.hpp>

namespace neuron {
    class ICommandRecorder {
      public:
        ICommandRecorder() = default;
        virtual ~ICommandRecorder() = default;

        ICommandRecorder(const ICommandRecorder &other)                = delete;
        ICommandRecorder(ICommandRecorder &&other) noexcept            = delete;
        ICommandRecorder &operator=(const ICommandRecorder &other)     = delete;
        ICommandRecorder &operator=(ICommandRecorder &&other) noexcept = delete;

        virtual const vk::raii::CommandBuffer &commandBuffer() const = 0;

        inline const vk::raii::CommandBuffer *operator->() const { return &commandBuffer(); }

        inline const vk::raii::CommandBuffer &operator*() const { return commandBuffer(); }
    };

    class CommandRecorder : public ICommandRecorder {
      public:
        explicit CommandRecorder(const vk::raii::CommandBuffer &commandBuffer, const vk::CommandBufferBeginInfo &beginInfo = {});
        ~CommandRecorder() override;

        inline const vk::raii::CommandBuffer &commandBuffer() const override { return m_CommandBuffer; }

      private:
        const vk::raii::CommandBuffer &m_CommandBuffer;
    };

} // namespace neuron
