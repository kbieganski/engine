#pragma once
#include "RenderPipeline.hpp"


class RenderTarget;


class RenderPipelineBuilder {
public:
	RenderPipelineBuilder(shared_ptr<const GraphicsContext> context, const RenderTarget& renderTarget);
	RenderPipelineBuilder(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderPass> renderPass, uvec2 screenSize);

	RenderPipeline build() const;

	void createAttributeBinding(uint32_t stride, const vector<VkFormat>& formats, const vector<uint32_t>& offsets);
	void createUniformBufferBinding(uint32_t index);
	void createTextureBinding(uint32_t index);

	void setFragmentShader(shared_ptr<const Shader> shader);
	void setVertexShader(shared_ptr<const Shader> shader);
	void setColorBlendAttachmentState(uint32_t attachment, VkPipelineColorBlendAttachmentState state);


private:
	VkPipelineLayout createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout) const;
	VkDescriptorSetLayout createDescriptorSetLayout() const;
	VkPipelineVertexInputStateCreateInfo makeVertexInputStateCreateInfo() const;
	VkPipelineViewportStateCreateInfo makeViewportStateCreateInfo(uvec2 screenSize) const;
	VkPipelineColorBlendStateCreateInfo makeColorBlendStateCreateInfo() const;
	void makeColorBlendAttachmentStates(uint32_t attachmentCcount);
	void makeVertexStageCreateInfo();
	void makeFragmentStageCreateInfo();
	void makeInputAssemblyStateCreateInfo();
	void makeRasterizationStateCreateInfo();
	void makeMultisampleStateCreateInfo();
	void makeDepthStencilStateCreateInfo();
	void makeDynamicStateCreateInfo();

	uvec2 screenSize;
	VkPipelineShaderStageCreateInfo vertexStageCreateInfo = {};
	VkPipelineShaderStageCreateInfo fragmentStageCreateInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStates;
	vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
	vector<VkVertexInputBindingDescription> bindingDescriptions;
	vector<VkVertexInputAttributeDescription> attributeDescriptions;
	shared_ptr<const GraphicsContext> context;
	shared_ptr<const RenderPass> renderPass;
	shared_ptr<const Shader> fragmentShader;
	shared_ptr<const Shader> vertexShader;
};
