#pragma once
#include "Texture.hpp"


class Sampler {
public:
	Sampler(shared_ptr<const GraphicsContext> context);
	Sampler(const Sampler& copied) = delete;
	Sampler(Sampler&& moved);
	~Sampler();

	Sampler& operator=(const Sampler& copied) = delete;
	Sampler& operator=(Sampler&& moved);

	VkSampler getHandle() const;


private:
	void create();

	VkSampler handle;
	shared_ptr<const GraphicsContext> context;
};
