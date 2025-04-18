//
// Created by andy on 4/17/2025.
//

#include "app.hpp"

#include "GLFW/glfw3.h"

#include <chrono>

namespace neuron {
    App::App(const AppConfiguration &config) : m_Config(config) {
        glfwInit();
        VULKAN_HPP_DEFAULT_DISPATCHER.init();
    }

    App::~App() {}

    void App::mainloop() {
        internalSetup();
        setup();
        internalPostSetup();

        using clock      = std::chrono::high_resolution_clock;
        using duration   = std::chrono::duration<double>;
        using time_point = std::chrono::time_point<clock, duration>;

        time_point thisFrame = clock::now();
        duration   deltaTime(1.0 / 60.0);
        time_point lastFrame = thisFrame - deltaTime;

        while (!m_Window->shouldClose()) {
            internalPreUpdate(deltaTime.count());
            update(deltaTime.count());
            internalPostUpdate(deltaTime.count());
            internalPreRender(deltaTime.count());
            render(deltaTime.count());
            internalPostRender(deltaTime.count());

            lastFrame = thisFrame;
            thisFrame = clock::now();
            deltaTime = thisFrame - lastFrame;
        }
    }

    void App::internalSetup() {
        m_RenderSystem = RenderSystem::create();
        m_Window       = Window::create(m_Config.windowTitle, m_Config.windowSize.width, m_Config.windowSize.height, m_RenderSystem);
    }

    // ReSharper disable CppMemberFunctionMayBeConst
    void App::internalPostSetup() {}

    void App::internalPreUpdate(double dt) {
        glfwPollEvents();
    }

    void App::internalPostUpdate(double dt) {}

    void App::internalPreRender(double dt) {}

    void App::internalPostRender(double dt) {}

    void App::internalPostLoop() {
        m_RenderSystem->waitIdle();
    }

    void App::internalFinal() {}

    // ReSharper restore CppMemberFunctionMayBeConst
} // namespace neuron
