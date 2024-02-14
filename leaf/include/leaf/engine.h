#ifndef __LEAF_ENGINE__
#include <vector>
#include <unordered_set>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <cstdlib>
#include "window.h"
#include "soundchannel.h"


namespace leaf {
	constexpr int NUM_SOUND_CHANNELS = 10;

	constexpr void throwIfFalse(bool condition, const char* msg) {
		if (!condition)
			throw std::exception(msg);
	}

	typedef _Window<Engine> Window;

	class Engine {
	public:
		Engine() {
			if (initialized)
				throw std::exception("another instance of the engine is already initialized");
			initialized = true;

			glContext = nullptr;
			SDL_SetMainReady();
			SDL_Init(SDL_INIT_EVERYTHING);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			alutInit(NULL, NULL);
			alDevice = alcOpenDevice(NULL);
			alContext = alcCreateContext(alDevice, NULL);
			alcMakeContextCurrent(alContext);
			for (int i = 0; i < NUM_SOUND_CHANNELS; i++) {
				soundChannels[i] = new SoundChannel(i);
			}
		}


		~Engine() {
			for (Window* win : windows)
				delete win;
			windows.clear();

			for (Texture* tex : Texture::_textures)
				delete tex;
			Texture::_textures.clear();

			if (defaultShader) {
				delete defaultShader;
				defaultShader = NULL;
			}
			for (int i = 0; i < NUM_SOUND_CHANNELS; i++)
				soundChannels[i] = NULL;
			initialized = false;
			if (globalVBO)
				glDeleteBuffers(1, (GLuint*)&globalVBO);
			if (globalVAO)
				glDeleteVertexArrays(1, (GLuint*)&globalVAO);
			if (glContext)
				SDL_GL_DeleteContext(glContext);
			SDL_Quit();
			for (int i = 0; i < NUM_SOUND_CHANNELS; i++) {
				if (soundChannels[i]) {
					soundChannels[i]->stop();
					delete soundChannels[i];
				}
			}
			if (alContext) {
				alcMakeContextCurrent(NULL);
				alcDestroyContext(alContext);
			}
			if (alDevice)
				alcCloseDevice(alDevice);
			alutExit();
		}


		Window* createWindow(const char* title, int width, int height, bool resizable) {
			Window* win = new Window(this, title, width, height, resizable);
			windows.insert(win);

			glContext = SDL_GL_CreateContext(win->getSDLWindow());
			if (!glContext)
				throw std::exception(("error creating context: " + std::string(SDL_GetError()) + ")").c_str());

			throwIfFalse(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "Could not initialize glad");

			//these needs to be after loading glad
			win->init();
			defaultShader = new Shader(vertCode, fragCode);
			glGenBuffers(1, (GLuint*)&globalVBO);
			glGenVertexArrays(1, (GLuint*)&globalVAO);

			glBindBuffer(GL_ARRAY_BUFFER, globalVBO);
			glBindVertexArray(globalVAO);
			float tmp[16];
			memset(tmp, 0, sizeof(tmp));
			glBufferData(GL_ARRAY_BUFFER, sizeof(tmp), tmp, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			return win;
		}


		static Shader* getDefaultShader() { return defaultShader; }
		static int getGlobalVBO() { return globalVBO; }
		static int getGlobalVAO() { return globalVAO; }
		unsigned int getTicks() const { return SDL_GetTicks(); }


		void playSound(Sound* sound, unsigned int channel) {
			if (channel >= NUM_SOUND_CHANNELS)
				throw std::exception("invalid channel id");
			auto c = soundChannels[channel];
			c->stop();
			c->play(sound);
		}


		SoundChannel* getSoundChannel(int channel) {
			if (channel >= NUM_SOUND_CHANNELS)
				throw std::exception("invalid channel id");
			return soundChannels[channel];
		}


	private:
		std::unordered_set<_Window<Engine>*> windows;
		void* glContext;
		SoundChannel* soundChannels[NUM_SOUND_CHANNELS];
		inline static bool initialized = false;
		inline static Shader* defaultShader = NULL;
		inline static int globalVBO = 0;
		inline static int globalVAO = 0;
		inline static ALCdevice* alDevice = NULL;
		ALCcontext* alContext;
		inline static const char* vertCode = R"(
			#version 330 core
			layout (location = 0) in vec3 pos;
			layout (location = 1) in vec2 uv;

			out vec2 fragCoord;

			uniform mat4 mvp;

			void main() {
				gl_Position = mvp * vec4(pos, 1.0);
				fragCoord = uv;
			}
		)";
		inline static const char* fragCode = R"(
			#version 330 core
			in vec2 fragCoord;
			out vec4 fragColor;
			uniform sampler2D tex;

			void main() {
				fragColor = texture(tex, fragCoord);
			}
		)";


		Engine(const Engine& other) = delete;
	};
}
#define __LEAF_ENGINE__
#endif
