//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2017 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
//http://www.sm.luth.se/csee/courses/d7045e/labs/
//http://www.sm.luth.se/csee/courses/d7045e/labs/lab3-va1.pdf
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "SimpleMaterial.h"
#include "Mesh.h"
#include "GraphicsNode.h"
#include "imgui.h"
#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#define STRING_BUFFER_SIZE 8192

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	this->window = new Display::Window();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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
bool
ExampleApp::Open()
{
	App::Open();
	this->window->Open();

	/*glm::mat4 transform1 = glm::mat4(1,0,0,0,0, cos(0.785),-sin(0.785),0,0, sin(0.785),cos(0.785),0,0,0,0,1);
	glm::mat4 transform2 = glm::mat4(cos(0.785), 0, sin(0.785), 0, 0, 1, 0, 0, -sin(0.785), 0, cos(0.785), 0, 0, 0, 0, 1);
	glm::mat4 transform = transform1 * transform2;
	*///glm::mat4 transform = glm::mat4(1.0);
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::mat4 camerasettings = glm::mat4(cameraRight.x, cameraRight.y, cameraRight.z, 0, cameraUp.x, cameraUp.y, cameraUp.z, 0, cameraDirection.x, cameraDirection.y, cameraDirection.z,0,0,0,0,1);
	glm::mat4 cameraposmat = glm::mat4(1, 0, 0,-cameraPos.x,0,1,0,-cameraPos.y, 0,0,1, -cameraPos.z,0,0,0,1);
	glm::mat4 LookAt = camerasettings * cameraposmat;
	

	GLfloat Colour[3] = {1.0f, 0.0f, 0.0f};
	GLfloat Colour2[3] = { 1.0f, 0.0f, 1.0f };
	

	std::vector<glm::vec3> points = {
			glm::vec3(-0.1f, -0.1f, -0.1f), // triangle 1 : begin
			glm::vec3(-0.1f, -0.1f, 0.1f),
			glm::vec3(-0.1f, 0.1f, 0.1f), // triangle 1 : end
			glm::vec3(0.1f, 0.1f, -0.1f), // triangle 2 : begin
			glm::vec3(-0.1f, -0.1f, -0.1f),
			glm::vec3(-0.1f, 0.1f, -0.1f), // triangle 2 : end
			glm::vec3(0.1f, -0.1f, 0.1f),
			glm::vec3(-0.1f, -0.1f, -0.1f),
			glm::vec3(0.1f, -0.1f, -0.1f),
			glm::vec3(0.1f, 0.1f, -0.1f),
			glm::vec3(0.1f, -0.1f, -0.1f),
			glm::vec3(-0.1f, -0.1f, -0.1f),
			glm::vec3(-0.1f, -0.1f, -0.1f),
			glm::vec3(-0.1f, 0.1f, 0.1f),
			glm::vec3(-0.1f, 0.1f, -0.1f),
			glm::vec3(0.1f, -0.1f, 0.1f),
			glm::vec3(-0.1f, -0.1f, 0.1f),
			glm::vec3(-0.1f, -0.1f, -0.1f),
			glm::vec3(-0.1f, 0.1f, 0.1f),
			glm::vec3(-0.1f, -0.1f, 0.1f),
			glm::vec3(0.1f, -0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, -0.1f, -0.1f),
			glm::vec3(0.1f, 0.1f, -0.1f),
			glm::vec3(0.1f, -0.1f, -0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, -0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, -0.1f),
			glm::vec3(-0.1f, 0.1f, -0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(-0.1f, 0.1f, -0.1f),
			glm::vec3(-0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(-0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, -0.1f, 0.1f)
	};



	
	/*float mindistanxce = 1000;
	for (int i = 0; i < points.size(); i++) {
		float distance = glm::distance(cameraPos, points.at(i));
		if (distance < mindistanxce) {
			mindistanxce = distance;
		}
	}
*/
	/*Colour[0] = Colour[0] / mindistanxce;
	Colour[1] = Colour[1] / mindistanxce;
	Colour[2] = Colour[2] / mindistanxce;
*/
	SimpleMaterial* material = new SimpleMaterial(LookAt, Colour);
	SimpleMaterial* material2 = new SimpleMaterial(LookAt, Colour2);

	Mesh* mesh = new Mesh();
	mesh->objectsarr(points);
	mesh->BindMesh();
	mesh->setupmesh();
	
	//glm::mat4 translation = glm::mat4(1, 0, 0, 2, 0, 1, 0, 2, 0, 0, 1, 2, 0, 0, 0, 1);
	glm::mat4 transform = glm::mat4(1.0);
	glm::mat4 translation = glm::mat4(1.0);

	translation = glm::translate(translation, glm::vec3(0.5f, 0.0f, 0.0f));
	glm::mat4 translation2 = glm::translate(transform, glm::vec3(-0.2f, -0.3f, 0.25f));

	SimpleMaterial* material3 = new SimpleMaterial(LookAt, Colour2);

	material3->changeTranslation(translation2);
	material3->changefscamerapos(cameraPos);

	//transform = glm::translate(transform, glm::vec3(0, 0, 0));
	material2->changeTranslation(translation);
	material->changeTranslation(transform);
	material->changefscamerapos(cameraPos);
	material2->changefscamerapos(cameraPos);

	/*GraphicsNode* gn = new GraphicsNode(mesh, material, LookAt);
	GraphicsNode* gn2 = new GraphicsNode(mesh, material2, LookAt);*/
	gnt = new GraphicsNode(mesh, material, LookAt);
	gnt2 = new GraphicsNode(mesh, material2, LookAt);
	GraphicsNode* gnt3 = new GraphicsNode(mesh, material3, LookAt);

	//this->vsBuffer = new GLchar[STRING_BUFFER_SIZE];
	//this->fsBuffer = new GLchar[STRING_BUFFER_SIZE];

	//// copy the hardcoded shader into buffer
	//strncpy_s(this->vsBuffer, STRING_BUFFER_SIZE, vs, STRING_BUFFER_SIZE);
	//strncpy_s(this->fsBuffer, STRING_BUFFER_SIZE, ps, STRING_BUFFER_SIZE);
	ExampleApp::gnv = {gnt2, gnt, gnt3};

	this->window->SetUiRender([this]()
	{
		this->RenderUI();
	});

	return true;

	
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

		float radius = 1.0f;
		float camX = sin(glfwGetTime()) * radius;
		//float camY = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		

		for (int i = 0; i < gnv.size(); i++) {
			
			glm::mat4 view = glm::mat4(1.0);
			
			if (i == 1) {
				glm::mat4 point1 = glm::translate(view, glm::vec3(cube1camX, cube1camY, cube1camZ));
				glm::mat4 rot = glm::rotate(view, cube1angle, glm::vec3(0, 1, 0));
				point1 = point1 * rot;
				gnv.at(i)->material->changeTranslation(point1);
				gnv.at(i)->Draw();
				

			}
			else  if (i==0) {
				glm::mat4 point2 = glm::translate(view, glm::vec3(cube2camX, cube2camY, cube2camZ));
				glm::mat4 rot2 = glm::rotate(view, cube2angle, glm::vec3(0, 1, 0));
				point2 = point2 * rot2;
				gnv.at(i)->material->changeTranslation(point2);
				gnv.at(i)->Draw();
			}
			else {
				gnv.at(i)->Draw();
			}



			//ExampleApp::gnt2->Draw();
			//gnt2->material->changeTranslation(point2);
			
			/*ExampleApp::gnv.at(0)->Draw();
			ExampleApp::gnv.at(1)->Draw();*/
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
		if (ImGui::Button("test"))
		{
			

		}
	
		ImGui::SliderFloat("square1 x", &cube1camX, -1, 1);
		ImGui::SliderFloat("square1 y", &cube1camY, -1, 1);
		ImGui::SliderFloat("square1 z", &cube1camZ, -0.9, 0.9);
		ImGui::SliderFloat("square1 rotation", &cube1angle, 0, 2*3.14159);
		ImGui::SliderFloat("square2 x", &cube2camX, -1, 1);
		ImGui::SliderFloat("square2 y", &cube2camY, -1, 1);
		ImGui::SliderFloat("square2 z", &cube2camZ, -0.9, 0.9);
		ImGui::SliderFloat("square2 rotation", &cube2angle, 0, 2*3.14159);

		// close window
		ImGui::End();
	}
}

} // namespace Example