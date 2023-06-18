#pragma once
#include "window.h"
#include "soundchannel.h"
#include <vector>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <AL/alc.h>

struct ALCdevice;
struct ALCcontext;

namespace leaf {
	constexpr int NUM_SOUND_CHANNELS = 10;

	constexpr void throwIfFalse(bool condition, const char* msg) {
		if (!condition)
			throw std::exception(msg);
	}


	class Engine {
	public:
		Engine() {
			prepareStatic();

			if (initialized)
				throw std::exception("another instance of the engine is already initialized");
			initialized = true;

			window = nullptr;
			glContext = nullptr;
			SDL_Init(SDL_INIT_EVERYTHING);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			alDevice = alcOpenDevice(NULL);
			alContext = alcCreateContext(alDevice, NULL);
			alcMakeContextCurrent(alContext);
			for (int i = 0; i < NUM_SOUND_CHANNELS; i++) {
				auto c = std::make_shared<SoundChannel>(i);
				soundChannels[i] = c;
			}
		}


		~Engine() {
			window = nullptr;
			defaultShader = nullptr;
			for (int i = 0; i < NUM_SOUND_CHANNELS; i++)
				soundChannels[i] = nullptr;
			initialized = false;
			if (globalVBO)
				glDeleteBuffers(1, (GLuint*)&globalVBO);
			if (globalVAO)
				glDeleteVertexArrays(1, (GLuint*)&globalVAO);
			if (glContext)
				SDL_GL_DeleteContext(glContext);
			SDL_Quit();
			if (alDevice)
				alcCloseDevice(alDevice);
		}


		WindowPtr createWindow(const char* title, int width, int height) {
			window = std::shared_ptr<Window>(new Window(title, width, height));

			glContext = SDL_GL_CreateContext(window->getSDLWindow());
			if (!glContext)
				throw std::exception(("error creating context: " + std::string(SDL_GetError()) + ")").c_str());

			throwIfFalse(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "Could not initialize glad");

			//these needs to be after loading glad
			window->init();
			defaultShader = std::make_shared<Shader>(vertCode, fragCode);
			glGenBuffers(1, (GLuint*)&globalVBO);
			glGenVertexArrays(1, (GLuint*)&globalVAO);

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			return window;
		}


		WindowPtr getWindow() const { return window; }
		static ShaderPtr getDefaultShader() { return defaultShader; }
		static int getGlobalVBO() { return globalVBO; }
		static int getGlobalVAO() { return globalVAO; }
		unsigned int getTicks() const { return SDL_GetTicks(); }


		void playSound(SoundPtr sound, unsigned int channel) {
			if (channel >= NUM_SOUND_CHANNELS)
				throw std::exception("invalid channel id");
			auto c = soundChannels[channel];
			c->play(sound);
		}


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


		const char* vertCode = R"(
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
		const char* fragCode = R"(
			#version 330 core
			in vec2 fragCoord;
			out vec4 fragColor;
			uniform sampler2D tex;

			void main() {
				fragColor = texture(tex, fragCoord);
			}
		)";


		static void prepareStatic() {
			static ShaderPtr defaultShader = nullptr;
			static bool initialized = false;
			static int globalVAO = 0, globalVBO = 0;
			static ALCdevice* alDevice = NULL;
		}
	};
}