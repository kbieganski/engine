#include <glm/gtc/quaternion.hpp>
#include "FirstPersonControls.hpp"
#include "Logger.hpp"


using std::abs;
using glm::angleAxis;
using glm::cross;
using glm::normalize;


FirstPersonControls::FirstPersonControls(Character& _character, Camera& _camera, UserInput<bool>& _press, UserInput<float>& _direction, UserInput<vec2>& _cursor)
	:	character(_character),
		camera(_camera),
		press(_press),
		direction(_direction),
		cursor(_cursor) {

}


void FirstPersonControls::update(float dt) {
	updateDirection(dt);
	updateMovement();
}


void FirstPersonControls::setMouseSensitivity(float sensitivity) {
	this->sensitivity = sensitivity;
}


float FirstPersonControls::getMouseSensitivity() const {
	return sensitivity;
}


void FirstPersonControls::updateMovement() {
	auto movement = vec2(direction.get("movement_horizontal"), direction.get("movement_vertical"));
	character.move(movement);
	if (press.get("jump")) {
		character.jump();
	}
}


void FirstPersonControls::updateDirection(float dt) {
	auto cameraDirection = camera.getDirection();
	auto forward = normalize(vec3(cameraDirection.x, 0, cameraDirection.z));
	auto right = cross(vec3(0, 1, 0), forward);
	auto rotationAngles = getRotationAngles(dt);
	auto pitchRotation = angleAxis(-rotationAngles.y, right);
	auto yawRotation = angleAxis(rotationAngles.x, vec3(0, 1, 0));
	auto rotation = yawRotation * pitchRotation;
	auto newCameraDirection = rotation * cameraDirection;
	camera.setLocalDirection(newCameraDirection);
	auto newCharDirection = yawRotation * forward;
	character.setLocalDirection(newCharDirection);
}


vec2 FirstPersonControls::getRotationAngles(float dt) const {
	vec2 rotationAngles(direction.get("look_horizontal"), direction.get("look_vertical"));
	auto cursorPosition = cursor.get("mouselook");
	cursorPosition.x *= camera.getAspectRatio();
	cursorPosition *= 100;
	if (abs(cursorPosition.x) > abs(rotationAngles.x)) {
		rotationAngles.x = cursorPosition.x;
	}
	if (abs(cursorPosition.y) > abs(rotationAngles.y)) {
		rotationAngles.y = cursorPosition.y;
	}
	return rotationAngles * dt * sensitivity;
}
