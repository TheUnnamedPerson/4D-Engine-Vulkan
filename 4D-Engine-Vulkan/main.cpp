
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

#include <glm/glm.hpp>

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

        /*engine.root->AddChild();

        std::cout << engine.root->GetChild(0)->transform.toString() << std::endl;

		Engine4D::MeshRenderer* rend = engine.root->GetChild(0)->AddComponent<Engine4D::MeshRenderer>();


		rend->AddShape(new Engine4D::HyperPlane());


		rend->material->color = Engine4D::Vector4(0.1f, 0.65f, 0.6f, 1.0f);

        rend->transform->position = Engine4D::Vector4(0, 0, 0, 0);*/





        engine.root->AddChild();

        std::cout << engine.root->GetChild(0)->transform.toString() << std::endl;

        Engine4D::MeshRenderer* rend = engine.root->GetChild(0)->AddComponent<Engine4D::MeshRenderer>();
        rend->AddShape(new Engine4D::Tesseract());
        rend->material->color = Engine4D::Vector4(1.0f, 0.0f, 0.0f, 1.0f);

        rend->transform->position = Engine4D::Vector4(0, 1, 0, 0);//Engine4D::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

        Engine4D::RigidBody* rb = engine.root->GetChild(0)->AddComponent<Engine4D::RigidBody>();
        rb->rotationalVelocity = Engine4D::Vector3(0.75f, 0.0f, 0.0f);
        rb->rotationalVelocityW = Engine4D::Vector3(0.0f, 0.0f, 1.0f);

        engine.UpdateInstructions();

        //std::cout << "Instructions Size" << engine.instructions.size() << std::endl;

		/*Engine4D::Vector4 result = Engine4D::Matrix::RotationMatrixEuler4D(Engine4D::Vector4(.3224, .123213, .123123, 23.34)) * Engine4D::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        std::cout << "(1, 1, 1, 1) Rotated: " << result.normalized() << std::endl;
        result = Engine4D::Matrix::RotationMatrixEuler4D(Engine4D::Vector4(.3224, .123213, .123123, 23.34)) * result;
        std::cout << "(1, 1, 1, 1) Rotated: " << result.normalized() << std::endl;*/

		std::cout << "Instructions: " << engine.instructionCount << std::endl;

		for (int i = 0; i < engine.instructionCount; i++)
        {
			std::cout << "Instruction: " << i << std::endl;
			//std::cout << "\tType: " << engine.instructions[i].type << std::endl;
			std::cout << "\tValue A: " << Engine4D::Matrix(engine.instructions[i].valueA) << std::endl;
            //std::cout << "\tFloat A: " << Engine4D::Vector4(engine.instructions[i].floatA) << std::endl;
            //std::cout << "\tFloat B: " << Engine4D::Vector4(engine.instructions[i].floatA) << std::endl;
            std::cout << "\tValue B: " << Engine4D::Vector4(engine.instructions[i].floatB, engine.instructions[i].floatC, engine.instructions[i].floatD, engine.instructions[i].valueE) << std::endl;
            std::cout << "\tValue B.w: " << engine.instructions[i].valueE << std::endl;
		}

        std::cout << "\rb.rotation: " << rb->transform->rotation << " ; " << rb->transform->rotationW << std::endl;

        std::cout << "\tMat(1): " << Engine4D::Matrix(glm::mat4(1)) << std::endl;

        //glm::mat4 _test1;

        app.run(); //vertices, indices);
        

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}