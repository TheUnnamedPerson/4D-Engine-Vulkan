module;
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

export module Engine4D.Renderer.Window;

namespace Engine4D {

	export class rWindow {
		public:
		rWindow(int w, int h, std::string name);
		~rWindow();

		rWindow(const rWindow&) = delete;
		rWindow& operator=(const rWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		GLFWwindow* getWindow() { return window; }

		private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		int width;
		int height;
		bool framebufferResized = false;

		std::string windowName;
		GLFWwindow* window;
	};
}
