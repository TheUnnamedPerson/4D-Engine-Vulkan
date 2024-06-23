module;

#include <string>
#include <vector>
#include <type_traits>
#include <unordered_map>

#include <GLFW/glfw3.h>

#include <iostream>

module Engine4D.Engine;

import Engine4D.Components;

namespace Engine4D
{
	Collision::Collision(Collider* collider1, Collider* collider2)
	{
		this->collider1 = collider1;
		this->collider2 = collider2;
	}

	Component::Component(GameObject* gameObject)
	{
		this->gameObject = gameObject;
		this->transform = &(gameObject->transform);
		this->gameObject->engine->AddUpdateComponent(this);
	}

	Component::Component(GameObject* gameObject, int _id)
	{
		this->id = _id;
		this->gameObject = gameObject;
		this->transform = &(gameObject->transform);
		this->gameObject->engine->AddUpdateComponent(this);
	}

	Component::~Component()
	{
		gameObject->engine->RemoveUpdateComponent(this);
	}

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

	MonoBehavior::MonoBehavior(GameObject* gameObject, int _id) : Component(gameObject, _id)
	{

	}

	Collider::Collider(GameObject* gameObject) : MonoBehavior(gameObject, 2)
	{
		this->AddShape(new Tesseract());
		this->name = "Collider";
	}

	Collider::Collider(GameObject* gameObject, Mesh colliderMesh) : MonoBehavior(gameObject, 2)
	{
		this->colliderMesh = colliderMesh;
		this->name = "Collider";
	}

	float Collider::SDF(Vector4 point)
	{
		point -= transform->position;
		point = Matrix::RotationMatrixDoubleEuler4D(transform->rotation * -1, transform->rotationW * -1) * point;
		point = point / transform->scale;
		return colliderMesh.SDF(point) * transform->scale.length() * 1.005;
	}

	Vector4 Collider::CollisionMarch(Collider* other, Vector4 reference)
	{
		Vector4 result = Vector4(0);

		float distance = std::fmaxf(this->SDF(reference), other->SDF(reference));
		float distance_squared = distance * distance;

		float distanceX = std::fmaxf(this->SDF(reference - Vector4(epsilon, 0, 0, 0)), other->SDF(reference - Vector4(epsilon, 0, 0, 0)));
		float distanceX_squared = distanceX * distanceX;

		float distanceY = std::fmaxf(this->SDF(reference - Vector4(0, epsilon, 0, 0)), other->SDF(reference - Vector4(0, epsilon, 0, 0)));
		float distanceY_squared = distanceY * distanceY;

		float distanceZ = std::fmaxf(this->SDF(reference - Vector4(0, 0, epsilon, 0)), other->SDF(reference - Vector4(0, 0, epsilon, 0)));
		float distanceZ_squared = distanceZ * distanceZ;

		float distanceW = std::fmaxf(this->SDF(reference - Vector4(0, 0, 0, epsilon)), other->SDF(reference - Vector4(0, 0, 0, epsilon)));
		float distanceW_squared = distanceW * distanceW;
		
		result.x = (distance_squared - distanceX_squared + epsilon_squared) / (2 * epsilon);
		result.y = (distance_squared - distanceY_squared + epsilon_squared) / (2 * epsilon);
		result.z = (distance_squared - distanceZ_squared + epsilon_squared) / (2 * epsilon);
		result.w = (distance_squared - distanceW_squared + epsilon_squared) / (2 * epsilon);

		return result * transform->scale.length() * 1.05f;
	}

	Vector4 Collider::FindNormal(Collider* other, Vector4 reference)
	{
		Vector4 result = Vector4(0);

		float distance = std::fmaxf(this->SDF(reference), other->SDF(reference));

		float distanceX = std::fmaxf(this->SDF(reference - Vector4(epsilon, 0, 0, 0)), other->SDF(reference - Vector4(epsilon, 0, 0, 0)));

		float distanceY = std::fmaxf(this->SDF(reference - Vector4(0, epsilon, 0, 0)), other->SDF(reference - Vector4(0, epsilon, 0, 0)));

		float distanceZ = std::fmaxf(this->SDF(reference - Vector4(0, 0, epsilon, 0)), other->SDF(reference - Vector4(0, 0, epsilon, 0)));

		float distanceW = std::fmaxf(this->SDF(reference - Vector4(0, 0, 0, epsilon)), other->SDF(reference - Vector4(0, 0, 0, epsilon)));

		//std::cout << "Distances: " << Vector4(distanceX, distanceY, distanceZ, distanceW) << std::endl;
		//std::cout << "Distance: " << distance << std::endl;
		result.x = (distance - distanceX);
		result.y = (distance - distanceY);
		result.z = (distance - distanceZ);
		result.w = (distance - distanceW);

		//std::cout << "result: " << result << std::endl;

		if (result.length() == 0)
		{
			return Vector4(0, 1, 0, 0);
		}

		result = result.normalized();

		//std::cout << "result: " << result << std::endl;

		return result;
	}
	
