#pragma once

#include <optional>
#include <set>
#include "Vulkan.h"

namespace greg::vulkan
{
struct QueueFamilies
{
    QueueFamilies(vk::PhysicalDevice device, const vk::UniqueSurfaceKHR& surface);

    uint32_t GetGraphicsFamily() const;
    uint32_t GetPresentFamily() const;
    uint32_t GetTransferFamily() const;
    
    bool IsComplete() const;

    std::set<uint32_t> GetUniqueQueueFamilies() const;

private:
    friend class PhysicalDevice;
    QueueFamilies() = default;
        
    std::optional<uint32_t> graphicsFamily {};
    std::optional<uint32_t> transferFamily {};
    std::optional<uint32_t> presentFamily {};
};
}