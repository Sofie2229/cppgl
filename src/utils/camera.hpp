#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

class Camera {
public:
	Camera() {}
	Camera(glm::vec3 pos): pos(pos) {}
	Camera(glm::vec3 pos, glm::quat orientation): pos(pos), orientation(orientation) {}
	glm::quat orientation{{0, 0, 0}};
	glm::vec3 pos {0, 0, 0};
	glm::vec3 forward(bool toplane = false) {
		auto r = orientation * glm::vec3{0,1,0};
		if (toplane) {
			r.z = 0;
			r = glm::normalize(r);
		}
		return r;
	}
	glm::vec3 left(bool toplane = false) {
		auto r = orientation * glm::vec3{-1,0,0};
		if (toplane) {
			r.z = 0;
			r = glm::normalize(r);
		}
		return r;
	}
	glm::vec3 up(bool toplane = false) {
		if (toplane) return glm::vec3{0,0,1};
		return orientation * glm::vec3{0,0,1};
	}
	glm::vec3 backward(bool toplane = false) {
		return -forward(toplane);
	}
	glm::vec3 right(bool toplane = false) {
		return -left(toplane);
	}
	glm::vec3 down(bool toplane = false) {
		return -up(toplane);
	}
	// yaw, pitch, roll
	glm::vec3 angles() {
		glm::vec3 a;
		glm::extractEulerAngleZXY(glm::toMat4(orientation), a.x, a.y, a.z);
		return a;
	}

	void angles(glm::vec3 ypr) {
		glm::quat yaw = glm::angleAxis(ypr.x, glm::vec3{0, 0, 1});
		glm::quat pitch = glm::angleAxis(ypr.y, glm::vec3{1, 0, 0});
		glm::quat roll = glm::angleAxis(ypr.z, glm::vec3{0, 1, 0});
		orientation = yaw * pitch * roll;
	}
	void rotate(glm::quat q) {
		orientation = glm::conjugate(q) * orientation;
		orientation = glm::normalize(orientation);
	}
	void yaw(float angle) {
		rotate(glm::angleAxis(angle, up(true)));
	}
	void pitch(float angle) {
		rotate(glm::angleAxis(angle, right(true)));
	}
	void roll(float angle) {
		rotate(glm::angleAxis(angle, forward()));
	}
	glm::mat4 viewMat() {
		return glm::lookAt(pos, pos + forward(), up());
	}
};