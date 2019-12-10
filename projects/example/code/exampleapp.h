#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.

	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
namespace Example
{
	class ExampleApp : public Core::App
	{
	public:
		/// constructor
		ExampleApp();
		/// destructor
		~ExampleApp();

		/// open app
		bool Open();
		/// run app
		void Run();
	private:

		void RenderUI();
		GLuint program;
		GLuint vertexShader;
		GLuint pixelShader;
		GLuint points;
		GLuint lineLoop;
		GLuint triangle;
		GLuint lines;
		Display::Window* window;
		GLchar *vsBuffer;
		GLchar *fsBuffer;
		std::string compilerLog;
	};
} // namespace Example