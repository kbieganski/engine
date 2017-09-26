#pragma once
#include "GraphicsBuffer.hpp"


class UniformBuffer : public GraphicsBuffer {
public:
	UniformBuffer(shared_ptr<const GraphicsContext> context, size_t size);

	void set(const void* uniform);
};
