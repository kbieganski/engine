#include "GamepadInput.hpp"
#include "Logger.hpp"


using std::abs;
using std::min;


GamepadInput::GamepadInput(GLFWwindow* window, int gamepad, UserInput<float>& _directionInput, UserInput<bool>& _pressInput)
	:	directionInput(_directionInput),
		pressInput(_pressInput) {
	this->gamepad = gamepad;
	this->window = window;
	update();
}


void GamepadInput::bindAxis(const string& binding, Direction direction, int axis) {
	pressInput.addGetter(binding, [this, axis]() {
			return abs(axes[axis].value) > 0.5;
		});
	directionInput.addGetter(binding, [this, axis]() {
			return axes[axis].value;
		});
	axes[axis].positive = binding;
	axes[axis].negative = binding;
	axes[axis].positiveDirection = direction;
	if (direction == POSITIVE) {
		axes[axis].negativeDirection = NEGATIVE;
	} else {
		axes[axis].negativeDirection = POSITIVE;
	}
	INFO("Bound ", glfwGetJoystickName(gamepad), " axis ", axis, " to '", binding, '\'');
}


void GamepadInput::bindPositiveAxis(const string& binding, Direction direction, int axis) {
	pressInput.addGetter(binding, [this, axis]() {
			return axes[axis].value > 0.5;
		});
	directionInput.addGetter(binding, [this, direction, axis]() {
			if (axes[axis].value > 0) {
				if (direction == POSITIVE) {
					return axes[axis].value;
				} else {
					return -axes[axis].value;
				}
			} else {
				return 0.0f;
			}
		});
	axes[axis].positive = binding;
	axes[axis].positiveDirection = direction;
	INFO("Bound ", glfwGetJoystickName(gamepad), " positive axis ", axis, " to '", binding, '\'');
}


void GamepadInput::bindNegativeAxis(const string& binding, Direction direction, int axis) {
	pressInput.addGetter(binding, [this, axis]() {
			return axes[axis].value < -0.5;
		});
	directionInput.addGetter(binding, [this, direction, axis]() {
			if (axes[axis].value < 0) {
				if (direction == NEGATIVE) {
					return axes[axis].value;
				} else {
					return -axes[axis].value;
				}
			} else {
				return 0.0f;
			}
		});
	axes[axis].negative = binding;
	axes[axis].negativeDirection = direction;
	INFO("Bound ", glfwGetJoystickName(gamepad), " negative axis ", axis, " to '", binding, '\'');
}


void GamepadInput::bindButton(const string& binding, Direction direction, int button) {
	pressInput.addGetter(binding, [this, button]() {
		return buttons[button].buttonPressed;
	});
	directionInput.addGetter(binding, [this, button, direction]() {
		if (buttons[button].buttonPressed) {
			if (direction == POSITIVE) {
				return 1.0f;
			} else {
				return -1.0f;
			}
		} else {
			return 0.0f;
		}
	});
	buttons[button].name = binding;
	buttons[button].direction = direction;
	INFO("Bound ", glfwGetJoystickName(gamepad), " button ", button, " to '", binding, '\'');
}


void GamepadInput::update() {
	updateAxes();
	updateButtons();
}


void GamepadInput::setDeadzone(int axis, float deadzone) {
	axes[axis].deadzone = deadzone;
}


size_t GamepadInput::getAxisCount() const {
	return axes.size();
}


size_t GamepadInput::getButtonCount() const {
	return buttons.size();
}


float GamepadInput::getDeadzone(int axis) const {
	return axes[axis].deadzone;
}


void GamepadInput::AxisBinding::update(UserInput<float>& directionInput, UserInput<bool>& pressInput, float newValue) {
	updatePositive(directionInput, pressInput, newValue);
	updateNegative(directionInput, pressInput, newValue);
	value = newValue;
}


void GamepadInput::AxisBinding::updatePositive(UserInput<float>& directionInput, UserInput<bool>& pressInput, float newValue) {
	if (positive != "") {
		if (newValue >= deadzone) {
			if (value < deadzone) {
				pressInput.invokeCallback(positive, true);
			}
			if (positiveDirection == POSITIVE) {
				directionInput.invokeCallback(positive, newValue);
			} else {
				directionInput.invokeCallback(positive, -newValue);
			}
		} else if (value >= deadzone && newValue < deadzone) {
			pressInput.invokeCallback(positive, false);
			directionInput.invokeCallback(positive, 0);
		}
	}
}


void GamepadInput::AxisBinding::updateNegative(UserInput<float>& directionInput, UserInput<bool>& pressInput, float newValue) {
	if (negative != "") {
		if (newValue <= -deadzone) {
			if (value > -deadzone) {
				pressInput.invokeCallback(negative, true);
			}
			if (negativeDirection == NEGATIVE) {
				directionInput.invokeCallback(negative, newValue);
			} else {
				directionInput.invokeCallback(negative, -newValue);
			}
		} else if (value <= -deadzone && newValue > -deadzone) {
			pressInput.invokeCallback(negative, false);
			directionInput.invokeCallback(negative, 0);
		}
	}
}


void GamepadInput::ButtonBinding::update(UserInput<float>& directionInput, UserInput<bool>& pressInput, bool newButtonPressed) {
	if (name != "") {
		if (!buttonPressed && newButtonPressed) {
			pressInput.invokeCallback(name, true);
			if (direction == POSITIVE) {
				directionInput.invokeCallback(name, 1);
			} else {
				directionInput.invokeCallback(name, -1);
			}
		} else if (buttonPressed && !newButtonPressed) {
			pressInput.invokeCallback(name, false);
			directionInput.invokeCallback(name, 0);
		}
	}
	buttonPressed = newButtonPressed;
}


void GamepadInput::updateAxes() {
	int count;
	const float* newAxes = glfwGetJoystickAxes(gamepad, &count);
	if (static_cast<size_t>(count) != axes.size()) {
		axes.resize(count);
	}
	for (size_t i = 0; i < axes.size(); i++) {
		axes[i].update(directionInput, pressInput, newAxes[i]);
	}
}


void GamepadInput::updateButtons() {
	int count;
	const unsigned char* newButtons = glfwGetJoystickButtons(gamepad, &count);
	if (static_cast<size_t>(count) != buttons.size()) {
		buttons.resize(count);
	}
	for (size_t i = 0; i < buttons.size(); i++) {
		buttons[i].update(directionInput, pressInput, newButtons[i]);
	}
}
