#include "../include/leaf/texture.h"
#include <SDL2/SDL_opengl.h>
#include <SOIL/SOIL.h>


namespace leaf {
	Texture::Texture() {
		handle = width = height = 0;
		repeated = false;
	}


	TexturePtr Texture::create(int width, int height, unsigned char* data) {
		int handle;
		glGenTextures(1, (GLuint*)&handle);
		if (!handle) {
			throw std::exception(("error creating texture with size " + std::to_string(width) + "x"
				+ std::to_string(height)).c_str());
		}
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glBindTexture(GL_TEXTURE_2D, 0);

		auto tex = std::shared_ptr<Texture>(new Texture());
		tex->handle = handle;
		tex->width = width;
		tex->height = height;
		return tex;
	}


	TexturePtr Texture::create(std::string path) {
		int handle, width, height, channels;
		unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
		if (!data)
			throw std::exception(("Could not load texture file: " + path).c_str());
		handle = SOIL_create_OGL_texture(data, width, height, channels, 0, SOIL_FLAG_MULTIPLY_ALPHA);
		if (!handle)
			throw std::exception(("Error creating texture from file: " + path).c_str());
		SOIL_free_image_data(data);

		auto tex = std::shared_ptr<Texture>(new Texture());
		tex->handle = handle;
		tex->width = width;
		tex->height = height;
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return tex;
	}


	Texture::~Texture() {
		if (handle)
			glDeleteTextures(1, (GLuint*)&handle);
	}


	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, handle);
	}


	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	bool Texture::isSmooth() const {
		GLint filter;
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &filter);
		return filter == GL_LINEAR;
	}


	void Texture::setSmooth(bool smooth) {
		glBindTexture(GL_TEXTURE_2D, handle);
		GLint param = smooth ? GL_LINEAR : GL_NEAREST;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	}


	bool Texture::isRepeated() const {
		return repeated;
	}


	void Texture::setRepeated(bool repeat) {
		glBindTexture(GL_TEXTURE_2D, handle);
		GLint param = (repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
		glBindTexture(GL_TEXTURE_2D, 0);
		repeated = repeat;
	}
}