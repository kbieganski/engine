#include "ApplicationLauncher.hpp"
#include "Engine.hpp"
#include "GraphicsDeviceSelector.hpp"
#include "Logger.hpp"


using std::make_shared;
using std::make_unique;


void ApplicationLauncher::launch() {
	if (!glfwInit()) {
		ERROR("Failed to initialize GLFW");
	}
	const uvec2 screenSize(1280, 720);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(screenSize.x, screenSize.y, "Game", nullptr, nullptr);
	INFO("Created window");
	{
		auto graphicsDriver = make_shared<GraphicsDriver>(window);
		GraphicsDeviceSelector selector(*graphicsDriver);
		graphicsContext = make_shared<GraphicsContext>(graphicsDriver, selector.selectBest());
		swapChain = make_shared<SwapChain>(graphicsContext, graphicsDriver->getSurface(), screenSize);
		Engine engine(window, graphicsContext, swapChain);
		vkDeviceWaitIdle(graphicsContext->getDevice());
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	INFO("Destroyed window");
}


int main(int argc, char** argv) {
	INITIALIZE_LOGGER();
	ApplicationLauncher launcher;
	try {
		launcher.launch();
	} catch (exception&) {
		FINALIZE_LOGGER();
		return 1;
	}
	FINALIZE_LOGGER();
	return 0;
}
