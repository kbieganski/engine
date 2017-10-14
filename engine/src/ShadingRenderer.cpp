#include "RenderPassBuilder.hpp"
#include "RenderPipelineBuilder.hpp"
#include "ShadingRenderer.hpp"


using std::make_shared;
using std::make_unique;


static const vector<float> screenRectangleVertexData = { -1,  1, 0, 1,
														  1, -1, 1, 0,
														 -1, -1, 0, 0,
														  1,  1, 1, 1 };
static const vector<uint32_t> screenRectangleIndexData = { 0, 1, 2, 1, 0, 3 };


ShadingRenderer::ShadingRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderTarget> geometryBuffer)
	:	Renderer(context) {
	this->geometryBuffer = geometryBuffer;
	createScreenSurface(geometryBuffer->getSize());
	createFramebuffer();
	createShadingRenderDescription();
}


RenderDescription& ShadingRenderer::addRender(shared_ptr<const UniformBuffer> uniformBuffer, shared_ptr<const Texture> shadowMap) {
	auto& description = Renderer::addRender(pipeline, 1, 6);
	description.bindUniform(0, uniformBuffer);
	description.bindUniform(1, geometryBuffer->getTextures()[0]);
	description.bindUniform(2, geometryBuffer->getTextures()[1]);
	description.bindUniform(3, geometryBuffer->getTextures()[2]);
	description.bindUniform(4, geometryBuffer->getTextures()[3]);
	description.bindUniform(5, geometryBuffer->getTextures()[4]);
	description.bindUniform(6, shadowMap);
	auto screenRectangleVertices = make_shared<VertexBuffer>(context, screenRectangleVertexData);
	auto screenRectangleIndices = make_shared<IndexBuffer>(context, screenRectangleIndexData);
	description.addVertices(screenRectangleVertices);
	description.setIndices(screenRectangleIndices);
	return description;
}


void ShadingRenderer::createScreenSurface(uvec2 size) {
	RenderPassBuilder renderPassBuilder(context);
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderTarget = make_shared<RenderTarget>(renderPassBuilder.buildRenderTarget(size));
}


void ShadingRenderer::createFramebuffer() {
	framebuffer = make_unique<Framebuffer>(context, renderTarget);
	framebuffer->setClearColor(0, {{{ 0, 0, 0, 1 }}});
}


void ShadingRenderer::createShadingRenderDescription() {
	auto frag = make_shared<Shader>(context, "shaders/shading_frag.spv");
	auto vert = make_shared<Shader>(context, "shaders/shading_vert.spv");
	RenderPipelineBuilder pipelineBuilder(context);
	pipelineBuilder.setFragmentShader(frag);
	pipelineBuilder.setVertexShader(vert);
	pipelineBuilder.createUniformBufferBinding(0);
	pipelineBuilder.createTextureBinding(1);
	pipelineBuilder.createTextureBinding(2);
	pipelineBuilder.createTextureBinding(3);
	pipelineBuilder.createTextureBinding(4);
	pipelineBuilder.createTextureBinding(5);
	pipelineBuilder.createTextureBinding(6);
	pipelineBuilder.createAttributeBinding(4 * sizeof(float), { VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32_SFLOAT }, { 0, 2 * sizeof(float) });
	pipeline = make_shared<RenderPipeline>(pipelineBuilder.build(*renderTarget));
}
