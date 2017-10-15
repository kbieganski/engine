#pragma once
#include <glm/gtc/quaternion.hpp>


using glm::mat4;
using glm::quat;
using glm::vec3;


class TransformComponent {
public:
	TransformComponent();

	void setParent(const TransformComponent& parent);
	void setLocalOrientation(quat orientation);
	void setLocalPosition(vec3 position);
	void setLocalScale(vec3 scale);

	quat getOrientation() const;
	vec3 getPosition() const;
	vec3 getScale() const;
	mat4 getTransform() const;

	quat getLocalOrientation() const;
	vec3 getLocalPosition() const;
	vec3 getLocalScale() const;
	mat4 getLocalTransform() const;


private:
	quat orientation;
	vec3 position;
	vec3 scale;
	const TransformComponent* parent;
};
