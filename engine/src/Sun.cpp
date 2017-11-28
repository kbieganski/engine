#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsSystem.hpp"
#include "Sun.hpp"


using std::make_shared;
using glm::lookAt;
using glm::normalize;
using glm::ortho;


Sun::Sun(const Transform& _transform, GraphicsSystem& graphicsSystem, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution)
	:	shadowMapRenderer(graphicsSystem.getContext(), shaderAssets, shadowMapResolution),
		transform(_transform) {
	shadowMapUniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(ShadowMapUniform));
	shadingUniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(ShadingUniform));
	shadowMapRenderer.setUniformBuffer(shadowMapUniformBuffer);
	graphicsSystem.add(*this);
}


void Sun::addTo(ShadingRenderer& shadingRenderer) const {
	shadingRenderer.addSunRender(shadingUniformBuffer, shadowMapRenderer.getRenderTarget()->getTextures()[0]);
}


void Sun::update(vec3 cameraPosition) {
	auto position = transform.getPosition();
	auto direction = getDirection();
	auto view = lookAt(position, position + direction, vec3(0, 1, 0));
	auto halfAreaSize = transform.getScale() * areaSize / 2.0f;
	auto projection = ortho(-halfAreaSize.x, halfAreaSize.x, -halfAreaSize.y, halfAreaSize.y, -halfAreaSize.z, halfAreaSize.z);
	auto viewProjection = projection * view;
	ShadowMapUniform shadowMapUniform;
	shadowMapUniform.viewProjection = viewProjection;
	shadowMapUniformBuffer->set(&shadowMapUniform);
	ShadingUniform shadingUniform;
	shadingUniform.lightColor = vec4(color, 1);
	shadingUniform.lightDirection = vec4(direction, 1);
	shadingUniform.lightViewProjection = viewProjection;
	shadingUniform.cameraPosition = vec4(cameraPosition, 1);
	shadingUniformBuffer->set(&shadingUniform);
	shadowMapRenderer.render();
}


void Sun::setAreaSize(vec3 areaSize) {
	this->areaSize = areaSize;
}


void Sun::setColor(vec3 color) {
	this->color = color;
}


void Sun::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


vec3 Sun::getColor() const {
	return color;
}


vec3 Sun::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 Sun::getAreaSize() const {
	return transform.getScale() * areaSize;
}


vec3 Sun::getLocalDirection() const {
	return localDirection;
}


ShadowMapRenderer& Sun::getShadowMapRenderer() {
	return shadowMapRenderer;
}


const ShadowMapRenderer& Sun::getShadowMapRenderer() const {
	return shadowMapRenderer;
}
