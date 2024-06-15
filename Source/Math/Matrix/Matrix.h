#pragma once

#include "../Number.h"
#include "../Vector.h"
#include <iostream>
#include <format>

#define ScalarOperator(op) for(int i = 0; i < SIZE; i++)\
{\
		elements[i] op= scalar;\
}\
return *this;

template<typename T, size_t R, size_t C> requires number<T>
struct Matrix
{
	const size_t SIZE = C * R;

	Matrix<T, R, C>()
	{
		Empty();
	}

	Matrix<T, R, C>(const T elements[C * R])
	{
		for(int i = 0; i < SIZE; i++)
		{
			this->elements[i] = elements[i];
		}
	}

	Matrix<T, R, C>(const std::initializer_list<T>& list)
	{
		if(list.size() != SIZE)
		{
			std::cerr << std::format("Initializer list is wrong size. It is {} when it should be {}", list.size(), SIZE) << std::endl;
			throw std::out_of_range("initializer_list_size");
		}

		//C++ forces you to use a for each >_<
		int index = 0;
		for(auto& element : list)
		{
			elements[index++] = element;
		}
	}

	Matrix<T, R, C>(Vec2<T> vec)
	{
		static_assert(R == 2, "Wrong dimensions specified. Must contain 2 rows to create matrix from 2d vector.");

		Empty();

		elements[0] = vec.x;
		elements[C + 1] = vec.y;
	}

	Matrix<T, R, C>(Vec3<T> vec)
	{
		static_assert(R == 3, "Wrong dimensions specified. Must contain 3 rows to create matrix from 3d vector.");

		Empty();

		elements[0] = vec.x;
		elements[C + 1] = vec.y;
		elements[C * 2 + 2] = vec.z;
	}

	Matrix<T, R, C>(Vec4<T> vec)
	{
		static_assert(R == 4, "Wrong dimensions specified. Must contain 4 rows to create matrix from 4d vector.");

		Empty();

		elements[0] = vec.x;
		elements[C + 1] = vec.y;
		elements[C * 2 + 2] = vec.z;
		elements[C * 3 + 3] = vec.w;
	}

	void Empty()
	{
		for(size_t i = 0; i < SIZE; i++)
		{
			elements[i] = 0;
		}
	}

	T& operator[](const size_t index)
	{
		return elements[index];
	}
	const T& operator[](const size_t index) const
	{
		return elements[index];
	}

	T& operator()(const size_t column, const size_t row)
	{
		if(C < R)
		{
			return elements[row * R + column];
		}

		return elements[column * C + row];
	}

	//Matrix = Matrix
	Matrix<T, R, C>& operator =(Matrix<T, C, R> other)
	{
		for(size_t i = 0; i < SIZE; i++)
		{
			elements[i] = other.elements[i];
		}

		return *this;
	}

	//Matrix + Scalar
	Matrix<T, R, C>& operator +(const T scalar)
	{
		ScalarOperator(+)
	}
	Matrix<T, R, C>& operator +=(const T scalar)
	{
		return *this + scalar;
	}

	//Matrix - Scalar
	Matrix<T, R, C>& operator -(const T scalar)
	{
		ScalarOperator(-)
	}
	Matrix<T, R, C>& operator -=(const T scalar)
	{
		return *this - scalar;
	}

	//Matrix * Scalar
	Matrix<T, R, C>& operator *(const T scalar)
	{
		ScalarOperator(*)
	}
	Matrix<T, R, C>& operator *=(const T scalar)
	{
		return *this * scalar;
	}

	//Matrix + Matrix
	Matrix<T, R, C>& operator +(const Matrix<T, R, C>& right)
	{
		Matrix<T, R, C> output;

		for(size_t i = 0; i < SIZE; i++)
		{
			output[i] = elements[i] + right[i];
		}

		return output;
	}
	Matrix<T, R, C>& operator +=(const Matrix<T, R, C>& right)
	{
		return *this = *this + right;
	}

