#pragma once
#include "ComponentSystem.hpp"
#include "EyeComponent.hpp"
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
	void addEye(EntityId entity);

	void update();
	void render();

	void setCurrentEye(EntityId entity);

	ModelRenderComponent& getModelRender(EntityId entity);
	LightSourceComponent& getLightSource(EntityId entity);
	EyeComponent& getEye(EntityId entity);
	EyeComponent& getCurrentEye();
	const ModelRenderComponent& getModelRender(EntityId entity) const;
	const LightSourceComponent& getLightSource(EntityId entity) const;
	const EyeComponent& getEye(EntityId entity) const;
	const EyeComponent& getCurrentEye() const;


private:
	shared_ptr<const GraphicsContext> context;
	AssetCache<Shader> &shaders;
	SceneRenderer sceneRenderer;
	ShadingRenderer shadingRenderer;
	ScreenRenderer screenRenderer;
	ComponentSystem<TransformComponent>& transforms;
	ComponentSystem<ModelRenderComponent> modelRenders;
	ComponentSystem<LightSourceComponent> lightSources;
	ComponentSystem<EyeComponent> eyes;
	EyeComponent* currentEye;
	EntityId currentEyeId;
};
