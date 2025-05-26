#include "VulkanRenderer.h"

#include <filesystem>
#include <fstream>

#include "Debugging.h"
#include "../../../Debugging/Log.h"

#include "../../../GregorianEngine.h"

static const bool WIREFRAME_MODE = false;

namespace greg::vulkan
{
VulkanRenderer::VulkanRenderer(HDC hdc, HINSTANCE hInstance, HWND hwnd)
: Renderer(hdc),
  hInstance(hInstance),
  windowHandle(hwnd)
{
    if(greg::vulkan::debug::ShouldUseValidationLayers() && !greg::vulkan::debug::AreAllValidationLayersSupported())
        log::Fatal("Vulkan", "Failed to find all requested validation layers!");

    instance = CreateInstance();
    loader.AddInstance(instance);
    
    debugMessenger = instance->createDebugUtilsMessengerEXTUnique(greg::vulkan::debug::GetDefaultDebugMessengerInfo(), nullptr, loader.GetDispatchLoader());
    surface = CreateSurface();

    LoadAllPhysicalDevices();
    if(physicalDevices.empty())
        greg::log::Fatal("Vulkan", "Failed to find any compatible graphics card!");
    
    preferredPhysicalDevice = FindPreferredPhysicalDevice();
    greg::log::Debug("Vulkan", std::format("Selecting graphics device \"{}\"", preferredPhysicalDevice->GetProperties().deviceName.data()));

    logicalDevice = greg::vulkan::LogicalDevice(*preferredPhysicalDevice);
    graphicsCommandPool = greg::vulkan::command::CommandPool(*logicalDevice, preferredPhysicalDevice->GetQueueFamilies().GetGraphicsFamily());

    imageAvailableSemaphore = CreateUniqueSemaphore(*logicalDevice);
    renderFinishedSemaphore = CreateUniqueSemaphore(*logicalDevice);
    inFlightFence = CreateFence(*logicalDevice, true);
    
    swapChain = greg::vulkan::SwapChain(preferredPhysicalDevice->GetVulkanDevice(), surface, logicalDevice->GetVulkanDevice(), preferredPhysicalDevice->GetQueueFamilies());
    renderPass = CreateRenderPass();
    swapChain->CreateFramebuffers(logicalDevice->GetVulkanDevice(), renderPass);

    graphicsPipeline = CreateGraphicsPipeline();
}

VulkanRenderer::~VulkanRenderer()
{
    logicalDevice->GetVulkanDevice()->waitIdle();
}

void VulkanRenderer::Render(const Color& clearColor)
{
    logicalDevice->GetVulkanDevice()->waitForFences(1, &*inFlightFence, vk::True, UINT64_MAX);
    logicalDevice->GetVulkanDevice()->resetFences(1, &*inFlightFence);

    uint32_t swapChainImageIndex;
    {
        vk::Result acquireResult = logicalDevice->GetVulkanDevice()->acquireNextImageKHR(*swapChain->GetVulkanSwapChain(), UINT64_MAX, *imageAvailableSemaphore, nullptr, &swapChainImageIndex);

        if(acquireResult == vk::Result::eErrorOutOfDateKHR)
        {
            //Recreate swapchain
            return;
        }
        else if(acquireResult != vk::Result::eSuccess && acquireResult != vk::Result::eSuboptimalKHR)
        {
            greg::log::Fatal("Vulkan", "Failed to acquire swap chain image!");
        }
    }

    graphicsCommandPool->GetBuffer(0)->reset();
    RecordDrawCommand(graphicsCommandPool->GetBuffer(0), clearColor, swapChainImageIndex);

    std::array<vk::PipelineStageFlags, 1> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    vk::Semaphore waitSemaphores[] = { *imageAvailableSemaphore };
    vk::Semaphore signalSemaphores[] = { *renderFinishedSemaphore };
    vk::SubmitInfo submitInfo(1, waitSemaphores, waitStages.data(), 1, &*graphicsCommandPool->GetBuffer(0), 1, signalSemaphores);
    if(logicalDevice->GetGraphicsQueue().submit(1, &submitInfo, *inFlightFence) != vk::Result::eSuccess)
        greg::log::Fatal("Vulkan", "Failed to submit draw command buffer!");

    vk::SwapchainKHR swapChains[] = { *swapChain->GetVulkanSwapChain() };
    vk::PresentInfoKHR presentInfo(1, signalSemaphores, 1, swapChains, &swapChainImageIndex, nullptr);
    vk::Result result = logicalDevice->GetPresentQueue().presentKHR(presentInfo);
    if(result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
    {
        //Recreate swapchain
    }
    else if(result != vk::Result::eSuccess)
    {
        greg::log::Fatal("Vulkan", "Failed to present swap chain!");
    }
}
void VulkanRenderer::SetupPrimitive(std::shared_ptr<Primitive> primitive)
{
    
}

vk::UniqueInstance VulkanRenderer::CreateInstance()
{
    std::vector<const char*> requiredExtensions = GetRequiredExtensions();
    std::vector<vk::ExtensionProperties> availableExtensions = vk::enumerateInstanceExtensionProperties();
    std::vector<const char*> foundRequiredExtensions;
    for(const char* requiredExtension : requiredExtensions)
    {
        auto extensionIterator = std::find_if(std::begin(availableExtensions), std::end(availableExtensions), [&requiredExtension](const vk::ExtensionProperties& extension)
        {
            return strcmp(requiredExtension, extension.extensionName) == 0;
        });
        if(extensionIterator != std::end(availableExtensions))
        {
            greg::log::Debug("Vulkan", std::format("Found required extension {}", requiredExtension));
            foundRequiredExtensions.push_back(requiredExtension);
        }
        else
        {
            greg::log::Fatal("Vulkan", std::format("Failed to find required extension {}", requiredExtension));
        }
    }

    vk::ApplicationInfo applicationInfo("Test", VK_MAKE_VERSION(1, 0, 0), GregorianEngine::GetEngineName(), VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0));

    std::vector<const char*> validationLayers = greg::vulkan::debug::GetValidationLayers();
    vk::DebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = greg::vulkan::debug::GetDefaultDebugMessengerInfo();
    vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo, static_cast<uint32_t>(validationLayers.size()), validationLayers.data(), static_cast<uint32_t>(requiredExtensions.size()), requiredExtensions.data(), &debugMessengerCreateInfo);
    
