#pragma once
#include "Matrix.h"
#include "../Vector.h"

template<typename T> requires number<T>
using Mat1x2 = Matrix<T, 1, 2>;
#define Mat1x2i Mat1x2<int>
#define Mat1x2f Mat1x2<float>
#define Mat1x2d Mat1x2<double>