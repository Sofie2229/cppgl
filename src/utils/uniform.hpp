#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "program.hpp"

class Uniform {
public:
	GLuint location;
	Uniform() {}
	Uniform(Program program, std::string name) {
		getLocation(program, name);
	}
	void getLocation(Program program, std::string name) {
		location = glGetUniformLocation(program, name.c_str());
	}
	void setMat4(glm::mat4 mat4) {
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(mat4));
	}
	void setFloat(float f) {
		glUniform1f(location, f);
	}
};