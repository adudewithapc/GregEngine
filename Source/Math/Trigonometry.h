#pragma once

#include <concepts>
#include <numbers>

namespace trigonometry
{
	template<typename T> requires std::floating_point<T>
	static T Radians(T radians)
	{
		return radians * static_cast<T>(std::numbers::pi) / static_cast<T>(180);
	}
}
