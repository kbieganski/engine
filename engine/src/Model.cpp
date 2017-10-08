#include "Model.hpp"


Model::Model(shared_ptr<const Mesh> mesh, shared_ptr<const Material> material) {
	this->mesh = mesh;
	this->material = material;
}


void Model::createAttributeBindings(RenderPipelineBuilder& pipelineBuilder) const {
	mesh->createAttributeBindings(pipelineBuilder);
}


void Model::createUniformBindings(RenderPipelineBuilder& pipelineBuilder, uint32_t offset) const {
	material->createUniformBindings(pipelineBuilder, offset);
}


void Model::describe(RenderDescription& renderDescription, uint32_t uniformOffset) const {
	describeAttributes(renderDescription);
	describeUniforms(renderDescription, uniformOffset);
}


void Model::describeAttributes(RenderDescription& renderDescription) const {
	mesh->describe(renderDescription);
}


void Model::describeUniforms(RenderDescription& renderDescription, uint32_t uniformOffset) const {
	material->describe(renderDescription, uniformOffset);
}
