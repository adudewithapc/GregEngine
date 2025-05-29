#pragma once

#ifdef GREG_VULKAN

//We cannot define a macro with parameters using premake.
//Instead, if it is defined, add parameters manually
#ifdef GREG_VULKAN_DISABLE_ASSERTS
#define VULKAN_HPP_ASSERT(x)
#endif

#include <vulkan/vulkan.hpp>

#else
#error Vulkan library was included even though Vulkan is disabled.
#endif
