#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsSystem.hpp"
#include "SpotlightComponent.hpp"


using std::make_shared;
using std::max;
using std::sqrt;
using glm::lookAt;
using glm::normalize;
using glm::radians;
using glm::perspective;


SpotlightComponent::SpotlightComponent(const TransformComponent& _transform, GraphicsSystem& graphicsSystem, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution)
	:	shadowMapRenderer(graphicsSystem.getContext(), shaderAssets, shadowMapResolution),
		transform(_transform) {
	shadowMapUniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(ShadowMapUniform));
	shadingUniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(ShadingUniform));
	shadowMapRenderer.setUniformBuffer(shadowMapUniformBuffer);
	graphicsSystem.add(*this);
}


void SpotlightComponent::addTo(ShadingRenderer& shadingRenderer) const {
	shadingRenderer.addSpotlightRender(shadingUniformBuffer, shadowMapRenderer.getRenderTarget()->getTextures()[0]);
}


void SpotlightComponent::update(vec3 cameraPosition) {
	auto position = transform.getPosition();
	auto view = lookAt(position, position + getDirection(), vec3(0, 1, 0));
	auto projection = perspective(radians(fov), 1.0f, 0.01f, getMaximumDistance());
	auto viewProjection = projection * view;
	ShadowMapUniform shadowMapUniform;
	shadowMapUniform.viewProjection = viewProjection;
	shadowMapUniformBuffer->set(&shadowMapUniform);
	ShadingUniform shadingUniform;
	shadingUniform.lightColor = vec4(color, power);
	shadingUniform.lightPosition = vec4(position, 1);
	shadingUniform.lightViewProjection = viewProjection;
	shadingUniform.cameraPosition = vec4(cameraPosition, 1);
	shadingUniformBuffer->set(&shadingUniform);
	shadowMapRenderer.render();
}


void SpotlightComponent::setAngle(float angle) {
	fov = angle;
}


void SpotlightComponent::setColor(vec3 color) {
	this->color = color;
}


void SpotlightComponent::setPower(float power) {
	this->power = power;
}


void SpotlightComponent::setThreshold(float threshold) {
	this->threshold = threshold;
}


void SpotlightComponent::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


float SpotlightComponent::getAngle() const {
	return fov;
}


vec3 SpotlightComponent::getColor() const {
	return color;
}


float SpotlightComponent::getPower() const {
	return power;
}


float SpotlightComponent::getMaximumDistance() const {
	return sqrt(max(color.r, max(color.g, color.b)) * power / threshold) - 1;
}


float SpotlightComponent::getThreshold() const {
	return threshold;
}


vec3 SpotlightComponent::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 SpotlightComponent::getLocalDirection() const {
	return localDirection;
}


ShadowMapRenderer& SpotlightComponent::getShadowMapRenderer() {
	return shadowMapRenderer;
}


const ShadowMapRenderer& SpotlightComponent::getShadowMapRenderer() const {
	return shadowMapRenderer;
}
