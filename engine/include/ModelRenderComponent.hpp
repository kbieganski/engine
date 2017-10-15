#pragma once
#include "Model.hpp"
#include "SceneRenderer.hpp"
#include "ShadowMapRenderer.hpp"
#include "TransformComponent.hpp"


class ModelRenderComponent {
public:
	struct Uniform {
		mat4 orientation;
		mat4 world;
		mat4 worldViewProjection;
	};

	ModelRenderComponent(shared_ptr<const GraphicsContext> context, const TransformComponent& transform, shared_ptr<const Model> model);

	void addTo(SceneRenderer& sceneRenderer) const;
	void addTo(ShadowMapRenderer& shadowMapRenderer) const;
	void update(const mat4& viewProjection);


private:
	const TransformComponent& transform;
	shared_ptr<UniformBuffer> uniformBuffer;
	shared_ptr<const Model> model;
};
