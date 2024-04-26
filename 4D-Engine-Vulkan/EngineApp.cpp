#include "EngineApp.h"

void Engine::EngineApp::run()
{
	while (!window.shouldClose())
	{
		glfwPollEvents();
	}
}
