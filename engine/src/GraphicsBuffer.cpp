#include "GraphicsBuffer.hpp"
#include "Logger.hpp"


using std::move;


GraphicsBuffer::GraphicsBuffer(shared_ptr<const GraphicsContext> context, size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties) {
	this->context = context;
	this->size = size;
	createBuffer(usage);
	allocateMemory(memoryProperties);
}


GraphicsBuffer::GraphicsBuffer(GraphicsBuffer&& moved) {
	*this = move(moved);
}


GraphicsBuffer::~GraphicsBuffer() {
	if (handle != VK_NULL_HANDLE) {
		vkDestroyBuffer(context->getDevice(), handle, nullptr);
		INFO("Destroyed graphics buffer ", handle);
	}
	if (memory != VK_NULL_HANDLE) {
		vkFreeMemory(context->getDevice(), memory, nullptr);
		INFO("Freed memory for graphics buffer ", handle);
	}
}


GraphicsBuffer& GraphicsBuffer::operator=(GraphicsBuffer&& moved) {
	size = moved.size;
	handle = moved.handle;
	memory = moved.memory;
	context = moved.context;
	moved.handle = VK_NULL_HANDLE;
	moved.memory = VK_NULL_HANDLE;
	return *this;
}


size_t GraphicsBuffer::getSize() const {
	return size;
}


VkBuffer GraphicsBuffer::getHandle() const {
	return handle;
}


VkDeviceMemory GraphicsBuffer::getMemory() const {
	return memory;
}


void GraphicsBuffer::createBuffer(VkBufferUsageFlags usage) {
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(context->getDevice(), &bufferCreateInfo, nullptr, &handle) != VK_SUCCESS) {
		ERROR("Failed to create graphics buffer");
	}
	INFO("Created graphics buffer ", handle);
}


void GraphicsBuffer::allocateMemory(VkMemoryPropertyFlags memoryProperties) {
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(context->getDevice(), handle, &memoryRequirements);
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = context->findMemoryType(memoryProperties, memoryRequirements.memoryTypeBits);
	if (vkAllocateMemory(context->getDevice(), &memoryAllocateInfo, nullptr, &memory)) {
		ERROR("Failed to allocate memory for a graphics buffer");
	}
	INFO("Allocated ", memoryAllocateInfo.allocationSize, " B of memory for graphics buffer ", handle);
	vkBindBufferMemory(context->getDevice(), handle, memory, 0);
}
