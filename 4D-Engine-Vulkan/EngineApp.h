#pragma once

#include "Engine_Window.h"
#include "Engine_Pipeline.h"

namespace Engine
{
	class EngineApp
	{
		public:
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

			void run();

		private:
			Engine_Window window{ WIDTH, HEIGHT, "HELLO? HELLO HELLO?" };
			Engine_Pipeline pipeline{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
	};
}
