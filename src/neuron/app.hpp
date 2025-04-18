//
// Created by andy on 4/17/2025.
//

#pragma once

#include "render_system.hpp"
#include "window.hpp"


#include <vulkan/vulkan.hpp>

namespace neuron {

    struct AppConfiguration {
        std::string  windowTitle{"App"};
        vk::Extent2D windowSize{800, 600};
    };

    class App {
      public:
        explicit App(const AppConfiguration &config = {});
        virtual ~App();

        App(const App &other)                = delete;
        App(App &&other) noexcept            = default;
        App &operator=(const App &other)     = delete;
        App &operator=(App &&other) noexcept = default;

        inline virtual void setup()           = 0;
        inline virtual void update(double dt) = 0;
        inline virtual void render(double dt) = 0;
        inline virtual void post_loop()       = 0;

        void mainloop();

      protected:
        std::shared_ptr<RenderSystem> m_RenderSystem;
        std::shared_ptr<Window>       m_Window;

      private:
        AppConfiguration m_Config;

        void internalSetup();
        void internalPostSetup();
        void internalPreUpdate(double dt);
        void internalPostUpdate(double dt);
        void internalPreRender(double dt);
        void internalPostRender(double dt);
        void internalPostLoop();
        void internalFinal();
    };

} // namespace neuron
