#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtx/transform.hpp>
#include "SunComponent.hpp"


using std::make_shared;
using glm::lookAt;
using glm::normalize;
using glm::ortho;


SunComponent::SunComponent(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uint32_t shadowMapResolution, const TransformComponent& _transform)
	:	shadowMapRenderer(context, shaderAssets, shadowMapResolution),
		transform(_transform) {
	shadowMapUniformBuffer = make_shared<UniformBuffer>(context, sizeof(ShadowMapUniform));
	shadingUniformBuffer = make_shared<UniformBuffer>(context, sizeof(ShadingUniform));
	shadowMapRenderer.setUniformBuffer(shadowMapUniformBuffer);
}


void SunComponent::addTo(ShadingRenderer& shadingRenderer) const {
	shadingRenderer.addSunRender(shadingUniformBuffer, shadowMapRenderer.getRenderTarget()->getTextures()[0]);
}


void SunComponent::update(vec3 cameraPosition) {
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


void SunComponent::setAreaSize(vec3 areaSize) {
	this->areaSize = areaSize;
}


void SunComponent::setColor(vec3 color) {
	this->color = color;
}


void SunComponent::setLocalDirection(vec3 direction) {
	localDirection = normalize(direction);
}


vec3 SunComponent::getColor() const {
	return color;
}


vec3 SunComponent::getDirection() const {
	return transform.getOrientation() * localDirection;
}


vec3 SunComponent::getAreaSize() const {
	return transform.getScale() * areaSize;
}


vec3 SunComponent::getLocalDirection() const {
	return localDirection;
}


ShadowMapRenderer& SunComponent::getShadowMapRenderer() {
	return shadowMapRenderer;
}


const ShadowMapRenderer& SunComponent::getShadowMapRenderer() const {
	return shadowMapRenderer;
}
