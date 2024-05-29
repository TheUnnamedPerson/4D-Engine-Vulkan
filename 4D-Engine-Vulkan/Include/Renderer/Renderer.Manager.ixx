module;

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

export module Engine4D.Renderer.Manager;

import Engine4D.Renderer.Rendering;
import Engine4D.Renderer.Device;
import Engine4D.Renderer.Model;
import Engine4D.Renderer.Pipeline;
import Engine4D.Renderer.Window;
import Engine4D.Renderer.Buffer;


import Engine4D.Structs;

import Engine4D.Time;

namespace Engine4D {
	export class rManager {
		public:
		void (*main_Update)();
		void (*main_Late_Update)();

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		static constexpr int MAX_FPS = 30;

		TimeClass* Time;

		std::vector<Instruction>* instructions;

		uint32_t frameIndex = 0;

		double lastUpdatedTime;
		double lastFrameTime;

		rManager();
		rManager(void (*_main_Update)(), void (*_main_Late_Update)(), TimeClass* time);
		~rManager();

		rManager(const rManager&) = delete;
		rManager& operator=(const rManager&) = delete;

		void run();

		private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();

		void doModelStuff(VkCommandBuffer commandBuffer);

		rWindow window{ WIDTH, HEIGHT, "Engine4D" };
		rDevice device{ window };
		rRendering renderer = rRendering(window, device);
		std::unique_ptr<rPipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::unique_ptr<rModel> model;

		float rotation = 0.0f;
		glm::vec4 cameraPosition = { 0.0f, 0.0f, 2.0f, 1.0f };
	};
}
