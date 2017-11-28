#include "PhysicsSystem.hpp"


PhysicsSystem::PhysicsSystem()
	:	dispatcher(&collisionConfiguration),
		dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration) {
	dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
}


void PhysicsSystem::update(float dt) {
	dynamicsWorld.stepSimulation(dt, 1, 0.016);
}


btDiscreteDynamicsWorld& PhysicsSystem::getDynamicsWorld() {
	return dynamicsWorld;
}


const btDiscreteDynamicsWorld& PhysicsSystem::getDynamicsWorld() const {
	return dynamicsWorld;
}
