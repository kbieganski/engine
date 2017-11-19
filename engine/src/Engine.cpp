#include "GraphicsDeviceSelector.hpp"
#include "InitialState.hpp"
#include "Logger.hpp"
#include "Properties.hpp"


using std::make_shared;
using std::make_unique;


Engine::Engine(GLFWwindow* window, shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain)
	:	shaders([context](const string& filename) { return new Shader(context, filename); }),
	 	meshes([context](const string& filename) { return new Mesh(context, filename); }),
	 	textures([context](const string& filename) { return new Texture(context, filename); }),
	 	materials([this](const string& filename) {
				Properties properties(filename);
				auto texture = textures.load(properties.get<string>("texture"));
				auto specularRed = properties.get<float>("specularRed");
				auto specularGreen = properties.get<float>("specularGreen");
				auto specularBlue = properties.get<float>("specularBlue");
				auto specularHardness = properties.get<float>("specularHardness");
				return new Material(this->context, texture, vec3(specularRed, specularGreen, specularBlue), specularHardness);
			}),
		models([this](const string& filename) {
				Properties properties(filename);
				auto mesh = meshes.load(properties.get<string>("mesh"));
				auto material = materials.load(properties.get<string>("material"));
				return new Model(mesh, material);
			}),
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
