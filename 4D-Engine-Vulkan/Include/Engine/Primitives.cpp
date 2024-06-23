module;

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		if (length() == 0) throw "Cannot Normalize Vector4 with Length of 0";
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

	/*bool Vector2::operator==(const Vector2& other)
	{
		bool result = true;
		if (x != other.x) result = false;
		if (y != other.y) result = false;
		return result;
	}*/

	std::ostream& operator<<(std::ostream& os, const Vector2& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ")";
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
		if (length() == 0) throw "Cannot Normalize Vector4 with Length of 0";
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

	/*bool Vector3::operator==(const Vector3& other)
	{
		bool result = true;
		if (x != other.x) result = false;
		if (y != other.y) result = false;
		if (z != other.z) result = false;
		return result;
	}*/

	std::ostream& operator<<(std::ostream& os, const Vector3& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
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
		if (length () == 0) throw "Cannot Normalize Vector4 with Length of 0";
		return *this / length();
	}

	float Vector4::minValue()
	{
		float min = x;
		if (y < min) min = y;
		if (z < min) min = z;
		if (w < min) min = w;
		return min;
	}

	float Vector4::maxValue()
	{
		float max = x;
		if (y > max) max = y;
		if (z > max) max = z;
		if (w > max) max = w;
		return max;
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

	Vector4 Vector4::operator*(const Matrix& other) const
	{
		if (other.rows != 4 || other.columns != 4) throw "Invalid Matrix";
		Vector4 result = Vector4(0, 0, 0, 0);
		float* vectorValues = new float[4];
		vectorValues[0] = x;
		vectorValues[1] = y;
		vectorValues[2] = z;
		vectorValues[3] = w;
		for (int i = 0; i < other.columns; i++) result.x += vectorValues[i] * other(0, i);
		for (int i = 0; i < other.columns; i++) result.y += vectorValues[i] * other(1, i);
		for (int i = 0; i < other.columns; i++) result.z += vectorValues[i] * other(2, i);
		for (int i = 0; i < other.columns; i++) result.w += vectorValues[i] * other(3, i);
		delete[] vectorValues;
		return result;
	}

	std::ostream& operator<<(std::ostream& os, const Vector4& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
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

	Vector4 Matrix::operator*(const Vector4& other) const
	{
		if (rows != 4 || columns != 4) throw "Invalid Matrix";
		float* vectorValues = new float[4];
		vectorValues[0] = other.x;
		vectorValues[1] = other.y;
		vectorValues[2] = other.z;
		vectorValues[3] = other.w;
		Vector4 result = Vector4(0, 0, 0, 0);
		for (int i = 0; i < rows; i++) result.x += vectorValues[i] * (*this)(i, 0);
		for (int i = 0; i < rows; i++) result.y += vectorValues[i] * (*this)(i, 1);
		for (int i = 0; i < rows; i++) result.z += vectorValues[i] * (*this)(i, 2);
		for (int i = 0; i < rows; i++) result.w += vectorValues[i] * (*this)(i, 3);
		delete[] vectorValues;
		return result;
	}

	Matrix Matrix::operator*(const Matrix& other) const
	{
		//if (rows != 4 || columns != 4) throw "Invalid Matrix";
		Matrix result = Matrix(rows, other.columns);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < other.columns; j++)
			{
				float sum = 0;
				for (int k = 0; k < columns; k++)
				{
					sum += (*this)(i, k) * other(k, j);
				}
				result(i, j) = sum;
			}
		}

		return result;
	}

	std::ostream& operator<<(std::ostream& Str, Matrix const& v)
	{
		Str << "[";
		for (int i = 0; i < v.rows; i++)
		{
			Str << "[";
			for (int j = 0; j < v.columns; j++)
			{
				Str << v(i, j);
				if (j != v.columns - 1) Str << ", ";
			}
			Str << "]";
			if (i != v.rows - 1) Str << ", ";
		}
		Str << "]";
		return Str;
	}

	Shape::Shape()
	{
		this->shapeType = 0;
		this->position = Vector4(0);
		this->rotation = Vector3(0);
		this->rotationMatrix = Matrix::Identity(4);
		this->scale = Vector4(1);
	}

	Shape::Shape(Vector4 position, Vector3 rotation, Vector4 scale)
	{
		this->shapeType = 0;
		this->position = position;
		this->rotation = rotation;
		this->rotationMatrix = Matrix::RotationMatrixEuler(rotation);
		this->scale = scale;
	}

	Shape::Shape(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale)
	{
		this->shapeType = 0;
		this->position = position;
		this->rotation = rotation;
		this->rotationW = rotationW;
		this->rotationMatrix = Matrix::RotationMatrixDoubleEuler4D(rotation, rotationW);
		this->scale = scale;
	}

	Mesh::Mesh()
	{
		this->shapes = std::vector<Shape*>();
	}

	Mesh::Mesh(std::vector<Shape*> shapes)
	{
		this->shapes = shapes;
	}

	Mesh::~Mesh()
	{
		for (int i = 0; i < this->shapes.size(); i++)
		{
			delete this->shapes[i];
		}
	}

	void Mesh::AddShape(Shape* shape)
	{
		this->shapes.push_back(shape);
	}

	float Mesh::SDF(Vector4 point)
	{
		if (shapes.size() <= 0) throw "No Shapes in Mesh";
		float result = shapes[0]->SDF(point);
		for (int i = 1; i < shapes.size(); i++)
		{
			result = std::fmin(result, shapes[i]->SDF(point));
		}
		return result;
	}

	HyperSphere::HyperSphere() : Shape()
	{
		this->shapeType = 1;
		setTransformationMatrix4D();
	}

	HyperSphere::HyperSphere(Vector4 position, Vector3 rotation, Vector4 scale) : Shape(position, rotation, scale)
	{
		this->shapeType = 1;
		setTransformationMatrix4D();
	}

	HyperSphere::HyperSphere(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale) : Shape(position, rotation, rotationW, scale)
	{
		this->shapeType = 1;
		setTransformationMatrix4D();
	}

	float HyperSphere::SDF(Vector4 point)
	{
		Vector4 _point = Matrix::RotationMatrixDoubleEuler4D(rotation, rotationW) * point;
		_point /= scale;
		_point -= position;
		float result = _point.length() - radius;
		float sF = scale.maxValue();
		return result * sF;
	}

	Instruction HyperSphere::getInstruction()
	{
		Instruction instruction = Instruction();
		instruction.type = shapeType;
		instruction.valueA = transformationMatrix;
		instruction.valueB = (glm::vec4)position;
		return instruction;
	}

	Tesseract::Tesseract() : Shape()
	{
		this->shapeType = 2;
		setTransformationMatrix4D();
	}

	Tesseract::Tesseract(Vector4 position, Vector3 rotation, Vector4 scale) : Shape(position, rotation, scale)
	{
		this->shapeType = 2;
		setTransformationMatrix4D();
	}

	Tesseract::Tesseract(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale) : Shape(position, rotation, rotationW, scale)
	{
		this->shapeType = 2;
		setTransformationMatrix4D();
	}

	float Tesseract::SDF(Vector4 point)
	{
		Vector4 _point = Matrix::RotationMatrixDoubleEuler4D(rotation, rotationW) * point;
		_point /= scale;
		_point -= position;
		Vector4 q = _point.abs() - size;
		float result = std::fmin(std::fmax(q.x, std::fmax(q.y, std::fmax(q.z, q.w))), 0.) + max(q, Vector4(0)).length();
		float sF = scale.maxValue();
		return result * sF;
	}

	Instruction Tesseract::getInstruction()
	{
		Instruction instruction = Instruction();
		instruction.type = shapeType;
		instruction.valueA = transformationMatrix;
		instruction.valueB = (glm::vec4)position;
		return instruction;
	}

	HyperPlane::HyperPlane() : Shape()
	{
		this->shapeType = 3;
		setTransformationMatrix4D();
	}

	HyperPlane::HyperPlane(Vector4 position, Vector3 rotation) : Shape(position, rotation, Vector4(1))
	{
		this->shapeType = 3;
		setTransformationMatrix4D();
	}

	HyperPlane::HyperPlane(Vector4 position, Vector3 rotation, Vector3 rotationW) : Shape(position, rotation, rotationW, Vector4(1))
	{
		this->shapeType = 3;
		setTransformationMatrix4D();
	}

	HyperPlane::HyperPlane(Vector4 position, Vector4 normal) : Shape(position, normal.xyz(), Vector3(normal.w, Vector2(0)), Vector4(1))
	{
		this->shapeType = 3;
		setTransformationMatrix4D();
	}

	float HyperPlane::SDF(Vector4 point)
	{
		Vector4 _point = Matrix::RotationMatrixDoubleEuler4D(rotation, rotationW) * point;
		_point /= scale;
		_point -= position;
		float result = dot(_point, normal) - position.y;
		float sF = scale.maxValue();
		return result * sF;
	}

	Instruction HyperPlane::getInstruction()
	{
		Instruction instruction = Instruction();
		instruction.type = shapeType;
		instruction.valueA = transformationMatrix;
		instruction.valueB = (glm::vec4)position;
		float x = instruction.valueB.x;
		instruction.valueB.x = instruction.valueB.y;
		instruction.valueB.y = x;
		return instruction;
	}

}