

import Engine4D.Renderer;

#include <iostream>
#include <exception>
#include <vector>

#include <vld.h>


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