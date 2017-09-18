#include "GraphicsDriver.hpp"
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


void Engine::run() {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(640, 480, "", nullptr, nullptr);
	INFO("Created window");
	{
		auto graphicsDriver = make_unique<GraphicsDriver>(window);
		running = true;
		currentState = make_shared<InitialState>(*this);
		while (!glfwWindowShouldClose(window)) {
			if (nextState != nullptr) {
				INFO("Changing application state");
				currentState = nextState;
				nextState = nullptr;
			}
			currentState->update();
			glfwPollEvents();
		}
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
