#include "GraphicsContext.hpp"
#include "Logger.hpp"


using std::move;


GraphicsContext::GraphicsContext(shared_ptr<const GraphicsDriver> driver, const GraphicsDeviceDescription& _deviceDescription)
	:	deviceDescription(_deviceDescription) {
	this->driver = driver;
	vkGetPhysicalDeviceMemoryProperties(deviceDescription.getPhysicalDevice(), &memoryProperties);
	createLogicalDevice(deviceDescription);
	createCommandPool(deviceDescription);
}


GraphicsContext::GraphicsContext(GraphicsContext&& moved)
	:	deviceDescription(moved.deviceDescription) {
	*this = move(moved);
}


GraphicsContext::~GraphicsContext() {
	if (commandPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, commandPool, nullptr);
	}
	if (device != VK_NULL_HANDLE) {
		vkDestroyDevice(device, nullptr);
	}
}


GraphicsContext& GraphicsContext::operator=(GraphicsContext&& moved) {
	device = moved.device;
	graphicsQueue = moved.graphicsQueue;
	presentationQueue = moved.presentationQueue;
	commandPool = moved.commandPool;
	memoryProperties = moved.memoryProperties;
	deviceDescription = deviceDescription;
	driver = move(moved.driver);
	moved.device = VK_NULL_HANDLE;
	moved.commandPool = VK_NULL_HANDLE;
	return *this;
}


uint32_t GraphicsContext::findMemoryType(VkMemoryPropertyFlags memoryPropertyFlags, uint32_t typeFilter) const {
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) {
			return i;
		}
	}
	ERROR("Failed to find suitable memory type");
}


VkCommandBuffer GraphicsContext::createCommandBuffer(VkCommandBufferUsageFlagBits usageFlags) const {
	auto commandBuffer = createCommandBuffer();
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = usageFlags;
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}


VkCommandBuffer GraphicsContext::createCommandBuffer() const {
	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandPool = commandPool;
	allocateInfo.commandBufferCount = 1;
	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer);
	INFO("Created command buffer ", commandBuffer);
	return commandBuffer;
}


void GraphicsContext::destroy(VkCommandBuffer commandBuffer) const {
	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	INFO("Destroyed command buffer ", commandBuffer);
}


void GraphicsContext::submit(VkCommandBuffer commandBuffer) const {
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submit(submitInfo);
}


void GraphicsContext::submit(const VkSubmitInfo& submitInfo) const {
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		ERROR("Failed to submit command buffer to graphics queue");
	}
}


void GraphicsContext::present(const VkPresentInfoKHR& presentInfo) const {
	vkQueuePresentKHR(presentationQueue, &presentInfo);
}


void GraphicsContext::waitForGraphicsQueue() const {
	vkQueueWaitIdle(graphicsQueue);
}


void GraphicsContext::waitForPresentationQueue() const {
	vkQueueWaitIdle(presentationQueue);
}


VkDevice GraphicsContext::getDevice() const {
	return device;
}


void GraphicsContext::createLogicalDevice(const GraphicsDeviceDescription& deviceDescription) {
	auto queueCreateInfos = makeQueueCreateInfos({ deviceDescription.getGraphicsQueueFamilyIndex(), deviceDescription.getPresentationQueueFamilyIndex() });
	VkPhysicalDeviceFeatures features = {};
	features.samplerAnisotropy = VK_TRUE;
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.pEnabledFeatures = &features;
	createInfo.enabledExtensionCount = 1;
	createInfo.ppEnabledExtensionNames = GraphicsDriver::getExtensionNames().data();
#ifndef NDEBUG
	INFO("Enabling validation layers");
	auto& validationLayerNames = GraphicsDriver::getValidationLayerNames();
	createInfo.enabledLayerCount = validationLayerNames.size();
	createInfo.ppEnabledLayerNames = validationLayerNames.data();
#endif
	if (vkCreateDevice(deviceDescription.getPhysicalDevice(), &createInfo, nullptr, &device) != VK_SUCCESS) {
		ERROR("Failed to create logical graphics device");
	}
	INFO("Created logical graphics device ", device);
	vkGetDeviceQueue(device, deviceDescription.getGraphicsQueueFamilyIndex(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, deviceDescription.getPresentationQueueFamilyIndex(), 0, &presentationQueue);
}


void GraphicsContext::createCommandPool(const GraphicsDeviceDescription& deviceDescription) {
	VkCommandPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolCreateInfo.queueFamilyIndex = deviceDescription.getPresentationQueueFamilyIndex();
	poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	if (vkCreateCommandPool(device, &poolCreateInfo, nullptr, &commandPool) != VK_SUCCESS) {
		ERROR("Failed to create command pool");
	}
	INFO("Created command pool ", commandPool);
}


vector<VkDeviceQueueCreateInfo> GraphicsContext::makeQueueCreateInfos(set<uint32_t> queueFamilyIndices) const {
	vector<VkDeviceQueueCreateInfo> createInfos;
	createInfos.reserve(queueFamilyIndices.size());
	for (auto queueFamilyIndex : queueFamilyIndices) {
		VkDeviceQueueCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		createInfo.queueFamilyIndex = queueFamilyIndex;
		createInfo.queueCount = 1;
		static float queuePriority = 1;
		createInfo.pQueuePriorities = &queuePriority;
		createInfos.push_back(createInfo);
	}
	return createInfos;
}
