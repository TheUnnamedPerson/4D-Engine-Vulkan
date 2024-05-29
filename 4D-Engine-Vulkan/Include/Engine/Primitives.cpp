module;

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>

module Engine4D.Primitives;

namespace Engine4D
{
	Vector2::Vector2() : x(0), y(0) {}

	Vector2::Vector2(float x, float y) : x(x), y(y) {}

	Vector2::Vector2(float xy) : x(xy), y(xy) {}

	Vector2 Vector2::abs()
	{
		return Vector2(std::fabs(x), std::fabs(y));
	}

	float Vector2::length()
	{
		return std::sqrtf(x * x + y * y);
	}

	Vector2 Vector2::normalized()
	{
		return *this / length();
	}

	Vector2 Vector2::operator+(const Vector2& other)
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator-(const Vector2& other)
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator*(const Vector2& other)
	{
		return Vector2(x * other.x, y * other.y);
	}

	Vector2 Vector2::operator/(const Vector2& other)
	{
		return Vector2(x / other.x, y / other.y);
	}

	Vector2 Vector2::operator+(const float& other)
	{
		return Vector2(x + other, y + other);
	}

	Vector2 Vector2::operator-(const float& other)
	{
		return Vector2(x - other, y - other);
	}

	Vector2 Vector2::operator*(const float& other)
	{
		return Vector2(x * other, y * other);
	}

	Vector2 Vector2::operator/(const float& other)
	{
		return Vector2(x / other, y / other);
	}

