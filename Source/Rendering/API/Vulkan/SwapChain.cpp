#include "SwapChain.h"

#include "Debugging.h"
#include "Image.h"
#include "PhysicalDevice.h"
#include "../../Window.h"
#include "../../../GregorianEngine.h"

namespace greg::vulkan
{
SwapChain::Details::Details(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface)
: capabilities(greg::vulkan::debug::TieResult(physicalDevice.getSurfaceCapabilitiesKHR(*surface), "Failed to fetch surface capabilities!")),
  formats(greg::vulkan::debug::TieResult(physicalDevice.getSurfaceFormatsKHR(*surface), "Failed to fetch surface formats!")),
  presentModes(greg::vulkan::debug::TieResult(physicalDevice.getSurfacePresentModesKHR(*surface), "Failed to fetch surface present modes!"))
{}

SwapChain::SwapChain(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface, const vk::UniqueDevice& logicalDevice, const greg::vulkan::QueueFamilies& queueFamilies, const std::optional<SwapChain>& oldSwapChain)
: details(physicalDevice, surface)
{
    uint32_t imageCount = details.capabilities.minImageCount + 1;

    if(details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        imageCount = details.capabilities.maxImageCount;

    const vk::SurfaceFormatKHR surfaceFormat = PickSurfaceFormat(details.formats);
    format = surfaceFormat.format;
    
    const vk::PresentModeKHR presentMode = PickPresentMode(details.presentModes);
    extent = PickExtent(details.capabilities);

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
                                          presentMode, vk::True, oldSwapChain ? *oldSwapChain->GetVulkanSwapChain() : nullptr);

    swapChain = greg::vulkan::debug::TieResult(logicalDevice->createSwapchainKHRUnique(createInfo), "Failed to create swap chain!");
    
    images = greg::vulkan::debug::TieResult(logicalDevice->getSwapchainImagesKHR(*swapChain), "Failed to fetch images from swap chain!");

    imageViews.reserve(images.size());
    for(const vk::Image& image : images)
    {
        imageViews.emplace_back(greg::vulkan::image::CreateImageView(logicalDevice, image, surfaceFormat.format, vk::ImageAspectFlagBits::eColor, 1));
    }
}

void SwapChain::CreateFramebuffers(const vk::UniqueDevice& logicalDevice, const vk::UniqueRenderPass& renderPass)
{
    framebuffers.reserve(imageViews.size());
    for(const vk::UniqueImageView& imageView : imageViews)
    {
        vk::ImageView attachments[] = { *imageView };

        vk::FramebufferCreateInfo framebufferCreateInfo({}, *renderPass, 1, attachments, extent.width, extent.height, 1);
        framebuffers.emplace_back(greg::vulkan::debug::TieResult(logicalDevice->createFramebufferUnique(framebufferCreateInfo), "Failed to create frame buffer!"));
    }
}

vk::Extent2D SwapChain::GetExtent() const
{
    return extent;
}

vk::Format SwapChain::GetFormat() const
{
    return format;
}

const vk::UniqueFramebuffer& SwapChain::GetFramebuffer(uint32_t frameIndex)
{
    return framebuffers[frameIndex];
}

const vk::UniqueSwapchainKHR& SwapChain::GetVulkanSwapChain() const
{
    return swapChain;
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

    Vec2i framebufferSize = GregorianEngine::Get().GetWindow().GetFramebufferSize();
    vk::Extent2D actualExtent
    {
        static_cast<uint32_t>(framebufferSize.x),
        static_cast<uint32_t>(framebufferSize.y),
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}
}
