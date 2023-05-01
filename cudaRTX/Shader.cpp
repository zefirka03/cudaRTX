#include "Shader.h"

Shader::Shader() {
	
}

void Shader::init() {
	if (!inited) {
		vertex_id = glCreateShader(GL_VERTEX_SHADER);
		fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
		program_id = glCreateProgram();
	}
	else std::cout << "Shader is already initialized!\n";
}

void Shader::setupFromString(std::string vertex_string, std::string fragment_string) {
	const char* vertex_source = vertex_string.c_str();
	const char* fragment_source = fragment_string.c_str();

	glShaderSource(vertex_id, 1, &vertex_source, NULL);
	glCompileShader(vertex_id);

	glShaderSource(fragment_id, 1, &fragment_source, NULL);
	glCompileShader(fragment_id);

	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	glLinkProgram(program_id);
}

void Shader::use() {
	glUseProgram(program_id);
}

Shader::~Shader() {

}