	//Matrix - Matrix
	Matrix<T, R, C>& operator -(const Matrix<T, R, C>& right)
	{
		Matrix<T, R, C> output;

		for(int i = 0; i < SIZE; i++)
		{
			output[i] = elements[i] - right[i];
		}

		return output;
	}
	Matrix<T, R, C>& operator -=(const Matrix<T, R, C>& right)
	{
		return *this = *this - right;
	}

	//Matrix * Matrix
	//Multiplying matrices is difficult :(
	template<size_t C2>
	Matrix<T, R, C2> operator *(const Matrix<T, R, C2>& right)
	{
		Matrix<T, R, C2> output;

		for(size_t i = 0; i < R; i++)
		{
			for(size_t j = 0; j < C2; j++)
			{
				for(size_t k = 0; k < R; k++)
				{
					output[C2 * i + j] += elements[i * R + k] * right[k * C2 + j];
				}
			}
		}

		return output;
	}
	template<size_t C2>
	Matrix<T, 4, 4>& operator *=(const Matrix<T, R, C2>& left)
	{
		return *this = left * *this;
	}

	//Matrix * Vector
	Vec3<T>& operator *(Vec3<T>& right)
	{
		Matrix<T, 4, 1> inputMatrix = Matrix<T, 4, 1>({ right.x, right.y, right.z, 1 });
		Matrix<T, 4, 1> matrixProduct = *this * inputMatrix;
		Vec3<T> output = Vec3<T>(matrixProduct[0], matrixProduct[1], matrixProduct[2]);

		return output;
	}

	//Make sure to apply translation after rotation and scaling (actually does it last, arithmetically)
	Matrix<T, 4, 4> Translate(Vec3<T> translation)
	{
		auto output = Matrix<T, 4, 4>({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });

		for(int row = 0; row < 3; row++)
		{
			output(3, row) += translation[row];
		}

		return *this * output;
	}

	//Make sure rotation is after scaling
	//Implementation of the famous harder-to-gimbal-lock algorithm
	//https://learnopengl.com/Getting-started/Transformations under "Rotation"
	Matrix<T, 4, 4> Rotate(Vec3<float> axis, double amount)
	{
		axis.Normalize();
		T const c = cos(amount);
		T const s = sin(amount);

		Matrix<T, 4, 4> output({c + (T) pow(axis.x, 2) * (1 - c), axis.x * axis.y * (1 - c) - axis.z * s, axis.x * axis.z * (1 - c) + axis.y * s, 0,
							   axis.y * axis.x * (1 - c) + axis.z * s, c + (T) pow(axis.y, 2) * (1 - c), axis.y * axis.z * (1 - c) - axis.x * s, 0,
							   axis.z * axis.x * (1 - c) - axis.y * s, axis.z * axis.y * (1 - c) + axis.x * s, c + (T) pow(axis.z, 2) * (1 - c), 0,
							   0, 0, 0, 1});

		return *this * output;
	}

	//Make sure scaling is first (actually does it last, arithmetically)
	Matrix<T, 4, 4> Scale(Vec3<T> scale)
	{
		auto output = Matrix<T, 4, 4>();

		for(int i = 0; i < 3; i++)
		{
			output(i, i) = scale[i];
		}

		output(3, 3) = 1;

		return *this * output;
	}

	//Conversion
	const T* ToArray() const
	{
		return elements;
	}

private:
	T elements[C * R];
};

template<typename T, size_t R, size_t C> requires number<T>
std::ostream& operator <<(std::ostream& stream, Matrix<T, R, C> mat)
{
	stream << "[";

	for(int row = 0; row < R; row++)
	{
		for(int column = 0; column < C; column++)
		{
			T element = abs(mat(column, row)) < FLT_EPSILON ? 0 : mat(column, row);
			stream << element;

			if(column == C - 1)
			{
				stream << "]";

				if(column != C - 1 || row != R - 1)
				{
					stream << "\n[";
				}
			}
			else
			{
				stream << ", ";
			}
		}
	}

	return stream;
}

#undef ScalarOperator