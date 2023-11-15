#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat4x3 = Matrix<T, 4, 3>;
#define Mat4x3i Mat4x3<int>
#define Mat4x3f Mat4x3<float>
#define Mat4x3d Mat4x3<double>