//
// Created by andy on 4/17/2025.
//

#pragma once
#include "render_system.hpp"
#include "render_target.hpp"

#include "GLFW/glfw3.h"
#include <string_view>

namespace neuron {

    class Window final : public std::enable_shared_from_this<Window>, public RenderTarget {
        Window(std::string_view title, uint32_t width, uint32_t height, std::shared_ptr<RenderSystem> renderSystem);

      public:
        inline static std::shared_ptr<Window> create(const std::string_view title, const uint32_t width, const uint32_t height, const std::shared_ptr<RenderSystem> &renderSystem) {
            return std::shared_ptr<Window>(new Window(title, width, height, renderSystem));
        }

        ~Window() override;

        Window(const Window &other)                = delete;
        Window(Window &&other) noexcept            = default;
        Window &operator=(const Window &other)     = delete;
        Window &operator=(Window &&other) noexcept = default;

        bool shouldClose() const;

        FrameInfo beginFrame(const FrameSyncInfo &frameSyncInfo) override;
        void      endFrame(const FrameSyncInfo &frameSyncInfo, FrameInfo frameInfo) override;

        vk::Format   getSwapchainFormat() const;
        vk::Extent2D getSwapchainExtent() const;

      private:
        GLFWwindow                      *m_Window;
        std::shared_ptr<RenderSystem>    m_RenderSystem;
        vk::raii::SurfaceKHR             m_Surface;
        vk::raii::SwapchainKHR           m_Swapchain;
        std::vector<vk::Image>           m_Images;
        std::vector<vk::raii::ImageView> m_ImageViews;
        vk::SurfaceFormatKHR             m_SurfaceFormat;
        vk::Extent2D                     m_Extent;
    };

} // namespace neuron
