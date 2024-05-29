module;

#include <vector>
#include <string>

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

	Material::Material()
	{
		this->color = Vector4();
	}

	Material::Material(Vector4 color)
	{
		this->color = color;
	}

	MeshRenderer::MeshRenderer(GameObject* gameObject) : MonoBehavior(gameObject)
	{
		this->mesh = Mesh();
		this->material = Material();
	}

	MeshRenderer::MeshRenderer(GameObject* gameObject, Mesh mesh, Material material) : MonoBehavior(gameObject)
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

	std::vector<Instruction> MeshRenderer::getInstructions()
	{
		std::vector<Instruction> instructions = std::vector<Instruction>();

		int n = this->mesh.shapes.size();

		if (n == 0)
		{
			return instructions;
		}
		else
		{
			Instruction initInstruction = Instruction();
			initInstruction.type = 11;
			initInstruction.valueA = (glm::vec4)transform->scale;
			initInstruction.valueB = glm::vec4(0);
			instructions.push_back(initInstruction);

			initInstruction = Instruction();
			initInstruction.type = 10;
			initInstruction.valueA = (glm::vec4)transform->rotation;
			initInstruction.valueB = glm::vec4(0);
			instructions.push_back(initInstruction);

			initInstruction = Instruction();
			initInstruction.type = 9;
			initInstruction.valueA = (glm::vec4)transform->position;
			initInstruction.valueB = glm::vec4(0);
			instructions.push_back(initInstruction);
			if (n == 1)
			{
				Shape* shape = this->mesh.shapes[0];

				Instruction instruction = Instruction();
				instruction.type = 11;
				instruction.valueA = (glm::vec4)shape->scale;
				instruction.valueB = glm::vec4(0);
				instructions.push_back(instruction);

				instruction = Instruction();
				instruction.type = 10;
				instruction.valueA = (glm::vec4)shape->rotation;
				instruction.valueB = glm::vec4(0);
				instructions.push_back(instruction);

				instruction = Instruction();
				instruction.type = 9;
				instruction.valueA = (glm::vec4)shape->position;
				instruction.valueB = glm::vec4(0);
				instructions.push_back(instruction);

				instruction = Instruction();
				instruction = shape->getInstruction();
				instruction.valueB = (glm::vec4)this->material.color;
				instructions.push_back(instruction);

				return instructions;
			}

			for (int i = 0; i < n; i++)
			{
				if (i < n - 1)
				{
					Instruction unionInstruction = Instruction();
					unionInstruction.type = 9;
					unionInstruction.valueA = glm::vec4(0);
					unionInstruction.valueB = glm::vec4(0);
					instructions.push_back(unionInstruction);
				}

				Shape* shape = this->mesh.shapes[i];

				Instruction shapeInstruction = Instruction();
				shapeInstruction = shape->getInstruction();
				shapeInstruction.valueB = (glm::vec4)this->material.color;
				instructions.push_back(shapeInstruction);
			}
		}
		return instructions;
	}

	RigidBody::RigidBody(GameObject* gameObject) : MonoBehavior(gameObject)
	{
		this->gravity = Vector4(0, -9.8f, 0, 0);
	}

	void RigidBody::Update()
	{
		//std::cout << "RigidBody Update" << std::endl;

		velocity += gravity * gameObject->engine->Time->deltaTime;
		transform->position += velocity * gameObject->engine->Time->deltaTime;
		transform->rotation += rotationalVelocity * gameObject->engine->Time->deltaTime;

		if (transform->position.y < -0.5)
		{
			transform->position.y = 20;
			velocity.y = 0;
		}

		gameObject->engine->sceneChanged = true;

		countdowntime += gameObject->engine->Time->deltaTime;
	}

	std::string RigidBody::toString()
	{
		return "RigidBody";
	}
}