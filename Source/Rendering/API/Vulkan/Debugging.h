#pragma once

#include <vulkan/vulkan.hpp>


namespace greg::vulkan::debug
{
vk::DebugUtilsMessengerCreateInfoEXT GetDefaultDebugMessengerInfo();
bool AreAllValidationLayersSupported();
VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT types, vk::DebugUtilsMessengerCallbackDataEXT const* callbackData, void* userData);
}