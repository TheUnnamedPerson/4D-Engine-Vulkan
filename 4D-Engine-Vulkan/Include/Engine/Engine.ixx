module;

#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <string>

export module Engine4D.Engine;

import Engine4D.Primitives;
import Engine4D.Time;
import Engine4D.Structs;

import Engine4D.Renderer;

namespace Engine4D {

	export class GameObject;
	export class Component;
	export class Engine;

	export class Transform
	{
		public:
			Vector4 position;
			Vector4 rotation;
			Vector4 scale;

			GameObject* gameObject;

			Transform() = delete;
			Transform(GameObject* gameObject);
			Transform(Vector4 position, Vector4 rotation, Vector4 scale, GameObject* gameObject);

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

			GameObject* GetChild(int index);
			//GameObject* GetChild(std::string name);


			template<typename T>
			T GetComponent();

			template<typename T>
			std::vector<T> GetComponents();

		protected:
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

		rRenderer* renderer;

		std::vector<GameObject*> gameObjects;
		GameObject* root;

		bool sceneChanged = false;
		std::vector<Instruction> instructions;

		void Initialize();

		void UpdateInstructions();

		void Update();
		void FixedUpdate();
		void LateUpdate();

		void AddGameObject(GameObject* gameObject);
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