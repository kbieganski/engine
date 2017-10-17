#include "KeyboardInput.hpp"
#include "Logger.hpp"


static unordered_map<GLFWwindow*, KeyboardInput*> keyboardInputs;


KeyboardInput::KeyboardInput(GLFWwindow* window, UserInput<float>& _directionInput, UserInput<bool>& _pressInput)
	:	directionInput(_directionInput),
		pressInput(_pressInput) {
	this->window = window;
	setupKeyCallback();
	keyboardInputs.insert(make_pair(window, this));
}


KeyboardInput::KeyboardInput(KeyboardInput&& moved)
	:	directionInput(moved.directionInput),
		pressInput(moved.pressInput) {
	window = moved.window;
	keys = move(moved.keys);
	moved.window = nullptr;
	keyboardInputs.at(window) = this;
}


KeyboardInput::~KeyboardInput() {
	if (window) {
		keyboardInputs.erase(window);
		glfwSetKeyCallback(window, nullptr);
	}
}


void KeyboardInput::bindKey(const string& binding, Direction direction, int key) {
	auto window = this->window;
	directionInput.setGetter(binding, [window, key, direction]() {
		if (glfwGetKey(window, key) == GLFW_PRESS) {
			if (direction == POSITIVE) {
				return 1.0f;
			} else {
				return -1.0f;
			}
		} else {
			return 0.0f;
		}
	});
	pressInput.setGetter(binding, [window, key]() {
		return glfwGetKey(window, key) == GLFW_PRESS;
	});
	auto it = keys.find(key);
	if (it != keys.end()) {
		it->second.name = binding;
		it->second.direction = direction;
	} else {
		keys.insert(make_pair(key, Binding{ binding, direction }));
	}
	INFO("Bound ", glfwGetKeyName(key, 0), " to '", binding, '\'');
}


void KeyboardInput::setupKeyCallback() {
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
		auto& keyboardInput = *keyboardInputs.at(window);
		auto it = keyboardInput.keys.find(key);
		if (it != keyboardInput.keys.end()) {
			keyboardInput.pressInput.invokeCallback(it->second.name, action == GLFW_PRESS);
			if (action == GLFW_PRESS) {
				if (it->second.direction == POSITIVE) {
					keyboardInput.directionInput.invokeCallback(it->second.name, 1);
				} else {
					keyboardInput.directionInput.invokeCallback(it->second.name, -1);
				}
			} else {
				keyboardInput.directionInput.invokeCallback(it->second.name, 0);
			}
		}
	});
}
