#pragma once
#include "RenderPass.hpp"
#include "Texture.hpp"


class RenderTarget {
public:
	RenderTarget(shared_ptr<const GraphicsContext> context, shared_ptr<RenderPass> renderPass, uvec2 size, const vector<Texture::Description> &textureDescriptions);
	RenderTarget(shared_ptr<const GraphicsContext> context, shared_ptr<RenderPass> renderPass, shared_ptr<Texture> texture);
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget(RenderTarget&& moved) = default;

	RenderTarget& operator=(const RenderTarget&) = delete;
	RenderTarget& operator=(RenderTarget&& moved) = default;

	shared_ptr<const RenderPass> getRenderPass() const;
	const vector<shared_ptr<Texture>>& getTextures() const;


private:
	shared_ptr<const GraphicsContext> context;
	shared_ptr<RenderPass> renderPass;
	vector<shared_ptr<Texture>> textures;
};
