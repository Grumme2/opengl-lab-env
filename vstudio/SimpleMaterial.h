#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.

	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "Material.h"
#include "config.h"
#include <iostream>
#include "glm/glm.hpp"


class SimpleMaterial : public Material
{
public:
	/// constructor
	SimpleMaterial(glm::mat4 transform, GLfloat Colour[3]);
	/// destructor
	~SimpleMaterial();

	/// open app
//bool Open();
	/// run app
	void ApplyMaterial();
	void changeTransform(glm::mat4 transform);
	void changeTranslation(glm::mat4 transform);
	void changeColour(GLfloat Colour[3]);
	void changefscamerapos(glm::vec3 transform);

private:
	GLfloat colour[3];
};
#pragma once

