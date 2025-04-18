//
// Created by andy on 4/17/2025.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace neuron {

    struct QueueFamilies {
        uint32_t main, present, transfer;
    };

    struct Queues {
        vk::raii::Queue main, present, transfer;
    };

    // shared so that objects can hold references easily
    class RenderSystem : public std::enable_shared_from_this<RenderSystem> {
        RenderSystem();

      public:
        inline static std::shared_ptr<RenderSystem> create() { return std::shared_ptr<RenderSystem>(new RenderSystem()); }

        ~RenderSystem();

        RenderSystem(const RenderSystem &other)                = delete;
        RenderSystem(RenderSystem &&other) noexcept            = default;
        RenderSystem &operator=(const RenderSystem &other)     = delete;
        RenderSystem &operator=(RenderSystem &&other) noexcept = default;

        void waitIdle() const;

        [[nodiscard]] const vk::raii::Context &context() const { return m_Context; }

        [[nodiscard]] const vk::raii::Instance &instance() const { return m_Instance; }

        [[nodiscard]] const vk::raii::PhysicalDevice &physicalDevice() const { return m_PhysicalDevice; }

        [[nodiscard]] const vk::raii::Device &device() const { return m_Device; }

        [[nodiscard]] const QueueFamilies &queueFamilies() const { return m_QueueFamilies; }

        [[nodiscard]] const Queues &queues() const { return m_Queues; }

        [[nodiscard]] const vk::raii::CommandPool &mainPool() const { return m_MainPool; }

        [[nodiscard]] vk::raii::CommandBuffers allocateCommandBuffers(uint32_t count, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;
      private:
        vk::raii::Context        m_Context;
        vk::raii::Instance       m_Instance;
        vk::raii::PhysicalDevice m_PhysicalDevice;
        vk::raii::Device         m_Device;
        QueueFamilies            m_QueueFamilies;
        Queues                   m_Queues;
        vk::raii::CommandPool    m_MainPool;
    };

} // namespace neuron
