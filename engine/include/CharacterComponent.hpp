#pragma once
#include "RigidBodyComponent.hpp"


using glm::vec2;


class CharacterComponent {
public:
	CharacterComponent(RigidBodyComponent& rigidBody);

	void move(vec2 movement);
	void update();

	void setLocalDirection(vec3 direction);

	vec3 getLocalDirection();
	vec3 getDirection();

private:
	vec3 direction;
	RigidBodyComponent& rigidBody;
};
