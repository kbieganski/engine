#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include <UserInput.hpp>


using std::vector;


class MouseInput {
public:
	enum Direction {
		POSITIVE,
		NEGATIVE
	};

	enum Scroll {
		HORIZONTAL,
		VERTICAL
	};

	MouseInput(GLFWwindow* window, UserInput<float>& directionInput, UserInput<bool>& pressInput);
	MouseInput(const MouseInput&) = delete;
	MouseInput(MouseInput&& moved);
	~MouseInput();

	MouseInput& operator=(const MouseInput&) = delete;
	MouseInput& operator=(MouseInput&&) = delete;

	void bindButton(const string& binding, Direction direction, int button);
	void bindScroll(const string& binding, Direction direction, Scroll scroll);
	void bindPositiveScroll(const string& binding, Direction direction, Scroll scroll);
	void bindNegativeScroll(const string& binding, Direction direction, Scroll scroll);


private:
	struct ButtonBinding {
		string name = "";
		Direction direction;
	};

	struct ScrollBinding {
		void updatePositive(UserInput<float>& directionInput, UserInput<bool>& pressInput);
		void updateNegative(UserInput<float>& directionInput, UserInput<bool>& pressInput);

		string positive = "";
		string negative = "";
		Direction positiveDirection;
		Direction negativeDirection;
	};

	void setupButtonCallback();
	void setupScrollCallback();

	GLFWwindow* window;
	vector<ButtonBinding> buttons;
	vector<ScrollBinding> scrolls;
	UserInput<float>& directionInput;
	UserInput<bool>& pressInput;
};
