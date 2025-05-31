#pragma once

#include "Vulkan.h"

struct Color;

namespace greg::vulkan
{
class LogicalDevice;

namespace command
{
class CommandPool
{
public:
    CommandPool(const greg::vulkan::LogicalDevice& logicalDevice, uint32_t queueFamilyIndex, uint32_t bufferCount);

    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;

    CommandPool(CommandPool&&) = default;
    CommandPool& operator=(CommandPool&&) = default;

    ~CommandPool() = default;

    vk::UniqueCommandBuffer CreateBuffer(const greg::vulkan::LogicalDevice& logicalDevice);
    std::vector<vk::UniqueCommandBuffer> CreateCommandBuffers(const greg::vulkan::LogicalDevice& logicalDevice, uint32_t bufferCount);
private:
    
    vk::UniqueCommandPool pool;
    std::vector<vk::UniqueCommandBuffer> buffers;
};
}
}
