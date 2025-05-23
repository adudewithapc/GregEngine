#pragma once

#include <vulkan/vulkan.hpp>


namespace greg::vulkan::debug
{
vk::DebugUtilsMessengerCreateInfoEXT GetDefaultDebugMessengerInfo();
bool AreValidationLayersSupported(const std::vector<const char*>& requestedLayers);
VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT types, vk::DebugUtilsMessengerCallbackDataEXT const* callbackData, void* userData);
}