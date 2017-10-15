#pragma once
#include "Renderer.hpp"


class SceneRenderer : public Renderer {
public:
	SceneRenderer(shared_ptr<const GraphicsContext> context, uvec2 size);
	SceneRenderer(SceneRenderer&& moved) = default;

	SceneRenderer& operator=(SceneRenderer&& moved) = default;

	RenderDescription& addRender();


private:
	void createGeometryBuffer(const GraphicsDeviceDescription& deviceDescription, uvec2 size);
	void createFramebuffer();
	void createRenderPipeline();

	shared_ptr<RenderPipeline> pipeline;
};
