#include <AL/al.h>
#include "Logger.hpp"
#include "SoundSource.hpp"
#include "SoundSystem.hpp"


using std::move;


SoundSystem::SoundSystem(Scene& _scene)
	:	scene(_scene) {
	currentCamera = nullptr;
	device = alcOpenDevice(nullptr);
	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);
}


SoundSystem::SoundSystem(SoundSystem&& moved)
	:	scene(moved.scene) {
	device = moved.device;
	context = moved.context;
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


void SoundSystem::update() {
	scene.update<SoundSource>();
	if (currentCamera) {
		auto direction = currentCamera->getDirection();
		auto forward = normalize(vec3(direction.x, 0, direction.z));
		auto right = cross(forward, vec3(0, 1, 0));
		auto up = cross(right, direction);
		float orientation[] = { direction.x, direction.y, direction.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, orientation);
		auto position = scene.get<Transform>(currentCameraId).getPosition();
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		alListener3f(AL_VELOCITY, 0, 0, 0);
	}
}


void SoundSystem::setVolume(float volume) {
	alListenerf(AL_GAIN, volume);
}


void SoundSystem::setCurrentCamera(EntityId entity) {
	currentCameraId = entity;
	currentCamera = &scene.get<Camera>(entity);
}


float SoundSystem::getVolume() const {
	ALfloat volume;
	alGetListenerf(AL_GAIN, &volume);
	return volume;
}


Camera& SoundSystem::getCurrentCamera() {
	return *currentCamera;
}


const Camera& SoundSystem::getCurrentCamera() const {
	return *currentCamera;
}
