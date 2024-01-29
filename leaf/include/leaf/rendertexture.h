#ifndef __LEAF_RENDERTEXTURE__
#define __LEAF_RENDERTEXTURE__

#include "texture.h"
#include "rendertarget.h"
#include "engine.h"
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace leaf {
	class RenderTexture;
	typedef std::shared_ptr<RenderTexture> RenderTexturePtr;


	class RenderTexture : public Texture, public RenderTarget {
	public:
		~RenderTexture() {
			if (fbo) {
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
				glDeleteFramebuffers(1, (GLuint*)&fbo);
			}
			//base destructor will be called here and delete the texture
		}


		virtual bool isRenderTexture() const { return true; }
		void bindFBO() { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
		static void unbindFBO() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		int getFBO() { return fbo; }


		static RenderTexturePtr create(int width, int height) {
			int handle;
			glGenTextures(1, (GLuint*)&handle);
			if (!handle) {
				throw std::exception(("error creating texture with size " + std::to_string(width) + "x"
					+ std::to_string(height)).c_str());
			}
			glBindTexture(GL_TEXTURE_2D, handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

			int fbo = 0;
			glGenFramebuffers(1, (GLuint*)&fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
			auto a = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (a != GL_FRAMEBUFFER_COMPLETE)
				throw std::exception("error creating FBO");

			auto tex = std::shared_ptr<RenderTexture>(new RenderTexture());
			tex->handle = handle;
			tex->width = width;
			tex->height = height;
			tex->fbo = fbo;
			return tex;
		}


		void draw(TexturePtr tex, int x, int y, int width, int height, ShaderPtr shader = nullptr) override {
			float u = 1.0f;
			float v = 1.0f;

			if (tex && tex->isRepeated()) {
				u = width / (float)tex->getWidth();
				v = height / (float)tex->getHeight();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glBindTexture(GL_TEXTURE_2D, tex ? tex->getHandle() : 0);
			glViewport(0, 0, this->width, this->height);
			glm::mat4 mvp = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height);
			if (shader == nullptr)
				shader = Engine::getDefaultShader();
			shader->bind();
			shader->setUniform("mvp", mvp);

			glBindVertexArray(Engine::getGlobalVAO());
			glBindBuffer(GL_ARRAY_BUFFER, Engine::getGlobalVBO());

			float vertices[16] = { (float)x, (float)y,							0.0f, 0.0f, //vertex (2 floats), uv (2 floats)
								   (float)x + width, (float)y,					u, 0.0f,
								   (float)x + width, (float)(y + height),		u, v,
								   (float)x, (float)(y + height),				0.0f, v };
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glDrawArrays(GL_QUADS, 0, 4);
		}


		void drawEx(TexturePtr tex, int x, int y, int width, int height, Rect<int> subrect, FlipMode flip, float angle, ShaderPtr shader = nullptr) override {
			float u0 = 0, v0 = 0, u1 = 1, v1 = 1;
			u0 = subrect.x / (float)tex->getWidth();
			v0 = subrect.y / (float)tex->getHeight();
			u1 = (subrect.x + subrect.w) / (float)tex->getWidth();
			v1 = (subrect.y + subrect.h) / (float)tex->getHeight();

			if (flip == FlipMode::Horizontal || flip == FlipMode::Both) {
				float tmp = u0;
				u0 = u1;
				u1 = tmp;
			}

			if (flip == FlipMode::Vertical || flip == FlipMode::Both) {
				float tmp = v0;
				v0 = v1;
				v1 = tmp;
			}

			float vertices[16] = { -width / 2.0f, -height / 2.0f,					u0, v0, //vertex (2 floats), uv (2 floats)
								   width / 2.0f, -height / 2.0f,					u1, v0,
								   width / 2.0f, height / 2.0f,						u1, v1,
								   -width / 2.0f, height / 2.0f,					u0, v1 };
			glBindVertexArray(Engine::getGlobalVAO());
			glBindBuffer(GL_ARRAY_BUFFER, Engine::getGlobalVBO());
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindTexture(GL_TEXTURE_2D, (tex ? tex->getHandle() : 0));
			shader = (shader ? shader : Engine::getDefaultShader());

			glm::mat4 model = glm::mat4(1.0f);
			model = model * glm::translate(glm::mat4(1.0f), glm::vec3(x + width / 2.0f, y + height / 2.0f, 0));
			if (angle != 0)
				model = model * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
			glm::mat4 view(1.0f);
			glm::mat4 proj = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height);
			glm::mat4 mvp = proj * view * model;
			shader->bind();
			shader->setUniform("mvp", mvp);

			glViewport(0, 0, this->width, this->height);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glDrawArrays(GL_QUADS, 0, 4);
		}


		void clear(const Color& color) override {
			GLubyte colorArr[4] = { (GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b, (GLubyte)color.a };
			glClearTexImage(handle, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorArr);
		}


	protected:
		int fbo;

		RenderTexture() : Texture() {
			fbo = 0;
		}
	};
}

#endif