	void Collider::CheckCollision(Collider* other)
	{
		bool debug_log = false;
		if (coll_debug_countdown > 120 && gameObject->name != "Floor" && other->gameObject->name != "Floor") debug_log = true;
		if (debug_log) std::cout << "Checking Collision! - " << this->gameObject->name << " ; " << other->gameObject->name << std::endl;
		Vector4 referenceA = transform->position;
		
		Vector4 referenceB = other->transform->position;
		
		bool resultA = false;
		bool resultB = false;

		Vector4 normalA;
		Vector4 normalB;

		if (debug_log) std::cout << "Starting RayMarch! Reference A ; Reference B - " << referenceA << " ; " << referenceB << std::endl;
		if (debug_log) std::cout << "SDFA = " << colliderMesh.SDF(referenceA) << " ; " << other->colliderMesh.SDF(referenceA) << std::endl;
		if (debug_log) std::cout << "SDFB = " << colliderMesh.SDF(referenceB) << " ; " << other->colliderMesh.SDF(referenceB) << std::endl;

		for (int i = 0; i < 10; i++)
		{
			Vector4 raymarchedA = CollisionMarch(other, referenceA) * transform->scale.length();
			referenceA -= raymarchedA;

			if (debug_log) std::cout << "Raymarching Collider A. i = " << i << " ; ReferenceA = " << referenceA << " ; rayMarchedA" << raymarchedA << std::endl;

			if (debug_log) std::cout << "SDF = " << colliderMesh.SDF(referenceA) << std::endl;

			if (colliderMesh.SDF(referenceA) < 0)
			{
				normalA = FindNormal(other, referenceA);
				resultA = true;
				break;
			}
		}
		for (int i = 0; i < 10; i++)
		{
			Vector4 raymarchedB = other->CollisionMarch(this, referenceB) * other->transform->scale.length();
			referenceB -= raymarchedB;

			if (debug_log) std::cout << "Raymarching Collider B. i = " << i << " ; ReferenceB = " << referenceB << " ; rayMarchedB" << raymarchedB << std::endl;

			if (debug_log) std::cout << "SDF = " << other->colliderMesh.SDF(referenceB) << std::endl;

			if (other->colliderMesh.SDF(referenceB) < 0)
			{
				normalB = other->FindNormal(this, referenceB);
				resultB = true;
				break;
			}
		}

		Collision collision = Collision(this, other);
		if (resultA)
		{
			collision.collisionPointA = referenceA * transform->scale;
			collision.collisionPointA = Matrix::RotationMatrixDoubleEuler4D(transform->rotation, transform->rotationW) * collision.collisionPointA;
			collision.collisionPointA = collision.collisionPointA + transform->position;

			collision.collisionNormalA = normalA;
		}
		if (resultB)
		{
			collision.collisionPointB = referenceB * other->transform->scale;
			collision.collisionPointB = Matrix::RotationMatrixDoubleEuler4D(other->transform->rotation, other->transform->rotationW) * collision.collisionPointB;
			collision.collisionPointB = collision.collisionPointB + other->transform->position;

			collision.collisionNormalB = normalB;
		}

		if (collisions.count(other) == 0 && (resultA || resultB))
		{
			collisions[other] = collision;
			TriggerCollisionEnters(collision);
			other->collisions[this] = collision;
			other->TriggerCollisionEnters(collision);
		}
		else if (collisions.count(other) > 0)
		{
			//std::cout << "else -> count(other) - " << collisions.count(other) << std::endl;
			//std::cout << "else -> count(this) - " << other->collisions.count(this) << std::endl;
			if (resultA || resultB)
			{
				collisions[other] = collision;
				other->collisions[this] = collision;
				TriggerCollisionStays(collision);
				other->TriggerCollisionStays(collision);
			}
			else
			{
				TriggerCollisionExits(collisions[other]);
				collisions.erase(other);
				//std::cout << "else - count(other) - " << collisions.count(other) << std::endl;
				other->TriggerCollisionExits(other->collisions[this]);
				other->collisions.erase(this);
				//std::cout << "else - count(this) - " << other->collisions.count(this) << std::endl;
			}
		}
		if (debug_log) coll_debug_countdown++;
		//std::cout << "count(other) - " << collisions.count(other) << std::endl;
		//std::cout << "count(this) - " << other->collisions.count(this) << std::endl;
	}

