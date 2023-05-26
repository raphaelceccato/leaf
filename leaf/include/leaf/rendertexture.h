#pragma once
#include "export.h"
#include "texture.h"
#include "rendertarget.h"

namespace leaf {
	class RenderTexture;
	typedef std::shared_ptr<RenderTexture> RenderTexturePtr;


	class EXPORT RenderTexture : public Texture, public RenderTarget {
	public:
		~RenderTexture();
		virtual bool isRenderTexture() const { return true; }
		void bindFBO();
		int getFBO() { return fbo; }

		static RenderTexturePtr create(int width, int height);
		static void unbindFBO();

		void draw(TexturePtr tex, int x, int y, int width, int height, ShaderPtr shader = nullptr) override;
		void drawEx(TexturePtr tex, int x, int y, int width, int height, Rect<int> subrect, float angle, ShaderPtr shader = nullptr) override;
		void clear(const Color& color) override;

	protected:
		int fbo;

		RenderTexture();
	};
}