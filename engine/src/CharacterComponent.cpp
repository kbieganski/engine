#include "CharacterComponent.hpp"


using std::make_shared;
using glm::cross;
using glm::normalize;


CharacterComponent::CharacterComponent(RigidBodyComponent& _rigidBody)
	:	rigidBody(_rigidBody) {
	rigidBody.setShape(make_shared<btCapsuleShape>(0.4, 2));
	rigidBody.setMass(80);
	rigidBody.getBody().setAngularFactor(0);
	rigidBody.getBody().setFriction(2.5);
}


void CharacterComponent::move(vec2 movement) {
	auto forward = direction;
	auto right = cross(forward, vec3(0, 1, 0));
	auto forceDirection = forward * movement.y + right * movement.x;
	if (forceDirection != vec3(0)) {
		forceDirection = normalize(forceDirection);
		auto force = forceDirection * 2000.0f;
		rigidBody.applyForce(force);
	}
}


void CharacterComponent::update() {
	auto velocity = rigidBody.getBody().getLinearVelocity();
	auto speed = velocity.length();
	if (speed > 10) {
		velocity *= 10 / speed;
		rigidBody.getBody().setLinearVelocity(velocity);
	}
}


void CharacterComponent::setLocalDirection(vec3 newDirection) {
	direction = newDirection;
}


vec3 CharacterComponent::getLocalDirection() {
	return direction;
}


vec3 CharacterComponent::getDirection() {
	return direction;
}
