#pragma once
#include <set>
#include "GraphicsDeviceDescription.hpp"


using std::set;


class GraphicsContext {
public:
	GraphicsContext(const GraphicsDeviceDescription& deviceDescription);
	GraphicsContext(const GraphicsContext&) = delete;
	GraphicsContext(GraphicsContext&& moved);
	~GraphicsContext();

	GraphicsContext& operator=(const GraphicsContext&) = delete;
	GraphicsContext& operator=(GraphicsContext&& moved);

	uint32_t findMemoryType(VkMemoryPropertyFlags memoryPropertyFlags, uint32_t typeFilter) const;
	VkCommandBuffer createCommandBuffer(VkCommandBufferUsageFlagBits usageFlags) const;
	VkCommandBuffer createCommandBuffer() const;
	void destroy(VkCommandBuffer commandBuffer) const;
	void submit(VkCommandBuffer commandBuffer) const;
	void submit(const VkSubmitInfo& submitInfo) const;
	void present(const VkPresentInfoKHR& presentInfo) const;
	void waitForGraphicsQueue() const;
	void waitForPresentationQueue() const;

	VkDevice getDevice() const;


private:
	void createLogicalDevice(const GraphicsDeviceDescription& deviceDescription);
	void createCommandPool(const GraphicsDeviceDescription& deviceDescription);
	vector<VkDeviceQueueCreateInfo> makeQueueCreateInfos(set<uint32_t> queueFamilyIndices) const;

	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;
	VkCommandPool commandPool;
	VkPhysicalDeviceMemoryProperties memoryProperties;
};