#pragma once
#include "Character.hpp"
#include "Camera.hpp"
#include "UserInput.hpp"


using glm::vec2;


class FirstPersonControls {
public:
	FirstPersonControls(Character& character, Camera& camera, UserInput<bool>& press, UserInput<float>& direction, UserInput<vec2>& cursor);

	void update(float dt);

	void setMouseSensitivity(float sensitivity);

	float getMouseSensitivity() const;


private:
	void updateDirection(float dt);
	void updateMovement();
	vec2 getRotationAngles(float dt) const;

	float sensitivity = 1;
	Character& character;
	Camera& camera;
	UserInput<bool>& press;
	UserInput<float>& direction;
	UserInput<vec2>& cursor;
};
