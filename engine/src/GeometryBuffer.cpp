#include "GeometryBuffer.hpp"
#include "RenderPassBuilder.hpp"


using std::make_shared;
using std::make_unique;


GeometryBuffer::GeometryBuffer(shared_ptr<const GraphicsContext> context, const GraphicsDeviceDescription& deviceDescription, uvec2 screenSize) {
	this->context = context;
	createRenderTarget(deviceDescription, screenSize);
	createFramebuffer(screenSize);
}


void GeometryBuffer::draw(const RenderDescription& renderDescription) const {
	framebuffer->bind(renderDescription);
	framebuffer->draw({}, {});
}


shared_ptr<const RenderTarget> GeometryBuffer::getRenderTarget() const {
	return renderTarget;
}


void GeometryBuffer::createRenderTarget(const GraphicsDeviceDescription& deviceDescription, uvec2 screenSize) {
	auto depthFormat = deviceDescription.findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	RenderPassBuilder renderPassBuilder(context);
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.setDepthStencilAttachment({ depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
	renderTarget = make_shared<RenderTarget>(renderPassBuilder.buildRenderTarget(screenSize));
}


void GeometryBuffer::createFramebuffer(uvec2 screenSize) {
	framebuffer = make_unique<Framebuffer>(context, screenSize, renderTarget);
	framebuffer->setClearColor(0, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(1, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(2, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(3, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(4, {{{ 1, 0 }}});
}
