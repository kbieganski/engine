#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include "Mesh.hpp"
#include "Scene.hpp"
#include "Transform.hpp"


class PhysicsSystem;


class RigidBody : public DependentOn<Transform>, public btMotionState {
public:
	RigidBody(Transform& transform, PhysicsSystem& physicsSystem);
	RigidBody(const RigidBody&) = delete;
	RigidBody(RigidBody&& moved);
	~RigidBody();

	RigidBody& operator=(const RigidBody&) = delete;
	RigidBody& operator=(RigidBody&& moved) = delete;

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

	bool isOnGround() const;


private:
	float mass = 0;
	Transform& transform;
	btDiscreteDynamicsWorld& dynamicsWorld;
	unique_ptr<btRigidBody> body;
	shared_ptr<const btCollisionShape> collisionShape;
};
