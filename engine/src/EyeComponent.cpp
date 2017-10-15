#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtx/transform.hpp>
#include "EyeComponent.hpp"


using std::make_shared;
using glm::lookAt;
using glm::normalize;
using glm::perspective;
using glm::radians;


EyeComponent::EyeComponent(const TransformComponent& _transform)
	:	transform(_transform) {
	aspectRatio = 1;
	fov = radians(45.0f);
	near = 0.1;
	far = 100;
	localDirection = vec3(0, 0, 1);
}


void EyeComponent::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
}


void EyeComponent::setFieldOfView(float fov) {
	this->fov = fov;
}


void EyeComponent::setNearPlane(float near) {
	this->near = near;
}


void EyeComponent::setFarPlane(float far) {
	this->far = far;
}


void EyeComponent::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


float EyeComponent::getAspectRatio() const {
	return aspectRatio;
}


float EyeComponent::getFieldOfView() const {
	return fov;
}


float EyeComponent::getNearPlane() const {
	return near;
}


float EyeComponent::getFarPlane() const {
	return far;
}


vec3 EyeComponent::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 EyeComponent::getLocalDirection() const {
	return localDirection;
}


mat4 EyeComponent::getViewTransform() const {
	vec3 position = transform.getPosition();
	return lookAt(position, position + getDirection(), vec3(0, 1, 0));
}


mat4 EyeComponent::getProjectionTransform() const {
	auto projection = perspective(fov, aspectRatio, near, far);
	projection[1][1] *= -1;
	return projection;
}


mat4 EyeComponent::getViewProjectionTransform() const {
	return getProjectionTransform() * getViewTransform();
}
