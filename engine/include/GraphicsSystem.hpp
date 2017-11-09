#pragma once
#include "ComponentSystem.hpp"
#include "CameraComponent.hpp"
#include "SunComponent.hpp"
#include "ModelRenderComponent.hpp"
#include "SceneRenderer.hpp"
#include "ScreenRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "SpotlightComponent.hpp"
#include "SwapChain.hpp"
#include "TransformComponent.hpp"


class GraphicsSystem {
public:
	GraphicsSystem(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, AssetCache<Shader> &shaders, ComponentSystem<TransformComponent>& transforms, ComponentSystem<CameraComponent>& cameras);

	void addModelRender(EntityId entity, shared_ptr<const Model> model);
	void addSun(EntityId entity, uint32_t resolution);
	void addSpotlight(EntityId entity, uint32_t resolution);

	void update();
	void render();

	void setAmbientLightColor(vec3 ambientColor);
	void setSkyColor(vec3 skyColor);
	void setCurrentCamera(EntityId entity);

	ModelRenderComponent& getModelRender(EntityId entity);
	SunComponent& getSun(EntityId entity);
	SpotlightComponent& getSpotlight(EntityId entity);
	CameraComponent& getCurrentCamera();
	vec3 getAmbientLightColor() const;
	vec3 getSkyColor() const;
	const ModelRenderComponent& getModelRender(EntityId entity) const;
	const SunComponent& getSun(EntityId entity) const;
	const CameraComponent& getCurrentCamera() const;


private:
	vec3 ambientColor;
	shared_ptr<const GraphicsContext> context;
	AssetCache<Shader> &shaders;
	SceneRenderer sceneRenderer;
	ShadingRenderer shadingRenderer;
	ScreenRenderer screenRenderer;
	ComponentSystem<TransformComponent>& transforms;
	ComponentSystem<CameraComponent>& cameras;
	ComponentSystem<ModelRenderComponent> modelRenders;
	ComponentSystem<SunComponent> suns;
	ComponentSystem<SpotlightComponent> spotlights;
	CameraComponent* currentCamera = nullptr;
	EntityId currentCameraId;
};
