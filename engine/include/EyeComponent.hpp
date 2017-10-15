#pragma once
#include "ShadowMapRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "TransformComponent.hpp"


using glm::vec4;


class EyeComponent {
public:
	EyeComponent(const TransformComponent& transform);

	void setAspectRatio(float aspectRatio);
	void setFieldOfView(float fov);
	void setNearPlane(float near);
	void setFarPlane(float far);
	void setLocalDirection(vec3 direction);

	float getAspectRatio() const;
	float getFieldOfView() const;
	float getNearPlane() const;
	float getFarPlane() const;
	vec3 getDirection() const;
	vec3 getLocalDirection() const;
	mat4 getViewTransform() const;
	mat4 getProjectionTransform() const;
	mat4 getViewProjectionTransform() const;


private:
	float aspectRatio;
	float fov;
	float near;
	float far;
	vec3 localDirection;
	const TransformComponent& transform;
};
