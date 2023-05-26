#include "../include/leaf/soundchannel.h"
#include <AL/al.h>
#include <exception>
#include <string>


namespace leaf {
	SoundChannel::SoundChannel(unsigned int id) : alSource(0) {
		this->id = id;
		alGenSources(1, (ALuint*)&alSource);
		if (!alSource) {
			throw std::exception(("failed to create sound source: " + std::string(alGetString(alGetError()))).c_str());
		}
	}


	SoundChannel::~SoundChannel() {
		if (alSource)
			alDeleteSources(1, (ALuint*)&alSource);
	}


	void SoundChannel::play(SoundPtr sound) {
		alSourcei(alSource, AL_BUFFER, sound->getALBuffer());
		alSourcePlay(alSource);
	}


	void SoundChannel::pause() {
		alSourcePause(alSource);
	}


	void SoundChannel::resume() {
		alSourcePlay(alSource);
	}


	void SoundChannel::stop() {
		alSourceStop(alSource);
	}


	float SoundChannel::getVolume() const {
		float value;
		alGetSourcef(alSource, AL_GAIN, &value);
		return value;
	}


	void SoundChannel::setVolume(float value) {
		alSourcef(alSource, AL_GAIN, value);
	}


	float SoundChannel::getPitch() const {
		float value;
		alGetSourcef(alSource, AL_PITCH, &value);
		return value;
	}


	void SoundChannel::setPitch(float value) {
		alSourcef(alSource, AL_PITCH, value);
	}


	bool SoundChannel::isLooping() const {
		int value;
		alGetSourcei(alSource, AL_LOOPING, &value);
		return (bool)value;
	}


	void SoundChannel::setLooping(bool loop) {
		alSourcei(alSource, AL_LOOPING, loop);
	}
}