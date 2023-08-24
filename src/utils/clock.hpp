#pragma once

#include <GLFW/glfw3.h>

class Clock {
public:
	double last;
	Clock(): last(glfwGetTime()) {}
	double dt() {
		double now = glfwGetTime();
		double dt = now - last;
		last = now;
		return dt;
	}
};