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
		this->components = std::vector<Component*>();
		this->AddComponent<ObjectMetaData>();
		if (_engine->freeIDs.empty()) id = _engine->gameObjects.size();
		else
		{
			id = _engine->freeIDs.front();
			_engine->freeIDs.pop();
		}
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
		engine->freeIDs.push(id);
		engine->gameObjects[id] = nullptr;
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

	void GameObject::RemoveChild(int index)
	{
		delete children[index];
		children.erase(children.begin() + index);
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
		this->root = new GameObject(this); this->root->name = "Root";
		this->gameObjects.push_back(this->root);
		this->freeIDs = std::queue<int>();
		this->FirstGameObject();
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
		renderer->materials = &materialsData;
		renderer->materialCount = &materialCount;
		renderer->updatedMaterials = &updatedMaterials;

		AddMaterial();
		materials[0]->diffuse = Vector4(0.1, 0.1, 0.1, 1);
		std::cout << "Engine mat[0] index: " << materials[0]->index << std::endl;
		UpdateMaterials();
		
	}

	void Engine::UpdateInstructions()
	{
		instructionCount = 0;
		instructions.clear();

		FirstGameObject();
		while (true)
		{
			int n = currentGameObject->components.size();
			for (int i = 0; i < n; i++)
			{
				Component* component = currentGameObject->components[i];

				if (MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(component))
				{
					std::vector<InstructionData> _instructions = meshRenderer->getInstructions();
					instructions.insert(instructions.end(), _instructions.begin(), _instructions.end());
					instructionCount += _instructions.size();
				}

			}
			if (isLastGameObject())
			{
				break;
			}
			iterateNextGameObject();
		} 
		instructions.resize(MAX_INSTRUCTIONS);
	}

	void Engine::UpdateMaterials()
	{
		materialsData.clear();

		for (int i = 0; i < materialCount; i++)
		{
			materialsData.push_back(MaterialToData(materials[i]));
		}
		materialsData.resize(MAX_MATERIAL_COUNT);
		updatedMaterials = true;
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

	int Engine::AddGameObject(GameObject* gameObject)
	{
		if (freeIDs.size() > 0)
		{
			int id = freeIDs.front();
			freeIDs.pop();
			gameObjects[id] = gameObject;
			return id;
		}
		else
		{
			gameObjects.push_back(gameObject);
			return gameObjects.size() - 1;
		}
	}

	void Engine::iterateNextGameObject()
	{
		currentGameObjectIndex++;
		if (currentGameObjectIndex >= gameObjects.size())
		{
			throw std::out_of_range("currentGameObjectIndex is Out of Bounds. (>= gameObjects.size())");
		}
		while (gameObjects[currentGameObjectIndex] == nullptr)
		{
			currentGameObjectIndex++;
			if (currentGameObjectIndex >= gameObjects.size())
			{
				throw std::out_of_range("currentGameObjectIndex is Out of Bounds. (>= gameObjects.size())");
			}
		}
		currentGameObject = gameObjects[currentGameObjectIndex];
	}

	void Engine::iteratePreviousGameObject()
	{
		currentGameObjectIndex--;
		if (currentGameObjectIndex < 0)
		{
			throw std::out_of_range("currentGameObjectIndex is Out of Bounds. (< 0)");
		}
		while (gameObjects[currentGameObjectIndex] == nullptr)
		{
			currentGameObjectIndex--;
			if (currentGameObjectIndex < 0)
			{
				throw std::out_of_range("currentGameObjectIndex is Out of Bounds. (< 0)");
			}
		}
		currentGameObject = gameObjects[currentGameObjectIndex];
	}

	void Engine::FirstGameObject()
	{
		currentGameObjectIndex = 0;
		currentGameObject = gameObjects[currentGameObjectIndex];
	}

	void Engine::iterateGameObjects(int n)
	{
		if (n > 0)
		{
			for (int i = 0; i < n; i++)
			{
				iterateNextGameObject();
			}
		}
		else
		{
			for (int i = 0; i < -n; i++)
			{
				iteratePreviousGameObject();
			}
		}
	}

	bool Engine::isLastGameObject()
	{
		int i = currentGameObjectIndex;
		try {
			iterateNextGameObject();
			iteratePreviousGameObject();
			return false;
		}
		catch (std::out_of_range &e)
		{
			currentGameObjectIndex = i;
			currentGameObject = gameObjects[currentGameObjectIndex];
			return true;
		}
	}

	ObjectMetaData::ObjectMetaData(GameObject* gameObject) : Component(gameObject)
	{

	}

	ObjectMetaData::ObjectMetaData(GameObject* gameObject, /*std::string name,*/ std::string tag, std::string layer) : Component(gameObject)
	{
		//this->name = name;
		this->tag = tag;
		this->layer = layer;
	}

	Material* Engine::AddMaterial()
	{
		if (materialCount >= MAX_MATERIAL_COUNT)
		{
			return nullptr;
		}

		materials.push_back(new Material(materialCount));
		materialCount++;
		return materials[materialCount - 1];
	}

}