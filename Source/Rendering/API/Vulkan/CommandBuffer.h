#pragma once

#include "Vulkan.h"

namespace greg::vulkan {
class MemoryBuffer;
}

namespace greg::vulkan::command
{
void CopyBuffer(vk::Buffer source, vk::Buffer destination, vk::DeviceSize size, vk::CommandBuffer commandBuffer);
void CopyBuffer(const greg::vulkan::MemoryBuffer& source, const greg::vulkan::MemoryBuffer& destination, vk::DeviceSize size, vk::CommandBuffer commandBuffer);
void FlushTransientBuffer(vk::UniqueCommandBuffer commandBuffer);
}
