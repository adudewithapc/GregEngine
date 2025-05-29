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

    const vk::UniqueCommandBuffer& GetBuffer(uint32_t frameIndex) const;
private:
    
    vk::UniqueCommandPool pool;
    std::vector<vk::UniqueCommandBuffer> buffers;
};
}
}
