#pragma once
#include <memory>
#include "RigidBody.hpp"
#include "Scene.hpp"
#include "SoundBuffer.hpp"
#include "SoundSource.hpp"


class ContactSound : public DependentOn<RigidBody, SoundSource> {
public:
	ContactSound(RigidBody& rigidBody, SoundSource& soundSource);

	void update();

	void setMaximumVolumeSpeed(float speed);

	float getMaximumVolumeSpeed() const;


private:
	float maxVolumeSpeed = 10;
	RigidBody& rigidBody;
	SoundSource& soundSource;
};
