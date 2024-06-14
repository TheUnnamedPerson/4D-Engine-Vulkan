module;

#include <string>
#include <vector>
#include <type_traits>

#include <iostream>

module Engine4D.Engine;

import Engine4D.Components;

namespace Engine4D
{

	std::string Component::toString()
	{
		return "Component";
	}

	Transform::Transform(GameObject* gameObject)
	{
		this->gameObject = gameObject;
		this->position = Vector4(0);
		this->rotation = Vector3(0);
		this->rotationW = Vector3(0);
		this->setRotationMatrix();
		this->scale = Vector4(1);
		this->setTransformationMatrix();
	}

	Transform::Transform(Vector4 position, Vector3 rotation, Vector4 scale, GameObject* gameObject)
	{
		this->gameObject = gameObject;
		this->position = position;
		this->rotation = rotation;
		this->setRotationMatrix();
		this->rotationW = Vector3(0);
		this->scale = scale;
		this->setTransformationMatrix();
	}

	Transform::Transform(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale, GameObject* gameObject)
	{
		this->gameObject = gameObject;
		this->position = position;
		this->rotation = rotation;
		this->rotationW = rotationW;
		this->setRotationMatrix();
		this->scale = scale;
		this->setTransformationMatrix();
	}

	std::string Transform::toString()
	{
		return "Transform";
	}

	GameObject::GameObject(Engine* _engine)
	{
		this->engine = _engine;
		this->name = "GameObject";
		this->transform = Transform(this);
	}

	GameObject::~GameObject()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] != nullptr) delete components[i];
		}
		components.clear();
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i] != nullptr) delete children[i];
		}
		children.clear();
	}

	GameObject* GameObject::AddChild()
	{
		GameObject* child = new GameObject(engine);
		children.push_back(child);
		engine->AddGameObject(&children[children.size() - 1]);
		return child;
	}

	GameObject* GameObject::AddChild(GameObject* child)
	{
		children.push_back(child);
		return child;
	}

	GameObject* GameObject::GetChild(int index)
	{
		return children[index];
	}

	void GameObject::Update()
	{
		for (Component* component : components)
		{
			if (dynamic_cast<MonoBehavior*>(component) != nullptr)
			{
				((MonoBehavior*)component)->Update();
			}
		}
		for (GameObject* child : children)
		{
			child->Update();
		}
	}

	void GameObject::FixedUpdate()
	{
		for (Component* component : components)
		{
			if (dynamic_cast<MonoBehavior*>(component) != nullptr)
			{
				((MonoBehavior*)component)->FixedUpdate();
			}
		}
		for (GameObject* child : children)
		{
			child->FixedUpdate();
		}
	}

	void GameObject::LateUpdate()
	{
		for (Component* component : components)
		{
			if (dynamic_cast<MonoBehavior*>(component) != nullptr)
			{
				((MonoBehavior*)component)->LateUpdate();
			}
		}
		for (GameObject* child : children)
		{
			child->LateUpdate();
		}
	}

	MonoBehavior::MonoBehavior(GameObject* gameObject) : Component(gameObject)
	{

	}

	Engine::Engine()
	{
		this->gameObjects = std::vector<GameObject**>();
		this->root = new GameObject(this);
		gameObjects.push_back(&this->root);
	}

	Engine::~Engine()
	{
		delete root;
		gameObjects.clear();
		instructions.clear();
	}

	void Engine::Initialize()
	{
		renderer->instructions = &instructions;
		renderer->instructionCount = &instructionCount;
		//renderer->materials = &materials;
	}

	void Engine::UpdateInstructions()
	{
		instructionCount = 0;
		instructions.clear();
		for (GameObject** gameObject : gameObjects)
		{
			for (Component* component : (*gameObject)->components)
			{
				if (dynamic_cast<MeshRenderer*>(component) != nullptr)
				{
					MeshRenderer* meshRenderer = (MeshRenderer*)component;
					std::vector<InstructionData> _instructions = meshRenderer->getInstructions();
					instructions.insert(instructions.end(), _instructions.begin(), _instructions.end());
					instructionCount += _instructions.size();
				}
			}
		}
		instructions.resize(MAX_INSTRUCTIONS);
	}

	void Engine::Update()
	{
		root->Update();
	}

	void Engine::FixedUpdate()
	{
		root->FixedUpdate();
	}

	void Engine::LateUpdate()
	{
		root->LateUpdate();

		if (sceneChanged)
		{
			UpdateInstructions();
			sceneChanged = false;
			
		}
	}

	void Engine::AddGameObject(GameObject** gameObject)
	{
		gameObjects.push_back(gameObject);
	}

	/*Material* Engine::AddMaterial()
	{
		if (materialCount >= MAX_MATERIAL_COUNT)
		{
			return nullptr;
		}

		Material* result = new Material(materialCount);

		materials.push_back(result);
		materialCount++;
		return result;
	}*/

}