#include "InitialState.hpp"
#include "Logger.hpp"


using std::make_shared;


void Engine::changeState(shared_ptr<ApplicationState> newState) {
	INFO("Application state change requested");
	nextState = newState;
}


void Engine::stop() {
	running = false;
}


void Engine::run() {
	running = true;
	currentState = make_shared<InitialState>(*this);
	while (running) {
		if (nextState != nullptr) {
			INFO("Changing application state");
			currentState = nextState;
			nextState = nullptr;
		}
		currentState->update();
	}
}


int main(int argc, char** argv) {
	INITIALIZE_LOGGER();
	Engine* engine = new Engine;
	engine->run();
	delete engine;
	FINALIZE_LOGGER();
	return 0;
}