    return vk::createInstanceUnique(instanceCreateInfo);
}

vk::UniqueHandle<vk::SurfaceKHR, vk::detail::DispatchLoaderStatic> VulkanRenderer::CreateSurface()
{
    vk::Win32SurfaceCreateInfoKHR createInfo({}, hInstance, windowHandle);

    return instance->createWin32SurfaceKHRUnique(createInfo);
}

vk::UniquePipeline VulkanRenderer::CreateGraphicsPipeline()
{
    std::vector<char> vertexShaderCode = LoadShaderFile("triangle_vertex");
    std::vector<char> fragmentShaderCode = LoadShaderFile("triangle_fragment");

    vk::UniqueShaderModule vertexShaderModule = logicalDevice->CreateShaderStage(vertexShaderCode);
    vk::UniqueShaderModule fragmentShaderModule = logicalDevice->CreateShaderStage(fragmentShaderCode);

    vk::PipelineShaderStageCreateInfo vertexShaderStage({}, vk::ShaderStageFlagBits::eVertex, *vertexShaderModule, "main");
    vk::PipelineShaderStageCreateInfo fragmentShaderStage({}, vk::ShaderStageFlagBits::eFragment, *fragmentShaderModule, "main");

    vk::PipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStage, fragmentShaderStage };

    vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo({}, 0, nullptr, 0, nullptr);
    
    std::vector<vk::DynamicState> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo({}, static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data());

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo({}, vk::PrimitiveTopology::eTriangleList, vk::False);

    vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(swapChain->GetExtent().width), static_cast<float>(swapChain->GetExtent().height), 0.0f, 1.0f);
    vk::Rect2D scissor(vk::Offset2D(0, 0), swapChain->GetExtent());
    vk::PipelineViewportStateCreateInfo viewportStateCreateInfo({}, 1, &viewport, 1, &scissor);

    //Fixed state
    vk::PolygonMode polygonMode = WIREFRAME_MODE ? vk::PolygonMode::eLine : vk::PolygonMode::eFill;
    vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo({}, vk::False, vk::False, polygonMode, vk::CullModeFlagBits::eBack,
                                                                  vk::FrontFace::eClockwise, vk::False, 0.0f, 0.0f, 0.0f, 1.0f);
    vk::PipelineMultisampleStateCreateInfo multisampleCreateInfo({}, vk::SampleCountFlagBits::e1, vk::False, 1.0f, nullptr, vk::False, vk::False);
    vk::PipelineDepthStencilStateCreateInfo depthStencilCreateInfo {};

    //Color blending
    vk::PipelineColorBlendAttachmentState colorBlendAttachment(vk::False, vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
                                                                vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd, 
                                                                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
    vk::PipelineColorBlendStateCreateInfo colorBlendingCreateInfo({}, vk::False, vk::LogicOp::eClear, 1, &colorBlendAttachment, {0, 0, 0, 0});

    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo({}, 0, nullptr, 0, nullptr);
    vk::UniquePipelineLayout pipelineLayout = logicalDevice->GetVulkanDevice()->createPipelineLayoutUnique(pipelineLayoutCreateInfo);

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo({}, 2, shaderStages, &vertexInputCreateInfo, &inputAssemblyCreateInfo, nullptr, &viewportStateCreateInfo, &rasterizerCreateInfo,
                                                              &multisampleCreateInfo, &depthStencilCreateInfo, &colorBlendingCreateInfo, &dynamicStateCreateInfo, *pipelineLayout, *renderPass, 0, nullptr, -1);

    auto [result, pipelines] = logicalDevice->GetVulkanDevice()->createGraphicsPipelinesUnique(nullptr, graphicsPipelineCreateInfo);
    if(result != vk::Result::eSuccess)
        greg::log::Fatal("Vulkan", "Failed to create graphics pipeline!");

    return std::move(pipelines.front());
}

