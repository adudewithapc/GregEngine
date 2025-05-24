#pragma once

#include <vulkan/vulkan.hpp>


namespace greg::vulkan::debug
{
constexpr bool ShouldUseValidationLayers();

vk::DebugUtilsMessengerCreateInfoEXT GetDefaultDebugMessengerInfo();
std::vector<const char*> GetValidationLayers();
bool AreAllValidationLayersSupported();
VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT types, vk::DebugUtilsMessengerCallbackDataEXT const* callbackData, void* userData);
}