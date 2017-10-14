#pragma once
#include "Renderer.hpp"


class ShadingRenderer : public Renderer {
public:
	ShadingRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderTarget> geometryBuffer);
	ShadingRenderer(ShadingRenderer&& moved) = default;

	ShadingRenderer& operator=(ShadingRenderer&& moved) = default;

	RenderDescription& addRender(shared_ptr<const UniformBuffer> uniformBuffer, shared_ptr<const Texture> shadowMap);


private:
	void createScreenSurface(uvec2);
	void createShadingRenderDescription();
	void createFramebuffer();

	shared_ptr<const RenderTarget> geometryBuffer;
	shared_ptr<RenderPipeline> pipeline;
};
