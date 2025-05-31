#include "CommandBuffer.h"

#include "MemoryBuffer.h"
#include "../../../Debugging/Log.h"

namespace greg::vulkan::command
{
void CopyBuffer(vk::Buffer source, vk::Buffer destination, vk::DeviceSize size, vk::CommandBuffer commandBuffer)
{
    vk::BufferCopy copyRegion(0, 0, size);
    commandBuffer.copyBuffer(source, destination, 1, &copyRegion);
}

void CopyBuffer(const greg::vulkan::MemoryBuffer& source, const greg::vulkan::MemoryBuffer& destination, vk::DeviceSize size, vk::CommandBuffer commandBuffer)
{
    CopyBuffer(source.GetBuffer(), destination.GetBuffer(), size, commandBuffer);
}

//Sends command buffer instructions to GPU.
//WARNING: Command buffer parameter will become invalid after usage!
void FlushTransientBuffer(vk::UniqueCommandBuffer commandBuffer, vk::Queue queue)
{
    commandBuffer->end();

    vk::SubmitInfo submitInfo(0, nullptr, {}, 1, &*commandBuffer);
    if(queue.submit(submitInfo) != vk::Result::eSuccess)
        greg::log::Fatal("Vulkan", "Failed to submit transient buffer!");

    queue.waitIdle();
}
}
