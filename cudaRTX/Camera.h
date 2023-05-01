#pragma once
#include "generalIncludes.h"

struct Camera{
public:
	Camera();
	bool update(GLFWwindow* win);
	~Camera();
	glm::vec3 cam_pos, cam_front, cam_up;
	float fov = 45.f;
	float min_dist = 0.1f;
private:
	double mouse_x = 0, mouse_y = 0;
	double yaw = 0, pitch = 0;
	double sence = 0.1f;
	bool start = true;
};

