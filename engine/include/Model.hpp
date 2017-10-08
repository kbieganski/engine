#pragma once
#include "Material.hpp"
#include "Mesh.hpp"


class Model {
public:
	Model(shared_ptr<const Mesh> mesh, shared_ptr<const Material> material);

	void createAttributeBindings(RenderPipelineBuilder& pipelineBuilder) const;
	void createUniformBindings(RenderPipelineBuilder& pipelineBuilder, uint32_t offset) const;
	void describe(RenderDescription& renderDescription, uint32_t uniformOffset) const;


private:
	shared_ptr<const Material> material;
	shared_ptr<const Mesh> mesh;
};