#pragma once
#include "AssetCache.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"


using std::unique_ptr;


class ScreenRenderer {
public:
	ScreenRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, shared_ptr<const RenderTarget> screenSurface, AssetCache<Shader> &shaderAssets);
	ScreenRenderer(ScreenRenderer&& moved) = default;

	ScreenRenderer& operator=(ScreenRenderer&& moved) = default;

private:
	void createScreenRenderDescription(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, shared_ptr<const RenderTarget> screenSurface, AssetCache<Shader> &shaderAssets);

	unique_ptr<RenderDescription> screenRender;
};
