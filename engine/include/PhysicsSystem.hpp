#pragma once
#include "ComponentSystem.hpp"
#include "RigidBodyComponent.hpp"


class PhysicsSystem {
public:
	PhysicsSystem(ComponentSystem<TransformComponent>& transforms);

	void addRigidBody(EntityId entity);

	void update(float dt);

	RigidBodyComponent& getRigidBody(EntityId entity);
	const RigidBodyComponent& getRigidBody(EntityId entity) const;

private:
	btDbvtBroadphase broadphase;
	btDefaultCollisionConfiguration collisionConfiguration;
	btCollisionDispatcher dispatcher;
	btSequentialImpulseConstraintSolver solver;
	btDiscreteDynamicsWorld dynamicsWorld;
	ComponentSystem<TransformComponent>& transforms;
	ComponentSystem<RigidBodyComponent> rigidBodies;
};
