module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cassert>
#include <stdexcept>
#include <memory>
#include <vector>

#include <iostream>

module Engine4D.Renderer.System;

namespace Engine4D {

    struct PushConstantObject {
        alignas(8) glm::vec2 resolution;
        alignas(4) float time;
        alignas(4) float rot;
        alignas(16) glm::vec4 cameraPosition;
        alignas(4) int instructionsCount;
    };

    rSystem::rSystem(rDevice& device, std::unique_ptr<rModel>& model, rRendering& renderer, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout) : device{ device }, model{ model }, renderer{ renderer } {
        createPipelineLayout(descriptorSetLayout);
        createPipeline(renderPass);
    }

    rSystem::~rSystem() { vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

    void rSystem::createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout) {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantObject);

		std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
        pipelineLayoutInfo.pSetLayouts = layouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Pipeline Layout!");
        }
    }

    void rSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot Create Pipeline before Pipeline Layout");

        PipelineConfigInfo pipelineConfig{};
        rPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<rPipeline>(
            device,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void rSystem::renderObjects(FrameInfo& frameInfo) {

        pipeline->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&frameInfo.globalDescriptorSet,
			0,
			nullptr);

        model->bind(frameInfo.commandBuffer);

        PushConstantObject push{};
        push.resolution = glm::vec2(renderer.getSwapChainExtent().width, renderer.getSwapChainExtent().height);
        push.time = static_cast<float>(glfwGetTime());
        push.rot = frameInfo.cameraInfo.rotation;
        push.cameraPosition = frameInfo.cameraInfo.cameraPosition;
        push.instructionsCount = frameInfo.instructionsCount;

        vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantObject), &push);

        model->draw(frameInfo.commandBuffer);
    }

}
