#include "PhysicalDevice.h"

#include <set>

#include "Debugging.h"


namespace greg::vulkan
{
PhysicalDevice::PhysicalDevice(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface)
: vulkanDevice(physicalDevice)
{
    properties = physicalDevice.getProperties();
    features = physicalDevice.getFeatures();

    if(!features.samplerAnisotropy)
        return;

    queueFamilies = QueueFamilies(physicalDevice, surface);

    if(!queueFamilies.IsComplete())
        return;

    if(!HasAllRequiredExtensions())
        return;

    if(!HasSwapChainSupport(surface))
        return;

    if(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
        rating += 1000;
    else if(properties.deviceType == vk::PhysicalDeviceType::eVirtualGpu)
        rating += 500;

    rating += properties.limits.maxImageDimension2D;
}

bool PhysicalDevice::operator >(const PhysicalDevice& right) const
{
    return rating > right.rating;
}

/*SwapChain PhysicalDevice::CreateSwapChain(const vk::UniqueSurfaceKHR& surface) const
{
    return SwapChain(vulkanDevice, surface, queueFamilies);
}*/

bool PhysicalDevice::IsValid() const
{
    return rating > 0;
}

const size_t& PhysicalDevice::GetRating() const
{
    return rating;
}

const vk::PhysicalDeviceProperties& PhysicalDevice::GetProperties() const
{
    return properties;
}

const vk::PhysicalDeviceFeatures& PhysicalDevice::GetFeatures() const
{
    return features;
}

const QueueFamilies& PhysicalDevice::GetQueueFamilies() const
{
    return queueFamilies;
}

const vk::PhysicalDevice& PhysicalDevice::GetVulkanDevice() const
{
    return vulkanDevice;
}

bool PhysicalDevice::HasAllRequiredExtensions() const
{
    std::vector<vk::ExtensionProperties> availableExtensions = greg::vulkan::debug::TieResult(vulkanDevice.enumerateDeviceExtensionProperties(), "Failed to enumerate extension properties on physical device!");
    std::vector<const char*> requiredExtensions = GetRequiredExtensions();
    std::set<const char*> uniqueRequiredExtensions = {std::begin(requiredExtensions), std::end(requiredExtensions)};

    for(const vk::ExtensionProperties& availableExtension : availableExtensions)
    {
        uniqueRequiredExtensions.erase(availableExtension.extensionName);
    }

    return !uniqueRequiredExtensions.empty();
}

std::vector<const char*> PhysicalDevice::GetRequiredExtensions() const
{
    return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}

bool PhysicalDevice::HasSwapChainSupport(const vk::UniqueSurfaceKHR& surface) const
{
    std::vector<vk::SurfaceFormatKHR> availableSurfaceFormats = greg::vulkan::debug::TieResult(vulkanDevice.getSurfaceFormatsKHR(*surface), "Failed to fetch swapchain surface formats!");
    std::vector<vk::PresentModeKHR> availablePresentModes = greg::vulkan::debug::TieResult(vulkanDevice.getSurfacePresentModesKHR(*surface), "Failed to fetch swapchain present modes!");
    return !availableSurfaceFormats.empty() && !availablePresentModes.empty();
}
}
