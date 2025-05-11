#pragma once

namespace greg
{
namespace math
{

inline int Clamp(int value, int min, int max)
{
    return value > max ? max : value < min ? min : value;
}

inline float Clamp(float value, float min, float max)
{
    return value > max ? max : value < min ? min : value;
}


}
}