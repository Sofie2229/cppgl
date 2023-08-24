// main.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "utils.hpp"

const std::string vssrc = R"(
#version 460 core
layout (location = 0) in vec3 pos;
uniform mat4 modelMat = mat4(1);
uniform mat4 viewMat = mat4(1);
uniform mat4 projMat = mat4(1);
void main() {
	gl_Position = projMat * viewMat * modelMat * vec4(pos, 1);
}
)";

const std::string fssrc = R"(
#version 460 core
out vec3 color;
void main() {
	color = vec3(1.0, 1.0, 1.0);
}
)";

int main() {
	if (glfwInit() != GLFW_TRUE) throw std::runtime_error("glfwInit() failed");
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	float width = 1000;
	float height = 600;
	auto window = glfwCreateWindow(width, height, "cppgl", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (gladLoadGL() != true) throw std::runtime_error("gladLoadGL() failed");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Shader vertexShader{vssrc, GL_VERTEX_SHADER, true};
	Shader fragmentShader{fssrc, GL_FRAGMENT_SHADER, true};
	Program program{{vertexShader, fragmentShader}, true};
	glUseProgram(program);
	
	glm::mat4 projMat = glm::perspective(glm::radians(90.0f), width / height, 0.1f, 1000.0f);
	glm::mat4 viewMat = glm::lookAt(glm::vec3{1,-2,1}, {0,0,0}, {0,0,1});
	glm::mat4 modelMat = glm::mat4{1};
	auto projMatLoc = glGetUniformLocation(program, "projMat");
	auto viewMatLoc = glGetUniformLocation(program, "viewMat");
	auto modelMatLoc = glGetUniformLocation(program, "modelMat");
	glUniformMatrix4fv(projMatLoc, 1, false, glm::value_ptr(projMat));
	glUniformMatrix4fv(viewMatLoc, 1, false, glm::value_ptr(viewMat));
	glUniformMatrix4fv(modelMatLoc, 1, false, glm::value_ptr(modelMat));

	VAO vao;
	VBO vbo;
	std::vector<glm::vec3> buff = {
		{0.0, 1.0, 0.0},
		{-1.0, -1.0, 0.0},
		{1.0, -1.0, 0.0}
	};
	vbo.data(buff);
	vao.attrib(vbo, 0, 3);
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vbo.size);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}