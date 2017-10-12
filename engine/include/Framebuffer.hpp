#pragma once
#include "RenderDescription.hpp"
#include "RenderTarget.hpp"


class Framebuffer {
public:
	Framebuffer(shared_ptr<const GraphicsContext> context, shared_ptr<RenderTarget> renderTarget);
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& moved);
	~Framebuffer();

	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer& operator=(Framebuffer&& moved);

	void bind(const RenderDescription& renderDescription) const;
	void bindClear() const;
	void draw(const vector<VkSemaphore>& waitSemaphores, const vector<VkSemaphore>& signalSemaphores) const;

	void setClearColor(uint32_t attachment, VkClearValue color);

	const RenderTarget& getRenderTarget() const;


private:
	void beginRenderPass() const;
	void endRenderPass() const;
	void createFramebuffer();

	VkFramebuffer handle;
	VkCommandBuffer commandBuffer;
	shared_ptr<RenderTarget> renderTarget;
	shared_ptr<const GraphicsContext> context;
	vector<VkClearValue> clearColors;
};
