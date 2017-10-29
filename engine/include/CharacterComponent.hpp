#pragma once
#include "RigidBodyComponent.hpp"


using glm::vec2;


class CharacterComponent {
public:
	CharacterComponent(RigidBodyComponent& rigidBody);

	void move(vec2 movement);
	void jump();
	void update();

	void setAcceleration(float acceleration);
	void setJumpSpeed(float maxSpeed);
	void setLocalDirection(vec3 direction);
	void setMaximumSpeed(float maxSpeed);

	float getAcceleration() const;
	vec3 getDirection() const;
	float getJumpSpeed() const;
	vec3 getLocalDirection() const;
	float getMaximumSpeed() const;


private:
	float acceleration = 36;
	float maxSpeed = 10;
	float jumpSpeed = 4;
	vec3 direction;
	RigidBodyComponent& rigidBody;
};
