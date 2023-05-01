#include "Camera.h"

Camera::Camera() {
	cam_pos = glm::vec3(0, 0, 0);
	cam_front = glm::vec3(0, 0, -1);
	cam_up = glm::vec3(0, 1, 0);
}

bool Camera::update(GLFWwindow* win) {
	double cur_mx, cur_my;
	glfwGetCursorPos(win, &cur_mx, &cur_my);

	double	delta_x = mouse_x - cur_mx,
			delta_y = mouse_y - cur_my;

	mouse_x = cur_mx;
	mouse_y = cur_my;

	if (!start) {
		yaw -= sence * delta_x;
		pitch += sence * delta_y;
	}
	else start = false;

	if (pitch > 89.f) pitch = 89.f;
	if (pitch < -89.f) pitch = -89.f;

	float	r_yaw = glm::radians(yaw),
			r_pitch = glm::radians(pitch);

	cam_front = glm::normalize(glm::vec3(	sin(r_yaw) * cos(r_pitch),
											sin(r_pitch),
											-cos(r_yaw) * cos(r_pitch)));
	cam_up = glm::normalize(glm::vec3(sin(r_yaw) * cos(pi / 2.f + r_pitch),
										sin(pi / 2.f + r_pitch),
										-cos(r_yaw) * cos(pi / 2.f + r_pitch)));

	glm::vec3 cam_right = glm::normalize(glm::cross(cam_front, cam_up));
	//cam_up = glm::normalize(glm::cross(cam_front, -cam_right));

	return (delta_x != 0) || (delta_y != 0);
}

Camera::~Camera() {

}