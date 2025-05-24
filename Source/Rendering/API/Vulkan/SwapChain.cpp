#include "SwapChain.h"

#include "Image.h"
#include "PhysicalDevice.h"
#include "../../Window.h"

namespace greg::vulkan
{
SwapChain::Details::Details(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface)
: capabilities(physicalDevice.getSurfaceCapabilitiesKHR(*surface)),
  formats(physicalDevice.getSurfaceFormatsKHR(*surface)),
  presentModes(physicalDevice.getSurfacePresentModesKHR(*surface))
{}

SwapChain::SwapChain(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface, const vk::UniqueDevice& logicalDevice, const greg::vulkan::QueueFamilies& queueFamilies)
: details(physicalDevice, surface)
{
    uint32_t imageCount = details.capabilities.minImageCount + 1;

    if(details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        imageCount = details.capabilities.maxImageCount;

    const vk::SurfaceFormatKHR surfaceFormat = PickSurfaceFormat(details.formats);
    const vk::PresentModeKHR presentMode = PickPresentMode(details.presentModes);
    const vk::Extent2D extent = PickExtent(details.capabilities);

    const bool isGraphicsFamilyAlsoPresent = queueFamilies.GetGraphicsFamily() == queueFamilies.GetPresentFamily();
    vk::SharingMode sharingMode;
    std::vector<uint32_t> queueFamilyIndices;
    if(isGraphicsFamilyAlsoPresent)
    {
        sharingMode = vk::SharingMode::eExclusive;
        queueFamilyIndices = { queueFamilies.GetGraphicsFamily() };
    }
    else
    {
        sharingMode = vk::SharingMode::eConcurrent;
        queueFamilyIndices = { queueFamilies.GetGraphicsFamily(), queueFamilies.GetPresentFamily() };
    }

    vk::SwapchainCreateInfoKHR createInfo({}, *surface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace, extent, 1, vk::ImageUsageFlagBits::eColorAttachment,
                                          sharingMode, static_cast<uint32_t>(queueFamilyIndices.size()), queueFamilyIndices.data(), details.capabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                          presentMode, vk::True);

    swapChain = logicalDevice->createSwapchainKHRUnique(createInfo);
    
    images = logicalDevice->getSwapchainImagesKHR(*swapChain);

    imageViews.reserve(images.size());
    for(size_t i = 0; i < images.size(); i++)
    {
        imageViews.emplace_back(greg::vulkan::image::CreateImageView(logicalDevice, images[i], surfaceFormat.format, vk::ImageAspectFlagBits::eColor, 1));
    }
}

vk::SurfaceFormatKHR SwapChain::PickSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    for(const vk::SurfaceFormatKHR& availableFormat : availableFormats)
    {
        if(availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear)
            return availableFormat;
    }

    return availableFormats[0];
}
vk::PresentModeKHR SwapChain::PickPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for(const vk::PresentModeKHR& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == vk::PresentModeKHR::eMailbox)
            return availablePresentMode;
    }

    return vk::PresentModeKHR::eFifo;
}
vk::Extent2D SwapChain::PickExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    if(capabilities.currentExtent.width != UINT_MAX)
    {
        return capabilities.currentExtent;
    }
    
    vk::Extent2D actualExtent
    {
        static_cast<uint32_t>(Window::WindowWidth),
        static_cast<uint32_t>(Window::WindowHeight),
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}
}
