#include "config.h"
//#include "exampleapp.h"
#include "SimpleMaterial.h"
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

#include "Shader.h"
#include <fstream>

using namespace std;


SimpleMaterial::SimpleMaterial(glm::mat4 transform, GLfloat Colour[3]) {
	GLchar* vs =
		"#version 430\n"
		"precision mediump float;\n"
		"layout(location=0) in vec3 pos;\n"
		"out vec3 vertexPosition;"
		"uniform mat4 transformMatrix;\n"
		"uniform mat4 translationMatrix;\n"
		"void main()\n"
		"{\n"
		"gl_Position = transformMatrix * translationMatrix * vec4(pos, 1.0f);\n"
		"vertexPosition =  pos;"
		"}\n";
	//"gl_Position = vec4(0.5f, 0.5f, 0.5f, -1.0f);\n"
	//"gl_Position = vec4(pos.x, pos.y, pos.z, -1);\n"
	GLchar* ps =
		"#version 430\n"
		"precision mediump float;\n"
		"uniform vec3 cameraPosition;"
		"in vec3 vertexPosition;"
		"uniform vec3 color;\n"
		"uniform mat4 transformMatrix2;\n"
		"uniform mat4 translationMatrix2;\n"
		"out vec4 Color;\n"
		"void main()\n"
		"{\n"
		"	vec4 position = transformMatrix2 * translationMatrix2 * vec4(vertexPosition, 1.0f); \n"
		"	float distance = ((cameraPosition.x - position.x) * (cameraPosition.x - position.x)) + ((cameraPosition.y - position.y) * (cameraPosition.y - position.y)) + ((cameraPosition.z - position.z) * (cameraPosition.z - position.z));"
		"	distance = 1/distance;"
		"	Color = vec4(color[0]-(color[0]/distance), color[1]-(color[1]/distance),color[2]-(color[2]/distance), 1.0f);\n"
		"}\n";

		//"	Color = vec4(0.5f,0.5f,0.5f,0.5f);\n"
		//"float distance = length(cameraPosition, vertexPosition);"
		//"	Color = vec4(1,1,1,1);\n"
		//"float distance =((cameraPosition.x-vertexPosition.x) * (cameraPosition.x-vertexPosition.x))+((cameraPosition.y-vertexPosition.y) * (cameraPosition.y-vertexPosition.y))+((cameraPosition.z-vertexPosition.z) * (cameraPosition.z-vertexPosition.z));"
		//"Color = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0f);\n"
	//SimpleMaterial::colour = Colour;
	ShaderType stv = vertex;
	ShaderType stf = fragment;

	Shader* fragmentshader = new Shader();
	Shader* vertexshader = new Shader();

	fragmentshader->shader(ps, stf);
	vertexshader->shader(vs, stv);

	std::vector<Shader*> test = {
		vertexshader,
		fragmentshader,
		
	};

	ShaderProgram* sp = new ShaderProgram(test);

	sp->shaderbinder();

	glUseProgram(sp->getProgram());
	GLuint Location4mat = glGetUniformLocation(sp->getProgram(), "color");

	glm::vec3 model = glm::vec3(Colour[0], Colour[1], Colour[2]);
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(Location4mat, 1, glm::value_ptr(model));


	Location4mat = glGetUniformLocation(sp->getProgram(), "transformMatrix");

	glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));

	Location4mat = glGetUniformLocation(sp->getProgram(), "transformMatrix2");

	glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniform4fv(Location4mat, 1, glm::value_ptr(transform));

	glUseProgram(0);

	SimpleMaterial::shaderProgram = sp;



}

void SimpleMaterial::changeColour(GLfloat Colour[3]) {
	glUseProgram(SimpleMaterial::shaderProgram->getProgram());
	GLuint Location4mat = glGetUniformLocation(SimpleMaterial::shaderProgram->getProgram(), "color");

	glm::vec3 model = glm::vec3(Colour[0], Colour[1], Colour[2]);
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(Location4mat, 1, glm::value_ptr(model));
	glUseProgram(0);

	SimpleMaterial::shaderProgram->shaderbinder();
}


void SimpleMaterial::changeTransform(glm::mat4 transform) {
	glUseProgram(SimpleMaterial::shaderProgram->getProgram());
	GLuint Location4mat = glGetUniformLocation(SimpleMaterial::shaderProgram->getProgram(), "transformMatrix");

	glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));

	GLuint Location4mat2 = glGetUniformLocation(SimpleMaterial::shaderProgram->getProgram(), "transformMatrix2");

	glUniformMatrix4fv(Location4mat2, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));
	glUseProgram(0);

	//SimpleMaterial::shaderProgram->shaderbinder();
}


void SimpleMaterial::changeTranslation(glm::mat4 transform) {
	glUseProgram(SimpleMaterial::shaderProgram->getProgram());
	GLuint Location4mat = glGetUniformLocation(SimpleMaterial::shaderProgram->getProgram(), "translationMatrix");

	glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));

	GLuint Location4mat2 = glGetUniformLocation(SimpleMaterial::shaderProgram->getProgram(), "translationMatrix2");

	glUniformMatrix4fv(Location4mat2, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));
	glUseProgram(0);

	//SimpleMaterial::shaderProgram->shaderbinder();
}


void SimpleMaterial::changefscamerapos(glm::vec3 transform) {
	glUseProgram(SimpleMaterial::shaderProgram->getProgram());
	GLuint Location4mat = glGetUniformLocation(SimpleMaterial::shaderProgram->getProgram(), "cameraPosition");

	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform3fv(Location4mat, 1, glm::value_ptr(transform));
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(transform));
	glUseProgram(0);

	//SimpleMaterial::shaderProgram->shaderbinder();
}

SimpleMaterial::~SimpleMaterial()
{
}

void SimpleMaterial::ApplyMaterial() {
	SimpleMaterial::shaderProgram->activateshader();

}

