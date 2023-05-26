#pragma once
#include "export.h"
#include "window.h"
#include "soundchannel.h"
#include <vector>

#define NUM_SOUND_CHANNELS 10

struct ALCdevice;
struct ALCcontext;

namespace leaf {
	class EXPORT Engine {
	public:
		Engine();
		~Engine();
		WindowPtr createWindow(const char* title, int width, int height);
		WindowPtr getWindow() const { return window; }
		static ShaderPtr getDefaultShader() { return defaultShader; }
		static int getGlobalVBO() { return globalVBO; }
		static int getGlobalVAO() { return globalVAO; }
		unsigned int getTicks() const;
		void playSound(SoundPtr sound, unsigned int channel);
	private:
		WindowPtr window;
		void* glContext;
		SoundChannelPtr soundChannels[NUM_SOUND_CHANNELS];
		static bool initialized;
		static ShaderPtr defaultShader;
		static int globalVBO;
		static int globalVAO;
		static ALCdevice* alDevice;
		ALCcontext* alContext;
	};
}