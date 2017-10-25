#pragma once
#include "ComponentSystem.hpp"
#include "CameraComponent.hpp"
#include "LightSourceComponent.hpp"
#include "ModelRenderComponent.hpp"
#include "SceneRenderer.hpp"
#include "ScreenRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "SwapChain.hpp"
#include "TransformComponent.hpp"


class GraphicsSystem {
public:
	GraphicsSystem(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, AssetCache<Shader> &shaders, ComponentSystem<TransformComponent>& transforms);

	void addModelRender(EntityId entity, shared_ptr<const Model> model);
	void addLightSource(EntityId entity, uint32_t resolution);
	void addCamera(EntityId entity);

	void update();
	void render();

	void setAmbientColor(vec3 ambientColor);
	void setCurrentCamera(EntityId entity);

	ModelRenderComponent& getModelRender(EntityId entity);
	LightSourceComponent& getLightSource(EntityId entity);
	CameraComponent& getCamera(EntityId entity);
	CameraComponent& getCurrentCamera();
	vec3 getAmbientColor() const;
	const ModelRenderComponent& getModelRender(EntityId entity) const;
	const LightSourceComponent& getLightSource(EntityId entity) const;
	const CameraComponent& getCamera(EntityId entity) const;
	const CameraComponent& getCurrentCamera() const;


private:
	vec3 ambientColor;
	shared_ptr<const GraphicsContext> context;
	AssetCache<Shader> &shaders;
	SceneRenderer sceneRenderer;
	ShadingRenderer shadingRenderer;
	ScreenRenderer screenRenderer;
	ComponentSystem<TransformComponent>& transforms;
	ComponentSystem<ModelRenderComponent> modelRenders;
	ComponentSystem<LightSourceComponent> lightSources;
	ComponentSystem<CameraComponent> cameras;
	CameraComponent* currentCamera;
	EntityId currentCameraId;
};
