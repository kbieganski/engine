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



void GraphicsSystem::add(ModelRender& modelRender) {
	modelRender.addTo(sceneRenderer);
	INFO("Sun count: ", scene.get<Sun>().size());
	for (auto& sun : scene.get<Sun>()) {
		modelRender.addTo(sun.second.getShadowMapRenderer());
	}
	INFO("Spotlight count: ", scene.get<Sun>().size());
	for (auto& spotlight : scene.get<Spotlight>()) {
		modelRender.addTo(spotlight.second.getShadowMapRenderer());
	}
}


void GraphicsSystem::add(Sun& sun) {
	sun.addTo(shadingRenderer);
	INFO("ModelRender count: ", scene.get<ModelRender>().size());
	for (auto& modelRender : scene.get<ModelRender>()) {
		modelRender.second.addTo(sun.getShadowMapRenderer());
	}
}


void GraphicsSystem::add(Spotlight& spotlight) {
	spotlight.addTo(shadingRenderer);
	INFO("ModelRender count: ", scene.get<ModelRender>().size());
	for (auto& modelRender : scene.get<ModelRender>()) {
		modelRender.second.addTo(spotlight.getShadowMapRenderer());
	}
}


void GraphicsSystem::update() {
	if (currentCamera) {
		scene.update<ModelRender>(currentCamera->getViewProjectionTransform());
		scene.update<Sun>(scene.get<Transform>(currentCameraId).getPosition());
		scene.update<Spotlight>(scene.get<Transform>(currentCameraId).getPosition());
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
	currentCamera = &scene.get<Camera>(entity);
}


Camera& GraphicsSystem::getCurrentCamera() {
	return *currentCamera;
}


vec3 GraphicsSystem::getAmbientLightColor() const {
	return shadingRenderer.getAmbientLightColor();
}


vec3 GraphicsSystem::getSkyColor() const {
	return shadingRenderer.getSkyColor();
}


const Camera& GraphicsSystem::getCurrentCamera() const {
	return *currentCamera;
}


shared_ptr<const GraphicsContext> GraphicsSystem::getContext() const {
	return context;
}
