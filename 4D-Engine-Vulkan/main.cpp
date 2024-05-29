
import Engine4D.Renderer;
import Engine4D.Engine;
import Engine4D.Time;
import Engine4D.Components;
import Engine4D.Primitives;

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

        std::cout << engine.root->GetChild(0)->transform.toString() << std::endl;

		Engine4D::MeshRenderer* rend = engine.root->GetChild(0)->AddComponent<Engine4D::MeshRenderer>();
		rend->AddShape(new Engine4D::HyperSphere());
		rend->material.color = Engine4D::Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		rend->transform->position = Engine4D::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

        engine.UpdateInstructions();

		std::cout << "Instructions: " << engine.instructions.size() << std::endl;

		for (int i = 0; i < engine.instructions.size(); i++)
        {
			std::cout << "Instruction: " << i << std::endl;
			std::cout << "\tType: " << engine.instructions[i].type << std::endl;
			std::cout << "\tValue A: " << Engine4D::Vector4(engine.instructions[i].valueA) << std::endl;
            std::cout << "\tValue A: " << Engine4D::Vector4(engine.instructions[i].valueB) << std::endl;
		}

        app.run(); //vertices, indices);
        

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}