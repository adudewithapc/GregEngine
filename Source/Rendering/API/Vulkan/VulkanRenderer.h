#pragma once

#include <vulkan/vulkan.hpp>

#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "VulkanLoader.h"
#include "../../Renderer.h"

namespace greg::vulkan
{
class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer(HDC hdc, HINSTANCE hInstance, HWND hwnd);
    
    void Render(const Color& clearColor) override;

protected:
    void SetupPrimitive(std::shared_ptr<Primitive> primitive) override;

private:
    vk::UniqueInstance CreateInstance();
    vk::UniqueHandle<vk::SurfaceKHR, vk::detail::DispatchLoaderStatic> CreateSurface();

    void LoadAllPhysicalDevices();
    PhysicalDevice FindPreferredPhysicalDevice();
    
    std::vector<const char*> GetRequiredExtensions();

    vk::UniqueInstance instance;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::detail::DispatchLoaderDynamic> debugMessenger;
    vk::UniqueSurfaceKHR surface;
    
    greg::vulkan::VulkanLoader loader;

    std::optional<greg::vulkan::LogicalDevice> logicalDevice {};
    std::optional<greg::vulkan::PhysicalDevice> preferredPhysicalDevice {};
    std::vector<greg::vulkan::PhysicalDevice> physicalDevices {};

    std::optional<greg::vulkan::SwapChain> swapChain {};
    
    ////Windows specific
    HINSTANCE hInstance;
    HWND windowHandle;
    ////
};
}