	void Collider::TriggerCollisionEnters(Collision collision)
	{
		std::cout << "Triggering Collision Enters!: " << collision.collider1->gameObject->name << " ; " << collision.collider2->gameObject->name << std::endl;
		std::vector<MonoBehavior*> components = gameObject->GetComponents<MonoBehavior>();
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnCollisionEnter(collision);
		}
	}

	void Collider::TriggerCollisionStays(Collision collision)
	{
		//std::cout << "Triggering Collision Enters!: " << gameObject->name << std::endl;
		std::vector<MonoBehavior*> components = gameObject->GetComponents<MonoBehavior>();
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnCollisionStay(collision);
		}
	}

	void Collider::TriggerCollisionExits(Collision collision)
	{
		std::vector<MonoBehavior*> components = gameObject->GetComponents<MonoBehavior>();
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnCollisionExit(collision);
		}
	}

	void Collider::AddShape(Shape* shape)
	{
		this->colliderMesh.AddShape(shape);
	}

	std::string Collider::toString()
	{
		return "Collider";
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
		materials[0]->ambient = 0;
		std::cout << "Engine mat[0] index: " << materials[0]->index << std::endl;
		UpdateMaterials();
		
	}

	void Engine::AddUpdateComponent(Component* component)
	{
		if (component->id == 0) return;
		if (updateComponents.count(component->id) == 0) updateComponents[component->id] = std::vector<Component*>();
		updateComponents[component->id].push_back(component);
	}

	void Engine::RemoveUpdateComponent(Component* component)
	{
		if (component->id == 0) return;
		if (updateComponents.count(component->id) == 0) return;
		std::vector<Component*> components = updateComponents[component->id];
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == component)
			{
				components.erase(components.begin() + i);
				break;
			}
		}
	}

	void Engine::UpdateAllComponents()
	{
		for (std::pair<int, std::vector<Component*>> keyValue : updateComponents)
		{
			keyValue.second.clear();
		}
		updateComponents.clear();

		FirstGameObject();
		while (true)
		{
			int n = currentGameObject->components.size();
			for (int i = 0; i < n; i++)
			{
				Component* component = currentGameObject->components[i];

				if (component->id == 0) continue;
				if (updateComponents.count(component->id) == 0) updateComponents[component->id] = std::vector<Component*>();
				updateComponents[component->id].push_back(component);

			}
			if (isLastGameObject())
			{
				break;
			}
			iterateNextGameObject();
		}
	}

	void Engine::UpdateInstructions()
	{
		instructionCount = 0;
		instructions.clear();

		int n = updateComponents[3].size();
		for (int i = 0; i < n; i++)
		{
			MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(updateComponents[3][i]);
			if (meshRenderer != nullptr)
			{
				std::vector<InstructionData> _instructions = meshRenderer->getInstructions();
				instructions.insert(instructions.end(), _instructions.begin(), _instructions.end());
				instructionCount += _instructions.size();
			}
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

	void Engine::UpdateCollisions()
	{
		int n = updateComponents[2].size();

		for (int i = 0; i < n; i++)
		{
			Collider* collider = dynamic_cast<Collider*>(updateComponents[2][i]);
			if (collider != nullptr)
			{
				for (int j = i + 1; j < n; j++)
				{
					Collider* collider2 = dynamic_cast<Collider*>(updateComponents[2][j]);
					if (collider2 != nullptr)
					{
						//std::cout << "Checking Collision! - " << collider->gameObject->name << " ; " << collider2->gameObject->name << std::endl;
						collider->CheckCollision(collider2);
					}
				}
			}
		}
	}

	void Engine::Update()
	{
		

		int stateH = glfwGetKey(renderer->window.getWindow(), GLFW_KEY_H);
		if (stateH == GLFW_PRESS && !pushedH)
		{
			std::cout << "PRESSED H: " << stateH << std::endl;
			pushedH = true;
			Engine4D::GameObject* cube = root->AddChild();

			cube->name = "Tesseract " + std::to_string(gameObjects.size() - 2);

			std::cout << cube->transform.toString() << std::endl;

			Engine4D::MeshRenderer* CubeRend = cube->AddComponent<Engine4D::MeshRenderer>();
			CubeRend->AddShape(new Engine4D::Tesseract());
			CubeRend->material = materials[2];

			std::cout << "Cube Mat Index: " << CubeRend->material->index << std::endl;

			UpdateMaterials();

			//rend->material->color = Engine4D::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
			//rend->material->index = 2;

			CubeRend->transform->position = Engine4D::Vector4(1.0f, 5.0f, 1.0f, 0.0f);
			/*
			Engine4D::RigidBody* rb = cube->AddComponent<Engine4D::RigidBody>();
			rb->mass = 1.0f;
			rb->elasticity = -0.05f;
			rb->objectType = 1;
			*/

			Engine4D::Collider* cubeCol = cube->AddComponent<Engine4D::Collider>();
			cubeCol->AddShape(new Engine4D::Tesseract());

			//UpdateAllComponents();
			std::cout << "Spawned Cube." << std::endl;
			//rb->rotationalVelocity = Engine4D::Vector3(0.75f, 0.0f, 0.0f);
			//rb->rotationalVelocityW = Engine4D::Vector3(0.0f, 0.0f, 1.0f);//*/
		}
		else if (stateH == GLFW_RELEASE)
		{
			if (pushedH) std::cout << "RELEASED H: " << stateH << std::endl;
			pushedH = false;
		}

		root->Update();
		UpdateCollisions();
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
		this->id = 1;
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