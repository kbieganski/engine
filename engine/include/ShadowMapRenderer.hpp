#pragma once
#include "AssetCache.hpp"
#include "Renderer.hpp"


class ShadowMapRenderer : public Renderer {
public:
	ShadowMapRenderer(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uint32_t resolution);
	ShadowMapRenderer(ShadowMapRenderer&& moved);
	virtual ~ShadowMapRenderer();

	ShadowMapRenderer& operator=(ShadowMapRenderer&& moved);

	RenderDescription& addRender();

	void setUniformBuffer(shared_ptr<UniformBuffer> buffer);


private:
	void createShadowMap(const GraphicsDeviceDescription& deviceDescription, uint32_t resolution);
	void createFramebuffer();
	void createRenderPipeline(AssetCache<Shader> &shaderAssets);

	shared_ptr<RenderPipeline> pipeline;
	shared_ptr<UniformBuffer> viewProjectionBuffer;
};
