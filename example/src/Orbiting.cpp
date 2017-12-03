#include "Orbiting.hpp"
#define PI 3.142


using glm::angleAxis;


Orbiting::Orbiting(Transform& _transform)
	: transform(_transform) {

}


void Orbiting::update(float dt) {
	elapsedTime += dt;
	float angle = elapsedTime * 2 * PI / cycleDuration;
	if (counterClockwise) {
		angle *= -1;
	}
	transform.setLocalPosition(pivot + vec3(orbitRadius * -sin(angle), 0.0f, orbitRadius * -cos(angle)));
	transform.setLocalOrientation(angleAxis(angle, vec3(0, 1, 0)));
}


void Orbiting::setCounterClockwise(bool cc) {
	counterClockwise = cc;
}


void Orbiting::setPivot(vec3 newPivot) {
	pivot = newPivot;
}


void Orbiting::setCycleDuration(float duration) {
	cycleDuration = duration;
}


void Orbiting::setOrbitRadius(float radius) {
	orbitRadius = radius;
}


vec3 Orbiting::getPivot() const {
	return pivot;
}


float Orbiting::getCycleDuration() const {
	return cycleDuration;
}


float Orbiting::getOrbitRadius() const {
	return orbitRadius;
}


bool Orbiting::isCounterClockwise() const {
	return counterClockwise;
}
