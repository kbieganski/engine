#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtx/transform.hpp>
#include "LightSourceComponent.hpp"


using std::make_shared;
using glm::lookAt;
using glm::normalize;
using glm::ortho;


LightSourceComponent::LightSourceComponent(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution, const TransformComponent& _transform)
	:	shadowMapRenderer(context, shaderAssets, shadowMapResolution),
		transform(_transform) {
	color = vec3(1);
	localDirection = vec3(0, 0, 1);
	shadowMapUniformBuffer = make_shared<UniformBuffer>(context, sizeof(ShadowMapUniform));
	shadingUniformBuffer = make_shared<UniformBuffer>(context, sizeof(ShadingUniform));
	shadowMapRenderer.setUniformBuffer(shadowMapUniformBuffer);
}


void LightSourceComponent::addTo(ShadingRenderer& shadingRenderer) const {
	shadingRenderer.addRender(shadingUniformBuffer, shadowMapRenderer.getRenderTarget()->getTextures()[0]);
}


void LightSourceComponent::update(vec3 eyePosition, vec3 ambientLightColor) {
	auto position = transform.getPosition();
	auto direction = getDirection();
	auto view = lookAt(position, position + direction, vec3(0, -1, 0));
	auto projection = ortho(-20.f, 20.0f, -20.0f, 20.0f, -100.0f, 100.f);
	auto viewProjection = projection * view;
	ShadowMapUniform shadowMapUniform;
	shadowMapUniform.viewProjection = viewProjection;
	shadowMapUniformBuffer->set(&shadowMapUniform);
	ShadingUniform shadingUniform;
	shadingUniform.ambientLightColor = vec4(ambientLightColor, 1);
	shadingUniform.lightColor = vec4(color, 1);
	shadingUniform.lightDirection = vec4(direction, 1);
	shadingUniform.lightViewProjection = viewProjection;
	shadingUniform.eyePosition = vec4(eyePosition, 1);
	shadingUniformBuffer->set(&shadingUniform);
	shadowMapRenderer.render();
}


void LightSourceComponent::setColor(vec3 color) {
	this->color = color;
}


void LightSourceComponent::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


vec3 LightSourceComponent::getColor() const {
	return color;
}


vec3 LightSourceComponent::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 LightSourceComponent::getLocalDirection() const {
	return localDirection;
}


ShadowMapRenderer& LightSourceComponent::getShadowMapRenderer() {
	return shadowMapRenderer;
}
