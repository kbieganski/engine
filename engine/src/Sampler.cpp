#include "Logger.hpp"
#include "Sampler.hpp"


using std::move;


Sampler::Sampler(shared_ptr<const GraphicsContext> context) {
	this->context = context;
	create();
}


Sampler::Sampler(Sampler&& moved) {
	*this = move(moved);
}


Sampler::~Sampler() {
	if (handle != VK_NULL_HANDLE) {
		vkDestroySampler(context->getDevice(), handle, nullptr);
		INFO("Destroyed sampler ", handle);
	}
}


Sampler& Sampler::operator=(Sampler&& moved) {
	handle = moved.handle;
	context = move(moved.context);
	moved.handle = VK_NULL_HANDLE;
	return *this;
}


VkSampler Sampler::getHandle() const {
	return handle;
}


void Sampler::create() {
	VkSamplerCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.magFilter = VK_FILTER_LINEAR;
	createInfo.minFilter = VK_FILTER_LINEAR;
	createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.anisotropyEnable = VK_TRUE;
	createInfo.maxAnisotropy = 16;
	createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	createInfo.unnormalizedCoordinates = VK_FALSE;
	createInfo.compareEnable = VK_FALSE;
	createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	createInfo.mipLodBias = 0.0f;
	createInfo.minLod = 0.0f;
	createInfo.maxLod = 0.0f;
	if (vkCreateSampler(context->getDevice(), &createInfo, nullptr, &handle) != VK_SUCCESS) {
		ERROR("Failed to create texture sampler");
	}
	INFO("Created sampler ", handle);
}
