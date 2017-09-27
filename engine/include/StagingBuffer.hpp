#pragma once
#include "GraphicsBuffer.hpp"


class Texture;


class StagingBuffer : public GraphicsBuffer {
public:
	StagingBuffer(shared_ptr<const GraphicsContext> context, size_t size, const void* data);

	void copyTo(const Texture& texture);
	void copyTo(const GraphicsBuffer& buffer);


private:
	void fillMemory(const void* data);
};
