#pragma once

#include "../../../Math/Matrix/Mat4x4.h"

namespace greg::vulkan
{
struct UniformBufferObject
{
public:
    UniformBufferObject() = default;
    UniformBufferObject(const Mat4x4f& model, const Mat4x4f& view, const Mat4x4f& projection);
    
    alignas(8) Mat4x4f model = mat4x4::Identity<float>;
    alignas(8) Mat4x4f view = mat4x4::Identity<float>;
    alignas(8) Mat4x4f projection = mat4x4::Identity<float>;
};
}