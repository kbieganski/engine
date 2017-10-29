#include <AL/al.h>
#include "Logger.hpp"
#include "SoundSystem.hpp"


using std::move;


SoundSystem::SoundSystem(ComponentSystem<TransformComponent>& _transforms, ComponentSystem<CameraComponent>& _cameras)
	:	transforms(_transforms),
		cameras(_cameras) {
	currentCamera = nullptr;
	device = alcOpenDevice(nullptr);
	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);
}


SoundSystem::SoundSystem(SoundSystem&& moved)
	:	transforms(moved.transforms),
		cameras(moved.cameras) {
	device = moved.device;
	context = moved.context;
	soundSources = move(moved.soundSources);
	currentCamera = moved.currentCamera;
	currentCameraId = moved.currentCameraId;
	moved.context = nullptr;
	moved.device = nullptr;
}


SoundSystem::~SoundSystem() {
	alcMakeContextCurrent(nullptr);
	if(context) {
		alcDestroyContext(context);
	}
	if(device) {
		alcCloseDevice(device);
	}
}


void SoundSystem::addSoundSource(EntityId entity, shared_ptr<const SoundBuffer> soundBuffer) {
	soundSources.add(entity, transforms[entity], soundBuffer);
}


void SoundSystem::update() {
	soundSources.update();
	if (currentCamera) {
		auto direction = currentCamera->getDirection();
		auto forward = normalize(vec3(direction.x, 0, direction.z));
		auto right = cross(forward, vec3(0, 1, 0));
		auto up = cross(right, direction);
		float orientation[] = { direction.x, direction.y, direction.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, orientation);
		auto position = transforms[currentCameraId].getPosition();
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		alListener3f(AL_VELOCITY, 0, 0, 0);
	}
}


void SoundSystem::setVolume(float volume) {
	alListenerf(AL_GAIN, volume);
}


void SoundSystem::setCurrentCamera(EntityId entity) {
	currentCameraId = entity;
	currentCamera = &cameras[entity];
}


SoundSourceComponent& SoundSystem::getSoundSource(EntityId entity) {
	return soundSources[entity];
}


float SoundSystem::getVolume() const {
	ALfloat volume;
	alGetListenerf(AL_GAIN, &volume);
	return volume;
}


CameraComponent& SoundSystem::getCurrentCamera() {
	return *currentCamera;
}


const SoundSourceComponent& SoundSystem::getSoundSource(EntityId entity) const {
	return soundSources[entity];
}


const CameraComponent& SoundSystem::getCurrentCamera() const {
	return *currentCamera;
}
