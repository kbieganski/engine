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
		return getLocalOrientation() * parent->getOrientation();
	} else {
		return getLocalOrientation();
	}
}


vec3 TransformComponent::getPosition() const {
	if (parent) {
		return getLocalPosition() + parent->getPosition();
	} else {
		return getLocalPosition();
	}
}


vec3 TransformComponent::getScale() const {
	if (parent) {
		return getLocalScale() * parent->getScale();
	} else {
		return getLocalScale();
	}
}


mat4 TransformComponent::getTransform() const {
	if (parent) {
		return getLocalTransform() * parent->getTransform();
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
	return translate(toMat4(orientation) * glm::scale(mat4(1), scale), position);
}
