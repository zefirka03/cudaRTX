#include "generalIncludes.h"
#include "CudaRTXSolver.h"

__global__ void init(int width, int height, curandState* randStateuc, uc* out_image);
__global__ void createWorld(Object** objects, int num_ind, int num_vert, int* d_indeces, glm::vec3* d_verteces);
__global__ void cudaRTXSolveScene(uc* out_image, int m_x, int m_y, Camera* cam, Object** rs, curandState* randState, int format, int qual, int obj_num);
__global__ void addObject_to_scene(Object** scene_obj, int num_ind, int num_vert, int* d_indeces, glm::vec3* d_verteces);



const int t_x = 16, t_y = 16;
int obj_num = 0;
int width, height;
Object** d_objects;
curandState* d_randState;
dim3 blocks;
dim3 threads;
uc* out_image;

void RTX_Init(int _width, int _height, Object3D& obj) {
	width = _width;
	height = _height;

	cudaDeviceSynchronize();
	cudaDeviceReset();

	cudaMallocManaged((void**)&out_image, width * height * 3 * sizeof(uc));

	cudaMalloc((void**)&d_randState, width * height * sizeof(curandState));
	cudaMalloc((void**)&d_objects, (obj.num_indeces / 3) * sizeof(Object*));

	glm::vec3* d_verteces;
	int* d_indeces;
	cudaMalloc((void**)&d_indeces, sizeof(int) * obj.num_indeces);

	cudaMalloc((void**)&d_verteces, sizeof(glm::vec3) * obj.num_vert);
	cudaMemcpy(d_indeces, obj.indeces, sizeof(int) * obj.num_indeces, cudaMemcpyHostToDevice);
	cudaMemcpy(d_verteces, obj.verteces, sizeof(glm::vec3) * obj.num_vert, cudaMemcpyHostToDevice);
	obj_num = obj.num_indeces / 3;

	createWorld << <1, 1 >> > (d_objects, obj.num_indeces, obj.num_vert, d_indeces, d_verteces);
	cudaDeviceSynchronize();

	blocks = dim3(width / t_x + 1, height / t_y + 1);
	threads = dim3(t_x, t_y);

	init << < blocks, threads >> > (width, height, d_randState, out_image); //очень долго... Не работает, если t_x = t_y < 32
	cudaDeviceSynchronize();
}

uc* RTXSolveScene(Camera& camera, int format, int qual) {
	Camera* d_camera;

	cudaMalloc((void**)&d_camera, sizeof(Camera));
	cudaMemcpy(d_camera, &camera, sizeof(Camera), cudaMemcpyHostToDevice);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	cudaRTXSolveScene << <blocks, threads >> > (out_image, width, height, d_camera, d_objects, d_randState, format, qual, obj_num);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	float tm;
	cudaEventElapsedTime(&tm, start, stop);
	std::cout << tm << " ms / fps " << 1000.f / tm << '\n';
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cudaDeviceSynchronize();

	cudaFree(d_camera);

	return out_image;
}
void host_addObjectToScene(Object** scene_obj, int num_ind, int num_vert, int* d_indeces, glm::vec3* d_verteces) {
	addObject_to_scene << <1, 1 >> > (scene_obj, num_ind, num_vert, d_indeces, d_verteces);
}
#define RANDVEC3  glm::vec3(curand_uniform(local_rand_state),curand_uniform(local_rand_state),curand_uniform(local_rand_state))

__device__ glm::vec3 random_in_unit_sphere(curandState* local_rand_state) {
	glm::vec3 p;
	p = 2.0f * RANDVEC3 - glm::vec3(1);
	return glm::normalize(p);
}

