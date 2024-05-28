module;

#include <vector>
#include <string>

export module Engine4D.Components;

import Engine4D.Engine;
import Engine4D.Primitives;
import Engine4D.Structs;

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

	export class Material
	{
		public:
			Vector4 color;

			Material();
			Material(Vector4 color);

			//Vector4 getColor();


	};

	export class MeshRenderer : public MonoBehavior
	{
		public:
			Mesh mesh;
			Material material;

			MeshRenderer() = delete;
			MeshRenderer(GameObject* gameObject);
			MeshRenderer(GameObject* gameObject, Mesh mesh, Material material);

			std::vector<Instruction> getInstructions();

			void AddShape(Shape* shape);

			std::string toString() override;

	};
}