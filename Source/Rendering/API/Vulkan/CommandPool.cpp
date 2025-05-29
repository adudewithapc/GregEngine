#include "CommandPool.h"

#include "Debugging.h"
#include "LogicalDevice.h"
#include "../../../Math/Color.h"

namespace greg::vulkan::command
{
CommandPool::CommandPool(const greg::vulkan::LogicalDevice& logicalDevice, uint32_t queueFamilyIndex, uint32_t bufferCount)
{
    vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIndex);
    pool = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->createCommandPoolUnique(createInfo), "Failed to create command pool!");
    
    vk::CommandBufferAllocateInfo allocInfo(*pool, vk::CommandBufferLevel::ePrimary, bufferCount);
    buffers = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->allocateCommandBuffersUnique(allocInfo), "Failed to allocate command buffers!");
}

const vk::UniqueCommandBuffer& CommandPool::GetBuffer(uint32_t frameIndex) const
{
    return buffers[frameIndex];
}
}
