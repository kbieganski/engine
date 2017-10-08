#pragma once
#include <memory>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "ApplicationState.hpp"
#include "Renderer.hpp"
#include "SwapChain.hpp"


using std::shared_ptr;


class Engine {
public:
	void changeState(shared_ptr<ApplicationState> newState);
	void stop();

	shared_ptr<const GraphicsContext> getGraphicsContext() const;
	shared_ptr<const Renderer> getRenderer() const;
	shared_ptr<const SwapChain> getSwapChain() const;


private:
	void run();

	bool running;
	GLFWwindow* window;
	shared_ptr<ApplicationState> currentState;
	shared_ptr<ApplicationState> nextState;
	shared_ptr<GraphicsContext> graphicsContext;
	shared_ptr<SwapChain> swapChain;
	shared_ptr<Renderer> renderer;

	friend int main(int argc, char** argv);
};
