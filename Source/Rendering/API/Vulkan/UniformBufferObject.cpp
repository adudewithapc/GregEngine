#include "UniformBufferObject.h"

namespace greg::vulkan
{
UniformBufferObject::UniformBufferObject(const Mat4x4<float>& model, const Mat4x4<float>& view, const Mat4x4<float>& projection)
: model(model),
  view(view),
  projection(projection)
{}
}
