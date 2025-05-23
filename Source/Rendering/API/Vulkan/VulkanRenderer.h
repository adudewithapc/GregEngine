#pragma once

#include <vulkan/vulkan.hpp>

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
    
    std::vector<const char*> GetRequiredExtensions();
    std::vector<const char*> GetRequestedValidationLayers();

    VulkanLoader loader;
    vk::UniqueInstance instance;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::detail::DispatchLoaderDynamic> debugMessenger;
    vk::UniqueSurfaceKHR surface;

    ////Windows specific
    HINSTANCE hInstance;
    HWND windowHandle;
    ////
    static constexpr bool enableValidationLayers = true;
};
}