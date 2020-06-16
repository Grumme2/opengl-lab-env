#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.

	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "ShaderProgram.h"
#include "glm/glm.hpp"

class Material : public Core::App
{
public:
		/// constructor
	Material();
	/// destructor
	~Material();

		/// open app
		//bool Open();
		/// run app
	ShaderProgram * shaderProgram;
	virtual void ApplyMaterial() = 0;
	virtual void changeTransform(glm::mat4 transform) = 0;
	virtual void changeColour(GLfloat Colour[3]) = 0;
	virtual void changeTranslation(glm::mat4 transform) = 0;
	virtual void changefscamerapos(glm::vec3 transform) = 0;
	private:
		Display::Window* window;
	};
#pragma once
