#include "CommandPool.h"

#include "Debugging.h"
#include "LogicalDevice.h"

namespace greg::vulkan::command
{
CommandPool::CommandPool(const greg::vulkan::LogicalDevice& logicalDevice, uint32_t queueFamilyIndex)
{
    vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIndex);
    pool = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->createCommandPoolUnique(createInfo), "Failed to create command pool!");
}

vk::UniqueCommandBuffer CommandPool::CreateBuffer(const greg::vulkan::LogicalDevice& logicalDevice)
{
    vk::CommandBufferAllocateInfo allocInfo(*pool, vk::CommandBufferLevel::ePrimary, 1);
    return std::move(greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->allocateCommandBuffersUnique(allocInfo), "Failed to create command buffer!").front());
}

std::vector<vk::UniqueCommandBuffer> CommandPool::CreateCommandBuffers(const greg::vulkan::LogicalDevice& logicalDevice, uint32_t bufferCount)
{
    vk::CommandBufferAllocateInfo allocInfo(*pool, vk::CommandBufferLevel::ePrimary, bufferCount);
    return greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->allocateCommandBuffersUnique(allocInfo), "Failed to create command buffers!");
}
}
