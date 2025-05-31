#include "MemoryBuffer.h"

#include "Debugging.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

namespace greg::vulkan
{
MemoryBuffer::MemoryBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                           vk::MemoryPropertyFlags memoryProperties, const greg::vulkan::PhysicalDevice& physicalDevice,
                           const greg::vulkan::LogicalDevice& logicalDevice)
{
    const std::array<uint32_t, 2> queueFamilyIndices = { physicalDevice.GetQueueFamilies().GetGraphicsFamily(), physicalDevice.GetQueueFamilies().GetTransferFamily() };
    vk::BufferCreateInfo bufferCreateInfo({}, size, usage, vk::SharingMode::eConcurrent, queueFamilyIndices);

    buffer = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->createBufferUnique(bufferCreateInfo), "Failed to create buffer!");
    
    vk::MemoryRequirements memoryRequirements = logicalDevice.GetVulkanDevice()->getBufferMemoryRequirements(*buffer);
    vk::MemoryAllocateInfo allocateInfo(memoryRequirements.size, FindMemoryType(memoryRequirements.memoryTypeBits, memoryProperties, physicalDevice.GetVulkanDevice()));

    memory = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->allocateMemoryUnique(allocateInfo), "Failed to allocate memory!");
    if(logicalDevice.GetVulkanDevice()->bindBufferMemory(*buffer, *memory, 0) != vk::Result::eSuccess)
        greg::log::Fatal("Vulkan", "Failed to bind buffer memory!");
}

vk::Buffer MemoryBuffer::GetBuffer() const
{
    return *buffer;
}

vk::DeviceMemory MemoryBuffer::GetMemory() const
{
    return *memory;
}

uint32_t MemoryBuffer::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlagBits properties, const vk::PhysicalDevice& physicalDevice)
{
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties(properties);

    for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if(typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }

    greg::log::Fatal("Vulkan" , "Failed to find any suitable memory type!");
}
}
