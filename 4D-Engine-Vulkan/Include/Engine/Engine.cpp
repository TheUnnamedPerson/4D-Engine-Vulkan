module;

#include <string>
#include <vector>
#include <type_traits>

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
		this->rotation = Vector4(0);
		this->scale = Vector4(1);
	}

	Transform::Transform(Vector4 position, Vector4 rotation, Vector4 scale, GameObject* gameObject)
	{
		this->gameObject = gameObject;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
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
		for (Component* component : components)
		{
			delete component;
		}
		for (GameObject* child : children)
		{
			delete child;
		}
	}

	GameObject* GameObject::AddChild()
	{
		GameObject* child = new GameObject(engine);
		children.push_back(child);
		engine->AddGameObject(child);
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
		this->gameObjects = std::vector<GameObject*>();
		gameObjects.push_back(new GameObject(this));
		this->root = gameObjects[0];
	}

	Engine::~Engine()
	{
		for (GameObject* gameObject : gameObjects)
		{
			delete gameObject;
		}
		instructions.clear();
	}

	void Engine::Initialize()
	{
		renderer->instructions = &instructions;
	}

	void Engine::UpdateInstructions()
	{
		instructions.clear();
		instructions = std::vector<Instruction>();
		for (GameObject* gameObject : gameObjects)
		{
			for (Component* component : gameObject->components)
			{
				if (dynamic_cast<MeshRenderer*>(component) != nullptr)
				{
					MeshRenderer* meshRenderer = (MeshRenderer*)component;
					std::vector<Instruction> _instructions = meshRenderer->getInstructions();
					instructions.insert(instructions.end(), _instructions.begin(), _instructions.end());
				}
			}
		}
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

	void Engine::AddGameObject(GameObject* gameObject)
	{
		gameObjects.push_back(gameObject);
	}

}