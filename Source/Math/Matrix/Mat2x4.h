#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat2x4 = Matrix<T, 2, 4>;
#define Mat2x4i Mat2x4<int>
#define Mat2x4f Mat2x4<float>
#define Mat2x4d Mat2x4<double>