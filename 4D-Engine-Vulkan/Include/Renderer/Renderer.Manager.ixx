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
import Engine4D.Renderer.System;
import Engine4D.Renderer.Descriptors;

import Engine4D.Primitives;

import Engine4D.Material;


import Engine4D.Structs;

import Engine4D.Time;

namespace Engine4D {
	export class rManager {
		public:
		void (*main_Update)();
		void (*main_Late_Update)();

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		static constexpr int MAX_FPS = 60;

		TimeClass* Time;

		std::vector<InstructionData>* instructions;
		int* instructionCount;

		std::vector<MaterialData>* materials;
		int* materialCount;
		bool* updatedMaterials;

		uint32_t frameIndex = 0;

		rManager();
		rManager(void (*_main_Update)(), void (*_main_Late_Update)(), TimeClass* time);
		~rManager();

		rManager(const rManager&) = delete;
		rManager& operator=(const rManager&) = delete;

		void cameraControls();

		void run();

		private:
		void loadModels();

		rWindow window{ WIDTH, HEIGHT, "Engine4D" };
		rDevice device{ window };
		rRendering renderer = rRendering(window, device);
		std::unique_ptr<rModel> model;

		std::unique_ptr<rDescriptorPool> globalDescriptorPool{};


		float rotation = 0.0f;
		glm::vec4 cameraPosition = { 0.0f, 0.0f, 2.0f, 1.0f };
	};
}
