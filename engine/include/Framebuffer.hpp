#pragma once
#include <memory>
#include "RenderDescription.hpp"
#include "RenderTarget.hpp"


using std::vector;


class Framebuffer {
public:
	Framebuffer(shared_ptr<const GraphicsContext> context, uvec2 screenSize, shared_ptr<RenderTarget> renderTarget);
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& moved);
	~Framebuffer();

	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer& operator=(Framebuffer&& moved);

	void bind(const RenderDescription& renderDescription) const;
	void draw(const vector<VkSemaphore>& waitSemaphores, const vector<VkSemaphore>& signalSemaphores) const;

	void setClearColor(uint32_t attachment, VkClearValue color);


private:
	void createFramebuffer();

	uvec2 screenSize;
	VkFramebuffer handle;
	VkCommandBuffer commandBuffer;
	shared_ptr<RenderTarget> renderTarget;
	shared_ptr<const GraphicsContext> context;
	vector<VkClearValue> clearColors;
};
