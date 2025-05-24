#pragma once

#include <vulkan/vulkan.hpp>

namespace greg::vulkan
{
class PhysicalDevice;

class LogicalDevice
{
public:
    LogicalDevice(const greg::vulkan::PhysicalDevice& physicalDevice);

private:
    vk::Queue graphicsQueue;
    vk::UniqueDevice device;
};
}