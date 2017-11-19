#include "GraphicsDeviceSelector.hpp"
#include "InitialState.hpp"
#include "Logger.hpp"


using std::make_shared;
using std::make_unique;


Engine::Engine(GLFWwindow* window, shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain)
	:	shaders([context](const string& filename) { return new Shader(context, filename); }),
	 	meshes([context](const string& filename) { return new Mesh(context, filename); }),
	 	textures([context](const string& filename) { return new Texture(context, filename); }),
	 	mouse(window, directionInput, pressInput),
	 	cursor(window, swapChain->getScreenSize(), positionInput),
	 	keyboard(window, directionInput, pressInput) {
	this->window = window;
	this->context = context;
	this->swapChain = swapChain;
	gamepads.reserve(GLFW_JOYSTICK_LAST + 1);
	for (uint8_t i = 0; i < GLFW_JOYSTICK_LAST + 1; i++) {
		if (glfwJoystickPresent(i)) {
			gamepads.push_back(GamepadInput(window, i, directionInput, pressInput));
		}
	}
	run();
}

void Engine::changeState(shared_ptr<ApplicationState> newState) {
	INFO("Application state change requested");
	nextState = newState;
}


void Engine::stop() {
	INFO("Engine stop requested");
	glfwSetWindowShouldClose(window, VK_TRUE);
}


shared_ptr<const GraphicsContext> Engine::getGraphicsContext() const {
	return context;
}


shared_ptr<const SwapChain> Engine::getSwapChain() const {
	return swapChain;
}


void Engine::run() {
	running = true;
	currentState = make_shared<InitialState>(*this);
	while (!glfwWindowShouldClose(window)) {
		if (nextState != nullptr) {
			INFO("Changing application state");
			currentState = nextState;
			nextState = nullptr;
		}
		glfwPollEvents();
		for (auto& gamepad : gamepads) {
			gamepad.update();
		}
		currentState->update();
		context->waitForPresentationQueue();
	}
}
