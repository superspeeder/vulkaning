//
// Created by andy on 4/17/2025.
//

#pragma once
#include "neuron/app.hpp"
#include "neuron/dynamic_rendering_pass.hpp"
#include "neuron/primary_renderer.hpp"

namespace game {
    class Game final : public neuron::App {
      public:
        Game();

        inline void setup() override;
        inline void update(double dt) override;
        inline void render(double dt) override;
        inline void post_loop() override;

      private:
        std::unique_ptr<neuron::PrimaryRenderer> m_PrimaryRenderer;
        neuron::DynamicRenderingPass             m_RenderingPass;
    };
} // namespace game
