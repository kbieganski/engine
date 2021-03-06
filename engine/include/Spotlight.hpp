#pragma once
#include "Scene.hpp"
#include "ShadowMapRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "Transform.hpp"


using glm::vec4;


class GraphicsSystem;


class Spotlight : public DependentOn<Transform> {
public:
	struct ShadowMapUniform {
		mat4 viewProjection;
	};

	struct ShadingUniform {
		vec4 lightColor;
		vec4 lightPosition;
		mat4 lightViewProjection;
		vec4 cameraPosition;
	};

	Spotlight(const Transform& transform, GraphicsSystem& graphicsSystem, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution);

	void addTo(ShadingRenderer& shadingRenderer) const;
	void update(vec3 cameraPosition);

	void setAngle(float size);
	void setColor(vec3 color);
	void setPower(float power);
	void setThreshold(float threshold);
	void setLocalDirection(vec3 direction);

	float getAngle() const;
	vec3 getColor() const;
	float getPower() const;
	float getMaximumDistance() const;
	float getThreshold() const;
	vec3 getDirection() const;
	vec3 getLocalDirection() const;
	ShadowMapRenderer& getShadowMapRenderer();
	const ShadowMapRenderer& getShadowMapRenderer() const;


private:
	float fov = 60;
	float power = 100;
	float threshold = 0.01;
	vec3 color = vec3(1);
	vec3 localDirection = vec3(0, -1, 0);
	ShadowMapRenderer shadowMapRenderer;
	const Transform& transform;
	shared_ptr<UniformBuffer> shadowMapUniformBuffer;
	shared_ptr<UniformBuffer> shadingUniformBuffer;
};
