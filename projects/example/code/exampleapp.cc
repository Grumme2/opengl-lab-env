//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2017 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
//#include <bits/stdc++.h>
#include "imgui.h"
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


const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 rotationMatrix;\n"
"void main()\n"
"{\n"
	"gl_Position = rotationMatrix * vec4(pos.x,pos.y,pos.z,1.0f);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/

std::vector<GLfloat> kochSnowFlake;
std::vector<GLfloat> kochSnowFlake2;
size_t bufferSize;
size_t bufferSize2;
GLfloat sqrt3 = 1.73205080757;
GLfloat* buf;
GLfloat* buf2;
GLfloat angle2 = 0;
int rect = 2; 
int refreshmill = 1;
bool lineBool =true ;
bool fillBool = true;
bool rotationbool = true;


/* 
Recursive function that calculates the points for a side in koch snowflake.
Returns every point except the right most point to make sure no points at
the same place
*/
std::vector<GLfloat> edgePointsLine(int rec, glm::vec2 p1, glm::vec2 p2, glm::vec2 b) {

	glm::vec2 third1 = glm::vec2((2 * p1.x + p2.x) / 3, (2 * p1.y + p2.y) / 3);
	glm::vec2 third2 = glm::vec2((p1.x + 2 * p2.x) / 3, (p1.y + 2 * p2.y) / 3);
	glm::vec2 middle = glm::vec2((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

	glm::vec2 base1 = glm::vec2((2 * p1.x + b.x) / 3, (2 * p1.y + b.y) / 3);
	glm::vec2 base2 = glm::vec2((2 * p2.x + b.x) / 3, (2 * p2.y + b.y) / 3);


	float dist = glm::distance(p1, p2);
	float pointheight = dist * (sqrt3 / 2.0f)*(1.0f / 3.0f);

	glm::vec2 mc = glm::vec2((middle.x - b.x), (middle.y - b.y));
	glm::vec2 mcnorm = glm::normalize(mc);
	glm::vec2 scalemcn = mcnorm * pointheight;
	glm::vec2 peak = glm::vec2((scalemcn.x + middle.x), (scalemcn.y + middle.y));

	if (rec == 2) {
		std::vector<GLfloat> calcpoints = {
		p1.x,	p1.y,	-1,
		0,	0,	0,	 0,
		third1.x,	third1.y,	-1,
		0,	0,	0,	 0,
		peak.x,	peak.y,	-1,
		0,	0,	0,	 0,
		third2.x,	third2.y,	-1,
		0,	0,	0,	 0
		};
		return  calcpoints;
	}

	std::vector<GLfloat> edge1points = edgePointsLine(rec - 1, p1, third1, base1);
	std::vector<GLfloat> edge2points = edgePointsLine(rec - 1, third1, peak, third2);
	std::vector<GLfloat> edge3points = edgePointsLine(rec - 1, peak, third2, third1);
	std::vector<GLfloat> edge4points = edgePointsLine(rec - 1, third2, p2, base2);

	edge1points.insert(edge1points.end(), edge2points.begin(), edge2points.end());
	edge1points.insert(edge1points.end(), edge3points.begin(), edge3points.end());
	edge1points.insert(edge1points.end(), edge4points.begin(), edge4points.end());

	return edge1points;
}

/*
Returns a single triangle with recusrsive depth 1 otherwise calls a function for each side 
that calculates the points for koch snowflake and concatinates the results from those function calls 
*/
std::vector<GLfloat> kochSnowFlakeLine(int rec) {
	glm::vec2 p1 = glm::vec2(-0.866 / 2.0f, -0.5f / 2.0f);
	glm::vec2 p2 = glm::vec2(0.866 / 2.0f, -0.5f / 2.0f);
	glm::vec2 p3 = glm::vec2(0.0, 1.0 / 2.0f);
	if (rec == 1){
		std::vector<GLfloat> fin= {
		p1.x,	p1.y,	-1,
		0,	0,	0,	 0,
		p2.x,	p2.y,	-1,
		0,	0,	0,	 0,
		p3.x,	p3.y,	-1,
		0,	0,	0,	 0
		};
		return  fin;
	}

	std::vector<GLfloat> edge1points = edgePointsLine(rec, p1, p2, p3);
	std::vector<GLfloat> edge2points = edgePointsLine(rec, p2, p3, p1);
	std::vector<GLfloat> edge3points = edgePointsLine(rec, p3, p1, p2);

	edge1points.insert(edge1points.end(), edge2points.begin(), edge2points.end());
	edge1points.insert(edge1points.end(), edge3points.begin(), edge3points.end());

	return edge1points;
}


/* 
Recursive function that calculates the points for a side in kosh snowflake.
Returns every point except the points sent in to the function.
*/
std::vector<GLfloat> edgePointsFill(int rec, glm::vec2 p1, glm::vec2 p2, glm::vec2 b) {

	glm::vec2 third1 = glm::vec2((2 * p1.x + p2.x) / 3, (2 * p1.y + p2.y) / 3);
	glm::vec2 third2 = glm::vec2((p1.x + 2 * p2.x) / 3, (p1.y + 2 * p2.y) / 3);
	glm::vec2 middle = glm::vec2((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

	glm::vec2 base1 = glm::vec2((2 * p1.x + b.x) / 3, (2 * p1.y + b.y) / 3);
	glm::vec2 base2 = glm::vec2((2 * p2.x + b.x) / 3, (2 * p2.y + b.y) / 3);

	float dist = glm::distance(p1, p2);
	float pointheight = dist * ((sqrt3) / 2.0f)*(1.0f / 3.0f);

	glm::vec2 mc = glm::vec2((middle.x - b.x), (middle.y - b.y));
	glm::vec2 mcnorm = glm::normalize(mc);
	glm::vec2 scalemcn = mcnorm * pointheight;
	glm::vec2 peak = glm::vec2((scalemcn.x + middle.x), (scalemcn.y + middle.y));

	std::vector<GLfloat> edgePoints = {
		third1.x,	third1.y,	-1,
		1,	0,	0,	1,
		peak.x,	peak.y,	-1,
		1,	0,	0,	 1,
		third2.x,	third2.y,	-1,
		1,	0,	0,	 1
	};

	if (rec == 2) {
		return  edgePoints;
	}

	std::vector<GLfloat> edge1points = edgePointsFill(rec - 1, p1, third1, base1);
	std::vector<GLfloat> edge2points = edgePointsFill(rec - 1, third1, peak, third2);
	std::vector<GLfloat> edge3points = edgePointsFill(rec - 1, peak, third2, third1);
	std::vector<GLfloat> edge4points = edgePointsFill(rec - 1, third2, p2, base2);

	edgePoints.insert(edgePoints.end(), edge1points.begin(), edge1points.end());
	edgePoints.insert(edgePoints.end(), edge2points.begin(), edge2points.end());
	edgePoints.insert(edgePoints.end(), edge3points.begin(), edge3points.end());
	edgePoints.insert(edgePoints.end(), edge4points.begin(), edge4points.end());

	return edgePoints;
}


/*
Returns a single triangle with recusrsive depth 1 otherwise calls a function for each side
that calculates the points for koch snowflake and concatinates the results from those function calls.
*/
std::vector<GLfloat> kochSnowFlakeFill(int rec) {
	glm::vec2 p1 = glm::vec2(-0.866 / 2.0f, -0.5f / 2.0f);
	glm::vec2 p2 = glm::vec2(0.866 / 2.0f, -0.5f / 2.0f);
	glm::vec2 p3 = glm::vec2(0.0, 1.0 / 2.0f);

	std::vector<GLfloat> snowflake = {
		p1.x,	p1.y,	-1,
		1,	0,	0,	 0,
		p2.x,	p2.y,	-1,
		1,	0,	0,	 0,
		p3.x,	p3.y,	-1,
		1,	0,	0,	 0
	};

	if (rec == 1) {
		return  snowflake;
	}

	std::vector<GLfloat> edge1points = edgePointsFill(rec, p1, p2, p3);

	std::vector<GLfloat> edge2points = edgePointsFill(rec, p2, p3, p1);

	std::vector<GLfloat> edge3points = edgePointsFill(rec, p3, p1, p2);

	snowflake.insert(snowflake.end(), edge1points.begin(), edge1points.end());
	snowflake.insert(snowflake.end(), edge2points.begin(), edge2points.end());
	snowflake.insert(snowflake.end(), edge3points.begin(), edge3points.end());

	return snowflake;

}

/**
Rotates my koch snowflake
*/
void rotate(GLuint programcurr,GLfloat angle2) {
	glUseProgram(programcurr);
	GLuint Location4mat = glGetUniformLocation(programcurr,"rotationMatrix");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(Location4mat, 1, GL_FALSE, glm::value_ptr(model));
	glUseProgram(0);
}
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	this->window->SetSize(1500, 1500);
	window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32)
	{
		if ((GLFW_KEY_UP == key) &&( action == GLFW_RELEASE)) {
			if (rect < 7) {
				rect++;
			}
		}
		else if ((GLFW_KEY_DOWN == key)&&( action == GLFW_RELEASE)) {
			if (rect > 1) {
				rect--;
			}
		}
		else if ((GLFW_KEY_E == key) && (action == GLFW_RELEASE)) {
			lineBool = !lineBool;
		}
		else if ((GLFW_KEY_F == key) && (action == GLFW_RELEASE)) {
			fillBool = !fillBool;
		}
		else if ((GLFW_KEY_ESCAPE == key) && (action == GLFW_RELEASE)) {
			this->window->Close();
		}
	});

	if (fillBool == true) {
		kochSnowFlake = kochSnowFlakeFill(rect);
	}
	if (lineBool == true) {
		kochSnowFlake2 = kochSnowFlakeLine(rect);
		int siizenr = kochSnowFlake2.size();
		printf("number of points:  %d number of rec %d", siizenr, rect);
	}
	
	GLfloat* buf = &kochSnowFlake[0];
	bufferSize = (kochSnowFlake.size());

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


		this->vsBuffer = new GLchar[STRING_BUFFER_SIZE];
		this->fsBuffer = new GLchar[STRING_BUFFER_SIZE];

		// copy the hardcoded shader into buffer
		strncpy_s(this->vsBuffer, STRING_BUFFER_SIZE, vs, STRING_BUFFER_SIZE);
		strncpy_s(this->fsBuffer, STRING_BUFFER_SIZE, ps, STRING_BUFFER_SIZE);

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = (GLint)std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		GLint InfoLogLength;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(this->vertexShader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("[info log length part]%s\n", &VertexShaderErrorMessage[0]);
		}
		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = (GLint)std::strlen(ps);
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
	
		// setup vbo
		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*kochSnowFlake.size(), buf, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});
		
		return true;
	}
	return false;
	
}

