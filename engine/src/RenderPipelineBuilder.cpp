#include <algorithm>
#include "Logger.hpp"
#include "RenderPipelineBuilder.hpp"
#include "RenderTarget.hpp"


using std::min;


RenderPipelineBuilder::RenderPipelineBuilder(shared_ptr<const GraphicsContext> context, const RenderTarget& renderTarget)
	:	RenderPipelineBuilder(context, renderTarget.getRenderPass(), renderTarget.getSize()) {

}


RenderPipelineBuilder::RenderPipelineBuilder(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderPass> renderPass, uvec2 screenSize) {
	this->screenSize = screenSize;
	this->context = context;
	this->renderPass = renderPass;
	makeColorBlendAttachmentStates(renderPass->getColorAttachmentCount());
	makeVertexStageCreateInfo();
	makeFragmentStageCreateInfo();
	makeInputAssemblyStateCreateInfo();
	makeRasterizationStateCreateInfo();
	makeMultisampleStateCreateInfo();
	makeDepthStencilStateCreateInfo();
	makeDynamicStateCreateInfo();
}


RenderPipeline RenderPipelineBuilder::build() const {
	auto vertexInputStateCreateInfo = makeVertexInputStateCreateInfo();
	VkGraphicsPipelineCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	createInfo.stageCount = 2;
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexStageCreateInfo, fragmentStageCreateInfo };
	createInfo.pStages = shaderStages;
	createInfo.pVertexInputState = &vertexInputStateCreateInfo;
	createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	auto viewportStateCreateInfo = makeViewportStateCreateInfo(screenSize);
	createInfo.pViewportState = &viewportStateCreateInfo;
	createInfo.pRasterizationState = &rasterizationStateCreateInfo;
	createInfo.pMultisampleState = &multisampleStateCreateInfo;
	createInfo.pDepthStencilState = &depthStencilStateCreateInfo;
	auto colorBlendStateCreateInfo = makeColorBlendStateCreateInfo();
	createInfo.pColorBlendState = &colorBlendStateCreateInfo;
	createInfo.pDynamicState = nullptr;
	auto descriptorSetLayout = createDescriptorSetLayout();
	createInfo.layout = createPipelineLayout(descriptorSetLayout);
	createInfo.renderPass = renderPass->getHandle();
	createInfo.subpass = 0;
	createInfo.basePipelineHandle = VK_NULL_HANDLE;
	createInfo.basePipelineIndex = -1;
	VkPipeline graphicsPipeline;
	if (vkCreateGraphicsPipelines(context->getDevice(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		ERROR("Failed to create graphics pipeline");
	}
	INFO("Created render pipeline ", graphicsPipeline);
	return RenderPipeline(context, renderPass, { vertexShader, fragmentShader }, graphicsPipeline, createInfo.layout, descriptorSetLayout);
}


void RenderPipelineBuilder::createAttributeBinding(uint32_t stride, const vector<VkFormat>& formats, const vector<uint32_t>& offsets) {
	uint32_t binding = bindingDescriptions.size();
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = binding;
	bindingDescription.stride = stride;
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	bindingDescriptions.push_back(bindingDescription);
	for (size_t i = 0; i < min(formats.size(), offsets.size()); i++) {
		auto format = formats[i];
		auto offset = offsets[i];
		VkVertexInputAttributeDescription attributeDescription = {};
		attributeDescription.binding = binding;
		attributeDescription.format = format;
		attributeDescription.location = i;
		attributeDescription.offset = offset;
		attributeDescriptions.push_back(attributeDescription);
	}
}


void RenderPipelineBuilder::createUniformBufferBinding(uint32_t index) {
	VkDescriptorSetLayoutBinding binding = {};
	binding.binding = index;
	binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	binding.descriptorCount = 1;
	binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	binding.pImmutableSamplers = nullptr;
	descriptorSetLayoutBindings.push_back(binding);
}


void RenderPipelineBuilder::createTextureBinding(uint32_t index) {
	VkDescriptorSetLayoutBinding binding = {};
	binding.binding = index;
	binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	binding.descriptorCount = 1;
	binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	binding.pImmutableSamplers = nullptr;
	descriptorSetLayoutBindings.push_back(binding);
}


VkPipelineLayout RenderPipelineBuilder::createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout) const {
	VkPipelineLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	if (descriptorSetLayout != VK_NULL_HANDLE) {
		createInfo.setLayoutCount = 1;
		createInfo.pSetLayouts = &descriptorSetLayout;
	}
	createInfo.pushConstantRangeCount = 0;
	createInfo.pPushConstantRanges = 0;
	VkPipelineLayout pipelineLayout;
	if (vkCreatePipelineLayout(context->getDevice(), &createInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		ERROR("Failed to create graphics pipeline layout");
	}
	INFO("Created graphics pipeline layout ", pipelineLayout);
	return pipelineLayout;
}


VkDescriptorSetLayout RenderPipelineBuilder::createDescriptorSetLayout() const {
	if (descriptorSetLayoutBindings.size() == 0) {
		return VK_NULL_HANDLE;
	}
	VkDescriptorSetLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.bindingCount = descriptorSetLayoutBindings.size();
	createInfo.pBindings = descriptorSetLayoutBindings.data();
	VkDescriptorSetLayout descriptorSetLayout;
	if (vkCreateDescriptorSetLayout(context->getDevice(), &createInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		ERROR("Failed to create descriptor set layout");
	}
	INFO("Created descriptor set layout ", descriptorSetLayout);
	return descriptorSetLayout;
}


void RenderPipelineBuilder::setFragmentShader(shared_ptr<const Shader> shader) {
	fragmentShader = shader;
	fragmentStageCreateInfo.module = shader->getHandle();
	fragmentStageCreateInfo.pName = "main";
}


void RenderPipelineBuilder::setVertexShader(shared_ptr<const Shader> shader) {
	vertexShader = shader;
	vertexStageCreateInfo.module = shader->getHandle();
	vertexStageCreateInfo.pName = "main";
}


void RenderPipelineBuilder::setColorBlendAttachmentState(uint32_t attachment, VkPipelineColorBlendAttachmentState state) {
	colorBlendAttachmentStates[attachment] = state;
}


VkPipelineViewportStateCreateInfo RenderPipelineBuilder::makeViewportStateCreateInfo(uvec2 screenSize) const {
	static VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(screenSize.x);
	viewport.height = static_cast<float>(screenSize.y);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	static VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = { screenSize.x, screenSize.y };
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;
	return viewportStateCreateInfo;
}


VkPipelineVertexInputStateCreateInfo RenderPipelineBuilder::makeVertexInputStateCreateInfo() const {
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
	vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	return vertexInputStateCreateInfo;
}


VkPipelineColorBlendStateCreateInfo RenderPipelineBuilder::makeColorBlendStateCreateInfo() const {
	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
	colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateCreateInfo.attachmentCount = colorBlendAttachmentStates.size();
	colorBlendStateCreateInfo.pAttachments = colorBlendAttachmentStates.data();
	colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[3] = 0.0f;
	return colorBlendStateCreateInfo;
}


void RenderPipelineBuilder::makeColorBlendAttachmentStates(uint32_t attachmentCount) {
	colorBlendAttachmentStates.resize(attachmentCount);
	for (auto& colorBlendAttachmentState : colorBlendAttachmentStates) {
		colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachmentState.blendEnable = VK_FALSE;
		colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	}
}


void RenderPipelineBuilder::makeVertexStageCreateInfo() {
	vertexStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
}


void RenderPipelineBuilder::makeFragmentStageCreateInfo() {
	fragmentStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
}


void RenderPipelineBuilder::makeInputAssemblyStateCreateInfo() {
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
}


void RenderPipelineBuilder::makeRasterizationStateCreateInfo() {
	rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationStateCreateInfo.lineWidth = 1.0f;
	rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
	rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
}


void RenderPipelineBuilder::makeMultisampleStateCreateInfo() {
	multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleStateCreateInfo.minSampleShading = 1.0f;
	multisampleStateCreateInfo.pSampleMask = nullptr;
	multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
}


void RenderPipelineBuilder::makeDepthStencilStateCreateInfo() {
	depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
	depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
	depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilStateCreateInfo.minDepthBounds = 0.0f;
	depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
	depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
	depthStencilStateCreateInfo.front = {};
	depthStencilStateCreateInfo.back = {};
}


void RenderPipelineBuilder::makeDynamicStateCreateInfo() {
	static VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = 2;
	dynamicStateCreateInfo.pDynamicStates = dynamicStates;
}
