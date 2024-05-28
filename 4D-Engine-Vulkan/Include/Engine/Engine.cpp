module;

#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>

import Engine4D.Components;

module Engine4D.Engine;

namespace Engine4D
{

	std::string Component::toString()
	{
		return "Component";
	}

	Transform::Transform(GameObject* gameObject)
	{
		this->gameObject = gameObject;
		this->position = Vector4();
		this->rotation = Vector4();
		this->scale = Vector4();
	}

	Transform::Transform(Vector4 position, Vector4 rotation, Vector4 scale, GameObject* gameObject)
	{
		this->gameObject = gameObject;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
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
	
	template<typename T>
	T* GameObject::AddComponent()
	{
		if (std::is_base_of<Component, T>::value)
		{
			components.push_back(new T(this));
			return (T*)components[components.size() - 1];
		}
		else
		{
			throw std::invalid_argument("T must be a subclass of Component");
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

	template<typename T>
	T GameObject::GetComponent()
	{
		for (Component component : components)
		{
			if (T* t = dynamic_cast<T*>(&component))
			{
				return *t;
			}
		}
	}

	template<typename T>
	std::vector<T> GameObject::GetComponents()
	{
		std::vector<T> result;
		for (Component component : components)
		{
			if (T* t = dynamic_cast<T*>(&component))
			{
				result.push_back(*t);
			}
		}
		return result;
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