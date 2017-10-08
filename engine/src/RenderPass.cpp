#include "Logger.hpp"
#include "RenderPass.hpp"


using std::make_shared;
using std::move;


RenderPass::RenderPass(shared_ptr<const GraphicsContext> context, VkRenderPass handle, uint32_t colorAttachmentCount) {
	this->colorAttachmentCount = colorAttachmentCount;
	this->handle = handle;
	this->context = context;
}


RenderPass::RenderPass(RenderPass&& moved) {
	*this = move(moved);
}


RenderPass::~RenderPass() {
	if (handle != VK_NULL_HANDLE) {
		vkDestroyRenderPass(context->getDevice(), handle, nullptr);
		INFO("Destroyed render pass ", handle);
	}
}


RenderPass& RenderPass::operator=(RenderPass&& moved) {
	colorAttachmentCount = moved.colorAttachmentCount;
	handle = moved.handle;
	context = moved.context;
	moved.handle = VK_NULL_HANDLE;
	return *this;
}


VkRenderPass RenderPass::getHandle() const {
	return handle;
}


uint32_t RenderPass::getColorAttachmentCount() const {
	return colorAttachmentCount;
}
