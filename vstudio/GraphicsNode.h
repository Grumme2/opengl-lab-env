#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.

	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "Mesh.h"
#include "Material.h"
#include "glm/glm.hpp" 


class GraphicsNode : public Core::App
{
public:
	/// constructor
	GraphicsNode(Mesh* mesh, Material* material, glm::mat4 transform);
	/// destructor
	~GraphicsNode();

	/// open app
//bool Open();
	/// run app
	
	Mesh* mesh;
	Material* material;
	glm::mat4 transform;
	void Draw();

private:
	
};
#pragma once

