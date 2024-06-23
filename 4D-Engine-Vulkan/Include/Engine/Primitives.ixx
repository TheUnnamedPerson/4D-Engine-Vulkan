module;

#include <iostream>
#include <glm/glm.hpp>

#include <vector>

export module Engine4D.Primitives;

import Engine4D.Structs;

namespace Engine4D {

	export constexpr float PI = 3.14159265359f;
	export constexpr float epsilon = 0.000001f;
	export constexpr float epsilon_squared = epsilon * epsilon;

	export class Matrix;

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

		//bool operator==(const Vector2& other);

		friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);

		explicit operator glm::vec2() const { return glm::vec2(x, y); }
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
		Vector3(glm::vec3 vec);
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

		//bool operator==(const Vector3& other);

		friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);

		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }
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
		float minValue();
		float maxValue();

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

		//friend bool operator==(const Vector4& base, const Vector4& other);

		Vector4 operator*(const Matrix& other) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);

		explicit operator glm::vec4() const { return glm::vec4(x, y, z, w); };
	};

	export Vector4 max(Vector4 a, Vector4 b);
	export Vector4 min(Vector4 a, Vector4 b);

	export float distance(Vector4 a, Vector4 b);

	export float dot(Vector4 a, Vector4 b);
	export Vector4 cross(Vector4 a, Vector4 b);

	class Matrix
	{

		public:

		enum RotationMatrix
		{
			Flat = 1,
			X = 2,
			Y = 3,
			Z = 4,
			ZW = 5,
			YW = 6,
			YZ = 7,
			XW = 8,
			XZ = 9,
			XY = 10
		};

		float* matrix;
		int rows, columns;

		Matrix()
		{
			rows = 4;
			columns = 4;
			matrix = new float[16];
		}
		Matrix(int _columns, int _rows)
		{
			rows = _rows;
			columns = _columns;
			matrix = new float[_rows * _columns];
		}

		~Matrix()
		{
			delete[] matrix;
		}

		Matrix(Matrix const& other) {
			this->rows = other.rows;
			this->columns = other.columns;
			this->matrix = new float[this->rows * this->columns];
			(void*)memcpy_s(this->matrix, this->rows * this->columns * sizeof(float), other.matrix, other.rows * other.columns * sizeof(float));
		}

		Matrix(glm::mat4 mat)
		{
			rows = 4;
			columns = 4;
			matrix = new float[16];
			for (int i = 0; i < 4; i++)
			{
				for (int ii = 0; ii < 4; ii++)
				{
					matrix[i * 4 + ii] = mat[i][ii];
				}
			}
		}

		Matrix& operator=(Matrix const& other) {

			delete[] this->matrix;

			this->rows = other.rows;
			this->columns = other.columns;
			this->matrix = new float[this->rows * this->columns];

			(void*)memcpy_s(this->matrix, this->rows * this->columns * sizeof(float), other.matrix, other.rows * other.columns * sizeof(float));

			return *this;
		}

		float& operator ()(int x, int y) {
			return matrix[y * columns + x];
		}

		float operator ()(int x, int y) const {
			return matrix[y * columns + x];
		}

		static Matrix Zero(int _columns, int _rows)
		{
			Matrix result(_columns, _rows);
			for (int i = 0; i < _rows; i++)
			{
				for (int j = 0; j < _columns; j++)
				{
					result.matrix[i * _columns + j] = 0;
				}
			}
			return result;
		}

		static Matrix One(int _columns, int _rows)
		{
			Matrix result(_columns, _rows);
			for (int i = 0; i < _rows; i++)
			{
				for (int j = 0; j < _columns; j++)
				{
					result.matrix[i * _columns + j] = 1;
				}
			}
			return result;
		}

		static Matrix Diagonal(int length, float value)
		{
			Matrix result(length, length);
			for (int i = 0; i < length; i++)
			{
				for (int j = 0; j < length; j++)
				{
					if (i == j)
					{
						result.matrix[i * length + j] = value;
					}
					else
					{
						result.matrix[i * length + j] = 0;
					}
				}
			}
			return result;
		}

		static Matrix Identity(int length)
		{
			return Diagonal(length, 1);
		}

		static Matrix RotationMatrix(RotationMatrix matrix, float theta)
		{
			int n = 0;
			switch (matrix)
			{
				case RotationMatrix::Flat:
					n = 2;
					break;
				case RotationMatrix::X:
				case RotationMatrix::Y:
				case RotationMatrix::Z:
					n = 4;
					break;
				case RotationMatrix::ZW:
				case RotationMatrix::YW:
				case RotationMatrix::YZ:
				case RotationMatrix::XW:
				case RotationMatrix::XZ:
				case RotationMatrix::XY:
					n = 4;
					break;
				default:
					throw "Invalid Rotation Matrix";
					break;
			}
			Matrix result = Zero(n, n);

			switch (matrix)
			{
				case RotationMatrix::Flat:
					result(0, 0) = cos(theta); result(0, 1) = -sin(theta);
					result(1, 0) = sin(theta); result(1, 1) = cos(theta);
					break;
				case RotationMatrix::X:
					result(0, 0) = 1;
					result(1, 1) = cos(theta); result(1, 2) = -sin(theta);
					result(2, 1) = sin(theta); result(2, 2) = cos(theta);
					result(3, 3) = 1;
					break;
				case RotationMatrix::Y:
					result(0, 0) = cos(theta); result(0, 2) = sin(theta);
					result(1, 1) = 1;
					result(2, 0) = -sin(theta); result(2, 2) = cos(theta);
					result(3, 3) = 1;
					break;
				case RotationMatrix::Z:
					result(0, 0) = cos(theta); result(0, 1) = -sin(theta);
					result(1, 0) = sin(theta); result(1, 1) = cos(theta);
					result(2, 2) = 1;
					result(3, 3) = 1;
					break;
				case RotationMatrix::ZW:
					result(0, 0) = cos(theta); result(0, 1) = -sin(theta);
					result(1, 0) = sin(theta); result(1, 1) = cos(theta);
					result(2, 2) = 1;
					result(3, 3) = 1;
					break;
				case RotationMatrix::YW:
					result(0, 0) = cos(theta); result(0, 2) = -sin(theta);
					result(1, 1) = 1;
					result(2, 0) = sin(theta); result(2, 2) = cos(theta);
					result(3, 3) = 1;
					break;
				case RotationMatrix::YZ:
					result(0, 0) = cos(theta); result(0, 3) = -sin(theta);
					result(1, 1) = 1;
					result(2, 2) = 1;
					result(3, 0) = sin(theta); result(3, 3) = cos(theta);
					break;
				case RotationMatrix::XW:
					result(0, 0) = 1;
					result(1, 1) = cos(theta); result(1, 2) = -sin(theta);
					result(2, 1) = sin(theta); result(2, 2) = cos(theta);
					result(3, 3) = 1;
					break;
				case RotationMatrix::XZ:
					result(0, 0) = 1;
					result(1, 1) = cos(theta); result(1, 3) = -sin(theta);
					result(2, 2) = 1;
					result(3, 1) = sin(theta); result(3, 3) = cos(theta);
					break;
				case RotationMatrix::XY:
					result(0, 0) = 1;
					result(1, 1) = 1;
					result(2, 2) = cos(theta); result(2, 3) = -sin(theta);
					result(3, 2) = sin(theta); result(3, 3) = cos(theta);
					break;
				default:
					throw "Invalid Rotation Matrix";
					break;
			}

			//round all values to nearest 10000th to avoid floating point errors
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					result(i, j) = round(result(i, j) * 10000) / 10000;
				}
			}

			return result;
		}

		static Matrix RotationMatrixEuler(Vector3 EulerAngles)
		{
			return RotationMatrix(RotationMatrix::X, EulerAngles.x) * RotationMatrix(RotationMatrix::Y, EulerAngles.y) * RotationMatrix(RotationMatrix::Z, EulerAngles.z);
		}

		static Matrix RotationMatrixEuler4D(Vector4 EulerAngles)
		{
			return RotationMatrix(RotationMatrix::XW, EulerAngles.x) * RotationMatrix(RotationMatrix::YW, EulerAngles.y) * RotationMatrix(RotationMatrix::ZW, EulerAngles.z) * RotationMatrix(RotationMatrix::XY, EulerAngles.w);
		}

		static Matrix RotationMatrixDoubleEuler4D(Vector3 RotationAngles, Vector3 RotationAnglesW);

		Vector4 operator*(const Vector4& other) const;
		Matrix operator*(const Matrix& other) const;
		friend std::ostream& operator<<(std::ostream& Str, Matrix const& v);

		explicit operator glm::mat4() const { glm::mat4 result; for (int i = 0; i < 4; i++) for (int ii = 0; ii < 4; ii++) result[i][ii] = (*this)(i, ii); return result; };
	};

	export class Shape
	{
		public:
		int shapeType;
		Vector4 position;
		Vector3 rotation;
		Vector3 rotationW;
		Vector4 scale;

		Matrix rotationMatrix;
		glm::mat4 transformationMatrix;
		glm::vec4 positionTransformed;


		Shape();
		Shape(Vector4 position, Vector3 rotation, Vector4 scale);
		Shape(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale);

		void setRotationMatrix() { rotationMatrix = Matrix::RotationMatrixEuler(rotation); }
		void setRotationMatrix4D() { rotationMatrix = Matrix::RotationMatrixDoubleEuler4D(rotation, rotationW); }

		void setTransformationMatrix()
		{
			setRotationMatrix();
			glm::mat4 _size = glm::mat4(1.0f);
			_size[0][0] = this->scale.x;
			_size[1][1] = this->scale.y;
			_size[2][2] = this->scale.z;
			_size[3][3] = 1;
			transformationMatrix = (glm::mat4)rotationMatrix * _size;
			positionTransformed = transformationMatrix * (glm::vec4)position;
		}

		void setTransformationMatrix4D()
		{
			setRotationMatrix4D();
			glm::mat4 _size = glm::mat4(1.0f);
			_size[0][0] = this->scale.x;
			_size[1][1] = this->scale.y;
			_size[2][2] = this->scale.z;
			_size[3][3] = this->scale.w;
			transformationMatrix = (glm::mat4)rotationMatrix * _size;
			positionTransformed = transformationMatrix * (glm::vec4)position;

		}

		virtual Instruction getInstruction() = 0;

		virtual float SDF(Vector4 point) = 0;
	};

	export class Mesh
	{
		public:
		std::vector<Shape*> shapes;

		Mesh();
		Mesh(std::vector<Shape> shapes);
		Mesh(std::vector<Shape*> shapes);

		~Mesh();

		void AddShape(Shape* shape);

		float SDF(Vector4 point);
	};

	export class HyperSphere : public Shape
	{
		public:
		float radius = 1;

		HyperSphere();
		HyperSphere(Vector4 position, Vector3 rotation, Vector4 scale);
		HyperSphere(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale);

		Instruction getInstruction() override;

		float SDF(Vector4 point) override;
	};

	export class Tesseract : public Shape
	{
		public:
		Vector4 size = Vector4(1, 1, 1, 1);

		Tesseract();
		Tesseract(Vector4 position, Vector3 rotation, Vector4 scale);
		Tesseract(Vector4 position, Vector3 rotation, Vector3 rotationW, Vector4 scale);

		Instruction getInstruction() override;

		float SDF(Vector4 point) override;
	};

	export class HyperPlane : public Shape
	{
		public:
		Vector4 normal = Vector4(0, 1, 0, 0);

		HyperPlane();
		HyperPlane(Vector4 position, Vector3 rotation);
		HyperPlane(Vector4 position, Vector3 rotation, Vector3 rotationW);
		HyperPlane(Vector4 position, Vector4 normal);

		Instruction getInstruction() override;

		float SDF(Vector4 point) override;
	};

}

