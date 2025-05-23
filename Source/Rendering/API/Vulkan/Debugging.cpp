#include "Debugging.h"
#include "../../../Debugging/Log.h"

namespace greg::vulkan::debug
{
vk::DebugUtilsMessengerCreateInfoEXT GetDefaultDebugMessengerInfo()
{
    using SeverityBits = vk::DebugUtilsMessageSeverityFlagBitsEXT;
    vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity = SeverityBits::eVerbose | SeverityBits::eWarning | SeverityBits::eError;

    using TypeBits = vk::DebugUtilsMessageTypeFlagBitsEXT;
    vk::DebugUtilsMessageTypeFlagsEXT messageTypes = TypeBits::eGeneral | TypeBits::eValidation | TypeBits::ePerformance;

    return vk::DebugUtilsMessengerCreateInfoEXT({}, messageSeverity, messageTypes, &greg::vulkan::debug::DebugMessengerCallback, nullptr);
}

bool AreValidationLayersSupported(const std::vector<const char*>& requestedLayers)
{
    std::vector<vk::LayerProperties> validationLayers = vk::enumerateInstanceLayerProperties();
    
    size_t foundLayers = 0;
    for(const char* requestedLayer : requestedLayers)
    {
        auto extensionIterator = std::find_if(std::begin(validationLayers), std::end(validationLayers), [&requestedLayer](const vk::LayerProperties& layer)
        {
            return strcmp(layer.layerName, requestedLayer) == 0;
        });
        if(extensionIterator != std::end(validationLayers))
        {
            foundLayers++;
            greg::log::Debug("Vulkan", std::format("Found required validation layer \"{}\"", requestedLayer));
        }
        else
        {
            return false;
        }
    }

    return true;
}

vk::Bool32 DebugMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                  vk::DebugUtilsMessageTypeFlagsEXT types, vk::DebugUtilsMessengerCallbackDataEXT const* callbackData,
                                  void* userData)
{
    switch(severity)
    {
        using SeverityBits = vk::DebugUtilsMessageSeverityFlagBitsEXT;
        case SeverityBits::eVerbose:
            greg::log::Debug("Vulkan Validation Layer", callbackData->pMessage);
        break;
        case SeverityBits::eInfo:
            greg::log::Info("Vulkan Validation Layer", callbackData->pMessage);
        break;
        case SeverityBits::eWarning:
            greg::log::Warning("Vulkan Validation Layer", callbackData->pMessage);
        break;
        case SeverityBits::eError:
            greg::log::Error("Vulkan Validation Layer", callbackData->pMessage);
        break;
    }

    return VK_TRUE;
}
}