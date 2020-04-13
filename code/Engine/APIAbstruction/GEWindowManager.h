#pragma once

/*
* This is a manager of the window creation. Will take decision of which window to create based on the solution configuration
* Game engine - GEWindowManager.h
* This is, ofcourse a singleton class
*/

#include "OpenGLWindow.h"
#include "Direct3DWindow.h"

using namespace GE;

//@TODO - inherit the base Object class here
class GEWindowManager // gonna be inheriting the base Object class later I implement the Object.h
{
private:
	static GEWindowManager* m_GEWindowManagerInstance;

	GEWindowManager();

public:

	virtual ~GEWindowManager();

	void Init();

	GEWindow* GetWindowInstance() const ;

	static GEWindowManager* Get();

};