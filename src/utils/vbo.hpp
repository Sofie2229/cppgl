#pragma once

#include <glad/glad.h>
#include <vector>

class VBO {
public:
	GLuint id;
	size_t size;
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
		size = data.size();
	}
	void bind(GLenum target = GL_ARRAY_BUFFER) { glBindBuffer(target, id); }
	void unbind(GLenum target = GL_ARRAY_BUFFER) { glBindBuffer(target, 0); }
};