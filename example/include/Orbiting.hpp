#pragma once
#include <Scene.hpp>
#include <Transform.hpp>


class Orbiting : public DependentOn<Transform> {
public:
	Orbiting(Transform& transform);

	void update(float dt);

	void setCounterClockwise(bool cc);
	void setCycleDuration(float cycleDuration);
	void setOrbitRadius(float radius);
	void setPivot(vec3 pivot);

	float getCycleDuration() const;
	float getOrbitRadius() const;
	vec3 getPivot() const;
	bool isCounterClockwise() const;


private:
	bool counterClockwise = false;
	float elapsedTime = 0;
	float cycleDuration = 1;
	float orbitRadius = 1;
	vec3 pivot = vec3(0);
	Transform& transform;
};
