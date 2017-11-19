#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "SwapChain.hpp"


class ApplicationLauncher {
public:
	void launch();

private:
	GLFWwindow* window;
	shared_ptr<GraphicsContext> graphicsContext;
	shared_ptr<SwapChain> swapChain;
};
