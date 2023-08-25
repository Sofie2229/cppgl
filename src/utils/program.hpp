#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include "shader.hpp"

class Program {
public:
	GLuint id {};
	std::vector<std::shared_ptr<Shader>> shaders {};
	Program() {}
	~Program() {
		glDeleteProgram(id);
	}
	Program(std::vector<std::shared_ptr<Shader>> shaders): shaders(shaders) {
		link();
	}
	operator GLuint() { return id; }
	GLuint link() {
		if (id) glDeleteProgram(id);
		id = glCreateProgram();
		for (auto &s : shaders) glAttachShader(id, *s);
		glLinkProgram(id);
		for (auto &s : shaders) glDetachShader(id, *s);
		GLint result = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &result);
		if (result != GL_TRUE) {
			GLint infosize;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infosize);
			std::string info;
			info.resize(infosize);
			glGetProgramInfoLog(id, infosize, NULL, info.data());
			std::cerr << "Error while linking program:" << std::endl;
			std::cerr << info << std::endl;
			glDeleteProgram(id);
			return 0;
		}
		return id;
	}
};