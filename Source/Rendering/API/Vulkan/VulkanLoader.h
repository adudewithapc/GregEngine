#pragma once
#include <vulkan/vulkan.hpp>

namespace greg::vulkan
{
class VulkanLoader
{
public:
    VulkanLoader();

    VulkanLoader(const VulkanLoader&) = delete;
    VulkanLoader& operator=(const VulkanLoader&) = delete;

    VulkanLoader(VulkanLoader&&) = delete;
    VulkanLoader& operator=(VulkanLoader&&) = delete;

    void AddInstance(const vk::UniqueInstance& instance);

    const vk::detail::DispatchLoaderDynamic& GetDispatchLoader() const;
    
private:
    static void* GetPlatformHandle();
    
    void* GetPlatformModule(const char* name) const;
    
    void* platformHandle = GetPlatformHandle();
    vk::detail::DispatchLoaderDynamic dynamicLoader;
    PFN_vkGetInstanceProcAddr getInstanceProcAddr = nullptr;
};
}
