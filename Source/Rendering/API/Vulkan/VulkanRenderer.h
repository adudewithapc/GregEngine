#pragma once

#include <vulkan/vulkan.hpp>

#include "PhysicalDevice.h"
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

    VulkanLoader loader;
    vk::UniqueInstance instance;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::detail::DispatchLoaderDynamic> debugMessenger;
    vk::UniqueSurfaceKHR surface;

    std::optional<PhysicalDevice> preferredPhysicalDevice {};
    std::vector<PhysicalDevice> physicalDevices;

    ////Windows specific
    HINSTANCE hInstance;
    HWND windowHandle;
    ////
};
}