vk::UniqueRenderPass VulkanRenderer::CreateRenderPass()
{
    //Color
    vk::AttachmentDescription colorAttachment({}, swapChain->GetFormat(), vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
                                              vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

    //Subpasses
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef,
                                   nullptr, nullptr, 0, nullptr);

    std::vector<vk::AttachmentDescription> attachments = { colorAttachment };
    vk::RenderPassCreateInfo renderPassCreateInfo({}, static_cast<uint32_t>(attachments.size()), attachments.data(), 1, &subpass, 0, nullptr);

    return logicalDevice->GetVulkanDevice()->createRenderPassUnique(renderPassCreateInfo);
}

void VulkanRenderer::RecordDrawCommand(const vk::UniqueCommandBuffer& commandBuffer, const Color& clearColor, uint32_t imageIndex)
{
    vk::CommandBuffer buffer = *graphicsCommandPool->GetBuffer(0);
    vk::CommandBufferBeginInfo beginInfo;

    vk::Extent2D swapChainExtent = swapChain->GetExtent();

    buffer.begin(beginInfo);

    vk::Rect2D renderArea(vk::Offset2D(0, 0), swapChainExtent);
    vk::ClearValue clearValue(vk::ClearColorValue(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
    vk::RenderPassBeginInfo renderPassBeginInfo(*renderPass, *swapChain->GetFramebuffer(imageIndex), renderArea, 1, &clearValue);

    buffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
    buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline);
    
    vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(swapChainExtent.width), static_cast<float>(swapChainExtent.height), 0.0f, 1.0f);
    buffer.setViewport(0, 1, &viewport);

    vk::Rect2D scissor(vk::Offset2D(0, 0), swapChainExtent);
    buffer.setScissor(0, 1, &scissor);

    buffer.draw(3, 1, 0, 0);

    buffer.endRenderPass();
    buffer.end();
}

void VulkanRenderer::LoadAllPhysicalDevices()
{
    std::vector<vk::PhysicalDevice> vulkanPhysicalDevices = instance->enumeratePhysicalDevices();
    physicalDevices.clear();
    physicalDevices.reserve(vulkanPhysicalDevices.size());

    size_t i = 0;
    for(const vk::PhysicalDevice& vulkanDevice : vulkanPhysicalDevices)
    {
        greg::vulkan::PhysicalDevice physicalDevice(vulkanDevice, surface);
        if(physicalDevice.IsValid())
            physicalDevices.push_back(physicalDevice);
        i++;
    }
}

PhysicalDevice VulkanRenderer::FindPreferredPhysicalDevice()
{
    std::sort(std::begin(physicalDevices), std::end(physicalDevices), std::greater<PhysicalDevice>());
    return physicalDevices.front();
}

vk::UniqueSemaphore VulkanRenderer::CreateUniqueSemaphore(const LogicalDevice& logicalDevice)
{
    vk::SemaphoreCreateInfo createInfo;
    return logicalDevice.GetVulkanDevice()->createSemaphoreUnique(createInfo);
}

vk::UniqueFence VulkanRenderer::CreateFence(const LogicalDevice& logicalDevice, bool startSignaled = false)
{
    vk::FenceCreateInfo createInfo;
    if(startSignaled)
        createInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);
    return logicalDevice.GetVulkanDevice()->createFenceUnique(createInfo);
}

std::vector<const char*> VulkanRenderer::GetRequiredExtensions()
{
    std::vector<const char*> extensions =
    {
    //Disable in shipping builds?
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,

    ////Windows specific
        "VK_KHR_win32_surface",
    ////
    };

    return extensions;
}

//Assumed to be in {targetdirectory}/Shaders
//Also assumed to use .spv extension
std::vector<char> LoadShaderFile(const std::string& fileName)
{
    std::string fullPath = "Shaders/" + fileName + ".spv";
    std::ifstream file(fullPath, std::ios::ate | std::ios::binary);

    if(!file.is_open())
    {
        char errorBuffer[256];
        ////Windows specific
        strerror_s(errorBuffer, sizeof(errorBuffer), errno);
        ////
        greg::log::Fatal("Vulkan Shaders", std::format("Failed trying to open shader file \"{}\"! Reason: {}", fullPath, errorBuffer));
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> fileBuffer(fileSize);

    file.seekg(0);
    file.read(fileBuffer.data(), fileSize);

    file.close();

    return fileBuffer;
}
}
