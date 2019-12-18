#include "config.h"
//#include "exampleapp.h"
#include "Shader.h"
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


Shader::Shader() {

}

Shader::~Shader()
{
}

void Shader::shader(GLchar* shaderSource, ShaderType shadertype){
	Shader::shaderHandle = 0;
	if (shadertype == vertex) {
		Shader::shaderHandle = glCreateShader(GL_VERTEX_SHADER);
	}
	else {
		Shader::shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	}
	GLint length = (GLint)std::strlen(shaderSource);
	glShaderSource(Shader::shaderHandle, 1, &shaderSource, &length);
	glCompileShader(Shader::shaderHandle);
	GLint shaderLogSize;
	glGetShaderiv(Shader::shaderHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(Shader::shaderHandle, shaderLogSize, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}
	Shader::shadertype = shadertype;								
}

GLuint Shader::getShaderHandle() {
	return Shader::shaderHandle;
}

ShaderType Shader::getShaderType() {
	return Shader::shadertype;
}


