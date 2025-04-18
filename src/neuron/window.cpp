//
// Created by andy on 4/17/2025.
//
#include "window.hpp"

namespace neuron {
    Window::Window(const std::string_view title, const uint32_t width, const uint32_t height, std::shared_ptr<RenderSystem> renderSystem)
        : m_RenderSystem(std::move(renderSystem)), m_Surface(nullptr), m_Swapchain(nullptr) {

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_Window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

        VkSurfaceKHR surf;
        if (glfwCreateWindowSurface(*m_RenderSystem->instance(), m_Window, nullptr, &surf) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }

        m_Surface = vk::raii::SurfaceKHR(m_RenderSystem->instance(), surf);

        const auto capabilities = m_RenderSystem->physicalDevice().getSurfaceCapabilitiesKHR(*m_Surface);
        const auto presentModes = m_RenderSystem->physicalDevice().getSurfacePresentModesKHR(*m_Surface);
        const auto formats      = m_RenderSystem->physicalDevice().getSurfaceFormatsKHR(*m_Surface);

        vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
        for (const auto mode : presentModes) {
            if (mode == vk::PresentModeKHR::eMailbox) {
                presentMode = vk::PresentModeKHR::eMailbox;
            }
        }

        m_SurfaceFormat = formats[0];
        for (const auto &format : formats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                m_SurfaceFormat = format;
            }
        }

        m_Extent = capabilities.currentExtent;
        if (m_Extent.height == UINT32_MAX) {
            int w, h;
            glfwGetFramebufferSize(m_Window, &w, &h);
            m_Extent.width  = std::clamp<uint32_t>(w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            m_Extent.height = std::clamp<uint32_t>(h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        }

        int imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }

        m_Swapchain = vk::raii::SwapchainKHR(m_RenderSystem->device(),
                                             vk::SwapchainCreateInfoKHR({}, *m_Surface, imageCount, m_SurfaceFormat.format, m_SurfaceFormat.colorSpace, m_Extent, 1,
                                                                        vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive, {}, capabilities.currentTransform,
                                                                        vk::CompositeAlphaFlagBitsKHR::eOpaque, presentMode, true));

        m_Images = m_Swapchain.getImages();

        for (const auto &image : m_Images) {
            m_ImageViews.emplace_back(
                m_RenderSystem->device(),
                vk::ImageViewCreateInfo({}, image, vk::ImageViewType::e2D, m_SurfaceFormat.format,
                                        vk::ComponentMapping{vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA},
                                        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}));
        }
    }

    Window::~Window() {
        m_Surface.clear();
        glfwDestroyWindow(m_Window);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    RenderTarget::FrameInfo Window::beginFrame(const FrameSyncInfo &frameSyncInfo) {
        const auto imageIndex = m_Swapchain.acquireNextImage(UINT64_MAX, *frameSyncInfo.imageAvailable, nullptr).second;
        return FrameInfo(m_Extent, m_SurfaceFormat.format, m_Images[imageIndex], *m_ImageViews[imageIndex], imageIndex);
    }

    void Window::endFrame(const FrameSyncInfo &frameSyncInfo, FrameInfo frameInfo) {
        vk::PresentInfoKHR presentInfo = {};
        presentInfo.setSwapchains(*m_Swapchain);
        presentInfo.setImageIndices(frameInfo.imageIndex);
        presentInfo.setWaitSemaphores(*frameSyncInfo.renderFinished);
        auto _ = m_RenderSystem->queues().present.presentKHR(presentInfo);
    }

    vk::Format Window::getSwapchainFormat() const {
        return m_SurfaceFormat.format;
    }

    vk::Extent2D Window::getSwapchainExtent() const {
        return m_Extent;
    }
} // namespace neuron
