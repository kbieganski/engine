#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtx/transform.hpp>
#include "LightSourceComponent.hpp"


using std::make_shared;
using glm::lookAt;
using glm::normalize;
using glm::ortho;


LightSourceComponent::LightSourceComponent(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution, const TransformComponent& _transform)
	:	shadowMapRenderer(context, shaderAssets, shadowMapResolution),
		transform(_transform) {
	areaSize = vec3(80);
	color = vec3(1);
	localDirection = vec3(0, 0, 1);
	shadowMapUniformBuffer = make_shared<UniformBuffer>(context, sizeof(ShadowMapUniform));
	shadingUniformBuffer = make_shared<UniformBuffer>(context, sizeof(ShadingUniform));
	shadowMapRenderer.setUniformBuffer(shadowMapUniformBuffer);
}


void LightSourceComponent::addTo(ShadingRenderer& shadingRenderer) const {
	shadingRenderer.addRender(shadingUniformBuffer, shadowMapRenderer.getRenderTarget()->getTextures()[0]);
}


void LightSourceComponent::update(vec3 cameraPosition) {
	auto position = transform.getPosition();
	auto direction = getDirection();
	auto view = lookAt(position, position + direction, vec3(0, 1, 0));
	auto halfAreaSize = transform.getScale() * areaSize / 2.0f;
	auto projection = ortho(-halfAreaSize.x, halfAreaSize.x, -halfAreaSize.y, halfAreaSize.y, halfAreaSize.z, -halfAreaSize.z);
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


void LightSourceComponent::setAreaSize(vec3 areaSize) {
	this->areaSize = areaSize;
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


vec3 LightSourceComponent::getAreaSize() const {
	return transform.getScale() * areaSize;
}


vec3 LightSourceComponent::getLocalDirection() const {
	return localDirection;
}


ShadowMapRenderer& LightSourceComponent::getShadowMapRenderer() {
	return shadowMapRenderer;
}


const ShadowMapRenderer& LightSourceComponent::getShadowMapRenderer() const {
	return shadowMapRenderer;
}
