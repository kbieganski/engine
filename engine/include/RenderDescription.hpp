#pragma once
#include "IndexBuffer.hpp"
#include "RenderPipeline.hpp"
#include "Sampler.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"


class RenderDescription {
public:
	RenderDescription(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderPipeline> pipeline);
	RenderDescription(const RenderDescription&) = delete;
	RenderDescription(RenderDescription&& moved);
	~RenderDescription();

	RenderDescription& operator=(const RenderDescription&) = delete;
	RenderDescription& operator=(RenderDescription&& moved);

	void addVertices(shared_ptr<const VertexBuffer> VertexBuffer);
	void setIndices(shared_ptr<const IndexBuffer> IndexBuffer);
	void bindUniform(uint32_t index, shared_ptr<const UniformBuffer> uniformBuffer);
	void bindUniform(uint32_t index, shared_ptr<const Texture> texture);
	void bindTo(VkCommandBuffer commandBuffer) const;


private:
	void createDescriptorPool();
	void createDescriptor();

	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	vector<VkBuffer> vertexBufferHandles;
	vector<VkDeviceSize> vertexOffsets;
	shared_ptr<const IndexBuffer> indexBuffer;
	vector<shared_ptr<const VertexBuffer>> vertexBuffers;
	vector<shared_ptr<const void>> uniforms;
	shared_ptr<const RenderPipeline> pipeline;
	shared_ptr<const GraphicsContext> context;
	Sampler sampler;
};
