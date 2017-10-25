#pragma once
#include "CharacterComponent.hpp"
#include "EyeComponent.hpp"
#include "UserInput.hpp"


using glm::vec2;


class FirstPersonControls {
public:
	FirstPersonControls(CharacterComponent& character, EyeComponent& eye, UserInput<float>& direction, UserInput<vec2>& cursor);

	void update(float dt);

private:
	void updateDirection(float dt);
	void updateMovement();

	CharacterComponent& character;
	EyeComponent& eye;
	UserInput<float>& direction;
	UserInput<vec2>& cursor;
};
