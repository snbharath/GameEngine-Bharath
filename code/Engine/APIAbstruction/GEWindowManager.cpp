/*
* class definition of GEWindowManager
* Game engine - GEWindowManager.cpp
*/

#include "GEWindowManager.h"
#include <assert.h>


GEWindowManager* GEWindowManager::m_GEWindowManagerInstance = nullptr;

GEWindowManager* GEWindowManager::Get()
{
	if (!m_GEWindowManagerInstance)
	{
		m_GEWindowManagerInstance = new GEWindowManager();
		m_GEWindowManagerInstance->Init();
	}
	return m_GEWindowManagerInstance;
}

void GEWindowManager::Init()
{
#ifdef DIRECT3D

	// Init 


#elif OPENGL


	//Create an instance of OpenGL Window here
	//Initialize the window
	assert(OpenGLWindow::Get()->InitWindow());

	// Create a GLFW window here
	assert(OpenGLWindow::Get()->CreateGEWindow() == 0);

	//Just make sure that the window instance has been created
	assert(reinterpret_cast<OpenGLWindow*>(OpenGLWindow::Get())->GetGLFWCreatedWindowInstance() != NULL);


#else


	// place holder


#endif
}


GEWindow* GEWindowManager::GetWindowInstance() const
{
#ifdef DIRECT3D
	// create an instance Direct X Window
#elif OPENGL
	return OpenGLWindow::Get();
#else
	// placeholder for any other renderer support if I plan to support
#endif
}