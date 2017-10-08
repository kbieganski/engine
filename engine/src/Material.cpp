#include "Material.hpp"
#include "RenderDescription.hpp"
#include "RenderPipelineBuilder.hpp"


using std::make_shared;


struct MaterialUniform {
	float specularHardness;
	vec3 specularColor;
};


Material::Material(shared_ptr<const GraphicsContext> context, shared_ptr<const Texture> texture, vec3 specularColor, float specularHardness) {
	this->texture = texture;
	MaterialUniform uniform = { specularHardness, specularColor };
	uniformBuffer = make_shared<UniformBuffer>(context, sizeof(MaterialUniform));
	uniformBuffer->set(&uniform);
}


void Material::bindTo(RenderDescription& renderDescription, uint32_t offset) const {
	renderDescription.bindUniform(offset + 0, uniformBuffer);
	renderDescription.bindUniform(offset + 1, texture);
}


void Material::createUniformBindings(RenderPipelineBuilder& pipelineBuilder, uint32_t offset) const {
	pipelineBuilder.createBufferBinding(offset + 0);
	pipelineBuilder.createSamplerBinding(offset + 1);
}
