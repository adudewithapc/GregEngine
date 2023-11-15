#pragma once
#include "Matrix.h"

template<typename T> requires number<T>
using Mat4x4 = Matrix<T, 4, 4>;
#define Mat4x4i Mat4x4<int>
#define Mat4x4f Mat4x4<float>
#define Mat4x4d Mat4x4<double>

namespace mat4x4
{
	template<typename T>
	inline static Mat4x4<T> Identity = Mat4x4<T>({1, 0, 0, 0,
												  0, 1, 0, 0,
												  0, 0, 1, 0,
												  0, 0, 0, 1});

	static Mat4x4f OrthographicView(int left, int right, int bottom, int top, float nearPlane, float farPlane)
	{
		return Mat4x4f({ 2.0f / (right - left), 0, 0, (float) - (right + left) / (right - left),
						  0, 2.0f / (top - bottom), 0, (float) -(top + bottom) / (top - bottom),
						  0, 0, -2.0f / (farPlane - nearPlane), -(farPlane + nearPlane) / (farPlane - nearPlane),
						  0, 0, 0, 1});
	}

	static Mat4x4f PerspectiveView(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		float const tanHalfFov = tan(fov / 2);
		Mat4x4f output;

		output(0, 0) = 1.0f / (aspectRatio * tanHalfFov);
		output(1, 1) = 1.0f / (tanHalfFov);
		output(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
		output(2, 3) = -1;
		output(3, 2) = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);
		return output;
	}
}