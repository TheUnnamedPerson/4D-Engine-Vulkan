module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <cassert>
#include <stdexcept>
#include <memory>
#include <string>

#include <iostream>

module Engine4D.Renderer.Manager;

namespace Engine4D {

    /*struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };*/

    struct UniformBufferObject {
		std::vector<Instruction> instructions;
    };    

	struct Scene {
		std::vector<Instruction> instructions;
	};

	struct PushConstantObject {
		alignas(8) glm::vec2 resolution;
		alignas(4) float time;
        alignas(4) float rot;
		alignas(16) glm::vec4 cameraPosition;
	};

    rManager::rManager() {
        loadModels();
		createDescriptorSetLayout();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    rManager::rManager(void (*_main_Update)(), void (*_main_Late_Update)(), TimeClass* time) {

		main_Update = _main_Update;
		main_Late_Update = _main_Late_Update;
		Time = time;

        loadModels();
        createDescriptorSetLayout();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    rManager::~rManager() { vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr); vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

	std::string vec4ToString(glm::vec4 vec) {
		return std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + " " + std::to_string(vec.w);
	}

	constexpr float RotateMultiplier = -2.0f;

    constexpr float speed = 4.0f;

    constexpr float MoveMultiplierX = 2.0f;
    constexpr float MoveMultiplierY = 2.0f;
    constexpr float MoveMultiplierZ = 2.0f;
    constexpr float MoveMultiplierW = 0.6f;

    void rManager::run() {

		rBuffer globalBuffer(
            device,
            sizeof(UniformBufferObject),
            rSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            device.properties.limits.minUniformBufferOffsetAlignment
        );
		globalBuffer.map();


        float timePassed = 0.0f;
        while (!window.shouldClose()) {

            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastUpdatedTime;

            glfwPollEvents();

            float shiftMult = 1;

            int stateShift = glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_SHIFT);
            if (stateShift == GLFW_PRESS)
				shiftMult += 1.0f;
            int stateControl = glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_CONTROL);
            if (stateControl == GLFW_PRESS)
                shiftMult -= 0.75f;

            int stateE = glfwGetKey(window.getWindow(), GLFW_KEY_E);
            if (stateE == GLFW_PRESS)
                rotation += RotateMultiplier * shiftMult * deltaTime;
            int stateQ = glfwGetKey(window.getWindow(), GLFW_KEY_Q);
            if (stateQ == GLFW_PRESS)
                rotation -= RotateMultiplier * shiftMult * deltaTime;
            rotation = fmod(rotation, 6.28318530718f);

            glm::vec4 nextCamPos = glm::vec4(0,0,0,0);

            int stateW = glfwGetKey(window.getWindow(), GLFW_KEY_W);
            if (stateW == GLFW_PRESS)
                nextCamPos.z += MoveMultiplierZ * shiftMult * speed * deltaTime;
            int stateS = glfwGetKey(window.getWindow(), GLFW_KEY_S);
            if (stateS == GLFW_PRESS)
                nextCamPos.z -= MoveMultiplierZ * shiftMult * speed * deltaTime;

            int stateA = glfwGetKey(window.getWindow(), GLFW_KEY_A);
            if (stateA == GLFW_PRESS)
                nextCamPos.x -= MoveMultiplierX * shiftMult * speed * deltaTime;
            int stateD = glfwGetKey(window.getWindow(), GLFW_KEY_D);
            if (stateD == GLFW_PRESS)
                nextCamPos.x += MoveMultiplierX * shiftMult * speed * deltaTime;

            int stateX = glfwGetKey(window.getWindow(), GLFW_KEY_X);
            if (stateX == GLFW_PRESS)
                nextCamPos.y += MoveMultiplierY * shiftMult * speed * deltaTime;
            int stateZ = glfwGetKey(window.getWindow(), GLFW_KEY_Z);
            if (stateZ == GLFW_PRESS)
                nextCamPos.y -= MoveMultiplierY * shiftMult * speed * deltaTime;

            glm::mat4 camRot = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, -1.0f, 0.0f));

            //std::cout << glm::to_string(camRot) << std::endl;

			nextCamPos = camRot * nextCamPos;

