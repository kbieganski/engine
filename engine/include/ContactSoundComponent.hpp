#pragma once
#include <memory>
#include "RigidBodyComponent.hpp"
#include "Scene.hpp"
#include "SoundBuffer.hpp"
#include "SoundSourceComponent.hpp"


class ContactSoundComponent : public DependentOn<RigidBodyComponent, SoundSourceComponent> {
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
