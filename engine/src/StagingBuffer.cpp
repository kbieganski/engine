#include <algorithm>
#include <cstring>
#include "Logger.hpp"
#include "StagingBuffer.hpp"
#include "Texture.hpp"


using std::min;


StagingBuffer::StagingBuffer(shared_ptr<const GraphicsContext> context, size_t size, const void* data)
	:	GraphicsBuffer(context, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
	fillMemory(data);
}


void StagingBuffer::copyTo(const Texture& texture) {
	auto commandBuffer = context->createCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	VkBufferImageCopy region = {};
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageExtent = { texture.getSize().x, texture.getSize().y, 1 };
	vkCmdCopyBufferToImage(commandBuffer, handle, texture.getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	vkEndCommandBuffer(commandBuffer);
	context->submit(commandBuffer);
	context->waitForGraphicsQueue();
	context->destroy(commandBuffer);
	INFO("Copied the contents of staging buffer ", handle, " to texture ", texture.getImage());
}


void StagingBuffer::copyTo(const GraphicsBuffer& buffer) {
	auto commandBuffer = context->createCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	VkBufferCopy bufferCopy = {};
	bufferCopy.size = min(size, buffer.getSize());
	vkCmdCopyBuffer(commandBuffer, handle, buffer.getHandle(), 1, &bufferCopy);
	vkEndCommandBuffer(commandBuffer);
	context->submit(commandBuffer);
	context->waitForGraphicsQueue();
	context->destroy(commandBuffer);
	INFO("Copied the contents of staging buffer ", handle, " to buffer ", buffer.getHandle());
}


void StagingBuffer::fillMemory(const void* dataToStage) {
	void* stagedData;
	vkMapMemory(context->getDevice(), memory, 0, size, 0, &stagedData);
	memcpy(stagedData, dataToStage, size);
	vkUnmapMemory(context->getDevice(), memory);
}
