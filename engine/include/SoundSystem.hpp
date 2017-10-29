#include "CameraComponent.hpp"
#include "ComponentSystem.hpp"
#include "SoundSourceComponent.hpp"
#include <AL/alc.h>


class SoundSystem {
public:
	SoundSystem(ComponentSystem<TransformComponent>& transforms, ComponentSystem<CameraComponent>& cameras);
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&& moved);
	~SoundSystem();

	SoundSystem& operator=(const SoundSystem&) = delete;
	SoundSystem& operator=(SoundSystem&&) = delete;

	void addSoundSource(EntityId entity, shared_ptr<const SoundBuffer> soundBuffer);

	void update();

	void setVolume(float volume);
	void setCurrentCamera(EntityId entity);

	float getVolume() const;
	SoundSourceComponent& getSoundSource(EntityId entity);
	CameraComponent& getCurrentCamera();
	const SoundSourceComponent& getSoundSource(EntityId entity) const;
	const CameraComponent& getCurrentCamera() const;


private:
	ALCdevice* device;
	ALCcontext* context;
	ComponentSystem<TransformComponent>& transforms;
	ComponentSystem<CameraComponent>& cameras;
	ComponentSystem<SoundSourceComponent> soundSources;
	CameraComponent* currentCamera = nullptr;
	EntityId currentCameraId;
};
