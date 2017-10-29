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

	void setAngularFactor(vec3 factor);
	void setAngularFactor(float factor);
	void setLinearFactor(vec3 factor);
	void setAngularVelocity(vec3 velocity);
	void setLinearVelocity(vec3 velocity);
	void setFriction(float friction);
	void setMass(float mass);
	void setShape(shared_ptr<const btCollisionShape> collisionShape);
	void setShape(shared_ptr<const Mesh> collisionMesh);
	virtual void setWorldTransform(const btTransform& physicsTransform);

	btRigidBody& getBody();
	vec3 getAngularFactor() const;
	vec3 getLinearFactor() const;
	vec3 getAngularVelocity() const;
	vec3 getLinearVelocity() const;
	float getFriction() const;
	float getMass() const;
	virtual void getWorldTransform(btTransform& physicsTransform) const;


private:
	float mass = 0;
	btDiscreteDynamicsWorld& dynamicsWorld;
	unique_ptr<btRigidBody> body;
	shared_ptr<const btCollisionShape> collisionShape;
	TransformComponent& transform;
};
