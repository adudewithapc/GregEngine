#pragma once

#include "Matrix.h"

template<typename T> requires number<T>
using Mat1x4 = Matrix<T, 1, 4>;
#define Mat1x4i Mat1x4<int>
#define Mat1x4f Mat1x4<float>
#define Mat1x4d Mat1x4<double>