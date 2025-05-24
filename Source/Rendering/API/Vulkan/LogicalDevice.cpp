#include "LogicalDevice.h"

#include "Debugging.h"
#include "PhysicalDevice.h"

namespace greg::vulkan
{
static const std::vector<const char*> requestedPhysicalExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

LogicalDevice::LogicalDevice(const greg::vulkan::PhysicalDevice& physicalDevice)
{
    QueueFamilies queueFamilies = physicalDevice.GetQueueFamilies();
    std::set<uint32_t> uniqueQueueFamilies = queueFamilies.GetUniqueQueueFamilies();

    const float queuePriority = 1.0f;
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos(uniqueQueueFamilies.size());
    int i = 0;
    for(uint32_t queueFamily : uniqueQueueFamilies)
    {
        queueCreateInfos[i] = vk::DeviceQueueCreateInfo({}, queueFamily, 1, &queuePriority);
        i++;
    }

    vk::PhysicalDeviceFeatures physicalDeviceFeatures = {};

    std::vector<const char*> validationLayers = greg::vulkan::debug::GetValidationLayers(); 
    vk::DeviceCreateInfo deviceCreateInfo({}, static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(), static_cast<uint32_t>(validationLayers.size()), validationLayers.data(), static_cast<uint32_t>(requestedPhysicalExtensions.size()), requestedPhysicalExtensions.data(), &physicalDeviceFeatures);
    device = physicalDevice.GetVulkanDevice().createDeviceUnique(deviceCreateInfo);

    graphicsQueue = device->getQueue(queueFamilies.GetGraphicsFamily(), 0);
    presentQueue = device->getQueue(queueFamilies.GetPresentFamily(), 0);
}

const vk::Queue& LogicalDevice::GetGraphicsQueue() const
{
    return graphicsQueue;
}

const vk::UniqueDevice& LogicalDevice::GetVulkanDevice() const
{
    return device;
}
}
