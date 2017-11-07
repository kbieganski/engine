#include "ContactSoundComponent.hpp"


ContactSoundComponent::ContactSoundComponent(RigidBodyComponent& _rigidBody, SoundSourceComponent& _soundSource)
	:	rigidBody(_rigidBody),
		soundSource(_soundSource) {
	soundSource.setLooped(true);
}


void ContactSoundComponent::update() {
	auto speed = length(rigidBody.getLinearVelocity());
	auto volume = speed / maxVolumeSpeed;
	if (volume > 1) {
		volume = 1;
	}
	if (rigidBody.isOnGround() && volume > 0) {
		if (!soundSource.isPlaying()) {
			soundSource.play();
		}
		soundSource.setVolume(volume);
	} else if (soundSource.isPlaying()) {
		soundSource.stop();
	}
}


void ContactSoundComponent::setMaximumVolumeSpeed(float speed) {
	maxVolumeSpeed = speed;
}


float ContactSoundComponent::getMaximumVolumeSpeed() const {
	return maxVolumeSpeed;
}
