#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace Engine
{
	class Engine_Window
	{
		public:
			Engine_Window(int w, int h, std::string name);
			~Engine_Window();

			Engine_Window(const Engine_Window&) = delete;
			Engine_Window& operator=(const Engine_Window&) = delete;

			bool shouldClose() { return glfwWindowShouldClose(window); }

		private:
			void initWindow();

			const int width;
			const int height;

			std::string windowName;
			GLFWwindow* window;
	};
}


