#include "GraphicsDeviceSelector.hpp"
#include "InitialState.hpp"
#include "Logger.hpp"


using std::make_shared;
using std::make_unique;


void Engine::changeState(shared_ptr<ApplicationState> newState) {
	INFO("Application state change requested");
	nextState = newState;
}


void Engine::stop() {
	INFO("Engine stop requested");
	glfwSetWindowShouldClose(window, VK_TRUE);
}


shared_ptr<const GraphicsContext> Engine::getGraphicsContext() const {
	return graphicsContext;
}


shared_ptr<const SwapChain> Engine::getSwapChain() const {
	return swapChain;
}


void Engine::run() {
	const uvec2 screenSize(800, 600);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(screenSize.x, screenSize.y, "Game", nullptr, nullptr);
	INFO("Created window");
	{
		auto graphicsDriver = make_shared<GraphicsDriver>(window);
		GraphicsDeviceSelector selector(*graphicsDriver);
		auto deviceDescription = selector.selectBest();
		graphicsContext = make_shared<GraphicsContext>(graphicsDriver, deviceDescription);
		swapChain = make_shared<SwapChain>(graphicsContext, deviceDescription, graphicsDriver->getSurface(), screenSize);
		running = true;
		currentState = make_shared<InitialState>(*this);
		while (!glfwWindowShouldClose(window)) {
			if (nextState != nullptr) {
				INFO("Changing application state");
				currentState = nextState;
				nextState = nullptr;
			}
			glfwPollEvents();
			currentState->update();
			graphicsContext->waitForPresentationQueue();
			swapChain->draw();
		}
		vkDeviceWaitIdle(graphicsContext->getDevice());
	}
	glfwDestroyWindow(window);
	INFO("Destroyed window");
}


int main(int argc, char** argv) {
	INITIALIZE_LOGGER();
	if (!glfwInit()) {
		ERROR("Failed to initialize GLFW");
		FINALIZE_LOGGER();
		return 1;
	}
	Engine* engine = new Engine;
	engine->run();
	delete engine;
	glfwTerminate();
	FINALIZE_LOGGER();
	return 0;
}
