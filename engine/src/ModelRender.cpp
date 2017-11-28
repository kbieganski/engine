#include "GraphicsSystem.hpp"
#include "ModelRender.hpp"


using std::make_shared;
using glm::mat4_cast;


ModelRender::ModelRender(const Transform& _transform, GraphicsSystem& graphicsSystem, shared_ptr<const Model> model)
	:	transform(_transform) {
	this->model = model;
	uniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(Uniform));
	graphicsSystem.add(*this);
}


void ModelRender::addTo(SceneRenderer& sceneRenderer) const {
	auto& renderDescription = sceneRenderer.addRender();
	renderDescription.bindUniform(0, uniformBuffer);
	model->describe(renderDescription, 1);
}


void ModelRender::addTo(ShadowMapRenderer& shadowMapRenderer) const {
	auto& renderDescription = shadowMapRenderer.addRender();
	renderDescription.bindUniform(0, uniformBuffer);
	model->describeAttributes(renderDescription);
}


void ModelRender::update(const mat4& viewProjection) {
	Uniform uniform;
	uniform.orientation = mat4_cast(transform.getOrientation());
	uniform.world = transform.getTransform();
	uniform.worldViewProjection = viewProjection * uniform.world;
	uniformBuffer->set(&uniform);
}
