#pragma once
#include <memory>
#include "export.h"
#include "sound.h"


namespace leaf {
	class EXPORT SoundChannel {
	public:
		SoundChannel(unsigned int id);
		~SoundChannel();
		void play(SoundPtr sound);
		void pause();
		void resume();
		void stop();
		float getVolume() const;
		void setVolume(float value);
		float getPitch() const;
		void setPitch(float value);
		bool isLooping() const;
		void setLooping(bool loop);
	private:
		unsigned int id;
		unsigned int alSource;
	};


	typedef std::shared_ptr<SoundChannel> SoundChannelPtr;
}