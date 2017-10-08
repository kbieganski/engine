#pragma once
#include "GraphicsContext.hpp"


class RenderPass {
public:
	RenderPass(shared_ptr<const GraphicsContext> context, VkRenderPass handle, uint32_t colorAttachmentCount);
	RenderPass(const RenderPass&) = delete;
	RenderPass(RenderPass&& moved);
	~RenderPass();

	RenderPass& operator=(const RenderPass&) = delete;
	RenderPass& operator=(RenderPass&& moved);

	VkRenderPass getHandle() const;
	uint32_t getColorAttachmentCount() const;


private:
	uint32_t colorAttachmentCount;
	VkRenderPass handle;
	shared_ptr<const GraphicsContext> context;
};
