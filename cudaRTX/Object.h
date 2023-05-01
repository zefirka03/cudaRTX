#pragma once
#include "generalIncludes.h"
#include "ray.h"
#include "Material.h"
class Object;

struct hitData {
	 __device__ hitData() {};
	 __device__ hitData(bool _isHit) : isHit(_isHit) {};
	bool isHit = false;
	glm::vec3 h_normal, h_point;
	double h_distance;
};

class Object {
public:
	__device__ virtual hitData hit(ray& _ray) {};
	__host__ __device__ virtual void update() {};
	glm::vec3 pos = glm::vec3(-1);
	Material* mat;
private:
};