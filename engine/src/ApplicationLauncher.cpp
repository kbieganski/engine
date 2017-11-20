#include "ApplicationLauncher.hpp"
#include "Engine.hpp"
#include "GraphicsDeviceSelector.hpp"
#include "Logger.hpp"


using std::make_shared;
using std::make_unique;


void ApplicationLauncher::launch() {
	createWindow();
	auto graphicsDriver = make_shared<GraphicsDriver>(window);
	GraphicsDeviceSelector selector(*graphicsDriver);
	graphicsContext = make_shared<GraphicsContext>(graphicsDriver, selector.selectBest());
	swapChain = make_shared<SwapChain>(graphicsContext, graphicsDriver->getSurface(), screenSize);
	Engine engine(window, graphicsContext, swapChain, settings);
	vkDeviceWaitIdle(graphicsContext->getDevice());
	destroyWindow();
}

void ApplicationLauncher::createWindow() {
	if (!glfwInit()) {
		ERROR("Failed to initialize GLFW");
	}
	try {
		settings.readFromFile("settings");
	} catch (Exception&) {
		INFO("Using default settings");
	}
	screenSize.x = settings.get<uint32_t>("screenWidth", 1280);
	screenSize.y = settings.get<uint32_t>("screenHeight", 720);
	GLFWmonitor* monitor = nullptr;
	if (settings.get<bool>("fullscreen", false)) {
		monitor = glfwGetPrimaryMonitor();
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(screenSize.x, screenSize.y, "Game", monitor, nullptr);
	INFO("Created window");
}


void ApplicationLauncher::destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
	INFO("Destroyed window");
}



int main(int argc, char** argv) {
	INITIALIZE_LOGGER();
	ApplicationLauncher launcher;
	try {
		launcher.launch();
	} catch (Exception&) {
		FINALIZE_LOGGER();
		return 1;
	}
	FINALIZE_LOGGER();
	return 0;
}
