#include "GraphicsSystem.hpp"


GraphicsSystem::GraphicsSystem(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, AssetCache<Shader> &shaderAssets, ComponentSystem<TransformComponent>& _transforms)
	:	shaders(shaderAssets),
		sceneRenderer(context, shaderAssets, swapChain->getScreenSize()),
		shadingRenderer(context, sceneRenderer.getRenderTarget(), shaderAssets),
		screenRenderer(context, swapChain, shadingRenderer.getRenderTarget(), shaderAssets),
		transforms(_transforms) {
	this->context = context;
}



void GraphicsSystem::addModelRender(EntityId entity, shared_ptr<const Model> model) {
	modelRenders.add(entity, context, transforms[entity], model);
	modelRenders[entity].addTo(sceneRenderer);
	for (auto& lightSource : lightSources) {
		modelRenders[entity].addTo(lightSource.second.getShadowMapRenderer());
	}
}


void GraphicsSystem::addLightSource(EntityId entity, uint32_t resolution) {
	lightSources.add(entity, context, shaders, resolution, transforms[entity]);
	lightSources[entity].addTo(shadingRenderer);
}


void GraphicsSystem::addEye(EntityId entity) {
	eyes.add(entity, transforms[entity]);
}


void GraphicsSystem::update() {
	if (currentEye) {
		modelRenders.update(currentEye->getViewProjectionTransform());
		lightSources.update(transforms[currentEyeId].getPosition(), vec4(0.075, 0.12, 0.15, 1.0));
	}
}


void GraphicsSystem::render() {
	if (currentEye) {
		sceneRenderer.render();
		shadingRenderer.render();
		screenRenderer.render();
	}
}


void GraphicsSystem::setCurrentEye(EntityId entity) {
	currentEyeId = entity;
	currentEye = &eyes[entity];
}


ModelRenderComponent& GraphicsSystem::getModelRender(EntityId entity) {
	return modelRenders[entity];
}


LightSourceComponent& GraphicsSystem::getLightSource(EntityId entity) {
	return lightSources[entity];
}


EyeComponent& GraphicsSystem::getEye(EntityId entity) {
	return eyes[entity];
}


EyeComponent& GraphicsSystem::getCurrentEye() {

	return *currentEye;
}


const ModelRenderComponent& GraphicsSystem::getModelRender(EntityId entity) const {
	return modelRenders[entity];
}


const LightSourceComponent& GraphicsSystem::getLightSource(EntityId entity) const {
	return lightSources[entity];
}


const EyeComponent& GraphicsSystem::getEye(EntityId entity) const {
	return eyes[entity];
}


const EyeComponent& GraphicsSystem::getCurrentEye() const {
	return *currentEye;
}
