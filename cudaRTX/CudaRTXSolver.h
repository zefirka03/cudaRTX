#pragma once
#include "generalIncludes.h"

#include "renderSphere.h"
#include "renderTriangle.h"

#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Object3D.h"

#define RTX_RENDER_NORMALS	000
#define RTX_RENDER_COLOR	001

void RTX_Init(int _width, int _height, Object3D& obj);
uc* RTXSolveScene(Camera& camera, int format, int qual);
void host_addObjectToScene(Object** scene_obj, int num_ind, int num_vert, int* d_indeces, glm::vec3* d_verteces);