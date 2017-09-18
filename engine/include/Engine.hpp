#pragma once
#include <memory>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "ApplicationState.hpp"


using std::shared_ptr;


class Engine {
public:
	void changeState(shared_ptr<ApplicationState> newState);
	void stop();

private:
	void run();

	bool running;
	shared_ptr<ApplicationState> currentState;
	shared_ptr<ApplicationState> nextState;
	GLFWwindow* window;

	friend int main(int argc, char** argv);
};
