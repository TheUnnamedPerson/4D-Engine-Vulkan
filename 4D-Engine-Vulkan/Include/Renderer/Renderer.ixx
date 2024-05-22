module;

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

export module Engine4D.Renderer;


import Engine4D.Renderer.SwapChain;
import Engine4D.Renderer.Device;
import Engine4D.Renderer.Model;
import Engine4D.Renderer.Pipeline;
import Engine4D.Renderer.Window;

namespace Engine4D {
	export class rRenderer {
		public:

		struct UniformBufferObject {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		};

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		static constexpr int MAX_FPS = 30;

		double lastUpdatedTime;
		double lastFrameTime;

		rRenderer();
		~rRenderer();

		rRenderer(const rRenderer&) = delete;
		rRenderer& operator=(const rRenderer&) = delete;

		void run();

		private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSets();
		void createUniformBuffers();

		rWindow window{ WIDTH, HEIGHT, "Engine4D" };
		rDevice device{ window };
		std::unique_ptr<rSwapChain> swapChain;
		std::unique_ptr<rPipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<rModel> model;

		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;
	};
}
