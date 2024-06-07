module;

export module Engine4D.Material;

import Engine4D.Primitives;

namespace Engine4D
{

	export constexpr int MAX_MATERIAL_COUNT = 128;

	export class Material
	{
		public:
		Vector4 color;

		int index;

		Material();
		Material(int _index);
		Material(Vector4 color);

		//Vector4 getColor();


	};

	Material::Material()
	{
		this->color = Vector4(0, 0, 0, 1);
	}

	Material::Material(int _index)
	{
		this->color = Vector4(0, 0, 0, 1);
		this->index = _index;
	}

	Material::Material(Vector4 color)
	{
		this->color = color;
	}
}