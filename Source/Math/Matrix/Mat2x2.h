#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat2x2 = Matrix<T, 2, 2>;
#define Mat2x2i Mat2x2<int>
#define Mat2x2f Mat2x2<float>
#define Mat2x2d Mat2x2<double>

namespace mat2x2
{
	template<typename T>
	inline static Mat2x2<T> identity = Mat2x2<T>({1, 0, 
												  0, 1});
}