#ifndef __LEAF_WINDOW__
#define __LEAF_WINDOW__

#include <memory>
#include <string>
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include "rendertarget.h"


struct SDL_Window;

namespace leaf {
	class Engine;


	template <typename TEngine> class _Window : public RenderTarget {
	public:
		~_Window() {
			if (vao)
				glDeleteVertexArrays(1, (GLuint*)&vao);
			if (vbo)
				glDeleteBuffers(1, (GLuint*)&vbo);
			if (win)
				SDL_DestroyWindow(win);
		}


		SDL_Window* getSDLWindow() const { return win; }


		glm::ivec2 getSize() const {
			int w, h;
			SDL_GetWindowSize(win, &w, &h);
			return glm::ivec2(w, h);
		}


		void setSize(int width, int height) { SDL_SetWindowSize(win, width, height); }


		bool pollEvent(SDL_Event* ev) {
			if (SDL_PollEvent(ev) == 0)
				return false;
			if (ev->type == SDL_WINDOWEVENT && ev->window.type == SDL_WINDOWEVENT_SIZE_CHANGED)
				onResize(ev);
			return true;
		}


		void useVSync(bool vsync) {
			if (SDL_GL_SetSwapInterval(vsync ? 1 : 0) < 0)
				throw std::exception(("failed to change vsync: " + std::string(SDL_GetError())).c_str());
		}


		void redraw() { SDL_GL_SwapWindow(win); }


		void draw(TexturePtr tex, int x, int y, int width, int height, ShaderPtr shader = nullptr) override {
			int winW, winH;
			SDL_GetWindowSize(win, &winW, &winH);

			float vertices[16] = { 0.0f, 0.0f,							0.0f, 0.0f, //vertex (2 floats), uv (2 floats)
								   (float)width, 0.0f,					1.0f, 0.0f,
								   (float)width, (float)height,			1.0f, 1.0f,
								   0.0f, (float)height,					0.0f, 1.0f };
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindTexture(GL_TEXTURE_2D, (tex ? tex->getHandle() : 0));
			shader = (shader ? shader : engine->getDefaultShader());

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
			glm::mat4 view(1.0f);
			glm::mat4 proj = glm::ortho(0.0f, (float)winW, (float)winH, 0.0f);
			glm::mat4 mvp = proj * view * model;
			shader->bind();
			shader->setUniform("mvp", mvp);

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, winW, winH);
			glDrawArrays(GL_QUADS, 0, 4);
		}


		void drawEx(TexturePtr tex, int x, int y, int width, int height, Rect<int> subrect, FlipMode flip, float angle, ShaderPtr shader = nullptr) override {
			int winW, winH;
			SDL_GetWindowSize(win, &winW, &winH);

			float u0 = 0, v0 = 0, u1 = 1, v1 = 1;
			u0 = subrect.x / (float)tex->getWidth();
			v0 = subrect.y / (float)tex->getHeight();
			u1 = (subrect.x + subrect.w) / (float)tex->getWidth();
			v1 = (subrect.y + subrect.h) / (float)tex->getHeight();

			float vertices[16] = { -width / 2.0f, -height / 2.0f,					u0, v0, //vertex (2 floats), uv (2 floats)
								   width / 2.0f, -height / 2.0f,					u1, v0,
								   width / 2.0f, height / 2.0f,						u1, v1,
								   -width / 2.0f, height / 2.0f,					u0, v1 };
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindTexture(GL_TEXTURE_2D, (tex ? tex->getHandle() : 0));
			shader = (shader ? shader : engine->getDefaultShader());

			glm::mat4 model = glm::mat4(1.0f);
			model = model * glm::translate(glm::mat4(1.0f), glm::vec3(x + width / 2.0f, y + height / 2.0f, 0));
			if (angle != 0)
				model = model * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
			glm::mat4 view(1.0f);
			glm::mat4 proj = glm::ortho(0.0f, (float)winW, (float)winH, 0.0f);
			glm::mat4 mvp = proj * view * model;
			shader->bind();
			shader->setUniform("mvp", mvp);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, winW, winH);
			glDrawArrays(GL_QUADS, 0, 4);
		}


		void clear(const Color& color) override {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}


	private:
		SDL_Window* win;
		TEngine* engine;
		int vbo, vao;

		friend class Engine;

		_Window(TEngine* engine, const char* title, int width, int height, bool resizable) {
			this->engine = engine;
			vao = vbo = 0;
			win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (resizable ? SDL_WINDOW_RESIZABLE : 0));
			if (!win)
				throw std::exception(("error creating SDL window: " + std::string(SDL_GetError()) + ")").c_str());
		}

                _Window(const _Window& other) = delete;

		void init() {
			int width, height;
			SDL_GetWindowSize(win, &width, &height);

			glGenVertexArrays(1, (GLuint*)&vao);
			glBindVertexArray(vao);

			glGenBuffers(1, (GLuint*)&vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			float vertices[16] = { 0.0f, 0.0f,							0.0f, 0.0f, //vertex (2 floats), uv (2 floats)
								   (float)width, 0.0f,					1.0f, 0.0f,
								   (float)width, (float)height,			1.0f, 1.0f,
								   0.0f, (float)height,					0.0f, 1.0f };

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
		}


		void onResize(SDL_Event* ev) {

		}
	};
}

template<typename T> struct std::hash<leaf::_Window<T>> {
	size_t operator()(const leaf::_Window<T>& win) const {
		return (size_t)win.getSDLWindow();
	}
};

#endif
