module;

#include <iostream>
#include <glm/glm.hpp>

export module Engine4D.Primitives;

import Engine4D.Structs;

namespace Engine4D {

	export class Vector2
	{
		public:
		float x, y;
		Vector2();
		Vector2(float x, float y);
		Vector2(float xy);

		Vector2 abs();
		float length();
		Vector2 normalized();

		Vector2 operator+(const Vector2& other);
		Vector2 operator-(const Vector2& other);
		Vector2 operator*(const Vector2& other);
		Vector2 operator/(const Vector2& other);

		Vector2 operator+(const float& other);
		Vector2 operator-(const float& other);
		Vector2 operator*(const float& other);
		Vector2 operator/(const float& other);

		Vector2 operator+=(const Vector2& other);
		Vector2 operator-=(const Vector2& other);
		Vector2 operator*=(const Vector2& other);
		Vector2 operator/=(const Vector2& other);

		Vector2 operator+=(const float& other);
		Vector2 operator-=(const float& other);
		Vector2 operator*=(const float& other);
		Vector2 operator/=(const float& other);
		
		friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
	};

	export Vector2 max(Vector2 a, Vector2 b);
	export Vector2 min(Vector2 a, Vector2 b);

	export float distance(Vector2 a, Vector2 b);

	export float dot(Vector2 a, Vector2 b);
	export float cross(Vector2 a, Vector2 b);

	export class Vector3
	{
		public:
		float x, y, z;
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(Vector2 xy, float z);
		Vector3(float x, Vector2 yz);
		Vector3(float xyz);

		Vector3 abs();
		float length();
		Vector3 normalized();

		Vector2 xy();
		Vector2 xz();
		Vector2 yz();

		Vector3 operator+(const Vector3& other);
		Vector3 operator-(const Vector3& other);
		Vector3 operator*(const Vector3& other);
		Vector3 operator/(const Vector3& other);

		Vector3 operator+(const float& other);
		Vector3 operator-(const float& other);
		Vector3 operator*(const float& other);
		Vector3 operator/(const float& other);

		Vector3 operator+=(const Vector3& other);
		Vector3 operator-=(const Vector3& other);
		Vector3 operator*=(const Vector3& other);
		Vector3 operator/=(const Vector3& other);

		Vector3 operator+=(const float& other);
		Vector3 operator-=(const float& other);
		Vector3 operator*=(const float& other);
		Vector3 operator/=(const float& other);

		friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
	};

	export Vector3 max(Vector3 a, Vector3 b);
	export Vector3 min(Vector3 a, Vector3 b);

	export float distance(Vector3 a, Vector3 b);

	export float dot(Vector3 a, Vector3 b);
	export Vector3 cross(Vector3 a, Vector3 b);

	export class Vector4
	{
		public:
		float x, y, z, w;
		Vector4();
		Vector4(glm::vec4 vec);
		Vector4(float x, float y, float z, float w);
		Vector4(Vector2 xy, float z, float w);
		Vector4(float x, Vector2 yz, float w);
		Vector4(float x, float y, Vector2 zw);
		Vector4(Vector2 xy, Vector2 zw);
		Vector4(Vector3 xyz, float w);
		Vector4(float x, Vector3 yzw);
		Vector4(float xyzw);
		Vector4(Vector2 xy_zw);

		Vector4 abs();
		float length();
		Vector4 normalized();

		Vector2 xy();
		Vector2 xz();
		Vector2 xw();
		Vector2 yz();
		Vector2 yw();
		Vector2 zw();

		Vector3 xyz();
		Vector3 xyw();
		Vector3 xzw();
		Vector3 yzw();

		Vector4 operator+(const Vector4& other);
		Vector4 operator-(const Vector4& other);
		Vector4 operator*(const Vector4& other);
		Vector4 operator/(const Vector4& other);

		Vector4 operator+(const float& other);
		Vector4 operator-(const float& other);
		Vector4 operator*(const float& other);
		Vector4 operator/(const float& other);

		Vector4 operator+=(const Vector4& other);
		Vector4 operator-=(const Vector4& other);
		Vector4 operator*=(const Vector4& other);
		Vector4 operator/=(const Vector4& other);

		Vector4 operator+=(const float& other);
		Vector4 operator-=(const float& other);
		Vector4 operator*=(const float& other);
		Vector4 operator/=(const float& other);

		friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);

		explicit operator glm::vec4() const { return glm::vec4(x, y, z, w); };
	};

	export Vector4 max(Vector4 a, Vector4 b);
	export Vector4 min(Vector4 a, Vector4 b);

	export float distance(Vector4 a, Vector4 b);

	export float dot(Vector4 a, Vector4 b);
	export Vector4 cross(Vector4 a, Vector4 b);

	export class Shape
	{
		public:
			int shapeType;
			Vector4 position;
			Vector4 rotation;
			Vector4 scale;

			Shape();
			Shape(Vector4 position, Vector4 rotation, Vector4 scale);

			virtual Instruction getInstruction() = 0;

			virtual float SDF(Vector4 point) = 0;
	};

	export class HyperSphere : public Shape
	{
		public:
			float radius;

			HyperSphere();
			HyperSphere(Vector4 position, Vector4 rotation, Vector4 scale, float radius);

			Instruction getInstruction() override;

			float SDF(Vector4 point) override;
	};

	export class Tesseract : public Shape
	{
		public:
			Vector4 size;

			Tesseract();
			Tesseract(Vector4 position, Vector4 rotation, Vector4 scale, Vector4 size);

			Instruction getInstruction() override;

			float SDF(Vector4 point) override;
	};
	
}