

import Engine4D.Renderer;
import Engine4D.Engine;
import Engine4D.Time;
import Engine4D.Components;

#include <iostream>
#include <exception>
#include <vector>

#include <vld.h>

#include <GLFW/glfw3.h>

Engine4D::Engine engine;

Engine4D::TimeClass Time = Engine4D::TimeClass();

void engine_Update()
{
	//std::cout << "Update" << std::endl;
    engine.Update();
}

void engine_Late_Update()
{
    //std::cout << "Update" << std::endl;
    engine.LateUpdate();
}

int main() {
	VLDEnable();

    Engine4D::rRenderer app = Engine4D::rRenderer(engine_Update, engine_Late_Update, &Time);

	engine.renderer = &app;

    try {

        engine.Initialize();

        engine.root->AddChild();

		Engine4D::MeshRenderer* rend = engine.root->GetChild(0)->AddComponent<Engine4D::MeshRenderer>();
		rend->AddShape(new Engine4D::HyperSphere());
		rend->material.color = Engine4D::Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		rend->transform->position = Engine4D::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

        app.run(); //vertices, indices);
        

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}