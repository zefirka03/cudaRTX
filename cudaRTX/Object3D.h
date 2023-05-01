#pragma once
#include "generalIncludes.h"
#include <fstream>
#include <vector>

class Object3D{
public:
	Object3D() ;
	void loadFromObj(std::string path);
	~Object3D();

	int num_vert, num_indeces;
	glm::vec3* verteces;
	int* indeces;
	glm::vec3* d_verteces;
	int* d_indeces;
	
private:
};

