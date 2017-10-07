#pragma once
#include "GraphicsBuffer.hpp"


using std::vector;


class IndexBuffer : public GraphicsBuffer {
public:
	IndexBuffer(shared_ptr<const GraphicsContext> context, const vector<uint32_t>& indices);


private:
	void fillMemory(const vector<uint32_t>& indices) const;
};
