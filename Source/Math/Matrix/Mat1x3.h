#pragma once
#include "Matrix.h"
#include "../Vector.h"

template<typename T> requires number<T>
using Mat1x3 = Matrix<T, 1, 3>;
#define Mat1x3i Mat1x3<int>
#define Mat1x3f Mat1x3<float>
#define Mat1x3d Mat1x3<double>