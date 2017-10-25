#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtx/transform.hpp>
#include "CameraComponent.hpp"


using std::make_shared;
using glm::lookAt;
using glm::normalize;
using glm::perspective;
using glm::radians;


CameraComponent::CameraComponent(const TransformComponent& _transform)
	:	transform(_transform) {
	aspectRatio = 1;
	fov = radians(90.0f);
	near = 0.1;
	far = 100;
	localDirection = vec3(0, 0, 1);
}


void CameraComponent::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
}


void CameraComponent::setFieldOfView(float fov) {
	this->fov = fov;
}


void CameraComponent::setNearPlane(float near) {
	this->near = near;
}


void CameraComponent::setFarPlane(float far) {
	this->far = far;
}


void CameraComponent::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


float CameraComponent::getAspectRatio() const {
	return aspectRatio;
}


float CameraComponent::getFieldOfView() const {
	return fov;
}


float CameraComponent::getNearPlane() const {
	return near;
}


float CameraComponent::getFarPlane() const {
	return far;
}


vec3 CameraComponent::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 CameraComponent::getLocalDirection() const {
	return localDirection;
}


mat4 CameraComponent::getViewTransform() const {
	vec3 position = transform.getPosition();
	return lookAt(position, position + getDirection(), vec3(0, 1, 0));
}


mat4 CameraComponent::getProjectionTransform() const {
	auto projection = perspective(fov, aspectRatio, near, far);
	return projection;
}


mat4 CameraComponent::getViewProjectionTransform() const {
	return getProjectionTransform() * getViewTransform();
}
