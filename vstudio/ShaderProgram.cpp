#include "config.h"
//#include "exampleapp.h"
#include "ShaderProgram.h"
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
//#include <bits/stdc++.h>
#include "imgui.h"
//#include "btree.h"
#include <glm/glm.hpp>
#include <math.h> 
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
# define M_PIl          3.141592653589793238462643383279502884L
#define STRING_BUFFER_SIZE 8192

#include <fstream>
using namespace std;



ShaderProgram::ShaderProgram(vector<Shader*> shaders) {
	ShaderProgram::shaders = shaders;
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::shaderbinder() {
	GLint shaderLogSize = 0;
	auto shaderbool = GL_TRUE;
	GLint temp;

	ShaderProgram::program = glCreateProgram();

	for (int i = 0; i < ShaderProgram::shaders.size(); i++) {
		Shader shader = *ShaderProgram::shaders.at(i);
		temp = shaderLogSize;
		glGetShaderiv(shader.getShaderHandle(), GL_INFO_LOG_LENGTH, &shaderLogSize);
		glGetShaderiv(shader.getShaderHandle(), GL_COMPILE_STATUS, &shaderbool);
		glAttachShader(ShaderProgram::program, shader.getShaderHandle());
		shaderLogSize += temp;
	}
	glLinkProgram(ShaderProgram::program);
	glGetProgramiv(ShaderProgram::program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	glGetProgramiv(ShaderProgram::program, GL_LINK_STATUS, &shaderbool);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}
	
}

void ShaderProgram::activateshader() {
	glUseProgram(ShaderProgram::program);
}


GLuint ShaderProgram::getProgram() {
	return ShaderProgram::program;
}