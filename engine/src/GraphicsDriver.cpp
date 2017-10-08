#include <algorithm>
#include <cstring>
#include "GraphicsDriver.hpp"
#include "Logger.hpp"


using std::any_of;
using std::move;


#ifndef NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char* message,	void*) {
	WARNING("Validation layer message: ", message);
	return VK_FALSE;
}
#endif


GraphicsDriver::GraphicsDriver(GLFWwindow* window) {
#ifndef NDEBUG
	if (!areValidationLayersAvailable()) {
		ERROR("Validation layers unavailable");
	}
#endif
	auto extensions = listRequiredExtensions();
	auto applicationInfo = makeApplicationInfo();
	auto createInfo = makeInstanceCreateInfo(&applicationInfo);
	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		ERROR("Failed to create a Vulkan instance");
	}
	INFO("Created Vulkan instance ", instance);
#ifndef NDEBUG
	setupDebugCallback();
#endif
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		ERROR("Failed to create a window surface");
	}
	INFO("Created window surface ", surface);
}


GraphicsDriver::GraphicsDriver(GraphicsDriver&& moved) {
	*this = move(moved);
}


GraphicsDriver::~GraphicsDriver() {
	if (surface != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(instance, surface, nullptr);
		INFO("Destroyed Vulkan surface ", surface);
	}
#ifndef NDEBUG
	if (debugCallbackHandle != VK_NULL_HANDLE) {
		auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		if (vkDestroyDebugReportCallbackEXT != nullptr) {
			vkDestroyDebugReportCallbackEXT(instance, debugCallbackHandle, nullptr);
		}
		INFO("Destroyed debug callback ", debugCallbackHandle);
	}
#endif
	if (instance != VK_NULL_HANDLE) {
		vkDestroyInstance(instance, nullptr);
		INFO("Destroyed window instance ", instance);
	}
}


GraphicsDriver & GraphicsDriver::operator=(GraphicsDriver&& moved) {
	instance = moved.instance;
	surface = moved.surface;
	moved.instance = VK_NULL_HANDLE;
	moved.surface = VK_NULL_HANDLE;
#ifndef NDEBUG
	debugCallbackHandle = moved.debugCallbackHandle;
	moved.debugCallbackHandle = VK_NULL_HANDLE;
#endif
	return *this;
}


VkInstance GraphicsDriver::getInstance() const {
	return instance;
}


VkSurfaceKHR GraphicsDriver::getSurface() const {
	return surface;
}


const vector<const char*>& GraphicsDriver::getExtensionNames() {
	static const vector<const char*> extensionNames = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	return extensionNames;
}


#ifndef NDEBUG
const vector<const char*>& GraphicsDriver::getValidationLayerNames() {
	static const vector<const char*> validationLayerNames = {
		"VK_LAYER_LUNARG_standard_validation"
	};
	return validationLayerNames;
}
#endif


VkApplicationInfo GraphicsDriver::makeApplicationInfo() const {
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "Game";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "Engine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;
	return applicationInfo;
}


VkInstanceCreateInfo GraphicsDriver::makeInstanceCreateInfo(VkApplicationInfo* applicationInfo) const {
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = applicationInfo;
#ifndef NDEBUG
	auto& validationLayerNames = getValidationLayerNames();
	instanceCreateInfo.enabledLayerCount = validationLayerNames.size();
	instanceCreateInfo.ppEnabledLayerNames = validationLayerNames.data();
#endif
	return instanceCreateInfo;
}


vector<const char*> GraphicsDriver::listRequiredExtensions() const {
	vector<const char*> extensions;
	uint32_t glfwExtensionCount = 0;
	auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
#ifdef NDEBUG
	extensions.reserve(glfwExtensionCount);
#else
	extensions.reserve(glfwExtensionCount + 1);
	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif
	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}
	return extensions;
}


#ifndef NDEBUG
void GraphicsDriver::setupDebugCallback() {
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;
	auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (vkCreateDebugReportCallbackEXT == nullptr) {
		ERROR("Could not load vkCreateDebugReportCallbackEXT");
	}
	else if (vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &debugCallbackHandle) != VK_SUCCESS) {
		ERROR("Failed to setup debug callback");
	}
	INFO("Created debug callback ", debugCallbackHandle);
}


bool GraphicsDriver::areValidationLayersAvailable() const {
	unsigned layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	auto& validationLayerNames = getValidationLayerNames();
	for (auto layerName : validationLayerNames) {
		auto nameMatches = [layerName](const auto& layer) {
			return strcmp(layerName, layer.layerName) == 0;
		};
		if (!any_of(availableLayers.begin(), availableLayers.end(), nameMatches)) {
			return false;
		}
	}
	return true;
}
#endif
