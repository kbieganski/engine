#pragma once
#include "Camera.hpp"
#include "Sun.hpp"
#include "ModelRender.hpp"
#include "SceneRenderer.hpp"
#include "ScreenRenderer.hpp"
#include "ShadingRenderer.hpp"
#include "Spotlight.hpp"
#include "SwapChain.hpp"


class GraphicsSystem {
public:
	GraphicsSystem(Scene& scene, shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, AssetCache<Shader> &shaders);

	void add(ModelRender& modelRender);
	void add(Sun& sun);
	void add(Spotlight& spotlight);

	void update();
	void render();

	void setAmbientLightColor(vec3 ambientColor);
	void setSkyColor(vec3 skyColor);
	void setCurrentCamera(EntityId entity);

	Camera& getCurrentCamera();
	vec3 getAmbientLightColor() const;
	vec3 getSkyColor() const;
	const ModelRender& getModelRender(EntityId entity) const;
	const Sun& getSun(EntityId entity) const;
	const Camera& getCurrentCamera() const;
	shared_ptr<const GraphicsContext> getContext() const;


private:
	vec3 ambientColor;
	shared_ptr<const GraphicsContext> context;
	AssetCache<Shader> &shaders;
	SceneRenderer sceneRenderer;
	ShadingRenderer shadingRenderer;
	ScreenRenderer screenRenderer;
	Scene& scene;
	Camera* currentCamera = nullptr;
	EntityId currentCameraId;
};
