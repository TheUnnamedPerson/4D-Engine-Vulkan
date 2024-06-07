
import Engine4D.Renderer.Manager;
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

    Engine4D::rManager app = Engine4D::rManager(engine_Update, engine_Late_Update, &Time);

	engine.Time = &Time;
	engine.renderer = &app;

    try {

        engine.Initialize();

        engine.root->AddChild();

        std::cout << engine.root->GetChild(0)->transform.toString() << std::endl;

		Engine4D::MeshRenderer* rend = engine.root->GetChild(0)->AddComponent<Engine4D::MeshRenderer>();
		rend->AddShape(new Engine4D::Tesseract());
		rend->material->color = Engine4D::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		rend->material->index = 2;

		rend->transform->position = Engine4D::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

        Engine4D::RigidBody* rb = engine.root->GetChild(0)->AddComponent<Engine4D::RigidBody>();
        rb->rotationalVelocity = Engine4D::Vector3(0.75f, 0.0f, 0.0f);
        rb->rotationalVelocityW = Engine4D::Vector3(0.0f, 0.0f, 1.0f);

        engine.UpdateInstructions();

		std::cout << "Instructions: " << engine.instructionCount << std::endl;

		for (int i = 0; i < engine.instructionCount; i++)
        {
			std::cout << "Instruction: " << i << std::endl;
            std::cout << "\tValue A: " << Engine4D::Matrix(engine.instructions[i].valueA) << std::endl;
            std::cout << "\tValue B: " << Engine4D::Vector4(engine.instructions[i].floatB, engine.instructions[i].floatC, engine.instructions[i].floatD, engine.instructions[i].valueE) << std::endl;
            std::cout << "\tValue B.w: " << engine.instructions[i].valueE << std::endl;
		}

        app.run(); //vertices, indices);
        

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}