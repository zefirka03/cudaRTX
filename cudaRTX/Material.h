#pragma once
#include "generalIncludes.h"

class Material{
public:
	__device__ Material() {};
	__device__ Material(glm::vec3 _color) : color(_color) {};
	glm::vec3 color = glm::vec3(1, 1, 1);
	int type = 0;
	float n=1.01f;
};

class Diffuse : public Material {
public:
	__device__ Diffuse() { 
		type = 0; 
	};
	__device__ Diffuse(glm::vec3 _color)   {
		color = _color;
		type = 0;
	};
};

class Reflection : public Material {
public:
	__device__ Reflection() {
		type = 1;
	};
	__device__ Reflection(glm::vec3 _color) {
		color = _color;
		type = 1;
	};
};

class Optical : public Material {
public:
	__device__ Optical() {
		type = 2;
	};
	__device__ Optical(float _n) {
		type = 2;
		n = _n;
	};
	__device__ Optical(glm::vec3 _color) {
		color = _color;
		type = 2;
	};
};