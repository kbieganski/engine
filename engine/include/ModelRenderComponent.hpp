#pragma once
#include "Model.hpp"
#include "Scene.hpp"
#include "SceneRenderer.hpp"
#include "ShadowMapRenderer.hpp"
#include "TransformComponent.hpp"


class GraphicsSystem;


class ModelRenderComponent : public DependentOn<TransformComponent> {
public:
	struct Uniform {
		mat4 orientation;
		mat4 world;
		mat4 worldViewProjection;
	};

	ModelRenderComponent(const TransformComponent& transform, GraphicsSystem& graphicsSystem, shared_ptr<const Model> model);

	void addTo(SceneRenderer& sceneRenderer) const;
	void addTo(ShadowMapRenderer& shadowMapRenderer) const;
	void update(const mat4& viewProjection);


private:
	const TransformComponent& transform;
	shared_ptr<UniformBuffer> uniformBuffer;
	shared_ptr<const Model> model;
};
