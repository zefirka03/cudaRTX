#pragma once
#include "generalIncludes.h"
#include "Shader.h"

class Quad{
public:
	Quad(double width, double height);
	void setTexture(uc* buff);
	uc* getBuffer();
	glm::vec2 getSize();
	void setSize(double width, double height);
	static void init();
	void draw();
	~Quad();
private:
	static Shader shader;
	double width, height, pos_x, pos_y;
	GLuint vbo_id, tx_vbo, vao_id;
	GLuint tx;
	GLuint vertex, fragment, program;
	uc* texture_buffer;
};

