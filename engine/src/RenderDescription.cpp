#include "Logger.hpp"
#include "RenderDescription.hpp"


using std::move;


RenderDescription::RenderDescription(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderPipeline> pipeline)
	:	sampler(context) {
	this->context = context;
	this->pipeline = pipeline;
	createDescriptorPool();
	createDescriptor();
}


RenderDescription::RenderDescription(RenderDescription&& moved)
:	sampler(move(moved.sampler)) {
	descriptorPool = moved.descriptorPool;
	descriptorSet = moved.descriptorSet;
	vertexBufferHandles = move(moved.vertexBufferHandles);
	vertexOffsets = move(moved.vertexOffsets);
	indexBuffer = move(moved.indexBuffer);
	vertexBuffers = move(moved.vertexBuffers);
	uniforms = move(moved.uniforms);
	pipeline = move(moved.pipeline);
	context = moved.context;
	moved.descriptorPool = VK_NULL_HANDLE;
}


RenderDescription::~RenderDescription() {
	if (descriptorPool != VK_NULL_HANDLE) {
		vkDestroyDescriptorPool(context->getDevice(), descriptorPool, nullptr);
		INFO("Destroyed descriptor set ", descriptorSet);
	}
}


RenderDescription& RenderDescription::operator=(RenderDescription&& moved) {
	descriptorPool = moved.descriptorPool;
	descriptorSet = moved.descriptorSet;
	vertexBufferHandles = move(moved.vertexBufferHandles);
	vertexOffsets = move(moved.vertexOffsets);
	indexBuffer = move(moved.indexBuffer);
	vertexBuffers = move(moved.vertexBuffers);
	uniforms = move(moved.uniforms);
	pipeline = move(moved.pipeline);
	context = moved.context;
	sampler = move(moved.sampler);
	moved.descriptorPool = VK_NULL_HANDLE;
	return *this;
}


void RenderDescription::addVertices(shared_ptr<const VertexBuffer> vertexBuffer) {
	vertexBuffers.push_back(vertexBuffer);
	vertexBufferHandles.push_back(vertexBuffer->getHandle());
	vertexOffsets.push_back(0);
}


void RenderDescription::setIndices(shared_ptr<const IndexBuffer> newIndexBuffer) {
	indexBuffer = newIndexBuffer;
}


void RenderDescription::bindUniform(uint32_t index, shared_ptr<const UniformBuffer> uniformBuffer) {
	if (index >= uniforms.size()) {
		uniforms.resize(index + 1);
	}
	uniforms[index] = uniformBuffer;
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = uniformBuffer->getHandle();
	bufferInfo.offset = 0;
	bufferInfo.range = uniformBuffer->getSize();
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = index;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;
	vkUpdateDescriptorSets(context->getDevice(), 1, &descriptorWrite, 0, nullptr);
}


void RenderDescription::bindUniform(uint32_t index, shared_ptr<const Texture> texture) {
	if (index >= uniforms.size()) {
		uniforms.resize(index + 1);
	}
	uniforms[index] = texture;
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = texture->getView();
	imageInfo.sampler = sampler.getHandle();
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = index;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;
	vkUpdateDescriptorSets(context->getDevice(), 1, &descriptorWrite, 0, nullptr);
}


void RenderDescription::bindTo(VkCommandBuffer commandBuffer) const {
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getHandle());
	if (descriptorSet != VK_NULL_HANDLE) {
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getLayout(), 0, 1, &descriptorSet, 0, nullptr);
	}
	vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBufferHandles.data(), vertexOffsets.data());
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT16);
	vkCmdDrawIndexed(commandBuffer, indexBuffer->getSize() / sizeof(uint16_t), 1, 0, 0, 0);
}


void RenderDescription::createDescriptorPool() {
	VkDescriptorPoolSize poolSize0 = {};
	poolSize0.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize0.descriptorCount = 1;
	VkDescriptorPoolSize poolSize1 = {};
	poolSize1.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize1.descriptorCount = 1;
	VkDescriptorPoolSize poolSizes[] = { poolSize0, poolSize1 };
	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.poolSizeCount = 2;
	createInfo.pPoolSizes = poolSizes;
	createInfo.maxSets = 1;
	if (vkCreateDescriptorPool(context->getDevice(), &createInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		ERROR("Failed to create descriptor pool");
	}
	INFO("Created descriptor pool ", descriptorPool);
}


void RenderDescription::createDescriptor() {
	VkDescriptorSetLayout layout = { pipeline->getDescriptorSetLayout() };
	if (layout != VK_NULL_HANDLE) {
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;
		if (vkAllocateDescriptorSets(context->getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
			ERROR("Failed to create descriptor set");
		}
		INFO("Created descriptor set ", descriptorSet);
	}
}
