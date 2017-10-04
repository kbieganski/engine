#pragma once
#include "Framebuffer.hpp"


using std::unique_ptr;


class GeometryBuffer {
public:
	GeometryBuffer(shared_ptr<const GraphicsContext> context, const GraphicsDeviceDescription& deviceDescription, uvec2 screenSize);
	GeometryBuffer(const GeometryBuffer&) = delete;
	GeometryBuffer(GeometryBuffer&&) = default;

	GeometryBuffer& operator=(const GeometryBuffer&) = delete;
	GeometryBuffer& operator=(GeometryBuffer&&) = default;

	void draw(const RenderDescription& renderDescription) const;

	shared_ptr<const RenderTarget> getRenderTarget() const;


private:
	void createRenderTarget(const GraphicsDeviceDescription& deviceDescription, uvec2 screenSize);
	void createFramebuffer(uvec2 screenSize);

	shared_ptr<RenderTarget> renderTarget;
	shared_ptr<const GraphicsContext> context;
	unique_ptr<Framebuffer> framebuffer;
};
