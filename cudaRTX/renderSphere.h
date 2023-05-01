#pragma once
#include "generalIncludes.h"
#include "Object.h"

class renderSphere : public Object {
public:
	 __host__ __device__ renderSphere() {};
	 __host__ __device__ renderSphere(float _radius) {
		 this->radius = _radius;
	 }
	 __device__ hitData hit(ray& _ray) override {
		glm::vec3 dir = glm::normalize(_ray.to - _ray.from);
		float t = glm::dot(this->pos - _ray.from, dir);
		glm::vec3 t_p = _ray.from + t * dir;
		if (t > 0 && glm::length(t_p - this->pos) <= this->radius) {
			hitData outData(true);
			outData.h_point = t_p - sqrt(radius * radius - glm::length(this->pos - t_p) * glm::length(this->pos - t_p)) * dir;
			outData.h_normal = glm::normalize(outData.h_point - this->pos);
			outData.h_distance = glm::length(outData.h_point - _ray.from);
			return outData;
		}	
		return hitData(false);
	}
	__host__ __device__ ~renderSphere() {};
	float radius = 1;
private:
};
