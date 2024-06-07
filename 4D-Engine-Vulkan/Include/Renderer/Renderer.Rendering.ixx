module;

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <cassert>

export module Engine4D.Renderer.Rendering;


import Engine4D.Renderer.SwapChain;
import Engine4D.Renderer.Device;
import Engine4D.Renderer.Window;

import Engine4D.Structs;

import Engine4D.Time;

namespace Engine4D {
	export class rRendering {
		public:

		rRendering() = delete;
		rRendering(rWindow& window, rDevice& device);
		~rRendering();

		rRendering(const rRendering&) = delete;
		rRendering& operator=(const rRendering&) = delete;

		VkCommandBuffer beginFrame();
		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
		VkExtent2D getSwapChainExtent() const { return swapChain->getSwapChainExtent(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		int getFrameIndex() const {
			assert(isFrameStarted && "Can't Get Frame Index While Frame is Not in Progress!");
			return currentFrameIndex;
		}

		int forceGetFrameIndex() const {
			return currentFrameIndex;
		}

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Can't Get Command Buffer While Frame is in Progress!");
			return commandBuffers[currentFrameIndex];
		}

		private:

		uint32_t currentImageIndex{0};
		int currentFrameIndex{ 0 };
		bool isFrameStarted{false};

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		rWindow& window;
		rDevice& device;
		std::unique_ptr<rSwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}
