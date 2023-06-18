#pragma once
#include "texture.h"
#include "shader.h"
#include "color.h"
#include "rect.h"


namespace leaf {
	class RenderTarget;
	typedef std::shared_ptr<RenderTarget> RenderTargetPtr;

	class RenderTarget {
	public:
		virtual void draw(TexturePtr tex, int x, int y, int width, int height, ShaderPtr shader = nullptr) = 0;
		virtual void drawEx(TexturePtr tex, int x, int y, int width, int height, Rect<int> subrect, float angle, ShaderPtr shader = nullptr) = 0;
		virtual void clear(const Color& color) = 0;
	};
}