
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

class TestBase {
    public:
	    std::string name;
		virtual void empty() {}
};

class TestDerived : public TestBase {
	public:
	    std::vector<int> intList;
	    TestDerived() {
		    name = "TestDerived";
			//intList = std::vector<int>();
	    }
		void empty() override {}
};

class TestDerived2 : public TestBase {
	public:
		int id;
		TestDerived2() {
			name = "TestDerived2";
			id = 2;
		}
		void empty() override {}
};

int main() {
	VLDEnable();

    Engine4D::rManager app = Engine4D::rManager(engine_Update, engine_Late_Update, &Time);

	engine.Time = &Time;
	engine.renderer = &app;

	TestDerived2 test = TestDerived2();

	std::cout << "Test 1: " << test.name << std::endl;

	TestBase* test2 = &test;

	std::cout << "Test 2: " << test2->name << std::endl;

	if (TestDerived2* test3 = dynamic_cast<TestDerived2*>(test2))
	{
		std::cout << "Test 3: " << test3->id << std::endl;
	}

	if (TestDerived* test4 = dynamic_cast<TestDerived*>(test2))
	{
		std::cout << "Test 4: " << test4->intList.size() << std::endl;
	}

	std::cout << "Done" << std::endl;

    try {

        engine.Initialize();

		Engine4D::GameObject* floor = engine.root->AddChild();

		floor->name = "Floor";

		std::cout << floor->transform.toString() << std::endl; \

		Engine4D::MeshRenderer* FloorRend = floor->AddComponent<Engine4D::MeshRenderer>();
		FloorRend->AddShape(new Engine4D::HyperPlane());

		FloorRend->transform->position = Engine4D::Vector4(0.0f);

        Engine4D::GameObject* cube = engine.root->AddChild();

		cube->name = "Tesseract";

        std::cout << cube->transform.toString() << std::endl;\

		Engine4D::MeshRenderer* CubeRend = cube->AddComponent<Engine4D::MeshRenderer>();
		CubeRend->AddShape(new Engine4D::Tesseract());
		//rend->material->color = Engine4D::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		//rend->material->index = 2;

		CubeRend->transform->position = Engine4D::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

        Engine4D::RigidBody* rb = cube->AddComponent<Engine4D::RigidBody>();
        rb->rotationalVelocity = Engine4D::Vector3(0.75f, 0.0f, 0.0f);
        rb->rotationalVelocityW = Engine4D::Vector3(0.0f, 0.0f, 1.0f);

		std::cout << "Getting Instructions." << std::endl;

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