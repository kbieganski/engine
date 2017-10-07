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
	mesh->describe(renderDescription);
	material->bindTo(renderDescription, uniformOffset);
}
