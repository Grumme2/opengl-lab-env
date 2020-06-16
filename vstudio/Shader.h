#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.

	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"

enum ShaderType {
	fragment,
	vertex
};

class Shader : public Core::App
	{
	public:
		/// constructor
	Shader();
		/// destructor
	~Shader();

		/// open app
	//bool Open();
		/// run app
	GLuint getShaderHandle();
	ShaderType getShaderType();
	void shader(GLchar* shaderSource, ShaderType shadertype);

	private:
		GLuint shaderHandle;
		ShaderType shadertype;
		
};
#pragma once
