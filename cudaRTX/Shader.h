#pragma once
#include "generalIncludes.h"

class Shader{
public:
	Shader();
	void init();
	void setupFromString(std::string vertex_string, std::string fragment_string);
	void use();
	~Shader();
private:
	GLuint vertex_id, fragment_id, program_id;
	bool inited = false;
};

