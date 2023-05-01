#include "Render.h"

Render::Render(int width, int height, std::string name) {
	glfwInit();
	this->window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();
}

bool Render::ShouldClose() {
	return glfwWindowShouldClose(this->window);
}

const void Render::clear()	{
	glClear(GL_COLOR_BUFFER_BIT);
};

const void Render::display(){
	glfwSwapBuffers(this->window);
	glfwPollEvents();
};

GLFWwindow* Render::getGLFWWindow() {
	return this->window;
}

Render::~Render()	{}
