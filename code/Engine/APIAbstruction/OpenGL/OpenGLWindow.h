/*
* Author @Bharath - 2017
* This code abstracts the basic window creation of OpenGL
* 
*/

#pragma once

// if this is for OpenGL, use this rendering API. This enables compiler to choose which window to create
#ifdef OPENGL

#include "GEWindow.h"
#include "glad/glad.h"
#include "glfw3.h"

namespace GE
{
	//this is going to be a singleton class
	class OpenGLWindow : public GEWindow
	{
	private:
		OpenGLWindow();
		~OpenGLWindow();

		GLFWwindow * m_pGLFWwindow;

		//Whenever the window is resized this function will be called to resize the frame buffer
		static void OpenGLFrameBufferResizeCallBack(GLFWwindow * pGlfwWindow, int width, int height);

		// this class instance
		static OpenGLWindow* s_pInstance;

	public:
		
		// Function implementation for window initialization 
		bool InitWindow() override;

		// Function implementation for Window creation. returns 0 if everything is Ok, else non-zero
		bool CreateGEWindow() override;

		// All implementation of Open GL render window will go here
		bool RenderFrameUpdate() override;

		// Function implementation for Closing OpenGL window. return 0 if closed properly, 1 if closed with some errors
		void CloseGEWindow() override;

		// Terminate GE window
		void TerminateGEWindow() override;

		void ProcessInput(GLFWwindow*);

		// Create a OpenGL window instance here and return it the base class
		static OpenGLWindow * GetInstance();
		//Delete singleton instance
		static bool DeleteInstance();
	};
}
#endif // OPENGL