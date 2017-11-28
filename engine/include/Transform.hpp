#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/quaternion.hpp>


using glm::mat4;
using glm::quat;
using glm::vec3;


class Transform {
public:
	void setParent(const Transform& parent);
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
	quat orientation = quat(1, 0, 0, 0);
	vec3 position = vec3(0);
	vec3 scale = vec3(1);
	const Transform* parent = nullptr;
};
