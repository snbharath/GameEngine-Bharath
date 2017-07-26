#ifdef OPENGL

#include "OpenGLWindow.h"

using namespace GE;

GEWindow* OpenGLWindow::s_WindowInstance = nullptr;


int OpenGLWindow::InitWindow()
{
	// GLFW initialization
	int retval = glfwInit();

	//Choose GLFW Major and Minor VErsion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // this is needed for the Max OS X

	return retval;
}

void OpenGLWindow::OpenGLFrameBuffersizeCallBack()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_WIDTH);
}

int OpenGLWindow::CreateGEWindow()
{
	m_CreatedWindowInstance = glfwCreateWindow(800, 600, "GameEngine - by Bharath", NULL, NULL);

	if (m_CreatedWindowInstance != NULL)
	{
		glfwMakeContextCurrent(m_CreatedWindowInstance);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return 1; // Load all OpenGL function pointers failed
		}
		return 0;
	}
	else
		return -1;
}

void OpenGLWindow::CloseGEWindow()
{
	glfwSetWindowShouldClose(m_CreatedWindowInstance, true);

	glfwTerminate();
	return;
}


GEWindow* OpenGLWindow::Get()
{
	if (!s_WindowInstance)
		s_WindowInstance = new OpenGLWindow();

	return s_WindowInstance;
}

#endif // OPENGL
