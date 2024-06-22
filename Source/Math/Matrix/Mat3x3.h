#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat3x3 = Matrix<T, 3, 3>;
#define Mat3x3i Mat3x3<int>
#define Mat3x3f Mat3x3<float>
#define Mat3x3d Mat3x3<double>

namespace mat3x3
{
	template<typename T>
	inline static Mat3x3<T> identity = Mat3x3<T>({1, 0, 0,
												  0, 1, 0,
												  0, 0, 1});
}