#include "GraphicsSystem.hpp"
#include "Logger.hpp"


GraphicsSystem::GraphicsSystem(Scene& _scene, shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, AssetCache<Shader> &shaderAssets)
	:	shaders(shaderAssets),
		sceneRenderer(context, shaderAssets, swapChain->getScreenSize()),
		shadingRenderer(context, sceneRenderer.getRenderTarget(), shaderAssets),
		screenRenderer(context, swapChain, shadingRenderer.getRenderTarget(), shaderAssets),
		scene(_scene) {
	ambientColor = vec3(0.1);
	this->context = context;
}



void GraphicsSystem::add(ModelRenderComponent& modelRender) {
	modelRender.addTo(sceneRenderer);
	INFO("Sun count: ", scene.get<SunComponent>().size());
	for (auto& sun : scene.get<SunComponent>()) {
		modelRender.addTo(sun.second.getShadowMapRenderer());
	}
	INFO("Spotlight count: ", scene.get<SunComponent>().size());
	for (auto& spotlight : scene.get<SpotlightComponent>()) {
		modelRender.addTo(spotlight.second.getShadowMapRenderer());
	}
}


void GraphicsSystem::add(SunComponent& sun) {
	sun.addTo(shadingRenderer);
	INFO("ModelRender count: ", scene.get<ModelRenderComponent>().size());
	for (auto& modelRender : scene.get<ModelRenderComponent>()) {
		modelRender.second.addTo(sun.getShadowMapRenderer());
	}
}


void GraphicsSystem::add(SpotlightComponent& spotlight) {
	spotlight.addTo(shadingRenderer);
	INFO("ModelRender count: ", scene.get<ModelRenderComponent>().size());
	for (auto& modelRender : scene.get<ModelRenderComponent>()) {
		modelRender.second.addTo(spotlight.getShadowMapRenderer());
	}
}


void GraphicsSystem::update() {
	if (currentCamera) {
		scene.update<ModelRenderComponent>(currentCamera->getViewProjectionTransform());
		scene.update<SunComponent>(scene.get<TransformComponent>(currentCameraId).getPosition());
		scene.update<SpotlightComponent>(scene.get<TransformComponent>(currentCameraId).getPosition());
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
	currentCamera = &scene.get<CameraComponent>(entity);
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


const CameraComponent& GraphicsSystem::getCurrentCamera() const {
	return *currentCamera;
}


shared_ptr<const GraphicsContext> GraphicsSystem::getContext() const {
	return context;
}
