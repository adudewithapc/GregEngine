#pragma once

#include <vulkan/vulkan.hpp>

#include "QueueFamilies.h"

namespace greg::vulkan
{
class PhysicalDevice
{
public:
    
    PhysicalDevice(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface);

    PhysicalDevice(const PhysicalDevice&) = default;
    PhysicalDevice& operator=(const PhysicalDevice&) = default;

    PhysicalDevice(PhysicalDevice&&) = default;
    PhysicalDevice& operator=(PhysicalDevice&&) = default;

    bool operator >(const PhysicalDevice& right) const;

    bool IsValid() const;
    const size_t& GetRating() const;
    const vk::PhysicalDeviceProperties& GetProperties() const;
    const vk::PhysicalDeviceFeatures& GetFeatures() const;
    const QueueFamilies& GetQueueFamilies() const;
    const vk::PhysicalDevice& GetVulkanDevice() const;
private:
    bool HasAllRequiredExtensions() const;
    std::vector<const char*> GetRequiredExtensions() const;
    bool HasSwapChainSupport(const vk::UniqueSurfaceKHR& surface) const;
    
    size_t rating = 0;
    vk::PhysicalDeviceProperties properties {};
    vk::PhysicalDeviceFeatures features {};
    QueueFamilies queueFamilies {};
    
    vk::PhysicalDevice vulkanDevice;
};
}
