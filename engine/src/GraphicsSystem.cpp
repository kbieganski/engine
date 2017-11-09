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
	for (auto& sun : suns) {
		modelRenders[entity].addTo(sun.second.getShadowMapRenderer());
	}
	for (auto& spotlight : spotlights) {
		modelRenders[entity].addTo(spotlight.second.getShadowMapRenderer());
	}
}


void GraphicsSystem::addSun(EntityId entity, uint32_t resolution) {
	suns.add(entity, context, shaders, resolution, transforms[entity]);
	suns[entity].addTo(shadingRenderer);
	for (auto& modelRender : modelRenders) {
		modelRender.second.addTo(suns[entity].getShadowMapRenderer());
	}
}


void GraphicsSystem::addSpotlight(EntityId entity, uint32_t resolution) {
	spotlights.add(entity, context, shaders, resolution, transforms[entity]);
	spotlights[entity].addTo(shadingRenderer);
	for (auto& modelRender : modelRenders) {
		modelRender.second.addTo(spotlights[entity].getShadowMapRenderer());
	}
}


void GraphicsSystem::update() {
	if (currentCamera) {
		modelRenders.update(currentCamera->getViewProjectionTransform());
		suns.update(transforms[currentCameraId].getPosition());
		spotlights.update(transforms[currentCameraId].getPosition());
	}
}


void GraphicsSystem::render() {
	if (currentCamera) {
		sceneRenderer.render();
		shadingRenderer.render();
		screenRenderer.render();
	}
}


void GraphicsSystem::setAmbientLightColor(vec3 ambientColor) {
	shadingRenderer.setAmbientLightColor(ambientColor);
}


void GraphicsSystem::setSkyColor(vec3 skyColor) {
	shadingRenderer.setSkyColor(skyColor);
}


void GraphicsSystem::setCurrentCamera(EntityId entity) {
	currentCameraId = entity;
	currentCamera = &cameras[entity];
}


ModelRenderComponent& GraphicsSystem::getModelRender(EntityId entity) {
	return modelRenders[entity];
}


SunComponent& GraphicsSystem::getSun(EntityId entity) {
	return suns[entity];
}


SpotlightComponent& GraphicsSystem::getSpotlight(EntityId entity) {
	return spotlights[entity];
}


CameraComponent& GraphicsSystem::getCurrentCamera() {
	return *currentCamera;
}


vec3 GraphicsSystem::getAmbientLightColor() const {
	return shadingRenderer.getAmbientLightColor();
}


vec3 GraphicsSystem::getSkyColor() const {
	return shadingRenderer.getSkyColor();
}


const ModelRenderComponent& GraphicsSystem::getModelRender(EntityId entity) const {
	return modelRenders[entity];
}


const SunComponent& GraphicsSystem::getSun(EntityId entity) const {
	return suns[entity];
}


const CameraComponent& GraphicsSystem::getCurrentCamera() const {
	return *currentCamera;
}
