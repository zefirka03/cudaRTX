#pragma once
#include "generalIncludes.h"

class ray {
public:
	__device__ ray() : from(glm::vec3(0)), to(glm::vec3(0)) {};
	__device__ ray(glm::vec3 _from, glm::vec3 _to) : from(_from), to(_to) {};
	__device__ ~ray() {};
	glm::vec3 from, to;
};