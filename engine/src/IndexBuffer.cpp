#include "IndexBuffer.hpp"
#include "StagingBuffer.hpp"


IndexBuffer::IndexBuffer(shared_ptr<const GraphicsContext> context, const vector<uint32_t> &indices)
	:	GraphicsBuffer(context, indices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
	fillMemory(indices);
}


void IndexBuffer::fillMemory(const vector<uint32_t> &indices) const {
	StagingBuffer stagingBuffer(context, size, indices.data());
	stagingBuffer.copyTo(*this);
}
