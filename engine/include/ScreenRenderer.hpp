#pragma once
#include "SwapChain.hpp"


using std::unique_ptr;


class ScreenRenderer {
public:
	ScreenRenderer(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, shared_ptr<const RenderTarget> screenSurface);
	ScreenRenderer(ScreenRenderer&& moved) = default;

	ScreenRenderer& operator=(ScreenRenderer&& moved) = default;

private:
	void createScreenRenderDescription(shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain, shared_ptr<const RenderTarget> screenSurface);

	unique_ptr<RenderDescription> screenRender;
};
