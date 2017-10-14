#pragma once
#include "Framebuffer.hpp"


using std::unique_ptr;


class Renderer {
public:
	Renderer(shared_ptr<const GraphicsContext> context);
	Renderer(Renderer&& moved) = default;

	Renderer& operator=(Renderer&& moved) = default;

	void render() const;

	shared_ptr<const RenderTarget> getRenderTarget() const;


protected:
	RenderDescription& addRender(shared_ptr<RenderPipeline> pipeline, uint32_t uniformBufferCount, uint32_t textureCount);

	shared_ptr<RenderTarget> renderTarget;
	shared_ptr<const GraphicsContext> context;
	unique_ptr<Framebuffer> framebuffer;

private:
	vector<RenderDescription> renderDescriptions;
};
