#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat2x3 = Matrix<T, 2, 3>;
#define Mat2x3i Mat2x3<int>
#define Mat2x3f Mat2x3<float>
#define Mat2x3d Mat2x3<double>