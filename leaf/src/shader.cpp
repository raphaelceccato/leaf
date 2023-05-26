#include "../include/leaf/shader.h"
#include <glad/glad.h>
#include <string>


namespace leaf {
	Shader::Shader(const char* vertexCode, const char* fragmentCode) {
		vert = frag = program = 0;
		if (vertexCode) {
			vert = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vert, 1, &vertexCode, NULL);
			compile(vert);
		}
		if (fragmentCode) {
			frag = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(frag, 1, &fragmentCode, NULL);
			compile(frag);
		}

		program = glCreateProgram();
		if (vert)
			glAttachShader(program, vert);
		if (frag)
			glAttachShader(program, frag);
		link();

		if (vert) {
			glDeleteShader(vert);
			vert = 0;
		}
		if (frag) {
			glDeleteShader(frag);
			frag = 0;
		}
	}


	Shader::~Shader() {
		if (vert)
			glDeleteShader(vert);
		if (frag)
			glDeleteShader(frag);
		if (program)
			glDeleteProgram(program);
	}


	void Shader::compile(int handle) {
		glCompileShader(handle);
		int success = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			char msg[512];
			glGetShaderInfoLog(handle, 512, NULL, msg);
			throw std::exception((std::string("error compiling shader: ") + msg).c_str());
		}
	}


	void Shader::link() {
		int success = 0;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char msg[512];
			glGetShaderInfoLog(program, 512, NULL, msg);
			throw std::exception((std::string("error linking shader program: ") + msg).c_str());
		}
	}


	void Shader::bind() {
		glUseProgram(program);
	}


	void Shader::unbind() {
		glUseProgram(0);
	}


	void Shader::setUniform(const char* name, int value) {
		int loc = glGetUniformLocation(program, name);
		glUniform1i(loc, value);
	}


	void Shader::setUniform(const char* name, float value) {
		int loc = glGetUniformLocation(program, name);
		glUniform1f(loc, value);
	}


	void Shader::setUniform(const char* name, glm::vec2 value) {
		int loc = glGetUniformLocation(program, name);
		glUniform2f(loc, value.x, value.y);
	}


	void Shader::setUniform(const char* name, glm::ivec2 value) {
		int loc = glGetUniformLocation(program, name);
		glUniform2i(loc, value.x, value.y);
	}


	void Shader::setUniform(const char* name, glm::vec3 value) {
		int loc = glGetUniformLocation(program, name);
		glUniform3f(loc, value.x, value.y, value.z);
	}


	void Shader::setUniform(const char* name, glm::ivec3 value) {
		int loc = glGetUniformLocation(program, name);
		glUniform3i(loc, value.x, value.y, value.z);
	}


	void Shader::setUniform(const char* name, int values[], int count) {
		int loc = glGetUniformLocation(program, name);
		glUniform1iv(loc, count, values);
	}


	void Shader::setUniform(const char* name, float values[], int count) {
		int loc = glGetUniformLocation(program, name);
		glUniform1fv(loc, count, values);
	}


	void Shader::setUniform(const char* name, glm::vec2 values[], int count) {
		float tmp[1024];
		for (int i = 0, j = 0; j < count; i += 2, j++) {
			tmp[i] = values[j].x;
			tmp[i + 1] = values[j].y;
		}
		int loc = glGetUniformLocation(program, name);
		glUniform2fv(loc, count, tmp);
	}


	void Shader::setUniform(const char* name, glm::vec3 values[], int count) {
		float tmp[1024];
		for (int i = 0, j = 0; j < count; i += 3, j++) {
			tmp[i] = values[j].x;
			tmp[i + 1] = values[j].y;
			tmp[i + 2] = values[j].z;
		}
		int loc = glGetUniformLocation(program, name);
		glUniform3fv(loc, count, tmp);
	}


	void Shader::setUniform(const char* name, TexturePtr texture, int unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		texture->bind();
		int loc = glGetUniformLocation(program, name);
		glUniform1i(loc, unit);
		glActiveTexture(GL_TEXTURE0);
	}


	void Shader::setUniform(const char* name, glm::mat4 matrix) {
		int loc = glGetUniformLocation(program, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
	}


	void Shader::setUniform(const char* name, Color color) {
		int loc = glGetUniformLocation(program, name);
		glUniform3f(loc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	}
}