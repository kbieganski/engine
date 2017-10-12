#include "RenderTarget.hpp"


using std::make_shared;
using std::move;


RenderTarget::RenderTarget(shared_ptr<const GraphicsContext> context, shared_ptr<RenderPass> renderPass, uvec2 size, const vector<Texture::Description> &textureDescriptions) {
	this->size = size;
	this->context = context;
	this->renderPass = renderPass;
	textures.reserve(textureDescriptions.size());
	for (auto& description : textureDescriptions) {
		textures.push_back(make_shared<Texture>(context, size, description));
	}
}


RenderTarget::RenderTarget(shared_ptr<const GraphicsContext> context, shared_ptr<RenderPass> renderPass, shared_ptr<Texture> texture) {
	this->size = texture->getSize();
	this->context = context;
	this->renderPass = renderPass;
	textures.push_back(texture);
}


shared_ptr<const RenderPass> RenderTarget::getRenderPass() const {
	return renderPass;
}


uvec2 RenderTarget::getSize() const {
	return size;
}


const vector<shared_ptr<Texture>>& RenderTarget::getTextures() const {
	return textures;
}
