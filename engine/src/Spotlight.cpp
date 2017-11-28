#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsSystem.hpp"
#include "Spotlight.hpp"


using std::make_shared;
using std::max;
using std::sqrt;
using glm::lookAt;
using glm::normalize;
using glm::radians;
using glm::perspective;


Spotlight::Spotlight(const Transform& _transform, GraphicsSystem& graphicsSystem, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution)
	:	shadowMapRenderer(graphicsSystem.getContext(), shaderAssets, shadowMapResolution),
		transform(_transform) {
	shadowMapUniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(ShadowMapUniform));
	shadingUniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(ShadingUniform));
	shadowMapRenderer.setUniformBuffer(shadowMapUniformBuffer);
	graphicsSystem.add(*this);
}


void Spotlight::addTo(ShadingRenderer& shadingRenderer) const {
	shadingRenderer.addSpotlightRender(shadingUniformBuffer, shadowMapRenderer.getRenderTarget()->getTextures()[0]);
}


void Spotlight::update(vec3 cameraPosition) {
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


void Spotlight::setAngle(float angle) {
	fov = angle;
}


void Spotlight::setColor(vec3 color) {
	this->color = color;
}


void Spotlight::setPower(float power) {
	this->power = power;
}


void Spotlight::setThreshold(float threshold) {
	this->threshold = threshold;
}


void Spotlight::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


float Spotlight::getAngle() const {
	return fov;
}


vec3 Spotlight::getColor() const {
	return color;
}


float Spotlight::getPower() const {
	return power;
}


float Spotlight::getMaximumDistance() const {
	return sqrt(max(color.r, max(color.g, color.b)) * power / threshold) - 1;
}


float Spotlight::getThreshold() const {
	return threshold;
}


vec3 Spotlight::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 Spotlight::getLocalDirection() const {
	return localDirection;
}


ShadowMapRenderer& Spotlight::getShadowMapRenderer() {
	return shadowMapRenderer;
}


const ShadowMapRenderer& Spotlight::getShadowMapRenderer() const {
	return shadowMapRenderer;
}
