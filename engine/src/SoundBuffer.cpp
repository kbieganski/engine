#include <vector>
#include <fstream>
#include <vorbis/vorbisfile.h>
#include "SoundBuffer.hpp"
#include "Logger.hpp"
#include <AL/al.h>


#define READ_BUFFER_SIZE 4096


using std::move;


struct SoundFile {
	SoundFile(const string &filename);

	size_t offset = 0;
	std::vector<int8_t> contents;
};


SoundFile::SoundFile(const string &filename){
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		ERROR("Failed to open file ", filename);
	}
	auto soundFileSize = file.tellg();
	contents.resize(soundFileSize);
	file.seekg(0);
	file.read(reinterpret_cast<char*>(contents.data()), contents.size());
}


static ALenum getOpenAlFormat(const unsigned channelCount) {
	switch (channelCount) {
	case 1:  return AL_FORMAT_MONO16;
	case 2:  return AL_FORMAT_STEREO16;
	case 4:  return alGetEnumValue("AL_FORMAT_QUAD16");
	case 6:  return alGetEnumValue("AL_FORMAT_51CHN16");
	case 7:  return alGetEnumValue("AL_FORMAT_61CHN16");
	case 8:  return alGetEnumValue("AL_FORMAT_71CHN16");
	default: return 0;
	}
}


static size_t oggVorbisRead(void* dataPtr, size_t byteSize, size_t sizeToRead, void* filePtr) {
	SoundFile &file =* reinterpret_cast<SoundFile*>(filePtr);
	size_t remainingSize = file.contents.size() - file.offset;
	size_t actualLength = byteSize * sizeToRead;
	if(actualLength > remainingSize) {
		actualLength = remainingSize;
	}
	memcpy(dataPtr, file.contents.data() + file.offset, actualLength);
	file.offset += actualLength;
	return actualLength;
}


static int oggVorbisSeek(void* filePtr, ogg_int64_t offset, int origin) {
	SoundFile &file = *reinterpret_cast<SoundFile*>(filePtr);
	switch(origin) {
	case SEEK_SET:
		file.offset = offset;
		break;
	case SEEK_CUR:
		file.offset += offset;
		break;
	case SEEK_END:
		file.offset = file.contents.size() - 1 - offset;
		break;
	default:
		return -1;
	}
	return 0;
}


static int oggVorbisClose(void*) {
	return 0;
}


static long oggVorbisTell(void* file) {
	return reinterpret_cast<SoundFile*>(file)->offset;
}


static ov_callbacks oggVorbisCallbacks = { oggVorbisRead, oggVorbisSeek, oggVorbisClose, oggVorbisTell };


SoundBuffer::SoundBuffer(const string &filename) {
	SoundFile soundFile(filename);
	OggVorbis_File ovFile;
	memset(&ovFile, 0, sizeof(OggVorbis_File));
	if(ov_open_callbacks(&soundFile, &ovFile, 0, 0, oggVorbisCallbacks) != 0) {
		ERROR("Failed to load sound file ", filename);
	}
	vorbis_info* info = ov_info(&ovFile, -1);
	int bitStream = 0;
	int bytes = 0;
	std::vector<char> samples;
	do {
		char array[READ_BUFFER_SIZE];
		bytes = ov_read(&ovFile, array, READ_BUFFER_SIZE, 0, 2, 1, &bitStream);
		samples.insert(samples.end(), array, array + bytes);
	} while(bytes > 0);
	alGenBuffers(1, &handle);
	alBufferData(handle, getOpenAlFormat(info->channels), samples.data(), samples.size(), info->rate);
	ov_clear(&ovFile);
	INFO("Loaded sound file ", filename);
}


SoundBuffer::SoundBuffer(SoundBuffer&& moved) {
	*this = move(moved);
}


SoundBuffer::~SoundBuffer() {
	if (handle) {
		alDeleteBuffers(1, &handle);
	}
}


SoundBuffer& SoundBuffer::operator=(SoundBuffer&& moved) {
	handle = moved.handle;
	moved.handle = 0;
	return *this;
}


float SoundBuffer::getDuration() const {
	ALint sizeInBytes, channels, bits, frequency;
	alGetBufferi(handle, AL_SIZE, &sizeInBytes);
	alGetBufferi(handle, AL_CHANNELS, &channels);
	alGetBufferi(handle, AL_BITS, &bits);
	alGetBufferi(handle, AL_FREQUENCY, &frequency);
	return static_cast<float>(sizeInBytes*  8 / (channels*  bits)) / frequency;
}


unsigned SoundBuffer::getHandle() const {
	return handle;
}
