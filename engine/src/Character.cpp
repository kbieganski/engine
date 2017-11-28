#include "Character.hpp"


using std::make_shared;
using glm::cross;
using glm::length;
using glm::normalize;


Character::Character(RigidBody& _rigidBody)
	:	rigidBody(_rigidBody) {
	rigidBody.setShape(make_shared<btCapsuleShape>(0.4, 1.8));
	rigidBody.setMass(80);
	rigidBody.setAngularFactor(0);
}


void Character::move(vec2 movement) {
	if (rigidBody.isOnGround()) {
		auto forward = direction;
		auto right = cross(vec3(0, 1, 0), forward);
		auto forceDirection = forward * movement.y + right * movement.x;
		if (length(forceDirection) > 1) {
			forceDirection = normalize(forceDirection);
		}
		auto force = forceDirection * acceleration * rigidBody.getMass();
		rigidBody.applyForce(force);
	}
}


void Character::jump() {
	if (rigidBody.isOnGround()) {
		auto velocity = rigidBody.getLinearVelocity();
		velocity.y = jumpSpeed;
		rigidBody.setLinearVelocity(velocity);
	}
}


void Character::update() {
	auto velocity = rigidBody.getLinearVelocity();
	auto speed = btVector3(velocity.x, 0, velocity.z).length();
	if (speed > maxSpeed) {
		velocity.x = velocity.x * maxSpeed / speed;
		velocity.z = velocity.z * maxSpeed / speed;
		rigidBody.setLinearVelocity(velocity);
	}
	if (rigidBody.isOnGround()) {
		rigidBody.getBody().setDamping(0.99, 0);
	} else {
		rigidBody.getBody().setDamping(0, 0);
	}
}


void Character::setAcceleration(float acceleration) {
	this->acceleration = acceleration;
}


void Character::setJumpSpeed(float jumpSpeed) {
	this->jumpSpeed = jumpSpeed;
}


void Character::setLocalDirection(vec3 newDirection) {
	direction = newDirection;
}


void Character::setMaximumSpeed(float maxSpeed) {
	this->maxSpeed = maxSpeed;
}


float Character::getAcceleration() const {
	return acceleration;
}


vec3 Character::getDirection() const {
	return direction;
}


float Character::getJumpSpeed() const {
	return jumpSpeed;
}


vec3 Character::getLocalDirection() const {
	return direction;
}


float Character::getMaximumSpeed() const {
	return maxSpeed;
}
