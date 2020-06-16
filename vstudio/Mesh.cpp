#include "config.h"
//#include "exampleapp.h"
#include "Mesh.h"
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


	/*Mesh::vertexBufferObject;
	Mesh::indexBufferObject;
	Mesh::vertexArrayObject;*/

	/*GLuint vertexBufferObject;
	GLuint indexBufferObject;
	GLuint vertexArrayObject;
*/
	


	Mesh::Mesh() {

	}

	Mesh::~Mesh()
	{
	}

	void Mesh::objectsarr(std::vector<glm::vec3> vertexaData) {
		std::vector<glm::vec3> noduplicates;

		for (int i = 0; i < vertexaData.size(); i++) {
			if (std::find(noduplicates.begin(), noduplicates.end(), vertexaData.at(i)) != noduplicates.end()) {
				continue;
			}
			else {
				noduplicates.push_back(vertexaData.at(i));
			}
		}

		std::vector<unsigned int> indexbuffer;
		for (int i = 0; i < vertexaData.size(); i++) {
			auto it = std::find(noduplicates.begin(), noduplicates.end(), vertexaData.at(i));
			if (it != noduplicates.end())
			{
				unsigned int index = std::distance(noduplicates.begin(), it);
				indexbuffer.push_back(index);
			}
		}

		vertdata = vec3ToGlfloat(vertexaData);
		nodup = vec3ToGlfloat(noduplicates);
		indbuff = indexbuffer;


		vertexbuffersize = nodup.size();
		vertexarraysize = vertdata.size();
		indexbuffersize = indbuff.size();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenVertexArrays(1, &vertexArrayObject);
		glGenBuffers(1, &vertexBufferObject);
		glGenBuffers(1, &indexBufferObject);


		Mesh::BindMesh();
		Mesh::setupmesh();

	}

	std::vector<GLfloat> Mesh::vec3ToGlfloat(std::vector<glm::vec3> points) {
		std::vector<GLfloat> glfloatvec;
		for (int i = 0; i < points.size(); i++) {
			std::vector<GLfloat> glfloatvecTemp = {
				points.at(i).x,	points.at(i).y,	points.at(i).z,
			};
			glfloatvec.insert(glfloatvec.end(), glfloatvecTemp.begin(), glfloatvecTemp.end());
		}
		return glfloatvec;
	}

	void Mesh::BindMesh()
	{
		glBindVertexArray(vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 3, NULL);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 3, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	}

	void Mesh::setupmesh() {

		/*glBindVertexArray(vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
*/

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nodup.size(), &nodup[0], GL_STATIC_DRAW);

		/*glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 3, NULL);

		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);*/
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indbuff.size() * sizeof(unsigned int), &indbuff[0], GL_STATIC_DRAW);

		
	}

	int Mesh::getindexbuffersize() {
		return Mesh::indexbuffersize;
	}

	int Mesh::getvertexarraysize() {
		return Mesh::vertexarraysize;
	}

	int Mesh::getvertexbuffersize() {
		return Mesh::vertexbuffersize;
	}