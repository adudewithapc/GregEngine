#pragma once

#include <vulkan/vulkan.hpp>

namespace greg::vulkan
{
class PhysicalDevice;

class LogicalDevice
{
public:
    LogicalDevice(const greg::vulkan::PhysicalDevice& physicalDevice);

    LogicalDevice(const LogicalDevice&) = delete;
    LogicalDevice& operator=(const LogicalDevice&) = delete;

    LogicalDevice(LogicalDevice&&) = default;
    LogicalDevice& operator=(LogicalDevice&&) = default;

    ~LogicalDevice() = default;

    const vk::Queue& GetGraphicsQueue() const;
    const vk::UniqueDevice& GetVulkanDevice() const;
    vk::UniqueShaderModule CreateShaderStage(const std::vector<char>& code);

private:
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    
    vk::UniqueDevice device;
};
}