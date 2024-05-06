

import Engine4D.Renderer;

#include <iostream>
#include <exception>
#include <vector>

#include "Vertex.h"

#include <vld.h>


const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {0.1f, 0.1f, 0.5f}, {1.0f, 0.0f}}, //0
    {{0.5f, -0.5f, 0.0f}, {0.2f, 0.5f, 0.7f}, {0.0f, 0.0f}}, //1
    {{0.5f, 0.5f, 0.0f}, {0.3f, 0.7f, 1.0f}, {0.0f, 1.0f}}, //2
    {{-0.5f, 0.5f, 0.0f}, {0.05f, 0.25f, 0.7f}, {1.0f, 1.0f}}, //3
};

const std::vector<uint32_t> indices = {
    0, 1, 2,
    2, 3, 0,
};

int main() {
	VLDEnable();
    

    Engine4D::rRenderer app = Engine4D::rRenderer();

    try {
        app.run();//vertices, indices);
        

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}