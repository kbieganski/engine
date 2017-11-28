#include "GraphicsSystem.hpp"
#include "ModelRenderComponent.hpp"


using std::make_shared;
using glm::mat4_cast;


ModelRenderComponent::ModelRenderComponent(const TransformComponent& _transform, GraphicsSystem& graphicsSystem, shared_ptr<const Model> model)
	:	transform(_transform) {
	this->model = model;
	uniformBuffer = make_shared<UniformBuffer>(graphicsSystem.getContext(), sizeof(Uniform));
	graphicsSystem.add(*this);
}


void ModelRenderComponent::addTo(SceneRenderer& sceneRenderer) const {
	auto& renderDescription = sceneRenderer.addRender();
	renderDescription.bindUniform(0, uniformBuffer);
	model->describe(renderDescription, 1);
}


void ModelRenderComponent::addTo(ShadowMapRenderer& shadowMapRenderer) const {
	auto& renderDescription = shadowMapRenderer.addRender();
	renderDescription.bindUniform(0, uniformBuffer);
	model->describeAttributes(renderDescription);
}


void ModelRenderComponent::update(const mat4& viewProjection) {
	Uniform uniform;
	uniform.orientation = mat4_cast(transform.getOrientation());
	uniform.world = transform.getTransform();
	uniform.worldViewProjection = viewProjection * uniform.world;
	uniformBuffer->set(&uniform);
}
