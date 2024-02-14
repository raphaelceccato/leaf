#ifndef __LEAF_RENDERTARGET__
#define __LEAF_RENDERTARGET__

#include "shader.h"
#include "texture.h"
#include "color.h"
#include "rect.h"


namespace leaf {
	enum class FlipMode {
		None,
		Horizontal,
		Vertical,
		Both
	};


	class RenderTarget;
	typedef std::shared_ptr<RenderTarget> RenderTargetPtr;

	class RenderTarget {
	public:
		virtual void draw(Texture* tex, int x, int y, int width, int height, Shader* shader = nullptr) = 0;
		virtual void drawEx(Texture* tex, int x, int y, int width, int height, Rect<int> subrect, FlipMode flip, float angle, Shader* shader = nullptr) = 0;
		virtual void clear(const Color& color) = 0;
	};
}

#endif
