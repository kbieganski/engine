#pragma once
#include "AssetCache.hpp"
#include "Renderer.hpp"


class ShadingRenderer : public Renderer {
public:
	ShadingRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderTarget> geometryBuffer, AssetCache<Shader> &shaderAssets);
	ShadingRenderer(ShadingRenderer&& moved);
	virtual ~ShadingRenderer();

	ShadingRenderer& operator=(ShadingRenderer&& moved);

	RenderDescription& addRender(shared_ptr<const UniformBuffer> uniformBuffer, shared_ptr<const Texture> shadowMap);


private:
	void createScreenSurface(uvec2);
	void createRenderPipeline(AssetCache<Shader> &shaderAssets);
	void createFramebuffer();

	shared_ptr<const RenderTarget> geometryBuffer;
	shared_ptr<RenderPipeline> pipeline;
};
