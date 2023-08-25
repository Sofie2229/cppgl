#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <memory>

class Shader {
public:
	GLuint id {};
	std::string src {};
	GLenum type {};
	Shader() {};
	~Shader() {
		glDeleteShader(id);
	}
	Shader(std::string src, GLenum type): src(src), type(type) {
		this->compile();
	}
	operator GLuint() { return id; }
	GLuint compile() {
		if (id) glDeleteShader(id);
		id = glCreateShader(type);
		const char* srcp = src.c_str();
		glShaderSource(id, 1, &srcp, NULL);
		glCompileShader(id);
		GLint result = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result != GL_TRUE) {
			GLint infosize;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infosize);
			std::string info;
			info.resize(infosize);
			glGetShaderInfoLog(id, infosize, NULL, info.data());
			std::cerr << "Error while compiling shader:" << std::endl;
			std::cerr << info << std::endl;
			glDeleteShader(id);
			return 0;
		}
		return id;
	}
};

class VertexShader: public Shader {
public:
	VertexShader(std::string src): Shader(src, GL_VERTEX_SHADER) {}
};

class FragmentShader: public Shader {
public:
	FragmentShader(std::string src): Shader(src, GL_FRAGMENT_SHADER) {}
};