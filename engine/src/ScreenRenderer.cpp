#include "RenderPassBuilder.hpp"
#include "RenderPipelineBuilder.hpp"
#include "ScreenRenderer.hpp"


using std::make_shared;
using std::make_unique;


static const vector<float> screenRectangleVertexData = { -1,  1, 0, 1,
														  1, -1, 1, 0,
														 -1, -1, 0, 0,
														  1,  1, 1, 1 };
static const vector<uint32_t> screenRectangleIndexData = { 0, 1, 2, 1, 0, 3 };


ScreenRenderer::ScreenRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, shared_ptr<const RenderTarget> screenSurface, AssetCache<Shader> &shaderAssets) {
	createScreenRenderDescription(context, swapChain, screenSurface, shaderAssets);
	swapChain->bind(*screenRender);
}


void ScreenRenderer::createScreenRenderDescription(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, shared_ptr<const RenderTarget> screenSurface, AssetCache<Shader> &shaderAssets) {
	auto screenVertexShader = make_shared<Shader>(context, "shaders/screen_vert.spv");
	auto screenFragmentShader = make_shared<Shader>(context, "shaders/screen_frag.spv");
	RenderPipelineBuilder pipelineBuilder(context);
	pipelineBuilder.setFragmentShader(screenFragmentShader);
	pipelineBuilder.setVertexShader(screenVertexShader);
	pipelineBuilder.createAttributeBinding(4 * sizeof(float), { VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32_SFLOAT }, { 0, 2 * sizeof(float) });
	pipelineBuilder.createTextureBinding(0);
	auto screenPipeline = make_shared<RenderPipeline>(pipelineBuilder.build(swapChain->getRenderPass(), swapChain->getScreenSize()));
	auto screenRectangleVertices = make_shared<VertexBuffer>(context, screenRectangleVertexData);
	auto screenRectangleIndices = make_shared<IndexBuffer>(context, screenRectangleIndexData);
	screenRender = make_unique<RenderDescription>(context, screenPipeline, 1, 1);
	screenRender->addVertices(screenRectangleVertices);
	screenRender->setIndices(screenRectangleIndices);
	screenRender->bindUniform(0, screenSurface->getTextures()[0]);
}
