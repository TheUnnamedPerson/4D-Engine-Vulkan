module;

#include <glm/glm.hpp>

export module Engine4D.Material;

import Engine4D.Primitives;

namespace Engine4D
{

	export constexpr int MAX_MATERIAL_COUNT = 128;

	export class Material
	{
		public:
		Vector4 diffuse;

		int index;

		Material();
		Material(int _index);
		Material(Vector4 color);

		//Vector4 getColor();


	};

	export struct MaterialData
	{
		alignas(16) glm::vec4 diffuse;
	};

	export MaterialData MaterialToData(Material* material)
	{
		MaterialData data;
		data.diffuse = (glm::vec4)material->diffuse;
		return data;
	}

	Material::Material()
	{
		this->diffuse = Vector4(0, 0, 0, 1);
	}

	Material::Material(int _index)
	{
		this->diffuse = Vector4(0, 0, 0, 1);
		this->index = _index;
	}

	Material::Material(Vector4 color)
	{
		this->diffuse = color;
	}
}