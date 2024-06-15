module;

#include <string>
#include <vector>
#include <queue>
#include <type_traits>
#include <stdexcept>
#include <string>

#include <glm/glm.hpp>

export module Engine4D.Engine;

import Engine4D.Primitives;
import Engine4D.Time;
import Engine4D.Structs;
import Engine4D.Material;

import Engine4D.Renderer.Manager;

namespace Engine4D {

	export class GameObject;
	export class Component;
	export class Engine;

	export class Transform
	{
		public:
		Vector4 position;
		Vector3 rotation;
		Vector3 rotationW;
		Vector4 scale;

		GameObject* gameObject;
		GameObject* parent;

		Matrix rotationMatrix;
		glm::mat4 transformationMatrix;
		glm::vec4 positionTransformed;

		Transform() = delete;
		Transform(GameObject* gameObject);
		Transform(Vector4 position, Vector3 rotation, Vector4 scale, GameObject* gameObject);
		Transform(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale, GameObject* gameObject);

		void setRotationMatrix() { rotationMatrix = Matrix::RotationMatrixDoubleEuler4D(rotation, rotationW); }

		void setTransformationMatrix()
		{
			setRotationMatrix();
			glm::mat4 _size = glm::mat4(1.0f);
			_size[0][0] = this->scale.x;
			_size[1][1] = this->scale.y;
			_size[2][2] = this->scale.z;
			_size[3][3] = this->scale.w;
			transformationMatrix = (glm::mat4)rotationMatrix * _size;
			positionTransformed = transformationMatrix * (glm::vec4)position;

		}

		void setTransformedPosition() { positionTransformed = transformationMatrix * (glm::vec4)position; }

		std::string toString();
	};

	class GameObject {
		public:
			Transform transform = Transform(this);
			std::string name;

			Engine* engine;

			GameObject() = delete;
			GameObject(Engine* _engine);
			~GameObject();

			GameObject(const GameObject&) = delete;
			GameObject& operator=(const GameObject&) = delete;
			/*GameObject(GameObject&&) = delete;
			GameObject& operator=(GameObject&&) = delete;*/

			template<typename T>
			T* AddComponent();

			GameObject* AddChild();
			GameObject* AddChild(GameObject* child);

			void RemoveChild(int index);

			GameObject* GetChild(int index);
			//GameObject* GetChild(std::string name);


			template<typename T>
			T GetComponent();

			template<typename T>
			std::vector<T> GetComponents();

		protected:

			int id;

			std::vector<Component*> components;
			std::vector<GameObject*> children;

			void Update();
			void FixedUpdate();
			void LateUpdate();

			friend class Engine;
	};

	export class Component
	{
		public:
		GameObject* gameObject;
		Transform* transform;

		std::string name = "Component";

		Component() = delete;
		Component(GameObject* gameObject) { this->gameObject = gameObject; this->transform = &(gameObject->transform); }

		virtual std::string toString();

		/*Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;*/
	};

	export class MonoBehavior : public Component
	{
		public:
			virtual void Start() {}
			virtual void Update() {}
			virtual void FixedUpdate() {}
			virtual void LateUpdate() {}

			virtual void onInitialization() {}

			MonoBehavior() = delete;
			MonoBehavior(GameObject* gameObject);
			
			//std::vector<std::type_identity_t> RequiredComponents;
		
	};

	class Engine
	{
		public:
		Engine();
		~Engine();

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;
		/*Engine(Engine&&) = delete;
					Engine& operator=(Engine&&) = delete;*/

		rManager* renderer;
		TimeClass* Time;

		std::vector<GameObject*> gameObjects;
		GameObject* root;

		std::queue <int> freeIDs;

		bool sceneChanged = false;
		int instructionCount = 0;
		std::vector<InstructionData> instructions;

		//std::vector<Material*> materials;
		//int materialCount = 0;

		void Initialize();

		void UpdateInstructions();

		void Update();
		void FixedUpdate();
		void LateUpdate();

		int AddGameObject(GameObject* gameObject);
		//Material* AddMaterial();

		GameObject* currentGameObject;
		
		void iterateNextGameObject();
		void iteratePreviousGameObject();

		//void LastGameObject();
		void FirstGameObject();

		void iterateGameObjects(int n);

		bool isLastGameObject();

		private:
			int currentGameObjectIndex = 0;
	};

	export class ObjectMetaData : public Component
	{
		public:
		//std::string name;
		std::string tag;
		std::string layer;

		ObjectMetaData() = delete;
		ObjectMetaData(GameObject* gameObject);
		ObjectMetaData(GameObject* gameObject, /*std::string name,*/ std::string tag, std::string layer);
	};


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
			return nullptr;
		}
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

}