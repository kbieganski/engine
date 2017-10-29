#include "SoundSourceComponent.hpp"
#include <AL/al.h>


SoundSourceComponent::SoundSourceComponent(TransformComponent& _transform, shared_ptr<const SoundBuffer> soundBuffer)
	:	transform(_transform) {
	this->soundBuffer = soundBuffer;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, soundBuffer->getHandle());
}


SoundSourceComponent::SoundSourceComponent(SoundSourceComponent&& moved)
	:	transform(moved.transform) {
	soundBuffer = move(moved.soundBuffer);
	source = moved.source;
	moved.source = 0;
}


SoundSourceComponent::~SoundSourceComponent() {
	stop();
	if (source) {
		alDeleteSources(1, &source);
	}
}


void SoundSourceComponent::update() {
	auto position = transform.getPosition();
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
}


void SoundSourceComponent::play() const {
	alSourcePlay(source);
}


void SoundSourceComponent::pause() const {
	alSourcePause(source);
}


void SoundSourceComponent::stop() const {
	alSourceStop(source);
}


void SoundSourceComponent::setAttenuation(float attenuation) {
	alSourcef(source, AL_ROLLOFF_FACTOR, attenuation);
}


void SoundSourceComponent::setLooped(bool looped) {
	alSourcei(source, AL_LOOPING, looped);
}


void SoundSourceComponent::setMinimalDistance(float distance) {
	alSourcef(source, AL_REFERENCE_DISTANCE, distance);
}


void SoundSourceComponent::setPitch(float pitch) {
	alSourcef(source, AL_PITCH, pitch);
}


void SoundSourceComponent::setRelative(bool relative) {
	alSourcei(source, AL_SOURCE_RELATIVE, relative);
}


void SoundSourceComponent::setVolume(float volume) {
	alSourcef(source, AL_GAIN, volume);
}


float SoundSourceComponent::getAttenuation() const {
	ALfloat rolloff;
	alGetSourcef(source, AL_ROLLOFF_FACTOR, &rolloff);
	return rolloff;
}


float SoundSourceComponent::getMinimalDistance() const {
	ALfloat distance;
	alGetSourcef(source, AL_REFERENCE_DISTANCE, &distance);
	return distance;
}


float SoundSourceComponent::getPitch() const {
	ALfloat pitch;
	alGetSourcef(source, AL_PITCH, &pitch);
	return pitch;
}


float SoundSourceComponent::getVolume() const {
	ALfloat volume;
	alGetSourcef(source, AL_GAIN, &volume);
	return volume;
}


bool SoundSourceComponent::isPaused() const {
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PAUSED;
}


bool SoundSourceComponent::isPlaying() const {
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}


bool SoundSourceComponent::isStopped() const {
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_STOPPED || state == AL_INITIAL;
}


bool SoundSourceComponent::isLooped() const {
	ALint looped;
	alGetSourcei(source, AL_LOOPING, &looped);
	return looped != 0;
}


bool SoundSourceComponent::isRelative() const {
	ALint relative;
	alGetSourcei(source, AL_SOURCE_RELATIVE, &relative);
	return relative != 0;
}
