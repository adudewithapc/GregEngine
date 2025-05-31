#include "Uniform.h"

#include "Debugging.h"
#include "LogicalDevice.h"
#include "UniformBufferObject.h"

namespace greg::vulkan
{
Uniform::Uniform(size_t bufferCount, const greg::vulkan::PhysicalDevice& physicalDevice, const greg::vulkan::LogicalDevice& logicalDevice)
: buffers(bufferCount),
  mappedMemory(bufferCount),
  descriptorSets(bufferCount)
{
    constexpr vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
    
    for(size_t i = 0; i < bufferCount; i++)
    {
        buffers[i] = MemoryBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, physicalDevice, logicalDevice);
        mappedMemory[i] = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->mapMemory(buffers[i].GetMemory(), 0, bufferSize, {}), std::format("Failed to map uniform buffer #{}!", i));
    }

    {
        vk::DescriptorSetLayoutBinding uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex);
        vk::DescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo({}, 1, &uboLayoutBinding);
        descriptorSetLayout = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->createDescriptorSetLayoutUnique(descriptorLayoutCreateInfo), "Failed to create uniform descriptor set layout!");
    }

    {
        vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(bufferCount));
        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo({}, static_cast<uint32_t>(bufferCount), poolSize);
        descriptorPool = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->createDescriptorPoolUnique(descriptorPoolCreateInfo), "Failed to create uniform descriptor pool!");
    }

    {
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(bufferCount, *descriptorSetLayout);
        vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo(*descriptorPool, static_cast<uint32_t>(bufferCount), descriptorSetLayouts.data());
        descriptorSets = greg::vulkan::debug::TieResult(logicalDevice.GetVulkanDevice()->allocateDescriptorSets(descriptorSetAllocateInfo), "Failed to allocate descriptor sets!");
    }

    for(size_t i = 0; i < bufferCount; i++)
    {
        vk::DescriptorBufferInfo descriptorBufferInfo(buffers[i].GetBuffer(), 0, sizeof(UniformBufferObject));
        vk::WriteDescriptorSet uniformDescriptorWrite(descriptorSets[i], 0, 0, vk::DescriptorType::eUniformBuffer, nullptr, descriptorBufferInfo);
        logicalDevice.GetVulkanDevice()->updateDescriptorSets(uniformDescriptorWrite, nullptr);
    }
}

vk::DescriptorSet Uniform::GetDescriptorSet(size_t setIndex) const
{
    return descriptorSets[setIndex];
}

const vk::DescriptorSetLayout& Uniform::GetDescriptorLayout() const
{
    return *descriptorSetLayout;
}

void Uniform::WriteNewObject(const UniformBufferObject& ubo, size_t frameIndex)
{
    memcpy(mappedMemory[frameIndex], &ubo, sizeof(UniformBufferObject));
}
}
