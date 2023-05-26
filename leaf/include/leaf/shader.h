#pragma once
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "export.h"
#include "texture.h"
#include "color.h"


namespace leaf {
	class EXPORT Shader {
	public:
		Shader(const char* vertex, const char* fragment);
		~Shader();
		int getHandle() { return program; }
		void bind();
		void setUniform(const char* name, int value);
		void setUniform(const char* name, float value);
		void setUniform(const char* name, glm::vec2 value);
		void setUniform(const char* name, glm::ivec2 value);
		void setUniform(const char* name, glm::vec3 value);
		void setUniform(const char* name, glm::ivec3 value);
		void setUniform(const char* name, int values[], int count);
		void setUniform(const char* name, float values[], int count);
		void setUniform(const char* name, glm::vec2 values[], int count);
		void setUniform(const char* name, glm::vec3 values[], int count);
		void setUniform(const char* name, TexturePtr texture, int unit);
		void setUniform(const char* name, glm::mat4 matrix);
		void setUniform(const char* name, Color color);

		static void unbind();
	private:
		int vert, frag;
		int program;

		void compile(int handle);
		void link();
	};


	typedef std::shared_ptr<Shader> ShaderPtr;
}