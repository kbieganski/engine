#pragma once
#include "GraphicsContext.hpp"


class GraphicsBuffer {
public:
	GraphicsBuffer(shared_ptr<const GraphicsContext> context, size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties);
	GraphicsBuffer(const GraphicsBuffer&) = delete;
	GraphicsBuffer(GraphicsBuffer&& moved);
	~GraphicsBuffer();

	GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
	GraphicsBuffer& operator=(GraphicsBuffer&& moved);

	size_t getSize() const;
	VkBuffer getHandle() const;
	VkDeviceMemory getMemory() const;


protected:
	void createBuffer(VkBufferUsageFlags usage);
	void allocateMemory(VkMemoryPropertyFlags memoryProperties);

	size_t size;
	VkBuffer handle;
	VkDeviceMemory memory;
	shared_ptr<const GraphicsContext> context;
};
