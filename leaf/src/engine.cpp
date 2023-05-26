#include "../include/leaf/engine.h"
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <string>

namespace leaf {
#define throwIfFalse(x, y) if (x == false) throw std::exception(y)

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

	ShaderPtr Engine::defaultShader = nullptr;
	bool Engine::initialized = false;
	int Engine::globalVAO = 0, Engine::globalVBO = 0;
	ALCdevice* Engine::alDevice = NULL;


	Engine::Engine() {
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


	Engine::~Engine() {
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


	WindowPtr Engine::createWindow(const char* title, int width, int height) {
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


	unsigned int Engine::getTicks() const {
		return SDL_GetTicks();
	}


	void Engine::playSound(SoundPtr sound, unsigned int channel) {
		if (channel >= NUM_SOUND_CHANNELS)
			throw std::exception("invalid channel id");
		auto c = soundChannels[channel];
		c->play(sound);
	}
}