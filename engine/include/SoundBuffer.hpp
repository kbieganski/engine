#pragma once
#include <string>


using std::string;


class SoundBuffer {
public:
	SoundBuffer(const string& filename);
	SoundBuffer(const SoundBuffer&) = delete;
	SoundBuffer(SoundBuffer&& moved);
	~SoundBuffer();

	SoundBuffer& operator=(const SoundBuffer&) = delete;
	SoundBuffer& operator=(SoundBuffer&& moved);

	float getDuration() const;
	unsigned getHandle() const;


private:
	unsigned handle;
};
