#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <sstream>
#include <limits>

#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <curand_kernel.h>
#include <stdio.h>

#define GLM_FORCE_CUDA
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtx/vector_angle.hpp>
#include <gtc/matrix_transform.hpp>

#define toRad(x) x * (3.141592f/180.0f)


#define uc unsigned char
#define pi 3.1415926535f

