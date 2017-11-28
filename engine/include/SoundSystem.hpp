#include "Camera.hpp"
#include <AL/alc.h>


class SoundSystem {
public:
	SoundSystem(Scene& scene);
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&& moved);
	~SoundSystem();

	SoundSystem& operator=(const SoundSystem&) = delete;
	SoundSystem& operator=(SoundSystem&&) = delete;

	void update();

	void setVolume(float volume);
	void setCurrentCamera(EntityId entity);

	float getVolume() const;
	Camera& getCurrentCamera();
	const Camera& getCurrentCamera() const;


private:
	ALCdevice* device;
	ALCcontext* context;
	Scene& scene;
	Camera* currentCamera = nullptr;
	EntityId currentCameraId;
};
