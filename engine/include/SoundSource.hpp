#pragma once
#include <memory>
#include "Scene.hpp"
#include "SoundBuffer.hpp"
#include "Transform.hpp"


using std::shared_ptr;


class SoundSource : public DependentOn<Transform> {
public:
	SoundSource(Transform& transform, shared_ptr<const SoundBuffer> soundBuffer);
	SoundSource(const SoundSource&) = delete;
	SoundSource(SoundSource&& moved);
	~SoundSource();

	SoundSource& operator=(const SoundSource&) = delete;
	SoundSource& operator=(SoundSource&&) = delete;

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
	Transform& transform;
};
