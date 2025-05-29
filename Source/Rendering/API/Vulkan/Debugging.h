#pragma once

#include "Vulkan.h"
#include "../../../Debugging/Log.h"

namespace greg::vulkan::debug
{
constexpr bool ShouldUseValidationLayers();

vk::DebugUtilsMessengerCreateInfoEXT GetDefaultDebugMessengerInfo();
std::vector<const char*> GetValidationLayers();
bool AreAllValidationLayersSupported();
VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT types, vk::DebugUtilsMessengerCallbackDataEXT const* callbackData, void* userData);

template<typename T>
static T TieResult(vk::ResultValue<T> resultValue, const std::string& errorMessage)
{
    if(resultValue.result != vk::Result::eSuccess)
        greg::log::Fatal("Vulkan", errorMessage);

    return std::move(resultValue.value);
}
}