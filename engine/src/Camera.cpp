#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"


using std::make_shared;
using glm::lookAt;
using glm::normalize;
using glm::perspective;
using glm::radians;


Camera::Camera(const Transform& _transform)
	:	transform(_transform) {
	aspectRatio = 1;
	fov = radians(90.0f);
}


void Camera::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
}


void Camera::setFieldOfView(float fov) {
	this->fov = fov;
}


void Camera::setNearPlane(float near) {
	this->near = near;
}


void Camera::setFarPlane(float far) {
	this->far = far;
}


void Camera::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


float Camera::getAspectRatio() const {
	return aspectRatio;
}


float Camera::getFieldOfView() const {
	return fov;
}


float Camera::getNearPlane() const {
	return near;
}


float Camera::getFarPlane() const {
	return far;
}


vec3 Camera::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 Camera::getLocalDirection() const {
	return localDirection;
}


mat4 Camera::getViewTransform() const {
	vec3 position = transform.getPosition();
	return lookAt(position, position + getDirection(), vec3(0, 1, 0));
}


mat4 Camera::getProjectionTransform() const {
	auto projection = perspective(fov, aspectRatio, near, far);
	return projection;
}


mat4 Camera::getViewProjectionTransform() const {
	return getProjectionTransform() * getViewTransform();
}
