#pragma once

#include "../utils/utils.hpp"

const std::string debugcrossvs = R"(
#version 460 core
layout (location = 0) in vec3 pos;
in vec3 vcol;
out vec3 fcol;
uniform mat4 modelMat = mat4(1);
uniform mat4 viewMat = mat4(1);
uniform mat4 projMat = mat4(1);
void main() {
	gl_Position = projMat * viewMat * modelMat * vec4(pos, 1);
	fcol = vcol;
}
)";

const std::string debugcrossfs = R"(
#version 460 core
in vec3 fcol;
out vec3 color;
void main() {
	if (length(fcol) > 0) color = fcol;
	else color = vec3(1.0, 0.0, 1.0);
}
)";

class DebugCross {
public:
	VAO vao{};
	VBO verts;
	VBO colors;
	std::shared_ptr<Program> program;
	Uniform umodelMat;
	Uniform uviewMat;
	Uniform uprojMat;
	DebugCross() {
		verts.data<glm::vec3>({
			{0, 0, 0}, {1, 0, 0},
			{0, 0, 0}, {0, 1, 0},
			{0, 0, 0}, {0, 0, 1}
		});
		colors.data<glm::vec3>({
			{1, 0, 0}, {1, 0, 0},
			{0, 1, 0}, {0, 1, 0},
			{0, 0, 1}, {0, 0, 1}
		});
		vao.attrib(verts, 0, 3);
		vao.attrib(colors, 1, 3);
		program = std::make_shared<Program>(std::vector<std::shared_ptr<Shader>>{
			std::make_shared<FragmentShader>(debugcrossfs),
			std::make_shared<VertexShader>(debugcrossvs),
		});
		umodelMat.getLocation(program, "modelMat");
		uviewMat.getLocation(program, "viewMat");
		uprojMat.getLocation(program, "projMat");
	}
	void update(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projMat) {
		umodelMat.setMat4(modelMat);
		uviewMat.setMat4(viewMat);
		uprojMat.setMat4(projMat);
	}
	void draw() {
		glUseProgram(*program);
		vao.bind();
		glDrawArrays(GL_LINES, 0, verts.size);
	}
};