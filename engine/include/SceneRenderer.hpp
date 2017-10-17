#pragma once
#include "AssetCache.hpp"
#include "Renderer.hpp"


class SceneRenderer : public Renderer {
public:
	SceneRenderer(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uvec2 size);
	SceneRenderer(SceneRenderer&& moved);
	virtual ~SceneRenderer();

	SceneRenderer& operator=(SceneRenderer&& moved);

	RenderDescription& addRender();


private:
	void createGeometryBuffer(const GraphicsDeviceDescription& deviceDescription, uvec2 size);
	void createFramebuffer();
	void createRenderPipeline(AssetCache<Shader> &shaderAssets);

	shared_ptr<RenderPipeline> pipeline;
};
