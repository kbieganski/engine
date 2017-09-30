#include "Logger.hpp"
#include "RenderPass.hpp"


using std::make_shared;
using std::move;


RenderPass::RenderPass(shared_ptr<const GraphicsContext> context, VkRenderPass handle) {
	this->context = context;
	this->handle = handle;
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
	context = moved.context;
	handle = moved.handle;
	moved.handle = VK_NULL_HANDLE;
	return *this;
}


VkRenderPass RenderPass::getHandle() const {
	return handle;
}