namespace Engine4D
{
	/*export inline Vector2 operator+(const float& base, const Vector2& other)
	{
		Vector2 result = other;
		result += base;
		return result;
	}*/
	export inline Vector2 operator-(const Vector2& other)
	{
		Vector2 result = other;
		result *= -1;
		return result;
	}
	/*export inline Vector2 operator*(const float& base, const Vector2& other)
	{
		Vector2 result = other;
		result *= base;
		return result;
	}*/

	/*export inline Vector3 operator+(const float& base, const Vector3& other)
	{
		Vector3 result = other;
		result += base;
		return result;
	}*/
	export inline Vector3 operator-(const Vector3& other)
	{
		Vector3 result = other;
		result *= -1;
		return result;
	}
	/*export inline Vector3 operator*(const float& base, const Vector3& other)
	{
		Vector3 result = other;
		result *= base;
		return result;
	}*/

	/*export inline Vector4 operator+(const float& base, const Vector4& other)
	{
		Vector4 result = other;
		result += base;
		return result;
	}*/
	export inline Vector4 operator-(const Vector4& other)
	{
		Vector4 result = other;
		result *= -1;
		return result;
	}

	export bool operator==(const Vector4& base, const Vector4& other)
	{
		bool result = true;
		if (base.x != other.x) result = false;
		if (base.y != other.y) result = false;
		if (base.z != other.z) result = false;
		if (base.w != other.w) result = false;
		return result;
	}
	/*export inline Vector4 operator*(const float& base, const Vector4& other)
	{
		Vector4 result = other;
		result *= base;
		return result;
	}*/

	Matrix Matrix::RotationMatrixDoubleEuler4D(Vector3 RotationAngles, Vector3 RotationAnglesW)
	{
		Matrix xwMatrix = RotationMatrix(RotationMatrix::XW, RotationAngles.x);
		Matrix ywMatrix = RotationMatrix(RotationMatrix::YW, RotationAngles.y);
		Matrix zwMatrix = RotationMatrix(RotationMatrix::ZW, RotationAngles.z);
		Matrix xyMatrix = RotationMatrix(RotationMatrix::XY, RotationAnglesW.x);
		Matrix xzMatrix = RotationMatrix(RotationMatrix::XZ, RotationAnglesW.y);
		Matrix yzMatrix = RotationMatrix(RotationMatrix::YZ, RotationAnglesW.z);

		return xwMatrix * ywMatrix * zwMatrix * xyMatrix * xzMatrix * yzMatrix;
	}

}