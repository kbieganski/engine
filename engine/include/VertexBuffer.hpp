#pragma once
#include "StagingBuffer.hpp"


class VertexBuffer : public GraphicsBuffer {
public:
	template<typename Vertex>
	VertexBuffer(shared_ptr<const GraphicsContext> context, const vector<Vertex>& vertices);
};


template<typename Vertex>
VertexBuffer::VertexBuffer(shared_ptr<const GraphicsContext> context, const vector<Vertex> &vertices)
:	GraphicsBuffer(context, vertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
	StagingBuffer stagingBuffer(context, size, vertices.data());
	stagingBuffer.copyTo(*this);
}
