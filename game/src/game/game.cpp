//
// Created by andy on 4/17/2025.
//

#include "game.hpp"

namespace game {
    Game::Game() : m_RenderingPass({vk::ClearColorValue(.25f, 0.5f, 0.5f, 1.0f)}, {}) {}

    void Game::setup() {
        m_RenderingPass.setRenderArea(vk::Rect2D({0, 0}, m_Window->getSwapchainExtent()));
        m_PrimaryRenderer = std::make_unique<neuron::PrimaryRenderer>(m_RenderSystem);
    }

    void Game::update(double dt) {}

    void Game::render(double dt) {
        m_PrimaryRenderer->renderFrame(m_Window, [&](const neuron::CommandRecorder &cmd, const neuron::RenderTarget::FrameInfo &frameInfo) {
            {
                neuron::DynamicRenderingPassRecorder pass(cmd, m_RenderingPass, {frameInfo.imageView});
            }
        });
    }

    void Game::post_loop() {}
} // namespace game

int main() {
    game::Game game;
    game.mainloop();
    return 0;
}
