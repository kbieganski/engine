#include "RenderPassBuilder.hpp"
#include "Logger.hpp"


using std::make_shared;
using std::move;


RenderPassBuilder::RenderPassBuilder(shared_ptr<const GraphicsContext> context) {
	this->context = context;
}


RenderTarget RenderPassBuilder::buildRenderTarget(uvec2 size) {
	auto renderPass = make_shared<RenderPass>(buildRenderPass());
	return RenderTarget(context, renderPass, size, textureDescriptions);
}


RenderPass RenderPassBuilder::buildRenderPass() {
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = colorAttachmentReferences.size();
	subpass.pColorAttachments = colorAttachmentReferences.data();
	if (usesDepthStencil) {
		subpass.pDepthStencilAttachment = &depthStencilReference;
	}
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	VkRenderPassCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = attachmentDescriptions.size();
	createInfo.pAttachments = attachmentDescriptions.data();
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subpass;
	createInfo.dependencyCount = 1;
	createInfo.pDependencies = &dependency;
	VkRenderPass renderPass;
	if (vkCreateRenderPass(context->getDevice(), &createInfo, nullptr, &renderPass) != VK_SUCCESS) {
		ERROR("Failed to create render pass with ", attachmentDescriptions.size(), " attachments");
	}
	INFO("Created render pass ", renderPass, " with ", attachmentDescriptions.size(), " attachments");
	return RenderPass(context, renderPass);
}


void RenderPassBuilder::addColorAttachment(VkFormat format, VkImageLayout layout) {
	addColorAttachment({ format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_ASPECT_COLOR_BIT, layout });
}


void RenderPassBuilder::addColorAttachment(Texture::Description textureDescription) {
	VkAttachmentDescription attachmentDescription = {};
	attachmentDescription.format = textureDescription.format;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = textureDescription.layout;
	VkAttachmentReference reference = {};
	reference.attachment = attachmentDescriptions.size();
	reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	textureDescriptions.push_back(textureDescription);
	attachmentDescriptions.push_back(attachmentDescription);
	colorAttachmentReferences.push_back(reference);
}


void RenderPassBuilder::setDepthStencilAttachment(Texture::Description textureDescription) {
	VkAttachmentDescription attachmentDescription = {};
	attachmentDescription.format = textureDescription.format;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthStencilReference.attachment = attachmentDescriptions.size();
	depthStencilReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	textureDescriptions.push_back(textureDescription);
	attachmentDescriptions.push_back(attachmentDescription);
	usesDepthStencil = true;
}
