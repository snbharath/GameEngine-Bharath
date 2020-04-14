/*
* Author @Bharath - 2017
* This code abstructs the basic window creation of OpenGL
* 
*/

#pragma once

// if this is for OpenGL, use this rendering API. This enables compiler to choose which window to create
#ifdef OPENGL

#include "GEWindow.h"
#include <glad/glad.h>
#include "glfw3.h"

namespace GE
{
	//this is going to be a singleton class
	class OpenGLWindow : public GEWindow
	{
	private:
		OpenGLWindow();

		GLFWwindow* m_pCreatedWindowInstance;

		//Whenever the window is resized this function will be called to resize the frame buffer
		void OpenGLFrameBuffersizeCallBack();

		// this class instance
		static OpenGLWindow* s_pInstance;

	public:

		virtual ~OpenGLWindow();
		
		// Function implementation for window initialization 
		int InitWindow() override;

		// Function implementation for Window creation. returns 0 if everything is Ok, else non-zero
		int CreateGEWindow() override;

		// Function implementation for Closing OpenGL window. return 0 if closed properly, 1 if closed with some errors
		void CloseGEWindow() override;

		// Used to get the OpenGL window instance
		GLFWwindow* GetGLFWCreatedWindowInstance() const;

		// Create a OpenGL window instance here and return it the base class
		static OpenGLWindow* GetInstance();

	};

	// Inline function definitions
	inline GLFWwindow* OpenGLWindow::GetGLFWCreatedWindowInstance() const
	{
		return m_pCreatedWindowInstance;
	}
}
#endif // OPENGL