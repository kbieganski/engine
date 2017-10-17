#pragma once
#include "RenderTarget.hpp"


class Framebuffer {
public:
	Framebuffer(shared_ptr<const GraphicsContext> context, shared_ptr<RenderTarget> renderTarget);
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& moved);
	~Framebuffer();

	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer& operator=(Framebuffer&& moved);

	void beginRenderPass(VkCommandBuffer commandBuffer) const;
	void draw(VkCommandBuffer commandBuffer) const;

	void addWaitSemaphore(VkSemaphore semaphore);
	void addSignalSemaphore(VkSemaphore semaphore);

	void setClearColor(uint32_t attachment, VkClearValue color);

	const RenderTarget& getRenderTarget() const;


private:
	void createFramebuffer();

	VkFramebuffer handle;
	vector<VkSemaphore> waitSemaphores;
	vector<VkSemaphore> signalSemaphores;
	shared_ptr<RenderTarget> renderTarget;
	shared_ptr<const GraphicsContext> context;
	vector<VkClearValue> clearColors;
};
