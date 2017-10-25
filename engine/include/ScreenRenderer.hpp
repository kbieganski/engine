#pragma once
#include "AssetCache.hpp"
#include "Renderer.hpp"
#include "SwapChain.hpp"


using std::unique_ptr;


class ScreenRenderer : public Renderer {
public:
	ScreenRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, shared_ptr<const RenderTarget> screenSurface, AssetCache<Shader> &shaderAssets);

	virtual void render();


private:
	void createScreenRenderDescription(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderTarget> screenSurface, AssetCache<Shader> &shaderAssets);

	shared_ptr<const SwapChain> swapChain;
};
