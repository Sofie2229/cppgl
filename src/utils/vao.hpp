#pragma once

#include <glad/glad.h>
#include "vbo.hpp"

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