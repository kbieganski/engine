#include <cstring>
#include "UniformBuffer.hpp"


UniformBuffer::UniformBuffer(shared_ptr<const GraphicsContext> context, size_t size)
	:	GraphicsBuffer(context, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {

}


void UniformBuffer::set(const void* uniform) {
	void* stagedData;
	vkMapMemory(context->getDevice(), memory, 0, size, 0, &stagedData);
	memcpy(stagedData, uniform, size);
	vkUnmapMemory(context->getDevice(), memory);
}
