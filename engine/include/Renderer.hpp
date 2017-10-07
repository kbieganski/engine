#pragma once
#include "GeometryBuffer.hpp"
#include "SwapChain.hpp"


class Renderer {
public:
	Renderer(shared_ptr<const GraphicsContext> context, const GraphicsDeviceDescription& deviceDescription, shared_ptr<SwapChain> swapChain);
	Renderer(Renderer&& moved) = default;

	Renderer& operator=(Renderer&& moved) = default;

	void drawToGeometryBuffer(const RenderDescription& renderDescription) const;
	void drawGeometryToScreen() const;

	const GeometryBuffer& getGeometryBuffer() const;


private:
	void createScreenRenderDescription(shared_ptr<const GraphicsContext> context);

	GeometryBuffer geometryBuffer;
	unique_ptr<RenderDescription> screenRender;
	shared_ptr<SwapChain> swapChain;
};
