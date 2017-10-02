#include "Framebuffer.hpp"
#include "Logger.hpp"


using std::move;


Framebuffer::Framebuffer(shared_ptr<const GraphicsContext> context, uvec2 screenSize, shared_ptr<RenderTarget> renderTarget) {
	this->context = context;
	this->screenSize = screenSize;
	this->renderTarget = renderTarget;
	clearColors.resize(renderTarget->getTextures().size());
	createFramebuffer();
	commandBuffer = context->createCommandBuffer();
}


Framebuffer::Framebuffer(Framebuffer&& moved) {
	*this = move(moved);
}


Framebuffer::~Framebuffer() {
	if (commandBuffer != VK_NULL_HANDLE) {
		context->destroy(commandBuffer);
	}
	if (handle != VK_NULL_HANDLE) {
		vkDestroyFramebuffer(context->getDevice(), handle, nullptr);
		INFO("Destroyed framebuffer ", handle);
	}
}


Framebuffer& Framebuffer::operator=(Framebuffer&& moved) {
	screenSize = moved.screenSize;
	handle = moved.handle;
	commandBuffer = moved.commandBuffer;
	context = moved.context;
	renderTarget = move(moved.renderTarget);
	clearColors = move(moved.clearColors);
	moved.commandBuffer = VK_NULL_HANDLE;
	moved.handle = VK_NULL_HANDLE;
	return *this;
}


void Framebuffer::bind(const RenderDescription& renderDescription) const {
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr;
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderTarget->getRenderPass()->getHandle();
	renderPassInfo.framebuffer = handle;
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = { screenSize.x, screenSize.y };
	renderPassInfo.clearValueCount = clearColors.size();
	renderPassInfo.pClearValues = clearColors.data();
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	renderDescription.bindTo(commandBuffer);
	vkCmdEndRenderPass(commandBuffer);
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		ERROR("Failed to record render command buffer");
	}
}


void Framebuffer::draw(const vector<VkSemaphore>& waitSemaphores, const vector<VkSemaphore>& signalSemaphores) const {
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = waitSemaphores.size();
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = signalSemaphores.size();
	submitInfo.pSignalSemaphores = signalSemaphores.data();
	context->submit(submitInfo);
}


void Framebuffer::setClearColor(uint32_t attachment, VkClearValue color) {
	clearColors[attachment] = color;
}


void Framebuffer::createFramebuffer() {
	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.renderPass = renderTarget->getRenderPass()->getHandle();
	vector<VkImageView> attachments;
	attachments.reserve(renderTarget->getTextures().size());
	for (auto texture : renderTarget->getTextures()) {
		attachments.push_back(texture->getView());
	}
	createInfo.attachmentCount = attachments.size();
	createInfo.pAttachments = attachments.data();
	createInfo.width = screenSize.x;
	createInfo.height = screenSize.y;
	createInfo.layers = 1;
	INFO("Creating framebuffer with ", attachments.size(), " attachments");
	if (vkCreateFramebuffer(context->getDevice(), &createInfo, nullptr, &handle) != VK_SUCCESS) {
		ERROR("Failed to create framebuffer with ", attachments.size(), " attachments");
	}
	INFO("Created framebuffer ", handle, " with ", attachments.size(), " attachments");
}
