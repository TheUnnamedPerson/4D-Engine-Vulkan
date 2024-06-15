module;

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

export module Engine4D.Renderer.System;

import Engine4D.Renderer.Device;
import Engine4D.Renderer.Model;
import Engine4D.Renderer.Pipeline;
import Engine4D.Renderer.Buffer;
import Engine4D.Renderer.Rendering;


import Engine4D.Structs;

import Engine4D.Time;

namespace Engine4D {
	export class rSystem {
		public:
		void (*main_Update)();
		void (*main_Late_Update)();

		static constexpr int MAX_FPS = 30;

		TimeClass* Time;

		std::vector<Instruction>* instructions;

		uint32_t frameIndex = 0;

		double lastUpdatedTime;
		double lastFrameTime;

		rSystem() = delete;
		rSystem(rDevice& device, std::unique_ptr<rModel>& model, rRendering& renderer, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSetLayout materialSetLayout);
		~rSystem();

		rSystem(const rSystem&) = delete;
		rSystem& operator=(const rSystem&) = delete;

		void renderObjects(FrameInfo& frameInfo);

		private:
		void createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSetLayout materialSetLayout);
		void createPipeline(VkRenderPass renderPass);

		rDevice& device;
		rRendering& renderer;
		std::unique_ptr<rModel>& model;
		std::unique_ptr<rPipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}
