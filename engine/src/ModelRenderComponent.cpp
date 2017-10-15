#include <glm/gtx/quaternion.hpp>
#include "ModelRenderComponent.hpp"


using std::make_shared;
using glm::toMat4;


ModelRenderComponent::ModelRenderComponent(shared_ptr<const GraphicsContext> context, const TransformComponent& _transform, shared_ptr<const Model> model)
	:	transform(_transform) {
	this->model = model;
	uniformBuffer = make_shared<UniformBuffer>(context, sizeof(Uniform));
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
	uniform.orientation = toMat4(transform.getOrientation());
	uniform.world = transform.getTransform();
	uniform.worldViewProjection = viewProjection * uniform.world;
	uniformBuffer->set(&uniform);
}
