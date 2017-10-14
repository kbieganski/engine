#pragma once
#include "Renderer.hpp"


class ShadowMapRenderer : public Renderer {
public:
	ShadowMapRenderer(shared_ptr<const GraphicsContext> context, const GraphicsDeviceDescription& deviceDescription, uint32_t resolution);
	ShadowMapRenderer(ShadowMapRenderer&& moved) = default;

	ShadowMapRenderer& operator=(ShadowMapRenderer&& moved) = default;

	RenderDescription& addRender();


private:
	void createShadowMap(const GraphicsDeviceDescription& deviceDescription, uint32_t resolution);
	void createFramebuffer();
	void createRenderPipeline();

	shared_ptr<RenderPipeline> pipeline;
};
