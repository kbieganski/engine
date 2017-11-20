#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Properties.hpp"
#include "SwapChain.hpp"


class ApplicationLauncher {
public:
	void launch();


private:
	void createWindow();
	void destroyWindow();

	uvec2 screenSize;
	GLFWwindow* window;
	shared_ptr<GraphicsContext> graphicsContext;
	shared_ptr<SwapChain> swapChain;
	Properties settings;
};
