#pragma once
#include <memory>
#include "SoundBuffer.hpp"
#include "TransformComponent.hpp"


using std::shared_ptr;


class SoundSourceComponent {
public:
	SoundSourceComponent(TransformComponent& transform, shared_ptr<const SoundBuffer> soundBuffer);
	SoundSourceComponent(const SoundSourceComponent&) = delete;
	SoundSourceComponent(SoundSourceComponent&& moved);
	~SoundSourceComponent();

	SoundSourceComponent& operator=(const SoundSourceComponent&) = delete;
	SoundSourceComponent& operator=(SoundSourceComponent&&) = delete;

	void update();
	void play() const;
	void pause() const;
	void stop() const;

	void setAttenuation(float attenuation);
	void setLooped(bool looped);
	void setMinimalDistance(float distance);
	void setPitch(float pitch);
	void setRelative(bool relative);
	void setVolume(float volume);

	float getAttenuation() const;
	float getMinimalDistance() const;
	float getPitch() const;
	float getVolume() const;

	bool isPaused() const;
	bool isPlaying() const;
	bool isStopped() const;
	bool isLooped() const;
	bool isRelative() const;

private:
	shared_ptr<const SoundBuffer> soundBuffer;
	unsigned source;
	TransformComponent& transform;
};
