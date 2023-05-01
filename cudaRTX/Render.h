#pragma once
#include "generalIncludes.h"

class Render{
public:
	Render(int width, int height, std::string name);
	bool ShouldClose();
	const void clear();
	const void display();
	GLFWwindow* getGLFWWindow();
	~Render();
private:
	GLFWwindow* window;
};

