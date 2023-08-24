// main.cpp

#include "headers.hpp"

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
	mat4 MVP = projMat * viewMat * modelMat;
	gl_Position = MVP * vec4(pos, 1);
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
	auto window = glfwCreateWindow(1000, 600, "cppgl", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	if (gladLoadGL() != true) throw std::runtime_error("gladLoadGL() failed");

	Shader vertexShader{vssrc, GL_VERTEX_SHADER, true};
	Shader fragmentShader{fssrc, GL_FRAGMENT_SHADER, true};
	Program program{{vertexShader, fragmentShader}, true};

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

		glUseProgram(program);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}