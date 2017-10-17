#pragma once
#include "Framebuffer.hpp"
#include "RenderDescription.hpp"


using std::unique_ptr;


class Renderer {
public:
	Renderer(shared_ptr<const GraphicsContext> context);
	Renderer(const Renderer&);
	Renderer(Renderer&& moved);
	virtual ~Renderer();

	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&& moved);

	virtual void render();

	shared_ptr<const RenderTarget> getRenderTarget() const;


protected:
	RenderDescription& addRender(shared_ptr<RenderPipeline> pipeline, uint32_t uniformBufferCount, uint32_t textureCount);

	shared_ptr<RenderTarget> renderTarget;
	shared_ptr<const GraphicsContext> context;
	const Framebuffer* framebuffer;

	bool dirty = false;

private:
	void recreateCommandBuffer();
	void beginCommandBuffer();
	void endCommandBuffer();

	VkCommandBuffer commandBuffer;
	vector<RenderDescription> renderDescriptions;
};
