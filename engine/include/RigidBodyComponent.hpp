#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include "Mesh.hpp"
#include "TransformComponent.hpp"


class RigidBodyComponent : public btMotionState {
public:
	RigidBodyComponent(TransformComponent& transform, btDiscreteDynamicsWorld& dynamicsWorld);
	RigidBodyComponent(const RigidBodyComponent&) = delete;
	RigidBodyComponent(RigidBodyComponent&& moved);
	~RigidBodyComponent();

	RigidBodyComponent& operator=(const RigidBodyComponent&) = delete;
	RigidBodyComponent& operator=(RigidBodyComponent&& moved) = delete;

	void applyForce(vec3 force);

	void setMass(float mass);
	void setShape(shared_ptr<const btCollisionShape> collisionShape);
	void setShape(shared_ptr<const Mesh> collisionMesh);
	virtual void setWorldTransform(const btTransform& physicsTransform);

	btRigidBody& getBody();
	float getMass();
	virtual void getWorldTransform(btTransform& physicsTransform) const;


private:
	float mass = 0;
	btDiscreteDynamicsWorld& dynamicsWorld;
	unique_ptr<btRigidBody> body;
	shared_ptr<const btCollisionShape> collisionShape;
	TransformComponent& transform;
};
