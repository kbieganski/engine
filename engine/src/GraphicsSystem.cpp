#include "GraphicsSystem.hpp"


GraphicsSystem::GraphicsSystem(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, AssetCache<Shader> &shaderAssets, ComponentSystem<TransformComponent>& _transforms, ComponentSystem<CameraComponent>& _cameras)
	:	shaders(shaderAssets),
		sceneRenderer(context, shaderAssets, swapChain->getScreenSize()),
		shadingRenderer(context, sceneRenderer.getRenderTarget(), shaderAssets),
		screenRenderer(context, swapChain, shadingRenderer.getRenderTarget(), shaderAssets),
		transforms(_transforms),
		cameras(_cameras) {
	ambientColor = vec3(0.1);
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


void GraphicsSystem::update() {
	if (currentCamera) {
		modelRenders.update(currentCamera->getViewProjectionTransform());
		lightSources.update(transforms[currentCameraId].getPosition(), ambientColor);
	}
}


void GraphicsSystem::render() {
	if (currentCamera) {
		sceneRenderer.render();
		shadingRenderer.render();
		screenRenderer.render();
	}
}


void GraphicsSystem::setAmbientColor(vec3 ambientColor) {
	this->ambientColor = ambientColor;
}


void GraphicsSystem::setCurrentCamera(EntityId entity) {
	currentCameraId = entity;
	currentCamera = &cameras[entity];
}


ModelRenderComponent& GraphicsSystem::getModelRender(EntityId entity) {
	return modelRenders[entity];
}


LightSourceComponent& GraphicsSystem::getLightSource(EntityId entity) {
	return lightSources[entity];
}


CameraComponent& GraphicsSystem::getCurrentCamera() {
	return *currentCamera;
}


vec3 GraphicsSystem::getAmbientColor() const {
	return ambientColor;
}


const ModelRenderComponent& GraphicsSystem::getModelRender(EntityId entity) const {
	return modelRenders[entity];
}


const LightSourceComponent& GraphicsSystem::getLightSource(EntityId entity) const {
	return lightSources[entity];
}


const CameraComponent& GraphicsSystem::getCurrentCamera() const {
	return *currentCamera;
}
