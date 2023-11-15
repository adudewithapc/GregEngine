#pragma once
#include "Matrix.h"
#include "../Vector.h"

template<typename T> requires number<T>
using Mat2x1 = Matrix<T, 2, 1>;
#define Mat2x1i Mat2x1<int>
#define Mat2x1f Mat2x1<float>
#define Mat2x1d Mat2x1<double>