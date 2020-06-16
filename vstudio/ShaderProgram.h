#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.

	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "Shader.h"
#include <vector>

class ShaderProgram : public Core::App
{
public:
	/// constructor
	ShaderProgram(std::vector<Shader*> shaders);
	/// destructor
	~ShaderProgram();

	/// open app
//bool Open();
	/// run app

	void shaderbinder();
	void activateshader();
	GLuint getProgram();

private:
	std::vector<Shader*> shaders;
	GLuint program;
};
#pragma once
