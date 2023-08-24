#pragma once

#include "headers.hpp"

class Shader {
public:
	GLuint id;
	std::string src;
	GLenum type;
	Shader() {};
	Shader(std::string src, GLenum type, bool compile = false): src(src), type(type) {
		if (compile) this->compile();
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
		if (result == GL_FALSE) {
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

	GLuint compile(std::string src, GLenum type) {
		this->src = src;
		return compile();
	}

	~Shader() {
		glDeleteShader(id);
	}
};

class Program {
public:
	GLuint id;
	std::vector<Shader> shaders;
	Program() {}
	Program(std::vector<Shader> shaders, bool link = false): shaders(shaders) {
		if (link) this->link();
	}
	operator GLuint() { return id; }
	GLuint link() {
		if (id) glDeleteProgram(id);
		id = glCreateProgram();
		for (auto &s : shaders) glAttachShader(id, s);
		glLinkProgram(id);
		for (auto &s : shaders) glDetachShader(id, s);
		GLint result = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &result);
		if (result == GL_FALSE) {
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

class VBO {
public:
	GLuint id;
	operator GLuint() { return id; }
	VBO() {
		glCreateBuffers(1, &id);
		bind();
	}
	~VBO() {
		glDeleteBuffers(1, &id);
	}
	template<typename T>
	void data(std::vector<T> data, GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW) {
		bind();
		glBufferData(target, data.size() * sizeof(data.front()), data.data(), GL_STATIC_DRAW);
	}
	void bind(GLenum target = GL_ARRAY_BUFFER) { glBindBuffer(target, id); }
	void unbind(GLenum target = GL_ARRAY_BUFFER) { glBindBuffer(target, 0); }
};

class VAO {
public:
	GLuint id;
	operator GLuint() { return id; }
	VAO() {
		glCreateVertexArrays(1, &id);
		bind();
	}
	~VAO() {
		glDeleteVertexArrays(1, &id);
	}
	void bind() { glBindVertexArray(id); }
	void unbind() { glBindVertexArray(0); }
	void attrib(VBO& vbo, GLuint loc, GLint size, GLenum type = GL_FLOAT) {
		bind();
		vbo.bind();
		glEnableVertexAttribArray(loc);
		glVertexAttribPointer(loc, size, type, false, 0, nullptr);
	}
};