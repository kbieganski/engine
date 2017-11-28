#include "ContactSound.hpp"


ContactSound::ContactSound(RigidBody& _rigidBody, SoundSource& _soundSource)
	:	rigidBody(_rigidBody),
		soundSource(_soundSource) {
	soundSource.setLooped(true);
}


void ContactSound::update() {
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


void ContactSound::setMaximumVolumeSpeed(float speed) {
	maxVolumeSpeed = speed;
}


float ContactSound::getMaximumVolumeSpeed() const {
	return maxVolumeSpeed;
}
