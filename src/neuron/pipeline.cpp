#include "pipeline.hpp"

#include <fstream>
#include <stdexcept>

namespace neuron {
    ShaderModule::ShaderModule(const std::shared_ptr<RenderSystem> &renderSystem, const std::filesystem::path &path) : m_ShaderModule(nullptr) {
        std::ifstream file(path, std::ios::binary | std::ios::in | std::ios::ate);
        if (!file.is_open()) {
            throw std::invalid_argument("Shader file couldn't be opened");
        }

        const auto end = file.tellg();
        file.seekg(0, std::ios::beg);
        auto code = std::vector<uint32_t>(end / sizeof(uint32_t), 0);
        file.read(reinterpret_cast<char *>(code.data()), code.size() * sizeof(uint32_t));
        file.close();

        vk::ShaderModuleCreateInfo createInfo{};
        createInfo.setCode(code);
        m_ShaderModule = vk::raii::ShaderModule(renderSystem->device(), createInfo);
    }

    ShaderModule::ShaderModule(const std::shared_ptr<RenderSystem> &renderSystem, const std::vector<uint32_t> &code) : m_ShaderModule(renderSystem->device(), {{}, code}) {}

    GraphicsPipeline::GraphicsPipeline(const std::shared_ptr<RenderSystem> &renderSystem, const Description &description) {
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        for (const auto &[module, stage, entry] : description.shaderStages) {
            shaderStages.emplace_back({}, stage, module->handle(), entry.c_str());
        }

        vk::PipelineVertexInputStateCreateInfo vertexInputState{};
        vertexInputState.setVertexBindingDescriptions(description.vertexInputLayout.bindings).setVertexAttributeDescriptions(description.vertexInputLayout.attributes);

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.setTopology(description.inputAssembler.topology);
        inputAssembly.setPrimitiveRestartEnable(description.inputAssembler.allowPrimitiveRestart);

        vk::PipelineViewportStateCreateInfo viewportState{};
        std::vector<vk::Viewport> viewports;
        std::vector<vk::Rect2D> scissors;
        for (const auto& [viewport, scissor] : description.viewports) {
            viewports.emplace_back(viewport);
            scissors.emplace_back(scissor);
        }
        viewportState.setViewports(viewports);
        viewportState.setScissors(scissors);

        vk::PipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.setRasterizerDiscardEnable(description.rasterizerState.discardOutput);
        rasterizer.setPolygonMode(description.rasterizerState.polygonMode);
        rasterizer.setCullMode(description.rasterizerState.cullMode);
        rasterizer.setFrontFace(description.rasterizerState.frontFace);
        rasterizer.setDepthClampEnable(description.rasterizerState.clampDepth);
        
    }
} // namespace neuron
