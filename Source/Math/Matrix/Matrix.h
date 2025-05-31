#pragma once

#include "../Number.h"
#include "../Vector.h"
#include <iostream>
#include <format>

#include "../../Debugging/Log.h"

template<typename T, size_t C, size_t R> requires number<T>
struct Matrix
{
private:
	T elements[R * C];
public:
	constexpr size_t GetSize()
	{
		return R * C;
	}

	Matrix<T, C, R>()
	{
		Empty();
	}

	Matrix<T, C, R>(const T elements[R * C])
	{
		for(int i = 0; i < GetSize(); i++)
		{
			this->elements[i] = elements[i];
		}
	}

	Matrix<T, C, R>(const std::initializer_list<T>& list)
	{
		if(list.size() != GetSize())
		{
			greg::log::Fatal("Matrix", std::format("Initializer list is wrong size. It is {} when it should be {}", list.size(), GetSize()));
			throw std::out_of_range("initializer_list_size");
		}

		//C++ forces you to use a for each >_<
		int index = 0;
		for(auto& element : list)
		{
			elements[index++] = element;
		}
	}

	Matrix<T, C, R>(Vec2<T> vec)
	{
		static_assert(R == 2, "Wrong dimensions specified. Must contain 2 rows to create matrix from 2d vector.");

		Empty();

		elements[0] = vec.x;
		elements[C + 1] = vec.y;
	}

	Matrix<T, C, R>(Vec3<T> vec)
	{
		static_assert(R == 3, "Wrong dimensions specified. Must contain 3 rows to create matrix from 3d vector.");

		Empty();

		elements[0] = vec.x;
		elements[C + 1] = vec.y;
		elements[C * 2 + 2] = vec.z;
	}

	Matrix<T, C, R>(Vec4<T> vec)
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
		for(size_t i = 0; i < GetSize(); i++)
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
		return elements[row * C + column];
	}

	const T& operator()(const size_t column, const size_t row) const
	{
		return elements[row * C + column];
	}

	//Matrix = Matrix
	Matrix<T, C, R>& operator =(Matrix<T, R, C> other)
	{
		for(size_t i = 0; i < GetSize(); i++)
		{
			elements[i] = other.elements[i];
		}

		return *this;
	}

	//Matrix + Scalar
	Matrix<T, C, R>& operator +(const T scalar)
	{
		for(int i = 0; i < GetSize(); i++)
		{
			elements[i] += scalar;
		}
		return *this;
	}
	Matrix<T, C, R>& operator +=(const T scalar)
	{
		return *this + scalar;
	}

	//Matrix - Scalar
	Matrix<T, C, R>& operator -(const T scalar)
	{
		for(int i = 0; i < GetSize(); i++)
		{
			elements[i] -= scalar;
		}
		return *this;
	}
	Matrix<T, C, R>& operator -=(const T scalar)
	{
		return *this - scalar;
	}

	//Matrix * Scalar
	Matrix<T, C, R>& operator *(const T scalar)
	{
		for(int i = 0; i < GetSize(); i++)
		{
			elements[i] *= scalar;
		}
		return *this;
	}
	Matrix<T, C, R>& operator *=(const T scalar)
	{
		return *this * scalar;
	}

	//Matrix + Matrix
	Matrix<T, C, R>& operator +(const Matrix<T, C, R>& right)
	{
		Matrix<T, C, R> output;

		for(size_t i = 0; i < GetSize(); i++)
		{
			output[i] = elements[i] + right[i];
		}

		return output;
	}
	Matrix<T, C, R>& operator +=(const Matrix<T, C, R>& right)
	{
		return *this = *this + right;
	}

	//Matrix - Matrix
	Matrix<T, C, R>& operator -(const Matrix<T, C, R>& right)
	{
		Matrix<T, R, C> output;

		for(int i = 0; i < GetSize(); i++)
			
		{
			output[i] = elements[i] - right[i];
		}

		return output;
	}
	Matrix<T, C, R>& operator -=(const Matrix<T, C, R>& right)
	{
		return *this = *this - right;
	}

	//Matrix * Matrix
	template<size_t C2>
	Matrix<T, C2, R> operator *(const Matrix<T, C2, R>& right)
	{
		Matrix<T, C2, R> output;

		/*As an example of what the below for loop does, here is how one 3x2 matrix and one 2x3 matrix get multiplied together
		output(0, 0) = left(0, 0) * right(0, 0) +
					   left(1, 0) * right(0, 1) +
					   left(2, 0) * right(0, 2);

		output(1, 0) = left(0, 0) * right(1, 0) +
					   left(1, 0) * right(1, 1) +
					   left(2, 0) * right(1, 2);

		output(0, 1) = left(0, 1) * right(0, 0) +
					   left(1, 1) * right(0, 1) +
					   left(2, 1) * right(0, 2);

		output(1, 1) = left(0, 1) * right(1, 0) +
					   left(1, 1) * right(1, 1) +
					   left(2, 1) * right(1, 2); */

		for (size_t leftRow = 0; leftRow < C; leftRow++)
		{
			for (size_t rightColumn = 0; rightColumn < C2; rightColumn++)
			{
				for (size_t leftColumnRightRow = 0; leftColumnRightRow < R; leftColumnRightRow++)
				{
					output[rightColumn + leftRow * C2] += elements[leftColumnRightRow + leftRow * R] * right[rightColumn + leftColumnRightRow * C2];
				}
			}
		}

		return output;
	}
	//We can't change the dimensions of "this", so right must have the same dimensions
	Matrix<T, C, R>& operator *=(const Matrix<T, C, R>& right)
	{
		return *this = *this * right;
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
		/*Matrix<T, 4, 4> output({c + (T) pow(axis.x, 2) * (1 - c), axis.y * axis.x * (1 - c) + axis.z * s, axis.z * axis.x * (1 - c) - axis.y * s, 0,
								axis.x * axis.y * (1 - c) - axis.z * s, c + (T) pow(axis.y, 2) * (1 - c), axis.z * axis.y * (1 - c) + axis.x * s, 0,
								axis.x * axis.z * (1 - c) + axis.y * s, axis.y * axis.z * (1 - c) - axis.x * s, c + (T) pow(axis.z, 2) * (1 - c), 0,
								0, 0, 0, 1});*/

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

	Matrix<T, R, C> Transpose()
	{
		Matrix<T, R, C> output;

		for(size_t column = 0; column < C; column++)
		{
			for(size_t row = 0; row < R; row++)
			{
				output(row, column) = elements[row * C + column];
			}
		}

		return output;
	}

	//Conversion
	const T* ToArray() const
	{
		return elements;
	}
};

template<typename T, size_t C, size_t R> requires number<T>
std::ostream& operator <<(std::ostream& stream, Matrix<T, C, R> mat)
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