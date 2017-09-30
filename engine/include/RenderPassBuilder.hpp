#pragma once
#include "RenderTarget.hpp"


class RenderPassBuilder {
public:
	RenderPassBuilder(shared_ptr<const GraphicsContext> context);

	RenderTarget buildRenderTarget(uvec2 size);
	RenderPass buildRenderPass();
	void addColorAttachment(VkFormat format, VkImageLayout layout);
	void addColorAttachment(Texture::Description textureDescription);
	void setDepthStencilAttachment(Texture::Description textureDescription);


private:
	bool usesDepthStencil = false;
	bool ownDepthStencil = false;
	size_t depthStencilTextureIndex;
	uvec2 size;
	shared_ptr<const GraphicsContext> context;
	VkAttachmentReference depthStencilReference = {};
	vector<VkAttachmentDescription> attachmentDescriptions;
	vector<VkAttachmentReference> colorAttachmentReferences;
	vector<Texture::Description> textureDescriptions;
};
