#pragma once
#include "ShadowMapRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "TransformComponent.hpp"


using glm::vec4;


class SunComponent {
public:
	struct ShadowMapUniform {
		mat4 viewProjection;
	};

	struct ShadingUniform {
		vec4 lightColor;
		vec4 lightDirection;
		mat4 lightViewProjection;
		vec4 cameraPosition;
	};

	SunComponent(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution, const TransformComponent& transform);

	void addTo(ShadingRenderer& shadingRenderer) const;
	void update(vec3 cameraPosition);

	void setAreaSize(vec3 size);
	void setColor(vec3 color);
	void setLocalDirection(vec3 direction);

	vec3 getAreaSize() const;
	vec3 getColor() const;
	vec3 getDirection() const;
	vec3 getLocalDirection() const;
	ShadowMapRenderer& getShadowMapRenderer();
	const ShadowMapRenderer& getShadowMapRenderer() const;


private:
	vec3 areaSize = vec3(50);
	vec3 color = vec3(1);
	vec3 localDirection = vec3(0, -1, 0);
	ShadowMapRenderer shadowMapRenderer;
	const TransformComponent& transform;
	shared_ptr<UniformBuffer> shadowMapUniformBuffer;
	shared_ptr<UniformBuffer> shadingUniformBuffer;
};
