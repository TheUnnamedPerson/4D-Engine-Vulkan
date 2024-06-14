module;

#include <vector>
#include <string>
#include <math.h>

#include <iostream>

#include <glm/glm.hpp>

module Engine4D.Components;

namespace Engine4D
{
	Mesh::Mesh()
	{
		this->shapes = std::vector<Shape*>();
	}

	Mesh::Mesh(std::vector<Shape*> shapes)
	{
		this->shapes = shapes;
	}

	Mesh::~Mesh()
	{
		for (int i = 0; i < this->shapes.size(); i++)
		{
			delete this->shapes[i];
		}
	}

	void Mesh::AddShape(Shape* shape)
	{
		this->shapes.push_back(shape);
	}

	MeshRenderer::MeshRenderer(GameObject* gameObject) : MonoBehavior(gameObject)
	{
		this->mesh = Mesh();
		this->name = "MeshRenderer";
		//this->material = gameObject->engine->AddMaterial();
		this->material = nullptr;
		if (this->material == nullptr)
		{
			std::cout << "Material is null" << std::endl;
		}
	}

	MeshRenderer::MeshRenderer(GameObject* gameObject, Material* material) : MonoBehavior(gameObject)
	{
		this->mesh = Mesh();
		this->material = material;
	}

	MeshRenderer::MeshRenderer(GameObject* gameObject, Mesh mesh, Material* material) : MonoBehavior(gameObject)
	{
		this->mesh = mesh;
		this->material = material;
	}

	void MeshRenderer::AddShape(Shape* shape)
	{
		this->mesh.AddShape(shape);
		gameObject->engine->sceneChanged = true;
	}

	std::string MeshRenderer::toString()
	{
		return "MeshRenderer";
	}

	std::vector<InstructionData> MeshRenderer::getInstructions()
	{
		std::vector<InstructionData> instructions = std::vector<InstructionData>();

		int n = this->mesh.shapes.size();

		if (n == 0)
		{
			return instructions;
		}
		else
		{
			Instruction initInstruction = Instruction();
			initInstruction.type = 21;
			initInstruction.valueA = glm::mat4(0);
			initInstruction.valueB = glm::vec4(2, 0, 0, 0);
			//initInstruction.valueB = (glm::vec4)material.color;
			instructions.push_back(InstructionToData(initInstruction));

			for (int i = 0; i < n; i++)
			{
				if (i < n - 1)
				{
					Instruction unionInstruction = Instruction();
					unionInstruction.type = 13;
					unionInstruction.valueA = glm::mat4(0);
					unionInstruction.valueB = glm::vec4(0);

					instructions.push_back(InstructionToData(unionInstruction));
				}

				Shape* shape = this->mesh.shapes[i];

				Instruction shapeInstruction = Instruction();
				shapeInstruction = shape->getInstruction();
				shapeInstruction.valueA = transform->transformationMatrix * shapeInstruction.valueA;
				shapeInstruction.valueB = (glm::vec4)(transform->position) + shapeInstruction.valueB;
				//shapeInstruction.valueB = (transform->transformationMatrix * shapeInstruction.valueB) + transform->positionTransformed;
				instructions.push_back(InstructionToData(shapeInstruction));
			}
		}
		std::cout << "\trb.position: " << this->transform->position << std::endl;
		std::cout << "\trb.rotation: " << this->transform->rotation << " ; " << this->transform->rotationW << std::endl;
		return instructions;
	}

	RigidBody::RigidBody(GameObject* gameObject) : MonoBehavior(gameObject)
	{
		this->gravity = Vector4(0, -9.8f, 0, 0);
		this->name = "RigidBody";
	}

	void RigidBody::Update()
	{
		//std::cout << "RigidBody Update" << std::endl;

		velocity += gravity * gameObject->engine->Time->deltaTime;
		transform->position += velocity * gameObject->engine->Time->deltaTime;
		transform->rotation += rotationalVelocity * gameObject->engine->Time->deltaTime;
		transform->rotation.x = std::fmod(transform->rotation.x, 2 * PI);
		transform->rotation.y = std::fmod(transform->rotation.y, 2 * PI);
		transform->rotation.z = std::fmod(transform->rotation.z, 2 * PI);
		transform->rotationW += rotationalVelocityW * gameObject->engine->Time->deltaTime;
		transform->rotationW.x = std::fmod(transform->rotationW.x, 2 * PI);
		transform->rotationW.y = std::fmod(transform->rotationW.y, 2 * PI);
		transform->rotationW.z = std::fmod(transform->rotationW.z, 2 * PI);

		if (transform->position.y < -0.5)
		{
			transform->position.y = 20;
			velocity.y = 0;
		}

		//MeshRenderer* rend = gameObject->GetComponent<MeshRenderer>();
		//if (rend != nullptr) rend->

		transform->setTransformationMatrix();
		gameObject->engine->sceneChanged = true;

		countdowntime += gameObject->engine->Time->deltaTime;
	}

	std::string RigidBody::toString()
	{
		return "RigidBody";
	}
}