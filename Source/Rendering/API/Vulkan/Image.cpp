#include "Image.h"

#include "LogicalDevice.h"

namespace greg::vulkan::image
{
vk::UniqueImageView CreateImageView(const LogicalDevice& logicalDevice, const vk::Image& image, vk::Format format, vk::ImageAspectFlagBits aspectFlags, uint32_t mipLevels)
{
    return CreateImageView(logicalDevice.GetVulkanDevice(), image, format, aspectFlags, mipLevels);
}

vk::UniqueImageView CreateImageView(const vk::UniqueDevice& logicalDevice, const vk::Image& image, vk::Format format, vk::ImageAspectFlagBits aspectFlags, uint32_t mipLevels)
{
    vk::ImageSubresourceRange subresourceRange(aspectFlags, 0, mipLevels, 0, 1);
    vk::ComponentMapping componentMapping;
    
    vk::ImageViewCreateInfo createInfo({}, image, vk::ImageViewType::e2D, format, componentMapping, subresourceRange);

    return logicalDevice->createImageViewUnique(createInfo);
}
}
