#pragma once
#include "CameraComponent.hpp"
#include "SunComponent.hpp"
#include "ModelRenderComponent.hpp"
#include "SceneRenderer.hpp"
#include "ScreenRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "SpotlightComponent.hpp"
#include "SwapChain.hpp"


class GraphicsSystem {
public:
	GraphicsSystem(Scene& scene, shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, AssetCache<Shader> &shaders);

	void add(ModelRenderComponent& modelRender);
	void add(SunComponent& sun);
	void add(SpotlightComponent& spotlight);

	void update();
	void render();

	void setAmbientLightColor(vec3 ambientColor);
	void setSkyColor(vec3 skyColor);
	void setCurrentCamera(EntityId entity);

	CameraComponent& getCurrentCamera();
	vec3 getAmbientLightColor() const;
	vec3 getSkyColor() const;
	const ModelRenderComponent& getModelRender(EntityId entity) const;
	const SunComponent& getSun(EntityId entity) const;
	const CameraComponent& getCurrentCamera() const;
	shared_ptr<const GraphicsContext> getContext() const;


private:
	vec3 ambientColor;
	shared_ptr<const GraphicsContext> context;
	AssetCache<Shader> &shaders;
	SceneRenderer sceneRenderer;
	ShadingRenderer shadingRenderer;
	ScreenRenderer screenRenderer;
	Scene& scene;
	CameraComponent* currentCamera = nullptr;
	EntityId currentCameraId;
};
