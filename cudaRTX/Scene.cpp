#include "Scene.h"
#include "CudaRTXSolver.h"
Scene::Scene() {
	cudaMalloc((void**)&objects, 20 * sizeof(Object*));
}
void Scene::addObject(Object3D& obj) {
	int* d_indeces;
	glm::vec3* d_verteces;
	cudaMalloc((void**)&d_indeces, sizeof(int) * obj.num_indeces);
	cudaMalloc((void**)&d_verteces, sizeof(glm::vec3) * obj.num_vert);

	cudaMemcpy(d_indeces, &obj.indeces, sizeof(int) * obj.num_indeces, cudaMemcpyHostToDevice);
	cudaMemcpy(d_verteces, &obj.verteces, sizeof(glm::vec3) * obj.num_vert, cudaMemcpyHostToDevice);

	host_addObjectToScene(objects, obj.num_indeces, obj.num_vert, d_indeces, d_verteces);
	num++;
}
Scene::~Scene() {};