#pragma once

#include "Vulkan.h"
#include "CommandPool.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "VulkanLoader.h"
#include "../../Renderer.h"
#include "../../../Debugging/Log.h"

namespace greg::vulkan
{
class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer(HDC hdc, HINSTANCE hInstance, HWND hwnd);

    ~VulkanRenderer() override;
    
    void Render(const Color& clearColor) override;

protected:
    void SetupPrimitive(std::shared_ptr<Primitive> primitive) override;

private:
    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;
    
private:
    vk::UniqueInstance CreateInstance();
    vk::UniqueHandle<vk::SurfaceKHR, vk::detail::DispatchLoaderStatic> CreateSurface();
    vk::UniquePipeline CreateGraphicsPipeline();
    vk::UniqueRenderPass CreateRenderPass();
    void LoadAllPhysicalDevices();
    PhysicalDevice FindPreferredPhysicalDevice();

    vk::UniqueSemaphore CreateUniqueSemaphore(const LogicalDevice& logicalDevice);
    vk::UniqueFence CreateFence(const LogicalDevice& logicalDevice, bool startSignaled);

    void RecordDrawCommand(const vk::UniqueCommandBuffer& buffer, const Color& clearColor, uint32_t imageIndex);
    
    std::vector<const char*> GetRequiredExtensions();

    vk::UniqueInstance instance;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::detail::DispatchLoaderDynamic> debugMessenger;
    vk::UniqueSurfaceKHR surface;
    
    greg::vulkan::VulkanLoader loader;

    size_t currentFrameIndex = 0;

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
    std::array<vk::UniqueSemaphore, MAX_FRAMES_IN_FLIGHT> imageAvailableSemaphores;
    std::array<vk::UniqueSemaphore, MAX_FRAMES_IN_FLIGHT> renderFinishedSemaphores;
    std::array<vk::UniqueFence, MAX_FRAMES_IN_FLIGHT> inFlightFences;
#pragma endregion

    
    ////Windows specific
    HINSTANCE hInstance;
    HWND windowHandle;
    ////
};
static std::vector<char> LoadShaderFile(const std::string& fileName);
}
