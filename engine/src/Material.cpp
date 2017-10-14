#include "Material.hpp"
#include "RenderDescription.hpp"
#include "RenderPipelineBuilder.hpp"


using std::make_shared;
using glm::vec4;


struct MaterialUniform {
	vec4 specularColor;
	float specularHardness;
};


Material::Material(shared_ptr<const GraphicsContext> context, shared_ptr<const Texture> texture, vec3 specularColor, float specularHardness) {
	this->texture = texture;
	MaterialUniform uniform = { vec4(specularColor, 1), specularHardness };
	uniformBuffer = make_shared<UniformBuffer>(context, sizeof(MaterialUniform));
	uniformBuffer->set(&uniform);
}


void Material::describe(RenderDescription& renderDescription, uint32_t offset) const {
	renderDescription.bindUniform(offset + 0, uniformBuffer);
	renderDescription.bindUniform(offset + 1, texture);
}


void Material::createUniformBindings(RenderPipelineBuilder& pipelineBuilder, uint32_t offset) {
	pipelineBuilder.createUniformBufferBinding(offset + 0);
	pipelineBuilder.createTextureBinding(offset + 1);
}
