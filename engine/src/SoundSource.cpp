#include "SoundSource.hpp"
#include <AL/al.h>


SoundSource::SoundSource(Transform& _transform, shared_ptr<const SoundBuffer> soundBuffer)
	:	transform(_transform) {
	this->soundBuffer = soundBuffer;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, soundBuffer->getHandle());
}


SoundSource::SoundSource(SoundSource&& moved)
	:	transform(moved.transform) {
	soundBuffer = move(moved.soundBuffer);
	source = moved.source;
	moved.source = 0;
}


SoundSource::~SoundSource() {
	stop();
	if (source) {
		alDeleteSources(1, &source);
	}
}


void SoundSource::update() {
	auto position = transform.getPosition();
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
}


void SoundSource::play() const {
	alSourcePlay(source);
}


void SoundSource::pause() const {
	alSourcePause(source);
}


void SoundSource::stop() const {
	alSourceStop(source);
}


void SoundSource::setAttenuation(float attenuation) {
	alSourcef(source, AL_ROLLOFF_FACTOR, attenuation);
}


void SoundSource::setLooped(bool looped) {
	alSourcei(source, AL_LOOPING, looped);
}


void SoundSource::setMinimalDistance(float distance) {
	alSourcef(source, AL_REFERENCE_DISTANCE, distance);
}


void SoundSource::setPitch(float pitch) {
	alSourcef(source, AL_PITCH, pitch);
}


void SoundSource::setRelative(bool relative) {
	alSourcei(source, AL_SOURCE_RELATIVE, relative);
}


void SoundSource::setVolume(float volume) {
	alSourcef(source, AL_GAIN, volume);
}


float SoundSource::getAttenuation() const {
	ALfloat rolloff;
	alGetSourcef(source, AL_ROLLOFF_FACTOR, &rolloff);
	return rolloff;
}


float SoundSource::getMinimalDistance() const {
	ALfloat distance;
	alGetSourcef(source, AL_REFERENCE_DISTANCE, &distance);
	return distance;
}


float SoundSource::getPitch() const {
	ALfloat pitch;
	alGetSourcef(source, AL_PITCH, &pitch);
	return pitch;
}


float SoundSource::getVolume() const {
	ALfloat volume;
	alGetSourcef(source, AL_GAIN, &volume);
	return volume;
}


bool SoundSource::isPaused() const {
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PAUSED;
}


bool SoundSource::isPlaying() const {
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}


bool SoundSource::isStopped() const {
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_STOPPED || state == AL_INITIAL;
}


bool SoundSource::isLooped() const {
	ALint looped;
	alGetSourcei(source, AL_LOOPING, &looped);
	return looped != 0;
}


bool SoundSource::isRelative() const {
	ALint relative;
	alGetSourcei(source, AL_SOURCE_RELATIVE, &relative);
	return relative != 0;
}
