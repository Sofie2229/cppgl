// main.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "game/debugcross.hpp"

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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	float width = 1600;
	float height = 900;
	auto window = glfwCreateWindow(width, height, "cppgl", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 100, 100);
	if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	if (gladLoadGL() != true) throw std::runtime_error("gladLoadGL() failed");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	auto program = std::make_shared<Program>(std::vector<std::shared_ptr<Shader>>{
		std::make_shared<VertexShader>(vssrc),
		std::make_shared<FragmentShader>(fssrc),
	});
	
	glm::mat4 projMat = glm::perspective(glm::radians(90.0f), width / height, 0.01f, 1000.0f);
	glm::mat4 viewMat = glm::lookAt(glm::vec3{1,-2,1}, {0,0,0}, {0,0,1});
	glm::mat4 modelMat = glm::mat4{1};
	Uniform uprojMat{program, "projMat"};
	Uniform uviewMat{program, "viewMat"};
	Uniform umodelMat{program, "modelMat"};
	
	Camera camera {{1,-2,1}};
	camera.angles(glm::radians(glm::vec3{30, -20, 0}));
	bool firstClick;

	Clock clock;
	DebugCross crossOnScreen;
	DebugCross crossOn00;
	
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);
		float dt = clock.dt();
		std::printf("%0.2fms\n",dt*1000);
		
		const auto camspeed = dt * 3;
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
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (firstClick) {
				firstClick = false;
				glfwSetCursorPos(window, 0, 0);
			}
			double mx, my;
			glfwGetCursorPos(window, &mx, &my);
			glfwSetCursorPos(window, 0, 0);
			if (mx || my) {
				const auto mangle = glm::radians(0.022) * 1.3;
				camera.yaw(mx * mangle);
				camera.pitch(my * mangle);
			}
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
		}
		uprojMat.setMat4(projMat);
		uviewMat.setMat4(camera.viewMat());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		
		glClear(GL_DEPTH_BUFFER_BIT);
		crossOnScreen.update(modelMat, glm::lookAt(camera.backward() * 3.0f, {0,0,0}, camera.up()), projMat);
		crossOnScreen.draw();
		crossOn00.update(modelMat, camera.viewMat(), projMat);
		crossOn00.draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}