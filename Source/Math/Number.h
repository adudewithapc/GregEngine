#pragma once
#include <concepts>

template<typename T>
concept number = std::integral<T> or std::floating_point<T>;