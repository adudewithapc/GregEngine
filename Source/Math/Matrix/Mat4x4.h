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

	static Mat4x4f OrthographicView(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	{
		Mat4x4f output = mat4x4::Identity<float>;

		output(0, 0) = 2.0f / (right - left);
		output(3, 0) = -(right + left) / (right - left);
		output(1, 1) = 2.0f / (top - bottom);
		output(3, 1) = -(top + bottom) / (top - bottom);
		output(2, 2) = -2.0f / (farPlane - nearPlane);
		output(3, 2) = -(farPlane - nearPlane) / (farPlane - nearPlane);
		output(3, 3) = 1;

		return output;
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
	
	template<typename T>
	static Mat4x4<T> LookAt(const Vec3<T>& cameraPos, const Vec3<T>& target, const Vec3<T>& relativeUp)
	{
		Vec3<T> lookDirection = (target - cameraPos).Normalize();

		Vec3<T> right = relativeUp.Cross(lookDirection).Normalize();
		Vec3<T> up = lookDirection.Cross(right);

		return Identity<float>.Translate(cameraPos) * Mat4x4<T>(
						 {right.x, up.x, lookDirection.x, 0,
						  right.y, up.y, lookDirection.y, 0,
						  right.z, up.z, lookDirection.z, 0,
						  0, 0, 0, 1});
	}
}