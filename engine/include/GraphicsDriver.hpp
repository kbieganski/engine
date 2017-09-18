#pragma once
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


using std::vector;


class GraphicsDriver {
public:
	GraphicsDriver(GLFWwindow* window);
	GraphicsDriver(const GraphicsDriver&) = delete;
	GraphicsDriver(GraphicsDriver&& moved);
	~GraphicsDriver();

	GraphicsDriver& operator=(const GraphicsDriver&) = delete;
	GraphicsDriver& operator=(GraphicsDriver&& moved);

	VkInstance getInstance() const;
	VkSurfaceKHR getSurface() const;

	static const vector<const char*>& getExtensionNames();
#ifndef NDEBUG
	static const vector<const char*>& getValidationLayerNames();
#endif


private:
	VkApplicationInfo makeApplicationInfo() const;
	VkInstanceCreateInfo makeInstanceCreateInfo(VkApplicationInfo* applicationInfo) const;
	vector<const char*> listRequiredExtensions() const;
#ifndef NDEBUG
	void setupDebugCallback();
	bool areValidationLayersAvailable() const;
#endif

	VkInstance instance;
	VkSurfaceKHR surface;
#ifndef NDEBUG
	VkDebugReportCallbackEXT debugCallbackHandle;
#endif
};
