#pragma once
#include <GLFW/glfw3.h>
#include <UserInput.hpp>


class KeyboardInput {
public:
	enum Direction {
		POSITIVE,
		NEGATIVE
	};

	KeyboardInput(GLFWwindow* window, UserInput<float>& directionInput, UserInput<bool>& pressInput);
	KeyboardInput(const KeyboardInput&) = delete;
	KeyboardInput(KeyboardInput&& moved);
	~KeyboardInput();

	KeyboardInput& operator=(const KeyboardInput&) = delete;
	KeyboardInput& operator=(KeyboardInput&&) = delete;

	void bindKey(const string& binding, Direction direction, int key);


private:
	struct Binding {
		string name;
		Direction direction;
	};

	void setupKeyCallback();

	GLFWwindow* window;
	unordered_map<int, Binding> keys;
	UserInput<float>& directionInput;
	UserInput<bool>& pressInput;
};
