#include "Engine_Window.h"

Engine::Engine_Window::Engine_Window(int w, int h, std::string name) : width(w), height(h), windowName(name)
{
	initWindow();
}

Engine::Engine_Window::~Engine_Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::Engine_Window::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
