#pragma once
#include "RenderPipeline.hpp"


class RenderPipelineBuilder {
public:
	RenderPipelineBuilder(shared_ptr<const GraphicsContext> context, uvec2 screenSize);

	RenderPipeline build(shared_ptr<const RenderPass> renderPass, uint32_t attachmentCount) const;

	void setFragmentShader(shared_ptr<const Shader> shader);
	void setVertexShader(shared_ptr<const Shader> shader);

	void createAttributeBinding(uint32_t stride, const vector<VkFormat>& formats, const vector<uint32_t>& offsets);
	void createBufferBinding(uint32_t index);
	void createSamplerBinding(uint32_t index);


private:
	VkPipelineLayout createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout) const;
	VkDescriptorSetLayout createDescriptorSetLayout() const;
	VkPipelineVertexInputStateCreateInfo makeVertexInputStateCreateInfo() const;
	VkPipelineColorBlendStateCreateInfo makeColorBlendStateCreateInfo(const vector<VkPipelineColorBlendAttachmentState>& attachments) const;
	vector<VkPipelineColorBlendAttachmentState> makeColorBlendAttachmentStates(uint32_t attachmentCount) const;
	void makeVertexStageCreateInfo();
	void makeFragmentStageCreateInfo();
	void makeInputAssemblyStateCreateInfo();
	void makeViewportStateCreateInfo(uvec2 screenSize);
	void makeRasterizationStateCreateInfo();
	void makeMultisampleStateCreateInfo();
	void makeDepthStencilStateCreateInfo();
	void makeDynamicStateCreateInfo();

	VkPipelineShaderStageCreateInfo vertexStageCreateInfo = {};
	VkPipelineShaderStageCreateInfo fragmentStageCreateInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
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
