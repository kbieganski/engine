#pragma once
#include "CharacterComponent.hpp"
#include "CameraComponent.hpp"
#include "UserInput.hpp"


using glm::vec2;


class FirstPersonControls {
public:
	FirstPersonControls(CharacterComponent& character, CameraComponent& camera, UserInput<bool>& press, UserInput<float>& direction, UserInput<vec2>& cursor);

	void update(float dt);

	void setMouseSensitivity(float sensitivity);

	float getMouseSensitivity() const;


private:
	void updateDirection(float dt);
	void updateMovement();

	float sensitivity = 1;
	CharacterComponent& character;
	CameraComponent& camera;
	UserInput<bool>& press;
	UserInput<float>& direction;
	UserInput<vec2>& cursor;
};
