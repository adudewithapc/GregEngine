#include "VulkanRenderer.h"

#include <fstream>

#include "Debugging.h"
#include "../../../Debugging/Log.h"

#include "../../../GregorianEngine.h"

namespace greg::vulkan
{
VulkanRenderer::VulkanRenderer(HDC hdc, HINSTANCE hInstance, HWND hwnd)
: Renderer(hdc),
  hInstance(hInstance),
  windowHandle(hwnd)
{
    if(greg::vulkan::debug::ShouldUseValidationLayers() && !greg::vulkan::debug::AreAllValidationLayersSupported())
        log::Fatal("Vulkan", "Failed to find all requested validation layers!");

    instance = CreateInstance();
    loader.AddInstance(instance);
    
    debugMessenger = instance->createDebugUtilsMessengerEXTUnique(greg::vulkan::debug::GetDefaultDebugMessengerInfo(), nullptr, loader.GetDispatchLoader());
    surface = CreateSurface();

    LoadAllPhysicalDevices();
    if(physicalDevices.empty())
        greg::log::Fatal("Vulkan", "Failed to find any compatible graphics card!");
    
    preferredPhysicalDevice = FindPreferredPhysicalDevice();
    greg::log::Debug("Vulkan", std::format("Selecting graphics device \"{}\"", preferredPhysicalDevice->GetProperties().deviceName.data()));

    logicalDevice = greg::vulkan::LogicalDevice(*preferredPhysicalDevice);
    swapChain = greg::vulkan::SwapChain(preferredPhysicalDevice->GetVulkanDevice(), surface, logicalDevice->GetVulkanDevice(), preferredPhysicalDevice->GetQueueFamilies());
}

void VulkanRenderer::Render(const Color& clearColor)
{
    
}
void VulkanRenderer::SetupPrimitive(std::shared_ptr<Primitive> primitive)
{
    
}

vk::UniqueInstance VulkanRenderer::CreateInstance()
{
    std::vector<const char*> requiredExtensions = GetRequiredExtensions();
    std::vector<vk::ExtensionProperties> availableExtensions = vk::enumerateInstanceExtensionProperties();
    std::vector<const char*> foundRequiredExtensions;
    for(const char* requiredExtension : requiredExtensions)
    {
        auto extensionIterator = std::find_if(std::begin(availableExtensions), std::end(availableExtensions), [&requiredExtension](const vk::ExtensionProperties& extension)
        {
            return strcmp(requiredExtension, extension.extensionName) == 0;
        });
        if(extensionIterator != std::end(availableExtensions))
        {
            greg::log::Debug("Vulkan", std::format("Found required extension {}", requiredExtension));
            foundRequiredExtensions.push_back(requiredExtension);
        }
        else
        {
            greg::log::Fatal("Vulkan", std::format("Failed to find required extension {}", requiredExtension));
        }
    }

    vk::ApplicationInfo applicationInfo("Test", VK_MAKE_VERSION(1, 0, 0), GregorianEngine::GetEngineName(), VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0));

    std::vector<const char*> validationLayers = greg::vulkan::debug::GetValidationLayers();
    vk::DebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = greg::vulkan::debug::GetDefaultDebugMessengerInfo();
    vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo, static_cast<uint32_t>(validationLayers.size()), validationLayers.data(), static_cast<uint32_t>(requiredExtensions.size()), requiredExtensions.data(), &debugMessengerCreateInfo);
    
    return vk::createInstanceUnique(instanceCreateInfo);
}

vk::UniqueHandle<vk::SurfaceKHR, vk::detail::DispatchLoaderStatic> VulkanRenderer::CreateSurface()
{
    vk::Win32SurfaceCreateInfoKHR createInfo({}, hInstance, windowHandle);

    return instance->createWin32SurfaceKHRUnique(createInfo);
}

void VulkanRenderer::LoadAllPhysicalDevices()
{
    std::vector<vk::PhysicalDevice> vulkanPhysicalDevices = instance->enumeratePhysicalDevices();
    physicalDevices.clear();
    physicalDevices.reserve(vulkanPhysicalDevices.size());

    size_t i = 0;
    for(const vk::PhysicalDevice& vulkanDevice : vulkanPhysicalDevices)
    {
        greg::vulkan::PhysicalDevice physicalDevice(vulkanDevice, surface);
        if(physicalDevice.IsValid())
            physicalDevices.push_back(physicalDevice);
        i++;
    }
}

PhysicalDevice VulkanRenderer::FindPreferredPhysicalDevice()
{
    std::sort(std::begin(physicalDevices), std::end(physicalDevices), std::greater<PhysicalDevice>());
    return physicalDevices.front();
}

std::vector<const char*> VulkanRenderer::GetRequiredExtensions()
{
    std::vector<const char*> extensions =
    {
    //Disable in shipping builds?
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,

    ////Windows specific
        "VK_KHR_win32_surface",
    ////
    };

    return extensions;
}

std::vector<char> LoadShader(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);

    if(!file.is_open())
        greg::log::Fatal("Vulkan Shaders", std::format("Failed trying to open shader file \"{}\"!", fileName));


    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> fileBuffer(fileSize);

    file.seekg(0);
    file.read(fileBuffer.data(), fileSize);

    file.close();

    return fileBuffer;
}
}
