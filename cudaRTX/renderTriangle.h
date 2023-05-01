#pragma once
#include "generalIncludes.h"
#include "Object.h"

class renderTriangle : public Object {
public:
	__host__ __device__ renderTriangle() {};
	__host__ __device__ renderTriangle(glm::vec3 vertex_0, glm::vec3 vertex_1, glm::vec3 vertex_2){
		vertex_pos[0] = vertex_0;
		vertex_pos[1] = vertex_1;
		vertex_pos[2] = vertex_2;
		this->update();
	}
	__host__ __device__ void update() override {
		v0v1 = vertex_pos[1] - vertex_pos[0];
		v0v2 = vertex_pos[2] - vertex_pos[0];
		normal = glm::normalize(glm::cross(v0v1, v0v2));
	}
	__device__ hitData hit(ray& _ray) override {
		hitData out(false);

		glm::vec3 dir = glm::normalize(_ray.to - _ray.from);
		double kEpsilon = 0.00001f;

		glm::vec3 pvec = glm::cross(dir, v0v2);
		float det = glm::dot(pvec, v0v1);

		if (abs(det) < kEpsilon) return out;

		float invDet = 1.f / det;
		
		glm::vec3 tvec = _ray.from - vertex_pos[0];
		float u = glm::dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return out;

		glm::vec3 qvec = glm::cross(tvec, v0v1);
		float v = glm::dot(dir, qvec) * invDet;
		if (v < 0 || u + v > 1) return out;

		float t = glm::dot(v0v2, qvec)* invDet;
		if (t <= 0) return out;

		out.isHit = true;
		out.h_distance = glm::length(dir * glm::vec3(t));
		out.h_point = dir * glm::vec3(t) + _ray.from;
		out.h_normal = normal;

		return out;
	}
private:
	glm::vec3 vertex_pos[3];
	glm::vec3 v0v1, v0v2, normal;
};