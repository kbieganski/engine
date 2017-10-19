#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "TransformComponent.hpp"


using glm::translate;
using glm::toMat4;


TransformComponent::TransformComponent() {
	scale = vec3(1);
	parent = nullptr;
}


void TransformComponent::setParent(const TransformComponent& parent) {
	this->parent = &parent;
}


void TransformComponent::setLocalOrientation(quat orientation) {
	this->orientation = orientation;
}


void TransformComponent::setLocalPosition(vec3 position) {
	this->position = position;
}


void TransformComponent::setLocalScale(vec3 scale) {
	this->scale = scale;
}


quat TransformComponent::getOrientation() const {
	if (parent) {
		return parent->getOrientation() * getLocalOrientation();
	} else {
		return getLocalOrientation();
	}
}


vec3 TransformComponent::getPosition() const {
	if (parent) {
		return parent->getPosition() + getLocalPosition();
	} else {
		return getLocalPosition();
	}
}


vec3 TransformComponent::getScale() const {
	if (parent) {
		return parent->getScale() * getLocalScale();
	} else {
		return getLocalScale();
	}
}


mat4 TransformComponent::getTransform() const {
	if (parent) {
		return parent->getTransform() * getLocalTransform();
	} else {
		return getLocalTransform();
	}
}


quat TransformComponent::getLocalOrientation() const {
	return orientation;
}


vec3 TransformComponent::getLocalPosition() const {
	return position;
}


vec3 TransformComponent::getLocalScale() const {
	return scale;
}


mat4 TransformComponent::getLocalTransform() const {
	auto scaleMatrix = glm::scale(mat4(1), scale);
	auto orientationMatrix = toMat4(orientation);
	auto translationMatrix = translate(mat4(1), position);
	return translationMatrix * orientationMatrix * scaleMatrix;
}
