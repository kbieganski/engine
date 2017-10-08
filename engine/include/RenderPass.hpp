#pragma once
#include "GraphicsContext.hpp"


class RenderPass {
public:
	RenderPass(shared_ptr<const GraphicsContext> context, VkRenderPass handle);
	RenderPass(const RenderPass&) = delete;
	RenderPass(RenderPass&& moved);
	~RenderPass();

	RenderPass& operator=(const RenderPass&) = delete;
	RenderPass& operator=(RenderPass&& moved);

	VkRenderPass getHandle() const;


private:
	VkRenderPass handle;
	shared_ptr<const GraphicsContext> context;
};
