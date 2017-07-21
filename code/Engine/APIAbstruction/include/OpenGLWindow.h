/*
* Developed by Bharath - 2017
* This code abstructs the basic window creation of OpenGL
* 
*/
#ifdef OPENGL // if this is for OpenGL, use this rendering API. This enables compiler to choose which window to create

#ifndef _OPENGL_WINDOW_H_
#define _OPENGL_WINDOW_H_


#include "GEWindow.h"
#include <glad/glad.h>
#include <glfw3.h>

//General namespace used across the entire engine
namespace GE
{
	//this is going to be a singleton class
	class OpenGLWindow : public GEWindow
	{
	private :
		OpenGLWindow():m_GLFWCreatedWindowInstance(nullptr)
		{ }

		GLFWwindow* m_GLFWCreatedWindowInstance;
		static GEWindow* s_WindowInstance;

		//Whenever the window is resized this function will be called to resize the frame buffer
		void OpenGLFrameBuffersizeCallBack();

	public:

		static const GEWindow* Get();

		//Function implementation for window initialization 
		void InitWindow() override;

		//Function implementation for Window creation. returns 0 if everything is Ok, else non-zero
		int CreateGEWindow() override;

		//Function implementation for Closing OpenGL window. return 0 if closed proerly, 1 if closed with some errors
		void CloseGEWindow() override;
	};

}
#endif //_OPENGL_WINDOW_H_

#endif // OPENGL