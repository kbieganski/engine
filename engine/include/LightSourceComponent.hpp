#pragma once
#include "ShadowMapRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "TransformComponent.hpp"


using glm::vec4;


class LightSourceComponent {
public:
	struct ShadowMapUniform {
		mat4 viewProjection;
	};

	struct ShadingUniform {
		vec4 ambientLightColor;
		vec4 lightColor;
		vec4 lightDirection;
		mat4 lightViewProjection;
		vec4 eyePosition;
	};

	LightSourceComponent(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution, const TransformComponent& transform);

	void addTo(ShadingRenderer& shadingRenderer) const;
	void update(vec3 eyePosition, vec3 ambientLightColor);

	void setColor(vec3 color);
	void setLocalDirection(vec3 direction);

	vec3 getColor() const;
	vec3 getDirection() const;
	vec3 getLocalDirection() const;
	ShadowMapRenderer& getShadowMapRenderer();


private:
	vec3 color;
	vec3 localDirection;
	ShadowMapRenderer shadowMapRenderer;
	const TransformComponent& transform;
	shared_ptr<UniformBuffer> shadowMapUniformBuffer;
	shared_ptr<UniformBuffer> shadingUniformBuffer;
};
