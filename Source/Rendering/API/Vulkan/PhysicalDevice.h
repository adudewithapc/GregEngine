#pragma once

#include <optional>
#include <set>
#include <vulkan/vulkan.hpp>

namespace greg::vulkan
{
class PhysicalDevice
{
public:
    struct QueueFamilies
    {
        QueueFamilies(vk::PhysicalDevice device, const vk::UniqueSurfaceKHR& surface);

        uint32_t GetGraphicsFamily() const;
        uint32_t GetPresentFamily() const;
        bool IsComplete() const;

        std::set<uint32_t> GetUniqueQueueFamilies() const;

    private:
        friend class PhysicalDevice;
        QueueFamilies() = default;
        
        std::optional<uint32_t> graphicsFamily = 0;
        std::optional<uint32_t> presentFamily = 0;
    };
    
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