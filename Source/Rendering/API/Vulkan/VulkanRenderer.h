#pragma once

#include <vulkan/vulkan.hpp>

#include "CommandPool.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "VulkanLoader.h"
#include "../../Renderer.h"
#include "../../../Math/Color.h"

namespace greg::vulkan
{
class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer(HDC hdc, HINSTANCE hInstance, HWND hwnd);

    ~VulkanRenderer();
    
    void Render(const Color& clearColor) override;

protected:
    void SetupPrimitive(std::shared_ptr<Primitive> primitive) override;

private:
    vk::UniqueInstance CreateInstance();
    vk::UniqueHandle<vk::SurfaceKHR, vk::detail::DispatchLoaderStatic> CreateSurface();
    vk::UniquePipeline CreateGraphicsPipeline();
    vk::UniqueRenderPass CreateRenderPass();
    void LoadAllPhysicalDevices();
    PhysicalDevice FindPreferredPhysicalDevice();

    vk::UniqueSemaphore CreateUniqueSemaphore(const LogicalDevice& logicalDevice);
    vk::UniqueFence CreateFence(const LogicalDevice& logicalDevice, bool startSignaled);

    void RecordDrawCommand(const vk::UniqueCommandBuffer& commandBuffer, const Color& clearColor, uint32_t imageIndex);
    
    std::vector<const char*> GetRequiredExtensions();

    vk::UniqueInstance instance;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::detail::DispatchLoaderDynamic> debugMessenger;
    vk::UniqueSurfaceKHR surface;
    
    greg::vulkan::VulkanLoader loader;

#pragma region Devices
    std::optional<greg::vulkan::LogicalDevice> logicalDevice {};
    std::optional<greg::vulkan::PhysicalDevice> preferredPhysicalDevice {};
    std::vector<greg::vulkan::PhysicalDevice> physicalDevices {};
#pragma endregion

#pragma region Rendering
    //Rendering
    std::optional<greg::vulkan::SwapChain> swapChain {};
    vk::UniqueRenderPass renderPass {};
    vk::UniquePipeline graphicsPipeline {};
#pragma endregion 

    std::optional<greg::vulkan::command::CommandPool> graphicsCommandPool {};

#pragma region Sync Objects
    vk::UniqueSemaphore imageAvailableSemaphore {};
    vk::UniqueSemaphore renderFinishedSemaphore {};
    vk::UniqueFence inFlightFence {};
#pragma endregion 
    
    ////Windows specific
    HINSTANCE hInstance;
    HWND windowHandle;
    ////
};

static std::vector<char> LoadShaderFile(const std::string& fileName);
}
