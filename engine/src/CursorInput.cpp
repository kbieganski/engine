#include "CursorInput.hpp"
#include "Logger.hpp"


static unordered_map<GLFWwindow*, CursorInput*> cursorInputs;


static vec2 transformCoordinates(vec2 coords, uvec2 windowSize) {
	return vec2(coords.x * 2 / windowSize.x - 1, 1 - coords.y * 2 / windowSize.y);
}


CursorInput::CursorInput(GLFWwindow* window, uvec2 windowSize, UserInput<vec2>& _positionInput)
	:	positionInput(_positionInput) {
	this->window = window;
	this->windowSize = windowSize;
	setupMovementCallback();
	cursorInputs.insert(make_pair(window, this));
}


CursorInput::CursorInput(CursorInput&& moved)
	:	positionInput(moved.positionInput) {
	movementBinding = moved.movementBinding;
	windowSize = moved.windowSize;
	window = moved.window;
	moved.window = nullptr;
	cursorInputs.at(window) = this;
}


CursorInput::~CursorInput() {
	if (window) {
		cursorInputs.erase(window);
		glfwSetCursorPosCallback(window, nullptr);
	}
}


void CursorInput::bindMovement(const string& binding) {
	positionInput.setGetter(binding, [this]() {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return transformCoordinates(vec2(x, y), windowSize);
	});
	movementBinding = binding;
	INFO("Bound cursor movement to '", binding, '\'');
}


void CursorInput::setupMovementCallback() {
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
		auto& cursorInput = *cursorInputs.at(window);
		if (cursorInput.movementBinding != "") {
			auto position = transformCoordinates(vec2(x, y), cursorInput.windowSize);
			cursorInput.positionInput.invokeCallback(cursorInput.movementBinding, position);
		}
	});
}
