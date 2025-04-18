//
// Created by andy on 4/17/2025.
//

#include "render_system.hpp"

#include "GLFW/glfw3.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

constexpr const char *DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

namespace neuron {
    RenderSystem::RenderSystem() : m_Instance(nullptr), m_PhysicalDevice(nullptr), m_Device(nullptr), m_Queues(nullptr, nullptr, nullptr), m_MainPool(nullptr) {
        constexpr vk::ApplicationInfo appInfo = {"App", vk::makeApiVersion(0, 1, 0, 0), "Neuron Engine", vk::makeApiVersion(0, 0, 1, 0), vk::ApiVersion14};
        uint32_t                      requiredExtensionsCount;
        const char                  **requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

        m_Instance       = m_Context.createInstance(vk::InstanceCreateInfo({}, &appInfo, 0, nullptr, requiredExtensionsCount, requiredExtensions));
        m_PhysicalDevice = m_Instance.enumeratePhysicalDevices()[0];

        const auto              queueFamilyProperties = m_PhysicalDevice.getQueueFamilyProperties();
        std::optional<uint32_t> mainFamily, presentFamily, transferFamily;

        for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i) {
            const auto &props = queueFamilyProperties[i];
            if (!mainFamily.has_value() && props.queueFlags & vk::QueueFlagBits::eGraphics) {
                mainFamily = i;
                if (glfwGetPhysicalDevicePresentationSupport(*m_Instance, *m_PhysicalDevice, i)) {
                    presentFamily = i;
                }
            } else if (!presentFamily.has_value() && glfwGetPhysicalDevicePresentationSupport(*m_Instance, *m_PhysicalDevice, i)) {
                presentFamily = i;
            }

            if (!transferFamily.has_value() && props.queueFlags & vk::QueueFlagBits::eTransfer &&
                !(props.queueFlags & vk::QueueFlagBits::eGraphics || props.queueFlags & vk::QueueFlagBits::eCompute)) {
                transferFamily = i;
            }
        }

        if (!mainFamily.has_value()) {
            throw std::runtime_error("GPU doesn't support graphics???");
        }

        if (!presentFamily.has_value()) {
            throw std::runtime_error("GPU doesn't support presentation to this OS");
        }

        m_QueueFamilies = {mainFamily.value(), presentFamily.value(), transferFamily.value_or(mainFamily.value())};

        constexpr std::array<float, 1>         queuePriorities  = {1.0f};
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {vk::DeviceQueueCreateInfo({}, m_QueueFamilies.main, queuePriorities)};

        if (m_QueueFamilies.main != m_QueueFamilies.present) {
            queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags{}, m_QueueFamilies.present, queuePriorities);
        }

        if (m_QueueFamilies.main != m_QueueFamilies.transfer && m_QueueFamilies.transfer != m_QueueFamilies.present) {
            queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags{}, m_QueueFamilies.transfer, queuePriorities);
        }

        vk::PhysicalDeviceFeatures2 f2{};
        f2.features.geometryShader            = true;
        f2.features.tessellationShader        = true;
        f2.features.wideLines                 = true;
        f2.features.largePoints               = true;
        f2.features.drawIndirectFirstInstance = true;
        f2.features.multiDrawIndirect         = true;
        f2.features.fillModeNonSolid          = true;
        f2.features.fragmentStoresAndAtomics  = true;

        vk::PhysicalDeviceVulkan11Features v11f{};
        v11f.shaderDrawParameters = true;

        vk::PhysicalDeviceVulkan12Features v12f{};
        v12f.timelineSemaphore   = true;
        v12f.bufferDeviceAddress = true;
        v12f.drawIndirectCount   = true;

        vk::PhysicalDeviceVulkan13Features v13f{};
        v13f.synchronization2        = true;
        v13f.dynamicRendering        = true;
        v13f.inlineUniformBlock      = true;
        v13f.shaderIntegerDotProduct = true;

        vk::PhysicalDeviceVulkan14Features v14f{};

        f2.pNext   = &v11f;
        v11f.pNext = &v12f;
        v12f.pNext = &v13f;
        v13f.pNext = &v14f;

        m_Device = vk::raii::Device(m_PhysicalDevice, vk::DeviceCreateInfo({}, queueCreateInfos, {}, DEVICE_EXTENSIONS, nullptr, &f2));
        m_Queues = {m_Device.getQueue(m_QueueFamilies.main, 0), m_Device.getQueue(m_QueueFamilies.present, 0), m_Device.getQueue(m_QueueFamilies.transfer, 0)};

        m_MainPool = vk::raii::CommandPool(m_Device, vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, m_QueueFamilies.main));
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::waitIdle() const {
        m_Device.waitIdle();
    }

    vk::raii::CommandBuffers RenderSystem::allocateCommandBuffers(const uint32_t count, const vk::CommandBufferLevel level) const {
        return vk::raii::CommandBuffers(m_Device, vk::CommandBufferAllocateInfo(m_MainPool, level, count));
    }
} // namespace neuron
