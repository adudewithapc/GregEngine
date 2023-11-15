#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat4x2 = Matrix<T, 4, 2>;
#define Mat4x2i Mat4x2<int>
#define Mat4x2f Mat4x2<float>
#define Mat4x2d Mat4x2<double>