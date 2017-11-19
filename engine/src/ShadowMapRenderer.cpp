#include "Mesh.hpp"
#include "RenderPassBuilder.hpp"
#include "RenderPipelineBuilder.hpp"
#include "ShadowMapRenderer.hpp"


using std::make_shared;
using std::make_unique;
using std::move;


ShadowMapRenderer::ShadowMapRenderer(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uint32_t resolution)
	:	Renderer(context) {
	this->context = context;
	createShadowMap(context->getDeviceDescription(), resolution);
	createFramebuffer();
	createRenderPipeline(shaderAssets);
}


ShadowMapRenderer::ShadowMapRenderer(ShadowMapRenderer&& moved)
	:	Renderer(move(moved)) {
	pipeline = move(moved.pipeline);
	viewProjectionBuffer = move(moved.viewProjectionBuffer);
}


ShadowMapRenderer::~ShadowMapRenderer() {
	if (framebuffer) {
		delete framebuffer;
	}
}


ShadowMapRenderer& ShadowMapRenderer::operator=(ShadowMapRenderer&& moved) {
	static_cast<Renderer&>(*this) = move(moved);
	pipeline = move(moved.pipeline);
	viewProjectionBuffer = move(moved.viewProjectionBuffer);
	return *this;
}


RenderDescription& ShadowMapRenderer::addRender() {
	auto& renderDescription = Renderer::addRender(pipeline, 2, 0);
	renderDescription.bindUniform(1, viewProjectionBuffer);
	return renderDescription;
}


void ShadowMapRenderer::setUniformBuffer(shared_ptr<UniformBuffer> buffer) {
	viewProjectionBuffer = buffer;
}


void ShadowMapRenderer::createShadowMap(const GraphicsDeviceDescription& deviceDescription, uint32_t resolution) {
	auto depthFormat = deviceDescription.findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	RenderPassBuilder renderPassBuilder(context);
	renderPassBuilder.setDepthStencilAttachment({ depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
	uvec2 shadowMapSize = uvec2(resolution, resolution);
	renderTarget = make_shared<RenderTarget>(renderPassBuilder.buildRenderTarget(shadowMapSize));
}


void ShadowMapRenderer::createFramebuffer() {
	auto framebuffer = new Framebuffer(context, renderTarget);
	framebuffer->setClearColor(0, {{{ 1, 0 }}});
	this->framebuffer = framebuffer;
}


void ShadowMapRenderer::createRenderPipeline(AssetCache<Shader> &shaderAssets) {
	auto fragmentShader = shaderAssets.load("shadowmap_frag.spv");
	auto vertexShader = shaderAssets.load("shadowmap_vert.spv");
	RenderPipelineBuilder pipelineBuilder(context, *renderTarget);
	pipelineBuilder.setFrontFace(VK_FRONT_FACE_CLOCKWISE);
	pipelineBuilder.setFragmentShader(fragmentShader);
	pipelineBuilder.setVertexShader(vertexShader);
	pipelineBuilder.createUniformBufferBinding(0);
	pipelineBuilder.createUniformBufferBinding(1);
	Mesh::createAttributeBindings(pipelineBuilder);
	pipeline = make_shared<RenderPipeline>(pipelineBuilder.build());
}
