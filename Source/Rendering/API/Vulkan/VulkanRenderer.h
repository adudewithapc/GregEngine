#pragma once
#include <vulkan/vulkan.hpp>

#include "VulkanLoader.h"
#include "../../Renderer.h"

namespace greg::vulkan
{
class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer(const HDC& hdc);
    
    void Render(const Color& clearColor) override;

protected:
    void SetupPrimitive(std::shared_ptr<Primitive> primitive) override;

private:
    vk::UniqueInstance CreateInstance();
    std::vector<const char*> GetRequiredExtensions();
    std::vector<const char*> GetRequestedValidationLayers();

    VulkanLoader loader;
    vk::UniqueInstance vulkanInstance;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::detail::DispatchLoaderDynamic> debugMessenger;
    
    static constexpr bool enableValidationLayers = true;
};
}