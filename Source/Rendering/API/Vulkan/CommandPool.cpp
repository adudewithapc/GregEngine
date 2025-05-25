#include "CommandPool.h"

#include "LogicalDevice.h"
#include "../../../Math/Color.h"

namespace greg::vulkan::command
{
CommandPool::CommandPool(const greg::vulkan::LogicalDevice& logicalDevice, uint32_t queueFamilyIndex)
{
    vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIndex);
    pool = logicalDevice.GetVulkanDevice()->createCommandPoolUnique(createInfo);
    
    vk::CommandBufferAllocateInfo allocInfo(*pool, vk::CommandBufferLevel::ePrimary, 1);
    buffers = logicalDevice.GetVulkanDevice()->allocateCommandBuffersUnique(allocInfo);
}

const vk::UniqueCommandBuffer& CommandPool::GetBuffer(uint32_t frameIndex) const
{
    return buffers[frameIndex];
}
}
