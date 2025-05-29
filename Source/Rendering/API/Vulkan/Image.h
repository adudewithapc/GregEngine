#pragma once

#include "Vulkan.h"

namespace greg::vulkan {
class LogicalDevice;
}

namespace greg::vulkan::image
{
vk::UniqueImageView CreateImageView(const LogicalDevice& logicalDevice, const vk::Image& image, vk::Format format, vk::ImageAspectFlagBits aspectFlags, uint32_t mipLevels);
vk::UniqueImageView CreateImageView(const vk::UniqueDevice& logicalDevice, const vk::Image& image, vk::Format format, vk::ImageAspectFlagBits aspectFlags, uint32_t mipLevels);
}
