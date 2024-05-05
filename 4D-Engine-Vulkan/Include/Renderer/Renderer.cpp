#include <stb_image.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <optional>
#include <set>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

module Renderer:rImplement;

using std::string;

namespace Renderer {
	rRenderer::rRenderer() {
		window = new rWindow(this);
		initVulkan();
		mainLoop();
	}
}