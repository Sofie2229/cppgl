// main.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.hpp"

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
	Uniform uprojMat{program, "projMat"};
	Uniform uviewMat{program, "viewMat"};
	Uniform umodelMat{program, "modelMat"};

	VAO vao;
	VBO vbo;
	std::vector<glm::vec3> buff = {
		{0.0, 1.0, 0.0},
		{-1.0, -1.0, 0.0},
		{1.0, -1.0, 0.0}
	};
	vbo.data(buff);
	vao.attrib(vbo, 0, 3);
	
	Camera camera {{1,-2,1}};
	camera.angles(glm::radians(glm::vec3{30, -20, 0}));
	
	Clock clock;
	while (!glfwWindowShouldClose(window)) {
		float dt = clock.dt();
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		uprojMat.setMat4(projMat);
		uviewMat.setMat4(camera.viewMat());
		float camspeed = dt * 3;
		if (glfwGetKey(window, GLFW_KEY_UP)) camera.pitch(-camspeed);
		if (glfwGetKey(window, GLFW_KEY_DOWN)) camera.pitch(camspeed);
		if (glfwGetKey(window, GLFW_KEY_LEFT)) camera.yaw(-camspeed);
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) camera.yaw(camspeed);
		if (glfwGetKey(window, GLFW_KEY_Q)) camera.roll(camspeed);
		if (glfwGetKey(window, GLFW_KEY_E)) camera.roll(-camspeed);
		if (glfwGetKey(window, GLFW_KEY_SPACE)) camera.pos += camera.up(true) * dt;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) camera.pos += camera.down(true) * dt;
		if (glfwGetKey(window, GLFW_KEY_W)) camera.pos += camera.forward(true) * dt;
		if (glfwGetKey(window, GLFW_KEY_S)) camera.pos += camera.backward(true) * dt;
		if (glfwGetKey(window, GLFW_KEY_A)) camera.pos += camera.left(true) * dt;
		if (glfwGetKey(window, GLFW_KEY_D)) camera.pos += camera.right(true) * dt;
		if (glfwGetKey(window, GLFW_KEY_R)) {
			camera.pos = {1, -2, 1};
			camera.angles(glm::radians(glm::vec3(30, -20, 0)));
		}
		
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vbo.size);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}