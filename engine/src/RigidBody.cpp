#include "PhysicsSystem.hpp"
#include "RigidBody.hpp"


using std::make_shared;
using std::make_unique;
using std::move;


RigidBody::RigidBody(Transform& _transform, PhysicsSystem& physicsSystem)
	:	transform(_transform),
		dynamicsWorld(physicsSystem.getDynamicsWorld()) {
	collisionShape = make_shared<btEmptyShape>();
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, this, const_cast<btCollisionShape*>(collisionShape.get()));
	body = make_unique<btRigidBody>(bodyInfo);
	dynamicsWorld.addRigidBody(body.get());
}


RigidBody::RigidBody(RigidBody&& moved)
	:	transform(moved.transform),
		dynamicsWorld(moved.dynamicsWorld) {
	mass = moved.mass;
	collisionShape = move(moved.collisionShape);
	body = move(moved.body);
	body->setMotionState(this);
}


RigidBody::~RigidBody() {
	if (body) {
		dynamicsWorld.removeRigidBody(body.get());
	}
}


void RigidBody::applyForce(vec3 force) {
	body->applyForce(btVector3(force.x, force.y, force.z), btVector3(0, 0, 0));
	body->activate();
}


void RigidBody::setAngularFactor(vec3 factor) {
	body->setAngularFactor(btVector3(factor.x, factor.y, factor.z));
}


void RigidBody::setAngularFactor(float factor) {
	body->setAngularFactor(factor);
}


void RigidBody::setLinearFactor(vec3 factor) {
	body->setLinearFactor(btVector3(factor.x, factor.y, factor.z));
}


void RigidBody::setAngularVelocity(vec3 velocity) {
	body->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	body->activate();
}


void RigidBody::setLinearVelocity(vec3 velocity) {
	body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	body->activate();
}


void RigidBody::setFriction(float friction) {
	body->setFriction(friction);
}


void RigidBody::setMass(float newMass) {
	mass = newMass;
	dynamicsWorld.removeRigidBody(body.get());
	btVector3 inertia;
	body->getCollisionShape()->calculateLocalInertia(mass, inertia);
	body->setMassProps(mass, inertia);
	dynamicsWorld.addRigidBody(body.get());
	body->activate();
}


void RigidBody::setShape(shared_ptr<const btCollisionShape> collisionShape) {
	dynamicsWorld.removeRigidBody(body.get());
	this->collisionShape = collisionShape;
	btVector3 inertia;
	collisionShape->calculateLocalInertia(mass, inertia);
	body->setCollisionShape(const_cast<btCollisionShape*>(collisionShape.get()));
	body->setMassProps(mass, inertia);
	dynamicsWorld.addRigidBody(body.get());
	body->activate();
}


void RigidBody::setWorldTransform(const btTransform& physicsTransform) {
	auto orientation = physicsTransform.getRotation();
	transform.setLocalOrientation(quat(orientation.w(), orientation.x(), orientation.y(), orientation.z()));
	auto position = physicsTransform.getOrigin();
	transform.setLocalPosition(vec3(position.x(), position.y(), position.z()));
}


void RigidBody::setShape(shared_ptr<const Mesh> collisionMesh) {
	setShape(shared_ptr<const btCollisionShape>(collisionMesh, &collisionMesh->getConvexHull()));
}


btRigidBody& RigidBody::getBody() {
	return *body;
}


vec3 RigidBody::getAngularFactor() const {
	auto factor = body->getAngularFactor();
	return vec3(factor.x(), factor.y(), factor.z());
}


vec3 RigidBody::getLinearFactor() const {
	auto factor = body->getLinearFactor();
	return vec3(factor.x(), factor.y(), factor.z());
}


vec3 RigidBody::getAngularVelocity() const {
	auto velocity = body->getAngularVelocity();
	return vec3(velocity.x(), velocity.y(), velocity.z());
}


vec3 RigidBody::getLinearVelocity() const {
	auto velocity = body->getLinearVelocity();
	return vec3(velocity.x(), velocity.y(), velocity.z());
}


float RigidBody::getFriction() const {
	return body->getFriction();
}


float RigidBody::getMass() const {
	return mass;
}


void RigidBody::getWorldTransform(btTransform& physicsTransform) const {
	physicsTransform.setIdentity();
	auto orientation = transform.getOrientation();
	physicsTransform.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	auto position = transform.getPosition();
	physicsTransform.setOrigin(btVector3(position.x, position.y, position.z));
}


bool RigidBody::isOnGround() const {
	auto position = transform.getPosition();
	btVector3 aabbMin, aabbMax;
	body->getAabb(aabbMin, aabbMax);
	btVector3 from(position.x, position.y, position.z);
	btVector3 to(position.x, aabbMin.y() - 0.1f, position.z);
	btDynamicsWorld::ClosestRayResultCallback result(from, to);
	dynamicsWorld.rayTest(from, to, result);
	return result.hasHit();
}
