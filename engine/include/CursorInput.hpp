#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <UserInput.hpp>


using glm::vec2;
using glm::uvec2;


class CursorInput {
public:
	CursorInput(GLFWwindow* window, uvec2 windowSize, UserInput<vec2>& positionInput);
	CursorInput(const CursorInput&) = delete;
	CursorInput(CursorInput&& moved);
	~CursorInput();

	CursorInput& operator=(const CursorInput&) = delete;
	CursorInput& operator=(CursorInput&&) = delete;

	void bindMovement(const string& binding);

	void setPosition(vec2 position);


private:
	void setupMovementCallback();

	string movementBinding;
	uvec2 windowSize;
	GLFWwindow* window;
	UserInput<vec2>& positionInput;
};
