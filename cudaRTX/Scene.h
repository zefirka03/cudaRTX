#pragma once
#include "generalIncludes.h"
#include "Object.h"
#include "Object3D.h"


class Scene {
public:
	Scene();
	void addObject(Object3D& obj);
	~Scene() ;
	int num = 0;
	Object** objects;
private:
};