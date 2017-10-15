#include "Material.hpp"
#include "Mesh.hpp"
#include "RenderPassBuilder.hpp"
#include "RenderPipelineBuilder.hpp"
#include "SceneRenderer.hpp"


using std::make_shared;
using std::make_unique;


SceneRenderer::SceneRenderer(shared_ptr<const GraphicsContext> context, AssetCache<Shader> &shaderAssets, uvec2 size)
	:	Renderer(context) {
	this->context = context;
	createGeometryBuffer(context->getDeviceDescription(), size);
	createFramebuffer();
	createRenderPipeline(shaderAssets);
}


RenderDescription& SceneRenderer::addRender() {
	return Renderer::addRender(pipeline, 2, 1);
}


void SceneRenderer::createGeometryBuffer(const GraphicsDeviceDescription& deviceDescription, uvec2 size) {
	auto depthFormat = deviceDescription.findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	RenderPassBuilder renderPassBuilder(context);
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderPassBuilder.setDepthStencilAttachment({ depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
	renderTarget = make_shared<RenderTarget>(renderPassBuilder.buildRenderTarget(size));
}


void SceneRenderer::createFramebuffer() {
	framebuffer = make_unique<Framebuffer>(context, renderTarget);
	framebuffer->setClearColor(0, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(1, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(2, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(3, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(4, {{{ 0, 0, 0, 1 }}});
	framebuffer->setClearColor(5, {{{ 1, 0 }}});
}


void SceneRenderer::createRenderPipeline(AssetCache<Shader> &shaderAssets) {
	auto fragmentShader = shaderAssets.load("shaders/scene_frag.spv");
	auto vertexShader = shaderAssets.load("shaders/scene_vert.spv");
	RenderPipelineBuilder pipelineBuilder(context);
	pipelineBuilder.setFragmentShader(fragmentShader);
	pipelineBuilder.setVertexShader(vertexShader);
	Mesh::createAttributeBindings(pipelineBuilder);
	pipelineBuilder.createUniformBufferBinding(0);
	Material::createUniformBindings(pipelineBuilder, 1);
	pipeline = make_shared<RenderPipeline>(pipelineBuilder.build(*renderTarget));
}
