#include <glm/gtc/quaternion.hpp>
#include "FirstPersonControls.hpp"


using glm::angleAxis;
using glm::cross;
using glm::normalize;


FirstPersonControls::FirstPersonControls(CharacterComponent& _character, CameraComponent& _camera, UserInput<float>& _direction, UserInput<vec2>& _cursor)
	:	character(_character),
		camera(_camera),
		direction(_direction),
		cursor(_cursor) {

}


void FirstPersonControls::update(float dt) {
	updateDirection(dt);
	updateMovement();
}


void FirstPersonControls::updateMovement() {
	auto movement = vec2(direction.get("horizontal"), -direction.get("vertical"));
	if (movement != vec2(0)) {
		movement = normalize(movement);
		character.move(movement);
	}
}


void FirstPersonControls::updateDirection(float dt) {
	auto cameraDirection = camera.getDirection();
	auto forward = normalize(vec3(cameraDirection.x, 0, cameraDirection.z));
	auto right = cross(forward, vec3(0, 1, 0));
	auto cursorPosition = cursor.get("mouselook");
	cursorPosition.x *= camera.getAspectRatio();
	auto rotationAngles = cursorPosition * 180.0f * dt;
	auto pitchRotation = angleAxis(-rotationAngles.y, right);
	auto yawRotation = angleAxis(rotationAngles.x, vec3(0, 1, 0));
	auto rotation = yawRotation * pitchRotation;
	auto newCameraDirection = rotation * cameraDirection;
	camera.setLocalDirection(newCameraDirection);
	auto newCharDirection = yawRotation * forward;
	character.setLocalDirection(newCharDirection);
}