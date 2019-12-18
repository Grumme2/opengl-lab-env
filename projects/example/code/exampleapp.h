#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2017 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "GraphicsNode.h"
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

	std::vector<GraphicsNode*> gnv;
	GraphicsNode* gnt;
	GraphicsNode* gnt2;
	Display::Window* window;
	float cube1camX;
	float cube1camY;
	float cube1camZ;
	float cube2camX;
	float cube2camY;
	float cube2camZ;
	float cube1angle;
	float cube2angle;
	
};
} // namespace Example