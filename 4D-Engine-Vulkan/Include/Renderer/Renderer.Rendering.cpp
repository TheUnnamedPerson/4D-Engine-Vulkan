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

module Engine4D.Renderer.Rendering;

namespace Engine4D {

    rRendering::rRendering(rWindow& window, rDevice& device) : window{ window }, device{ device } {
        recreateSwapChain();
        createCommandBuffers();
    }

    rRendering::~rRendering() { freeCommandBuffers(); }
    
    void rRendering::recreateSwapChain() {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            if (glfwWindowShouldClose(window.getWindow()))
                return;
            extent = window.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.device());
        swapChain = nullptr;
        if (swapChain == nullptr) {
            swapChain = std::make_unique<rSwapChain>(device, extent);
        }
        else {
			std::shared_ptr<rSwapChain> oldSwapChain = std::move(swapChain);
            swapChain = std::make_unique<rSwapChain>(device, extent, std::move(oldSwapChain));

			if (!oldSwapChain->compareSwapFormats(*swapChain.get())) {
				throw std::runtime_error("Swap Chain Image/Vulkan Format Mismatch!");
			}

        }

    }

    void rRendering::createCommandBuffers() {
        commandBuffers.resize(rSwapChain::MAX_FRAMES_IN_FLIGHT);

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

    void rRendering::freeCommandBuffers() {
        vkFreeCommandBuffers(
            device.device(),
            device.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer rRendering::beginFrame()
    {
        assert(!isFrameStarted && "Can't Start New Frame While Previous Frame is in Progress!");
        auto result = swapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to Acquire Swap Chain Image!");
        }

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Begin Recording Command Buffer!");
        }

        return commandBuffer;
    }

    void rRendering::endFrame()
    {
		assert(isFrameStarted && "Can't End Frame While Frame is Not in Progress!");
		auto commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Record Command Buffer!");
        }

        auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            window.wasWindowResized()) {
            window.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to Present Swap Chain image!");
        }

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % rSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void rRendering::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
		assert(isFrameStarted && "Can't Begin Render Pass While Frame is Not in Progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't Begin Render Pass on Command Buffer Outside of Frame!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }
    void rRendering::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
		assert(isFrameStarted && "Can't End Render Pass While Frame is Not in Progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't End Render Pass on Command Buffer Outside of Frame!");

        vkCmdEndRenderPass(commandBuffer);
    }

}
