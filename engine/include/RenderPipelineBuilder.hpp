#pragma once
#include "RenderPipeline.hpp"


class RenderTarget;


class RenderPipelineBuilder {
public:
	RenderPipelineBuilder(shared_ptr<const GraphicsContext> context);

	RenderPipeline build(const RenderTarget& renderTarget) const;
	RenderPipeline build(shared_ptr<const RenderPass> renderPass, uvec2 screenSize) const;

	void setFragmentShader(shared_ptr<const Shader> shader);
	void setVertexShader(shared_ptr<const Shader> shader);

	void createAttributeBinding(uint32_t stride, const vector<VkFormat>& formats, const vector<uint32_t>& offsets);
	void createUniformBufferBinding(uint32_t index);
	void createTextureBinding(uint32_t index);


private:
	VkPipelineLayout createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout) const;
	VkDescriptorSetLayout createDescriptorSetLayout() const;
	VkPipelineVertexInputStateCreateInfo makeVertexInputStateCreateInfo() const;
	VkPipelineViewportStateCreateInfo makeViewportStateCreateInfo(uvec2 screenSize) const;
	VkPipelineColorBlendStateCreateInfo makeColorBlendStateCreateInfo(const vector<VkPipelineColorBlendAttachmentState>& attachments) const;
	vector<VkPipelineColorBlendAttachmentState> makeColorBlendAttachmentStates(uint32_t attachmentCount) const;
	void makeVertexStageCreateInfo();
	void makeFragmentStageCreateInfo();
	void makeInputAssemblyStateCreateInfo();
	void makeRasterizationStateCreateInfo();
	void makeMultisampleStateCreateInfo();
	void makeDepthStencilStateCreateInfo();
	void makeDynamicStateCreateInfo();

	VkPipelineShaderStageCreateInfo vertexStageCreateInfo = {};
	VkPipelineShaderStageCreateInfo fragmentStageCreateInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
	vector<VkVertexInputBindingDescription> bindingDescriptions;
	vector<VkVertexInputAttributeDescription> attributeDescriptions;
	shared_ptr<const GraphicsContext> context;
	shared_ptr<const Shader> fragmentShader;
	shared_ptr<const Shader> vertexShader;
};
