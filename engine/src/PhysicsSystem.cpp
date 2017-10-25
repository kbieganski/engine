#include "PhysicsSystem.hpp"


PhysicsSystem::PhysicsSystem(ComponentSystem<TransformComponent>& _transforms)
	:	dispatcher(&collisionConfiguration),
		dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration),
		transforms(_transforms) {
	dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
}


void PhysicsSystem::addRigidBody(EntityId entity) {
	rigidBodies.add(entity, transforms[entity], dynamicsWorld);
}


void PhysicsSystem::update(float dt) {
	dynamicsWorld.stepSimulation(dt, 1, 0.016);
}


RigidBodyComponent& PhysicsSystem::getRigidBody(EntityId entity) {
	return rigidBodies[entity];
}


const RigidBodyComponent& PhysicsSystem::getRigidBody(EntityId entity) const {
	return rigidBodies[entity];
}
