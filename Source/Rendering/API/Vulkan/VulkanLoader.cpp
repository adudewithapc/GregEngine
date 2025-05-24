#include "VulkanLoader.h"

#include <Windows.h>
#include <vulkan/vulkan.hpp>

#include "../../../Debugging/Log.h"

namespace greg::vulkan
{
static const char* GET_INSTANCE_PROC_ADDR_NAME = "vkGetInstanceProcAddr";
static const char* ENUMERATE_INSTANCE_EXTENSION_PROPERTIES_NAME = "vkEnumerateInstanceExtensionProperties";

VulkanLoader::VulkanLoader()
{
    if(!platformHandle)
    {
        greg::log::Fatal("Vulkan", "Failed to find vulkan library!");
    }
    
    getInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetPlatformModule(GET_INSTANCE_PROC_ADDR_NAME));
    if(!getInstanceProcAddr)
    {
        greg::log::Fatal("Vulkan", "Failed to load required vulkan module \"" + std::string(GET_INSTANCE_PROC_ADDR_NAME) + "\"!");
    }
    
    if(!reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(GetPlatformModule(ENUMERATE_INSTANCE_EXTENSION_PROPERTIES_NAME)))
    {
        greg::log::Fatal("Vulkan", "Failed to load required vulkan module \"" + std::string(ENUMERATE_INSTANCE_EXTENSION_PROPERTIES_NAME) + "\"!");
    }

    dynamicLoader.init(getInstanceProcAddr);
}

//Needs to be called after the VulkanInstance is created
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
