#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>


using glm::uvec2;
using std::vector;


class GraphicsDeviceDescription {
public:
	GraphicsDeviceDescription(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkFormat findSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	uint32_t rateDevice() const;
	uvec2 chooseScreenSize(uvec2 size) const;

	uint32_t getGraphicsQueueFamilyIndex() const;
	uint32_t getPresentationQueueFamilyIndex() const;
	VkPhysicalDevice getPhysicalDevice() const;
	VkSurfaceFormatKHR getBestSurfaceFormat() const;
	VkSurfaceCapabilitiesKHR getSurfaceCapabilities() const;
	VkPresentModeKHR getBestPresentationMode() const;


private:
	vector<VkSurfaceFormatKHR> findAvailableSurfaceFormats();
	vector<VkPresentModeKHR> findAvailablePresentationModes() const;
	void listExtensions();
	void listQueueFamilies();
	void findGraphicsQueueFamilyIndex();
	void findPresentationQueueFamilyIndex();
	void findBestSurfaceFormat(const vector<VkSurfaceFormatKHR>& formats);
	void findBestPresentationMode(const vector<VkPresentModeKHR>& presentationModes);

	uint32_t graphicsQueueFamilyIndex;
	uint32_t presentationQueueFamilyIndex;
	VkPhysicalDevice physicalDevice;
	VkSurfaceFormatKHR surfaceFormat;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	VkPresentModeKHR presentationMode;
	VkSurfaceKHR surface;
	vector<VkQueueFamilyProperties> queueFamilies;
	vector<VkExtensionProperties> extensions;
};
