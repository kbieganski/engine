#pragma once
#include "Engine.hpp"


class InitialState : public ApplicationState {
public:
	InitialState(Engine& engine);

	virtual void update() {}

private:
	void main();

	Engine& engine;
};
