#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "program.hpp"
#include <memory>

class Uniform {
public:
	GLuint location;
	Uniform() {}
	std::shared_ptr<Program> program;
	Uniform(std::shared_ptr<Program> program, std::string name) {
		getLocation(program, name);
	}
	void getLocation(std::shared_ptr<Program> program, std::string name) {
		location = glGetUniformLocation(*program, name.c_str());
		this->program = program;
	}
	void setMat4(glm::mat4 mat4) {
		glUseProgram(*program);
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(mat4));
	}
	void setFloat(float f) {
		glUseProgram(*program);
		glUniform1f(location, f);
	}
};