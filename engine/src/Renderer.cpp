#include "Renderer.hpp"


Renderer::Renderer(shared_ptr<const GraphicsContext> context) {
	this->context = context;
}


void Renderer::render() const {
	for (auto& renderDescription : renderDescriptions) {
		framebuffer->bind(renderDescription);
		framebuffer->draw({}, {});
	}
}


RenderDescription& Renderer::addRender(shared_ptr<RenderPipeline> pipeline, uint32_t uniformBufferCount, uint32_t textureCount) {
	renderDescriptions.push_back(RenderDescription(context, pipeline, uniformBufferCount, textureCount));
	return renderDescriptions.back();
}


shared_ptr<const RenderTarget> Renderer::getRenderTarget() const {
	return renderTarget;
}
