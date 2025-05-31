#pragma once
#include "MemoryBuffer.h"

namespace greg::vulkan
{
struct UniformBufferObject;

class Uniform
{
public:
    Uniform(size_t bufferCount, const greg::vulkan::PhysicalDevice& physicalDevice, const greg::vulkan::LogicalDevice& logicalDevice);

    Uniform(Uniform&& other) = default;
    Uniform& operator=(Uniform&& other) = default;
    
    ~Uniform() = default;

    vk::DescriptorSet GetDescriptorSet(size_t setIndex) const;
    const vk::DescriptorSetLayout& GetDescriptorLayout() const;
    void WriteNewObject(const UniformBufferObject& ubo, size_t frameIndex);

private:
    std::vector<greg::vulkan::MemoryBuffer> buffers;
    std::vector<void*> mappedMemory;

    vk::UniqueDescriptorPool descriptorPool;
    vk::UniqueDescriptorSetLayout descriptorSetLayout;
    //Not unique to allow simultaneous deletion when the owning pool is deleted.
    std::vector<vk::DescriptorSet> descriptorSets;
};
}
