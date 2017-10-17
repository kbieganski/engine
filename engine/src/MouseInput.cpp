#include "MouseInput.hpp"
#include "Logger.hpp"


static unordered_map<GLFWwindow*, MouseInput*> mouseInputs;


MouseInput::MouseInput(GLFWwindow* window, UserInput<float>& _directionInput, UserInput<bool>& _pressInput)
	:	directionInput(_directionInput),
		pressInput(_pressInput) {
	this->window = window;
	buttons.resize(GLFW_MOUSE_BUTTON_LAST + 1);
	scrolls.resize(2);
	setupButtonCallback();
	setupScrollCallback();
	mouseInputs.insert(make_pair(window, this));
}


MouseInput::~MouseInput() {
	if (window) {
		mouseInputs.erase(window);
		glfwSetMouseButtonCallback(window, nullptr);
	}
}


MouseInput::MouseInput(MouseInput&& moved)
	:	directionInput(moved.directionInput),
		pressInput(moved.pressInput) {
	window = moved.window;
	buttons = move(moved.buttons);
	scrolls = move(moved.scrolls);
	moved.window = nullptr;
	mouseInputs.at(window) = this;
}


void MouseInput::bindButton(const string& binding, Direction direction, int button) {
	auto window = this->window;
	directionInput.setGetter(binding, [window, button, direction]() {
		if (glfwGetMouseButton(window, button) == GLFW_PRESS) {
			if (direction == POSITIVE) {
				return 1.0f;
			} else {
				return -1.0f;
			}
		} else {
			return 0.0f;
		}
	});
	pressInput.setGetter(binding, [window, button]() {
		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	});
	buttons[button].name = binding;
	buttons[button].direction = direction;
	INFO("Bound mouse button ", button, " to '", binding, '\'');
}


void MouseInput::bindScroll(const string& binding, Direction direction, Scroll scroll) {
	pressInput.setGetter(binding, []() {
			return false;
		});
	directionInput.setGetter(binding, []() {
			return 0.0f;
		});
	scrolls[scroll].positive = binding;
	scrolls[scroll].negative = binding;
	scrolls[scroll].positiveDirection = direction;
	if (direction == POSITIVE) {
		scrolls[scroll].negativeDirection = NEGATIVE;
	} else {
		scrolls[scroll].negativeDirection = POSITIVE;
	}
	INFO("Bound mouse scroll ", scroll, " to '", binding, '\'');
}


void MouseInput::bindPositiveScroll(const string& binding, Direction direction, Scroll scroll) {
	pressInput.setGetter(binding, []() {
			return false;
		});
	directionInput.setGetter(binding, []() {
			return 0.0f;
		});
	scrolls[scroll].positive = binding;
	scrolls[scroll].positiveDirection = direction;
	INFO("Bound positive mouse scroll ", scroll, " to '", binding, '\'');
}


void MouseInput::bindNegativeScroll(const string& binding, Direction direction, Scroll scroll) {
	pressInput.setGetter(binding, []() {
			return false;
		});
	directionInput.setGetter(binding, []() {
			return 0.0f;
		});
	scrolls[scroll].negative = binding;
	scrolls[scroll].negativeDirection = direction;
	INFO("Bound negative mouse scroll ", scroll, " to '", binding, '\'');
}


void MouseInput::ScrollBinding::updatePositive(UserInput<float>& directionInput, UserInput<bool>& pressInput) {
	if (positive != "") {
		pressInput.invokeCallback(positive, true);
		pressInput.invokeCallback(positive, false);
		if (positiveDirection == POSITIVE) {
			directionInput.invokeCallback(positive, 1);
		} else {
			directionInput.invokeCallback(positive, -1);
		}
		directionInput.invokeCallback(positive, 0);
	}
}


void MouseInput::ScrollBinding::updateNegative(UserInput<float>& directionInput, UserInput<bool>& pressInput) {
	if (negative != "") {
		pressInput.invokeCallback(negative, true);
		pressInput.invokeCallback(negative, false);
		if (negativeDirection == NEGATIVE) {
			directionInput.invokeCallback(negative, -1);
		} else {
			directionInput.invokeCallback(negative, 1);
		}
		directionInput.invokeCallback(negative, 0);
	}
}


void MouseInput::setupButtonCallback() {
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int) {
			auto& mouseInput = *mouseInputs.at(window);
			if (mouseInput.buttons[button].name != "") {
				mouseInput.pressInput.invokeCallback(mouseInput.buttons[button].name, action == GLFW_PRESS);
				if (action == GLFW_PRESS) {
					if (mouseInput.buttons[button].direction == POSITIVE) {
						mouseInput.directionInput.invokeCallback(mouseInput.buttons[button].name, 1);
					} else {
						mouseInput.directionInput.invokeCallback(mouseInput.buttons[button].name, -1);
					}
				} else {
					mouseInput.directionInput.invokeCallback(mouseInput.buttons[button].name, 0);
				}
			}
		});
}


void MouseInput::setupScrollCallback() {
	glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
			auto& mouseInput = *mouseInputs.at(window);
			if (x > 0) {
				mouseInput.scrolls[HORIZONTAL].updatePositive(mouseInput.directionInput, mouseInput.pressInput);
			} else if (x < 0) {
				mouseInput.scrolls[HORIZONTAL].updateNegative(mouseInput.directionInput, mouseInput.pressInput);
			}
			if (y > 0) {
				mouseInput.scrolls[VERTICAL].updatePositive(mouseInput.directionInput, mouseInput.pressInput);
			} else if (y < 0) {
				mouseInput.scrolls[VERTICAL].updateNegative(mouseInput.directionInput, mouseInput.pressInput);
			}
		});
}