            int stateT = glfwGetKey(window.getWindow(), GLFW_KEY_T);
            if (stateT == GLFW_PRESS)
                nextCamPos.w += MoveMultiplierW * shiftMult * speed * deltaTime;
            int stateG = glfwGetKey(window.getWindow(), GLFW_KEY_G);
            if (stateG == GLFW_PRESS)
                nextCamPos.w -= MoveMultiplierW * shiftMult * speed * deltaTime;

			cameraPosition += nextCamPos;

            //std::cout << vec4ToString(cameraPosition) << " ; " << vec4ToString(nextCamPos) << " ; " << rotation << std::endl;            

            if (currentTime - lastFrameTime >= 1.0 / MAX_FPS) {
                Time->deltaTime = deltaTime;
				main_Update();
                deltaTime = currentTime - lastUpdatedTime;
				Time->deltaTime = deltaTime;
				main_Late_Update();

                

                drawFrame();
                lastFrameTime = currentTime;
            }

            timePassed += deltaTime;

            lastUpdatedTime = currentTime;

        }

        vkDeviceWaitIdle(device.device());
    }

    void rManager::loadModels() {
        rModel::Builder modelBuilder{};
        modelBuilder.vertices = {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}} };		
        model = std::make_unique<rModel>(device, modelBuilder);
    }

    void rManager::createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device.device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Descriptor Set Layout!");
        }
    }

    void rManager::createUniformBuffers() {
        const int MAX_FRAMES_IN_FLIGHT = rSwapChain::MAX_FRAMES_IN_FLIGHT;
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            device.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

            vkMapMemory(device.device(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
        }
    }

    void rManager::createDescriptorPool() {
        const int MAX_FRAMES_IN_FLIGHT = rSwapChain::MAX_FRAMES_IN_FLIGHT;
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Descriptor Pool!");
        }
    }

    void rManager::createDescriptorSets() {
        const int MAX_FRAMES_IN_FLIGHT = rSwapChain::MAX_FRAMES_IN_FLIGHT;;
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(device.device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Allocate Descriptor Sets!");
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = textureImageView;
            imageInfo.sampler = textureSampler;

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(device.device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }



    void rManager::createPipelineLayout() {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantObject);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr; //&descriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Pipeline Layout!");
        }
    }

    void rManager::recreateSwapChain() {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = window.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.device());
		swapChain = nullptr;
        if (swapChain == nullptr) {
            swapChain = std::make_unique<rSwapChain>(device, extent);
        }
        else {
            swapChain = std::make_unique<rSwapChain>(device, extent, std::move(swapChain));
            if (swapChain->imageCount() != commandBuffers.size()) {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        createPipeline();
    }

    void rManager::createPipeline() {
        assert(swapChain != nullptr && "Cannot Create Pipeline before Swap Chain");
        assert(pipelineLayout != nullptr && "Cannot Create Pipeline before Pipeline Layout");

        PipelineConfigInfo pipelineConfig{};
        rPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = swapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<rPipeline>(
            device,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void rManager::createCommandBuffers() {
        commandBuffers.resize(swapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to Allocate Command Buffers!");
        }
    }

    void rManager::freeCommandBuffers() {
        vkFreeCommandBuffers(
            device.device(),
            device.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    void rManager::recordCommandBuffer(int imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Begin Recording Command Buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        pipeline->bind(commandBuffers[imageIndex]);
        model->bind(commandBuffers[imageIndex]);


		PushConstantObject push{};
		push.resolution = glm::vec2(swapChain->getSwapChainExtent().width, swapChain->getSwapChainExtent().height);
		push.time = static_cast<float>(glfwGetTime());
		push.rot = rotation;
		push.cameraPosition = cameraPosition;

		vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantObject), &push);



        model->draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Record Command Buffer!");
        }
    }

    void rManager::drawFrame() {
        uint32_t imageIndex;
        auto result = swapChain->acquireNextImage(&imageIndex);
		frameIndex = imageIndex;

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to Acquire Swap Chain Image!");
        }

        recordCommandBuffer(imageIndex);
        result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            window.wasWindowResized()) {
            window.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to Present Swap Chain image!");
        }
    }

}
