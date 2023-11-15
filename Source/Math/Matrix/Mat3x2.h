#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat3x2 = Matrix<T, 3, 2>;
#define Mat3x2i Mat3x2<int>
#define Mat3x2f Mat3x2<float>
#define Mat3x2d Mat3x2<double>