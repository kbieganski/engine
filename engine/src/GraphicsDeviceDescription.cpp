#include <algorithm>
#include "GraphicsDeviceDescription.hpp"
#include "GraphicsDriver.hpp"
#include "Logger.hpp"


using std::any_of;
using std::find_if;
using std::max;
using std::min;
using std::numeric_limits;


GraphicsDeviceDescription::GraphicsDeviceDescription(VkPhysicalDevice device, VkSurfaceKHR surface) {
	physicalDevice = device;
	this->surface = surface;
	auto presentationModes = findAvailablePresentationModes();
	auto surfaceFormats = findAvailableSurfaceFormats();
	listExtensions();
	listQueueFamilies();
	findGraphicsQueueFamilyIndex();
	findPresentationQueueFamilyIndex();
	findBestPresentationMode(presentationModes);
	findBestSurfaceFormat(surfaceFormats);
}


VkFormat GraphicsDeviceDescription::findSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	ERROR("Failed to find supported format!");
}



static bool isSuitableGraphicsQueueFamily(const VkQueueFamilyProperties& properties) {
	return properties.queueCount > 0 && properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}


uint32_t GraphicsDeviceDescription::rateDevice() const {
	uint32_t score = 1;
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(physicalDevice, &features);
	if (!features.samplerAnisotropy || !features.geometryShader) {
		return 0;
	}
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1;
	}
	if (!any_of(queueFamilies.begin(), queueFamilies.end(), isSuitableGraphicsQueueFamily)) {
		return 0;
	}
	auto& extensionNames = GraphicsDriver::getExtensionNames();
	for (uint32_t i = 0; i < extensionNames.size(); i++) {
		if (find_if(extensions.begin(), extensions.end(), [i, &extensionNames](const auto& extension) {
			return strcmp(extension.extensionName, extensionNames[i]) == 0; }) == extensions.end()) {
			return 0;
		}
	}
	return score;
}


uvec2 GraphicsDeviceDescription::chooseScreenSize(uvec2 size) const {
	if (surfaceCapabilities.currentExtent.width != numeric_limits<uint32_t>::max()) {
		return uvec2(surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height);
	}
	size.x = max(surfaceCapabilities.minImageExtent.width, min(surfaceCapabilities.maxImageExtent.width, size.x));
	size.y = max(surfaceCapabilities.minImageExtent.height, min(surfaceCapabilities.maxImageExtent.height, size.y));
	return size;
}


uint32_t GraphicsDeviceDescription::getGraphicsQueueFamilyIndex() const {
	return graphicsQueueFamilyIndex;
}


uint32_t GraphicsDeviceDescription::getPresentationQueueFamilyIndex() const {
	return presentationQueueFamilyIndex;
}


VkPhysicalDevice GraphicsDeviceDescription::getPhysicalDevice() const {
	return physicalDevice;
}


VkSurfaceFormatKHR GraphicsDeviceDescription::getBestSurfaceFormat() const {
	return surfaceFormat;
}


VkSurfaceCapabilitiesKHR GraphicsDeviceDescription::getSurfaceCapabilities() const {
	return surfaceCapabilities;
}


VkPresentModeKHR GraphicsDeviceDescription::getBestPresentationMode() const {
	return presentationMode;
}


vector<VkSurfaceFormatKHR> GraphicsDeviceDescription::findAvailableSurfaceFormats() {
	vector<VkSurfaceFormatKHR> surfaceFormats;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		surfaceFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());
	}
	return surfaceFormats;
}


vector<VkPresentModeKHR> GraphicsDeviceDescription::findAvailablePresentationModes() const {
	vector<VkPresentModeKHR> presentationModes;
	uint32_t presentationModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationModeCount, nullptr);
	if (presentationModeCount != 0) {
		presentationModes.resize(presentationModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationModeCount, presentationModes.data());
	}
	return presentationModes;
}


void GraphicsDeviceDescription::listExtensions() {
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
	extensions.resize(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());
}


void GraphicsDeviceDescription::listQueueFamilies() {
	uint32_t familyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, nullptr);
	queueFamilies.resize(familyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, queueFamilies.data());
}


void GraphicsDeviceDescription::findGraphicsQueueFamilyIndex() {
	auto graphicsQueueFamilyIterator = find_if(queueFamilies.begin(), queueFamilies.end(), isSuitableGraphicsQueueFamily);
	graphicsQueueFamilyIndex = graphicsQueueFamilyIterator - queueFamilies.begin();
}


void GraphicsDeviceDescription::findPresentationQueueFamilyIndex() {
	int32_t index = -1;
	for (size_t i = 0; i < queueFamilies.size(); i++) {
		VkBool32 supportsPresentation = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &supportsPresentation);
		if (queueFamilies[i].queueCount > 0 && supportsPresentation) {
			index = i;
		}
	}
	if (index < 0) {
		ERROR("Failed to find suitable presentation queue family");
	}
	presentationQueueFamilyIndex = index;
}


void GraphicsDeviceDescription::findBestSurfaceFormat(const vector<VkSurfaceFormatKHR>& formats) {
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
		surfaceFormat = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	} else {
		for (const auto& format : formats) {
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
				surfaceFormat = format;
				return;
			}
		}
		surfaceFormat = formats[0];
	}
}


void GraphicsDeviceDescription::findBestPresentationMode(const vector<VkPresentModeKHR>& presentationModes) {
	presentationMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& mode : presentationModes) {
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentationMode = mode;
			return;
		} else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			presentationMode = mode;
		}
	}
}
