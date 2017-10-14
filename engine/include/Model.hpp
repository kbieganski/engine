#pragma once
#include "Material.hpp"
#include "Mesh.hpp"


class Model {
public:
	Model(shared_ptr<const Mesh> mesh, shared_ptr<const Material> material);

	void describe(RenderDescription& renderDescription, uint32_t uniformOffset) const;
	void describeAttributes(RenderDescription& renderDescription) const;
	void describeUniforms(RenderDescription& renderDescription, uint32_t uniformOffset) const;


private:
	shared_ptr<const Material> material;
	shared_ptr<const Mesh> mesh;
};
