#include "RenderPassBuilder.hpp"
#include "RenderPipelineBuilder.hpp"
#include "ShadowMapRenderer.hpp"


using std::make_shared;
using std::make_unique;


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
};


ShadowMapRenderer::ShadowMapRenderer(shared_ptr<const GraphicsContext> context, const GraphicsDeviceDescription& deviceDescription, uint32_t resolution)
	:	Renderer(context) {
	this->context = context;
	createShadowMap(deviceDescription, resolution);
	createFramebuffer();
	createRenderPipeline();
}


RenderDescription& ShadowMapRenderer::addRender() {
	return Renderer::addRender(pipeline, 1, 0);
}


void ShadowMapRenderer::createShadowMap(const GraphicsDeviceDescription& deviceDescription, uint32_t resolution) {
	auto depthFormat = deviceDescription.findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	RenderPassBuilder renderPassBuilder(context);
	renderPassBuilder.setDepthStencilAttachment({ depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
	uvec2 shadowMapSize = uvec2(resolution, resolution);
	renderTarget = make_shared<RenderTarget>(renderPassBuilder.buildRenderTarget(shadowMapSize));
}


void ShadowMapRenderer::createFramebuffer() {
	framebuffer = make_unique<Framebuffer>(context, renderTarget);
	framebuffer->setClearColor(0, {{{ 1, 0 }}});
}


void ShadowMapRenderer::createRenderPipeline() {
	auto fragmentShader = make_shared<Shader>(context, "shaders/shadowmap_frag.spv");
	auto vertexShader = make_shared<Shader>(context, "shaders/shadowmap_vert.spv");
	RenderPipelineBuilder pipelineBuilder(context);
	pipelineBuilder.setFragmentShader(fragmentShader);
	pipelineBuilder.setVertexShader(vertexShader);
	pipelineBuilder.createUniformBufferBinding(0);
	pipelineBuilder.createAttributeBinding(sizeof(Vertex),
										   { VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32_SFLOAT },
										   { offsetof(Vertex, position), offsetof(Vertex, normal), offsetof(Vertex, textureCoordinates) });
	pipeline = make_shared<RenderPipeline>(pipelineBuilder.build(*renderTarget));
}