	Vector2 Vector2::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2 Vector2::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2 Vector2::operator*=(const Vector2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vector2 Vector2::operator/=(const Vector2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vector2 Vector2::operator+=(const float& other)
	{
		x += other;
		y += other;
		return *this;
	}

	Vector2 Vector2::operator-=(const float& other)
	{
		x -= other;
		y -= other;
		return *this;
	}

	Vector2 Vector2::operator*=(const float& other)
	{
		x *= other;
		y *= other;
		return *this;
	}

	Vector2 Vector2::operator/=(const float& other)
	{
		x /= other;
		y /= other;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& vec)
	{
		os << "Vector2(" << vec.x << ", " << vec.y << ")";
		return os;
	}

	Vector2 max(Vector2 a, Vector2 b)
	{
		return Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
	}

	Vector2 min(Vector2 a, Vector2 b)
	{
		return Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
	}

	float distance(Vector2 a, Vector2 b)
	{
		return (a - b).length();
	}

	float dot(Vector2 a, Vector2 b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float cross(Vector2 a, Vector2 b)
	{
		return cross(Vector3(a, 0), Vector3(b, 0)).xy().length();
	}

	Vector3::Vector3() : x(0), y(0), z(0) {}

	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3::Vector3(Vector2 xy, float z) : x(xy.x), y(xy.y), z(z) {}

	Vector3::Vector3(float x, Vector2 yz) : x(x), y(yz.x), z(yz.y) {}

	Vector3::Vector3(float xyz) : x(xyz), y(xyz), z(xyz) {}

	Vector3 Vector3::abs()
	{
		return Vector3(std::fabs(x), std::fabs(y), std::fabs(z));
	}

	float Vector3::length()
	{
		return std::sqrtf(x * x + y * y + z * z);
	}

	Vector3 Vector3::normalized()
	{
		return *this / length();
	}

	Vector2 Vector3::xy()
	{
		return Vector2(x, y);
	}

	Vector2 Vector3::xz()
	{
		return Vector2(x, z);
	}

	Vector2 Vector3::yz()
	{
		return Vector2(y, z);
	}

	Vector3 Vector3::operator+(const Vector3& other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 Vector3::operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 Vector3::operator*(const Vector3& other)
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	Vector3 Vector3::operator/(const Vector3& other)
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	Vector3 Vector3::operator+(const float& other)
	{
		return Vector3(x + other, y + other, z + other);
	}

	Vector3 Vector3::operator-(const float& other)
	{
		return Vector3(x - other, y - other, z - other);
	}

	Vector3 Vector3::operator*(const float& other)
	{
		return Vector3(x * other, y * other, z * other);
	}

	Vector3 Vector3::operator/(const float& other)
	{
		return Vector3(x / other, y / other, z / other);
	}

	Vector3 Vector3::operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3 Vector3::operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3 Vector3::operator*=(const Vector3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vector3 Vector3::operator/=(const Vector3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vector3 Vector3::operator+=(const float& other)
	{
		x += other;
		y += other;
		z += other;
		return *this;
	}

	Vector3 Vector3::operator-=(const float& other)
	{
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	Vector3 Vector3::operator*=(const float& other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vector3 Vector3::operator/=(const float& other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector3& vec)
	{
		os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	Vector3 max(Vector3 a, Vector3 b)
	{
		return Vector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}

	Vector3 min(Vector3 a, Vector3 b)
	{
		return Vector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}

	float distance(Vector3 a, Vector3 b)
	{
		return (a - b).length();
	}

	float dot(Vector3 a, Vector3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	Vector3 cross(Vector3 a, Vector3 b)
	{
		return Vector3(a.y * b.z - a.z * b.y, a.x * b.z - a.z * b.x, a.x * b.y - a.y * b.x);
	}

	Vector4::Vector4() : x(0), y(0), z(0), w(0) {}

	Vector4::Vector4(glm::vec4 vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

	Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	Vector4::Vector4(Vector2 xy, float z, float w) : x(xy.x), y(xy.y), z(z), w(w) {}

	Vector4::Vector4(float x, Vector2 yz, float w) : x(x), y(yz.x), z(yz.y), w(w) {}

	Vector4::Vector4(float x, float y, Vector2 zw) : x(x), y(y), z(zw.x), w(zw.y) {}

	Vector4::Vector4(Vector2 xy, Vector2 zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}

	Vector4::Vector4(Vector3 xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

	Vector4::Vector4(float x, Vector3 yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}

	Vector4::Vector4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}

	Vector4::Vector4(Vector2 xy_zw) : x(xy_zw.x), y(xy_zw.y), z(xy_zw.x), w(xy_zw.y) {}

	Vector4 Vector4::abs()
	{
		return Vector4(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
	}

	float Vector4::length()
	{
		return std::sqrtf(x * x + y * y + z * z + w * w);
	}

	Vector4 Vector4::normalized()
	{
		return *this / length();
	}

	Vector2 Vector4::xy()
	{
		return Vector2(x, y);
	}

	Vector2 Vector4::xz()
	{
		return Vector2(x, z);
	}

	Vector2 Vector4::yz()
	{
		return Vector2(y, z);
	}

	Vector3 Vector4::xyz()
	{
		return Vector3(x, y, z);
	}

	Vector3 Vector4::xyw()
	{
		return Vector3(x, y, w);
	}

	Vector3 Vector4::xzw()
	{
		return Vector3(x, z, w);
	}

	Vector3 Vector4::yzw()
	{
		return Vector3(y, z, w);
	}

	Vector4 Vector4::operator+(const Vector4& other)
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 Vector4::operator-(const Vector4& other)
	{
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vector4 Vector4::operator*(const Vector4& other)
	{
		return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	Vector4 Vector4::operator/(const Vector4& other)
	{
		return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
	}

	Vector4 Vector4::operator+(const float& other)
	{
		return Vector4(x + other, y + other, z + other, w + other);
	}

	Vector4 Vector4::operator-(const float& other)
	{
		return Vector4(x - other, y - other, z - other, w - other);
	}

	Vector4 Vector4::operator*(const float& other)
	{
		return Vector4(x * other, y * other, z * other, w * other);
	}

	Vector4 Vector4::operator/(const float& other)
	{
		return Vector4(x / other, y / other, z / other, w / other);
	}

	Vector4 Vector4::operator+=(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	Vector4 Vector4::operator-=(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	Vector4 Vector4::operator*=(const Vector4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	Vector4 Vector4::operator/=(const Vector4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	Vector4 Vector4::operator+=(const float& other)
	{
		x += other;
		y += other;
		z += other;
		w += other;
		return *this;
	}

	Vector4 Vector4::operator-=(const float& other)
	{
		x -= other;
		y -= other;
		z -= other;
		w -= other;
		return *this;
	}

	Vector4 Vector4::operator*=(const float& other)
	{
		x *= other;
		y *= other;
		z *= other;
		w *= other;
		return *this;
	}

	Vector4 Vector4::operator/=(const float& other)
	{
		x /= other;
		y /= other;
		z /= other;
		w /= other;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector4& vec)
	{
		os << "Vector4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}

	Vector4 max(Vector4 a, Vector4 b)
	{
		return Vector4(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
	}

	Vector4 min(Vector4 a, Vector4 b)
	{
		return Vector4(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
	}

	float distance(Vector4 a, Vector4 b)
	{
		return (a - b).length();
	}

	float dot(Vector4 a, Vector4 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	Vector4 cross(Vector4 a, Vector4 b)
	{
		return Vector4(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0);
	}

	Shape::Shape ()
	{
		this->shapeType = 0;
		this->position = Vector4(0);
		this->rotation = Vector4(0);
		this->scale = Vector4(1);
	}

	Shape::Shape (Vector4 position, Vector4 rotation, Vector4 scale)
	{
		this->shapeType = 0;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	HyperSphere::HyperSphere() : Shape()
	{
		this->shapeType = 1;
		this->radius = 1;
	}

	HyperSphere::HyperSphere(Vector4 position, Vector4 rotation, Vector4 scale, float radius) : Shape(position, rotation, scale)
	{
		this->shapeType = 1;
		this->radius = radius;
	}

	float HyperSphere::SDF(Vector4 point)
	{
		return point.length() - radius;
	}

	Instruction HyperSphere::getInstruction()
	{
		Instruction instruction = Instruction();
		instruction.type = shapeType;
		instruction.valueA = glm::vec4(radius, 0, 0, 0);
		instruction.valueB = glm::vec4(0);
		return instruction;
	}

	Tesseract::Tesseract() : Shape()
	{
		this->shapeType = 2;
		this->size = Vector4(1, 1, 1, 1);
	}

	Tesseract::Tesseract(Vector4 position, Vector4 rotation, Vector4 scale, Vector4 size) : Shape(position, rotation, scale)
	{
		this->shapeType = 2;
		this->size = size;
	}

	float Tesseract::SDF(Vector4 point)
	{
		Vector4 q = point.abs() - size;
		return std::fmin(std::fmax(q.x, std::fmax(q.y, std::fmax(q.z, q.w))), 0.) + max(q, Vector4(0)).length();
	}

	Instruction Tesseract::getInstruction()
	{
		Instruction instruction = Instruction();
		instruction.type = shapeType;
		instruction.valueA = glm::vec4(size.x, size.y, size.z, size.w);
		instruction.valueB = glm::vec4(0);
		return instruction;
	}

}