#include "VulkanLoader.h"

#include <Windows.h>
#include <vulkan/vulkan.hpp>

#include "../../../Debugging/Log.h"

namespace greg::vulkan
{
VulkanLoader::VulkanLoader()
{
    if(!platformHandle)
    {
        greg::log::Fatal("Vulkan", "Failed to find vulkan library!");
    }
    
    getInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetPlatformModule("vkGetInstanceProcAddr"));
    if(!getInstanceProcAddr)
    {
        greg::log::Fatal("Vulkan", "Failed to load vulkan module \"vkGetInstanceProcAddr\"!");
    }
    
    if(!reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(GetPlatformModule("vkEnumerateInstanceExtensionProperties")))
    {
        greg::log::Fatal("Vulkan", "Failed to load vulkan \"vkEnumerateInstanceExtensionProperties\"!");
    }

    dynamicLoader.init(getInstanceProcAddr);
}

void VulkanLoader::AddInstance(const vk::UniqueInstance& instance)
{
    dynamicLoader.init(*instance, getInstanceProcAddr);
}

const vk::detail::DispatchLoaderDynamic& VulkanLoader::GetDispatchLoader() const
{
    return dynamicLoader;
}

void* VulkanLoader::GetPlatformHandle()
{
    return LoadLibraryA("vulkan-1.dll");
}

void* VulkanLoader::GetPlatformModule(const char* name) const
{
    return GetProcAddress(static_cast<HMODULE>(platformHandle), name);
}
}
