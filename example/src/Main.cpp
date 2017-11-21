#include <InitialState.hpp>
#include "ExampleState.hpp"


using std::make_shared;


void InitialState::main() {
	engine.changeState(make_shared<ExampleState>(engine));
}
