#pragma once
#include <memory>
#include <glm/vec2.hpp>
#include <SDL2/SDL_events.h>
#include "export.h"
#include "rendertarget.h"

struct SDL_Window;

namespace leaf {
	class Engine;
	class Window;
	typedef std::shared_ptr<Window> WindowPtr;

	class EXPORT Window : public RenderTarget {
	public:
		~Window();
		SDL_Window* getSDLWindow() const { return win; }
		glm::ivec2 getSize() const;
		void setSize(int width, int height);
		bool pollEvent(SDL_Event* ev);
		void useVSync(bool vsync);
		void redraw();

		void draw(TexturePtr tex, int x, int y, int width, int height, ShaderPtr shader = nullptr) override;
		void drawEx(TexturePtr tex, int x, int y, int width, int height, Rect<int> subrect, float angle, ShaderPtr shader = nullptr) override;
		void clear(const Color& color) override;
	private:
		SDL_Window* win;
		int vbo, vao;

		friend class Engine;

		Window(const char* title, int width, int height);
		void init();
		void onResize(SDL_Event* ev);
	};
}