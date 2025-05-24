#include "PhysicalDevice.h"

#include <set>

namespace greg::vulkan
{
PhysicalDevice::QueueFamilies::QueueFamilies(vk::PhysicalDevice device, const vk::UniqueSurfaceKHR& surface)
{
    std::vector<vk::QueueFamilyProperties> queueFamilyPropertyVector = device.getQueueFamilyProperties();

    int i = 0;
    for(const vk::QueueFamilyProperties& queueFamilyProperties : queueFamilyPropertyVector)
    {
        if(queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
            graphicsFamily = i;

        vk::Bool32 hasPresentSupport = device.getSurfaceSupportKHR(i, *surface);

        if(hasPresentSupport)
            presentFamily = i;

        i++;
    }
}

uint32_t PhysicalDevice::QueueFamilies::GetGraphicsFamily() const
{
    return *graphicsFamily;
}
uint32_t PhysicalDevice::QueueFamilies::GetPresentFamily() const
{
    return *presentFamily;
}
bool PhysicalDevice::QueueFamilies::IsComplete() const
{
    return graphicsFamily && presentFamily;
}

std::set<uint32_t> PhysicalDevice::QueueFamilies::GetUniqueQueueFamilies() const
{
    return { *graphicsFamily, *presentFamily };
}

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

    if(!HasAllRequiredExtensions(physicalDevice))
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

const PhysicalDevice::QueueFamilies& PhysicalDevice::GetQueueFamilies() const
{
    return queueFamilies;
}

const vk::PhysicalDevice& PhysicalDevice::GetVulkanDevice() const
{
    return vulkanDevice;
}

bool PhysicalDevice::HasAllRequiredExtensions(vk::PhysicalDevice device) const
{
    std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();
    std::vector<const char*> requiredExtensions = GetRequiredExtensions();
    std::set<const char*> uniqueRequiredExtensions = {std::begin(requiredExtensions), std::end(requiredExtensions)};

    for(const vk::ExtensionProperties& availableExtension : availableExtensions)
    {
        uniqueRequiredExtensions.erase(availableExtension.extensionName);
    }

    return uniqueRequiredExtensions.empty();
}

std::vector<const char*> PhysicalDevice::GetRequiredExtensions() const
{
    return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}
}
