#include "RigidBodyComponent.hpp"


using std::make_shared;
using std::make_unique;
using std::move;


RigidBodyComponent::RigidBodyComponent(TransformComponent& _transform, btDiscreteDynamicsWorld& _dynamicsWorld)
	:	dynamicsWorld(_dynamicsWorld),
		transform(_transform) {
	collisionShape = make_shared<btEmptyShape>();
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, this, const_cast<btCollisionShape*>(collisionShape.get()));
	body = make_unique<btRigidBody>(bodyInfo);
	dynamicsWorld.addRigidBody(body.get());
}


RigidBodyComponent::RigidBodyComponent(RigidBodyComponent&& moved)
	:	dynamicsWorld(moved.dynamicsWorld),
		transform(moved.transform) {
	mass = moved.mass;
	collisionShape = move(moved.collisionShape);
	body = move(moved.body);
	body->setMotionState(this);
}


RigidBodyComponent::~RigidBodyComponent() {
	if (body) {
		dynamicsWorld.removeRigidBody(body.get());
	}
}


void RigidBodyComponent::applyForce(vec3 force) {
	body->applyForce(btVector3(force.x, force.y, force.z), btVector3(0, 0, 0));
	body->activate();
}


void RigidBodyComponent::setWorldTransform(const btTransform& physicsTransform) {
	auto orientation = physicsTransform.getRotation();
	transform.setLocalOrientation(quat(orientation.w(), orientation.x(), orientation.y(), orientation.z()));
	auto position = physicsTransform.getOrigin();
	transform.setLocalPosition(vec3(position.x(), position.y(), position.z()));
}


void RigidBodyComponent::getWorldTransform(btTransform& physicsTransform) const {
	physicsTransform.setIdentity();
	auto orientation = transform.getOrientation();
	physicsTransform.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	auto position = transform.getPosition();
	physicsTransform.setOrigin(btVector3(position.x, position.y, position.z));
}


void RigidBodyComponent::setMass(float newMass) {
	mass = newMass;
	dynamicsWorld.removeRigidBody(body.get());
	btVector3 inertia;
	body->getCollisionShape()->calculateLocalInertia(mass, inertia);
	body->setMassProps(mass, inertia);
	dynamicsWorld.addRigidBody(body.get());
	body->activate();
}


void RigidBodyComponent::setShape(shared_ptr<const btCollisionShape> collisionShape) {
	dynamicsWorld.removeRigidBody(body.get());
	this->collisionShape = collisionShape;
	btVector3 inertia;
	collisionShape->calculateLocalInertia(mass, inertia);
	body->setCollisionShape(const_cast<btCollisionShape*>(collisionShape.get()));
	body->setMassProps(mass, inertia);
	dynamicsWorld.addRigidBody(body.get());
	body->activate();
}


void RigidBodyComponent::setShape(shared_ptr<const Mesh> collisionMesh) {
	setShape(shared_ptr<const btCollisionShape>(collisionMesh, &collisionMesh->getConvexHull()));
}


btRigidBody& RigidBodyComponent::getBody() {
	return *body;
}


float RigidBodyComponent::getMass() {
	return mass;
}
