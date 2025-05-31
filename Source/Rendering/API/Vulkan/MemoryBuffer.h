#pragma once

#include "Vulkan.h"

namespace greg::vulkan {
class LogicalDevice;
}

namespace greg::vulkan {
class PhysicalDevice;
}

namespace greg::vulkan
{
class MemoryBuffer
{
public:
    MemoryBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryProperties, const greg::vulkan::PhysicalDevice& physicalDevice, const greg::vulkan::LogicalDevice& logicalDevice);

    vk::Buffer GetBuffer() const;
    vk::DeviceMemory GetMemory() const;

private:
    uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, const vk::PhysicalDevice& physicalDevice);
    
    vk::UniqueBuffer buffer;
    vk::UniqueDeviceMemory memory;
};
}
