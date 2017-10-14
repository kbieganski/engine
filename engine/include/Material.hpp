#pragma once
#include "Texture.hpp"
#include "UniformBuffer.hpp"


using glm::vec3;


class RenderDescription;
class RenderPipelineBuilder;
class UniformBuffer;


class Material {
public:
	Material(shared_ptr<const GraphicsContext> context, shared_ptr<const Texture> texture, vec3 specularColor, float specularHardness);

	void describe(RenderDescription& renderDescription, uint32_t offset) const;

	static void createUniformBindings(RenderPipelineBuilder& pipelineBuilder, uint32_t offset);


private:
	shared_ptr<const Texture> texture;
	shared_ptr<UniformBuffer> uniformBuffer;
};
