#include <glm/gtc/matrix_transform.hpp>
#include "Transform.hpp"


using glm::translate;
using glm::mat4_cast;


void Transform::setParent(const Transform& parent) {
	this->parent = &parent;
}


void Transform::setLocalOrientation(quat orientation) {
	this->orientation = orientation;
}


void Transform::setLocalPosition(vec3 position) {
	this->position = position;
}


void Transform::setLocalScale(vec3 scale) {
	this->scale = scale;
}


quat Transform::getOrientation() const {
	if (parent) {
		return parent->getOrientation() * getLocalOrientation();
	} else {
		return getLocalOrientation();
	}
}


vec3 Transform::getPosition() const {
	if (parent) {
		return parent->getPosition() + getLocalPosition();
	} else {
		return getLocalPosition();
	}
}


vec3 Transform::getScale() const {
	if (parent) {
		return parent->getScale() * getLocalScale();
	} else {
		return getLocalScale();
	}
}


mat4 Transform::getTransform() const {
	if (parent) {
		return parent->getTransform() * getLocalTransform();
	} else {
		return getLocalTransform();
	}
}


quat Transform::getLocalOrientation() const {
	return orientation;
}


vec3 Transform::getLocalPosition() const {
	return position;
}


vec3 Transform::getLocalScale() const {
	return scale;
}


mat4 Transform::getLocalTransform() const {
	auto scaleMatrix = glm::scale(mat4(1), scale);
	auto orientationMatrix = mat4_cast(orientation);
	auto translationMatrix = translate(mat4(1), position);
	return translationMatrix * orientationMatrix * scaleMatrix;
}