__device__  glm::vec3 color(const ray& r, Object** world, curandState* local_rand_state, int obj_num) {
	ray cur_ray = r;
	float cur_attenuation = 1.0f;
	Object* last_hitted = nullptr;
	Material curMat;
	glm::vec3 end_color;
	float n[5] = { 1,1,1,1,1 };
	int n_p = 0;

	for (int i = 0; i < 10; i++) {
		hitData rec(false);
		double min_dist = 1.79769e+308;
		for (int i = 0; i < obj_num; i++) {
			if (*(world + i) != last_hitted) {
				hitData cur_data = (*(world + i))->hit(cur_ray);
				if (min_dist > cur_data.h_distance && cur_data.isHit) {
					min_dist = cur_data.h_distance;
					last_hitted = *(world + i);
					rec = cur_data;
				}
			}
		}
		
		if (rec.isHit){//ЦВЕТ
			if (cur_attenuation == 1.0f) {
				curMat = *(last_hitted->mat);
				end_color = (*last_hitted->mat).color;
			}
			
			if (last_hitted->mat->type == 0) {
				cur_attenuation *= 0.5f;
				cur_ray = ray(rec.h_point + glm::vec3(0.001f) * rec.h_normal,
					rec.h_point + glm::vec3(1.0f) * rec.h_normal + glm::vec3(1.0f) * random_in_unit_sphere(local_rand_state));
			}
			else if (last_hitted->mat->type == 1) {
				cur_attenuation *= 0.5f;
				glm::vec3 dir = glm::normalize(cur_ray.to - cur_ray.from);
				cur_ray = ray(rec.h_point + glm::vec3(0.001f) * rec.h_normal,
					rec.h_point + glm::vec3(0.001f) * rec.h_normal + glm::normalize(-glm::vec3(2.f) * glm::dot(dir, rec.h_normal) * rec.h_normal + dir));
			}
			else if (last_hitted->mat->type == 2) {
				glm::vec3 dir = glm::normalize(cur_ray.to - cur_ray.from);
				float cs = glm::dot(dir, rec.h_normal);
				if (cs < 0) {
					float ang = glm::acos(cs);
					float sin2 = (n[n_p] / last_hitted->mat->n) * glm::sin(ang);
					
					if (sin2 < 1) {

						n_p++;
						n[n_p] = last_hitted->mat->n;
						float ang2 = glm::asin(sin2);
						cur_ray = ray(rec.h_point - glm::vec3(0.001f) * rec.h_normal,
							rec.h_point - glm::vec3(0.001f) * rec.h_normal + glm::normalize(glm::rotate(glm::vec3(-1) * rec.h_normal, -ang2, glm::cross(rec.h_normal, dir))));
					}
					else {
						cur_ray = ray(rec.h_point - glm::vec3(0.001f) * rec.h_normal,
							rec.h_point - glm::vec3(0.001f) * rec.h_normal + glm::normalize(glm::dot(dir, glm::vec3(-1) * rec.h_normal) * glm::vec3(-1) * rec.h_normal + dir));
					}
				}
				else {
					float ang = glm::acos(cs);
					float sin2 = (n[n_p] / n[n_p - 1]) * glm::sin(ang);
					
					if (sin2 < 1) {
						cur_attenuation *= 0.8f;
						n[n_p] = 1;
						n_p--;
						float ang2 = glm::asin(sin2);
						cur_ray = ray(rec.h_point + glm::vec3(0.001f) * rec.h_normal,
							rec.h_point + glm::vec3(0.001f) * rec.h_normal + glm::normalize(glm::rotate(rec.h_normal, ang2, glm::cross(rec.h_normal, dir))));
					}
					else {
						cur_ray = ray(rec.h_point + glm::vec3(0.001f) * rec.h_normal,
							rec.h_point + glm::vec3(0.001f) * rec.h_normal + glm::normalize(-glm::vec3(2.f) * glm::dot(dir, rec.h_normal) * rec.h_normal + dir));
					}
				}
				
			}
			if (curMat.type == 1) {
				end_color = (end_color + (*last_hitted->mat).color * cur_attenuation) / 2.f;
			}
		}
		else {
			glm::vec3 unit_direction = glm::normalize(cur_ray.to - cur_ray.from);
			float t = 0.5f * (unit_direction.y + 1.0f);
			glm::vec3 c = (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
			if (cur_attenuation != 1)
				return cur_attenuation * end_color;
			else return glm::vec3(1, 1, 1);
		}
	}
	return cur_attenuation * end_color;
}

__global__ void init(int width, int height, curandState* randState, uc* out_image) {
	int _x = threadIdx.x + blockDim.x * blockIdx.x,
		_y = threadIdx.y + blockDim.y * blockIdx.y;
	if ((_x >= width) || (_y >= height)) return;
	int pixel_id = _y * width + _x;
	curand_init(1337, pixel_id, 0, &randState[pixel_id]);
	out_image[pixel_id + 0] = 0;
	out_image[pixel_id + 1] = 0;
	out_image[pixel_id + 2] = 0;
}

__global__ void createWorld(Object** objects, int num_ind, int num_vert, int* d_indeces, glm::vec3* d_verteces) {
	 if (threadIdx.x == 0 && blockIdx.x == 0) {

		for (int i = 0; i < num_ind / 3; i++) {
		 *(objects + i) = new renderTriangle(d_verteces[d_indeces[i * 3] - 1], d_verteces[d_indeces[i * 3 + 1] - 1], d_verteces[d_indeces[i * 3 + 2] - 1]);
		 (*(objects + i))->mat = new Diffuse(glm::vec3(0.8, 0.8, 0.8));
		}
		
		//*(objects + 0) = new renderTriangle(glm::vec3(-5, 0, -5), glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5) );
		//(*(objects + 0))->mat = new Diffuse(glm::vec3(1,1,1));
		//*(objects + 1) = new renderTriangle(glm::vec3(5, 0, -5), glm::vec3(-5, 0, -5), glm::vec3(5, 0, 5) );
		//(*(objects + 1))->mat =new Diffuse(glm::vec3(1,1,1));
		//*(objects + 2) = new renderSphere(3);
		//(*(objects + 2))->pos = glm::vec3(0,3,0);
		//(*(objects + 2))->mat = new Optical(1.01f);

	 }
}

__global__ void cudaRTXSolveScene(uc* out_image, int m_x, int m_y, Camera* cam, Object** rs, curandState* randState, int format, int qual, int obj_num) {
	int _x = threadIdx.x + blockDim.x * blockIdx.x,
		_y = threadIdx.y + blockDim.y * blockIdx.y;
	if ((_x >= m_x) || (_y >= m_y)) return;
	
	// Расчет луча к пикселю
	glm::vec3 cam_right = glm::cross(cam->cam_front, cam->cam_up);
	double sc_u = (2 * cam->min_dist * tan((cam->fov / 2.f) / 180.f * pi)) / m_x;
	ray r;
	r.from = cam->cam_pos;
	r.to = cam->cam_pos + cam->min_dist * cam->cam_front + 
			(float)(sc_u * ((-m_x / 2) + _x))* cam_right + 
			(float)(sc_u * ((m_y / 2) - _y)) * cam->cam_up;
	//
	int pix_id = (_y * m_x + _x) * 3;

	curandState local_rand_state = randState[pix_id / 3];

	// Собсна цвет

	if (format == RTX_RENDER_COLOR) {
		glm::vec3 col(0, 0, 0);
		for (int s = 0; s < qual; s++) {
			//printf("%d\n", (int)100 * ((float)s / qual));
			col += color(r, rs, &local_rand_state, obj_num);
		}
		col /= float(qual);
		col.x = sqrt(col.x);
		col.y = sqrt(col.y);
		col.z = sqrt(col.z);
		out_image[pix_id + 0] = 255 * col.x;
		out_image[pix_id + 1] = 255 * col.y;
		out_image[pix_id + 2] = 255 * col.z;
	}
	else if (format == RTX_RENDER_NORMALS) {
		hitData rec;
		double min_dist = 1.79769e+308;
		clock_t a, b;

		
		for (int i = 0; i < obj_num; i++) {
			hitData cur_data = (*(rs + i))->hit(r);
			if (min_dist > cur_data.h_distance && cur_data.isHit) {
				min_dist = cur_data.h_distance;
				rec = cur_data;
			}
		}

		if (rec.isHit) {
			out_image[pix_id + 0] = 127.5f * (1 + rec.h_normal.x);
			out_image[pix_id + 1] = 127.5f * (1 + rec.h_normal.y);
			out_image[pix_id + 2] = 127.5f * (1 + rec.h_normal.z);
		}
		else {
			out_image[pix_id + 0] = 255;
			out_image[pix_id + 1] = 255;
			out_image[pix_id + 2] = 255;
		}
	}

	//
}

__global__ void addObject_to_scene(Object** scene_obj, int num_ind, int num_vert, int* d_indeces, glm::vec3* d_verteces) {
	
}