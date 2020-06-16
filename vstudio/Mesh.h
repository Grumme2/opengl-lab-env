#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.

	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include <vector>
#include "glm/glm.hpp"

class Mesh : public Core::App
{
public:
		/// constructor
	Mesh();
		/// destructor
	~Mesh();

	void objectsarr(std::vector<glm::vec3> vertexaData);

	void BindMesh();
	/// open app
		//bool Open();
		/// run app
	std::vector<GLfloat> vec3ToGlfloat(std::vector<glm::vec3> points);
	void setupmesh();

	int getvertexbuffersize();
	int getindexbuffersize();
	int getvertexarraysize();

private:
	int vertexbuffersize;
	int indexbuffersize;
	int vertexarraysize;
	GLuint program;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	GLuint vertexArrayObject;

	std::vector<GLfloat> vertdata;
	std::vector<GLfloat> nodup;
	std::vector<unsigned int> indbuff;


	Display::Window* window;
};
#pragma once