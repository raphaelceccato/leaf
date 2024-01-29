#ifndef __LEAF_SOUNDCHANNEL__
#define __LEAF_SOUNDCHANNEL__

#include <memory>
#include <exception>
#include <string>
#include <AL/al.h>
#include "sound.h"


namespace leaf {
	class SoundChannel;
	typedef std::shared_ptr<SoundChannel> SoundChannelPtr;

	class SoundChannel {
	public:
		SoundChannel(unsigned int id) : alSource(0) {
			this->id = id;
			alGenSources(1, (ALuint*)&alSource);
			if (!alSource) {
				throw std::exception(("failed to create sound source: " + std::string(alGetString(alGetError()))).c_str());
			}
		}


		~SoundChannel() {
			if (alSource)
				alDeleteSources(1, (ALuint*)&alSource);
		}


		void play(SoundPtr sound) {
			alSourcei(alSource, AL_BUFFER, sound->getALBuffer());
			alSourcePlay(alSource);
		}


		float getVolume() const {
			float value;
			alGetSourcef(alSource, AL_GAIN, &value);
			return value;
		}


		float getPitch() const {
			float value;
			alGetSourcef(alSource, AL_PITCH, &value);
			return value;
		}


		bool isLooping() const {
			int value;
			alGetSourcei(alSource, AL_LOOPING, &value);
			return (bool)value;
		}


		void pause() { alSourcePause(alSource); }
		void resume() { alSourcePlay(alSource); }
		void stop() { alSourceStop(alSource); }
		void setVolume(float value) { alSourcef(alSource, AL_GAIN, value); }
		void setPitch(float value) { alSourcef(alSource, AL_PITCH, value); }
		void setLooping(bool loop) { alSourcei(alSource, AL_LOOPING, loop); }


	private:
		unsigned int id;
		unsigned int alSource;
	};
}

#endif
