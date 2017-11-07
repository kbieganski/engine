#pragma once
#include <memory>
#include "SoundBuffer.hpp"
#include "RigidBodyComponent.hpp"
#include "SoundSourceComponent.hpp"


class ContactSoundComponent {
public:
	ContactSoundComponent(RigidBodyComponent& rigidBody, SoundSourceComponent& soundSource);

	void update();

	void setMaximumVolumeSpeed(float speed);

	float getMaximumVolumeSpeed() const;


private:
	float maxVolumeSpeed = 10;
	RigidBodyComponent& rigidBody;
	SoundSourceComponent& soundSource;
};
