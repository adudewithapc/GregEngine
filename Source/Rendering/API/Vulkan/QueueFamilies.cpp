#include "QueueFamilies.h"

namespace greg::vulkan
{
QueueFamilies::QueueFamilies(vk::PhysicalDevice device, const vk::UniqueSurfaceKHR& surface)
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

        if(IsComplete())
            break;

        i++;
    }
}

uint32_t QueueFamilies::GetGraphicsFamily() const
{
    return *graphicsFamily;
}
uint32_t QueueFamilies::GetPresentFamily() const
{
    return *presentFamily;
}
bool QueueFamilies::IsComplete() const
{
    return graphicsFamily && presentFamily;
}

std::set<uint32_t> QueueFamilies::GetUniqueQueueFamilies() const
{
    return { *graphicsFamily, *presentFamily };
}
}