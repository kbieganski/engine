#pragma once
#include "Scene.hpp"
#include "ShadowMapRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "TransformComponent.hpp"


using glm::vec4;


class CameraComponent : public DependentOn<TransformComponent> {
public:
	CameraComponent(const TransformComponent& transform);

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
	float aspectRatio = 1;
	float fov;
	float near = 0.1;
	float far = 100;
	vec3 localDirection = vec3(0, 0, 1);
	const TransformComponent& transform;
};
