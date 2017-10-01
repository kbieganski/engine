#include <utility>
#include "Logger.hpp"
#include "RenderPipeline.hpp"


using std::move;


RenderPipeline::RenderPipeline(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderPass> renderPass, vector<shared_ptr<const Shader>>&& shaders, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkDescriptorSetLayout descriptorSetLayout) {
	handle = pipeline;
	layout = pipelineLayout;
	this->descriptorSetLayout = descriptorSetLayout;
	this->context = context;
	this->renderPass = renderPass;
	this->shaders = move(shaders);
}


RenderPipeline::RenderPipeline(RenderPipeline&& moved) {
	*this = move(moved);
}


RenderPipeline::~RenderPipeline() {
	if (handle != VK_NULL_HANDLE) {
		vkDestroyPipeline(context->getDevice(), handle, nullptr);
		INFO("Destroyed render pipeline ", handle);
	}
	if (layout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(context->getDevice(), layout, nullptr);
		INFO("Destroyed graphics pipeline layout ", layout);
	}
	if (descriptorSetLayout != VK_NULL_HANDLE) {
		vkDestroyDescriptorSetLayout(context->getDevice(), descriptorSetLayout, nullptr);
		INFO("Destroyed descriptor set layout ", descriptorSetLayout);
	}
}


RenderPipeline& RenderPipeline::operator=(RenderPipeline&& moved) {
	handle = moved.handle;
	layout = moved.layout;
	descriptorSetLayout = moved.descriptorSetLayout;
	context = move(moved.context);
	renderPass = move(moved.renderPass);
	moved.handle = VK_NULL_HANDLE;
	moved.layout = VK_NULL_HANDLE;
	moved.descriptorSetLayout = VK_NULL_HANDLE;
	return *this;
}


VkPipeline RenderPipeline::getHandle() const {
	return handle;
}


VkPipelineLayout RenderPipeline::getLayout() const {
	return layout;
}


VkDescriptorSetLayout RenderPipeline::getDescriptorSetLayout() const {
	return descriptorSetLayout;
}
