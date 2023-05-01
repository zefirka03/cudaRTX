#include "Quad.h"

Shader Quad::shader =  Shader();

Quad::Quad(double width, double height) {
	this->width = width;
	this->height = height;

	std::vector<GLfloat> quad = {
		 1,  1,
		-1,  1,
		-1, -1,
		 1,  1,
		 1, -1,
		-1, -1
	};
	std::vector<GLfloat> tx_coord = {
		 1,  1,
		0,  1,
		0, 0,
		 1,  1,
		 1, 0,
		0, 0
	};


	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER ,vbo_id);
	glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(GLfloat), quad.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &tx_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tx_vbo);
	glBufferData(GL_ARRAY_BUFFER, tx_coord.size() * sizeof(GLfloat), tx_coord.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tx_vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &tx);
	glBindTexture(GL_TEXTURE_2D, tx);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Quad::init() {
	std::string vrtx = "#version 330 core\n"

		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 tex_coord;\n"
		"out vec2 tex_out;\n"
		"void main()\n"
		"{\n"
		"		gl_Position = vec4(position.x, position.y, 0, 1.0);\n"
		"	    tex_out = tex_coord;\n"
		"};\n";

	std::string frgm = "#version 330 core\n"
		"uniform sampler2D txtre;\n"
		"in vec2 tex_out;\n"
		"out vec4 color;\n"

		"void main()\n"
		"{\n"
		"color = texture(txtre, vec2(tex_out.x, 1-tex_out.y));\n"
		"};\n";
	shader.init();
	shader.setupFromString(vrtx, frgm);
}

void Quad::setSize(double width, double height) {
	this->width = width;
	this->height = height;
}

uc* Quad::getBuffer() {
	return texture_buffer;
}

glm::vec2 Quad::getSize() {
	return glm::vec2(width, height);
}

void Quad::setTexture(uc* buff) {
	glBindTexture(GL_TEXTURE_2D, this->tx);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buff);
	glGenerateMipmap(GL_TEXTURE_2D);
	texture_buffer = new uc[width * height * 3];
	memcpy(texture_buffer, buff, width * height * sizeof(uc) * 3);
}

void Quad::draw() {
	glBindTexture(GL_TEXTURE_2D, this->tx);
	shader.use();
	glBindVertexArray(this->vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

Quad::~Quad() {

}