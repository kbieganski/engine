#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include <UserInput.hpp>


using std::vector;


class GamepadInput {
public:
	enum Direction {
		POSITIVE,
		NEGATIVE
	};

	GamepadInput(GLFWwindow* window, int gamepad, UserInput<float>& directionInput, UserInput<bool>& pressInput);

	void bindAxis(const string& binding, Direction direction, int axis);
	void bindPositiveAxis(const string& binding, Direction direction, int axis);
	void bindNegativeAxis(const string& binding, Direction direction, int axis);
	void bindButton(const string& binding, Direction direction, int button);

	void update();

	void setDeadzone(int axis, float deadzone);

	size_t getAxisCount() const;
	size_t getButtonCount() const;
	float getDeadzone(int axis) const;


private:
	struct AxisBinding {
		void update(UserInput<float>& directionInput, UserInput<bool>& pressInput, float newAxisValue);

		float value = 0;
		float deadzone = 0.1;
		string positive = "";
		string negative = "";
		Direction positiveDirection;
		Direction negativeDirection;


	private:
		void updatePositive(UserInput<float>& directionInput, UserInput<bool>& pressInput, float newAxisValue);
		void updateNegative(UserInput<float>& directionInput, UserInput<bool>& pressInput, float newAxisValue);
	};

	struct ButtonBinding {
		void update(UserInput<float>& directionInput, UserInput<bool>& pressInput, bool newButtonPressed);

		bool buttonPressed = false;
		string name = "";
		Direction direction;
	};

	void updateAxes();
	void updateButtons();

	int gamepad;
	float deadzone = 0.1;
	GLFWwindow* window;
	vector<AxisBinding> axes;
	vector<ButtonBinding> buttons;
	UserInput<float>& directionInput;
	UserInput<bool>& pressInput;
};
