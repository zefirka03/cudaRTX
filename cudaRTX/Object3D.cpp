#include "Object3D.h"

Object3D::Object3D() {

}

void Object3D::loadFromObj(std::string path) {
	std::ifstream fin(path.c_str());
	std::string ss;

	std::vector<glm::vec3> vert;
	std::vector<int> ind;
	while (fin >> ss) {
		if (ss == "v") {
			float x, y, z;
			fin >> x >> y >> z;
			vert.push_back(glm::vec3(x, y, z));
		}
		else if (ss == "f") {
			int x, y, z;
			fin >> ss;
			sscanf_s(ss.c_str(), "%d/", &x);
			ind.push_back(x);
			fin >> ss;
			sscanf_s(ss.c_str(),"%d/", &y);
			ind.push_back(y);
			fin >> ss;
			sscanf_s(ss.c_str(),"%d/", &z);
			ind.push_back(z);
		}
	}
	verteces = new glm::vec3[vert.size()];
	indeces = new int[ind.size()];

	memcpy(verteces, vert.data(), vert.size() * sizeof(glm::vec3));
	memcpy(indeces, ind.data(), ind.size() * sizeof(int));

	num_indeces = ind.size();
	num_vert = vert.size();

	cudaMalloc((void**)&d_indeces, sizeof(int) * num_indeces);
	cudaMalloc((void**)&d_verteces, sizeof(glm::vec3) * num_vert);

	cudaMemcpy(d_indeces, indeces, sizeof(int) * num_indeces, cudaMemcpyHostToDevice);
	cudaMemcpy(d_verteces, verteces, sizeof(glm::vec3) * num_vert, cudaMemcpyHostToDevice);
}

Object3D::~Object3D() {

}
