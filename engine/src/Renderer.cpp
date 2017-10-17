#include "Renderer.hpp"
#include "Logger.hpp"


using std::move;


Renderer::Renderer(shared_ptr<const GraphicsContext> context) {
	this->context = context;
	commandBuffer = context->createCommandBuffer();
}


Renderer::Renderer(Renderer&& moved) {
	*this = move(moved);
}


Renderer::~Renderer() {
	if (commandBuffer != VK_NULL_HANDLE) {
		context->destroy(commandBuffer);
	}
}


Renderer& Renderer::operator=(Renderer&& moved) {
	commandBuffer = moved.commandBuffer;
	context = moved.context;
	renderTarget = move(moved.renderTarget);
	framebuffer = move(moved.framebuffer);
	moved.commandBuffer = VK_NULL_HANDLE;
	moved.framebuffer = nullptr;
	return *this;
}


void Renderer::render() {
	if (dirty) {
		recreateCommandBuffer();
	}
	framebuffer->draw(commandBuffer);
}


shared_ptr<const RenderTarget> Renderer::getRenderTarget() const {
	return renderTarget;
}


RenderDescription& Renderer::addRender(shared_ptr<RenderPipeline> pipeline, uint32_t uniformBufferCount, uint32_t textureCount) {
	renderDescriptions.push_back(RenderDescription(context, pipeline, uniformBufferCount, textureCount));
	dirty = true;
	return renderDescriptions.back();
}


void Renderer::recreateCommandBuffer() {
	beginCommandBuffer();
	framebuffer->beginRenderPass(commandBuffer);
	for (auto& renderDescription : renderDescriptions) {
		renderDescription.bindTo(commandBuffer);
	}
	vkCmdEndRenderPass(commandBuffer);
	endCommandBuffer();
	dirty = false;
}


void Renderer::beginCommandBuffer() {
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr;
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
}


void Renderer::endCommandBuffer() {
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		ERROR("Failed to record render command buffer");
	}
}
