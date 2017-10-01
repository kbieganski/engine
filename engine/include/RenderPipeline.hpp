#pragma once
#include "RenderPass.hpp"
#include "Shader.hpp"


class RenderPipeline {
public:
	RenderPipeline(shared_ptr<const GraphicsContext> context, shared_ptr<const RenderPass> renderPass, vector<shared_ptr<const Shader>>&& shaders, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkDescriptorSetLayout descriptorSetLayout);
	RenderPipeline(const RenderPipeline&) = delete;
	RenderPipeline(RenderPipeline&& moved);
	~RenderPipeline();

	RenderPipeline& operator=(const RenderPipeline&) = delete;
	RenderPipeline& operator=(RenderPipeline&& moved);

	VkPipeline getHandle() const;
	VkPipelineLayout getLayout() const;
	VkDescriptorSetLayout getDescriptorSetLayout() const;


private:
	VkPipeline handle;
	VkPipelineLayout layout;
	VkDescriptorSetLayout descriptorSetLayout;
	shared_ptr<const GraphicsContext> context;
	shared_ptr<const RenderPass> renderPass;
	vector<shared_ptr<const Shader>> shaders;
};
