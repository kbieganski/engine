#define GLM_SWIZZLE
#include "RenderPassBuilder.hpp"
#include "RenderPipelineBuilder.hpp"
#include "ShadingRenderer.hpp"


using std::make_shared;
using std::make_unique;
using std::move;


static const vector<float> screenRectangleVertexData = { -1,  1, 0, 1,
														  1, -1, 1, 0,
														 -1, -1, 0, 0,
														  1,  1, 1, 1 };
static const vector<uint32_t> screenRectangleIndexData = { 0, 1, 2, 1, 0, 3 };


ShadingRenderer::ShadingRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderTarget> geometryBuffer, AssetCache<Shader> &shaderAssets)
	:	Renderer(context) {
	this->geometryBuffer = geometryBuffer;
	createScreenSurface(geometryBuffer->getSize());
	createFramebuffer();
	createAmbientRenderPipeline(shaderAssets);
	createLightRenderPipeline(shaderAssets);
	ambientShadingUniform = make_shared<UniformBuffer>(context, sizeof(AmbientShadingUniform));
	ambientShadingUniform->set(&ambientShadingUniformData);
	screenRectangleVertices = make_shared<VertexBuffer>(context, screenRectangleVertexData);
	screenRectangleIndices = make_shared<IndexBuffer>(context, screenRectangleIndexData);
	auto& description = Renderer::addRender(ambientPipeline, 1, 3);
	description.addVertices(screenRectangleVertices);
	description.setIndices(screenRectangleIndices);
	description.bindUniform(0, ambientShadingUniform);
	description.bindUniform(1, geometryBuffer->getTextures()[1]);
	description.bindUniform(2, geometryBuffer->getTextures()[2]);
}


ShadingRenderer::ShadingRenderer(ShadingRenderer&& moved)
	:	Renderer(move(moved)) {
	geometryBuffer = move(moved.geometryBuffer);
	ambientPipeline = move(moved.ambientPipeline);
	lightPipeline = move(moved.lightPipeline);
}


ShadingRenderer::~ShadingRenderer() {
	if (framebuffer) {
		delete framebuffer;
	}
}


ShadingRenderer& ShadingRenderer::operator=(ShadingRenderer&& moved) {
	static_cast<Renderer&>(*this) = move(moved);
	geometryBuffer = move(moved.geometryBuffer);
	ambientPipeline = move(moved.ambientPipeline);
	lightPipeline = move(moved.lightPipeline);
	return *this;
}


RenderDescription& ShadingRenderer::addRender(shared_ptr<const UniformBuffer> uniformBuffer, shared_ptr<const Texture> shadowMap) {
	auto& description = Renderer::addRender(lightPipeline, 1, 6);
	description.bindUniform(0, uniformBuffer);
	description.bindUniform(1, geometryBuffer->getTextures()[0]);
	description.bindUniform(2, geometryBuffer->getTextures()[1]);
	description.bindUniform(3, geometryBuffer->getTextures()[2]);
	description.bindUniform(4, geometryBuffer->getTextures()[3]);
	description.bindUniform(5, geometryBuffer->getTextures()[4]);
	description.bindUniform(6, shadowMap);
	description.addVertices(screenRectangleVertices);
	description.setIndices(screenRectangleIndices);
	return description;
}


void ShadingRenderer::setAmbientLightColor(vec3 ambientLightColor) {
	ambientShadingUniformData.ambientLightColor = vec4(ambientLightColor, 1);
	ambientShadingUniform->set(&ambientShadingUniformData);
}


void ShadingRenderer::setSkyColor(vec3 skyColor) {
	ambientShadingUniformData.skyColor = vec4(skyColor, 1);
	ambientShadingUniform->set(&ambientShadingUniformData);
}


vec3 ShadingRenderer::getAmbientLightColor() const {
	return ambientShadingUniformData.ambientLightColor.xyz();
}


vec3 ShadingRenderer::getSkyColor() const {
	return ambientShadingUniformData.skyColor.xyz();
}


void ShadingRenderer::createScreenSurface(uvec2 size) {
	RenderPassBuilder renderPassBuilder(context);
	renderPassBuilder.addColorAttachment({ VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	renderTarget = make_shared<RenderTarget>(renderPassBuilder.buildRenderTarget(size));
}


void ShadingRenderer::createFramebuffer() {
	auto framebuffer = new Framebuffer(context, renderTarget);
	framebuffer->setClearColor(0, {{{ 0, 0, 0, 1 }}});
	this->framebuffer = framebuffer;
}


void ShadingRenderer::createAmbientRenderPipeline(AssetCache<Shader> &shaderAssets) {
	auto frag = shaderAssets.load("shaders/ambient_shading_frag.spv");
	auto vert = shaderAssets.load("shaders/shading_vert.spv");
	RenderPipelineBuilder pipelineBuilder(context, *renderTarget);
	pipelineBuilder.setFragmentShader(frag);
	pipelineBuilder.setVertexShader(vert);
	pipelineBuilder.createUniformBufferBinding(0);
	pipelineBuilder.createTextureBinding(1);
	pipelineBuilder.createTextureBinding(2);
	pipelineBuilder.createAttributeBinding(4 * sizeof(float), { VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32_SFLOAT }, { 0, 2 * sizeof(float) });
	ambientPipeline = make_shared<RenderPipeline>(pipelineBuilder.build());
}


void ShadingRenderer::createLightRenderPipeline(AssetCache<Shader> &shaderAssets) {
	auto frag = shaderAssets.load("shaders/light_shading_frag.spv");
	auto vert = shaderAssets.load("shaders/shading_vert.spv");
	RenderPipelineBuilder pipelineBuilder(context, *renderTarget);
	pipelineBuilder.setFragmentShader(frag);
	pipelineBuilder.setVertexShader(vert);
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
	colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentState.blendEnable = VK_TRUE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	pipelineBuilder.setColorBlendAttachmentState(0, colorBlendAttachmentState);
	pipelineBuilder.createUniformBufferBinding(0);
	pipelineBuilder.createTextureBinding(1);
	pipelineBuilder.createTextureBinding(2);
	pipelineBuilder.createTextureBinding(3);
	pipelineBuilder.createTextureBinding(4);
	pipelineBuilder.createTextureBinding(5);
	pipelineBuilder.createTextureBinding(6);
	pipelineBuilder.createAttributeBinding(4 * sizeof(float), { VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32_SFLOAT }, { 0, 2 * sizeof(float) });
	lightPipeline = make_shared<RenderPipeline>(pipelineBuilder.build());
}
