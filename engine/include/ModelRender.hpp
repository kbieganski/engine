#pragma once
#include "Model.hpp"
#include "Scene.hpp"
#include "SceneRenderer.hpp"
#include "ShadowMapRenderer.hpp"
#include "Transform.hpp"


class GraphicsSystem;


class ModelRender : public DependentOn<Transform> {
public:
	struct Uniform {
		mat4 orientation;
		mat4 world;
		mat4 worldViewProjection;
	};

	ModelRender(const Transform& transform, GraphicsSystem& graphicsSystem, shared_ptr<const Model> model);

	void addTo(SceneRenderer& sceneRenderer) const;
	void addTo(ShadowMapRenderer& shadowMapRenderer) const;
	void update(const mat4& viewProjection);


private:
	const Transform& transform;
	shared_ptr<UniformBuffer> uniformBuffer;
	shared_ptr<const Model> model;
};
