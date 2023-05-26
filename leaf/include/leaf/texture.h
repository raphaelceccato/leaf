#pragma once
#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include "export.h"

namespace leaf {
	class Texture;
	typedef std::shared_ptr<Texture> TexturePtr;


	class EXPORT Texture {
	public:
		~Texture();
		int getHandle() const { return handle; }
		int getWidth() const { return width; }
		int getHeight() const { return height; }
		void bind();
		int getHandle() { return handle; }
		virtual bool isRenderTexture() const { return false; }
		bool isSmooth() const;
		void setSmooth(bool smooth);
		bool isRepeated() const;
		void setRepeated(bool repeat);
		glm::ivec2 getSize() { return glm::ivec2(width, height); }

		static TexturePtr create(int width, int height, unsigned char* data = nullptr);
		static TexturePtr create(std::string path);
		static void unbind();

	protected:
		int handle;
		int width, height;
		bool repeated;

		Texture();
	};
}