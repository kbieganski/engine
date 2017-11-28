#pragma once
#include <bullet/btBulletDynamicsCommon.h>


class PhysicsSystem {
public:
	PhysicsSystem();

	void update(float dt);

	btDiscreteDynamicsWorld& getDynamicsWorld();
	const btDiscreteDynamicsWorld& getDynamicsWorld() const;


private:
	btDbvtBroadphase broadphase;
	btDefaultCollisionConfiguration collisionConfiguration;
	btCollisionDispatcher dispatcher;
	btSequentialImpulseConstraintSolver solver;
	btDiscreteDynamicsWorld dynamicsWorld;
};