//------------------------------------------------------------------------------
/**
*/

void
ExampleApp::Run()
{
	while (this->window->IsOpen())
	{
		glLineWidth(3);
		if (fillBool == true) {
			kochSnowFlake = kochSnowFlakeFill(rect);
		}
		if (lineBool == true) {
			kochSnowFlake2 = kochSnowFlakeLine(rect);
		}


		buf = &kochSnowFlake[0];
		bufferSize = (kochSnowFlake.size());
		buf2 = &kochSnowFlake2[0];
		bufferSize2 = (kochSnowFlake2.size());	

		glClear(GL_COLOR_BUFFER_BIT);
		
		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*kochSnowFlake.size(), buf, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->Update();

		if (rotationbool) {
			rotate(this->program, (angle2));
		}

		//draw filled traingles
		if (fillBool == true) {
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glUseProgram(this->program);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));

			

			glDrawArrays(GL_TRIANGLES, 0, bufferSize / 7);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		}


		//Draw lineloop 
		if (lineBool == true) {
			glGenBuffers(1, &this->triangle);
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*kochSnowFlake2.size(), buf2, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glUseProgram(this->program);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));

			glDrawArrays(GL_LINE_LOOP, 0, bufferSize2 / 7);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		if (rotationbool) {
			angle2 = angle2 - 1;
		}
		this->window->SwapBuffers();
	}
}

void
ExampleApp::RenderUI()
{
	if (this->window->IsOpen())
	{
		bool show = true;
		// create a new window
		ImGui::Begin("Shader Sources", &show, ImGuiWindowFlags_NoSavedSettings);

		// apply button
		if (ImGui::Button("Line"))
		{
			//if pressed boolean cthat controls to draw line is toggled
			lineBool = !lineBool;
		}
		if (ImGui::Button("fill"))
		{
			//if pressed boolean cthat controls to fill triangles is toggled
			fillBool = !fillBool;
		}
		if (ImGui::Button("rotation"))
		{
			//if pressed boolean cthat controls rotation is toggled
			rotationbool = !rotationbool;
		}

		ImGui::SliderInt("recursion depth", &rect, 1, 7);

		if (this->compilerLog.length())
		{
			// if compilation produced any output we display it here
			ImGui::TextWrapped(this->compilerLog.c_str());
		}
		// close window
		ImGui::End();
	}
}

}