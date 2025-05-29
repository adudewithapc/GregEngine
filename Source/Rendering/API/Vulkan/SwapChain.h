#pragma once

#include "Vulkan.h"

#include "PhysicalDevice.h"

namespace greg::vulkan
{
class SwapChain
{
public:
    struct Details
    {
    public:
        Details(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface);
        
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    SwapChain(vk::PhysicalDevice physicalDevice, const vk::UniqueSurfaceKHR& surface, const vk::UniqueDevice& device,
              const greg::vulkan::QueueFamilies& queueFamilies, const std::optional<SwapChain>& oldSwapChain = {});

    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;

    SwapChain(SwapChain&&) = default;
    SwapChain& operator=(SwapChain&&) = default;

    void CreateFramebuffers(const vk::UniqueDevice& logicalDevice, const vk::UniqueRenderPass& renderPass);
    vk::Extent2D GetExtent() const;
    vk::Format GetFormat() const;
    const vk::UniqueFramebuffer& GetFramebuffer(uint32_t frameIndex);
    const vk::UniqueSwapchainKHR& GetVulkanSwapChain() const;
private:
    static vk::SurfaceFormatKHR PickSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    static vk::PresentModeKHR PickPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    static vk::Extent2D PickExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

    Details details;

    vk::Extent2D extent;
    vk::Format format;
    
    std::vector<vk::Image> images {};
    std::vector<vk::UniqueImageView> imageViews {};
    std::vector<vk::UniqueFramebuffer> framebuffers {};
    vk::UniqueSwapchainKHR swapChain {};
};
}
