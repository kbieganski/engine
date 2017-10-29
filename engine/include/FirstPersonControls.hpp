#pragma once
#include "CharacterComponent.hpp"
#include "CameraComponent.hpp"
#include "UserInput.hpp"


using glm::vec2;


class FirstPersonControls {
public:
	FirstPersonControls(CharacterComponent& character, CameraComponent& camera, UserInput<bool>& press, UserInput<float>& direction, UserInput<vec2>& cursor);

	void update(float dt);

private:
	void updateDirection(float dt);
	void updateMovement();

	CharacterComponent& character;
	CameraComponent& camera;
	UserInput<bool>& press;
	UserInput<float>& direction;
	UserInput<vec2>& cursor;
};
