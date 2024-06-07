module;

#include <vector>
#include <string>

export module Engine4D.Components;

import Engine4D.Engine;
import Engine4D.Primitives;
import Engine4D.Structs;
import Engine4D.Material;

namespace Engine4D
{
	export class Mesh
	{
		public:
			std::vector<Shape*> shapes;

			Mesh();
			Mesh(std::vector<Shape> shapes);
			Mesh(std::vector<Shape*> shapes);

			~Mesh();

			void AddShape(Shape* shape);
	};

	export class MeshRenderer : public MonoBehavior
	{
		public:
			Mesh mesh;
			Material* material;

			MeshRenderer() = delete;
			MeshRenderer(GameObject* gameObject);
			MeshRenderer(GameObject* gameObject, Material* material);
			MeshRenderer(GameObject* gameObject, Mesh mesh, Material* material);

			std::vector<InstructionData> getInstructions();

			void AddShape(Shape* shape);

			std::string toString() override;

	};

	export class RigidBody : public MonoBehavior
	{
		public:
		Vector4 gravity;
		Vector4 velocity;
		Vector3 rotationalVelocity;
		Vector3 rotationalVelocityW;

		float countdowntime = 0.0f;

		RigidBody() = delete;
		RigidBody(GameObject* gameObject);

		void Update() override;

		std::string toString() override;
	};
}